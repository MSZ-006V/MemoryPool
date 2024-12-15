## MemoryPool
- This is a C++ memorypool template class implementation, which is simple to use, convenient and fast. It can replace the original `new\malloc`, `delete\free` operations, and also I provide a singleton class memorypool template class implementation
- This memory pool ensures multi-threaded safety (by locking at the allocate\deallocate), and uses perfect forwarding and variable parameter templates to support the creation of any type of memory pool
- This memorypool is developed based on the C++11 standard. The code refers to the cacay/MemoryPool, muduo memorypool and boost/memorypool
## How to use
- When using this, you need to declare `include` at the beginning to include the memory pool library
```cpp
#include"MemoryPool.h"
// #include"MemoryPool_singleton.h" Also can use singleton memorypool
```
- `T` is the class type of the specific implementation, and `BlockSize` is the size of the chunks MemoryPool allocates. Use the following code to create the memorypool and allocate space
```cpp
// MemoryPool
#include<iostream>
#include"MemoryPool.h"

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
    person* p = memorypool.newElement(10, "Peter");

    std::cout << p->getAge() << ' ' << p->getName() << std::endl;

    
    return 0;
}
```
```cpp
// Singleton MemoryPool
#include<iostream>
#include"MemoryPool_singleton.h"

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
    auto& memorypool = MemoryPool<person>::getInstance();
    person* p = memorypool.newElement(10, "Peter");

    std::cout << p->getAge() << ' ' << p->getName() << std::endl;
    return 0;
}
```
- The memorypool is thread-safe, it locks during the allocation and deallocation to ensure thread safety
- When using, just use `newElement()` and `deleteElement()` to allocate and deallocate, and no need to use new/malloc, delete/free
## About
- If there are any bugs and any optimization or suggestions, please let me know! XD
- Gmail: msz006incubus@gmail.com
