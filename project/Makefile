CXX = u++										# compiler
CXXFLAGS = -g -Wall -MMD -std=c++11  			# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = main.o config.o printer.o bank.o parent.o watcardOffice.o watcard.o groupoff.o nameServer.o vendingMachine.o bottlingPlant.o truck.o student.o
EXEC1 = soda

OBJECTS = ${OBJECTS1}				# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC1}					# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}					# build all executables


${EXEC1} : ${OBJECTS1}			# compile and link 1st executable
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS}
