/**
 * @file waveform_interface.h
 *		Containes the interface function declarations to the user and to the other part of the code.
 * @author László Veréb
 * @date 2010.05.21.
 */

#ifndef WAVEFORM_INTERFACE_H
#define WAVEFORM_INTERFACE_H

#include <lal/Units.h>
#include <lal/LALInspiral.h>
#include <lal/SeqFactories.h>

#include "waveform.h"
#include "util_debug.h"

NRCSID (WAVEFORM_INTERFACEH, "$Id$");

/**		The function provides interface to the other parts of the code.
 * @param[in,out]	status	: LAL universal status structure
 * @param[out]	wave_out	: structure containing the waveform
 * @param[in]		params	: structure containing the inspiral parameters
 * @param[in]	ppnParams	: \test esetleg ki is lehetne szedni
 */
void interface(LALStatus *status, CoherentGW *wave_out,
		InspiralTemplate *params, PPNParamStruc *ppnParams);

/**		The function calculates the used parameters from the inspiral
 * parameters.
 * @param[in,out]	status	: LAL universal status structure
 * @param[in]		params	: the inspiral parameters
 * @param[in]	ppnParams	: \test esetleg ki is lehetne szedni
 * @param[out]	wave		: the used parameters
 */
void fill_Params(LALStatus *status, InspiralTemplate *params,
		PPNParamStruc *ppnParams, waveform_Params *wave);

/**		The function allocates memory for the waveform.
 * @param[in,out]	status	: LAL universal status structure
 * @param[in]		length	: the length of the waveform
 * @param[out]		waveform: pointer to the allocated waveform
 */
void allocate_CoherentGW(LALStatus *status, UINT4 length, CoherentGW *waveform);

/**		The function allocates timeseries structures for the waveform.
 * @param[in,out]	status	: LAL universal status structure
 * @param[in]		mode	: if 1 allocates memory for the h+,hx; if 2 then for
 * the a+,ax; and if 3 then for both
 * @param[out]		wave	: pointer to the wavformstructure
 */
void
choose_CoherentGW_Component(LALStatus *status, INT2 mode, CoherentGW *wave);

#endif /* WAVEFOMR_INTERFACE_H */
