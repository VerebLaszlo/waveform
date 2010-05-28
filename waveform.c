/**
 * @file waveform.c
 *		Containes the function definition to create GWforms.
 * @author László Veréb
 * @date 2010.05.21.
 */

#include "waveform.h"
#include "integrator.h"

NRCSID (WAVEFORMC, "$Id$");

#if SPIN==1 || SPIN==3 || SPIN==5 || SPIN==7
#define _S1S2_
#endif
#if SPIN==2 || SPIN==3 || SPIN==6 || SPIN==7
#define _SS_
#endif
#if SPIN==4 || SPIN==5 || SPIN==6 || SPIN==7
#define _QM_
#endif

void fill_Coefficients(LALStatus *status, waveform_Params * const params) {
	INITSTATUS(status, "fill_Coefficients", WAVEFORMC);
	ATTATCHSTATUSPTR(status);

	// variable declaration and initialization
	REAL8 thetahat = 1039. / 4620.;
	REAL8 spin_MPow2[2];
	REAL8 m_m[2] = { params->mass[1] / params->mass[0], params->mass[0]
			/ params->mass[1] };
	REAL8 piPow2 = SQR(LAL_PI);
	REAL8 etaPow2 = SQR(params->eta);
	REAL8 etaPow3 = etaPow2 * params->eta;
	INT2 i;
	for (i = 0; i < 2; i++) {
		spin_MPow2[i] = params->chi_Amp[i] * SQR(params->mass[i])
				/SQR(params->total_Mass);
	}

	// calculating the coefficients
	params->coeff.domega_Global = params->eta * 96. / 5.;
	for (i = LAL_PNORDER_NEWTONIAN; i < LAL_PNORDER_PSEUDO_FOUR; i += 2) {
		params->coeff.MECO[i] = -5. * params->eta * (REAL8) (i + 2) / 3.;
	}
	switch (params->order) {
		case LAL_PNORDER_THREE_POINT_FIVE:
			params->coeff.domega[LAL_PNORDER_THREE_POINT_FIVE] = (-4415.
					/ 4032. + params->eta * 358675. / 6048. + etaPow2 * 91495.
					/ 1512.) * LAL_PI;
		case LAL_PNORDER_THREE:
			params->coeff.domega[LAL_PNORDER_THREE]
					= (16447322263. / 139708800. - LAL_GAMMA * 1712. / 105.
							+ piPow2 * 16. / 3.) + (-273811877. / 1088640.
							+ piPow2 * 451. / 48. - thetahat * 88. / 3.)
							* params->eta + etaPow2 * 541. / 896. - etaPow3
							* 5605. / 2592.;
			params->coeff.ln_coeff = -856. / 105.;
			params->coeff.MECO[LAL_PNORDER_THREE] *= -675. / 64. + (209323.
					/ 4032. - 205. * piPow2 / 96. + (110. / 9.) * (1987.
					/ 3080.)) * params->eta - 155. * etaPow2 / 96. - 35.
					* etaPow3 / 5184.;
		case LAL_PNORDER_TWO_POINT_FIVE:
			params->coeff.domega[LAL_PNORDER_TWO_POINT_FIVE] = -(4159. + 15876.
					* params->eta) * LAL_PI / 672.;
		case LAL_PNORDER_TWO:
			params->coeff.domega[LAL_PNORDER_TWO] = 34103. / 18144.
					+ params->eta * 13661. / 2016. + etaPow2 * 59. / 18.;
			params->coeff.SS_Omega_C = 0.;
			params->coeff.QM_Omega_C = 0.;
			params->coeff.S1S2_Chih[0] = spin_MPow2[1] / 2.;
			params->coeff.S1S2_Chih[1] = spin_MPow2[0] / 2.;
			//params->coeff.dchih[0][LAL_PNORDER_TWO] = spin_MPow2[1] / 2.;
			//params->coeff.dchih[1][LAL_PNORDER_TWO] = spin_MPow2[0] / 2.;
			for (i = 0; i < 2; i++) {
				params->coeff.SS_Omega[i] = -spin_MPow2[i] * params->chi_Amp[i]
						/ 96.;
				params->coeff.SS_Omega_C += 7. * spin_MPow2[i]
						* params->chi_Amp[i] / 96.;
				params->coeff.QM_Omega[i] = spin_MPow2[i] * params->chi_Amp[i]
						* params->flatness[i] * 7.5;
				params->coeff.QM_Omega_C -= spin_MPow2[i] * params->chi_Amp[i]
						* params->flatness[i] * 2.5;
				params->coeff.QM_Chih[i] = -params->flatness[i] * params->eta
						* params->chi_Amp[i] * 3. / 2.;
			}
			params->coeff.S1S2_Omega[0] = 721. * params->eta
					* params->chi_Amp[0] * params->chi_Amp[1] / 48.;
			params->coeff.S1S2_Omega[1] = -247. * params->eta
					* params->chi_Amp[0] * params->chi_Amp[1] / 48.;
			params->coeff.MECO[LAL_PNORDER_TWO] *= (-81. + 57. * params->eta
					- etaPow2) / 24.;
			params->coeff.S1S2_MECO = -spin_MPow2[0] * spin_MPow2[1];
			//params->coeff.MECO_Spin[2] = -spin_MPow2[0] * spin_MPow2[1];
			params->coeff.QM_MECO = 2. * params->eta;
		case LAL_PNORDER_ONE_POINT_FIVE:
			params->coeff.domega[LAL_PNORDER_ONE_POINT_FIVE] = 4. * LAL_PI;
			for (i = 0; i < 2; i++) {
				params->coeff.SO_Chih[i] = (4. + 3. * m_m[i]) * params->eta
						/ 2.;
				//params->coeff.dchih[i][LAL_PNORDER_ONE_POINT_FIVE] = (4. + 3.
				//		* m_m[i]) * params->eta / 2.;
				params->coeff.dLNh[i] = -spin_MPow2[i] / params->eta;
				params->coeff.SO_Omega[i] = -spin_MPow2[i] * (113. + 75.
						* m_m[i]) / 12.;
				params->coeff.SO_MECO[i] = -spin_MPow2[i] * 5. * params->eta
						* (4. + 3. * m_m[i]) / 9.;
				//params->coeff.MECO_Spin[i] = -spin_MPow2[i] * 5. * params->eta
				//		* (4. + 3. * m_m[i]) / 9.;
			}
		case LAL_PNORDER_ONE:
			params->coeff.domega[LAL_PNORDER_ONE]
					= -(743. + 924. * params->eta) / 336.;
			params->coeff.MECO[LAL_PNORDER_ONE] *= -(9. + params->eta) / 12.;
		case LAL_PNORDER_HALF:
			params->coeff.domega[LAL_PNORDER_HALF] = 0.;
		case LAL_PNORDER_NEWTONIAN:
			params->coeff.domega[LAL_PNORDER_NEWTONIAN] = 1.;
		default:
			break;
	}
	DETATCHSTATUSPTR(status);
	RETURN(status);
}

int derivator(REAL8 t, const REAL8 values[], REAL8 dvalues[], void * param) {
	waveform_Params *params = param;

	// variable declaration and initialization
	INT2 i, j; // indexes
	for (i = OMEGA; i < NUM_OF_VAR; i++) {
		dvalues[i] = 0.;
	}
	REAL8 omegaPowi_3[8];
	omegaPowi_3[0] = 1.;
	omegaPowi_3[1] = cbrt(values[OMEGA]);
	for (i = 2; i < 8; i++) {
		omegaPowi_3[i] = omegaPowi_3[i - 1] * omegaPowi_3[1];
	}
	REAL8 S1S2_Omega, SS_Omega, QM_Omega;
	S1S2_Omega = SS_Omega = QM_Omega = 0.;
	REAL8 chih1chih2, chih1xchih2[2][3], LNhchih[2], LNhxchih[2][3], temp;
	chih1chih2 = scalar_Product3(values + CHIH1_1, values + CHIH2_1);
	for (i = 0; i < 2; i++) {
		LNhchih[i] = scalar_Product3(values + LNH_1, values + CHIH1_1 + 3 * i);
	}

	// calculating domega and MECO without the spin components
	for (i = params->order; i >= LAL_PNORDER_NEWTONIAN; i--) {
		dvalues[OMEGA] += params->coeff.domega[i] * omegaPowi_3[i];
	}
	dvalues[MECO] += params->coeff.MECO[0] / omegaPowi_3[1];
	for (i = LAL_PNORDER_NEWTONIAN; i <= params->order; i += 2) {
		dvalues[MECO] += params->coeff.MECO[i] * omegaPowi_3[i - 1];
	}

	// calculating the other derivatives and the domega and MECO with spin
	// components
	switch (params->order) {
		case LAL_PNORDER_THREE_POINT_FIVE:
		case LAL_PNORDER_THREE:
			dvalues[OMEGA] += params->coeff.ln_coeff
					* log(16. * omegaPowi_3[2])
					* omegaPowi_3[LAL_PNORDER_THREE];
		case LAL_PNORDER_TWO_POINT_FIVE:
		case LAL_PNORDER_TWO:
#ifdef _S1S2_
			// S1S2 for domega
			S1S2_Omega = params->coeff.S1S2_Omega[0] * LNhchih[0] * LNhchih[1]
					+ params->coeff.S1S2_Omega[1] * chih1chih2;
			// S1S2 for MECO
			dvalues[MECO] += params->coeff.S1S2_MECO * (chih1chih2 - 3
					* LNhchih[0] * LNhchih[1]) * omegaPowi_3[3];
#endif
#ifdef _SS_
			// SS for domega
			SS_Omega = params->coeff.SS_Omega_C;
#endif
#ifdef _QM_
			// QM for domega
			QM_Omega = params->coeff.QM_Omega_C;
#endif
			for (i = 0; i < 2; i++) {
				j = (i + 1) % 2; // the opposite index
				// the 3*index is used, to acces the first spin, if index=0,
				// otherwise the second spin
#ifdef _S1S2_
				//S1S2 for dchih
				vector_product3(values + CHIH1_1 + 3 * j, values + CHIH1_1 + 3
						* i, params->coeff.S1S2_Chih[i], chih1xchih2[i]);
				temp = -3 * params->coeff.S1S2_Chih[i] * LNhchih[j];
				vector_product3(values + LNH_1, values + CHIH1_1 + 3 * i, temp,
						LNhxchih[i]);
				for (j = 0; j < 3; j++) {
					dvalues[CHIH1_1 + 3 * i + j] += (chih1xchih2[i][j]
							+ LNhxchih[i][j]) * omegaPowi_3[6];
				}
#endif
#ifdef _SS_
				// SS for domega
				SS_Omega += params->coeff.SS_Omega[i] * SQR(LNhchih[i]);
#endif
#ifdef _QM_
				// QM for domega
				QM_Omega += params->coeff.QM_Omega[i] * LNhchih[i];
				// QM for dchih
				temp = params->coeff.QM_Chih[i] * LNhchih[i];
				vector_product3(values + LNH_1, values + CHIH1_1 + 3 * i, temp,
						LNhxchih[i]);
				dvalues[CHIH1_1 + 3 * i + j] += LNhxchih[i][j] * omegaPowi_3[6];
			}
			dvalues[MECO] += params->coeff.QM_MECO * QM_Omega * omegaPowi_3[3];
#else
		}
#endif
			dvalues[OMEGA] += (QM_Omega + SS_Omega + S1S2_Omega)
					* omegaPowi_3[LAL_PNORDER_TWO];
		case LAL_PNORDER_ONE_POINT_FIVE:
			for (i = 0; i < 2; i++) {
				vector_product3(values + LNH_1, values + CHIH1_1 + 3 * i,
						params->coeff.SO_Chih[i] * omegaPowi_3[5], LNhxchih[i]);
				dvalues[OMEGA] += params ->coeff.SO_Omega[i] * LNhchih[i]
						* omegaPowi_3[LAL_PNORDER_ONE_POINT_FIVE];
				dvalues[MECO] += params->coeff.SO_MECO[i] * LNhchih[i]
						* omegaPowi_3[2];
			}
			for (i = 0; i < 3; i++) {
				dvalues[CHIH1_1 + i] += LNhxchih[0][i];
				dvalues[CHIH2_1 + i] += LNhxchih[1][i];
				dvalues[LNH_1 + i] += (params->coeff.dLNh[0] * dvalues[CHIH1_1
						+ i] + params->coeff.dLNh[1] * dvalues[CHIH2_1 + i])
						* omegaPowi_3[1];
			}
		case LAL_PNORDER_ONE:
		case LAL_PNORDER_HALF:
		case LAL_PNORDER_NEWTONIAN:
		default:
			break;
	}
	dvalues[OMEGA] *= params->coeff.domega_Global * omegaPowi_3[7]
			* omegaPowi_3[4];
	dvalues[PHASE] = values[OMEGA] + values[LNH_3] * (values[LNH_2]
			* dvalues[LNH_1] - values[LNH_1] * dvalues[LNH_2])
			/ (SQR(values[LNH_1]) + SQR(values[LNH_2]));
	return GSL_SUCCESS;
}

void generator(LALStatus *status, waveform_Params *params, CoherentGW *waveform) {
	INITSTATUS(status, "generator", WAVEFORMC);
	ATTATCHSTATUSPTR(status);

	// variable declaration and initialization
	UINT4 i = 0; // index
	REAL8 time = 0.;
	REAL8 LNhztol = 1.0e-8; // tolerancia LNhz-re
	const REAL8 geometrized_m_total = params->total_Mass * LAL_MTSUN_SI;
	const REAL8 freq_Step = geometrized_m_total * LAL_PI;
	const REAL8 step = params->sampling_Time / geometrized_m_total;
	REAL8 values[NUM_OF_VAR], dvalues[NUM_OF_VAR]; // + 1 for MECO
	integrator_System integrator;
	ERR_STR_END("runge_kutta_init");
	integrator_init(status->statusPtr, NUM_OF_VAR - 1, params, derivator,
			&integrator);
	CHECKSTATUSPTR(status);

	// initializing the dynamic variables
	values[PHASE] = params->phi;
	values[OMEGA] = params->lower_Freq * freq_Step;
	values[LNH_1] = sin(params->inclination);
	values[LNH_2] = 0.;//+ 1.e-100;
	values[LNH_3] = cos(params->inclination);
	for (i = 0; i < 3; i++) {
		values[CHIH1_1 + i] = params->chih[0][i];
		values[CHIH2_1 + i] = params->chih[1][i];
	}

	// filling the coefficients
	fill_Coefficients(status->statusPtr, params);
	CHECKSTATUSPTR(status);
	derivator(time, values, dvalues, params);
	dvalues[MECO] = -1.; // to be able to start the loop
	i = 0;
	ERR_STR_END("while start");
	while (dvalues[MECO] < 0. && dvalues[OMEGA] > 0.0 && SQR(values[LNH_3])
			< 1. - LNhztol && values[OMEGA] / freq_Step < params->sampling_Freq
			/ 2.) {
		REAL8 alpha = atan2(values[LNH_2], values[LNH_1]);
		REAL8 amp = params->signal_Amp * pow(values[OMEGA], 2. / 3.);

		// writing the output
		if (waveform->h) {
			REAL8 temp1 = -0.5 * amp * cos(2. * (values[PHASE] - params->phi))
					* (values[LNH_3] * values[LNH_3] + 1.);
			REAL8 temp2 = amp * sin(2. * values[PHASE]) * values[LNH_3];
			waveform->h->data->data[2 * i] = temp1 * cos(2. * alpha) + temp2
					* sin(2. * alpha);
			waveform->h->data->data[2 * i + 1] = temp1 * sin(2. * alpha)
					- temp2 * cos(2. * alpha);
		}
		if (waveform->a && waveform->phi->data && waveform->shift->data
				&& waveform->f->data) {
			waveform->a->data->data[2 * i] = -amp * 0.5 * (1. + values[LNH_3]
					* values[LNH_3]);
			waveform->a->data->data[2 * i + 1] = -amp * values[LNH_3];
			waveform->phi->data->data[i] = 2. * values[PHASE];
			waveform->shift->data->data[i] = 2. * alpha;
		}
		waveform->f->data->data[i] = values[OMEGA] / freq_Step;

		// evolving
		time = i++ * params->sampling_Time;
		integrator_Func(status->statusPtr, &integrator, step, values);
		CHECKSTATUSPTR(status);

		// if one of the variables is nan, the PN approximation braked down
		if (isnan(values[PHASE]) || isnan(values[OMEGA])
				|| isnan(values[LNH_1]) || isnan(values[LNH_2])
				|| isnan(values[LNH_3]) || isnan(values[CHIH1_1])
				|| isnan(values[CHIH1_2]) || isnan(values[CHIH1_3])
				|| isnan(values[CHIH2_1]) || isnan(values[CHIH2_2])
				|| isnan(values[CHIH2_3])) {
			ERR_STR_END("nan");
			break;
		}
		derivator(time, values, dvalues, params);
		if (i == waveform->f->data->length + 1) {
			break;
		}
#if DEBUG==1
		/*fprintf(stdout,
		 "index: %d, %10.5lg < .0, %10.5lg > 0., %10.5lg < %lg, "
		 "%10.5lg < %10.5lg\n", i, dvalues[MECO], dvalues[OMEGA],
		 SQR(values[LNH_3]), 1. - LNhztol,
		 values[OMEGA] / f->data_Step, params->sampling_Freq / 2.);
		 fflush(stdout);*/
#endif
	}
	ERR_STR_END("while end");
	waveform->f->data->length = i;
	printf("%d\n", i);
	/// \bug there is double free or corruption if uncommented
	//integrator_free(status->statusPtr, &integrator);
	CHECKSTATUSPTR(status);
	DETATCHSTATUSPTR(status);
	RETURN(status);
}
