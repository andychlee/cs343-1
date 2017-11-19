#ifndef __Q3BUFFER__
#define __Q3BUFFER__

#include <vector>

template<typename T> class BoundedBuffer {
    unsigned int size;
    uOwnerLock mlk;
    uOwnerLock sum_lock;                                                                // used for adding sum
    uCondLock p_lock;
    uCondLock c_lock;
    std::vector<T> buffer;
    #ifdef NOBUSY
        bool in_use;
        uCondLock barge;
    #endif                                                                                                             

  public:
    #ifdef BUSY
    BoundedBuffer( const unsigned int size = 10 ): size(size) {}
    #endif

    #ifdef NOBUSY
    BoundedBuffer( const unsigned int size = 10 ): size(size), in_use(false) {}
    #endif

    void insert( T elem );
    T remove();
};



_Task Producer {
    void main();
    BoundedBuffer<int> *buffer;
    int Produce;
    int Delay;
  public:
    Producer( BoundedBuffer<int> &buffer, const int Produce, const int Delay ): buffer(&buffer), Produce(Produce), Delay(Delay) {}
};

_Task Consumer {
    void main();
    BoundedBuffer<int> *buffer;
    int Delay;
    int* sum;
    int local_sum;
    int sentinel;
  public:
    Consumer( BoundedBuffer<int> &buffer, const int Delay, const int Sentinel, int &sum ): buffer(&buffer), Delay(Delay), sum(&sum), local_sum(0), sentinel(Sentinel) {}
    ~Consumer() {(*sum) += local_sum;}
};

#endif