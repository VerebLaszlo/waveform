/**
 * @file waveform_init.h
 * @author László Veréb
 * @date 2010.04.23.
 */

 #ifndef WAVEFORM_INIT_H_
 #define WAVEFORM_INIT_H_

#include "util.h"
#include "waveform.h"
#include <stdio.h>

/**
 *		The function checks, that the waveform generation is it even needed, the
 *	termination conditions are fulfilled, if
 *	<ul>
 *		<li>the \f$f_C\f$ cutoff frequency is lower than the detectors lower
 *		sensitivity</li>
 *		<li>the \f$t_C\f$ chirp-time is non-positive</li>
 *		<li>the waveform's estimated \f$t_n\f$ length is non-positive</li>
 *	</ul>
 *	\f[
 *		f_C=\min\left(f_S, \frac{1}{M\pi\sqrt{216}}\right)
 *	\f]
 *	\f{eqnarray*}{
 *		t_C&=&\underbrace{\frac{5}{256\eta M^{5/3}(\pi f_L)^{8/3}}}_{0.5PN}+
 *		\underbrace{\frac{3715+4620\eta}{64512\eta M (\pi f_L)^{6/3}}}_{1PN}+
 *		\underbrace{\left(-\frac{\pi}{8\eta M^{2/3}(\pi f_L)^{5/3}}\right)}_{1.5PN}+
*		\underbrace{\frac{5}{128\eta M^{1/3}(\pi f_L)^{4/3}}\left(\displaystyle\frac{3058673}{10160464}+\frac{5429}{1008}\eta+\frac{617}{144}\eta^2\right)}_{2PN}+
 *		\underbrace{\frac{5}{256\eta f_L^{3/3}}\left(\displaystyle\frac{7729}{252}-\frac{13}{3}\eta\right)}_{2.5PN}\\
 *		&+&\underbrace{\frac{5M}{256\eta(\pi
 *		f_L)^{2/3}}\left(-\frac{10052469856691}{23471078400}+\frac{128}{3}\pi^2\left(\frac{15335597827}{15240960}-\frac{451}{12}\pi^2+\frac{352}{3}\left(-\frac{11831}{9240}\right)-\frac{2464}{9}\left(-\frac{1987}{3080}\right)\right)\eta+\frac{6848}{105}\gamma-\frac{15211}{1728}\eta^2-\frac{25565}{1296}\eta^3+\frac{6848}{105}\log(\pi
 *		f_L M)^{1/3}\right)}_{3PN}\\
 *		&+&\underbrace{\frac{5
 *		M\pi}{256\eta\left(M^{1/3}\right)}\left(-\frac{15419335}{127008}-\frac{75703}{756}\eta+\frac{14809}{378}\eta^2\right)}_{3.5PN}
 *	\f}
 *	\f[
 *		t_n=\int dv
 *	\f]
 */
bool waveform_check(waveform_Params params);

 #endif
