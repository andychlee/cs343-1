// this is the version that get rid of the throw and break

#include <iostream>
#include <cstdlib>                      // access: rand, srand
using namespace std;
#include <unistd.h>                     // access: getpid

int times = 1000;                       // default value

int rtn1( int i ) {
    int flag2 = -1;      // this flag is used to hold the return code for the routine 
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 10000 == 42 ) {
            flag2 = j;
            j = times;              
        }
    }
    return flag2;
}
int rtn2( int i ) {
    int flag3 = -1;
    for ( int j = times; j >= 0; j -= 1 ) {
        if ( rand() % 10000 == 42 ) {
            flag3 = j;
            j = -1;              
        }
    }
    return flag3;
}
int g(int i) {
    int flag4 = -1;
    for ( int j = 0; j < times; j += 1 ) {
        if ( rand() % 2 == 0 ) {
            flag4 = rtn1( i );
        } else {
            flag4 = rtn2( i );
        }
        if (flag4 != -1) j = times;  // my way of stopping the loop
    }
    if (flag4 == -1) { 
        if ( i % 2 ) {
            flag4 = rtn2( i );
        }
        if (flag4 == -1) flag4 = rtn1( i );
    }
    return flag4;   // flag4 == -1 then things are normal, otherwise it should behave like that the exceptions has been thrown
}
int f(int i) {
    int flag5 = -1;
    for ( int j = 0; j < times; j += 1 ) {
        flag5 = g( i );
        if (flag5 != -1) j = times;
    }
    if (flag5 == -1) {
        if ( i % 2 ) {
            flag5 = g(i);
        }
        if(flag5 == -1) flag5 = g(i);
    }
    return flag5;
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


    int flag6 = f(3);
    if (flag6 == -1) {
        cout << "seed:" << seed << " times:" << times << " complete" << endl;
    } else {
        cout << "seed:" << seed << " times:" << times << " rc:" << flag6 << endl;
    }

    // try {                // begin program
    //     f( 3 );         // f => g  => rtn1 and rtn2 
    //     cout << "seed:" << seed << " times:" << times << " complete" << endl;
    // } catch( int rc ) {
    //     cout << "seed:" << seed << " times:" << times << " rc:" << rc << endl;
    // } // try
}







