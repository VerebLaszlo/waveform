/**
 * @file waveform.c
 *		Containes the function definition to create GWforms.
 * @author László Veréb
 * @date 2010.04.06.
 */

#include "waveform.h"

void fill_Coefficients(waveform_Params * const params) {
	double spin_MPow2[2];
	double m_m[2];
	m_m[0] = params->mass[1] / params->mass[0];
	m_m[1] = params->mass[0] / params->mass[1];
	double piPow2 = LAL_PI * LAL_PI;
	double etaPow2 = SQR(params->eta);
	double etaPow3 = etaPow2 * params->eta;
	int i;
	for (i = 0; i < 2; i++) {
		spin_MPow2[i] = params->chi_Amp[i] * SQR(params->mass[i])
				/ SQR(params->total_Mass);
	}
	params->coeff.domega_Global = params->eta * 96. / 5.;
	for (i = LAL_PNORDER_NEWTONIAN; i <= LAL_PNORDER_PSEUDO_FOUR; i += 2) {
		params->coeff.MECO[i] = -5. * params->eta * (double) (i + 2) / 3.;
		params->coeff.MECO[i + 1] = 0.;
	}
	switch (params->order) {
	case LAL_PNORDER_PSEUDO_FOUR:
	case LAL_PNORDER_THREE_POINT_FIVE:
		params->coeff.domega[LAL_PNORDER_THREE_POINT_FIVE] = (-4415. / 4032.
				+ params->eta * 358675. / 6048. + etaPow2 * 91495. / 1512.)
				* LAL_PI;
	case LAL_PNORDER_THREE:
		params->coeff.domega[LAL_PNORDER_THREE] = (16447322263. / 139708800.
				- LAL_GAMMA * 1712. / 105. + SQR(LAL_PI) * 16. / 3.)
				+ (-273811877. / 1088640. + SQR(LAL_PI) * 451. / 48. - 88.
						* 1039. / 3. / 4620.) * params->eta + etaPow2 * 541.
				/ 896. - etaPow3 * 5605. / 2592.;
		params->coeff.ln_coeff = -856. / 105.;
		params->coeff.MECO[LAL_PNORDER_THREE] *= -675. / 64. + (209323. / 4032.
				- 205. / 96. * piPow2 + 110. / 9. * 1987. / 3080.)
				* params->eta - 155. / 96. * etaPow2 - 35. / 5184. * etaPow3;
	case LAL_PNORDER_TWO_POINT_FIVE:
		params->coeff.domega[LAL_PNORDER_TWO_POINT_FIVE] = -(4159. + 15876.
				* params->eta) * LAL_PI / 672.;
	case LAL_PNORDER_TWO:
		params->coeff.domega[LAL_PNORDER_TWO] = 34103. / 18144. + params->eta
				* 13661. / 2016. + etaPow2 * 59. / 18.;
		params->coeff.SS_const = 0.;
		params->coeff.QM_const = 0.;
		params->coeff.dchih[0][LAL_PNORDER_TWO] = spin_MPow2[1] / 2.;
		params->coeff.dchih[1][LAL_PNORDER_TWO] = spin_MPow2[0] / 2.;
		for (i = 0; i < 2; i++) {
			params->coeff.SS[i] = -spin_MPow2[i] * params->chi_Amp[i] / 96.;
			params->coeff.SS_const += spin_MPow2[i] * params->chi_Amp[i] * 7.
					/ 96.;
			params->coeff.QM[i] = spin_MPow2[i] * params->chi_Amp[i]
					* params->flatness[i] * 7.5;
			params->coeff.QM_const -= spin_MPow2[i] * params->chi_Amp[i]
					* params->flatness[i] / 2.5;
		}
		params->coeff.S1S2[0] = 721. * params->eta * params->chi_Amp[0]
				* params->chi_Amp[1] / 48.;
		params->coeff.S1S2[1] = -247 * params->eta * params->chi_Amp[0]
				* params->chi_Amp[1] / 48.;
		params->coeff.MECO[LAL_PNORDER_TWO] *= (-81. + 57. * params->eta
				- etaPow2) / 24.;
		params->coeff.MECO_Spin[2] = -spin_MPow2[0] * spin_MPow2[1];
	case LAL_PNORDER_ONE_POINT_FIVE:
		params->coeff.domega[LAL_PNORDER_ONE_POINT_FIVE] = 4. * LAL_PI;
		for (i = 0; i < 2; i++) {
			params->coeff.dchih[i][LAL_PNORDER_ONE_POINT_FIVE] = (4. + 3.
					* m_m[i]) * params->eta / 2.;
			params->coeff.dLNh[i] = -spin_MPow2[i] / params->eta;
			params ->coeff.SO[i] = -spin_MPow2[i] * (113. + 75. * m_m[i]);
			params->coeff.MECO_Spin[i] = -5. * spin_MPow2[i] * (4. + 3.
					* m_m[i]) / 9.;
		}
	case LAL_PNORDER_ONE:
		params->coeff.domega[LAL_PNORDER_ONE] = -(743. + 924. * params->eta)
				/ 336.;
		params->coeff.MECO[LAL_PNORDER_ONE] *= -(9. + params->eta) / 12.;
	case LAL_PNORDER_HALF:
		params->coeff.domega[LAL_PNORDER_HALF] = 0.;
	case LAL_PNORDER_NEWTONIAN:
		params->coeff.domega[LAL_PNORDER_NEWTONIAN] = 1.;
		params->coeff.MECO[LAL_PNORDER_NEWTONIAN] *= 1.;
	default:
		break;
	}
}

void derivator(double t, const double values[], double dvalues[], void * params) {
	waveform_Params *param = params;
	double MECO = 0., dphase = 0.;
	double dLNh[3], LNh[3];
	double dchih[2][3], chih[2][3];
	double domega = 0., omega = values[1];
	int i, j;
	for (i = 0; i < 3; i++) {
		LNh[i] = values[i + 2];
		chih[0][i] = values[i + 5];
		chih[1][i] = values[i + 8];
		dLNh[i] = 0.;
		dchih[0][i] = 0.;
		dchih[1][i] = 0.;
	}
	double omegaPowi_3[8];
	omegaPowi_3[0] = 1.;
	omegaPowi_3[1] = cbrt(omega);
	for (i = 2; i < 8; i++) {
		omegaPowi_3[i] = omegaPowi_3[i - 1] * omegaPowi_3[1];
	}
	for (i = LAL_PNORDER_NEWTONIAN; i < LAL_PNORDER_PSEUDO_FOUR; i++) {
		domega += param->coeff.domega[i] * omegaPowi_3[i];
	}
	double QM, SS, S1S2, chih1chih2, chih1xchih2[2][3], LNhchih[2],
			LNhxchih[2][3], temp;
	chih1chih2 = scalar_Product3(chih[0], chih[1]);
	for (i = 0; i < 2; i++) {
		LNhchih[i] = scalar_Product3(LNh, chih[i]);
	}
	switch (param->order) {
	case LAL_PNORDER_PSEUDO_FOUR:
	case LAL_PNORDER_THREE_POINT_FIVE:
	case LAL_PNORDER_THREE:
		domega += param->coeff.ln_coeff * log(16. * omegaPowi_3[2])
				* omegaPowi_3[LAL_PNORDER_THREE];
	case LAL_PNORDER_TWO_POINT_FIVE:
	case LAL_PNORDER_TWO:
		QM = param->coeff.QM_const;
		SS = param->coeff.SS_const;
		S1S2 = param->coeff.S1S2[0] * LNhchih[0] * LNhchih[1]
				+ param->coeff.S1S2[1] * chih1chih2;
		vector_product3(chih[1], chih[0],
				param->coeff.dchih[0][LAL_PNORDER_TWO], chih1xchih2[0]);
		vector_product3(chih[0], chih[1],
				param->coeff.dchih[1][LAL_PNORDER_TWO], chih1xchih2[1]);
		for (i = 0; i < 2; i++) {
			temp = -3 * param->coeff.dchih[i][LAL_PNORDER_TWO] * LNhchih[i];
			vector_product3(LNh, chih[i], temp, LNhxchih[i]);
		}
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 2; j++) {
				dchih[j][i] += (chih1xchih2[j][i] + LNhxchih[j][i])
						* omegaPowi_3[6];
			}
		}
		for (i = 0; i < 2; i++) {
			SS += param->coeff.SS[i] * SQR(LNhchih[i]);
			QM += param->coeff.QM[i] * LNhchih[i];
		}
		domega += (QM + SS + S1S2) * omegaPowi_3[LAL_PNORDER_TWO];
	case LAL_PNORDER_ONE_POINT_FIVE:
		for (i = 0; i < 2; i++) {
			domega += param ->coeff.SO[i] * LNhchih[i]
					* omegaPowi_3[LAL_PNORDER_ONE_POINT_FIVE];
		}
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 2; j++) {
				dchih[j][i]
						+= param->coeff.dchih[j][LAL_PNORDER_ONE_POINT_FIVE]
								* LNhxchih[j][i]
								* omegaPowi_3[LAL_PNORDER_ONE_POINT_FIVE];
				dLNh[i] += param->coeff.dLNh[j] * dchih[j][i];
			}
			dLNh[i] *= omegaPowi_3[1];
		}
	case LAL_PNORDER_ONE:
	case LAL_PNORDER_HALF:
	case LAL_PNORDER_NEWTONIAN:
	default:
		break;
	}
	domega *= param->coeff.domega_Global;
	dphase = omega + LNh[2] * (LNh[1] * dLNh[0] - LNh[0] * dLNh[1]) / (SQR(LNh[0]) * SQR(LNh[1]));

	dvalues[-1] = MECO;
	dvalues[0] = dphase;
	dvalues[1] = domega;
	for (i = 0; i < 3; i++) {
		dvalues[i + 2] = dLNh[i];
		dvalues[i + 5] = dchih[0][i];
		dvalues[i + 8] = dchih[1][i];
	}
}
