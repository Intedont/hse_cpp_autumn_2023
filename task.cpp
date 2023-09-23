#include <iostream>

class Allocator
{
private:
    char* data;
    size_t len;
    size_t offset;
public:
    void makeAllocator(size_t maxSize) {
        delete [] data;
        data = new char[maxSize];
        len = maxSize;
        offset = 0;
    }


    char* alloc(size_t size){
        if((size > len - offset) || size == 0){
            return nullptr;
        } else {
            offset += size;
            return &(data[offset - size]);
        }
    }
    void reset(){
        offset = 0;
    }
    // Это деструктор!
    ~Allocator(){
        delete [] data;
    }
};
