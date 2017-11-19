//--------------------------------------------------------------------------------------------------------------------
// Program implements an string parser that determine whether a string is a valid string literal
// It uses coroutine to implement the functionality
//--------------------------------------------------------------------------------------------------------------------
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <csetjmp>
#include <unistd.h>                                                 
#include <string>
#include <fstream>
using namespace std;

string simple_escape_string = "\'\"?\\abfnrtv";                     // all the simple escape sequence that we need
//--------------------------------------------------------------------------------------------------------------------
// The coroutine that has a next(char) function interface that allows character to be passed one by one
// And the main() function will raise two types of events resumpation at uMain()
// Match() when a match is found
// Error() when an error has occured
//--------------------------------------------------------------------------------------------------------------------
_Coroutine StringLiteral {
    char ch;

    // the main function that handles each char one by one
    void main() {
        // this is the loop that hanldes the prefix
        while(1) {
            if (ch == 'u') {                                        // start with u
                suspend();
                if (ch == '8') {                                    // start with u8
                    suspend();
                }
            } else if (ch == 'U' || ch == 'L') {                    // start with U or L
                suspend();
            }
            if (ch != '\"') {                                       // start with "
                _Resume Error() _At resumer();
                suspend();
            }
            else break;                                             // can now go to the loop that hanlde the string start with "
        }
        suspend();                                                  // get here becuase the cur char is ", and we should wait for the next char to get into next loop
        // this is the loop that handles the string starts with "
        while(1) {
            if(ch == '\\') {
                suspend();
                if (simple_escape_string.find(ch) != string::npos) {
                    suspend();
                } else if(ch >= '0' && ch <= '7') {
                    suspend();
                    if (ch >= '0' && ch <= '7') {
                        suspend();
                        if (ch >= '0' && ch <= '7') {
                            suspend();
                        }
                    }
                } else if(ch == 'x') {                          // \x should be hex decimal, it can have infinite lenght, use a loop to handle
                    for(;;) {                                   
                        if (isxdigit(ch)) suspend();            // keep reading hex decimals and until stop
                        else break;                             // hex decimal ended, enter the loop again and handle this
                    } // for
                } else {                                        // \followed by some other chars, should be an error 
                    _Resume Error() _At resumer();
                    suspend();
                }
            } else if(ch == '\"') {                             // the " means the end of the string so it is a match
                _Resume Match() _At resumer();
                suspend();
            } else if(ch == '\n') {
                _Resume Match() _At resumer();
                suspend();
            } else {                                            // handle all other characters
                suspend();
            }
        } // while
    }

  public:
    _Event Match {};                                            // last character match
    _Event Error {};                                            // last character invalid

    // rapper function that wrapps the resume() call from another function
    void next(char c) {
        ch = c;                                                 // communication input
        resume();                                               // activate
    }
};


//--------------------------------------------------------------------------------------------------------------------
// Helper function
// It echos the input that read in and add '' and : for formatting purpose
//--------------------------------------------------------------------------------------------------------------------
void printInput(string s) {
    cout << "'" << s << "' : ";
}


//--------------------------------------------------------------------------------------------------------------------
// Helper function
// It prints out the already matched substring of the input string
//--------------------------------------------------------------------------------------------------------------------
void printMatched(string s, int i) {
    cout << "'" << s.substr(0,i+1) << "'" << " ";
}


//--------------------------------------------------------------------------------------------------------------------
// Helper function
// It prints out the already matched substring of the input string with prefix -- extraneous characters
//--------------------------------------------------------------------------------------------------------------------
void printUnmatched(string s, int i) {
    cout << " -- extraneous characters " << "'" <<s.substr(i+1, s.length()) << "'" << endl;
}


//--------------------------------------------------------------------------------------------------------------------
// The main coroutine
// It has two types of input sources: from file and from stdin
// If reads from input stream line by line, and then pass it to StringLiteral one char by one char
// And then handles the resumption raised by StringLiteral,
// there are two types of events that are raised: Match() or Error()
// if it is a Match(), then print yes
// if it is an Error(), then print no
// if there is extra part, print that extra part with prefix "-- extraneous characters"
//--------------------------------------------------------------------------------------------------------------------
void uMain::main() {
    istream *infile = &cin;                     // default value is reading from stdin
    switch (argc) {
        case 2:                                 // if there are two aurguments, meaning from file
            try {
                infile = new ifstream(argv[1]);
            } catch(uFile::Failure ) {
                cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl; // handle the case the path given is invalid
                exit(0);
            } // try
        default:
            break;
    } // switch

    *infile >> noskipws;                       // skip white spaces

    while(1) {
        StringLiteral s;                       // coroutine that has a public interface next(char)
        string input;                          // string that holds one line of input

        getline(*infile, input);               // read in input line by line

        if (infile->fail()) break;             // handle the end of input

        printInput(input);                     // print the input without any parsing as it is done every time the input comes in

        if (input == "") {                     // check for the blank line and print warning when necessary
            cout << "Warning! Blank line." << endl; 
            continue;
        } // if

        // parsing the input to coroutine one char by one char
        int i=0;
        bool caught_resume = false;             // flag variable, indicate whether a match() or an error() has been raised
                                                // useful for dealing with the case that the input does not cause a problem but not complete
        for (i=0; i<input.length(); i++) {
            try {
                _Enable {
                    s.next(input[i]);
                }
            } catch(StringLiteral::Match) {     // a match is found, print matched part, and print yes
                printMatched(input, i);
                cout << "yes";
                caught_resume = true;
                break;
            } catch(StringLiteral::Error) {     // an error is found, print matched part and print no
                printMatched(input, i);
                cout << "no";
                caught_resume = true;
                break;
            }
        } // for

        if (!caught_resume) {                   // handle case where the coroutine raises no error or match 
            printMatched(input, i);
            cout << "no";         
        } // if

        if (i < input.length() - 1) {           // check if we need to print the extra unmatched parts on the back
            printUnmatched(input, i);
        } else {                                // all is matched, print new line
            cout << endl;
        } // if
    } // while
    if ( infile != &cin ) delete infile;        // close file, do not delete cin!
} // main
