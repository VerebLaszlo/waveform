/**
 * @file LALSQTPNWaveform.h
 *		Contains the enumerations, structures and functions declarations to create GWforms.
 * @author László Veréb
 * @date 2010.05.21.
 */

#ifndef LALSQTPNWAVEFORM_H
#define LALSQTPNWAVEFORM_H

#include <math.h>

#include <lal/LALStatusMacros.h>
#include <lal/LALInspiral.h>
#include <lal/Units.h>

#include "util_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

NRCSID (LALSQTPNWAVEFORMH, "$Id$ LALSQTPNWaveform.h");

/**		Enumeration to specify which component will be used in the waveform
 * generation. Their combination also can be used by the bitwise or.
 **/
typedef enum {
	LALSQTPN_SOComp = 1, ///< Spin-orbit component
	LALSQTPN_SSComp = 2, ///< Spin-spin component
	LALSQTPN_SSselfComp = 4, ///< Spin-spin-self component
	LALSQTPN_QMComp = 8, ///< quadropole-monopole component
	LALSQTPN_ErrorComp = 16, ///< for error reporting
	LALSQTPN_LALSTPNComp = 3
///< combined: spin-orbit and spin-spin component together, like the LALSTPNWaveform
} LALSQTPNChooseSpinComponent;

/**		The structure contains the coefficients for calculating the derivatives
 * of the evolving quantities.
 */
typedef struct tagLALSQTPNCoefficients{
	///@name coefficients for domega
	//@{
	REAL8 domegaGlobal; ///< global coefficient for domega
	REAL8 domega[LAL_PNORDER_PSEUDO_FOUR]; ///< coefficients for domega for every PN order
	REAL8 domegaSO[2]; ///< the spin-orbit coefficients for domega
	REAL8 domegaSS[2]; ///< the spin1-spin2 coefficients for domega
	REAL8 domegaSSself[2]; ///< the spin-selft coefficients for domega
	REAL8 domegaSSselfConst; ///< the constant spin-selft coefficients for domega
	REAL8 domegaQM[2]; ///< the quadropole-monopole coefficients for domega
	REAL8 domegaQMConst; ///< the constant quadropole-monopole coefficients for domega
	REAL8 domegaLN; ///< coefficient for the ln component in domega
	//@}
	///@name coefficients for dchih and dLNh
	REAL8 dchihSO[2]; ///< the spin-orbit coefficients for dchih
	REAL8 dchihSS[2]; ///< the spin1-spin2 coefficientd for dchih
	REAL8 dchihQM[2]; ///< the quadropole-monopole coefficients for dchih
	REAL8 dLNh[2]; ///< coefficients for dLNh
	//@}
	///@name coefficients for MECO
	//@{
	REAL8 meco[8]; ///< coefficients for MECO-test
	REAL8 mecoSO[2]; ///< spin-orbit coefficients for MECO
	REAL8 mecoSS; ///< spin1-spin2 coefficients for MECO
	REAL8 mecoQM; ///< quadropole-monopole coefficients for MECO
	//@}
} LALSQTPNCoefficients;

/**		The structure contains the system's and the generator's parameters.
 */
typedef struct tagLALSQTPNWaveformParams{
	///@name mass-Parameters
	//@{
	REAL8 mass[2]; ///< masses of the BHs in \f$M_\odot\f$
	REAL8 totalMass; ///< total mass in \f$M_\odot\f$
	REAL8 chirpMass; ///< chirp mass in \f$M_\odot\f$
	REAL8 mu; ///< reduced mass in \f$M_\odot\f$
	REAL8 eta; ///< symmetric mass ratio
	//@}
	///@name spin-parameters
	//@{
	REAL8 chi[2][3]; ///< components of the normalized spin
	REAL8 chih[2][3]; ///< components of the unity-vectors of the normalized spin
	REAL8 chiAmp[2]; ///< amplitude of the normalized spin
	//@}
	///@name other system-parameters
	//@{
	REAL8 flatness[2]; ///< flatness of the BHs or NSs
	REAL8 distance; ///< distance to the source in \f$Mps\f$
	REAL8 inclination; ///< inclination of the system \f$rad\f$
	REAL8 phi; ///< the initial phase (currently not in use)
	//@}
	///@name other parameters
	//@{
	REAL8 signalAmp; ///< the amplitude of the signal
	REAL8 lowerFreq; ///< the detectors sensitivityband's lower border in \f$Hz\f$
	REAL8 samplingFreq; ///< sampling frequency in \f$Hz\f$
	REAL8 samplingTime; ///< sampling time in \f$s\f$
	LALPNOrder order; ///< the Post_Newtonian order of the GW generation
	LALSQTPNChooseSpinComponent spinComponent; ///< which spin components will be included in the generation
	LALSQTPNCoefficients coeff; ///< coefficients for the deriving the parameters
	//@}
} LALSQTPNWaveformParams;

/**		The function fills the LALSQTPNCoefficients structure with the needed
 *	coefficients for generating the waveform up to the given PN-order.
 *
 *		The orders above 2PN are incomplete, so use them if you want to try their
 *	effects.
 * @param[in,out]	status	: LAL universal status structure
 * @param[in,out]	params	: the LALSQTPN_Generator's parameters
 */
void LALSQTPNFillCoefficients(LALStatus *status, LALSQTPNWaveformParams * const params);

/**		The function calculates the derived values.
 * The formulas are:
 *	\f{eqnarray}{
 *		\newcommand{\BM}[1]{\mbox{\boldmath$#1$}}
 *		\newcommand{\OM}[1]{\left(M\omega\right)^{#1/3}}
 *		\newcommand{\SPU}[2]{\left(\BM{\hat{#1}\hat{#2}}\right)}
 *		\newcommand{\VPU}[2]{\left(\BM{\hat{#1}\times\hat{#2}}\right)}
 *		\begin{array}{c}	
 *			\displaystyle\frac{d\BM{\hat{\chi_i}}}{d\left(t/M\right)}=
 *				{SO}_{\chi}^i\OM{5}+\left({SS}_{\chi}^i+{QM}_{\chi}^i\right)\OM{6}\\
 *			\displaystyle{SO}_{\chi}^i=\frac{\eta}{2}\left(4+3\frac{m_j}{m_i}\right)\VPU{L_N}{\chi_i};\quad
 *			{SS}_{\chi}^i=\frac{1}{2}\frac{\chi_jm_j^2}{M^2}\left[\BM{\hat{\chi_j}}
 *				-3\SPU{L_N}{\chi_j}\BM{\hat{L_N}}\right]\times\BM{\hat{\chi_i}}\\
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
 *					+SO_{\omega}\right)\OM{3}+
 *			\right.\\
 *			\left.\left(\displaystyle\frac{34103}{18144}+
 *					\frac{13661}{2016}\eta+\frac{59}{18}\eta^2+
 *					SS_{\omega}+SSself_{\omega}+QM_{\omega}\right)\OM{4}
 *			\right]\\
 *			\displaystyle SO_{\omega}=\sum_{i\ne j}-
 *				\frac{1}{12}\frac{\chi_im_i^2}{M^2}
 *				\left(113+75\frac{m_j}{m_i}\right)\SPU{L_N}{\chi_i};\quad
 *			SS_{\omega}=\frac{\eta\chi_1\chi_2}{48}
 *				\left[721\SPU{L_N}{\chi_1}\SPU{L_N}{\chi_2}-247\SPU{\chi_1}{\chi_2}\right]\\
 *			\displaystyle SSself_{\omega}=\sum_{i}\frac{1}{96}\frac{\chi_im_i}{M^2}
 *			\chi_i\left[7-\SPU{L_N}{\chi_i}^2\right];\quad
 *			QM_{\omega}=\sum_{i}\frac{5}{2}\frac{\chi_im_i^2}{M^2}\chi_iw_i
 *				\left[3\SPU{L_N}{\chi_i}-1\right]
 *		\end{array}\\
 *		\newcommand{\BM}[1]{\mbox{\boldmath$#1$}}
 *		\newcommand{\OM}[1]{\left(M\omega\right)^{#1/3}}
 *		\newcommand{\SP}[2]{\left(\BM{#1}\BM{#2}\right)}
 *		\begin{array}{c}
 *			\displaystyle MECO=-5\eta\frac{2}{3}\OM{-1}+\left(5\eta\frac{4}{3}
 *				\frac{9+\eta}{12}\right)\OM{1}+SO_{MECO}\OM{2}\\
 *				\displaystyle\left(5\eta\frac{6}{3}\frac{81-57\eta+\eta^2}{24}+
 *				SS_{MECO}+QM_{MECO}\right)\OM{3}\\
 *			\displaystyle
 *			SO_{MECO}=\sum_{i}-\frac{5}{9}\eta\frac{\chi_im_i^2}{M^2}
 *				\left(4+3\frac{m_j}{m_i}\right)\SP{\hat{L}_N}{\hat{\chi}_i};\quad
 *			QM_{MECO}=2\eta QM_{\omega}\\
 *			SS_{MECO}=-\displaystyle\frac{\chi_1m_1^2}{M^2}\frac{\chi_2m_2^2}{M^2}
 *				\left[\SP{\hat{\chi}_1}{\hat{\chi}_2}-3\SP{\hat{L}_N}
 *				{\hat{\chi}_1}\SP{\hat{L}_N}{\hat{\chi}_2}\right]
 *		\end{array}
 *	\f}
 */
/**	The constant parts are calculated by the LALSQTPNFillCoefficients() function and
 *	then used in this function with the dynamic parts: the products of the
 *	vectors and the powers of the \f$M\omega\f$.
 *
 *
 *	The orders above 2PN are incomplete, so use them if you want to try their
 *	effects.
 * @param[in]	t		: evolution time, not in used
 * @param[in]	values	: the values to be derived
 * @param[out]	dvalues	: the derived values and the last element is the MECO
 * @param[in]	params	: the LALSQTPN_Generator's parameters
 */
int LALSQTPNDerivator(REAL8 t, const REAL8 values[], REAL8 dvalues[],
		void * params);

/**		Enumeration to index the dynamic variables in the LALSQTPNGenerator function.
 */
typedef enum {
	LALSQTPN_PHASE,	///< index of the phase
	LALSQTPN_OMEGA,	///< index of the \f$M\omega\f$
	LALSQTPN_LNH_1,	///< index of the \f$\hat{L}_N\f$'s x component
	LALSQTPN_LNH_2,	///< index of the \f$\hat{L}_N\f$'s y component
	LALSQTPN_LNH_3,	///< index of the \f$\hat{L}_N\f$'s z component
	LALSQTPN_CHIH1_1,	///< index of the \f$\hat{\chi}_1\f$'s x component
	LALSQTPN_CHIH1_2,	///< index of the \f$\hat{\chi}_1\f$'s y component
	LALSQTPN_CHIH1_3,	///< index of the \f$\hat{\chi}_1\f$'s z component
	LALSQTPN_CHIH2_1,	///< index of the \f$\hat{\chi}_2\f$'s x component
	LALSQTPN_CHIH2_2,	///< index of the \f$\hat{\chi}_2\f$'s y component
	LALSQTPN_CHIH2_3,	///< index of the \f$\hat{\chi}_2\f$'s z component
	LALSQTPN_MECO,	///< index of the MECO
	LALSQTPN_NUM_OF_VAR	///< number of the dynamic variables
} LALSQTPNGeneratorVariables;

/**		The function generates the waveform.
 * @param[in,out]	status		: LAL universal status structure
 * @param[out]		waveform	: the generated waveform
 * @param[in]		params		: the input parameters
 */
void
LALSQTPNGenerator(LALStatus *status, CoherentGW *waveform,
		LALSQTPNWaveformParams *params);

#ifdef __cplusplus
}
#endif

#endif /* LALSQTPN_WAVEFORM_H */
