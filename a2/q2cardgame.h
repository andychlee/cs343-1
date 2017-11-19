#ifndef Q2CARDGAME_H
#define Q2CARDGAME_H

#include <vector>

class Info {
  public:
    int id;
    int took;
    int deck_remain;
    int direction;                          // 0 left, 1 right
    int dead;
    int drunk;                              // 0 is sobber, 1 is drunk
    bool flushed;                           // flushed
    bool win;                               // win or not
    Info(int id, int took, int deck_remain, int direction, int dead, int drunk);
};


class Printer {
    // YOU MAY ADD PRIVATE MEMBERS
    std::vector<Info*> v;
    unsigned int NoOfPlayers;
    unsigned int NoOfCards;
    void flush();           // the one that actually prints
    void printPlayerHeader();
  public:
    Printer( const unsigned int NoOfPlayers, const unsigned int NoOfCards );
    void prt( unsigned int id, int took, int RemainingPlayers );

};

_Coroutine Player {
    // YOU MAY ADD PRIVATE MEMBERS, INCLUDING STATICS
    static unsigned int player_count;
    unsigned int deck;
    unsigned int id;
    Printer &printer;
    Player *left;
    Player *right;
    void passCard(int deck);
    void main();
    bool drunk;
  public:
    enum { DEATH_DECK_DIVISOR = 7 };
    static void players( unsigned int num );
    Player( Printer &printer, unsigned int id );
    void start( Player &lp, Player &rp );
    void play(unsigned int deck);
    void drink();
};

unsigned int Player:: player_count;

#endif