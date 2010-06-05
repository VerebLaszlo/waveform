/**
 * @file waveform.h
 *		Containes the enums structs and functions declarations to create GWforms.
 * @author László Veréb
 * @date 2010.05.21.
 */

#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <math.h>

#include <lal/LALStatusMacros.h>
#include <lal/LALInspiral.h>
#include <lal/Units.h>

#include "util_math.h"
#include "util_debug.h"

NRCSID (WAVEFORMH, "$Id$");

/**		The structure contains the coefficients for calculating the derivatives
 * of the evolving quantities.
 */
typedef struct coefficients_Tag {
	REAL8 domega_Global; ///< global coefficient for domega
	REAL8 domega[LAL_PNORDER_PSEUDO_FOUR]; ///< coefficients for domega for every PN order
	REAL8 SO_Omega[2]; ///< the spin-orbit coefficients for domega
	REAL8 S1S2_Omega[2]; ///< the spin1-spin2 coefficients for domega
	REAL8 SS_Omega[2]; ///< the spin-selft coefficients for domega
	REAL8 SS_Omega_C; ///< the constant spin-selft coefficients for domega
	REAL8 QM_Omega[2]; ///< the quadropole-monopole coefficients for domega
	REAL8 QM_Omega_C; ///< the constant quadropole-monopole coefficients for domega
	REAL8 SO_Chih[2]; ///< the spin-orbit coefficients for dchih
	REAL8 S1S2_Chih[2]; ///< the spin1-spin2 coefficientd for dchih
	REAL8 QM_Chih[2]; ///< the quadropole-monopole coefficients for dchih
	REAL8 dLNh[2]; ///< coefficients for dLNh
	REAL8 MECO[8]; ///< coefficients for MECO-test
	REAL8 SO_MECO[2]; ///< spin-orbit coefficients for MECO
	REAL8 S1S2_MECO; ///< spin1-spin2 coefficients for MECO
	REAL8 QM_MECO; ///< quadropole-monopole coefficients for MECO
	REAL8 ln_coeff; ///< coefficient for the ln component in domega
} coefficients;

/**		The structure contains the system's and the generator's parameters.
 */
typedef struct waveform_Params_Tag {
	//@{@name mass-parameters
	REAL8 mass[2]; ///< masses of the BHs in \f$M_\odot\f$
	REAL8 total_Mass; ///< total mass in \f$M_\odot\f$
	REAL8 chirp_Mass; ///< chirp mass in \f$M_\odot\f$
	REAL8 mu; ///< reduced mass in \f$M_\odot\f$
	REAL8 eta; ///< symmetric mass ratio	//@}
	//@{@name spin-parameters
	REAL8 chi[2][3]; ///< components of the normalized spin
	REAL8 chih[2][3]; ///< components of the unity-vectors of the normalized spin
	REAL8 chi_Amp[2]; ///< amplitude of the normalized spin //@}
	//@{@name other system-parameters
	REAL8 flatness[2]; ///< flatness of the BHs or NSs
	REAL8 distance; ///< distance to the source in \f$Mps\f$
	REAL8 inclination; ///< inclination of the system \f$rad\f$
	REAL8 phi; ///< the initial phase (currently not in use)	//@}
	//@{@name other parametersa
	REAL8 signal_Amp; ///< the amplitude of the signal
	REAL8 lower_Freq; ///< the detectors sensitivityband's lower border in \f$Hz\f$
	//	REAL8 upper_Freq; ///< the detectors sensitivityband's lower border
	//	REAL8 cutoff_Freq; ///< the highest detectable frequency of the GW
	//	REAL8 chirp_Time; ///< \todo dokot írni
	REAL8 sampling_Freq; ///< sampling frequency in \f$Hz\f$
	REAL8 sampling_Time; ///< sampling time in \f$s\f$
	LALPNOrder order; ///< the Post_Newtonian order of the GW generation
	coefficients coeff; ///< coefficients for the deriving the parameters	//@}
} waveform_Params;

/**		The function fills the coefficients structure with the needed
 *	coefficients for generating the waveform up to the given PN-order.
 *
 *	The orders above 2PN are incomplete, so use them if you want to try their
 *	effects.
 * @param[in,out]	status	: LAL universal status structure
 * @param[in,out]	params	: the generator's parameters
 */
void fill_Coefficients(LALStatus *status, waveform_Params * const params);

/**		The function calculates the derived values.
 * The formulas are:
 *	\f{eqnarray}{
 *		\newcommand{\BM}[1]{\mbox{\boldmath$#1$}}
 *		\newcommand{\OM}[1]{\left(M\omega\right)^{#1/3}}
 *		\newcommand{\SPU}[2]{\left(\BM{\hat{#1}\hat{#2}}\right)}
 *		\newcommand{\VPU}[2]{\left(\BM{\hat{#1}\times\hat{#2}}\right)}
 *		\begin{array}{c}	
 *			\displaystyle\frac{d\BM{\hat{\chi_i}}}{d\left(t/M\right)}=
 *				{SO}_{\chi}^i\OM{5}+\left({S1S2}_{\chi}^i+{QM}_{\chi}^i\right)\OM{6}\\
 *			\displaystyle{SO}_{\chi}^i=\frac{\eta}{2}\left(4+3\frac{m_j}{m_i}\right)\VPU{L_N}{\chi_i};\quad
 *			{S1S2}_{\chi}^i=\frac{1}{2}\frac{\chi_jm_j^2}{M^2}\left[\BM{\hat{\chi_j}}
 *				-3\SPU{L_N}{\chi_j}\BM{\hat{L_N}}\right]\times\BM{\hat{\chi_i}};\quad
 *			{QM}_{\chi}^i=-\frac{3}{2}\eta\chi_iw_i\SPU{L_N}{\chi_i}\VPU{L_N}{\chi_i}
 *		\end{array}\\
 *		\newcommand{\BM}[1]{\mbox{\boldmath$#1$}}
 *		\frac{d\BM{\hat{L_N}}}{d\left(t/M\right)}=\sum_{i}-
 *			\frac{1}{\eta}\frac{\chi_im_i^2}{M^2}\frac{d\BM{\hat{\chi_i}}}
 *			{d\left(t/M\right)}\\
 *		\newcommand{\BM}[1]{\mbox{\boldmath$#1$}}
 *		\newcommand{\OM}[1]{\left(M\omega\right)^{#1/3}}
 *		\newcommand{\SPU}[2]{\left(\BM{\hat{#1}\hat{#2}}\right)}
 *		\begin{array}{c}
 *			\displaystyle\frac{d\left(M\omega\right)}{d\left(t/M\right)}=
 *				\frac{96\eta}{5}\OM{11}
 *			\left[
 *				1-\frac{743+924\eta}{336}\OM{2}+\left(4\pi
 *					+SO_{\omega}\right)\OM{3}+\left(\frac{34103}{18144}+
 *					\frac{13661}{2016}\eta+\frac{59}{18}\eta^2+
 *					S1S2_{\omega}+Si^2_{\omega}+QM_{\omega}\right)\OM{4}
 *			\right]\\
 *			\displaystyle SO_{\omega}=\sum_{i\ne j}-
 *				\frac{1}{12}\frac{\chi_im_i^2}{M^2}
 *				\left(113+75\frac{m_j}{m_i}\right)\SPU{L_N}{\chi_i};\quad
 *			S1S2_{\omega}=\frac{\eta\chi_1\chi_2}{48}
 *				\left[721\SPU{L_N}{\chi_1}\SPU{L_N}{\chi_2}-247\SPU{\chi_1}{\chi_2}\right]\\
 *			\displaystyle SS_{\omega}=\sum_{i}\frac{1}{96}\frac{\chi_im_i}{M^2}
 *			\chi_i\left[7-\SPU{L_N}{\chi_i}^2\right];\quad
 *			QM_{\omega}=\sum_{i}\frac{5}{2}\frac{\chi_im_i^2}{M^2}\chi_iw_i
 *				\left[3\SPU{L_N}{\chi_i}-1\right]
 *		\end{array}\\
 *		\newcommand{\BM}[1]{\mbox{\boldmath$#1$}}
 *		\newcommand{\OM}[1]{\left(M\omega\right)^{#1/3}}
 *		\newcommand{\SP}[2]{\left(\BM{#1}\BM{#2}\right)}
 *		\begin{array}{c}
 *			\displaystyle MECO=-5\eta\frac{2}{3}\OM{-1}+\left(5\eta\frac{4}{3}
 *				\frac{9+\eta}{12}\right)\OM{1}+SO_{MECO}\OM{2}+
 *				\left(5\eta\frac{6}{3}\frac{81-57\eta+\eta^2}{24}+
 *				S1S2_{MECO}+QM_{MECO}\right)\OM{3}\\
 *			\displaystyle
 *			SO_{MECO}=\sum_{i}-\frac{5}{9}\eta\frac{\chi_im_i^2}{M^2}
 *				\left(4+3\frac{m_j}{m_i}\right)\SP{\hat{L}_N}{\hat{\chi}_i};\quad
 *			S1S2_{MECO}=-\frac{\chi_1m_1^2}{M^2}\frac{\chi_2m_2^2}{M^2}
 *				\left[\SP{\hat{\chi}_1}{\hat{\chi}_2}-3\SP{\hat{L}_N}
 *				{\hat{\chi}_1}\SP{\hat{L}_N}{\hat{\chi}_2}\right];\quad
 *			QM_{MECO}=2\eta QM_{\omega}
 *		\end{array}
 *	\f}
 *	The constant parts are calculated by the fill_Coefficients() function and
 *	then used in this function applying the dynamic parts: the products of the
 *	vectors and the powers of the \f$M\omega\f$.
 *
 *	The orders above 2PN are incomplete, so use them if you want to try their
 *	effects.
 * @param[in]	t		: evolution time, not in used
 * @param[in]	values	: the values to be derivated
 * @param[out]	dvalues	: the derivated values and the last element is the MECO
 * @param[in]	params	: the generator's parameters
 */
int derivator(REAL8 t, const REAL8 values[], REAL8 dvalues[], void * params);
/**		Enumeration to index the dynamic variables in the generator function.
 */
typedef enum {
	PHASE,
	OMEGA,
	LNH_1,
	LNH_2,
	LNH_3,
	CHIH1_1,
	CHIH1_2,
	CHIH1_3,
	CHIH2_1,
	CHIH2_2,
	CHIH2_3,
	MECO,
	NUM_OF_VAR
} generator_variables;

/**		The structure's contents are the frequency in the freq->data array and
 *	one of the following:
 *	1. the \f$h_+\f$ components in the odd numbered h->data->data member and
 *	\f$h_\times\f$ componets in the even numbered h->data->data member.
 *	2. the \f$a_+\f$ amplitude components in the odd numbered a->data->data members and
 *	\f$a_\times\f$ amplitude componets in the even numbered a->data->data
 *	members. And also the phase function in the phase->data, and the
 *	polarization shift in pol->data members.
 *	3. simultaneously the 1. and 2. options.
 */
typedef struct waveform_Tag {
	size_t length; ///< the length of the vectors, but half the length of the h and a vectors
	REAL4TimeVectorSeries *h; ///< the wave components
	REAL4TimeVectorSeries *a; ///< the amplitude components
	REAL8Vector *phase; ///< the phase function
	REAL4Vector *pol; ///< the polarization shift
	REAL4Vector *freq; ///< the frequency
} waveform;

/**		The function generates the waveform.
 * @param[in,out]	status		: LAL universal status structure
 * @param[in]		params		: the input parameters
 * @param[out]		waveform	: the generated waveform
 */
void
generator(LALStatus *status, waveform_Params *params, CoherentGW *waveform);

#endif /* WAVEFORM_H */
