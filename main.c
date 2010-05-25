/**
 * @file main.c
 *		The user interface.
 * @author László Veréb
 * @date 2010.05.21.
 * \todo	egy h-a választó függvényt írni, amelyik nem kell, 0-ra állítja
 * \todo	egy memóriafoglaló függvény ami az előbbi alapján működik
 * ======================
 * \todo	beolvasztani a lal-ba
 */

#include <lal/GenerateInspiral.h>

#include "util_debug.h"
#include "waveform_interface.h"

NRCSID(MAINC, "$Id: main.c,v 0.1 2010/05/21");

extern REAL8 lapultsag[2]; ///< \bug be kell rakni a paraméterekbe

int main(int argc, char *argv[]) {

	// variable declaration and initialization
	static LALStatus mystatus;
	CoherentGW thewaveform;
	SimInspiralTable injParams;
	PPNParamStruc ppnParams;
	const char *filename = argv[17];
	FILE *outputfile;
	INT4 i, length;
	REAL8 dt;
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

	//	setting the parameters
	injParams.mass1 = atof(argv[1]);
	injParams.mass2 = atof(argv[2]);
	injParams.spin1x = atof(argv[3]);
	injParams.spin1y = atof(argv[4]);
	injParams.spin1z = atof(argv[5]);
	injParams.spin2x = atof(argv[6]);
	injParams.spin2y = atof(argv[7]);
	injParams.spin2z = atof(argv[8]);
	lapultsag[0] = atof(argv[9]);
	lapultsag[1] = atof(argv[10]);
	injParams.inclination = atof(argv[11]);
	injParams.f_lower = atof(argv[12]);
	injParams.f_final = atof(argv[13]);
	injParams.distance = atof(argv[14]);
	ppnParams.deltaT = atof(argv[15]);
	injParams.polarization = 0;
	sprintf(PNString, "SpinTaylor%s", argv[16]); ///< \todo ezt a sajátunkra kell átírni

	LALSnprintf(injParams.waveform, LIGOMETA_WAVEFORM_MAX * sizeof(CHAR),
			PNString);

	choose_CoherentGW_Component(&mystatus, 3, &thewaveform);

	/*************************************************************************/
	/******************** ez majd nem kell a végső kódban ********************/
	InspiralTemplate inspiralParams; // structure for inspiral package

	LALGetApproximantFromString(&mystatus, injParams.waveform,
			&inspiralParams.approximant);

	LALGetOrderFromString(&mystatus, injParams.waveform, &inspiralParams.order);

	/* We fill ppnParams */
	LALGenerateInspiralPopulatePPN(&mystatus, &ppnParams, &injParams);

	/* we fill inspiralParams structure as well.*/
	LALGenerateInspiralPopulateInspiral(&mystatus, &inspiralParams, &injParams,
			&ppnParams);

	ERR_STR_END("interface start");
	/* the waveform generation itself */
	interface(&mystatus, &thewaveform, &inspiralParams, &ppnParams);
	ERR_STR_END("interface end");
	/* we populate the simInspiral table with the fFinal needed for
	 template normalisation. */
	injParams.f_final = inspiralParams.fFinal;
	/******************************** eddig extra *****************************/
	/**************************************************************************/

	if (mystatus.statusCode) {
		fprintf(stderr, "LALSTPNWaveformTest: error generating waveform\n");
		exit(1);
	}
	/* --- and finally save in a file --- */

	outputfile = fopen(filename, "w");

	length = thewaveform.f->data->length;
	dt = thewaveform.phi->deltaT;
	for (i = 0; i < length; i++) {
		fprintf(outputfile, "%e\t%e\t%e\n", i * dt, thewaveform.h->data->data[2
				* i], thewaveform.h->data->data[2 * i + 1]);
	}
	//    fclose(outputfile);
	ERR_STR_END("Done.");
	return 0;
}

