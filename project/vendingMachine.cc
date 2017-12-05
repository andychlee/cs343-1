//--------------------------------------------------------------------------------------------------------------------
// Implementation to the Vending machine task
//--------------------------------------------------------------------------------------------------------------------
#include "vendingMachine.h"

//--------------------------------------------------------------------------------------------------------------------
// Constructor of VendingMachine
//--------------------------------------------------------------------------------------------------------------------
VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
    unsigned int maxStockPerFlavour ) : prt( prt ), nameServer( nameServer ), id( id ), sodaCost( sodaCost ),
    maxStockPerFlavour( maxStockPerFlavour ), isRestocking( false ), raiseType( RaiseType::none ) {

    for ( int i = 0; i < numFlavours; i += 1 ) {            // new vending machine is empty
        stocks[i] = 0;
    } // for
    nameServer.VMregister( this );                          // begins by registering with the name server
} // VendingMachine::VendingMachine

//--------------------------------------------------------------------------------------------------------------------
// Obtain one of the sodas
//--------------------------------------------------------------------------------------------------------------------
void VendingMachine::buy( Flavours flavour, WATCard &card ) {
    // update communicate variables
    this->flavour = flavour;
    this->card = &card;

    cond.wait();                                            // return transfer to main to decide raise type

    RaiseType rT = raiseType;
    raiseType = RaiseType::none;                            // reset flag variable

    switch ( rT ) {                                         // raise the exception if needs
        case ( RaiseType::funds ):
            _Throw Funds();
        case ( RaiseType::stock ):
            _Throw Stock();
        case ( RaiseType::free ):
            _Throw Free();
        default:
            break;
    } // switch
} // VendingMachine::buy

//--------------------------------------------------------------------------------------------------------------------
// Implementation to the Vending machine task
//--------------------------------------------------------------------------------------------------------------------
unsigned int *VendingMachine::inventory() {
    return stocks;
} // VendingMachine::inventory

//--------------------------------------------------------------------------------------------------------------------
// Indicate the operation is complete
//--------------------------------------------------------------------------------------------------------------------
void VendingMachine::restocked() {

} // VendingMachine::restocked

//--------------------------------------------------------------------------------------------------------------------
// returns the cost of purchasing a soda for this machine
//--------------------------------------------------------------------------------------------------------------------
unsigned int VendingMachine::cost() {
    return sodaCost;
} // VendingMachine::cost

//--------------------------------------------------------------------------------------------------------------------
// returns the identification number of the vending machine
//--------------------------------------------------------------------------------------------------------------------
unsigned int VendingMachine::getId() {
    return id;
} // VendingMachine::getId

//--------------------------------------------------------------------------------------------------------------------
// VendingMachine::main
//--------------------------------------------------------------------------------------------------------------------
void VendingMachine::main() {
    prt.print( Printer::Kind::Vending, id, 'S' , sodaCost );
    for ( ;; ) {
        _Accept( ~VendingMachine ) {
            break;
        } or _Accept( restocked ) {                         // cannot accept buy calls during restocking
            isRestocking = false;                           // indicate the restocking is completed
            prt.print( Printer::Kind::Vending, id,  'R' );  // complete reloading by truck
        } or _Accept( inventory ) {
            isRestocking = true;                            // indicate the vending machine is restocking
            prt.print( Printer::Kind::Vending, id, 'r' );   // start reloading by truck
        } or _When( !isRestocking ) _Accept( buy ) {
            if ( stocks[flavour] == 0 ) {                   // rasie Stock if the specified soda is unavailable
                raiseType = RaiseType::stock;
            } else if ( mprng( 0, 4 ) == 0 ) {              // 1 in 5 chance the soda is free, raising exception
                stocks[flavour] -= 1;                       // update stocks
                raiseType = RaiseType::free;
            } else if ( card->getBalance() < sodaCost ) {   // raise Funds if student has insufficient funds
                raiseType = RaiseType::funds;
            } else {
                stocks[flavour] -= 1;                       // update stocks
                card->withdraw( sodaCost );                 // WATCard is debited by the cost of a soda
                prt.print( Printer::Kind::Vending, id, 'B', flavour, stocks[flavour] );
            }

            cond.signalBlock();                             // let buy raise the exception
        } // _Accept
    } // for

    prt.print( Printer::Kind::Vending, id, 'F' );           // finished
} // VendingMachine::main
