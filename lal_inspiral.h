/**
 * @file lal_inspiral.h
 */

#ifndef _LALINSPIRAL_H
#define _LALINSPIRAL_H

#define LAL_PI 3.14
#define EulerC 0.577215665

/**
 *		The enumeration contains of the PN orders.
 */
typedef enum {
	LAL_PNORDER_NEWTONIAN,
	LAL_PNORDER_HALF,
	LAL_PNORDER_ONE,
	LAL_PNORDER_ONE_POINT_FIVE,
	LAL_PNORDER_TWO,
	LAL_PNORDER_TWO_POINT_FIVE,
	LAL_PNORDER_THREE,
	LAL_PNORDER_THREE_POINT_FIVE,
	LAL_PNORDER_PSEUDO_FOUR
} LALPNOrde;

#endif
