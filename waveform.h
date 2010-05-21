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
#include <lal/LALConstants.h>	// \test
#include <lal/Units.h>

#include "util_math.h"
#include "util_other.h"
#include "util_debug.h"

NRCSID (WAVEFORMH, "$Id$");

/**
 *		The structure contains the coefficients for calculating the derivatives of the evolving quantities.
 */
typedef struct coefficients_Tag {
	REAL8 domega_Global; ///< global coefficient for domega
	REAL8 domega[LAL_PNORDER_PSEUDO_FOUR]; ///< coefficients for domega for every PN order
	REAL8 SO[2]; ///< the spin-orbit coefficients for domega
	REAL8 S1S2[2]; ///< the spin1-spin2 coefficients for domega
	REAL8 SS[2]; ///< the spin-selft coefficients for domega
	REAL8 SS_const; ///< the constant spin-selft coefficients for domega
	REAL8 QM[2]; ///< the quadropole-monopole coefficients for domega
	REAL8 QM_const; ///< the constant quadropole-monopole coefficients for domega
	REAL8 dchih[2][LAL_PNORDER_PSEUDO_FOUR]; ///< coefficients for dchih for every PN order
	REAL8 dLNh[2]; ///< coefficients for dLNh
	REAL8 MECO[8]; ///< coefficients for MECO-test
	REAL8 MECO_Spin[LAL_PNORDER_PSEUDO_FOUR]; ///< the spin related coefficients for MECO-test
	REAL8 ln_coeff; ///< coefficient for the ln component in domega
} coefficients;

/**
 *		The structure contains the system's and the generator's parameters.
 */
typedef struct waveform_Params_Tag {
	//@{@name mass-parameters
	REAL8 mass[2]; ///< masses of the BHs in $M_\odot$
	REAL8 total_Mass; ///< total mass in $M_\odot$
	REAL8 chirp_Mass; ///< chirp mass in $M_\odot$
	REAL8 mu; ///< reduced mass in $M_\odot$
	REAL8 eta; ///< symmetric mass ratio	//@}
	//@{@name spin-parameters
	REAL8 chi[2][3]; ///< components of the normalized spin
	REAL8 chih[2][3]; ///< components of the unity-vectors of the normalized spin
	REAL8 chi_Amp[2]; ///< amplitude of the normalized spin //@}
	//@{@name other system-parameters
	REAL8 flatness[2]; ///< flatness of the BHs or NSs
	REAL8 distance; ///< distance to the source in $Mps$
	REAL8 inclination; ///< inclination of the system $rad$
	REAL8 phi; ///< the initial phase (currently not in use)	//@}
	//@{@name other parametersa
	REAL8 signal_Amp; ///< the amplitude of the signal
	REAL8 lower_Freq; ///< the detectors sensitivityband's lower border in $Hz$
	//	REAL8 upper_Freq; ///< the detectors sensitivityband's lower border
	//	REAL8 cutoff_Freq; ///< the highest detectable frequency of the GW
	//	REAL8 chirp_Time; ///< \todo dokot írni
	REAL8 sampling_Freq; ///< sampling frequency in $Hz$
	REAL8 sampling_Time; ///< sampling time in $s$
	LALPNOrder order; ///< the Post_Newtonian order of the GW generation
	coefficients coeff; ///< coefficients for the deriving the parameters	//@}
} waveform_Params;

/**
 *		The function fills the coefficients structure with the needed
 *		coefficients for generating the waveform up to the given PN-order.
 * @param[in,out]	status	: LAL universal status structure
 * @param[in,out]	params	: the generator's parameters
 */
void fill_Coefficients(LALStatus *status, waveform_Params * const params);

/**
 *		The function calculates the derived values.
 *	\f[
 *		\frac{d\hat{\chi}_i}{d\left(t/m\right)}=\frac{\eta}{2}
 *		\left(4+3\frac{m_j}{m_i}\right)\hat{L}_N\times\hat{\chi}_i
 *		\left(M\omega\right)^{5/3}+\frac{\chi_j m_j^2}{2 M^2}
 *		\left[
 *			\hat{\chi}_j-3
 *			\left(\hat{L}_N\hat{\chi}_j\right)\hat{L}_N
 *		\right]\times\hat{\chi}_i\left(M\omega\right)^{6/3};\quad
 *		\frac{d\hat{L}_N}{d\left(t/M\right)}=-\frac{1}{\eta}
 *		\left[
 *			\sum_{i=1,j}\frac{\chi_i m_i^2}{M^2}
 *			\frac{d\hat{\chi}_i}{d\left(t/M\right)}
 *		\right]\left(M\omega\right)^{6/3}
 *	\f]
 *	\f[
 *		\frac{d\left(M\omega\right)}{d\left(t/M\right)}=
 *		\frac{96\eta}{5}\left(M\omega\right)^{11/3}
 *		\left[
 *			1-\frac{743+924\eta}{336}\left(M\omega\right)^{2/3}+
 *			\left(4\pi-\sigma_{SO}\right)\left(M\omega\right)^{3/3}+
 *			\left(
 *				\frac{34103}{18144}+\frac{13661}{2016}\eta+
 *				\frac{59}{18}\eta^2+\sigma_{S1S2}+\sigma_{SS}+\sigma_{QM}
 *			\right)\left(M\omega\right)^{4/3}-
 *			\frac{4159+15876\eta}{672}\pi\left(M\omega\right)^{5/3}
 *		\right]
 *	\f]
 *	\f[
 *		\sigma_{SO}=\frac{1}{12}\sum_{i,j}\frac{\chi_i m_i^2}{M^2}
 *			\left(113+75\frac{m_j}{m_i}\right)\hat{L}_N\hat{\chi}_i;\quad
 *		\sigma_{S1S2}=\frac{\eta\chi_1\chi_2}{48}
 *		\left[
 *			721\left(\hat{L}_N\hat{\chi}_1\right)
 *			\left(
 *				(\hat{L}_N\hat{\chi}_2\right)-247\left(\hat{\chi}_1\hat{\chi}_2
 *			\right)
 *		\right];\quad
 *	\f]
 *	\f[
 *		\sigma_{SS}=\frac{1}{96}\sum_{i,j}\frac{\chi_i^2 m_i^2}{M^2}
 *			\left[7-\left(\hat{L}_N\hat{\chi}_i\right)\right];\quad
 *		\sigma_{QM}=\frac{5}{2}\sum_{i,j}\frac{\chi_i^2 m_i^2 a_i}{M^2}
 *			\left[3\left(\hat{L}_N\hat{\chi}_i\right)^2-1\right]
 *	\f]
 * @param[in]	t		: evolution time, not in used
 * @param[in]	values	: the values to be derivated
 * @param[out]	dvalues	: the derivated values and the last element is the MECO
 * @param[in]	params	: the generator's parameters
 */
int derivator(REAL8 t, const REAL8 values[], REAL8 dvalues[], void * params);

/**
 *		Enumeration to indexing the dynamic variables in the generator function.
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

/**
 *		\todo dekumentáció jobb merírása
 *		The structure's contents are the frequency in the freq->data array and
 *	the one of the following:
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
	REAL8Vector *phase; ///< the phase function
	REAL4Vector *pol; ///< the polarization shift
	REAL4Vector *freq; ///< the frequency
	REAL4TimeVectorSeries *a; ///< the amplitude components
} waveform;

/**
 *		The function generates the waveform.
 * @param[in,out]	status	: LAL universal status structure
 * @param[in]		params	: the input parameters
 * @param[out]		wave	: the generated waveform
 */
void generator(LALStatus *status, waveform_Params *params, waveform *wave);

#endif /* WAVEFORM_H */
