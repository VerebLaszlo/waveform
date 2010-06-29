/**
 * @file LALSQTPNWaveformInterface.h
 *		Contains function declarations to integrate the SpinQuadTaylor code into the other parts of the LALSuit.
 * @author László Veréb
 * @date 2010.06.27.
 */

#ifndef LALSQTPNWAVEFORMINTERFACE_H
#define LALSQTPNWAVEFORMINTERFACE_H

#include <lal/Units.h>
#include <lal/LALInspiral.h>
#include <lal/SeqFactories.h>
#include <lal/LALStatusMacros.h>
#include <lal/GenerateInspiral.h>
#include <lal/LALSQTPNWaveform.h>

#ifdef __cplusplus
extern "C" {
#endif

NRCSID (LALSQTPNWAVEFORMINTERFACEH, "$Id LALSQTPNWaveformInterface.h$");

#define LALSQTPN_MSGPPNPARAMS "the PPNParamsStruct structure is null"
#define LALSQTPN_MSGINSPIRALTEMPLATE "the InspiralTemplate structure is null"
#define LALSQTPN_ZEROLENGTH 3
#define LALSQTPN_MSGZEROLENGTH "the given length is not positive"

/**		The function returns the generated waveform for injection.
 * @param[in,out]	status	: LAL universal status structure
 * @param[out]	wave_out	: structure containing the waveform
 * @param[in]	params		: structure containing the inspiral parameters
 * @param[in]	ppnParams	: parameters for restricted post-Newtonian waveform
 */
void LALSQTPNWaveformForInjection(LALStatus *status, CoherentGW *wave_out,
		InspiralTemplate *params, PPNParamStruc *ppnParams);

/**		The function allocates memory for the waveform.
 * @param[in,out]	status		: LAL universal status structure
 * @param[out]		waveform	: pointer to the allocated waveform
 * @param[in]		length		: the length of the waveform
 */
void LALSQTPNAllocateCoherentGW(LALStatus *status, CoherentGW *waveform, UINT4 length);

/**		The function deallocates memory of the waveform.
 * @param[in,out]	status		: LAL universal status structure
 * @param[out]		waveform	: pointer to the allocated waveform
 */
void LALSQTPNDestroyCoherentGW(LALStatus *status, CoherentGW *waveform);

/**		The function calculates the parameters from the InspiralTemplate
 * structure. <em>The used parameters are:</em>
 * <ul>
 * <li>masses \f$m_1\f$, \f$m_2\f$ in \f$M_\odot\f$</li>
 * <li>initial spin-parameter components in the radiation frame \f$\chi_{1j}\f$, \f$\chi_{2j}\f$; \f$j=x,y,z\f$ (for example \f$\chi_{1x}\f$ is the component of the compact object's spin-parameter having mass \f$m_1\f$)
 * <li>the flatness \f$w_1\f$, \f$w_2\f$</li>
 * <li>the inclination \f$\iota\f$ in \f$rad\f$
 * <li>the initial frequency \f$f_L\f$ in \f$Hz\f$</li>
 * <li>the distance \f$d\f$ in \f$Mps\f$</li>
 * <li>the sampling time \f$t_s\f$ in \f$s\f$</li>
 * <li>the order, see #LALPNOrder</li>
 * <li>binary code for the used spin-component, see #LALSQTPNChooseSpinComponent </li>
 * </ul><em>The calculated parameters:</em>
 *	\f{eqnarray}{
 *		\displaystyle M_{in}=m_1+m_2,\quad
 *		\mu=\frac{m_1m_2}{M_{in}},\quad
 *		\eta=\frac{\mu}{M_{in}},\\
 *		\chi_i=\sqrt{\sum_{j}\chi_{ij}^2},\quad
 *		\hat{\chi}_{ij}=\dfrac{\chi_{ij}}{\chi_i},\\
 *		f_s=t_s^{-1}\\
 *		A=\frac{4\cdot\eta M_{in}M_\odot\displaystyle\frac{G}{c^2}}{d\cdot3.0856775807\cdot10^{16}\cdot10^6}
 *	\f}
 * and the initial phase \f$\phi=0\f$
 * @param[in,out]	status	: LAL universal status structure
 * @param[out]	wave		: the used parameters
 * @param[in]		params	: the inspiral parameters
 * @param[in]	ppnParams	: parameters for restricted post-Newtonian waveform
 */
void LALSQTPNFillParams(LALStatus *status, LALSQTPNWaveformParams *wave,
		InspiralTemplate *params, PPNParamStruc *ppnParams);

#ifdef __cplusplus
}
#endif

#endif /* LALSQTPNWAVEFOMRINTERFACE_H */
