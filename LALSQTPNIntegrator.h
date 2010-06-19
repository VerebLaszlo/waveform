/**
 * @file LALSQTPNIntegrator.h
 *		Contains the function declaration and structures needed by the
 *	integration method.
 * @author László Veréb
 * @date 2010.05.21.
 */

#ifndef LALSQTPNINTEGRATOR_H
#define LALSQTPNINTEGRATOR_H

#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_odeiv.h>

#include <lal/LALInspiral.h>
#include <lal/LALGSL.h>
#include <lal/LALStatusMacros.h>

#ifdef __cplusplus
extern "C" {
#endif

NRCSID (LALSQTPNINTEGRATORH, "$Id LALSQTPNIntegrator.h$");

/**		The structure contains the integration method and its settings.
 */
typedef struct tagLALSQTPNIntegratorSystem{
	const gsl_odeiv_step_type* solverType;
	gsl_odeiv_step* solverStep;
	gsl_odeiv_control* solverControl;
	gsl_odeiv_evolve* solverEvolve;
	gsl_odeiv_system solverSystem;
} LALSQTPNIntegratorSystem;

/**		The function initialize the integration method.
 * @param[in,out]	status	: The LAL universal status structure
 * @param[out]	integrator	: the structure containing the integration method
 * @param[in]		num		: the number of the dynamic variables
 * @param[in]		params	: the parameters used in the derivative function
 * @param[in]	derivator	: pointer to the derivative function
 */
void LALSQTPNIntegratorInit(LALStatus *status, LALSQTPNIntegratorSystem *integrator,
		INT2 num, void *params, int(*derivator)(REAL8, const REAL8[], REAL8[], void *));

/**		The function evolves the system with the given time-step.
 * @param[in,out]	status	: The LAL universal status structure
 * @param[in,out]	values	: as input parameters the system's actual position,
 * as ouput the system's next position.
 * @param[in]	integrator	: the integration method
 * @param[in]		step	: the step size
 */
void LALSQTPNIntegratorFunc(LALStatus *status, REAL8 values[],
		LALSQTPNIntegratorSystem *integrator, REAL8 step);

#ifdef __cplusplus
}
#endif

#endif /* LALSQTPNINTEGRATOR_H */
