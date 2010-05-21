/*
 * waveform_interface.c
 *
 *  Created on: 2010.05.07.
 *      Author: vereb
 */

#include "waveform_interface.h"

REAL8 lapultsag[2];

NRCSID (WAVEFORM_INTERFACEC, "$Id$");

void interface(LALStatus *status, CoherentGW *waveform_out,
		InspiralTemplate *params, PPNParamStruc *ppnParams) {
	UINT4 count, i;
	REAL8 phiC;/* phase at coalescence */
	InspiralInit paramsInit;

	CreateVectorSequenceIn in;

	INITSTATUS(status, "interface", WAVEFORM_INTERFACEC);
	ATTATCHSTATUSPTR(status);

	/* Make sure parameter and waveform structures exist. */
	ASSERT(params, status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	ASSERT(waveform_out, status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	/* Make sure waveform fields don't exist. */
	ASSERT(!(waveform_out->a), status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	ASSERT(!(waveform_out->f), status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	ASSERT(!(waveform_out->phi), status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	ASSERT(!(waveform_out->shift), status, LALINSPIRALH_ENULL,
			LALINSPIRALH_MSGENULL);

	/* Compute some parameters*/
	LALInspiralInit(status->statusPtr, params, &paramsInit);
	CHECKSTATUSPTR(status);
	if (paramsInit.nbins == 0) {
		DETATCHSTATUSPTR(status);
		RETURN(status);
	}

	/* Allocate the waveform structures. */
	if ((waveform_out->h = (REAL4TimeVectorSeries *) LALMalloc(
			sizeof(REAL4TimeVectorSeries))) == NULL) {
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	memset(waveform_out->h, 0, sizeof(REAL4TimeVectorSeries));
	if ((waveform_out->a = (REAL4TimeVectorSeries *) LALMalloc(
			sizeof(REAL4TimeVectorSeries))) == NULL) {
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	memset(waveform_out->a, 0, sizeof(REAL4TimeVectorSeries));

	if ((waveform_out->f = (REAL4TimeSeries *) LALMalloc(
			sizeof(REAL4TimeSeries))) == NULL) {
		LALFree(waveform_out->a);
		waveform_out->a = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	memset(waveform_out->f, 0, sizeof(REAL4TimeSeries));

	if ((waveform_out->phi = (REAL8TimeSeries *) LALMalloc(
			sizeof(REAL8TimeSeries))) == NULL) {
		LALFree(waveform_out->a);
		waveform_out->a = NULL;
		LALFree(waveform_out->f);
		waveform_out->f = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	memset(waveform_out->phi, 0, sizeof(REAL8TimeSeries));

	if ((waveform_out->shift = (REAL4TimeSeries *) LALMalloc(
			sizeof(REAL4TimeSeries))) == NULL) {
		LALFree(waveform_out->a);
		waveform_out->a = NULL;
		LALFree(waveform_out->f);
		waveform_out->f = NULL;
		LALFree(waveform_out->phi);
		waveform_out->phi = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	memset(waveform_out->shift, 0, sizeof(REAL4TimeSeries));

	in.length = (UINT4) paramsInit.nbins;
	in.vectorLength = 2;
	//REAL4VectorSequence *proba;
	//LALSCreateVectorSequence(status->statusPtr, &(proba), &in);
	//CHECKSTATUSPTR(status);
	LALSCreateVectorSequence(status->statusPtr, &(waveform_out->h->data), &in);
	CHECKSTATUSPTR(status);
	LALSCreateVectorSequence(status->statusPtr, &(waveform_out->a->data), &in);
	CHECKSTATUSPTR(status);
	LALSCreateVector(status->statusPtr, &(waveform_out->f->data), in.length);
	CHECKSTATUSPTR(status);
	LALDCreateVector(status->statusPtr, &(waveform_out->phi->data), in.length);
	CHECKSTATUSPTR(status);
	LALSCreateVector(status->statusPtr, &(waveform_out->shift->data), in.length);
	CHECKSTATUSPTR(status);
	waveform_Params wave_Params;
	waveform wave;
	wave.length = in.length;
	wave.h = waveform_out->h;
	wave.a = waveform_out->a;
	wave.pol = waveform_out->shift->data;
	wave.freq = waveform_out->f->data;
	wave.phase = waveform_out->phi->data;
	/* Call the engine function */
	fill_Params(status->statusPtr, params, ppnParams, &wave_Params);
	CHECKSTATUSPTR(status);
#if DEBUG==1
	fprintf(stderr, "generator_start\n");
	fflush(stderr);
#endif
	generator(status->statusPtr, &wave_Params, &wave);
	wave.h->data->vectorLength = wave.a->data->vectorLength = wave.length;
	wave.pol->length = wave.freq->length = wave.phase->length = wave.length;
	CHECKSTATUSPTR(status);
#if DEBUG==1
	fprintf(stderr, "generator_end\n");
	fflush(stderr);
#endif
	count = wave.length;
	/* Check an empty waveform hasn't been returned */
	for (i = 0; i < wave.length; i++) {
		if (wave.phase->data[i] != 0.0)
			break;
	}

	{
		phiC = wave.phase->data[wave.length - 1];

		for (i = 0; i < wave.length; i++) {
			wave.phase->data[i] = -phiC + wave.phase->data[i] + ppnParams->phi;
		}
		waveform_out->a->deltaT = waveform_out->f->deltaT
				= waveform_out->phi->deltaT = waveform_out->shift->deltaT = 1.
						/ params->tSampling;

		waveform_out->a->sampleUnits = lalStrainUnit;
		waveform_out->f->sampleUnits = lalHertzUnit;
		waveform_out->phi->sampleUnits = lalDimensionlessUnit;
		waveform_out->shift->sampleUnits = lalDimensionlessUnit;

		waveform_out->position = ppnParams->position;
		waveform_out->psi = ppnParams->psi;

		snprintf(waveform_out->a->name, LALNameLength,
				"STPN inspiral amplitudes");
		snprintf(waveform_out->f->name, LALNameLength,
				"STPN inspiral frequency");
		snprintf(waveform_out->phi->name, LALNameLength, "STPN inspiral phase");
		snprintf(waveform_out->shift->name, LALNameLength,
				"STPN inspiral polshift");

		/* --- fill some output ---*/
		ppnParams->tc = (REAL8) (count - 1) / params->tSampling;
		ppnParams->length = count;
		ppnParams->dfdt = ((REAL4) (waveform_out->f->data->data[count - 1]
				- waveform_out->f->data->data[count - 2])) * ppnParams->deltaT;
		ppnParams->fStop = params->fFinal;
		ppnParams->termCode = GENERATEPPNINSPIRALH_EFSTOP;
		ppnParams->termDescription = GENERATEPPNINSPIRALH_MSGEFSTOP;

		ppnParams->fStart = ppnParams->fStartIn;
	} /* end phase condition*/
	DETATCHSTATUSPTR(status);
	RETURN(status);
}

void fill_Params(LALStatus *status, InspiralTemplate *params,
		PPNParamStruc *ppnParams, waveform_Params *wave) {
	INITSTATUS(status, "fill_Params", WAVEFORM_INTERFACEC);
	ATTATCHSTATUSPTR(status);
	wave->mass[0] = params->mass1;
	wave->mass[1] = params->mass2;
	wave->total_Mass = wave->mass[0] + wave->mass[1];
	wave->mu = wave->mass[0] * wave->mass[1] / wave->total_Mass;
	wave->eta = wave->mu / wave->total_Mass;
	wave->chirp_Mass = wave->total_Mass * pow(wave->eta, 3. / 5.);
	wave->chi_Amp[0] = wave->chi_Amp[1] = 0.;
	INT2 i;
	for (i = 0; i < 3; i++) {
		wave->chi[0][i] = params->spin1[i];
		wave->chi[1][i] = params->spin2[i];
		wave->chi_Amp[0] += SQR(wave->chi[0][i]);
		wave->chi_Amp[1] += SQR(wave->chi[1][i]);
	}
	wave->chi_Amp[0] = sqrt(wave->chi_Amp[0]);
	wave->chi_Amp[1] = sqrt(wave->chi_Amp[1]);
	for (i = 0; i < 3; i++) {
		wave->chih[0][i] = wave->chi[0][i] / wave->chi_Amp[0];
		wave->chih[1][i] = wave->chi[1][i] / wave->chi_Amp[1];
	}
	wave->flatness[0] = lapultsag[0];
	wave->flatness[1] = lapultsag[1];
	wave->distance = params->distance;
	wave->inclination = params->inclination;
	wave->phi = 0.;
	wave->signal_Amp = params->signalAmplitude * LAL_PC_SI * 1e6;
	wave->lower_Freq = params->fLower;
	wave->order = params->order;
	wave->sampling_Time = ppnParams->deltaT;
	wave->sampling_Freq = 1. / wave->sampling_Time;
	DETATCHSTATUSPTR(status);
	RETURN(status);
}
