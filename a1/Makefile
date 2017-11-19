CXX = u++				# compiler
CXXFLAGS = -g -Wall -MMD -std=c++11	# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}# makefile name

OBJECTS01 = q1exception.o		# optional build of given program
EXEC01 = exception				# given executable name

OBJECTS1 = q1flags.o # object files forming 1st executable with prefix "q1"
EXEC1 = flags					# 1st executable name

OBJECTS2 = q1returncodes.o # object files forming 2nd executable with prefix "q1"
EXEC2 = returncodes				# 2nd executable name

OBJECTS3 = q1globalstatusflag.o # object files forming 3rd executable with prefix "q1"
EXEC3 = globalstatusflag		# 3rd executable name

OBJECTS02 = q2throwcatch.o		# optional build of given program
EXEC02 = throwcatch				# given executable name

OBJECTS4 = q2longjmp.o 				# object files forming 4th executable with prefix "q2"
EXEC4 = longjmp					# 4th executable name

OBJECTS5 = q3.o 				# object files forming 5th executable with prefix "q3"
EXEC5 = stringliteral			# 5th executable name

OBJECTS = ${OBJECTS1} ${OBJECTS2} ${OBJECTS3} ${OBJECTS4} ${OBJECTS5}
DEPENDS = ${OBJECTS:.o=.d}
EXECS = ${EXEC1} ${EXEC2} ${EXEC3} ${EXEC4} ${EXEC5}

#############################################################

.PHONY : all clean

all : ${EXECS}				# build all executables

${EXEC01} : ${OBJECTS01}		# optional build of given program
	g++-6 ${CXXFLAGS} $^ -o $@

q1.o : q1%.cc				# change compiler 1st executable, ADJUST SUFFIX (for .C/.cpp)
	g++-6 ${CXXFLAGS} -c $< -o $@

q1%.o : q1%.cc				# change compiler 1st executable, ADJUST SUFFIX (for .C/.cpp)
	g++-6 ${CXXFLAGS} -c $< -o $@

${EXEC1} : ${OBJECTS1}			# compile and link 1st executable
	g++-6 ${CXXFLAGS} $^ -o $@

${EXEC02} : ${OBJECTS02}		# optional build of given program
	g++-6 ${CXXFLAGS} $^ -o $@

${EXEC2} : ${OBJECTS2}			# compile and link 2nd executable
	g++-6 ${CXXFLAGS} $^ -o $@

${EXEC3} : ${OBJECTS3}			# compile and link 3rd executable
	g++-6 ${CXXFLAGS} $^ -o $@

q2.o : q2.cc				# change compiler 2nd executable, ADJUST SUFFIX (for .C/.cpp)
	g++-6 ${CXXFLAGS} -c $< -o $@

q2%.o : q2%.cc				# change compiler 2nd executable, ADJUST SUFFIX (for .C/.cpp)
	g++-6 ${CXXFLAGS} -c $< -o $@

${EXEC4} : ${OBJECTS4}			# compile and link 4th executable
	g++-6 ${CXXFLAGS} $^ -o $@

${EXEC5} : ${OBJECTS5}			# compile and link 5th executable
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}		# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}			# include *.d files containing program dependences

clean :					# remove files that can be regenerated
	rm -f *.d *.o ${EXEC01} ${EXEC02} ${EXECS}