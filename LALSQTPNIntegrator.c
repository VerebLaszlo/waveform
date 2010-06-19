/**
 * @file LALSQTPNIntegrator.c
 *		Contains the function definitions needed by the integration method.
 * @author László Veréb
 * @date 2010.05.21.
 */

#include "LALSQTPNIntegrator.h"
#include "util_debug.h"

NRCSID (LALSQTPNINTEGRATORC, "$Id LALSQTPNIntegrator.h$");

void LALSQTPNIntegratorInit(LALStatus *status,
		LALSQTPNIntegratorSystem *integrator, INT2 num, void *params,
		int(*derivator)(REAL8, const REAL8[], REAL8[], void *)) {
	INITSTATUS(status, "LALSQTPNIntegratorInit", LALSQTPNINTEGRATORC);
	ATTATCHSTATUSPTR(status);
	integrator->solverType = gsl_odeiv_step_rkf45;
	TRYGSL(integrator->solverStep
			= gsl_odeiv_step_alloc(integrator->solverType, num), status);
	TRYGSL(integrator->solverControl = gsl_odeiv_control_standard_new(1.0e-9, 1.0e-9,
			.2, .2), status);
	TRYGSL(integrator->solverEvolve = gsl_odeiv_evolve_alloc(num), status);
	integrator->solverSystem.jacobian = NULL;
	integrator->solverSystem.dimension = num;
	integrator->solverSystem.params = params;
	integrator->solverSystem.function = derivator;
	DETATCHSTATUSPTR(status);
	RETURN (status);
}

void LALSQTPNIntegratorFunc(LALStatus *status, REAL8 values[], LALSQTPNIntegratorSystem *integrator,
		REAL8 step) {
	INITSTATUS(status, "LALSQTPNIntegratorFunc", LALSQTPNINTEGRATORC);
	ATTATCHSTATUSPTR(status);
	REAL8 time = 0., time_Old, step_X = step;
	while (time < step) {
		time_Old = time;
		TRYGSL(gsl_odeiv_evolve_apply(integrator->solverEvolve,
				integrator->solverControl, integrator->solverStep,
				&(integrator->solverSystem), &time, step, &step_X, values), status);
		if (time == time_Old) {
			memset(values, 0, integrator->solverSystem.dimension + 1);
			ABORT(status, LALINSPIRALH_ESTOPPED, LALINSPIRALH_MSGESTOPPED);
		}
	}
	DETATCHSTATUSPTR(status);
	RETURN (status);
}
