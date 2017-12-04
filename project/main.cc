//--------------------------------------------------------------------------------------------------------------------
// Main program of WATCola
//--------------------------------------------------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <string>
#include "MPRNG.h"
#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watcardOffice.h"
#include "groupoff.h"
#include "nameServer.h"
#include "vendingMachine.h"
#include "bottlingPlant.h"
#include "student.h"

using namespace std;

MPRNG mprng;												// initialize global random generator

//--------------------------------------------------------------------------------------------------------------------
// Indicate if the value can be converted to integer
//--------------------------------------------------------------------------------------------------------------------
bool convert( int &val, char *buffer ) {                    // convert C string to integer
    std::stringstream ss( buffer );                         // connect stream and buffer
    string temp;
    ss >> dec >> val;                                       // convert integer from buffer
    return ! ss.fail() &&                                   // conversion successful ?
    ! ( ss >> temp );                                       // characters after conversion all blank ?
} // convert

//--------------------------------------------------------------------------------------------------------------------
// Given command line arguments, print the usage and exit
// This is used for command line arguments error handling
//--------------------------------------------------------------------------------------------------------------------
void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0]
     << " [ config-file [ random-seed (> 0) ] ]"
     << endl;
    exit( EXIT_FAILURE );                                   // TERMINATE
} // usage

//--------------------------------------------------------------------------------------------------------------------
// uMain::main
//--------------------------------------------------------------------------------------------------------------------
void uMain::main() {
	int seed = getpid(); 									// seed for the random-number generator
	string configFile = "soda.config"; 						// name of text file containing the configuration constants
	switch ( argc ) {
		case 3:												// read seed
			if ( !convert( seed, argv[2] ) || seed <= 0 ) {	// invalid integer
				usage( argv );
			} // if
		case 2:												// read configFile
			configFile = argv[1];
		case 1:
			break;
		default:
			usage( argv );									// wrong number of arguments
	} // switch

	mprng.set_seed( seed );									// setup the seed of random number generator
	ConfigParms configParms;								// reserved for the parsed values
	processConfigFile( configFile.c_str(), configParms );	// read and parse the simulation configurations

	// creater in order the printer, bank, parent, WATCARD office, groupoff, name server, vending machines, bottling plant, and students
	Printer printer( configParms.numStudents, configParms.numVendingMachines, configParms.numCouriers );

	Bank bank( configParms.numStudents );

	Parent parent( printer, bank, configParms.numStudents, configParms.parentalDelay );

	WATCardOffice cardOffice( printer, bank, configParms.numCouriers );

	Groupoff groupoff( printer, configParms.numStudents, configParms.sodaCost, configParms.groupoffDelay );

	NameServer nameServer( printer, configParms.numVendingMachines, configParms.numStudents );

	VendingMachine * machines[configParms.numVendingMachines];
	for ( unsigned int m = 0; m < configParms.numVendingMachines; m += 1 ) {
		machines[m] = new VendingMachine( printer, nameServer, m, configParms.sodaCost, configParms.maxStockPerFlavour);
	} // for

	BottlingPlant * plant = new BottlingPlant( printer, nameServer, configParms.numVendingMachines, configParms.maxShippedPerFlavour,
			configParms.maxStockPerFlavour, configParms.timeBetweenShipments);

	Student * students[configParms.numStudents];
	for ( unsigned int s = 0; s < configParms.numStudents; s += 1 ) {
		students[s] = new Student( printer, nameServer, cardOffice, groupoff, s, configParms.maxPurchases );
	} // for

	// the program terminates once all of the students have purchased their specified number of bottles
	for ( unsigned int s = 0; s < configParms.numStudents; s += 1 ) {
		delete students[s];
	} // for

	// delete the bottling plant before deleting the vending machines
	delete plant;

	for ( unsigned int m = 0; m < configParms.numVendingMachines; m += 1 ) {
		delete machines[m];
	} // for
} // uMain::main
