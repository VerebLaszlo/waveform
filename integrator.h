/**
 * @file integrator.h
 *		Containes the function declaration and structures needed by the
 *		integrator function.
 * @author László Veréb
 * @date 2010.05.21.
 */

#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_odeiv.h>

#include <lal/LALStatusMacros.h>

/**		The structure contains the integrator.
 */
typedef struct integrator_System_Tag {
	const gsl_odeiv_step_type* solver_type;
	gsl_odeiv_step* solver_step;
	gsl_odeiv_control* solver_control;
	gsl_odeiv_evolve* solver_evolve;
	gsl_odeiv_system solver_system;
} integrator_System;

/**		The function initialize the integrator.
 * @param[in,out]	status	: The LAL universal status structure
 * @param[in]		num		: the number of the dynamic variables
 * @param[in]		params	: the parameters used in the derivator function
 * @param[out]	integrator	: the structure conaining the integrator
 */
void integrator_init(LALStatus *status, INT2 num, void *params,  integrator_System *integrator);

/**		The function evolves the system with the given time-step.
 * @param[in,out]	status	: The LAL universal status structure
 * @param[in]	integrator	: the integrator
 * @param[in]		step	: the step size
 * @param[in,out]	values	: as input parameters the system's actual position,
 * as ouput the system's next position.
 */
void integrator_Func(LALStatus *status, integrator_System *integrator, REAL8 step, REAL8 values[]);

/**		The function deallocates the integrator.
 * @param[in,out]	status	: The LAL universal status structure
 * @param[in]	integrator	: the integrator
 */
void integrator_free(LALStatus *status, integrator_System *integrator);

#endif /* INTEGRATOR_H */
