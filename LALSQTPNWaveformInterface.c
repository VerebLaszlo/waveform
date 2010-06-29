/**
 * @file LALSQTPNWaveformInterface.c
 *		Contains function definitions to integrate the SpinQuadTaylor code into the other parts of the LALSuit.
 *	If you want to run the program use the LALSQTPNWaveformTest.c file int the
 *	test directory.
 * @author László Veréb
 * @date 2010.06.27.
 */

#include <lal/LALSQTPNWaveformInterface.h>
#include <lal/LALSQTPNWaveform.h>

NRCSID (LALSQTPNWAVEFORMINTERFACEC, "$Id LALSQTPNWaveformInterface.c$");

/**		The macro function returns the square of the argument.
 * Do not use with incrementing or decrementing operators!
 * @param[in] a	 : the number
 * @return the square of the number
 */
#define SQR(a) ((a)*(a))

void LALSQTPNWaveformForInjection(LALStatus *status, CoherentGW *waveform,
		InspiralTemplate *params, PPNParamStruc *ppnParams) {

	// variable declaration and initialization
	UINT4 count, i;
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
	BEGINFAIL(status) {
		LALSQTPNDestroyCoherentGW(status->statusPtr, waveform);
	} ENDFAIL(status);

	// calling the engine function
	LALSQTPNGenerator(status->statusPtr, waveform, &wave_Params);
	BEGINFAIL(status) {
		LALSQTPNDestroyCoherentGW(status->statusPtr, waveform);
	} ENDFAIL(status);
	count = waveform->f->data->length;

	{
		if (waveform->a != NULL) {
			for (i = 0; i < waveform->f->data->length; i++) {
				// (PPNParamStruct)ppnParams->phi === (InspiralTemplate)params->startPhase === (SimInspiralTable)injparams/this_event->coa_phase it is set to 0 in LALSQTPNWaveformTest.c at line 83.
				waveform->phi->data->data[i] = waveform->phi->data->data[i] + ppnParams->phi - waveform->phi->data->data[waveform->f->data->length-1];
			}
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
	ASSERT(wave != NULL, status, LALSQTPN_ENULL, LALSQTPN_MSGWAVESTRUCTNULL);
	ASSERT(length > 0, status, LALSQTPN_ZEROLENGTH, LALSQTPN_MSGZEROLENGTH);
	ATTATCHSTATUSPTR(status);
	CreateVectorSequenceIn in;
	/* Make sure parameter and waveform structures exist. */

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
	ASSERT(wave != NULL, status, LALSQTPN_ENULL, LALSQTPN_MSGWAVESTRUCTNULL);
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
	ASSERT(wave != NULL, status, LALSQTPN_ENULL, LALSQTPN_MSGWAVEPARAMSNULL);
	ASSERT(params != NULL, status, LALSQTPN_ENULL, LALSQTPN_MSGINSPIRALTEMPLATE);
	ASSERT(ppnParams != NULL, status, LALSQTPN_ENULL, LALSQTPN_MSGPPNPARAMS);
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
	wave->flatness[0] = params->flatness[0];
	wave->flatness[1] = params->flatness[1];
	wave->distance = params->distance;
	wave->inclination = params->inclination;
	wave->lowerFreq = params->fLower;
	wave->samplingTime = ppnParams->deltaT;
	wave->samplingFreq = 1. / wave->samplingTime;
	wave->phi = 0.;
	wave->signalAmp = 4. * wave->totalMass * wave->eta * LAL_MRSUN_SI / wave->distance;
	wave->order = params->order;
	wave->spinComponent = *((LALSQTPNChooseSpinComponent*)(params->settings));
	if (wave->spinComponent != 0)
		wave->spinComponent |= LALSQTPN_SOComp;
	RETURN(status);
}
