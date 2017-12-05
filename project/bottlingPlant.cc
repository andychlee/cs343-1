//--------------------------------------------------------------------------------------------------------------------
// Implementation to the bottlingPlant Task
//--------------------------------------------------------------------------------------------------------------------
#include "bottlingPlant.h"

//--------------------------------------------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------------------------------------------
BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour,
                     unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments):
              prt(prt), nameServer(nameServer), numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour),
              maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments), timeToShutdown(false),
              currentStorage(0) {} // Constructor


//--------------------------------------------------------------------------------------------------------------------
// The getShipment function of the bottlingPlant
//--------------------------------------------------------------------------------------------------------------------
void BottlingPlant::getShipment(unsigned int cargo[]) {
    if (timeToShutdown) _Throw Shutdown();                                      // in the shutdown case, no shipement is copied
    for (int i =0; i < numFlavours; i++) {                                      // assumming we have 4 flavours
        cargo[i] = currentStorage;                                              // put the storage in the crago
    }
    currentStorage = 0;                                                         // no storage left in the plant
}   // BottlingPlant::getShipment

//--------------------------------------------------------------------------------------------------------------------
// The main function of the bottlingPlant
//--------------------------------------------------------------------------------------------------------------------
void BottlingPlant::main() {
    prt.print(Printer::Kind::BottlingPlant, 'S');                               // print starting message
    Truck t(prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);    // create a truck

    for(;;) {
        if (currentStorage == 0) {                                              // if the storage is empty
            currentStorage = mprng(maxShippedPerFlavour);                       // create a production run
            prt.print(Printer::Kind::BottlingPlant, 'G', currentStorage * numFlavours);
        }
        yield(timeBetweenShipments);                                            // stop for a period of time
         _Accept(~BottlingPlant) {
            timeToShutdown = true;                                              // set the flag, next getshipment will throw execption
            break;
        } or _Accept(getShipment) {
            prt.print(Printer::Kind::BottlingPlant, 'P');                       // shipment got by truck, assuming truck can take infinite amount
        }   //_Accept
    }   // for

    try {
        _Accept(getShipment) {}                                                 // the last getShipement call, wil throw exception
    } catch(uMutexFailure::RendezvousFailure) {                                 // special kind of exception that indicates problem on task's thread
    }  // try

    prt.print(Printer::Kind::BottlingPlant, 'F');                               // print stopping message
} // BottlingPlant::main
