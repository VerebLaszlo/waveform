//#include <lal/LALStdlib.h>
//#include <lal/LALInspiral.h>
//#include <lal/GeneratePPNInspiral.h>
#include <lal/GenerateInspiral.h>
//#include <lal/SeqFactories.h>
//#include <lal/Units.h>
#include "util_debug.h"
#include "waveform_interface.h"

NRCSID(MAINC, "$Id: main.c,v 1.1 2004/05/05 20:06:23 thomas Exp");

extern REAL8 lapultsag[2];

int main(int argc, char *argv[]) {
	static LALStatus mystatus;
	CoherentGW thewaveform;
	SimInspiralTable injParams;
	PPNParamStruc ppnParams;
	const char *filename = argv[17];//"wave1.dat";
	FILE *outputfile;
	INT4 i, length;
	REAL8 dt;
	//REAL8       a1, a2, phi, shift, phi0;
	char PNString[50];

	if (argc != 18) {
		printf(
				"                         1  2  3   4   5   6   7   8   9     10    11   12      13      14       15 16      17\n");
		printf(
				"Correct parameter order: m1 m2 S1x S1y S1z S2x S2y S2z flat1 flat2 incl f_lower f_final distance dt PNorder outputfile\n");
		return (1);
	}

	memset(&mystatus, 0, sizeof(LALStatus));
	memset(&thewaveform, 0, sizeof(CoherentGW));
	memset(&injParams, 0, sizeof(SimInspiralTable));
	memset(&ppnParams, 0, sizeof(PPNParamStruc));

	/* --- first we fill the SimInspiral structure --- */
	injParams.mass1 = atof(argv[1]);//10.;
	injParams.mass2 = atof(argv[2]);//10.;
	injParams.spin1x = atof(argv[3]);//0.1;
	injParams.spin1y = atof(argv[4]);//0.2;
	injParams.spin1z = atof(argv[5]);//0.3;
	injParams.spin2x = atof(argv[6]);//0.4;
	injParams.spin2y = atof(argv[7]);//0.5;
	injParams.spin2z = atof(argv[8]);//0.6;
	lapultsag[0] = atof(argv[9]);
	lapultsag[1] = atof(argv[10]);
	injParams.inclination = atof(argv[11]);//0.8;

	/* MV-20060224: I believe this is not used in the SpinTaylor code! */
	injParams.f_lower = atof(argv[12]);//40;
	injParams.f_final = atof(argv[13]);//500.0;

	/* this is given in Mpc */
	injParams.distance = atof(argv[14]);//100.;
	ppnParams.deltaT = atof(argv[15]);//1.0 / 16384;//4096.0;
	injParams.polarization = 0;
	sprintf(PNString, "SpinTaylor%s", argv[16]);
	LALSnprintf(injParams.waveform, LIGOMETA_WAVEFORM_MAX * sizeof(CHAR),
			PNString);//"SpinTaylortwoPN");

	//fprintf(stderr, "Lower cut-off frequency used will be %fHz\n", injParams.f_lower);
	InspiralTemplate inspiralParams; /* structure for inspiral package */
	//	CHAR              warnMsg[1024];
	/* --- now we can call the injection function --- read the event waveform approximant and order */

	//INITSTATUS(&mystatus, "main", MAINC);
	//ATTATCHSTATUSPTR(&mystatus);
	LALGetApproximantFromString(&mystatus, injParams.waveform,
			&inspiralParams.approximant);
	//CHECKSTATUSPTR(&mystatus);

	LALGetOrderFromString(&mystatus, injParams.waveform, &inspiralParams.order);
	//CHECKSTATUSPTR(&mystatus);

	/* We fill ppnParams */
	LALGenerateInspiralPopulatePPN(&mystatus, &ppnParams, &injParams);
	//CHECKSTATUSPTR(&mystatus);

	/* we fill inspiralParams structure as well.*/
	LALGenerateInspiralPopulateInspiral(&mystatus, &inspiralParams, &injParams,
			&ppnParams);
	//CHECKSTATUSPTR(&mystatus);

	// nem akar működni
	//ERR_STR_END("interface start");
	/* the waveform generation itself */
	interface(&mystatus, &thewaveform, &inspiralParams, &ppnParams);
	//CHECKSTATUSPTR(&mystatus);
	// ez sem akar működni
	ERR_STR_END("interface end");
	/* we populate the simInspiral table with the fFinal needed for
	 template normalisation. */
	injParams.f_final = inspiralParams.fFinal;

	if (mystatus.statusCode) {
		fprintf(stderr, "LALSTPNWaveformTest: error generating waveform\n");
		exit(1);
	}
	/* --- and finally save in a file --- */

	outputfile = fopen(filename, "w");

	length = thewaveform.phi->data->length;
	dt = thewaveform.phi->deltaT;
	//phi0    = thewaveform.phi->data->data[0];
	for (i = 0; i < length; i++) {
		/*a1  = thewaveform.a->data->data[2*i];
		 a2  = thewaveform.a->data->data[2*i+1];
		 phi     = thewaveform.phi->data->data[i] - phi0;
		 shift   = thewaveform.shift->data->data[i];

		 fprintf(outputfile,"%e\t%e\t%e\n",
		 i*dt,
		 a1*cos(shift)*cos(phi) - a2*sin(shift)*sin(phi),
		 a1*sin(shift)*cos(phi) + a2*cos(shift)*sin(phi));*/
		fprintf(outputfile, "%e\t%e\t%e\n", i * dt, thewaveform.h->data->data[2
				* i], thewaveform.h->data->data[2 * i + 1]);
	}
	//    fclose(outputfile);
	//fprintf(stdout,"waveform saved in %s\n",argv[16]);
	ERR_STR_END("Done.");
	return 0;
}

