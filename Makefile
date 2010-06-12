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

SPIN=1		# 0 = SO, 1 = S1S2, 2 = SS, 4 = QM
RENORM=0	# 0 = NOT RENORMALIZED, 1 = RENORMALIZED
DEBUG=0		# 0 = non, 1 = yes
OTHER=Makefile util_debug.h
GCC=colorgcc -Wall -W -g3 #-O3
LIBS=-lm -lrt
GSL_LIB=-lgsl -lgslcblas
LAL_INC=-I/opt/lscsoft/lal/include -I/opt/lscsoft/lalinspiral/include -I/opt/lscsoft/lalmetaio/include
LAL_LIB=-L/opt/lscsoft/lal/lib/ -L/opt/lscsoft/lalinspiral/lib/ -llal -llalinspiral

all: own lal

lal : LALSTPNWaveformTestMod.c
	${GCC} ${LIBS} ${LAL_INC} ${LAL_LIB} -DDEBUG=${DEBUG} -o lal LALSTPNWaveformTestMod.c
	@echo ''

own : main.c LALSTPNQM_Waveform_Interface.o LALSTPNQM_Waveform.o LALSTPNQM_Integrator.o ${OTHER}
	${GCC} ${LIBS} ${LAL_INC} ${LAL_LIB} -DDEBUG=${DEBUG} -o own main.c LALSTPNQM_Waveform_Interface.o LALSTPNQM_Waveform.o LALSTPNQM_Integrator.o
	@echo ''

LALSTPNQM_Waveform_Interface.o : LALSTPNQM_Waveform_Interface.c LALSTPNQM_Waveform_Interface.h LALSTPNQM_Waveform.o LALSTPNQM_Integrator.o ${OTHER}
	${GCC} -c ${LIBS} ${LAL_INC} ${LAL_LIB} -DDEBUG=${DEBUG} LALSTPNQM_Waveform_Interface.c LALSTPNQM_Waveform.o LALSTPNQM_Integrator.o
	@echo ''

LALSTPNQM_Waveform.o : LALSTPNQM_Waveform.c LALSTPNQM_Waveform.h LALSTPNQM_Integrator.o ${OTHER}
	${GCC} -c ${LIBS} ${LAL_INC} ${LAL_LIB} ${GSL_LIB_F} -DDEBUG=${DEBUG} -DSPIN=${SPIN} -DRENORM=${RENORM} LALSTPNQM_Waveform.c LALSTPNQM_Integrator.o
	@echo ''

LALSTPNQM_Integrator.o : LALSTPNQM_Integrator.c LALSTPNQM_Integrator.h ${OTHER}
	${GCC} -c ${LIBS} ${LAL_INC} ${LAL_LIB} -DDEBUG=${DEBUG} LALSTPNQM_Integrator.c
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
	./lal `head -n 2 input.data | tail -n 1`
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
