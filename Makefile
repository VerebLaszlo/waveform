# Makefile
#############################################################################
#		LALInspiral instalálása												#
#	1. instalálld fel a lal csomagot követve a képernyő és a				#
#	https://www.lsc-group.phys.uwm.edu/daswg/docs/howto/lal-install.html	#
#	oldalon lévő utasításokat.												#
#	2. tölts le és instalálld fel a metaio csomagot
#	3. instalálld fel a lalmetaio csomagot hasonlóan						#
#	4. végül a lalinspiral csomagot											#
#	5. ha nem találja a könyvtárakat, add hozzá az útvonalakat, az			#
#	/etc/ld.so.conf fájlhoz, és hajtsd végra a ldconfig parancsot.			#
#############################################################################

RENORM=0 # 0,1
BUILD_TYPE=normal# debug, normal, prod
CFLAGS=#ddd
ifeq (${BUILD_TYPE},debug)
	CFLAGS=-Wall -W -g3
	DEBUG=1
else
ifeq (${BUILD_TYPE},prod)
	CFLAGS=-O3
	DEBUG=0
else
	CFLAGS=-Wall -W -g3
	DEBUG=0
endif
endif

CC=colorgcc -c
GSL_LIB=-lgsl -lgslcblas
LAL_INC=-I/opt/lscsoft/lal/include -I/opt/lscsoft/lalinspiral/include -I/opt/lscsoft/lalmetaio/include
LAL_LIB=-L/opt/lscsoft/lal/lib/ -L/opt/lscsoft/lalinspiral/lib/ -llal -llalinspiral
OBJ=LALSQTPNWaveformInterface.o LALSQTPNWaveform.o LALSQTPNIntegrator.o

all: own lal

lal: LALSTPNWaveformTestMod.c
	colorgcc ${CFLAGS} ${LAL_INC} ${LAL_LIB} -o lal LALSTPNWaveformTestMod.c -lm
	@echo ''
 
own: LALSQTPNWaveformTest.c ${OBJ}
	colorgcc ${CFLAGS} ${LAL_INC} ${LAL_LIB} -DDEBUG=${DEBUG} -o own LALSQTPNWaveformTest.c ${OBJ}
	@echo ''

LALSQTPNWaveformInterface.o: LALSQTPNWaveformInterface.c LALSQTPNWaveformInterface.h LALSQTPNWaveform.o
	${CC} ${CFLAGS} ${LIBS} ${LAL_INC} ${LAL_LIB} -DDEBUG=${DEBUG} LALSQTPNWaveformInterface.c -lm
	@echo ''

LALSQTPNWaveform.o: LALSQTPNWaveform.c LALSQTPNWaveform.h LALSQTPNIntegrator.o
	${CC} ${CFLAGS} ${LIBS} ${LAL_INC} ${LAL_LIB} ${GSL_LIB} -DDEBUG=${DEBUG} -DRENORM=${RENORM} LALSQTPNWaveform.c
	@echo ''

LALSQTPNIntegrator.o: LALSQTPNIntegrator.c LALSQTPNIntegrator.h
	${CC} ${CFLAGS} ${LIBS} ${LAL_INC} ${LAL_LIB} -DDEBUG=${DEBUG} LALSQTPNIntegrator.c
	@echo ''

clean:
	rm -rf *.o *.out *.b
	@echo ''

cleanrun:
	rm -rf lal own
	@echo ''

cleanall:
	make clean
	make cleanrun
	@echo ''

run :
	./own `tail -n 1 input.data`
	./lal `head -n 2 input.data | tail -n 1`
	@echo ''

help :
	@echo 'all       : makes everything'
	@echo 'lal       : makes just the LALSTPNWaveform.c part'
	@echo 'own       : makes the whole own code'
	@echo 'clean     : deletes the object files'
	@echo 'cleanrun : deletes the exe files'
	@echo 'cleanall : invokes the "clean" and "cleanrun" commands'
	@echo 'run       : runs the two programs'
	@echo 'help      : prints this message'
	@echo ''

.PHONY: all clean cleanall cleanrun run help
