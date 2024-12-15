#include<climits>
#include<mutex>
#include<thread>

template<typename T, size_t BlockSize = 4096>
class MemoryPool{
public:

    union Slot{
        T value;
        Slot* next;
    };

    static MemoryPool& getInstance(){
        static MemoryPool instance;
        return instance;
    }

    MemoryPool& operator=(const MemoryPool& memorypool) = delete;
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& getMemoryPool(int id);

    T* allocate();
    void deallocate(T* ptr);

    template <class... Args> T* newElement(Args&&... args);
    void deleteElement(T* p);

private:

    MemoryPool();
    ~MemoryPool();
    
    Slot* currentBlock;
    Slot* currentSlot;
    Slot* freeSlot;
    Slot* lastSlot;

    std::mutex mutex_freeSlot;
    std::mutex mutex_other;

    void allocateBlock();
    size_t padPointer(char* p, size_t align) const noexcept;
    
};

template<typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool(){
    currentBlock = nullptr;
    currentSlot = nullptr;
    freeSlot = nullptr;
    lastSlot = nullptr;
}

template<typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::~MemoryPool(){
    Slot* cur = currentBlock;
    
    while(cur){
        Slot* next = cur->next;
        operator delete(reinterpret_cast<void*>(cur));
        cur = next;
    }
}

template<typename T, size_t BlockSize>
size_t MemoryPool<T, BlockSize>::padPointer(char* p, size_t align) const noexcept {
    size_t result = reinterpret_cast<size_t>(p);
    return (align - result) % align;
}

template<typename T, size_t BlockSize>
void MemoryPool<T, BlockSize>::allocateBlock(){
    char* newBlock = reinterpret_cast<char*>(operator new(BlockSize));

    char* body = newBlock + sizeof(Slot*);
    size_t bodyPadding = padPointer(body, sizeof(Slot));

    {
        std::lock_guard<std::mutex> lock(mutex_other);

        reinterpret_cast<Slot*>(newBlock)->next = currentBlock;
        currentBlock = reinterpret_cast<Slot*>(newBlock);

        currentSlot = reinterpret_cast<Slot*>(body + bodyPadding);
        lastSlot = reinterpret_cast<Slot*>(newBlock + BlockSize - sizeof(Slot) + 1);
    }
}

template<typename T, size_t BlockSize>
T* MemoryPool<T, BlockSize>::allocate(){
    if(freeSlot != nullptr){
        std::lock_guard<std::mutex> lock(mutex_freeSlot);
        if(freeSlot != nullptr){
            T* result = reinterpret_cast<T*>(freeSlot);
            freeSlot = freeSlot->next;
            return result;
        }
    }
    else{
        if(currentSlot >= lastSlot){
            allocateBlock();
        }
        std::lock_guard<std::mutex> lock(mutex_other);
        return reinterpret_cast<T*>(currentSlot++);
    }
}

template<typename T, size_t BlockSize>
void MemoryPool<T, BlockSize>::deallocate(T* ptr){
    if(ptr != nullptr){
        std::lock_guard<std::mutex> lock(mutex_freeSlot);
        reinterpret_cast<Slot*> (ptr)->next = freeSlot;
        freeSlot = reinterpret_cast<Slot*>(ptr);
    }
}

template<typename T, size_t BlockSize>
template <class... Args> T* MemoryPool<T, BlockSize>::newElement(Args&&... args){
    T* result = allocate();
    new (result) T(std::forward<Args>(args)...);
    return result;
}

template<typename T, size_t BlockSize>
void MemoryPool<T, BlockSize>::deleteElement(T* ptr){
    if(ptr != nullptr){
        ptr->~T();
        deallocate(ptr);
    }
}