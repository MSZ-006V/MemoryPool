#include<iostream>
#include<string>
#include"MemoryPool.h"

using namespace std;

class person{
private:
    int age;
    std::string name;
public:
    person(int age_, string name_): age(age_), name(std::move(name_)) {}
    int getAge() {return age;}
    string getName() {return name;}
};

int main(){
    MemoryPool<person> memorypool;
    person* ptr = memorypool.newElement(10, "peter");

    cout << ptr->getAge() << ' ' << ptr->getName() << endl;

    return 0;
}