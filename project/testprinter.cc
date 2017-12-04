#include "printer.h"
#include <iostream>
#include <string>
using namespace std;					                       // direct access to std

void uMain::main() {
	Printer prt( 2,3,1 );
	//Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier 
	prt.print(Printer::Kind::Parent, 'S');
	prt.print(Printer::Kind::Groupoff, 'S');
	prt.print(Printer::Kind::WATCardOffice, 'S');
	prt.print(Printer::Kind::NameServer, 'S');
	prt.print(Printer::Kind::Courier,0, 'S');
	prt.print(Printer::Kind::Parent, 'D',0, 1);
	prt.print(Printer::Kind::NameServer, 'R', 0);
	prt.print(Printer::Kind::Vending, 0,'S', 2);
	prt.print(Printer::Kind::Vending, 1,'S', 2);
	prt.print(Printer::Kind::Student,0, 'S', 2 , 4);
	prt.print(Printer::Kind::Vending, 0,'F');
	prt.print(Printer::Kind::WATCardOffice, 'C',0,5);
	prt.print(Printer::Kind::BottlingPlant, 'G',11);
}