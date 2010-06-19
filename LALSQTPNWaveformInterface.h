/**
 * @file LALSQTPNWaveformInterface.h
 *		Contains the interfaces to the user and to the other parts of the code.
 * @author László Veréb
 * @date 2010.05.21.
 */

#ifndef LALSQTPNWAVEFORMINTERFACE_H
#define LALSQTPNWAVEFORMINTERFACE_H

#include <lal/Units.h>
#include <lal/LALInspiral.h>
#include <lal/SeqFactories.h>
#include <lal/LALStatusMacros.h>
#include <lal/GenerateInspiral.h>

#include "util_debug.h"
#include "LALSQTPNWaveform.h"

#ifdef __cplusplus
extern "C" {
#endif

NRCSID (LALSQTPNWAVEFORMINTERFACEH, "$Id LALSQTPNWaveformInterface.h$");

/**		Ez nem a programunk része, csak összeköti a teszt-programot a mienkkel.
 */
void interface(LALStatus *status, CoherentGW * thewaveform, SimInspiralTable *injParams, PPNParamStruc *ppnParams);

/**		The function provides interface to the other parts of the code.
 * @param[in,out]	status	: LAL universal status structure
 * @param[out]	wave_out	: structure containing the waveform
 * @param[in]	params		: structure containing the inspiral parameters
 * @param[in]	ppnParams	: \test esetleg ki is lehetne szedni
 */
void LALSQTPNWaveformForInjection(LALStatus *status, CoherentGW *wave_out,
		InspiralTemplate *params, PPNParamStruc *ppnParams);

/**		The function allocates memory for the waveform.
 * @param[in,out]	status		: LAL universal status structure
 * @param[out]		waveform	: pointer to the allocated waveform
 * @param[in]		length		: the length of the waveform
 */
void LALSQTPNAllocateCoherentGW(LALStatus *status, CoherentGW *waveform, UINT4 length);

/**		The function deallocates memory of the waveform.
 * @param[in,out]	status		: LAL universal status structure
 * @param[out]		waveform	: pointer to the allocated waveform
 */
void LALSQTPNDestroyCoherentGW(LALStatus *status, CoherentGW *waveform);

/**		The function calculates the used parameters from the inspiral
 * parameters.
 * @param[in,out]	status	: LAL universal status structure
 * @param[out]	wave		: the used parameters
 * @param[in]		params	: the inspiral parameters
 * @param[in]	ppnParams	: \test esetleg ki is lehetne szedni
 */
void LALSQTPNFillParams(LALStatus *status, LALSQTPNWaveformParams *wave,
		InspiralTemplate *params, PPNParamStruc *ppnParams);

#ifdef __cplusplus
}
#endif

#endif /* LALSQTPNWAVEFOMRINTERFACE_H */
