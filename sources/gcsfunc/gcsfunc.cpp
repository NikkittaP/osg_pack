#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gcsfunc.h"

double  WGS_A     = 6378137.0;
double  WGS_alfa  = 1/298.257223563;
double  PZ_A      = 6378136.0;
double  PZ_alfa   = 1/298.25784;
double  KRAS_A    = 6378245.0;
double  KRAS_alfa = 1/298.3;



extern "C"
{
EXPORT void FromBLH_ToXYZ(double B, double L, double H, double A, double alfa, double *X, double *Y, double *Z)
{ double tmp;
  double N, e2;
  double XX, YY, ZZ;
  e2 = (2.0 - alfa)*alfa; 
  tmp = e2*sin(B)*sin(B);
  N  = A/sqrt(1.0 - tmp);
  XX  = (N + H)*cos(B)*cos(L);
  YY  = (N + H)*cos(B)*sin(L);
  ZZ  = ((1.0 - e2)*N + H)*sin(B);
  *X = XX;
  *Y = YY;
  *Z = ZZ;
}

EXPORT void FromXYZ_ToBLH(double X, double Y, double Z, double A, double alfa, double *B, double *L, double *H)
{
  double D, e12, b_prom;
  double r, c, p,s1, s2;
  double s, BB, HH, LL;
  double tmp;
  e12 = (2.0 - alfa)*alfa;
   D = sqrt(X*X + Y*Y);

   if (D == 0.0)
   {
      if ((Z == 0.0) || (Z > 0.0)) 
          BB = 0.5*PI;
      else
          BB = -0.5*PI;
      *B = BB;
      *L = 0.0;
      tmp = e12*sin(BB)*sin(BB);
      HH = 1*sin(BB) - A*sqrt(1. - tmp);
      *H = (double) HH;
   }
   else
   {
      LL = asin(Y/D);
      if (LL < 0) LL = -LL;
      if ((Y < 0.0) && (X > 0.0)) LL = 2.*PI - LL;
      if ((Y < 0.0) && (X < 0.0)) LL = PI + LL;
      if ((Y > 0.0) && (X < 0.0)) LL = PI - LL;
	  if ((Y == 0.0) && (X > 0.0)) LL = 0;
	  if ((Y == 0.0) && (X < 0.0)) LL = PI;
      
	  *L = (double) LL;
   }
   if (Z == 0.0)
   {
      *B = (double) 0.0;
      *H = (double) (D - A);
   }
   else
   {
      r  = sqrt(X*X + Y*Y + Z*Z);
      c  = asin(Z/r);
      p  = e12*A/(2.*r);
      s1 = 0.;  s = 1.0;

      while(s > 0.01)
      {
         b_prom = c + s1;
         tmp = e12*sin(b_prom)*sin(b_prom);   
         s2 = asin(p*sin(2.*b_prom)/sqrt(1. - tmp));
         if (fabs(s2 - s1) < 0.00000001)
	       {
            BB = b_prom;
            *B = (double) BB;
            tmp = e12 * sin(BB)*sin(BB);
	    			HH = D*cos(BB) + Z*sin(BB) - A*sqrt(1.0 -  tmp );
            *H = (double) HH;
	          s = 0.0;
         }
         else
        s1 = s2;
     }
   }
}

EXPORT void FromWGS_ToPZ90(double B0, double L0, double H0,double *B1,  double *L1, double *H1)
{
  double  m = 0.00000012;
  double wx = 0.0;
  double wy = 0.0;
  double wz = 0.0000009696;
  double dx = 1.10;
  double dy = 0.30;
  double dz = 0.90;
  double X0,Y0,Z0;
  double X1,Y1,Z1;
  double BB1, LL1, HH1;
   
  FromBLH_ToXYZ(B0, L0, H0, WGS_A, WGS_alfa,
   (double*)&X0, (double*)&Y0, (double*)&Z0);

  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;
  FromXYZ_ToBLH(X1, Y1, Z1, PZ_A,PZ_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void FromPZ90_ToWGS(double B0, double L0, double H0, double *B1, double *L1,double *H1)
{
  double  m =-0.00000012;
  double wx = 0.0;
  double wy = 0.0;
  double wz =-0.0000009696;
  double dx =-1.10;
  double dy =-0.30;
  double dz =-0.90;
  double X0,Y0,Z0;
  double X1,Y1,Z1;
  double BB1, LL1, HH1;
  FromBLH_ToXYZ(B0,L0,H0,PZ_A,PZ_alfa,  (double*)&X0, (double*)&Y0, (double*)&Z0);

  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1= (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;
  FromXYZ_ToBLH(X1,Y1,Z1, WGS_A,WGS_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;  
}

EXPORT void FromWGS_ToPZ90_02(double B0, double L0, double  H0, double *B1, double *L1, double *H1)
{
  double m  = 0.0;
  double wx = 0.0;
  double wy = 0.0;
  double wz = 0.0;
  double dx = 0.36;
  double dy =-0.08;
  double dz =-0.18;
  double  X0,Y0,Z0;
  double  X1,Y1,Z1;
  double BB1, LL1, HH1;
  FromBLH_ToXYZ(B0,L0,H0, WGS_A,WGS_alfa,(double*)&X0, (double*)&Y0, (double*)&Z0);
  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;
  FromXYZ_ToBLH(X1,Y1,Z1, PZ_A,PZ_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void FromPZ90_02_ToWGS(double B0, double L0, double H0, double *B1, double *L1, double * H1)
{
const
  double m  = 0.0;
  double wx = 0.0;
  double wy = 0.0;
  double wz = 0.0;
  double dx =-0.36;
  double dy =+0.08;
  double dz =+0.18;
  double X0,Y0,Z0;
  double X1,Y1,Z1;
  double BB1, LL1, HH1;  
  FromBLH_ToXYZ(B0,L0,H0,PZ_A,PZ_alfa, (double*)&X0, (double*)&Y0, (double*)&Z0);

  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;
  FromXYZ_ToBLH(X1,Y1,Z1, WGS_A,WGS_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);

  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void FromPZ90_ToPZ90_02(double B0, double L0, double H0, double *B1,double *L1, double *H1)
{
  double m  =-0.00000022;
  double wx = 0.0;
  double wy = 0.0;
  double wz =-0.0000006302;
  double dx =-1.07;
  double dy = 0.03;
  double dz = 0.02;
  double X0,Y0,Z0;
  double X1,Y1,Z1;
  double BB1, LL1, HH1;
  FromBLH_ToXYZ(B0,L0,H0, PZ_A,PZ_alfa, (double*)&X0, (double*)&Y0, (double*)&Z0);
  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;
  FromXYZ_ToBLH(X1,Y1,Z1, PZ_A,PZ_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void FromPZ90_02_ToPZ90(double B0, double L0, double H0, double *B1, double *L1, double *H1)
{
  double m  = 0.00000022;
  double wx = 0.0;
  double wy = 0.0;
  double wz = 0.0000006302;
  double dx = 1.07;
  double dy =-0.03;
  double dz =-0.02;
  double X0,Y0,Z0;
  double X1,Y1,Z1;
  double BB1, LL1, HH1;
  FromBLH_ToXYZ(B0,L0,H0, PZ_A,PZ_alfa, (double*)&X0, (double*)&Y0, (double*)&Z0);
  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;
  FromXYZ_ToBLH(X1,Y1,Z1, PZ_A,PZ_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void FromPZ90_ToSK42(double B0, double L0, double H0, double *B1, double *L1, double *H1)
{
  double m  = 0.0;
  double wx = 0.0;
  double wy = 0.0000016968;
  double wz = 0.0000031998;
  double dx = -25.0;
  double dy = 141.0;
  double dz = 80.0;
  double X0,Y0,Z0;
  double X1,Y1,Z1; 
  double BB1, LL1, HH1;
  FromBLH_ToXYZ(B0,L0,H0, PZ_A,PZ_alfa, (double*)&X0, (double*)&Y0, (double*)&Z0);
  X1  = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1  = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1  = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;

  FromXYZ_ToBLH(X1,Y1,Z1, KRAS_A,KRAS_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void FromSK42_ToPZ90(double B0, double L0, double H0,  double *B1, double *L1, double *H1)
{
  double m  = 0.0;
  double wx = 0.0;
  double wy =-0.0000016968;
  double wz =-0.0000031998;
  double dx = 25.0;
  double dy =-141.0;
  double dz =-80.0;
  double X0,Y0,Z0; 
  double X1,Y1,Z1;
  double BB1, LL1, HH1;
  FromBLH_ToXYZ(B0,L0,H0, KRAS_A,KRAS_alfa, (double*)&X0, (double*)&Y0, (double*)&Z0);
  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;
  FromXYZ_ToBLH(X1,Y1,Z1, PZ_A,PZ_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void FromPZ90_ToSK95(double B0, double L0, double H0, double *B1, double *L1, double *H1)
{ 
  double m  = 0.0;
  double wx = 0.0;
  double wy = 0.0;
  double wz = 0.0;
  double dx =-25.9;
  double dy = 130.94;
  double dz = 81.76;
  double X0,Y0,Z0;
  double X1,Y1,Z1; 
  double BB1, LL1, HH1;
  FromBLH_ToXYZ(B0,L0,H0, PZ_A,PZ_alfa, (double*)&X0, (double*)&Y0, (double*)&Z0);

  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;

  FromXYZ_ToBLH(X1,Y1,Z1, KRAS_A,KRAS_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void FromSK95_ToPZ90(double B0, double L0, double H0, double *B1, double *L1, double *H1)
{
  double m  = 0.0;
  double wx = 0.0;
  double wy = 0.0;
  double wz = 0.0;
  double dx = 25.9;
  double dy =-130.94;
  double dz =-81.76;
  double X0,Y0,Z0; 
  double X1,Y1,Z1;
  double BB1, LL1, HH1;
  FromBLH_ToXYZ(B0,L0,H0, KRAS_A,KRAS_alfa, (double*)&X0, (double*)&Y0, (double*)&Z0);
  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;
  FromXYZ_ToBLH(X1,Y1,Z1, PZ_A,PZ_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void  FromPZ90_02_ToSK42(double B0, double  L0, double H0, double *B1, double *L1, double *H1)
{
  double m  = 0.00000022;
  double  wx = 0.0;
  double wy = 0.0000016968;
  double wz = 0.0000038300;
  double dx =-23.93;
  double dy = 141.03;
  double dz = 79.98;
  double X0,Y0,Z0;
  double X1,Y1,Z1;
  double BB1, LL1, HH1;
  FromBLH_ToXYZ(B0,L0,H0, PZ_A,PZ_alfa, (double*)&X0, (double*)&Y0, (double*)&Z0);

  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;
  FromXYZ_ToBLH(X1,Y1,Z1, KRAS_A,KRAS_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void FromSK42_ToPZ90_02(double B0, double L0, double H0, double *B1, double *L1, double *H1)
{
  double m  =-0.00000022;
  double wx = 0.0;
  double wy =-0.0000016968;
  double wz =-0.0000038300;
  double dx = 23.93;
  double dy =-141.03;
  double dz =-79.98;
  double X0,Y0,Z0;
  double X1,Y1,Z1; 
  double BB1, LL1, HH1;
  FromBLH_ToXYZ(B0,L0,H0, KRAS_A,KRAS_alfa, (double*)&X0, (double*)&Y0, (double*)&Z0);
  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;

  FromXYZ_ToBLH(X1,Y1,Z1, PZ_A,PZ_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void FromPZ90_02_ToSK95(double B0, double L0, double H0, double *B1, double *L1, double *H1)
{
  double m  = 0.00000022;
  double wx = 0.0;
  double wy = 0.0;
  double wz = 0.0000006302;
  double dx =-24.83;
  double dy = 130.97;
  double dz = 81.74;
  double  X0,Y0,Z0;
  double  X1,Y1,Z1; 
  double BB1, LL1, HH1;
  FromBLH_ToXYZ(B0,L0,H0, PZ_A,PZ_alfa, (double*)&X0, (double*)&Y0, (double*)&Z0);
  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;

  FromXYZ_ToBLH(X1,Y1,Z1, KRAS_A,KRAS_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

EXPORT void FromSK95_ToPZ90_02(double B0, double L0, double H0, double *B1,double *L1,double *H1)
{
  double m  =-0.00000022;
  double wx = 0.0;
  double wy = 0.0;
  double wz =-0.0000006302;
  double dx = 24.83;
  double dy =-130.97;
  double dz =-81.74;
  double X0,Y0,Z0;
  double X1,Y1,Z1; 
  double BB1, LL1, HH1;
  
  FromBLH_ToXYZ(B0,L0,H0, KRAS_A,KRAS_alfa, (double*)&X0, (double*)&Y0, (double*)&Z0);
  X1 = (1.0 + m)*( X0 + wz*Y0 - wy*Z0) + dx;
  Y1 = (1.0 + m)*(-wz*X0 + Y0 + wx*Z0) + dy;
  Z1 = (1.0 + m)*( wy*X0 - wx*Y0 + Z0) + dz;
  FromXYZ_ToBLH(X1,Y1,Z1, PZ_A,PZ_alfa, (double*)&BB1, (double*)&LL1, (double*)&HH1);
  *B1 = BB1;
  *L1 = LL1;
  *H1 = HH1;
}

static const double d3 = 3 * PI / 180.0;
static const double d6 = 6 * PI / 180.0;

/// <summary>
/// Zone index by SK-42 longitude
/// </summary>
/// <param name="aLon">Longitude</param>
/// <returns>Zone index</returns>
int GetIndexZoneSK42(double L)
{
	if (L < 0)
	{
		L = L + 2 * PI;
	}
	double dN = L / d6;
	int iN = (int)dN;
	iN++;
	return iN;
}

/// <summary>
/// Вычисление плоских прямоугольных координат Гаусса-Крюгера по геодезичеким (по геоиду GK42)
/// </summary>
EXPORT void FromBL_ToXY(double B, double L, unsigned int forcedZone, double *x, double *y)
{
	int index = forcedZone == 0 ? GetIndexZoneSK42(L) : (int)forcedZone;

	double lo = (L - (d3 + d6 * (index - 1)));
	if (lo > PI)
	{
		lo = lo - 2 * PI;
	}
	else if (lo < -PI)
	{
		lo = lo + 2 * PI;
	}
	
	double bo = B;
	double lPow2 = pow(lo, 2);
	double sinBo = sin(bo);
	double sinBoPow2 = pow(sinBo, 2);
	double sinBoPow4 = pow(sinBo, 4);
	double sinBoPow6 = pow(sinBo, 6);
	double xa = lPow2 * (109500 - 574700 * sinBoPow2 + 863700 * sinBoPow4 - 398600 * sinBoPow6);
	double xb = lPow2 * (278194 - 830174 * sinBoPow2 + 572434 * sinBoPow4 - 16010 * sinBoPow6 + xa);
	double xc = lPow2 * (672483.4 - 811219.9 * sinBoPow2 + 5420 * sinBoPow4 - 10.6 * sinBoPow6 + xb);
	double xd = lPow2 * (1594561.25 + 5336.535 * sinBoPow2 + 26.79 * sinBoPow4 + 0.149 * sinBoPow6 + xc);
	*x = 6367558.4968 * bo - sin(bo * 2) * (16002.89 + 66.9607 * sinBoPow2 + 0.3515 * sinBoPow4 - xd);

	double ya = lPow2 * (79690 - 866190 * sinBoPow2 + 1730360 * sinBoPow4 - 945460 * sinBoPow6);
	double yb = lPow2 * (270806 - 1523417 * sinBoPow2 + 1327645 * sinBoPow4 - 21701 * sinBoPow6 + ya);
	double yc = lPow2 * (1070204.16 - 2136826.66 * sinBoPow2 + 17.98 * sinBoPow4 - 11.99 * sinBoPow6 + yb);
	*y = (5 + 10 * index) * 100000 + lo * cos(bo) * (6378245 + 21346.1415 * sinBoPow2 + 107.159 * sinBoPow4 + 0.5977 * sinBoPow6 + yc);
}

/// <summary>
/// Вычисление геодезичеких координат по плоским прямоугольным координатам Гаусса-Крюгера
/// </summary>
EXPORT void FromXY_ToBL(double x, double y, double *B, double *L)
{
	//трансформация координат, для работы в отдельной зоне
	//широта остается без изменений
	double dX = x;
	//а вот в значении долготы необходимо удалить номер зоны и восточное смещение
	//(в начало координаты дописан номер зоны)
	int iNumZone = (int)(y / 1000000);
	double dY = y - iNumZone * 1000000; //номер зоны из значения долготы удален
	dY = dY - 500000; //значение долготы без восточного смещения
					  //
	const double dP = 180.0 / PI;
	double dBeta = (dX / 6367558.4969) * dP;
	double dSinBeta = sin(dX / 6367558.4969);
	double dCosBeta = cos(dX / 6367558.4969);
	double dCosBeta2 = dCosBeta * dCosBeta;
	//
	double dB0 = 50221746 + (293622 + (2350 + 22 * dCosBeta2) * dCosBeta2) * dCosBeta2;
	dB0 = dB0 * 0.0000000001 * dSinBeta * dCosBeta * dP;
	dB0 = dBeta + dB0;
	//
	double dB0R = dB0 * PI / 180.0;
	double dSinB0 = sin(dB0R);
	double dCosB0 = cos(dB0R);
	double dCosB02 = dCosB0 * dCosB0;
	//
	double dN0 = 6399698.902 - (21562.267 - (108.973 - 0.612 * dCosB02) * dCosB02) * dCosB02;
	double dB2 = (0.5 + 0.003369 * dCosB02) * dSinB0 * dCosB0;
	double dB3 = 0.333333 - (0.166667 - 0.001123 * dCosB02) * dCosB02;
	double dB4 = 0.25 + (0.16161 + 0.00562 * dCosB02) * dCosB02;
	double dB5 = 0.2 - (0.1667 - 0.0088 * dCosB02) * dCosB02;
	//
	double dZ = dY / (dN0 * dCosB0);
	double dZ2 = dZ * dZ;
	//
	double dB = dB0 - (1 - (dB4 - 0.12 * dZ2) * dZ2) * dZ2 * dB2 * dP;
	double dL = (1 - (dB3 - dB5 * dZ2) * dZ2) * dZ * dP;
	//
	*B = dB * PI / 180.0;
	double dL0 = iNumZone * 6 - 3; //L0_ ---> долгота осевого меридиана
	*L = (dL0 + dL) * PI / 180.0;
}

}
