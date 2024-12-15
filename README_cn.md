## MemoryPool
- 这是一个C++内存池模板类实现，使用简单，方便快捷。可以替代原先的new\malloc，delete\free操作，同时提供单例类内存池模板类实现
- 这个内存池保证多线程安全（通过在分配\释放处加锁实现），同时使用完美转发与可变参数模板，支持任意类型的内存池创建
- 该内存池基于C++11标准编写，代码参考cacay/MemoryPool与muduo的内存池模块
## 使用用例
- 在使用时，在开头声明，包含该内存池库
```cpp
#include"MemoryPool.h"
// #include"MemoryPool_singleton.h" 也可以选择使用单例类内存池
```
- T为具体实现的类类型，BlockSize为MemoryPool分配的块大小。使用下面的代码创建并分配
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
- 该内存池是线程安全的，在分配和释放时进行加锁，保证线程安全
- 在使用时，只要调用newElement()与deleteElement()进行分配与释放即可，不需要再调用new/malloc, delete/free方法
- 可以使用项目中的`test.cpp`代码测试标准内存池，使用`test_singleton.cpp`代码测试单例类内存池库
## 关于代码
- 如果有任何bug和任何优化方案，修改建议，请告诉我！
- Gmail: msz006incubus@gmail.com
- QQ: 49353914