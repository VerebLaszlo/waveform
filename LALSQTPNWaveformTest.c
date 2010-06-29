/**
 * @file LALSQTPNWaveformTest.c
 *		The user interface for the SpinQuadTaylor program.
 * This file is an example howto use the SpinQuadTaylor program.
 * The input parameters are:<br/>
 * <em>Important parameters:</em>
 *	<ul>
 *	<li>masses of the BHs (or NSs) \f$m_1\f$, \f$m_2\f$ in \f$M_\odot\f$</li>
 *	<li>the spin components \f$\chi_{1x}\f$, \f$\chi_{1y}\f$, \f$\chi_{1z}\f$, \f$\chi_{2x}\f$,
 *	\f$\chi_{2y}\f$, \f$\chi_{2z}\f$, the values of \f$\chi_1\f$, \f$\chi_2\f$ are between 0 and 1</li>
 *	<li>the flatness \f$w_1\f$, \f$w_2\f$, for BHs are 1</li>
 *	<li>the inclination \f$\iota\f$ in \f$rad\f$
 *	<li>the initial frequency \f$f_L\f$ in \f$Hz\f$</li>
 *	<li>the distance \f$d\f$ in \f$Mps\f$</li>
 *	<li>the sampling time \f$t_s\f$ in \f$s\f$</li>
 *	<li>the PN order in string format</li>
 *	<li>and the name of the output file</li>
 *	</ul>
 *	<em>optional parameters are:</em>
 *	<ul>
 *	<li>binary code for the used spin-components
 *		<ul>
 *		<li>spin-orbit component: 1</li>
 *		<li>spin-spin component: 2</li>
 *		<li>spin-self component: 4</li>
 *		<li>quadropole-monopole component: 8</li>
 *		<li>and the sum of these, spin-orbit + spin-spin component: 3 like in the
 *		SpinTaylor program.</li>
 *		</ul>
 *	</li>
 *	<li>debug level</li>
 *	</ul>
 *	The output file contains three coloums, the first is the elapsed time, the
 *	second and the third are the \f$h_+\f$ and \f$h_x\f$ polarized GWforms.
 *	\f{center}
 *	\begin{gather*}
 *		h_+=a_1\cos\left(2\alpha\right)\cos\left(2\Phi\right)-a_2\sin\left(2\alpha\right)\sin\left(2\Phi\right),\\
 *		h_x=a_1\sin\left(2\alpha\right)\cos\left(2\Phi\right)+a_2\cos\left(2\alpha\right)\sin\left(2\Phi\right)
 *		\end{gather*}
 *	\f}where \f$a_1\f$, \f$a_2\f$ are the amplitude, \f$\alpha\f$ are the polarization shift, and \f$\Phi\f$ is the phase (Eq. (4.28)-(4.30) of [1] upto leading order. The \f$\Phi\f$ is shifted by \f$\pi\f$ with respect to [1]. We note that \f$\Theta=0\f$ in leading order because we use radiation frame).<br />
 *	<b>References</b><br />
 *	[1] L. E. Kidder, Phys.Rev.D 52, 821 (1995) 
 * @author László Veréb
 * @date 2010.06.27.
 */

#include <lal/GenerateInspiral.h>
#include <lal/LALSQTPNWaveformInterface.h>

NRCSID(LALSQTPNWAVEFORMTESTC, "$Id: LALSQTPNWaveformTest.c,v 0.1 2010/05/21");

int lalDebugLevel = 0;	///< the debug level

/** The main program.
 */
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
	LALSQTPNChooseSpinComponent spin_Component = LALSQTPN_AllComp;

	if (argc == 20) {
		lalDebugLevel = atoi(argv[19]);
		spin_Component = atoi(argv[18]);
	} else if (argc == 19) {
		spin_Component = atoi(argv[18]);
	} else if (argc != 18) {
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
	injParams.flatness[0] = atof(argv[9]);
	injParams.flatness[1] = atof(argv[10]);
	injParams.inclination = atof(argv[11]);
	injParams.f_lower = atof(argv[12]);
	injParams.f_final = atof(argv[13]);
	injParams.distance = atof(argv[14]);
	ppnParams.deltaT = atof(argv[15]);
	injParams.polarization = 0;
	injParams.settings = &spin_Component;
	sprintf(PNString, "SpinQuadTaylor%s", argv[16]);

	LALSnprintf(injParams.waveform, LIGOMETA_WAVEFORM_MAX * sizeof(CHAR),
			PNString);

	//interface(&mystatus, &thewaveform, &injParams, &ppnParams);
	LALGenerateInspiral(&mystatus, &thewaveform, &injParams, &ppnParams);
	if (mystatus.statusCode && (lalDebugLevel > 0)) {
		fprintf(stderr, "Error[0] 1: program %s, file %s, line %i, %s\n"
			"         Function interface() failed\n", argv[0], __FILE__,
				__LINE__, LALSQTPNWAVEFORMTESTC);
		LALSQTPNDestroyCoherentGW(&mystatus, &thewaveform);
		return mystatus.statusCode;
	}
	// and finally save in a file
	outputfile = fopen(filename, "w");

	length = thewaveform.f->data->length;
	dt      = thewaveform.phi->deltaT;
    REAL8       a1, a2, phi, shift;
    for(i = 0; i < length; i++) {
        a1  = thewaveform.a->data->data[2*i];
        a2  = thewaveform.a->data->data[2*i+1];
        phi     = thewaveform.phi->data->data[i] - thewaveform.phi->data->data[0];
        shift   = thewaveform.shift->data->data[i];

        //fprintf(outputfile,"%e\t%e\t%e\n",
        fprintf(outputfile,"% 10.7e\t% 10.7e\t% 10.7e\n",
            i*dt,
            a1*cos(shift)*cos(phi) - a2*sin(shift)*sin(phi),
            a1*sin(shift)*cos(phi) + a2*cos(shift)*sin(phi));
    }
	LALSQTPNDestroyCoherentGW(&mystatus, &thewaveform);
	//    fclose(outputfile);
	puts("Done.");
	LALCheckMemoryLeaks();
	return mystatus.statusCode;
}

