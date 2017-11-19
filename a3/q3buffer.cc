#include <string>
#include <iostream>
#include "q3buffer.h"
#include "MPRNG.h"

using namespace std;

MPRNG *mprng;

template<typename T>
void BoundedBuffer<T>::insert(T elem) {
    // first, try to accquire the lock to get access to the buffer
    
    // handle the case when the buffer if full
    // should sleep if busy waiting
    #ifdef BUSY                                         // busy waiting implementation
        mlk.acquire();
        try {
            while ((unsigned int)buffer.size() == size) {
                p_lock.wait(mlk);
            }
            assert((unsigned int)buffer.size() != size);
            buffer.push_back(elem);                     // add to the buffer
            c_lock.signal();
        } _Finally {                                    // ensure the lock is released
            mlk.release();  
        } 
    #endif // BUSY

    #ifdef NOBUSY
        mlk.acquire();
        try {
            if (in_use) barge.wait(mlk);                // prevent barging
            if ((unsigned int)buffer.size() == size) {
                if(!barge.empty()) {
                    barge.signal();
                } else {
                    in_use = false;
                }
                p_lock.wait(mlk);
            }
            assert((unsigned int)buffer.size() != size);
            buffer.push_back(elem);                     // add to the buffer
            in_use = true;
            if(!c_lock.empty()) {
                c_lock.signal();
            } else if(!barge.empty()) {
                barge.signal();
            } else {
                in_use = false;
            }
        } _Finally {
            mlk.release();
        }
    #endif
}

template<typename T>
T BoundedBuffer<T>::remove() {
    // first try to access the buffer
    T copy;
    #ifdef BUSY                                         // busy waiting implementation
        mlk.acquire();
        try {
            while (buffer.size() == 0) {
                c_lock.wait(mlk);
            }
            assert(buffer.size() != 0);
            copy = buffer[0];
            buffer.erase(buffer.begin());               // remove it from the buffer
            p_lock.signal();

        } _Finally {                                    // ensure the lock is released
            mlk.release();
        }
    #endif // BUSY

    #ifdef NOBUSY
        mlk.acquire();
        try {
            if (in_use) barge.wait(mlk);                // prevent barging
            if ((unsigned int)buffer.size() == 0) {
                if(!barge.empty()) {
                    barge.signal();
                } else {
                    in_use = false;
                }
                c_lock.wait(mlk);
            }
            assert((unsigned int)buffer.size() != 0);
            copy = buffer[0];
            buffer.erase(buffer.begin());               // remove it from the buffer
            in_use = true;
            if(!p_lock.empty()) {
                p_lock.signal();
            } else if(!barge.empty()) {
                barge.signal();
            } else {
                in_use = false;
            }
        } _Finally {
            mlk.release();
        }
    #endif // NOBUSY
    return copy;
}

void Producer::main() {
    for(int i=1; i <= Produce; i++) {
        yield((*mprng)(0, Delay-1));
        buffer->insert(i);
    }
    // exit the task, producer's job is done
}

void Consumer::main() {
    int value = 0; 
    while(1) {
        yield((*mprng)(Delay - 1));
        value = buffer->remove();
        if (value == sentinel) break;
        local_sum += value;
    }
    // exit the task, the job is done
}

void usage() {
    cerr << "" << endl;
    exit(-1);
}

void handle_input(char* argv[], int index, string error_msg, int &update_target) {
    try {
        update_target = stoi(argv[index]);
        if (update_target <= 0) throw -1; 
    } catch(...) {
        cerr << error_msg << endl;
        usage();
    }
}

int main(int argc, char * argv[]) {
    int sum = 0;
    int Cons = 5, Prods = 3, Produce = 10, BufferSize = 10;
    int Delay = -1;                                                                 // indicate that the delay is not specified

    string err5 = "Invalid argument for Delay, Delay must be an positive integer";
    string err4 = "Invalid argument for BufferSize, BufferSize must be an positive integer";
    string err3 = "Invalid argument for Produce, Produce must be an positive integer";
    string err2 = "Invalid argument for Prods, Prods must be an positive integer";
    string err1 = "Invalid argument for Cons, Cons must be an positive integer";

    // reading the input

    switch (argc) {
        case 6:
            handle_input(argv, 5, err5, Delay);
            // follow through
        
        case 5:
            handle_input(argv, 4, err4, BufferSize);
            // follow through
        
        case 4:
            handle_input(argv, 3, err3, Produce);
            // follow through
        
        case 3:
            handle_input(argv, 2, err2, Prods);
            // follow through

        case 2:
            handle_input(argv, 1, err1, Cons);
            break;

        case 1:
            break;                                                                                  // just continue execution and use default vale for all

        default:
            cerr << "Invalid number of arguments passed" << endl;
            usage();
            exit(-1);

    }

    if (Delay == -1) Delay = Cons + Prods;

    mprng = new MPRNG();

    // print out the generated values
    cout << Cons << " ";
    cout << Prods << " ";
    cout << Produce << " ";
    cout << BufferSize << " ";
    cout << Delay << " ";
    cout << endl;

    BoundedBuffer<TYPE> b(BufferSize);

    vector<Consumer*> v_c;
    vector<Producer*> p_c;

    for (int i=0; i < Prods; i++) {                                                                     // construct producers
        Producer *cur = new Producer(b, Produce, Delay);
        p_c.push_back(cur);
    }

    for (int i=0; i < Cons; i++) {
        Consumer *cur = new Consumer(b, Delay, SENTINEL, sum);                                          // construct consumers
        v_c.push_back(cur);
    }

    for (int i=0; i < Prods; i++) {
        delete p_c[i];
    }
                                                                                                        // acquire lock to insert SENTINEL
    for (int i=0; i < Cons; i++) {
        b.insert(SENTINEL);
    }

    for (int i =0; i < Cons; i++) {
        delete v_c[i];
    }

    cout << "total: " << sum << endl;
    delete mprng;

}




















