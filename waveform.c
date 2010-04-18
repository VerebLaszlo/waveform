/**
 * @file waveform.c
 *		Containes the function definition to create GWforms.
 * @author László Veréb
 * @version 0.1
 * @date 2010.04.06.
 */

#include "waveform.h"
#include "util_math.h"

void fill_Coefficients(waveform_Params * const params) {
	double spin_MPow2[2];
	double m_m[2];
	m_m[0] = param->mass[1] / params->mass[0];
	m_m[1] = param->mass[0] / params->mass[1];
	double etaPow2 = SQR(params->eta);
	double etaPow3 = etaPow2 * params->eta;
	int i;
	for (i = 0; i < 2; i++) {
		spin_MPow2[i] = params.chiAmp[i] * SQR(params.mass[i]) / SQR(params.total_Mass);
	}
	params->coeff.domega_Global = params.eta * 96. / 5.;
	switch (order) {
	case LAL_PNORDER_PSEUDO_FOUR:
	case LAL_PNORDER_THREE_POINT_FIVE:
		params->coeff.domega[LAL_PNORDER_THREE_POINT_FIVE] = (-4415. / 4032. +
		params->eta * 358675. / 6048. + etaPow2 * 91495. / 1512.) * LAL_PI;
	case LAL_PNORDER_THREE:
		params->coeff.domega[LAL_PNORDER_THREE] = (16447322263. / 139708800. -
		EulerC * 1712. / 105. + SQR(LAL_PI) * 16. / 3.) + (-273811877. /
		1088640. + SQR(LAL_PI) * 451. / 48. - 88. * 1039. / 3. / 4620.) *
		params->eta + etaPow2 * 541. / 896. - etaPow3 * 5605. / 2592.;
		params->coeff.ln_coeff = -856. / 105.;
	case LAL_PNORDER_TWO_POINT_FIVE:
		params->coeff.domega[LAL_PNORDER_TWO_POINT_FIVE] = -(4159. + 15876. *
		params->eta) * LAL_PI / 672.;
	case LAL_PNORDER_TWO:
		params->coeff.domega[LAL_PNORDER_TWO] = 34103. / 18144. + params->eta *
		13661. / 2016. + etaPow2 * 59. / 18.;
		params->coeff.SS_const = 0.;
		params->coeff.QM_const = 0.;
		for (i = 0; i < 2; i++) {
			params->coeff.dchih[i][LAL_PNORDER_TWO] = spin_MPow2[i] / 2.;
			params->coeff.SS[i] = -spin_MPow2[i] * params->chi_Amp[i] / 96.;
			params->coeff.SS_const += spin_MPow2[i] * params->chi_Amp[i] * 7. / 96.;
			params->coeff.QM = spin_MPow2[i] * params->chi_Amp[i] * flatness[i] * 7.5;
			params->coeff.QM_const -= spin_MPow2[i] * params->chi_Amp[i] *
			params->flatness[i] / 2.5;
		}
		params->coeff.S1S2[0] = 721. * params->eta * params->chi_Amp[0] *
		params->chi_Amp[1] / 48.;
		params->coeff.S0S2[1] = - 247 * params->eta * params->chi_Amp[0] *
		params->chi_Amp[1] / 48.;
	case LAL_PNORDER_ONE_POINT_FIVE:
		params->coeff.domega[LAL_PNORDER_ONE_POINT_FIVE] = 4. * LAL_PI;
		for (i = 0; i < 2; i++) {
			params->coeff.dchih[i][LAL_PNORDER_ONE_POINT_FIVE] = (4. + 3. * m_m[i]) *
			params->eta / 2.;
			params->coeff.dLNh[i] = - spin_MPowe2[i] / params->eta;
			params->coeff.SO[i] = spin_MPow2[i] * (113. + 75. * m_m[i])
		}
	case LAL_PNORDER_ONE:
		params->coeff.domega[LAL_PNORDER_ONE] = (743. + 924. * params->eta) /
		336.;
	case LAL_PNORDER_HALF:
		params->coeff.domega[LAL_PNORDER_HALF] = 0.;
	case LAL_PNORDER_NEWTONIAN:
		params->coeff.domega[LAL_PNORDER_NEWTONIAN] = 1.;
	default:
		break;
	}
}

