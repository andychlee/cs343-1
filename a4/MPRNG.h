// Concurrent-Safe Pseudo Random-Number Generator : generate repeatable sequence of values that
//   appear random by scrambling the bits of an integer value.

#ifndef __MPRNG_H__
#define __MPRNG_H__

_Monitor MPRNG {
    unsigned int seed;
  public:
    MPRNG( unsigned int seed_ = 1009 ) { seed = seed_; srand( seed ); }	// set seed
    void set_seed( unsigned int seed_ ) { seed = seed_; srand( seed ); } // set seed
    _Nomutex unsigned int get_seed() const { return seed; } // get seed
    unsigned int operator()() { return rand(); }	// [0,UINT_MAX]
    unsigned int operator()( unsigned int u ) { return operator()() % (u + 1); } // [0,u]
    unsigned int operator()( unsigned int l, unsigned int u ) { return operator()( u - l ) + l; } // [l,u]
}; // MPRNG

#endif // __MPRNG_H__