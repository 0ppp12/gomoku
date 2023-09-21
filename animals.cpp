// #include <iostream>

// using namespace std;

// class Animals
// {
// private:
//     string name;
//     int age;
// public:
//     Animals(const string& name ,int age):name(name),age(age){
//     }
//     ~Animals(){
//     }
// public:
//     void sound(){
//         cout<<"the Animals"<<name<<"the age is"<<age<<endl;
//     }
// };

// class Flight:virtual public Animals{
// private:
//     int speed;
// public:
//     void Flight(int speed):speed(speed){
//         cout<<"the animal is flying"<<speed<<"km/h"<<endl;
//     }
// };

// class Swim:virtual public Animals{
// private:
//     int height;
// public:
//     void Swim(int height):height(height){
//         cout<<"the animal is "<<height<<"kilograms"<<endl;
//     }

// };

// class Goose:public Flight,public Swim{
// public:
//     Goose(const string& name,int age ,int friends,int weight,int speed):Animals(name, age), Flight(speed), Swim(weight), friends(friends){
//         cout << "Goose" << endl;
//         cout << "the Goose has " << friends << " friend" << endl;
//     }
//     void sound() {
//         cout << "the Goose makes a honking sound" << endl;
//     }
// private:
//     int friends;
// };

// int main()
// {
//     Goose goose("Goosey", 3 , 2, 5, 40);
//     return 0;
// }

#include <iostream>
#include <string>

using namespace std;

class Animals
{
private:
    string name;
    int age;

public:
    Animals(const string& name, int age) : name(name), age(age) {}

    void sound()
    {
        cout << "the Animals " << name << " the age is " << age << endl;
    }
};

class Flight : virtual public Animals
{
private:
    int speed;

public:
    Flight(int speed, const string& name, int age) : Animals(name, age), speed(speed)
    {
        cout << "the animal is flying at " << speed << " km/h" << endl;
    }
};

class Swim : virtual public Animals
{
private:
    int height;

public:
    Swim(int height, const string& name, int age) : Animals(name, age), height(height)
    {
        cout << "the animal is " << height << " kilograms" << endl;
    }
};

class Goose : public Flight, public Swim
{
public:
    Goose(const string& name, int age, int friends, int weight, int speed)
        : Animals(name, age), Flight(speed, name, age), Swim(weight, name, age), friends(friends)
    {
        cout << "Goose" << endl;
        cout << "the Goose has " << friends << " friend" << endl;
    }

    void sound()
    {
        cout << "the Goose makes a honking sound" << endl;
    }

private:
    int friends;
};

int main()
{
    Goose goose("Goosey", 3, 2, 5, 40);//菱形继承
    goose.Animals::sound();//类域
    goose.sound();//子类
    // 打印Goose的信息
    return 0;
}