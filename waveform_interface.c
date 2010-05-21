/**
 * @file waveform_interface.c
 *		Containes the interfaces to the user and to the other part of the code.
 * @author László Veréb
 * @date 2010.05.21.
 * \todo LE KELL TISZTÍTANI!!!!!!!!!!!!!!!!!!!
 */

#include "waveform_interface.h"

REAL8 lapultsag[2];	///< \bug át kell rakni a paraméterekhez

NRCSID (WAVEFORM_INTERFACEC, "$Id$");

void interface(LALStatus *status, CoherentGW *waveform_out, InspiralTemplate *params, PPNParamStruc *ppnParams) {

	// variable declaration and initialization
	UINT4 count, i;
	REAL8 phiC;	///< phase at coalescence
	InspiralInit paramsInit;

	INITSTATUS(status, "interface", WAVEFORM_INTERFACEC);
	ATTATCHSTATUSPTR(status);
	ASSERT(params, status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	/* Compute some parameters*/
	LALInspiralInit(status->statusPtr, params, &paramsInit);
	CHECKSTATUSPTR(status);
	if (paramsInit.nbins == 0) {
		DETATCHSTATUSPTR(status);
		RETURN(status);
	}

	/* Allocate the waveform structures. */
	allocate_CoherentGW(status->statusPtr, paramsInit.nbins, waveform_out);
	CHECKSTATUSPTR(status);
	waveform_Params wave_Params;
	waveform wave;
	wave.length = waveform_out->f->data->length;
	wave.h = waveform_out->h;
	wave.a = waveform_out->a;
	wave.pol = waveform_out->shift->data;
	wave.freq = waveform_out->f->data;
	wave.phase = waveform_out->phi->data;

	// filling the parameters
	fill_Params(status->statusPtr, params, ppnParams, &wave_Params);
	CHECKSTATUSPTR(status);

	// calling the engine function
	generator(status->statusPtr, &wave_Params, &wave);
	CHECKSTATUSPTR(status);
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

void allocate_CoherentGW(LALStatus *status, UINT4 length, CoherentGW *wave) {
	INITSTATUS(status, "allocate_CoherentGW", WAVEFORM_INTERFACEC);
	ATTATCHSTATUSPTR(status);
	CreateVectorSequenceIn in;
	/* Make sure parameter and waveform structures exist. */
	ASSERT(wave, status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	/* Make sure waveform fields don't exist. */
	ASSERT(!(wave->a), status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	ASSERT(!(wave->f), status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	ASSERT(!(wave->phi), status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	ASSERT(!(wave->shift), status, LALINSPIRALH_ENULL, LALINSPIRALH_MSGENULL);
	if ((wave->h = (REAL4TimeVectorSeries *) LALMalloc(
			sizeof(REAL4TimeVectorSeries))) == NULL) {
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	if ((wave->a = (REAL4TimeVectorSeries *) LALMalloc(
			sizeof(REAL4TimeVectorSeries))) == NULL) {
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	if ((wave->f = (REAL4TimeSeries *) LALMalloc(
			sizeof(REAL4TimeSeries))) == NULL) {
		LALFree(wave->a);
		wave->a = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	if ((wave->phi = (REAL8TimeSeries *) LALMalloc(
			sizeof(REAL8TimeSeries))) == NULL) {
		LALFree(wave->a);
		wave->a = NULL;
		LALFree(wave->f);
		wave->f = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	if ((wave->shift = (REAL4TimeSeries *) LALMalloc(
			sizeof(REAL4TimeSeries))) == NULL) {
		LALFree(wave->a);
		wave->a = NULL;
		LALFree(wave->f);
		wave->f = NULL;
		LALFree(wave->phi);
		wave->phi = NULL;
		ABORT(status, LALINSPIRALH_EMEM, LALINSPIRALH_MSGEMEM);
	}
	in.length = length;
	in.vectorLength = 2;
	LALSCreateVectorSequence(status->statusPtr, &(wave->h->data), &in);
	CHECKSTATUSPTR(status);
	LALSCreateVectorSequence(status->statusPtr, &(wave->a->data), &in);
	CHECKSTATUSPTR(status);
	LALSCreateVector(status->statusPtr, &(wave->f->data), in.length);
	CHECKSTATUSPTR(status);
	LALDCreateVector(status->statusPtr, &(wave->phi->data), in.length);
	CHECKSTATUSPTR(status);
	LALSCreateVector(status->statusPtr, &(wave->shift->data), in.length);
	CHECKSTATUSPTR(status);
	DETATCHSTATUSPTR(status);
	RETURN(status);
}

void fill_Params(LALStatus *status, InspiralTemplate *params, PPNParamStruc *ppnParams, waveform_Params *wave) {
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
