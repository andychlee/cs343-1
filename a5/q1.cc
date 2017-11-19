#include <iostream>
using namespace std;

#ifdef PAD
#define ALIGN __attribute__(( aligned(64) )) // align on 64-byte boundaries
#else
#define ALIGN
#endif // PAD

volatile long int
    pad1 ALIGN,                              // bracket counters with alignment
    counter1 ALIGN, counter2 ALIGN,
    pad2 ALIGN;

unsigned long int times = 1000000000;

_Task Worker {
    volatile long int &counter;
    void main() {
        for ( volatile unsigned int i = 0; i < times; i += 1 ) {
            counter += 1;
        }
        cout << counter << endl;
    }
  public:
    Worker( volatile long int &counter ) : counter( counter ) {}
};

int main( int argc, char *argv[] ) {
    switch ( argc ) {
      case 2:
        times = atol( argv[1] );
    }
    cout << (void *)&pad1 << ' ' << (void *)&counter1 << ' '
         << (void *)&counter2 << ' ' << (void *)&pad2 << ' ' << endl;

    uProcessor p;                            // add virtual processor
    Worker w1( counter1 ), w2( counter2 );   // create threads
}