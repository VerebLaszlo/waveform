#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_odeiv.h>
#include <lal/LALStatusMacros.h>

/**
 * \todo írni
 */
typedef struct rk_Tag {
	const gsl_odeiv_step_type* solver_type;
	gsl_odeiv_step* solver_step;
	gsl_odeiv_control* solver_control;
	gsl_odeiv_evolve* solver_evolve;
	gsl_odeiv_system solver_system;
} rk_Struct;

/**
 * \todo írni
 */
void integrator_init(LALStatus *status, rk_Struct *rk,
		INT2 num_evolution_variables);

/**
 * \todo írni
 */
void
integrator_Func(LALStatus *status, rk_Struct *rk, REAL8 step, REAL8 values[]);

/**
 * \todo írni
 */
void integrator_free(LALStatus *status, rk_Struct *rk);

#endif /* INTEGRATOR_H */
