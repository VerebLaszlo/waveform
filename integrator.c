#include <lal/LALInspiral.h>

#include "integrator.h"
#include "util_debug.h"

NRCSID (INTEGRATORC, "$Id$");

void integrator_init(LALStatus *status, rk_Struct *rk,
		INT2 num_evolution_variables) {
	INITSTATUS(status, "runge_kutta_init", INTEGRATORC);
	ATTATCHSTATUSPTR(status);
	rk->solver_type = gsl_odeiv_step_rkf45;
	rk->solver_step = gsl_odeiv_step_alloc(rk->solver_type,
			num_evolution_variables);
	rk->solver_control = gsl_odeiv_control_standard_new(1.0e-9, 1.0e-9, .2, .2);
	rk->solver_evolve = gsl_odeiv_evolve_alloc(num_evolution_variables);
	rk->solver_system.jacobian = NULL;
	rk->solver_system.dimension = num_evolution_variables;
	DETATCHSTATUSPTR(status);
	RETURN (status);
}

void integrator_Func(LALStatus *status, rk_Struct *rk, REAL8 step,
		REAL8 values[]) {
	INITSTATUS(status, "runge_kutta", INTEGRATORC);
	ATTATCHSTATUSPTR(status);
	REAL8 time = 0., time_Old, step_X = step;
	while (time < step) {
		time_Old = time;
		gsl_odeiv_evolve_apply(rk->solver_evolve, rk->solver_control,
				rk->solver_step, &(rk->solver_system), &time, step, &step_X,
				values);
		if (time == time_Old) {
			INT2 i;
			for (i = 0; i < rk->solver_system.dimension + 1; i++) {
				values[i] = 0.;
			}
			ABORT(status, LALINSPIRALH_ESTOPPED, LALINSPIRALH_MSGESTOPPED);
		}
	}
	DETATCHSTATUSPTR(status);
	RETURN (status);
}

void integrator_free(LALStatus *status, rk_Struct *rk) {
	INITSTATUS(status, "runge_kutta_free", INTEGRATORC);
	ATTATCHSTATUSPTR(status);
	ERR_LINE;
	gsl_odeiv_step_free(rk->solver_step);
	ERR_LINE;
	gsl_odeiv_evolve_free(rk->solver_evolve);
	ERR_LINE;
	gsl_odeiv_control_free(rk->solver_control);
	ERR_LINE;
	DETATCHSTATUSPTR(status);
	RETURN (status);
}
