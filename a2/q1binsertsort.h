#ifndef Q1BINSERTSORT_H
#define Q1BINSERTSORT_H

template<typename T> _Coroutine Binsertsort {
    const T Sentinel;                                                       // value denoting end of set
    T value;                                                                // communication: value being passed down/up the tree
    T pivot;                                                                // the pivot value
    void main();
  public:
    Binsertsort(T Sentinel);
    void sort(T value);
    T retrieve();
};  // Binsertsort

#endif