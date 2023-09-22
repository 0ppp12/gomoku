/*** 
 * @Author: PENG
 * @Date: 2023-09-22 14:24:41
 * @LastEditTime: 2023-09-22 15:21:06
 * @LastEditors: PENG
 * @Description: 
 * @FilePath: \c++\c++ 阶段项目\Gobang\src\通讯\thread_pool.h
 * @Copyright(c)
 */
#pragma once 

#include <iostream>
extern "C"{
    #include <stdio.h>
    #include <stdbool.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <string.h>
    #include <strings.h>
    #include <errno.h>
    #include <pthread.h>
}

#define MAX_WAITING_TASKS	1000
#define MAX_ACTIVE_THREADS	20

//任务结点
struct task
{
	void *(*do_task)(void *arg); //任务函数指针
	void *arg;					 //需要传递给任务的参数，如果不需要，则NULL
	struct task *next;			 //指向下一个任务结点的指针
};


//线程池的管理结构体
typedef struct thread_pool
{
	pthread_mutex_t lock;    //互斥锁
	pthread_cond_t  cond;	 //条件
	bool shutdown;
	struct task *task_list;
	pthread_t *tids;	//tid
	unsigned max_waiting_tasks;		//最大等待处理的任务数
	unsigned waiting_tasks;			//正在等待处理的任务数
	unsigned active_threads;		//激活线程数
}thread_pool;


class ThreadPool{
public:
    ThreadPool();
    ~ThreadPool();
    bool init_pool(thread_pool *pool, unsigned int threads_number);
    bool add_task(thread_pool *pool, void *(*do_task)(void *arg), void *task);
    int  add_thread(thread_pool *pool, unsigned int additional_threads_number);
    int  remove_thread(thread_pool *pool, unsigned int removing_threads_number);
    bool destroy_pool(thread_pool *pool);
};