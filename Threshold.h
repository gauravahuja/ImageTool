#ifndef _Threshold_H
#define _Threshold_H

#include <cxcore.h>

#define X		0
#define Y		1

#define Bmax	3
#define Gmax	4
#define Rmax	5

#define Bmin	7
#define Gmin	8
#define Rmin	9

#define Bavg	11
#define Gavg	12
#define Ravg	13

#define Hmax	15
#define Smax	16
#define Vmax	17

#define Hmin	19
#define Smin	20
#define Vmin	21

#define Havg	23
#define Savg	24
#define Vavg	25

struct _Threshold
{
	CvScalar Min;
	CvScalar Max;
	CvScalar Avg;
};

void GetThreshold(IplImage* Frame/*BGR Image*/, _Threshold SelectionThresholds[2]);

#endif