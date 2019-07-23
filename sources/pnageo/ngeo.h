#ifndef _NGEO
#define _NGEO

#include <math.h>

#define PI 3.14159265358979323846

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C"
{
EXPORT void FromBLH_ToXYZ(double B, double L, double H, double A, double alfa, double *X, double *Y, double *Z);
EXPORT void FromXYZ_ToBLH(double X, double Y, double Z, double A, double alfa, double *B, double *L, double *H);
EXPORT void FromWGS_ToPZ90(double B0, double L0, double H0,double *B1,  double *L1, double *H1);
EXPORT void FromPZ90_ToWGS(double B0, double L0, double H0, double *B1, double *L1,double *H1);
EXPORT void FromWGS_ToPZ90_02(double B0, double L0,double H0, double *B1, double *L1, double *H1);
EXPORT void FromPZ90_02_ToWGS(double B0, double L0,double  H0, double *B1, double *L1, double * H1);
EXPORT void FromPZ90_ToPZ90_02(double B0,double L0,double H0, double *B1,double *L1, double *H1);
EXPORT void FromPZ90_02_ToPZ90(double B0,double L0, double H0, double *B1, double *L1, double *H1);
EXPORT void FromPZ90_ToSK42(double B0, double L0, double H0, double *B1, double *L1, double *H1);
EXPORT void FromSK42_ToPZ90(double B0, double L0, double H0,  double *B1, double *L1, double *H1);
EXPORT void FromPZ90_ToSK95(double B0, double L0, double H0, double *B1, double *L1, double *H1);
EXPORT void FromSK95_ToPZ90(double B0, double L0, double H0, double *B1, double *L1, double *H1);
EXPORT void FromPZ90_02_ToSK42(double B0, double L0, double H0, double *B1, double *L1, double *H1);
EXPORT void FromSK42_ToPZ90_02(double B0, double L0, double H0, double *B1, double *L1, double *H1);
EXPORT void FromPZ90_02_ToSK95(double B0, double L0, double H0, double *B1, double *L1, double *H1);
EXPORT void FromSK95_ToPZ90_02(double B0, double L0, double H0, double *B1,double *L1,double *H1);
EXPORT void FromBL_ToXY(double B, double L, unsigned int forcedZone, double *x, double *y);
EXPORT void FromXY_ToBL(double x, double y, double *B, double *L);
EXPORT int GetIndexZoneSK42(double L);
}

#endif
