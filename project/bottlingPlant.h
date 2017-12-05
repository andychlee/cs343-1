//--------------------------------------------------------------------------------------------------------------------
// Define the interface to the Bottling Plant
// Periodically produces random new quantities of each flavour of soda
//--------------------------------------------------------------------------------------------------------------------
#ifndef __BOTTLING_PLANT_H__
#define __BOTTLING_PLANT_H__

#include "printer.h"
#include "nameServer.h"
#include "MPRNG.h"
_Task BottlingPlant;
#include "truck.h"

_Task BottlingPlant {
      static const int numFlavours = 4;     // number of flavours
      Printer &prt;                         // printer to print state info
      NameServer & nameServer;              // name server to locate vending machine
      unsigned int numVendingMachines;      // number of vending machines
      unsigned int maxShippedPerFlavour;    // max num of bottles of each flavour
      unsigned int maxStockPerFlavour;      // max number of bottles in a vending machine
      unsigned int timeBetweenShipments;    // the length of time between shipment pickups
      bool timeToShutdown;
      unsigned int currentStorage;          // current storage per flavour!
      void main();
    public:
      _Event Shutdown {};                   // shutdown plant
      BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines, unsigned int maxShippedPerFlavour,
                     unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments );
      void getShipment( unsigned int cargo[] );
};

#endif
