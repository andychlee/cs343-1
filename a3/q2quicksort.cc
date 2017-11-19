#include <iostream>
#include <fstream>
#include <string>

#include "q2quicksort.h"

using namespace std;

unsigned int uDefaultStackSize() {
    return 512 * 1000;        // set task stack-size to 512K
}

void usage() {
    cerr << "Usage: ./quicksortINT ( -s unsorted-file [ sorted-file ] | -t size (>= 0) [ depth (>= 0) ] )" << endl;
    exit(-1);
}

void sorting_task(TYPE values[], int depth, int size, ostream* outfile, bool print_output) {
    {
        uProcessor p[ (1 << depth) - 1 ] __attribute__(( unused )); // 2^depth-1 kernel threads                                                                                                      // make sure ther sorting is done before printing
        Quicksort<TYPE> q(values, 0, (unsigned int)size , (unsigned int)depth);
    }
    if (print_output) {
        for(int i=0; i < size+1; i++) {
            *outfile << values[i];
            if (((i+1) % 22 != 0 && i != size) || (i == 0 && size != 0)) {
                *outfile << " ";
                continue;
            }
            if ((i+1) % 22 == 0) *outfile << endl << "  ";
        }
        *outfile << endl;
    }
}

int main( int argc, char * argv[] ) {
    unsigned int depth = 0;
    int size;
    istream *infile = &cin;                                                                                 // default value
    ostream *outfile = &cout;                                                                               // default value
    TYPE *values;                                                                                           // store the input values

    if (argc < 3  || argc > 4) {
        cerr << "Invalid number of arguments" << endl;
        usage();
    }

    if (string(argv[1]) == "-s") {
        try {
            infile = new ifstream(argv[2]);
        } catch (...) {
            cerr << "unable to open input file with " <<  argv[2] << endl;
            usage();
        }
        if (argc > 3) {
            try {
                outfile = new ofstream(argv[3]);
            } catch (...) {
                cerr << "unable to create output file" << endl;
                usage();
            }
        } 
    } else if (string(argv[1]) == "-t") {
        try {
            size = stoi(argv[2]);
            if (size < 0)  {
                cerr << "Invalid size passed" << endl;
                usage();
            }
        } catch(...) {
            cerr << "Failed to read input file" << endl;
            usage();            
        }
        if (argc > 3) {
            try {
                depth = stoi(argv[3]);
            } catch(...) {
                cerr << "Failed to read size" << endl;
                usage();
            }
        }
    } else {
        cerr << "invalid first option" << endl;
        usage();
    }

    int group_length;                                                                                      // length of the group
    TYPE tmp;                                                                                              // to hold the current value 
    if (string(argv[1]) == "-s") {
        while (*infile >> group_length) {
            values = new TYPE[group_length];
            for(int i=0; i < group_length; i++) {
                *infile >> tmp;
                values[i] = tmp;
                *outfile << values[i];
                if (((i+1) % 22 != 0 && i != group_length-1) || (i == 0 && group_length-1 != 0)) {
                    *outfile << " ";
                    continue;
                }
                if ((i+1) % 22 == 0) *outfile << endl << "  ";
            }
            *outfile << endl;
            sorting_task(values, depth, group_length-1, outfile, true);
            delete values;
            *outfile << endl;
        }
    } else {                                                                                               // -t opetion dpest not read input
        values = new TYPE[size];
        for(int i=size; i >= 1; i--) {
            values[i] = i;
        }
        sorting_task(values, depth, size, outfile, false);
        delete values;
    }
    if (infile != &cin) delete infile;
    if (outfile != &cout) delete outfile;


} // main
