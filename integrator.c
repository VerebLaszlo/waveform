/**
 * @file integrator.c
 *		Containes the function definitons needed by the integrator function.
 * @author László Veréb
 * @date 2010.05.21.
 */

#include <lal/LALInspiral.h>
#include <lal/LALGSL.h>

#include "integrator.h"
#include "util_debug.h"

NRCSID (INTEGRATORC, "$Id integrator.c$");

void integrator_init(LALStatus *status, INT2 num, void *params,
		int(*derivator)(REAL8, const REAL8[], REAL8[], void *),
		integrator_System *integrator) {
	INITSTATUS(status, "integrator_init", INTEGRATORC);
	ATTATCHSTATUSPTR(status);
	integrator->solver_type = gsl_odeiv_step_rkf45;
	integrator->solver_step
			= gsl_odeiv_step_alloc(integrator->solver_type, num);
	integrator->solver_control = gsl_odeiv_control_standard_new(1.0e-9, 1.0e-9,
			.2, .2);
	integrator->solver_evolve = gsl_odeiv_evolve_alloc(num);
	integrator->solver_system.jacobian = NULL;
	integrator->solver_system.dimension = num;
	integrator->solver_system.params = params;
	integrator->solver_system.function = derivator;
	DETATCHSTATUSPTR(status);
	RETURN (status);
}

void integrator_Func(LALStatus *status, integrator_System *integrator,
		REAL8 step, REAL8 values[]) {
	INITSTATUS(status, "integrator_Func", INTEGRATORC);
	ATTATCHSTATUSPTR(status);
	REAL8 time = 0., time_Old, step_X = step;
	while (time < step) {
		time_Old = time;
		TRYGSL(gsl_odeiv_evolve_apply(integrator->solver_evolve,
					integrator->solver_control, integrator->solver_step,
					&(integrator->solver_system), &time, step, &step_X, values),
				status);
		CHECKSTATUSPTR(status);
		if (time == time_Old) {
			INT2 i;
			for (i = 0; i < integrator->solver_system.dimension + 1; i++) {
				values[i] = 0.;
			}
			ABORT(status, LALINSPIRALH_ESTOPPED, LALINSPIRALH_MSGESTOPPED);
		}
	}
	ERR_STR_END("F1");
	DETATCHSTATUSPTR(status);
	ERR_STR_END("F2");
	RETURN (status);
}

void integrator_free(LALStatus *status, integrator_System *integrator) {
	INITSTATUS(status, "runge_kutta_free", INTEGRATORC);
	ATTATCHSTATUSPTR(status);
	gsl_odeiv_step_free(integrator->solver_step);
	gsl_odeiv_evolve_free(integrator->solver_evolve);
	gsl_odeiv_control_free(integrator->solver_control);
	DETATCHSTATUSPTR(status);
	RETURN (status);
}
