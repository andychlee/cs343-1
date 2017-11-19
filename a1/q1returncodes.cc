#include <iostream>
#include <cstdlib>                      // access: rand, srand
using namespace std;
#include <unistd.h>                     // access: getpid

int times = 1000;                       // default value

// -1 is a number means normal execution successed in my program

int rtn1( int i ) {
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 10000 == 42 ) return j;
    }
    return -1;
}
int rtn2( int i ) {
    for ( int j = times; j >= 0; j -= 1 ) {
        if ( rand() % 10000 == 42 ) return j;
    }
    return -1;
}
int g( int i ) {
    int return_code;
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 2 == 0 ) return_code = rtn1(i);
        else return_code = rtn2( i );
        if (return_code != -1) return return_code;
    }
    if ( i % 2 ) return_code = rtn2( i );
    if (return_code != -1) return return_code;
    return rtn1( i );
}
int f( int i ) {
    int return_code;
    for ( int j = 0; j < times; j += 1 ) {
        return_code = g(i);
        if (return_code != -1) return return_code;
    }
    if(i % 2) return_code = g(i);
    if (return_code != -1) return return_code;
    return g(i);
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

    int result = f(3);
    if (result == -1) {
        cout << "seed:" << seed << " times:" << times << " complete" << endl;
    } else {
        cout << "seed:" << seed << " times:" << times << " rc:" << result << endl;
    }
}