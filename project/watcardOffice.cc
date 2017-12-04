//--------------------------------------------------------------------------------------------------------------------
// Implementation to the watcardOffice
//--------------------------------------------------------------------------------------------------------------------

#include "watcardOffice.h"
#include "MPRNG.h"

//--------------------------------------------------------------------------------------------------------------------
// Constructor of WATCardOffice
//--------------------------------------------------------------------------------------------------------------------
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : prt( prt ), bank( bank ),
	numCouriers( numCouriers ) {
	// creates a courier pool with numCouriers courier tasks
	courierPool = new Courier*[numCouriers];
	for ( unsigned int c = 0; c < numCouriers; c += 1 ) {
		courierPool[c] = new Courier( prt, bank, *this, c );
	} // for
} // WATCardOffice::WATCardOffice

//--------------------------------------------------------------------------------------------------------------------
// Destructor of WATCardOffice
//--------------------------------------------------------------------------------------------------------------------
WATCardOffice::~WATCardOffice() {
	for ( unsigned i = 0; i < numCouriers; i += 1 ) {				// free memory
		delete courierPool[i];
	} // for
	delete courierPool;

	while ( !cardList.empty() ) {
		delete cardList.front();
		cardList.pop_front();
	}
} // WATCardOffice::~WATCardOffice

//--------------------------------------------------------------------------------------------------------------------
// Constructor of WATCardOffice::Courier
//--------------------------------------------------------------------------------------------------------------------
WATCardOffice::Courier::Courier( Printer &prt, Bank &bank, WATCardOffice &cardOffice, unsigned int lid ) :
	prt( prt ), bank( bank ), cardOffice( cardOffice ), lid( lid ) {}

//--------------------------------------------------------------------------------------------------------------------
// Create a "real" WATCard with an initial balance
//--------------------------------------------------------------------------------------------------------------------
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
	WATCard *card = new WATCard();	 										// create a real watcard
	cardList.push_back(card);												// update card list
	Job *job = new Job( Job::Args( sid, amount, card ) ); 					// create work request
	requests.push_back( job );												// add to list of requests

	prt.print( Printer::Kind::WATCardOffice, 'C', sid, amount ); 			// print state info
	return job->result;
} // WATCardOffice::create

//--------------------------------------------------------------------------------------------------------------------
// Transfer funds from a student's bank-account to its WATCard
//--------------------------------------------------------------------------------------------------------------------
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
	Job *job = new Job( Job::Args( sid, amount, card ) ); 					// create work request
	requests.push_back( job );												// add to list of requests

	prt.print( Printer::Kind::WATCardOffice, 'T', sid, amount ); 			// print state info
	return job->result;
} // WATCardOffice::transfer

//--------------------------------------------------------------------------------------------------------------------
// WATCardOffice::Courier::main
//--------------------------------------------------------------------------------------------------------------------
void WATCardOffice::Courier::main() {
	prt.print( Printer::Kind::Courier, lid, 'S' );							// starting

	for ( ;; ) {
		_Accept( WATCardOffice::~Courier ) {
			break;
		} _Else {
			Job *job = cardOffice.requestWork(); 							// take next work request
			if (job == NULL) break;											// courier is about to be deleted

			WATCard *card = job->args.card; 								// student's watcard
			unsigned int sid = job->args.sid;								// student id
			unsigned int amount = job->args.amount;							// amount to transfer

			prt.print( Printer::Kind::Courier, lid, 't', sid, amount ); 	// start funds transfer
			bank.withdraw( sid, amount );									// obtain money from the bank
			card->deposit( amount );										// update the student's WATCard

			// there is a 1 in 6 chance a courier losses a student's WATCard
			if (mprng(0, 5) == 0) {
				job->result.exception( new Lost() ); 						// inserted exception into the future
				prt.print( Printer::Kind::Courier, lid, 'L', sid ); 		// lost WATCard card
			} else {
				job->result.delivery( card ); 								// making the future available
				prt.print( Printer::Kind::Courier, lid, 'T', sid, amount ); // complete funds transfer
			} // if

			delete job;
		} // _Accept
	} // for

	prt.print(Printer::Kind::Courier, lid, 'F'); 							// finished
} // WATCardOffice::Courier::main

//--------------------------------------------------------------------------------------------------------------------
// WATCardOffice::main
//--------------------------------------------------------------------------------------------------------------------
void WATCardOffice::main() {
	prt.print(Printer::Kind::WATCardOffice, 'S');							// starting

	for ( ;; ) {
		_Accept( WATCardOffice::~WATCardOffice ) {
			for ( unsigned i = 0; i < numCouriers; i += 1 ) {
				// courier is still waiting for the work, should let them through
				_Accept( requestWork );
			} // for

			break;
		} or _When( requests.size() > 0 ) _Accept( requestWork ) { 			// blocks until a Job request is ready
		} or _Accept( create ) {
		} or _Accept( transfer ) {
		}
	} // for

	prt.print(Printer::Kind::WATCardOffice, 'F');							// finished
} // WATCardOffice::main

//--------------------------------------------------------------------------------------------------------------------
// Return the next Job request
//--------------------------------------------------------------------------------------------------------------------
WATCardOffice::Job *WATCardOffice::requestWork() {
	if ( requests.empty() ) {
		// technically, the statement will be true when the destructor is called
		return NULL;
	}

	Job *job = requests.front();											// take next work request
	requests.pop_front();													// remove request
	prt.print( Printer::Kind::WATCardOffice, 'W' ); 						// request work call complete
	return job;
} // WATCardOffice::requestWork
