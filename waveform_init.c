/**
 * @file waveform_init.c
 * @author László Veréb
 * @date 2010.04.24.
 */

 #include "waveform_init.h"

bool waveform_check(waveform_Params params) {
	params.cutoff_Freq = fmin(params.sampling_Freq,1. / (params.total_Mass *
	LAL_PI * sqrt(216.)));
	if (params.cutoff_Freq < params.lower_Freq) {
		fprintf(stderr, "The cutoff frequency is lower than"
		"the detectors lower sensitivity: f_C = %lg, f_L = %lg.\n", params.cutoff_Freq,
		params.lower_Freq);
		return false;
	}
	double etaPow2 = SQR(params.eta);
	double etaPow3 = etaPow2 * params.eta;
	double etaPow4 = etaPow2 * params.eta;
	double total_MassPowi_3[9];
	total_MassPowi_3[1] = cbrt(params.total_Mass);
	int i;
	for (i = 2; i < 9; i++) {
		total_MassPowi_3[i] = total_MassPowi_3[i - 1] * total_MassPowi_3[1];
	}
	double piLowerFreq[9];
	piLowerFreq[0] = 1.;
	piLowerFreq[1] = cbrt(LAL_PI * params.lower_Freq);
	for (i = 2; i < 6; i++) {
		piLowerFreq[i] = piLowerFreq[i - 1] * piLowerFreq[1];
	}
	switch (params.order) {
	case LAL_PNORDER_PSEUDO_FOUR:
	case LAL_PNORDER_THREE_POINT_FIVE:
		params.chirp_Time -= (-15419335. / 127008. - 75703. / 756. *
		params.eta + 14809. / 378. * etaPow2) * LAL_PI * 5. *
		params.total_Mass / (256. * params.eta * total_MassPowi_3[1]);
	case LAL_PNORDER_THREE:
		params.chirp_Time += 5. * params.total_Mass * (-10052469856691. /
		23471078400. + 128. / 3. * LAL_PI * LAL_PI + (15335597827. / 15240960. -
		451. / 12. * LAL_PI * LAL_PI + 352. / 3. * (-11831. / 9240.) - 2464. /
		9. * (-1987. / 3080.)) * params.eta + 6848. / 105. * LAL_GAMMA - 15211. / 1728.
		* etaPow2 + 25565. /	1296. * etaPow3 + 6848. / 105. * log(4. *
		total_MassPowi_3[8] * piLowerFreq[8])) * total_MassPowi_3[6] / (256. *
		params.eta * total_MassPowi_3[8] * piLowerFreq[8]);
	case LAL_PNORDER_TWO_POINT_FIVE:
		params.chirp_Time += 5. * (7729. / 252. - 13. / 3. * params.eta) /
		(256. * params. eta * params.lower_Freq);
	case LAL_PNORDER_TWO:
		params.chirp_Time += 5. * (3058673. / 1016064. + 5429. * params.eta /
		1008. + 617. * params.eta * params.eta / 144.) / (128. * params.eta *
		total_MassPowi_3[1] * piLowerFreq[4]);
	case LAL_PNORDER_ONE_POINT_FIVE:
		params.chirp_Time -= LAL_PI / (8. * params.eta * total_MassPowi_3[2] *
		piLowerFreq[5]);
	case LAL_PNORDER_ONE:
		params.chirp_Time += (3715. + 4620. * params.eta) / (64512. *
		params.eta * total_MassPowi_3[3] * piLowerFreq[6]);
	case LAL_PNORDER_HALF:
		params.chirp_Time += 5. / (256. * params.eta * total_MassPowi_3[5] *
		piLowerFreq[8]);
	case LAL_PNORDER_NEWTONIAN:
	default:
		break;
	}
	if (params.chirp_Time <= 0.) {
		fprintf(stderr, "The chirp time is nonpositive: t_C"
		" = %lg.\n", params.chirp_Time);
		return false;
	}

	return true;
}

