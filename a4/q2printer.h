#ifndef __Q2_PRITNER_H__
#define __Q2_PRINTER_H__

#include "q2voter.h"
#include "string"

_Monitor Printer {        // chose one of the two kinds of type constructor
    unsigned int voters;
    string* buffer;
    void flush(string output, unsigned int id);
  public:
    Printer( unsigned int voters );
    ~Printer();
    void print( unsigned int id, Voter::States state );
    void print( unsigned int id, Voter::States state, TallyVotes::Tour tour );
    void print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot );
    void print( unsigned int id, Voter::States state, unsigned int numBlocked );
};

#endif