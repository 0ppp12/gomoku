/*** 
 * @Author: PENG
 * @Date: 2023-09-22 13:56:40
 * @LastEditTime: 2023-09-22 15:16:12
 * @LastEditors: PENG
 * @Description: 
 * @FilePath: \c++\c++ 阶段项目\Gobang\src\通讯\thread_pool.cpp
 * @Copyright(c)
 */
#include "thread_pool.h"



//这是一个线程清理函数，用于在线程退出时释放互斥锁。它会打印当前线程的ID，并释放传入的互斥锁
void handler(void *arg)
{
	printf("[%u] is ended.\n",
		(unsigned)pthread_self());

	pthread_mutex_unlock((pthread_mutex_t *)arg);
}


//线程池中每个线程的执行函数
void *routine(void *arg)
{
	#ifdef DEBUG
	printf("[%u] is started.\n",
		(unsigned)pthread_self());
	#endif
	/*传递的参数为pool,强转为thread_pool*/
	thread_pool *pool = (thread_pool *)arg;

	/*暂存任务队列中的要执行的任务*/
	struct task *p;

	while(1)
	{
		/*
		** push a cleanup functon handler(), make sure that
		** the calling thread will release the mutex properly
		** even if it is cancelled during holding the mutex.
		**
		** NOTE:
		** pthread_cleanup_push() is a macro which includes a
		** loop in it, so if the specified field of codes that 
		** paired within pthread_cleanup_push() and pthread_
		** cleanup_pop() use 'break' may NOT break out of the
		** truely loop but break out of these two macros.
		** see line 61 below.
		*/
		//================================================//
		pthread_cleanup_push(handler, (void *)&pool->lock);
		pthread_mutex_lock(&pool->lock);
		//================================================//

		// 1, no task, and is NOT shutting down, then wait
		while(pool->waiting_tasks == 0 && !pool->shutdown)
		{
			/*等待任务创建完成条件变量*/
			pthread_cond_wait(&pool->cond, &pool->lock);
		}

		// 2, no task, and is shutting down, then exit
		if(pool->waiting_tasks == 0 && pool->shutdown == true)
		{
			pthread_mutex_unlock(&pool->lock);
			pthread_exit(NULL); // CANNOT use 'break';
		}

		// 3, have some task, then consume it
		/*取出队首*/
		p = pool->task_list->next;
		pool->task_list->next = p->next;
		pool->waiting_tasks--;

		//================================================//
		pthread_mutex_unlock(&pool->lock);
		pthread_cleanup_pop(0);
		//================================================//
		/*设置此线程在执行时为不可取消状态，其他线程无法通过取消操作来终止该线程的执行*/
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		(p->do_task)(p->arg);
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

		free(p);
	}

	pthread_exit(NULL);
}
//析构和构造函数
ThreadPool::ThreadPool() {}    
ThreadPool::~ThreadPool() {}
//初始化线程池
bool ThreadPool::init_pool(thread_pool *pool, unsigned int threads_number)
{
	/*初始化互斥锁和条件变量*/
	pthread_mutex_init(&pool->lock, NULL);
	pthread_cond_init(&pool->cond, NULL);

	/*初始化结构体中变量*/
	pool->shutdown = false;//线程池状态
	pool->task_list = (struct task *)malloc(sizeof(struct task));
	pool->tids = (pthread_t *)malloc(sizeof(pthread_t) * MAX_ACTIVE_THREADS);

	if(pool->task_list == NULL || pool->tids == NULL)
	{
		perror("allocate memory error");
		return false;
	}

	pool->task_list->next = NULL;

	pool->max_waiting_tasks = MAX_WAITING_TASKS;
	pool->waiting_tasks = 0;
	pool->active_threads = threads_number;

	unsigned int  i;
	for(i=0; i<pool->active_threads; i++)
	{
		if(pthread_create(&((pool->tids)[i]), NULL,
					routine, (void *)pool) != 0)
		{
			perror("create threads error");
			return false;
		}

		#ifdef DEBUG
		printf("[%u]:[%s] ==> tids[%d]: [%u] is created.\n",
			(unsigned)pthread_self(), __FUNCTION__,
			i, (unsigned)pool->tids[i]);
		#endif
	}

	return true;
}


//向任务队列添加任务
bool ThreadPool::add_task(thread_pool *pool,
	      void *(*do_task)(void *arg), void *arg)
{
	struct task *new_task = (struct task *)malloc(sizeof(struct task));
	if(new_task == NULL)
	{
		perror("allocate memory error");
		return false;
	}
	new_task->do_task = do_task;
	new_task->arg = arg;
	new_task->next = NULL;

	//============ LOCK =============//
	pthread_mutex_lock(&pool->lock);
	//===============================//

	if(pool->waiting_tasks >= MAX_WAITING_TASKS)
	{
		pthread_mutex_unlock(&pool->lock);

		fprintf(stderr, "too many tasks.\n");
		free(new_task);

		return false;
	}
	
	struct task *tmp = pool->task_list;
	while(tmp->next != NULL)
		tmp = tmp->next;

	tmp->next = new_task;
	pool->waiting_tasks++;

	//=========== UNLOCK ============//
	pthread_mutex_unlock(&pool->lock);
	//===============================//

	#ifdef DEBUG
	printf("[%u][%s] ==> a new task has been added.\n",
		(unsigned)pthread_self(), __FUNCTION__);
	#endif

	pthread_cond_signal(&pool->cond);
	return true;
}


//添加线程
int ThreadPool::add_thread(thread_pool *pool, unsigned additional_threads)
{
	if(additional_threads == 0)
		return 0;

	unsigned total_threads =
			pool->active_threads + additional_threads;

    unsigned int i;
	int actual_increment = 0;
	for(i = pool->active_threads;
	    i < total_threads && i < MAX_ACTIVE_THREADS;
	    i++)
	{
		if(pthread_create(&((pool->tids)[i]),
					NULL, routine, (void *)pool) != 0)
		{
			perror("add threads error");

			// no threads has been created, return fail
			if(actual_increment == 0)
				return -1;

			break;
		}
		actual_increment++; 

		#ifdef DEBUG
		printf("[%u]:[%s] ==> tids[%d]: [%u] is created.\n",
			(unsigned)pthread_self(), __FUNCTION__,
			i, (unsigned)pool->tids[i]);
		#endif
	}

	pool->active_threads += actual_increment;
	return actual_increment;
}

int ThreadPool::remove_thread(thread_pool *pool, unsigned int removing_threads)
{
	if(removing_threads == 0)
		return pool->active_threads;

	unsigned int remaining_threads = pool->active_threads - removing_threads;
	remaining_threads = remaining_threads > 0 ? remaining_threads : 1;

	unsigned int  i;
	for(i=pool->active_threads-1; i>remaining_threads-1; i--)
	{
		errno = pthread_cancel(pool->tids[i]);

		if(errno != 0)
			break;

		#ifdef DEBUG
		printf("[%u]:[%s] ==> cancelling tids[%d]: [%u]...\n",
			(unsigned)pthread_self(), __FUNCTION__,
			i, (unsigned)pool->tids[i]);
		#endif
	}

	if(i == pool->active_threads-1)
		return -1;
	else
	{
		pool->active_threads = i+1;
		return i+1;
	}
}

bool ThreadPool::destroy_pool(thread_pool *pool)
{
	// 1, activate all threads
	pool->shutdown = true;
	pthread_cond_broadcast(&pool->cond);

	// 2, wait for their exiting
	unsigned int i;
	for(i=0; i<pool->active_threads; i++)
	{
		errno = pthread_join(pool->tids[i], NULL);
		if(errno != 0)
		{
			printf("join tids[%d] error: %s\n",
					i, strerror(errno));
		}
		else
			printf("[%u] is joined\n", (unsigned)pool->tids[i]);
		
	}

	// 3, free memories
	free(pool->task_list);
	free(pool->tids);
	free(pool);

	return true;
}




/****************test************************/

void *mytask(void *arg)
{
	
    int n =*(int*)arg;  

	printf("[%u][%s] ==> job will be done in %d sec...\n",
		(unsigned)pthread_self(), __FUNCTION__, n);

	sleep(n);

	printf("[%u][%s] ==> job done!\n",
		(unsigned)pthread_self(), __FUNCTION__);

	return NULL;
}

void *count_time(void *arg)
{
	int i = 0;
	while(1)
	{
		sleep(1);
		printf("sec: %d\n", ++i);
	}
}

int main(void)
{
    ThreadPool tp;
	pthread_t a;
	pthread_create(&a, NULL, count_time, NULL);

	// 1, initialize the pool
	thread_pool *pool = (thread_pool *)malloc(sizeof(thread_pool));
	tp.init_pool(pool, 2);

	// 2, throw tasks
    int n=rand()%10;
	printf("throwing 3 tasks...\n");
	tp.add_task(pool, mytask, (void *)(&n));
	tp.add_task(pool, mytask, (void *)(&n));
	tp.add_task(pool, mytask, (void *)(&n));

	// 3, check active threads number
	printf("current thread number: %d\n",
			tp.remove_thread(pool, 0));
	sleep(9);

	// 4, throw tasks
	printf("throwing another 2 tasks...\n");
	tp.add_task(pool, mytask, (void *)(&n));
	tp.add_task(pool, mytask, (void *)(&n));

	// 5, add threads
	tp.add_thread(pool, 2);

	sleep(5);

	// 6, remove threads
	printf("remove 3 threads from the pool, "
	       "current thread number: %d\n",
			tp.remove_thread(pool, 3));

	// 7, destroy the pool
	tp.destroy_pool(pool);
	return 0;
}

