#ifndef WAVEFORM_INTERFACE_H_
#define WAVEFORM_INTERFACE_H_

#include <lal/Units.h>
#include <lal/LALInspiral.h>
#include <lal/SeqFactories.h>

#include "waveform.h"
#include "util_debug.h"

/**
 * \todo írni
 */
void interface(LALStatus *status, CoherentGW *waveform_out,
		InspiralTemplate *params, PPNParamStruc *ppnParams);

/**
 * \todo írni
 */
void fill_Params(LALStatus *status, InspiralTemplate *params,
		PPNParamStruc *ppnParams, waveform_Params *wave);

#endif
