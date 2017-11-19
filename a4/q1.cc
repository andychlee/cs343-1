#include <iostream>
using namespace std;

int main( int argc, char *argv[] ) {
    int times = 1000000, size = 40;                     // defaults

    try {
        switch ( argc ) {
          case 3: size = stoi( argv[2] ); if ( size <= 0 ) throw 1;
          case 2: times = stoi( argv[1] ); if ( times <= 0 ) throw 1;
          case 1: break;                                // use defaults
          default: throw 1;
        } // switch
    } catch( ... ) {
        cout << "Usage: " << argv[0] << " [ times (> 0) [ size (> 0) ] ]" << endl;
        exit( 1 );
    } // try

    for ( int i = 0; i < times; i += 1 ) {
#ifndef STR
        int intbuf[size];                               // internal-data buffer
        for ( int i = 0; i < size; i += 1 ) intbuf[i] = i;
        for ( int i = 0; i < size; i += 1 ) cout << intbuf[i] << ' '; // internal buffering
        cout << '\n';
#else
        string strbuf;                                  // external-data buffer
        for ( int i = 0; i < size; i += 1 ) strbuf += to_string( i ) + ' '; // external buffering
        cout << strbuf << '\n';
#endif
    } // for
}