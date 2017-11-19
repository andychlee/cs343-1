#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#include "q1binsertsort.h"

using namespace std;

// ------------------------------------------------------------------------------------
// Example of sorting non-basic type
// We defining a new struct Triple
// it contains three integers x, y, z

struct Triple {
    int x;
    int y;
    int z;

    Triple(int x, int y, int z) : x(x), y(y), z(z) { }
};

bool operator==(const Triple& l, const Triple& r) {
    return (l.x == r.x && l.y == r.y && l.z == r.z);
}

bool operator<=(const Triple& l, const Triple& r) {
    return (l.x <= r.x && l.y <= r.y && l.z <= r.z);
}

istream& operator>>(istream& is, Triple& t) {
    is >> t.x;
    t.y = 0;
    t.z = 0;
    return is;
}

ostream& operator<<(ostream& os, const Triple& t) {
    os << "(" << t.x << "," << t.y << "," << t.z << ")";
    return os;
}
// ------------------------------------------------------------------------------------


template<typename T>
void Binsertsort<T>::main() {
    _Coroutine Binsertsort greater(Sentinel);                                   // the right vertex
    _Coroutine Binsertsort less(Sentinel);                                      // the left vertex
    pivot = value;
    suspend();                                                                  // the first call, the coroutine stores the pivot.
    for(;;) {
        if (value == Sentinel) {
            less.sort(Sentinel);                                                // let all my children know that it is the end of the inserting stage
            greater.sort(Sentinel);                                             // so that now they will all enter this if statement and break this loop
            break;                                                              // break the loop, sorting is done, now enter the loop of retrieving values
        }
        if (value < pivot) {
            less.sort(value);                                                   // go to less when value < pivot
            suspend();
        } else {
            greater.sort(value);                                                // go to greater when value >= pivot
            suspend();
        }
    }
    suspend();

    // using the value as a placeholder to let parents know the value is returning
    // the loop of retrieving left values 
    for(;;) {
        value = less.retrieve();
        if (value == Sentinel) break;
        suspend();
    }

    // now return the middle values
    value = pivot;
    suspend();

    // the loop of retrieving right values
    for(;;) {
        value = greater.retrieve();
        if (value == Sentinel) break;
        suspend();
    }
    value = Sentinel;                                                           // indicate that this guy is done
}


template<typename T>
Binsertsort<T>::Binsertsort( T Sentinel ) : Sentinel( Sentinel ) {}

  
template<typename T>
void Binsertsort<T>::sort( T value ) {                                          // value to be sorted
    Binsertsort::value = value;
    resume();
}


template<typename T>
T Binsertsort<T>::retrieve() {                                                  // retrieve sorted value
    resume();
    return value;
}


void usage(char *argv[]) {
    cerr << "Usage: " << argv[0] << "[ input-file [ output-file ] ] ] ]" << endl;
    exit(-1);               // TERMINATE
} // usage


void uMain::main() {
    istream *infile = &cin;                                                     // default value
    ostream *outfile = &cout;                                                   // default value

    switch ( argc ) {
        case 3:                                                                 // have both input and output                                                         
            try {
                outfile = new ofstream(argv[2]);
            } catch(uFile::Failure) {          // open failed ?
                cerr << "Error! Could not open output file \"" << argv[2] << "\"" << endl;
                usage(argv);
            } // try
            // fall through
        case 2:
            try {
                infile = new ifstream(argv[1]);
            } catch(uFile::Failure) {
                cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
                usage(argv);
            } // try
            break;
        default:                       // all else
            cerr << "Invalid number of arguments" << endl;
            usage(argv);       
    } // switch

    int size;
    TYPE input;
    for(;;) {
        *infile >> size;                                                        // read in the size of the input
        Binsertsort<TYPE> tree(SENTINEL);                                       // coroutine instance

        if(!*infile) break;                                                     // handle the read input fail                     

        
        for (int i = 0; i < size; i++) {                                        // sorting loop
            *infile >> input;                                                   // read a T
            if (!*infile) {                                                     // handle the read input fail
                cerr << endl << "input has invalid structure";
                exit(-1);
            }
            *outfile << input;                                                  // print the original input
            if (i < size - 1) *outfile << " ";                                  // print space
            tree.sort(input);
        } // for
        tree.sort(SENTINEL);                                                    // indicate the end of a set of input
        *outfile << endl;                      

        for (int i = 0; i < size; i++) {                                        // output loop
            *outfile << tree.retrieve();                                        // print the sorted value
            if (i < size - 1) *outfile << " ";                                  // print space
        } // for
        *outfile << endl << endl;           
    }
    
    if ( infile != &cin ) delete infile;                                        // close file, do not delete cin!
    if ( outfile != &cout ) delete outfile;                                     // close file, do not delete cout!
}
