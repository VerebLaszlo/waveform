/**
 * @file LALSQTPNWaveformInterface.c
 *		Contains the interfaces to the user and to the other part of the code.
 * @author László Veréb
 * @date 2010.05.21.
 */

#include "LALSQTPNWaveformInterface.h"
#include "LALSQTPNWaveform.h"

REAL8 lapultsag[2]; ///< \bug át kell rakni a paraméterekhez
LALSQTPNChooseSpinComponent spin_Component;///< \bug át kell rakni a paraméterekhez

NRCSID (LALSQTPNWAVEFORMINTERFACEC, "$Id LALSQTPNWaveformInterface.c$");

/**		The macro function returns the square of the argument.
 * Do not use with incrementing or decrementing operators!
 * @param[in] a	 : the number
 * @return the square of the number
 */
#define SQR(a) ((a)*(a))

void interface(LALStatus *status, CoherentGW * thewaveform, SimInspiralTable *injParams, PPNParamStruc *ppnParams) {
	INITSTATUS(status, "interface", LALSQTPNWAVEFORMINTERFACEC);
	ATTATCHSTATUSPTR(status);
	ASSERT(injParams, status, GENERATEINSPIRALH_ENULL, GENERATEINSPIRALH_MSGENULL);
	InspiralTemplate inspiralParams; // structure for inspiral package
	CHAR              warnMsg[1024];
	TRY(LALGetApproximantFromString(status->statusPtr, injParams->waveform, &inspiralParams.approximant), status);
	TRY(LALGetOrderFromString(status->statusPtr, injParams->waveform, &inspiralParams.order), status);
	/* We fill ppnParams */
	TRY(LALGenerateInspiralPopulatePPN(status->statusPtr, ppnParams, injParams), status);
	/* we fill inspiralParams structure as well.*/
	TRY(LALGenerateInspiralPopulateInspiral(status->statusPtr, &inspiralParams, injParams, ppnParams), status);
	/* the waveform generation itself */
	TRY(LALSQTPNWaveformForInjection(status->statusPtr, thewaveform, &inspiralParams, ppnParams), status);
	/* we populate the simInspiral table with the fFinal needed for
	 template normalisation. */
	injParams->f_final = inspiralParams.fFinal;/* If no waveform has been generated. (AmpCorPPN fills waveform.h) */
	if ( thewaveform->a == NULL && inspiralParams.approximant != AmpCorPPN ) {
		snprintf( warnMsg, sizeof(warnMsg)/sizeof(*warnMsg),
				"No waveform generated (check lower frequency)\n");
		LALInfo( status, warnMsg );
		ABORT( status, LALINSPIRALH_ENOWAVEFORM, LALINSPIRALH_MSGENOWAVEFORM );
	}
	/* If sampling problem. (AmpCorPPN may not be compatible) */
	if ( ppnParams->dfdt > 2.0 && inspiralParams.approximant != AmpCorPPN ) {
		snprintf( warnMsg, sizeof(warnMsg)/sizeof(*warnMsg),
				"Waveform sampling interval is too large:\n"
				"\tmaximum df*dt = %f", ppnParams->dfdt );
		LALInfo( status, warnMsg );
		ABORT( status, GENERATEINSPIRALH_EDFDT, GENERATEINSPIRALH_MSGEDFDT );
	}

	/* Some info should add everything (spin and so on) */
	snprintf( warnMsg, sizeof(warnMsg)/sizeof(*warnMsg),
		"Injected waveform parameters:\n"
		"ppnParams->mTot\t= %"LAL_REAL4_FORMAT"\n"
		"ppnParams->eta\t= %"LAL_REAL4_FORMAT"\n"
		"ppnParams->d\t= %"LAL_REAL4_FORMAT"\n"
		"ppnParams->inc\t= %"LAL_REAL4_FORMAT"\n"
		"ppnParams->phi\t= %"LAL_REAL4_FORMAT"\n"
		"ppnParams->psi\t= %"LAL_REAL4_FORMAT"\n"
		"ppnParams->fStartIn\t= %"LAL_REAL4_FORMAT"\n"
		"ppnParams->fStopIn\t= %"LAL_REAL4_FORMAT"\n"
		"ppnParams->position.longitude\t= %"LAL_REAL8_FORMAT"\n"
		"ppnParams->position.latitude\t= %"LAL_REAL8_FORMAT"\n"
		"ppnParams->position.system\t= %d\n"
		"ppnParams->epoch.gpsSeconds\t= %"LAL_INT4_FORMAT"\n"
		"ppnParams->epoch.gpsNanoSeconds\t= %"LAL_INT4_FORMAT"\n"
		"ppnParams->tC\t= %"LAL_REAL8_FORMAT"\n"
		"ppnParams->dfdt\t =%"LAL_REAL4_FORMAT"\n",
		ppnParams->mTot,
		ppnParams->eta,
		ppnParams->d,
		ppnParams->inc,
		ppnParams->phi,
		ppnParams->psi,
		ppnParams->fStartIn,
		ppnParams->fStopIn,
		ppnParams->position.longitude,
		ppnParams->position.latitude,
		ppnParams->position.system,
		ppnParams->epoch.gpsSeconds,
		ppnParams->epoch.gpsNanoSeconds,
		ppnParams->tc,
		ppnParams->dfdt );
	LALInfo( status, warnMsg );

	DETATCHSTATUSPTR(status);
	RETURN(status);
}

void LALSQTPNWaveformForInjection(LALStatus *status, CoherentGW *waveform,
		InspiralTemplate *params, PPNParamStruc *ppnParams) {

	// variable declaration and initialization
	UINT4 count, i;
	REAL8 phiC; // phase at coalescence
	InspiralInit paramsInit;

	INITSTATUS(status, "LALSQTPNInterface", LALSQTPNWAVEFORMINTERFACEC);
	ATTATCHSTATUSPTR(status);
	ASSERT(params, status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	ASSERT(waveform != NULL, status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	// Compute some parameters
	TRY(LALInspiralInit(status->statusPtr, params, &paramsInit), status);
	if (paramsInit.nbins == 0) {
		DETATCHSTATUSPTR(status);
		RETURN(status);
	}

	// Allocate the waveform structures.
	TRY(LALSQTPNAllocateCoherentGW(status->statusPtr, waveform, paramsInit.nbins), status);
	LALSQTPNWaveformParams wave_Params;

	// filling the parameters
	LALSQTPNFillParams(status->statusPtr, &wave_Params, params, ppnParams);
	if (status->statusCode) {
		LALSQTPNDestroyCoherentGW(status->statusPtr, waveform);
		ABORT(status, status->statusCode, status->statusDescription);
	}

	// calling the engine function
	LALSQTPNGenerator(status->statusPtr, waveform, &wave_Params);
	if (status->statusCode) {
		LALSQTPNDestroyCoherentGW(status->statusPtr, waveform);
		ABORT(status, status->statusCode, status->statusDescription);
	}
	count = waveform->f->data->length;
	/* Check an empty waveform hasn't been returned */
	/*for (i = 0; i < waveform->f->data->length; i++) {
	 if (waveform->phi->data->data[i] != 0.0)
	 break;
	 }*/

	{
		if (waveform->a != NULL) {
			/// \bug meg kell nézi, hogy ez miért nem kell!!!!
			/*phiC = waveform->phi->data->data[waveform->f->data->length - 1];

			for (i = 0; i < waveform->f->data->length; i++) {
				waveform->phi->data->data[i] = -phiC
						+ waveform->phi->data->data[i] + ppnParams->phi;
			}*/
			waveform->a->deltaT = waveform->f->deltaT = waveform->phi->deltaT
					= waveform->shift->deltaT = 1. / params->tSampling;

			waveform->a->sampleUnits = lalStrainUnit;
			waveform->f->sampleUnits = lalHertzUnit;
			waveform->phi->sampleUnits = lalDimensionlessUnit;
			waveform->shift->sampleUnits = lalDimensionlessUnit;

			waveform->position = ppnParams->position;
			waveform->psi = ppnParams->psi;

			snprintf(waveform->a->name, LALNameLength,
					"STPN inspiral amplitudes");
			snprintf(waveform->f->name, LALNameLength,
					"STPN inspiral frequency");
			snprintf(waveform->phi->name, LALNameLength, "STPN inspiral phase");
			snprintf(waveform->shift->name, LALNameLength,
					"STPN inspiral polshift");
		}
		// --- fill some output ---
		ppnParams->tc = (REAL8) (count - 1) / params->tSampling;
		ppnParams->length = count;
		ppnParams->dfdt = ((REAL4) (waveform->f->data->data[count - 1]
				- waveform->f->data->data[count - 2])) * ppnParams->deltaT;
		ppnParams->fStop = params->fFinal;
		ppnParams->termCode = GENERATEPPNINSPIRALH_EFSTOP;
		ppnParams->termDescription = GENERATEPPNINSPIRALH_MSGEFSTOP;

		ppnParams->fStart = ppnParams->fStartIn;
	} /* end phase condition*/
	DETATCHSTATUSPTR(status);
	RETURN(status);
}

void LALSQTPNAllocateCoherentGW(LALStatus *status, CoherentGW *wave, UINT4 length) {
	INITSTATUS(status, "LALSQTPNAllocateCoherentGW", LALSQTPNWAVEFORMINTERFACEC);
	ATTATCHSTATUSPTR(status);
	CreateVectorSequenceIn in;
	/* Make sure parameter and waveform structures exist. */
	ASSERT(wave, status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);

	in.length = length;
	in.vectorLength = 2;

	/*
		if ((wave->h = (REAL4TimeVectorSeries *)
				LALMalloc(sizeof(REAL4TimeVectorSeries))) == NULL) {
			LALFree(wave->f);
			wave->f = NULL;
			ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
		}
		memset(wave->h, 0, sizeof(REAL4TimeVectorSeries));
		LALSCreateVectorSequence(status->statusPtr, &(wave->h->data), &in);
		if (wave->h->data == NULL) {
			LALFree(wave->f);
			wave->f = NULL;
			LALFree(wave->h);
			wave->h = NULL;
			ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
		}
	*/
	// allocating the frequency array
	if ((wave->f = (REAL4TimeSeries *) LALMalloc(
			sizeof(REAL4TimeSeries))) == NULL) {
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	memset(wave->f, 0, sizeof(REAL4TimeSeries));
	LALSCreateVector(status->statusPtr, &(wave->f->data), in.length);
	if (wave->f->data == NULL) {
		LALFree(wave->f);
		wave->f = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}

	// allocating the amplitude array
	if ((wave->a = (REAL4TimeVectorSeries *)
			LALMalloc(sizeof(REAL4TimeVectorSeries))) == NULL) {
		TRY(LALSDestroyVector(status->statusPtr, &(wave->f->data)), status);
		LALFree(wave->f);
		wave->f = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	memset(wave->a, 0, sizeof(REAL4TimeVectorSeries));
	LALSCreateVectorSequence(status->statusPtr, &(wave->a->data), &in);
	if(wave->a->data == NULL) {
		TRY(LALSDestroyVector(status->statusPtr, &(wave->f->data)), status);
		LALFree(wave->f);
		wave->f = NULL;
		LALFree(wave->a);
		wave->a = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}

	// allocating the phase array
	if ((wave->phi = (REAL8TimeSeries *) LALMalloc(
			sizeof(REAL8TimeSeries))) == NULL) {
		TRY(LALSDestroyVector(status->statusPtr, &(wave->f->data)), status);
		LALFree(wave->f);
		wave->f = NULL;
		TRY(LALSDestroyVectorSequence(status->statusPtr, &(wave->a->data)), status);
		LALFree(wave->a);
		wave->a = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	memset(wave->phi, 0, sizeof(REAL4TimeSeries));
	LALDCreateVector(status->statusPtr, &(wave->phi->data), in.length);
	if(wave->phi->data == NULL) {
		TRY(LALSDestroyVector(status->statusPtr, &(wave->f->data)), status);
		LALFree(wave->f);
		wave->f = NULL;
		TRY(LALSDestroyVectorSequence(status->statusPtr, &(wave->a->data)), status);
		LALFree(wave->a);
		wave->a = NULL;
		LALFree(wave->phi);
		wave->phi = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}

	// allocating the polarisation shift array
	if ((wave->shift = (REAL4TimeSeries *) LALMalloc(
			sizeof(REAL4TimeSeries))) == NULL) {
		TRY(LALSDestroyVector(status->statusPtr, &(wave->f->data)), status);
		LALFree(wave->f);
		wave->f = NULL;
		TRY(LALSDestroyVectorSequence(status->statusPtr, &(wave->a->data)), status);
		LALFree(wave->a);
		wave->a = NULL;
		TRY(LALDDestroyVector(status->statusPtr, &(wave->phi->data)), status);
		LALFree(wave->phi);
		wave->phi = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	memset(wave->shift, 0, sizeof(REAL4TimeSeries));
	LALSCreateVector(status->statusPtr, &(wave->shift->data), in.length);
	if (wave->shift->data == NULL) {
		TRY(LALSDestroyVector(status->statusPtr, &(wave->f->data)), status);
		LALFree(wave->f);
		wave->f = NULL;
		TRY(LALSDestroyVectorSequence(status->statusPtr, &(wave->a->data)), status);
		LALFree(wave->a);
		wave->a = NULL;
		TRY(LALDDestroyVector(status->statusPtr, &(wave->phi->data)), status);
		LALFree(wave->phi);
		wave->phi = NULL;
		LALFree(wave->shift);
		wave->shift = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	DETATCHSTATUSPTR(status);
	RETURN(status);
}

void LALSQTPNDestroyCoherentGW(LALStatus *status, CoherentGW *wave) {
	INITSTATUS(status, "LALSQTPNDestroyCoherentGW", LALSQTPNWAVEFORMINTERFACEC);
	ATTATCHSTATUSPTR(status);
	TRY(LALSDestroyVector(status->statusPtr, &(wave->f->data)), status);
	LALFree(wave->f);
	if(wave->a != NULL) {
		TRY(LALSDestroyVectorSequence(status->statusPtr, &(wave->a->data)), status);
		LALFree(wave->a);
		TRY(LALSDestroyVector(status->statusPtr, &(wave->shift->data)), status);
		LALFree(wave->shift);
		TRY(LALDDestroyVector(status->statusPtr, &(wave->phi->data)), status);
		LALFree(wave->phi);
	}
	if (wave->h != NULL) {
		TRY(LALSDestroyVectorSequence(status->statusPtr, &(wave->h->data)), status);
		LALFree(wave->h);
	}
	DETATCHSTATUSPTR(status);
	RETURN(status);
}

void LALSQTPNFillParams(LALStatus *status, LALSQTPNWaveformParams *wave,
		InspiralTemplate *params, PPNParamStruc *ppnParams) {
	INITSTATUS(status, "LALSQTPNFillParams", LALSQTPNWAVEFORMINTERFACEC);
	wave->mass[0] = params->mass1;
	wave->mass[1] = params->mass2;
	wave->totalMass = wave->mass[0] + wave->mass[1];
	wave->mu = wave->mass[0] * wave->mass[1] / wave->totalMass;
	wave->eta = wave->mu / wave->totalMass;
	wave->chirpMass = wave->totalMass * pow(wave->eta, 3. / 5.);
	wave->chiAmp[0] = wave->chiAmp[1] = 0.;
	INT2 i;
	for (i = 0; i < 3; i++) {
		wave->chi[0][i] = params->spin1[i];
		wave->chi[1][i] = params->spin2[i];
		wave->chiAmp[0] += SQR(wave->chi[0][i]);
		wave->chiAmp[1] += SQR(wave->chi[1][i]);
	}
	wave->chiAmp[0] = sqrt(wave->chiAmp[0]);
	wave->chiAmp[1] = sqrt(wave->chiAmp[1]);
	for (i = 0; i < 3; i++) {
		if (wave->chiAmp[0] != 0.) {
			wave->chih[0][i] = wave->chi[0][i] / wave->chiAmp[0];
		} else {
			wave->chih[0][i] = 0.;
		}
		if (wave->chiAmp[1] != 0.) {
			wave->chih[1][i] = wave->chi[1][i] / wave->chiAmp[1];
		} else {
			wave->chih[1][i] = 0.;
		}
	}
	wave->flatness[0] = lapultsag[0];
	wave->flatness[1] = lapultsag[1];
	wave->distance = params->distance;
	wave->inclination = params->inclination;
	wave->phi = 0.;
	wave->signalAmp = params->signalAmplitude * LAL_PC_SI * 1e6;
	wave->lowerFreq = params->fLower;
	wave->order = params->order;
	if (spin_Component != 0)
		spin_Component |= LALSQTPN_SOComp;
	wave->spinComponent = spin_Component;
	wave->samplingTime = ppnParams->deltaT;
	wave->samplingFreq = 1. / wave->samplingTime;
	RETURN(status);
}
