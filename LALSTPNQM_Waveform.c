/**
 * @file waveform.c
 *		Containes the function definition to create GWforms.
 * @author László Veréb
 * @date 2010.05.21.
 */

#include "LALSTPNQM_Waveform.h"
#include "LALSTPNQM_Integrator.h"

NRCSID (WAVEFORMC, "$Id LALSTPNQM_Waveform.c$");

#if SPIN==1 || SPIN==3 || SPIN==5 || SPIN==7
#define _S1S2_
#endif
#if SPIN==2 || SPIN==3 || SPIN==6 || SPIN==7
#define _SS_
#endif
#if SPIN==4 || SPIN==5 || SPIN==6 || SPIN==7
#define _QM_
#endif

/**		The macrofunction returns the square of the argument.
 * \warn Do not use with incrementing or decrementing operators!
 * @param[in] a	 : the number
 * @return the square of the number
 */
#define SQR(a) ((a)*(a))

/**		The function calculates the scalar product of two vectors.
 * @param[in]  left		: the left vector
 * @param[in]  right	: the right vector
 * @return	the product
 */
#define SCALAR_PRODUCT3(a1, a2) \
	((a1)[0] * (a2)[0] + (a1)[1] * (a2)[1] + (a1)[2] * (a2)[2]);

/**		The function calculates the wightened vector product of two vectors.
 * @param[in]  left		: the left vector
 * @param[in]  right	: the right vector
 * @param[in]  weight	: the product is multiplied by this value
 * @param[out] product	: the wightened product
 */
#define VECTOR_PRODUCT3W(left, right, weight, product)\
	(product)[0] = (weight) * ((left)[1] * (right)[2] - (left)[2] * (right)[1]);\
	(product)[1] = (weight) * ((left)[2] * (right)[0] - (left)[0] * (right)[2]);\
	(product)[2] = (weight) * ((left)[0] * (right)[1] - (left)[1] * (right)[0]);

/**		The function calculates the wightened vector product of two vectors.
 * @param[in]  left		: the left vector
 * @param[in]  right	: the right vector
 * @param[out] product	: the wightened product
 */
#define VECTOR_PRODUCT3(left, right, product)\
	(product)[0] = ((left)[1] * (right)[2] - (left)[2] * (right)[1]);\
	(product)[1] = ((left)[2] * (right)[0] - (left)[0] * (right)[2]);\
	(product)[2] = ((left)[0] * (right)[1] - (left)[1] * (right)[0]);

void LALSTPNQM_Fill_Coefficients(LALStatus *status, LALSTPNQM_Waveform_Params * const params) {
	INITSTATUS(status, "LALSTPNQM_Fill_Coefficients", WAVEFORMC);

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
			for (i = 0; i < 2; i++) {
#ifdef _SS_
				params->coeff.SS_Omega[i] = -spin_MPow2[i] * params->chi_Amp[i]
						/ 96.;
				params->coeff.SS_Omega_C -= 7. * params->coeff.SS_Omega[i];
#endif
#ifdef _QM_
				params->coeff.QM_Omega[i] = spin_MPow2[i] * params->chi_Amp[i]
						* params->flatness[i] * 7.5;
				params->coeff.QM_Omega_C -= params->coeff.QM_Omega[i] / 3.;
				params->coeff.QM_Chih[i] = -params->flatness[i] * params->eta
						* params->chi_Amp[i] * 3. / 2.;
#endif
			}
#ifdef _S1S2_
			params->coeff.S1S2_Chih[0] = spin_MPow2[1] / 2.;
			params->coeff.S1S2_Chih[1] = spin_MPow2[0] / 2.;
#if RENORM==0
			params->coeff.S1S2_Omega[0] = 721. * params->eta
					* params->chi_Amp[0] * params->chi_Amp[1] / 48.;
			params->coeff.S1S2_Omega[1] = -247. * params->coeff.S1S2_Omega[0]
					/ 721.;
			params->coeff.S1S2_MECO = -spin_MPow2[0] * spin_MPow2[1];
#else
			params->coeff.S1S2_Omega[0] = 79. * params->eta * params->chi_Amp[0] * params->chi_Amp[1] / 8.;
			params->coeff.S1S2_MECO = 2. * spin_MPow2[0] * spin_MPow2[1];
#endif
#endif
			params->coeff.MECO[LAL_PNORDER_TWO] *= (-81. + 57. * params->eta
					- etaPow2) / 24.;
#ifdef _QM_
			params->coeff.QM_MECO = 2. * params->eta;
#endif
		case LAL_PNORDER_ONE_POINT_FIVE:
			params->coeff.domega[LAL_PNORDER_ONE_POINT_FIVE] = 4. * LAL_PI;
			for (i = 0; i < 2; i++) {
				params->coeff.SO_Chih[i] = (4. + 3. * m_m[i]) * params->eta
						/ 2.;
				params->coeff.dLNh[i] = -spin_MPow2[i] / params->eta;
				params->coeff.SO_Omega[i] = -spin_MPow2[i] * (113. + 75.
						* m_m[i]) / 12.;
				params->coeff.SO_MECO[i] = -spin_MPow2[i] * 5. * params->eta
						* (4. + 3. * m_m[i]) / 9.;
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
	RETURN(status);
}

int LALSTPNQM_Derivator(REAL8 t, const REAL8 values[], REAL8 dvalues[], void * param) {
	LALSTPNQM_Waveform_Params *params = param;

	// variable declaration and initialization
	const REAL8 *chi_p[2] = { values + CHIH1_1, values + CHIH2_1 };
	static INT2 i, j, k; // indexes
	memset(dvalues, 0, NUM_OF_VAR * sizeof(REAL8));
	static REAL8 omegaPowi_3[8];
	omegaPowi_3[0] = 1.;
	omegaPowi_3[1] = cbrt(values[OMEGA]);
	for (i = 2; i < 8; i++) {
		omegaPowi_3[i] = omegaPowi_3[i - 1] * omegaPowi_3[1];
	}
	static REAL8 S1S2_Omega, SS_Omega, QM_Omega;
	S1S2_Omega = SS_Omega = QM_Omega = 0.;
	static REAL8 chih1chih2, chih1xchih2[2][3], LNhchih[2], LNhxchih[2][3];
	chih1chih2 = SCALAR_PRODUCT3(chi_p[0], chi_p[1]);
	for (i = 0; i < 2; i++) {
		LNhchih[i] = SCALAR_PRODUCT3(values + LNH_1, chi_p[i]);
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
#if RENORM==0
			// S1S2 for domega
			S1S2_Omega = params->coeff.S1S2_Omega[0] * LNhchih[0] * LNhchih[1]
					+ params->coeff.S1S2_Omega[1] * chih1chih2;
			// S1S2 for MECO
			dvalues[MECO] += params->coeff.S1S2_MECO * (chih1chih2 - 3
					* LNhchih[0] * LNhchih[1]) * omegaPowi_3[3];
#else
			S1S2_Omega = params->coeff.S1S2_Omega[0] * LNhchih[0] * LNhchih[1];
			dvalues[MECO] += params->coeff.S1S2_MECO * LNhchih[0] * LNhchih[1] * omegaPowi_3[3];
#endif
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
				k = (i + 1) % 2; // the opposite index
				// the 3*index is used, to acces the first spin, if index=0,
				// otherwise the second spin
#ifdef _S1S2_
				//S1S2 for dchih
				VECTOR_PRODUCT3W(chi_p[k], chi_p[i], params->coeff.S1S2_Chih[i], chih1xchih2[i]);
				VECTOR_PRODUCT3(values + LNH_1, chi_p[i], LNhxchih[i]);
				for (j = 0; j < 3; j++) {
					dvalues[CHIH1_1 + 3 * i + j] += (chih1xchih2[i][j] - 3.
							* params->coeff.S1S2_Chih[i] * LNhchih[k]
							* LNhxchih[i][j]) * omegaPowi_3[6];
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
				for (j = 0; j < 3; j++) {
					dvalues[CHIH1_1 + 3 * i + j] += params->coeff.QM_Chih[i]
							* LNhchih[i] * LNhxchih[i][j] * omegaPowi_3[6];
				}
#endif
			}
#ifdef _QM_
			dvalues[MECO] += params->coeff.QM_MECO * QM_Omega * omegaPowi_3[3];
#endif
			dvalues[OMEGA] += (QM_Omega + SS_Omega + S1S2_Omega)
					* omegaPowi_3[LAL_PNORDER_TWO];
		case LAL_PNORDER_ONE_POINT_FIVE:
			for (i = 0; i < 2; i++) {
				dvalues[OMEGA] += params ->coeff.SO_Omega[i] * LNhchih[i]
						* omegaPowi_3[LAL_PNORDER_ONE_POINT_FIVE];
				dvalues[MECO] += params->coeff.SO_MECO[i] * LNhchih[i]
						* omegaPowi_3[2];
			}
			for (i = 0; i < 3; i++) {
				dvalues[CHIH1_1 + i] += params->coeff.SO_Chih[0]
						* LNhxchih[0][i] * omegaPowi_3[5];
				dvalues[CHIH2_1 + i] += params->coeff.SO_Chih[1]
						* LNhxchih[1][i] * omegaPowi_3[5];
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

void LALSTPNQM_Generator(LALStatus *status, LALSTPNQM_Waveform_Params *params, CoherentGW *waveform) {
	INITSTATUS(status, "LALSTPNQM_Generator", WAVEFORMC);
	ATTATCHSTATUSPTR(status);

	// variable declaration and initialization
	UINT4 i = 0; // index
	REAL8 time = 0.;
	REAL8 LNhztol = 1.0e-8; // tolerancia LNhz-re
	REAL8 alpha, amp, temp1, temp2;
	const REAL8 geometrized_m_total = params->total_Mass * LAL_MTSUN_SI;
	const REAL8 freq_Step = geometrized_m_total * LAL_PI;
	const REAL8 step = params->sampling_Time / geometrized_m_total;
	REAL8 values[NUM_OF_VAR], dvalues[NUM_OF_VAR]; // + 1 for MECO
	LALSTPNQM_Integrator_System integrator;
	ERR_STR_END("runge_kutta_init");
	TRY(LALSTPNQM_Integrator_Init(status->statusPtr, NUM_OF_VAR - 1, params, LALSTPNQM_Derivator,
			&integrator), status);

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

	// filling the LALSTPNQM_Coefficients
	TRY(LALSTPNQM_Fill_Coefficients(status->statusPtr, params), status);
	LALSTPNQM_Derivator(time, values, dvalues, params);
	dvalues[MECO] = -1.; // to be able to start the loop
	i = 0;
	ERR_STR_END("while start")
	while (dvalues[MECO] < 0. && dvalues[OMEGA] > 0.0 && SQR(values[LNH_3])
			< 1. - LNhztol && values[OMEGA] / freq_Step < params->sampling_Freq
			/ 2.) {
		alpha = atan2(values[LNH_2], values[LNH_1]);
		amp = params->signal_Amp * pow(values[OMEGA], 2. / 3.);

		// writing the output
		if (waveform->h) {
			temp1 = -0.5 * amp * cos(2. * (values[PHASE] - params->phi))
					* (values[LNH_3] * values[LNH_3] + 1.);
			temp2 = amp * sin(2. * values[PHASE] - params->phi) * values[LNH_3];
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
		ERR_LINE;
		TRY(LALSTPNQM_Integrator_Func(status->statusPtr, &integrator, step, values), status);

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
		LALSTPNQM_Derivator(time, values, dvalues, params);
		ERR_LINE;
		if (i == waveform->f->data->length) {
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
	ERR_D(i);
	/// \bug there is double free or corruption if uncommented
	//LALSTPNQM_Integrator_Free(status->statusPtr, &integrator);
	CHECKSTATUSPTR(status);
	DETATCHSTATUSPTR(status);
	RETURN(status);
}