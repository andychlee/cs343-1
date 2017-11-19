#include <iostream>
#include <cstdlib>                      // access: rand, srand
using namespace std;
#include <unistd.h>                     // access: getpid

int times = 1000;                       // default value
int g_flag = -1;

void rtn1( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 10000 == 42  && g_flag == -1) g_flag = j;
    }
}

void rtn2( int i ) {
    for ( int j = times; j >= 0; j -= 1 ) {
        if ( rand() % 10000 == 42  && g_flag == -1) g_flag = j;
    }
}
void g( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 2 == 0 ) rtn1( i );
        else rtn2( i );
    }
    if ( i % 2 ) rtn2( i );
    rtn1( i );
}
void f( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        g( i );
    }
    if ( i % 2 ) g( i );
    g( i );
}
int main( int argc, char *argv[] ) {
    int seed = getpid();                // default value
    try {               // process command-line arguments
        switch ( argc ) {
          case 3: times = stoi( argv[2] ); if ( times <= 0 ) throw 1;
          case 2: seed = stoi( argv[1] ); if ( seed <= 0 ) throw 1;
          case 1: break;                // use defaults
          default: throw 1;
        } // switch
    } catch( ... ) {
        cout << "Usage: " << argv[0] << " [ seed (> 0) [ times (> 0) ] ]" << endl;
        exit( 1 );
    } // try
    srand( seed );          // set random-number seed
    f(3);
    if (g_flag == -1) {
        cout << "seed:" << seed << " times:" << times << " complete" << endl;
    } else {
        cout << "seed:" << seed << " times:" << times << " rc:" << g_flag << endl;
    }
}