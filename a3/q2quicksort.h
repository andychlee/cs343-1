#ifndef __Q2QUICKSORT_H__
#define __Q2QUICKSORT_H__

#include <iostream>
#include <fstream>
#include <string>

using namespace std;


template<typename T> _Task Quicksort {
    T *values;
    unsigned int low;
    unsigned int high;
    unsigned int depth;

    void swap(T values[], int a, int b) {
        T tmp = values[a];
        values[a] = values[b];
        values[b] = tmp;
    }

    int partition(T values[], int low, int high, int pivot_idx) {
        swap(values, pivot_idx, high);                                                                     // first, make a switch
        int store_idx = low;
        for(int i=low; i< high; i++) {
            if(values[i] < values[high]) {
                swap(values, i, store_idx);                                                                // swap the small values to the front
                store_idx += 1;
            }
        }
        swap(values, store_idx, high);
        return store_idx;
    }

    void quicksort(T values[], int low, int high) {
        if(low >= high) return;
        int pivot_idx = low + (high - low) / 2;
        int new_pivot = partition(values, low, high, pivot_idx);
        // first, check if depth equals to 0
        // if so, only using recursion to sort both halves
        // else, first half using new task to sort, second half using recursion to sort
        if (depth == 0) {
            quicksort(values, low, new_pivot-1);                                                            // sort using recursion
            quicksort(values, new_pivot+1, high);                                                           // always sorting the second half using recursion
        } else {
            Quicksort child(values, low, new_pivot-1, depth-1);                                             // sort using a new task
            quicksort(values, new_pivot+1, high);                                                           // always sorting the second half using recursion
        }
    }

    void main() {
        quicksort(values, low, high);
    }
  public:
    Quicksort( T values[], unsigned int low, unsigned int high, unsigned int depth ): values(values), low(low), high(high), depth(depth) {}
};

#endif