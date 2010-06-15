/**
 * @file LALSTPNQM_Integrator.c
 *		Contains the function definitions needed by the integration function.
 * @author László Veréb
 * @date 2010.05.21.
 */

#include "LALSTPNQM_Integrator.h"
#include "util_debug.h"

NRCSID (INTEGRATORC, "$Id LALSTPNQM_Integrator.h$");

void LALSTPNQM_Integrator_Init(LALStatus *status, INT2 num, void *params,
		int(*derivator)(REAL8, const REAL8[], REAL8[], void *),
		LALSTPNQM_Integrator_System *integrator) {
	INITSTATUS(status, "LALSTPNQM_Integrator_Init", INTEGRATORC);
	ATTATCHSTATUSPTR(status);
	integrator->solver_type = gsl_odeiv_step_rkf45;
	TRYGSL(integrator->solver_step
			= gsl_odeiv_step_alloc(integrator->solver_type, num), status);
	TRYGSL(integrator->solver_control = gsl_odeiv_control_standard_new(1.0e-9, 1.0e-9,
			.2, .2), status);
	TRYGSL(integrator->solver_evolve = gsl_odeiv_evolve_alloc(num), status);
	integrator->solver_system.jacobian = NULL;
	integrator->solver_system.dimension = num;
	integrator->solver_system.params = params;
	integrator->solver_system.function = derivator;
	DETATCHSTATUSPTR(status);
	RETURN (status);
}

void LALSTPNQM_Integrator_Func(LALStatus *status, LALSTPNQM_Integrator_System *integrator,
		REAL8 step, REAL8 values[]) {
	INITSTATUS(status, "LALSTPNQM_Integrator_Func", INTEGRATORC);
	ATTATCHSTATUSPTR(status);
	REAL8 time = 0., time_Old, step_X = step;
	while (time < step) {
		time_Old = time;
		TRYGSL(gsl_odeiv_evolve_apply(integrator->solver_evolve,
				integrator->solver_control, integrator->solver_step,
				&(integrator->solver_system), &time, step, &step_X, values), status);
		if (time == time_Old) {
			memset(values, 0, integrator->solver_system.dimension + 1);
			//INT2 i; for (i = 0; i < integrator->solver_system.dimension + 1; i++) {values[i] = 0.;}
			ABORT(status, LALINSPIRALH_ESTOPPED, LALINSPIRALH_MSGESTOPPED);
		}
	}
	DETATCHSTATUSPTR(status);
	RETURN (status);
}

void LALSTPNQM_Integrator_Free(LALStatus *status, LALSTPNQM_Integrator_System *integrator) {
	INITSTATUS(status, "LALSTPNQM_Integrator_Free", INTEGRATORC);
	ATTATCHSTATUSPTR(status);
	TRYGSL(gsl_odeiv_step_free(integrator->solver_step), status);
	TRYGSL(gsl_odeiv_evolve_free(integrator->solver_evolve), status);
	TRYGSL(gsl_odeiv_control_free(integrator->solver_control), status);
	DETATCHSTATUSPTR(status);
	RETURN (status);
}
