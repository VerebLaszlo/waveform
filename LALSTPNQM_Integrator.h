/**
 * @file LALSTPNQM_Integrator.h
 *		Contains the function declaration and structures needed by the
 *		integral function.
 * @author László Veréb
 * @date 2010.05.21.
 */

#ifndef LALSTPNQM_INTEGRATOR_H
#define LALSTPNQM_INTEGRATOR_H

#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_odeiv.h>

#include <lal/LALInspiral.h>
#include <lal/LALGSL.h>
#include <lal/LALStatusMacros.h>

NRCSID (INTEGRATORH, "$Id LALSTPNQM_Integrator.h$");

/**		The structure contains the integration method.
 */
typedef struct {
	const gsl_odeiv_step_type* solver_type;
	gsl_odeiv_step* solver_step;
	gsl_odeiv_control* solver_control;
	gsl_odeiv_evolve* solver_evolve;
	gsl_odeiv_system solver_system;
} LALSTPNQM_Integrator_System;

/**		The function initialize the integration method.
 * @param[in,out]	status	: The LAL universal status structure
 * @param[in]		num		: the number of the dynamic variables
 * @param[in]		params	: the parameters used in the derivative function
 * @param[in]	derivator	: pointer to the derivative function
 * @param[out]	integrator	: the structure containing the integration method
 */
void LALSTPNQM_Integrator_Init(LALStatus *status, INT2 num, void *params,
		int(*derivator)(REAL8, const REAL8[], REAL8[], void *),
		LALSTPNQM_Integrator_System *integrator);

/**		The function evolves the system with the given time-step.
 * @param[in,out]	status	: The LAL universal status structure
 * @param[in]	integrator	: the integration method
 * @param[in]		step	: the step size
 * @param[in,out]	values	: as input parameters the system's actual position,
 * as ouput the system's next position.
 */
void LALSTPNQM_Integrator_Func(LALStatus *status, LALSTPNQM_Integrator_System *integrator,
		REAL8 step, REAL8 values[]);

/**		The function deallocates the integration method.
 * @param[in,out]	status	: The LAL universal status structure
 * @param[in]	integrator	: the integration method
 */
void LALSTPNQM_Integrator_Free(LALStatus *status, LALSTPNQM_Integrator_System *integrator);

#endif /* LALSTPNQM_INTEGRATOR_H */
