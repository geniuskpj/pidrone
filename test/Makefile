CC := g++

EXTRAINCDIRS += -I "../inc"
EXTRALDFLAGS += -L/usr/local/lib
EXTRALIBS += -lRTIMULib -lwiringPi

CFLAGS +=

MYOBJS = ../inc/Sonar.o

.PHONY: all	
all: bin/sonartest bin/imutest bin/ledtest

.PHONY: clean
clean:
	rm -rf bin/*.o bin/*

../inc/%.o: ../inc/%.cxx
	${CC} -c $< ${CFLAGS} -o $@ ${EXTRALDFLAGS} ${EXTRALIBS}

bin/imutest : imutest.cxx
	${CC} ${CFLAGS} $^ -o $@ ${EXTRALDFLAGS} ${EXTRALIBS} ${EXTRAINCDIRS} ${MYOBJS}

bin/sonartest : sonartest.cxx
	${CC} ${CFLAGS} $^ -o $@ ${EXTRALDFLAGS} ${EXTRALIBS} ${EXTRAINCDIRS} ${MYOBJS}

bin/ledtest : ledtest.cxx
	${CC} ${CFLAGS} $^ -o $@ ${EXTRALDFLAGS} ${EXTRALIBS} ${EXTRAINCDIRS} ${MYOBJS}

