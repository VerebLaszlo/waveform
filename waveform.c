/**
 * @file waveform.c
 *		Containes the function definition to create GWforms.
 * @author László Veréb
 * @version 0.1
 * @date 2010.04.06.
 */

#include "waveform.h"
#include "util_match.h"

void fill_Coefficients(waveform_Params * const params) {
	double spin_MPow2[2];
	double m_m[2];
	m_m[0] = param->mass[1] / params->mass[0];
	m_m[1] = param->mass[0] / params->mass[1];
	int i;
	for (i = 0; i < 2; i++) {
		spin_MPow2[i] = params.chiAmp[i] * SQR(params.mass[i]) / SQR(params.total_Mass);
	}
	switch (order) {
	case LAL_PNORDER_PSEUDO_FOUR:
	case LAL_PNORDER_THREE_POINT_FIVE:
	case LAL_PNORDER_THREE:
	case LAL_PNORDER_TWO_POINT_FIVE:
	case LAL_PNORDER_TWO:
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
		for (i = 0; i < 2; i++) {
			params->coeff.dchih[i][LAL_PNORDER_ONE_POINT_FIVE] = (4. + 3. * m_m[i]) *
			params->eta / 2.;
			params->coeff.dLNh[i] = - spin_MPowe2[i] / params->eta;
			params->coeff.SO[i] = spin_MPow2[i] * (113. + 75. * m_m[i])
		}
	case LAL_PNORDER_ONE:
	case LAL_PNORDER_HALF:
	case LAL_PNORDER_NEWTONIAN:
	default:
		break;
	}
}

