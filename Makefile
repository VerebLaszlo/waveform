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
#

####################################
#	debugot is írni
####################################
SPIN=1	# 0 = SO, 1 = S1S2, 2 = SS, 4 = QM
OLD=0	# 1 = LALSTPN, 0 = LALSTPN + SS + QM
DEBUG=0	# 0 = non, 1 = yes
OTHERS=Makefile util_debug.h
ALL_F=colorgcc -c -Wall
#LAL_F=-I/opt/lscsoft/lal/include -L/opt/lscsoft/lal/lib/ -llal 
GSL_LIB_F=-lgsl -lgslcblas
LAL_INC_F=-I/opt/lscsoft/lal/include -I/opt/lscsoft/lalinspiral/include -I/opt/lscsoft/lalmetaio/include
LAL_LIB_F=-L/opt/lscsoft/lal/lib/ -L/opt/lscsoft/lalinspiral/lib/ -llal -llalinspiral

all: own lal

lal : LALSTPNWaveformTestMod.c
	gcc -Wall ${LAL_F} ${LAL_INC_F} ${LAL_LIB_F} -DDEBUG=${DEBUG} -o lal LALSTPNWaveformTestMod.c
	@echo ''

own : main.c waveform_interface.o waveform.o util_math.o integrator.o ${OTHERS}
	gcc -Wall ${LAL_F} ${LAL_INC_F} ${LAL_LIB_F} -DDEBUG=${DEBUG} -o own main.c waveform_interface.o waveform.o util_math.o integrator.o
	@echo ''

waveform_interface.o : waveform_interface.c waveform_interface.h waveform.o integrator.o ${OTHERS}
	${ALL_F} ${LAL_INC_F} ${LAL_LIB_F} -DDEBUG=${DEBUG} waveform_interface.c waveform.o integrator.o
	@echo ''

waveform.o : waveform.c waveform.h util_math.o integrator.o ${OTHERS}
	${ALL_F} ${LAL_INC_F} ${LAL_LIB_F} -DDEBUG=${DEBUG} -DOLD=${OLD} -DSPIN=${SPIN} ${GSL_LIB_F} waveform.c util_math.o integrator.o  -lm
	@echo ''

integrator.o : integrator.c integrator.h ${OTHERS}
	${ALL_F} ${LAL_INC_F} ${LAL_LIB_F} -DDEBUG=${DEBUG} integrator.c -lm
	@echo ''

util_math.o : util_math.c util_math.h ${OTHERS}
	${ALL_F} util_math.c -lm
	@echo ''

clean_run:
	rm -rf lal own
	@echo ''

clean_all :
	make clean_run
	make clean
	@echo ''

clean :
	rm -rf *.o *.out *.b
	@echo ''

run :
	./own `tail -n 1 input.data`
	./lal `head -n 2 input.data | tail -n 1 `
	@echo ''

help :
	@echo 'all       : makes everything'
	@echo 'lal       : makes just the LALSTPNWaveform.c part'
	@echo 'own       : makes the whole own code'
	@echo 'clean     : deletes the object files'
	@echo 'clean_run : deletes the exe files'
	@echo 'clean_all : invokes the "clean" and "clean_run" commands'
	@echo 'run       : runs the two programs'
	@echo 'help      : prints this message'
	@echo ''

.PHONY: all clean clean_all run help
