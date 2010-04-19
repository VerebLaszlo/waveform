/**
 * @file waveform.h
 *		Containes the enums structs and function to create GWforms.
 * @author László Veréb
 * @version 0.1
 * @date 2010.04.06.
 */

/*
 kivpróbálni doxygen tag-ek:
 retval <return value> {leírás}
 warning
 deprecated
 bug
 test
 xrefitem <key>  "(heading)" "(list title)" {text}
 */

#ifndef WAVEFORM_H_
#define WAVEFORM_H_

#include <math.h>
//#include <lal/LALInspiral.hu>
#ifndef _LALINSPIRAL_H
#include "lal_inspiral.h"
#endif
#include "util_math.h"

/**
 *		The structure contains the coefficients for calculating the derivatives of the evolving quantities.
 */
typedef struct coefficients_Tag {
	double domega_Global; ///< global coefficient for domega
	double domega[8]; ///< coefficients for domega for every PN order
	double SO[2]; ///< the spin-orbit coefficients for domega
	double S1S2[2]; ///< the spin1-spin2 coefficients for domega
	double SS[2]; ///< the spin-selft coefficients for domega
	double SS_const; ///< the constant spin-selft coefficients for domega
	double QM[2]; ///< the quadropole-monopole coefficients for domega
	double QM_const; ///< the constant quadropole-monopole coefficients for domega
	double dchih[2][8]; ///< coefficients for dchih for every PN order
	double dLNh[2]; ///< coefficients for dLNh
	double MECO[8]; ///< coefficients for MECO-test
	double MECO_Spin[3]; ///< the spin related coefficients for MECO-test
	double ln_coeff; ///< coefficient for the ln component in domega
} coefficients;

/**
 *		The structure contains the system's and the generator's parameters.
 */
typedef struct waveform_Params_Tag {
	//@{@name mass-parameters
	double mass[2]; ///< masses of the BHs
	double total_Mass; ///< total mass
	double chirp_Mass; ///< chirp mass
	double mu; ///< reduced mass
	double eta; ///< symmetric mass ratio	//@}
	//@{@name spin-parameters
	double chi[2][3]; ///< components of the normalized spin
	double chih[2][3]; ///< components of the unity-vectors of the normalized spin
	double chi_Amp[2]; ///< amplitude of the normalized spin //@}
	//@{@name other system-parameters
	double flatness[2]; ///< flatness of the BHs or NSs
	double distance; ///< distance to the source in Mps
	double inclination; ///< inclination of the system
	double phi; ///< the initial phase	//@}
	//@{@name other parameters
	double initial_Freq; ///< the initial frequency of the GW
	double maximal_Freq; ///< the maximal frequency of the GW
	double sampling_Freq; ///< sampling frequency
	double sampling_Time; ///< sampling time
	LALPNOrder order; ///< the Post_Newtonian order of the GW generation
	coefficients coeff; ///< coefficients for the deriving the parameters	//@}
} waveform_Params;

/**
 *		The function fills the @link coefficients @endlink structure with the needed
 *		coefficients for generating the waveform up to the given PN-order.
 * @warning asnuming that every member of the @link params @endlink is set.
 * @param[in,out]	params	: the generator's parameters
 */
void fill_Coefficients(waveform_Params * const params);

/**
 *		The function calculates the derived values.
 \f[
 \frac{d\hat{\chi}_i}{d\left(t/m\right)}=\frac{\eta}{2}
 \left(
 4+3\frac{m_j}{m_i}
 \right)\hat{L}_N\times\hat{\chi}_i\left(M\omega\right)^{5/3}+
 \frac{\chi_j m_j^2}{2 M^2}
 \left[
 \hat{\chi}_j-3
 \left(
 \hat{L}_N\hat{\chi}_j
 \right)\hat{L}_N
 \right]\times\hat{\chi}_i\left(M\omega\right)^{6/3};\quad
 \frac{d\hat{L}_N}{d\left(t/M\right)}=-\frac{1}{\eta}
 \left[
 \sum_{i=1,j}\frac{\chi_i m_i^2}{M^2}\frac{d\hat{\chi}_i}{d\left(t/M\right)}
 \right]\left(M\omega\right)^{6/3}
 \f]
 \f[
 \frac{d\left(M\omega\right)}{d\left(t/M\right)}=\frac{96\eta}{5}\left(M\omega\right)^{11/3}
 \left[
 1-\frac{743+924\eta}{336}\left(M\omega\right)^{2/3}+\left(4\pi-\sigma_{SO}\right)\left(M\omega\right)^{3/3}+
 \left(
 \frac{34103}{18144}+\frac{13661}{2016}\eta+\frac{59}{18}\eta^2+\sigma_{S1S2}+\sigma_{SS}+\sigma_{QM}
 \right)\left(M\omega\right)^{4/3}-
 \frac{4159+15876\eta}{672}\pi\left(M\omega\right)^{5/3}
 \right]
 \f]
 \f[
 \sigma_{SO}=\frac{1}{12}\sum_{i,j}\frac{\chi_i m_i^2}{M^2}\left(113+75\frac{m_j}{m_i}\right)\hat{L}_N\hat{\chi}_i;\quad
 \sigma_{S1S2}=\frac{\eta\chi_1\chi_2}{48}\left[721\left(\hat{L}_N\hat{\chi}_1\right)\left(\hat{L}_N\hat{\chi}_2\right)-247\left(\hat{\chi}_1\hat{\chi}_2\right)\right];\quad
 \f]
 \f[
 \sigma_{SS}=\frac{1}{96}\sum_{i,j}\frac{\chi_i^2 m_i^2}{M^2}\left[7-\left(\hat{L}_N\hat{\chi}_i\right)\right];\quad
 \sigma_{QM}=\frac{5}{2}\sum_{i,j}\frac{\chi_i^2 m_i^2 a_i}{M^2}\left[3\left(\hat{L}_N\hat{\chi}_i\right)^2-1\right]
 \f]
 * @param[in]	t		: ?????????????
 * @param[in]	values	: the values to be derivated
 * @param[out]	dvalues	: the derivated values (-1 for MECO, 0 for dphi)
 * @param[in]	params	: the generator's parameters
 */
void
derivator(double t, const double values[], double dvalues[], void * params);

#endif /* WAVEFORM_H_ */
