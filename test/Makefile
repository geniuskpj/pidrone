CC := g++

EXTRAINCDIRS += -I "../inc"
EXTRALDFLAGS += -L/usr/local/lib
EXTRALIBS += -lRTIMULib -lwiringPi -lm -lmosquitto -lwiringPiPca9685 -lpthread -lwiringPi

CFLAGS +=

MYOBJS = ../inc/Sonar.o ../inc/nmea.o ../inc/gps.o ../inc/serial.o

.PHONY: all	
all: ../inc/Sonar.o ../inc/nmea.o ../inc/gps.o ../inc/serial.o bin/sonartest bin/imutest bin/ledtest

.PHONY: clean
clean:
	rm -rf bin/*.o bin/* rm -rf ../inc/*.o

../inc/%.o : ../inc/%.cxx 
	${CC} -c $< ${CFLAGS} -o $@ ${EXTRALDFLAGS} ${EXTRALIBS}

../inc/gps.o : ../inc/gps.c
	${CC} -c $< ${CFLAGS} -o $@ ${EXTRALDFLAGS} ${EXTRALIBS}

../inc/serial.o : ../inc/serial.c
	${CC} -c $< ${CFLAGS} -o $@ ${EXTRALDFLAGS} ${EXTRALIBS}

../inc/nmea.o : ../inc/nmea.c
	${CC} -c $< ${CFLAGS} -o $@ ${EXTRALDFLAGS} ${EXTRALIBS}

bin/imutest : imutest.cxx
	${CC} ${CFLAGS} $^ -o $@ ${EXTRALDFLAGS} ${EXTRALIBS} ${EXTRAINCDIRS} ${MYOBJS}

bin/sonartest : sonartest.cxx
	${CC} ${CFLAGS} $^ -o $@ ${EXTRALDFLAGS} ${EXTRALIBS} ${EXTRAINCDIRS} ${MYOBJS}

bin/ledtest : ledtest.cxx
	${CC} ${CFLAGS} $^ -o $@ ${EXTRALDFLAGS} ${EXTRALIBS} ${EXTRAINCDIRS} ${MYOBJS}

