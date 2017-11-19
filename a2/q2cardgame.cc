#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "q2cardgame.h"
#include "PRNG.h"

using namespace std;

PRNG* prng;                                                                                         // random number generator, put in global scope so everyone can access
int DEATH_DECK_DIVISOR = 7;
_Event Schmiubilik {};

int min(int a, int b) {
    return a < b ? a : b;
}

// constructor of Info struct that is used to buffer the information that the printer will print
Info::Info(int id, int took, int deck_remain, int direction, int dead, int drunk):
    id(id), took(took), deck_remain(deck_remain), direction(direction), dead(dead), drunk(drunk), flushed(true), win(false) { }


// constructor of the Printer
Printer::Printer(const unsigned int NoOfPlayers, const unsigned int NoOfCards): NoOfPlayers(NoOfPlayers), NoOfCards(NoOfCards) {
    printPlayerHeader();                                                                            // print header for each game

    for(unsigned int i=0; i < NoOfPlayers; i++) {
        v.push_back(new Info((int)i, 0, NoOfCards, 0, 0, 0));                                       // indicate that there is no info to be printed
    }
}

void Printer::prt(unsigned int id, int took, int RemainingPlayers) {
    if (!v[id]->flushed) {                                                                          // need to flush the buffer at this point
        flush();                                                                                    // flush routine will set everyone to be flushed
    }

    if (took != -1) NoOfCards -= took;
    v[id]->flushed = false;
    v[id]->took = took;
    v[id]->deck_remain = NoOfCards;
    v[id]->direction = v[id]->deck_remain % 2 == 0 ? 1 : 0;
    if (took != -1) v[id]->dead = (v[id]->deck_remain + took) % DEATH_DECK_DIVISOR == 0 ? 1 : 0;
    v[id]->drunk = 0;
    v[id]->win = RemainingPlayers == 1;

    if (RemainingPlayers == 1 || NoOfCards == 0) {
        flush();                                                                                    // game over, now flush everything
        for (unsigned int i=0; i < NoOfPlayers; i++) {
            delete v[i];                                                                            // clean up
        }
    }
}

void Printer::flush() {
    int print_till = 0;                                                                             // we need find the rightmost player that need to be printerd
    for(int i=(int)NoOfPlayers-1; i >= 0; i--) {
        if (!v[i]->flushed) {
            print_till=i;
            break;
        }
    }

    for(int i=0; i <= print_till; i++) {
        Info* temp = v[i];
        if (!temp->flushed) {
            if (temp->win) {
                cout << "#" << NoOfCards << "#";
            } else {
                cout << temp->took << ":" << temp->deck_remain;
                if (temp->deck_remain == 0) cout << "#";                                                // game over
                if (temp->deck_remain != 0) cout << (temp->direction == 0 ? "<" : ">");                 // going left or going right
                if (temp->dead) cout << "X";                                                            // death
            }
            temp->flushed = true;
        }
        if (i < print_till) cout << "\t";
    }
    cout << endl;
}

void Printer::printPlayerHeader() {
    cout << "Players: " << NoOfPlayers << "    " << "Cards: " << NoOfCards << endl;                 // the output program uses space in between here and tab everywhere else
    for (int i=0; i<NoOfPlayers; i++) {
        cout << "P" << i;
        if (i < NoOfPlayers - 1) cout << "\t";
    }
    cout << endl;
}   


// constructor
Player::Player(Printer &printer, unsigned int id): 
    printer(printer), id(id), deck(0), left(NULL), right(NULL) { }


void Player::players(unsigned int num) {
    player_count = num;
}


// start the game
void Player::start(Player &lp, Player &rp) {
    left = &lp;
    right = &rp;
    resume();                                                                   // this resume makes sure the starter is uMain, and this is important
}                                                                               // as we make sure all coroutines goes back to uMain


// player set the deck of cards
void Player::play(unsigned int deck) {
    Player::deck = deck;
    resume();
}

void Player::drink_helper() {
    drunk = true;
    if (!right->drunk) {
        _Resume E() at *right;
        printer.ptr(id, -1, player_count);
    } 
    drunk = false;
    suspend();
}

// player take a drink
void Player::drink() {
    resume();
}


void Player::passCard(int deck) {
    if (deck % 2 == 0) {
        right->play(deck);
    } else {
        left->play(deck);
    }
}


void Player::main() {
    suspend();                                                                          // first resume is just get everything started, nothing done

    try {
        _Enable {
            for (;;) { 
                // cout << id << endl;
                // cout << "left" << left->id << endl;
                // cout << "right" << right->id << endl;

                if (player_count == 1) {                                                // print before terminate
                    printer.prt(id, 0, player_count);
                    return;
                }
                                                                                        // the main loop for playing cards
                int random_draw = min(deck, (*prng)(1,8));                              // have to draw a card before dead
                printer.prt(id, random_draw, player_count);

                if (deck % DEATH_DECK_DIVISOR == 0) {                                   // dead
                    player_count -= 1;
                    left->right = right;
                    right->left = left;
                    // deck -= random_draw;
                    // passCard(deck);
                    // return
                }        
                deck -= random_draw;                                                    // update the deck count and pass it
                                                                                        // schmiubilik
                if ((*prng)(0,9) == 0)   {                                              
                    _Resume E() at *right;
                } 
                passCard(deck);                                                         // pass the deck  

                if (deck == 0) {
                    player_count -= 1;
                    return;   
                }
            }
        } _CatchResume(Schmiubilik) {
            drinking();
            suspend();
        } 
    }
}

// print usage message
void usage(char *argv[]) {
    cerr << "Usage: " << argv[0] << " [ games >= 0 | 'x' [ players >= 2 | 'x' [ cards > 0 | 'x' [ seed > 0 | 'x' ] ] ] ]" << endl;
    exit(0);               // TERMINATE
} // usage


// handle command line arguments games
// raise excepition and quit when recieve bad comand line argument
void setUpGames(int* games, string g, char* argv[]) {
    if (g == "x") *games = 5; 
    else {
        try {
            *games = stoi(argv[1]);
            if (*games < 0) throw invalid_argument("games cannot be negative");
        } catch(...) {
            cerr << "passed an invalid games number" << endl;
            usage(argv);
        }
    } 
}

// handle command line arguments seed
// raise excepition and quit when recieve bad comand line argument
void setUpSeed(int* seed, string s, char* argv[]) {
    if (s == "x") *seed = getpid(); 
    else {
        try {
            *seed = stoi(argv[4]);
            if (*seed <= 0) throw invalid_argument("seed has to be positive");
        } catch(...) {
            cerr << "passed an invalid seed number" << endl;
            usage(argv);
        }
    } 
}

// handle command line arguments players
// raise excepition and quit when recieve bad comand line argument
void setUpPlayers(int* players, string p, char* argv[], PRNG *prng) {
    if (p == "x") *players = (int)((*prng)(2, 10));                        // range in 2 to 10 
    else {
        try {
            *players = stoi(argv[2]);
            if (*players < 2 || *players > 10) throw invalid_argument("players has to be in range 2 and 10");
        } catch(...) {
            cerr << "passed an invalid players number" << endl;
            usage(argv);
        }
    } 
}


// handle command line arguments cards
// raise excepition and quit when recieve bad comand line argument
void setUpCards(int* cards, string c, char* argv[], PRNG *prng) {
    if (c == "x") *cards = (int)((*prng)(10, 200));                        // range in 10 to 200 
    else {
        try {
            *cards = stoi(argv[3]);
            if (*cards < 0) throw invalid_argument("cards has to be greater than 0");
        } catch(...) {
            cerr << "passed an invalid cards number" << endl;
            usage(argv);
        }
    } 
}

void uMain::main() {
    int games = 5;
    int seed = getpid();
    int cards;
    int players;
    string s;
    string c;
    string p;
    string g;

    prng = new PRNG();            // random number generator
    switch ( argc ) {
        case 5:
            s = argv[4];
            setUpSeed(&seed, s, argv);        
            // follow through
        case 4:
            c = argv[3];
            p = argv[2];
            g = argv[1];
            prng->seed(seed);                                                   // pass the seed to random number generator
            
            setUpPlayers(&players, p, argv, prng);
            setUpCards(&cards, c, argv, prng);   
            setUpGames(&games, g, argv);
            break;
        case 3:
            p = argv[2];
            g = argv[1];
            prng->seed(seed);                                                   // pass the seed to random number generator
            
            setUpPlayers(&players, p, argv, prng);
            setUpCards(&cards, "x", argv, prng);                                // pass "x" so that it will randomly assign a number 
            setUpGames(&games, g, argv);                                                                                                                    
            break;
        case 2:
            g = argv[1];
            prng->seed(seed);                                                   // pass the seed to random number generator

            setUpPlayers(&players, "x", argv, prng);                            // pass "x" so that it will randomly assign a number
            setUpCards(&cards, "x", argv, prng);                                // pass "x" so that it will randomly assign a number 
            setUpGames(&games, g, argv);
            break;

        case 1: 
            prng->seed(seed);                                                   // pass the seed to random number generator
                                                                                // no arguments passed
            setUpPlayers(&players, "x", argv, prng);                            // pass "x" so that it will randomly assign a number
            setUpCards(&cards, "x", argv, prng);                                // pass "x" so that it will randomly assign a number 
            setUpGames(&games, "x", argv);                                      // pass "x" so that it will randomly assign a number 
            break;                                                          

        default:                       // all else
            cerr << "Invalid number of arguments" << endl;
            usage(argv);       
    } // switch

    for (int j =0; j < games; j++) {
        // enter the loop of creating players
        if (j >0) {
            if (argc < 4 || c == "x") setUpCards(&cards, "x", argv, prng);
            if (argc < 3 || p == "x") setUpPlayers(&players, "x", argv, prng);
        }
        Player::players(players);
        vector<Player*> v;
        Printer printer(players, cards);                                        // create a new printer for each round

        for (int i=0; i < players; i++) {
            Player* p = new Player(printer, i);
            v.push_back(p);
        }

        // start the game, assign the left neighbour and the right neighbour
        for (int i=0; i < players;i++) {
            if (i == 0) v[i]->start(*v[players-1], *v[1]);
            else if (i == players-1) v[i]->start(*v[players-2], *v[0]);
            else v[i]->start(*v[i-1], *v[i+1]);
        }

        // generate a random number to see who starts
        int first = (int)(*prng)(0, players - 1);
        v[first]->play(cards);

        // clean up
        for (int i=0; i < players; i++) delete v[i];

        // print new line for ending the game
        if (j < games-1) cout << endl << endl;
    }

    // clean up
    delete prng;
}