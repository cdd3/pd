/*__________________________________________________________________________
 
 ambilib Copyright (C) 2002 - 2010 Dave Malham, Dogma Design
 PD port by Matthew Paradis
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
 ____________________________________________________________________________*/

#include "ambilib~.h"
/**********************************************ambipan~******************************/
static t_class *ambipan_tilde_class;

typedef struct _ambipan_tilde {
	t_object  x_obj;
	t_sample APf;
	t_float APverbose;
	float APfAzimuthLeft;
	float APfElevationLeft;
	float APfZerothLeft;
	float APfFirstLeft;
	float APfVolumeLeft;
	float APfDistanceLeft;
	float APfDistanceFactorLeft;
	float APfCentreLeft;
	float APfAzimuthRight;
	float APfElevationRight;
	float APfZerothRight;
	float APfFirstRight;
	float APfVolumeRight;
	float APfDistanceRight;
	float APfDistanceFactorRight;
	float APfCentreRight;
	double APdALeft;
	double APdELeft;
	double APdWLeft;
	double APdXLeft;
	double APdYLeft;
	double APdZLeft;
	double APdWLeftOld;
	double APdXLeftOld;
	double APdYLeftOld;
	double APdZLeftOld;
	double APdARight;
	double APdERight;
	double APdWRight;
	double APdXRight;
	double APdYRight;
	double APdZRight;
	double APdWRightOld;
	double APdXRightOld;
	double APdYRightOld;
	double APdZRightOld;
	double APdOldPanAngle;
	double APvol;
	
	double APdAzLeft;
	double APdAzRight;
	double APdElLeft;
	double APdElRight;
	double APdDistanceRight;
	double APdDistanceLeft;
	double APdCentreRight;
	double APdCentreLeft;
	double APdVolumeLeft;
	double APdVolumeRight;
	double APdFirstLeft;
	double APdZerothLeft;
	double APdFirstLeftDistance;
	double APdZerothLeftDistance;
	double APdDistanceMultiplierLeft;
	double APdZerothRight;
	double APdFirstRight;
	double APdZerothRightDistance;
	double APdFirstRightDistance;
	double APdDistanceMultiplierRight;
	
	float APpansampleL;
	float APpansampleR;
	float APtempout1;
	float APtempout2;
	float APtempout3;
	float APtempout4;
	float APtempout5;
	float APtempout6;
	float APtempout7;
	float APtempout8;
	
} t_ambipan_tilde;

void ambipanProgram(t_ambipan_tilde *x);
double distancingLeft (t_ambipan_tilde *x,double APdBFactor);
double distancingRight (t_ambipan_tilde *x,double APdBFactor);
double gainLaw (t_ambipan_tilde *x, double APgain);

void ambipanProgram(t_ambipan_tilde *x)
{
	x->APfAzimuthLeft	=  1.0;		
	x->APfElevationLeft	=  0.5;		
	x->APfZerothLeft 	=  0.707;
	x->APfFirstLeft 		=  1.0;
	x->APfDistanceLeft 	=  0.1;
	x->APfCentreLeft		=  0.1;		
	x->APfDistanceFactorLeft =  0.75;	
	x->APfVolumeLeft 	= k6dBPoint;
	x->APfAzimuthRight	= 1.0;		
	x->APfElevationRight	=  0.5;		
	x->APfZerothRight 	=  0.707;
	x->APfFirstRight 	=  1.0;
	x->APfDistanceRight	=  0.1;
	x->APfCentreRight 	=  0.1;		
	x->APfDistanceFactorRight =  0.75;
	x->APfVolumeRight 	= k6dBPoint;
	
	x->APdALeft		= 0.0;
	x->APdELeft		= 0.0;
	x->APdWLeft		= 0.0;
	x->APdXLeft		= 0.0;
	x->APdYLeft		= 0.0;
	x->APdZLeft		= 0.0;
	x->APdWLeftOld		= 0.0;
	x->APdXLeftOld		= 0.0;
	x->APdYLeftOld		= 0.0;
	x->APdZLeftOld		= 0.0;
	x->APdOldPanAngle	= 0.0;
	x->APdARight		= 0.0;
	x->APdERight		= 0.0;
	x->APdWRight		= 0.0;
	x->APdXRight		= 0.0;
	x->APdYRight		= 0.0;
	x->APdZRight		= 0.0;
	x->APdWRightOld	= 0.0;
	x->APdXRightOld	= 0.0;
	x->APdYRightOld	= 0.0;
	x->APdZRightOld	= 0.0;
}


double distancingLeft (t_ambipan_tilde *x,double APdBFactor)
{
	return  pow(10, (x->APfDistanceFactorLeft * APdBFactor * log(x->APfDistanceLeft / x->APfCentreLeft)) / kLog2);
}


double distancingRight (t_ambipan_tilde *x,double APdBFactor)
{
	return pow(10, (x->APfDistanceFactorRight * APdBFactor * log(x->APfDistanceRight / x->APfCentreRight)) / kLog2);
}


double gainLaw (t_ambipan_tilde *x, double APgain)
{
	if (APgain >= 0.5) 
	{
		x->APvol = pow(10, ((APgain-1) * 2.0));	
	} 
	else 
		if (APgain >= 0.2) 
		{
			x->APvol = pow(10, ((APgain-0.625)*8.0));	
		} 
		else 
		{							
			x->APvol = pow(10, ((APgain-0.483333333333)*12.0));
		}
	return x->APvol;
}

void ambipan_tilde_APverbose(t_ambipan_tilde *x, t_floatarg APverb )
{
	t_ambipan_tilde *this = (t_ambipan_tilde *)x;
	x->APverbose = (x->APverbose<0)?0.0:(x->APverbose>1)?1.0:x->APverbose;
	x->APverbose = APverb;
}	

void ambipan_tilde_azl(t_ambipan_tilde *x, t_floatarg APazl)
{
	x->APfAzimuthLeft = APazl;
	x->APfAzimuthLeft = (x->APfAzimuthLeft<0)?0.0:(x->APfAzimuthLeft>1)?1.0:x->APfAzimuthLeft;
	x->APdALeft = (-x->APfAzimuthLeft) * (2 * kPI);	
	if(x->APverbose == 1)post("Changed azl to %f\n",x->APfAzimuthLeft);
}
void ambipan_tilde_azr(t_ambipan_tilde *x, t_floatarg APazr )
{
	x->APfAzimuthRight =APazr;
	x->APfAzimuthRight = (x->APfAzimuthRight<0)?0.0:(x->APfAzimuthRight>1)?1.0:x->APfAzimuthRight;
	x->APdARight = (-x->APfAzimuthRight) * (2 * kPI);;
	if(x->APverbose == 1)post("Changed azr to %f\n",x->APfAzimuthRight);
}
void ambipan_tilde_ell(t_ambipan_tilde *x, t_floatarg APell )
{
	x->APfElevationLeft = APell;
	x->APfElevationLeft = (x->APfElevationLeft<0)?0.0:(x->APfElevationLeft>1)?1.0:x->APfElevationLeft;
	x->APdELeft = (x->APfElevationLeft - 0.5) * kPI;	
	if(x->APverbose == 1)post("Changed ell to %f\n",x->APfElevationLeft);
}
void ambipan_tilde_elr(t_ambipan_tilde *x, t_floatarg APelr )
{
	x->APfElevationRight = APelr;
	x->APfElevationRight = (x->APfElevationRight<0)?0.0:(x->APfElevationRight>1)?1.0:x->APfElevationRight;
	x->APdERight = (x->APfElevationRight - 0.5)  * kPI;
	if(x->APverbose == 1)post("Changed elr to %f\n",x->APfElevationRight);
}
void ambipan_tilde_ztl(t_ambipan_tilde *x, t_floatarg APztl )
{
	x->APfZerothLeft = APztl;
	x->APfZerothLeft = (x->APfZerothLeft<0)?0.0:(x->APfZerothLeft>1)?1.0:x->APfZerothLeft;
	if(x->APverbose == 1)post("Changed ztl to %f\n",x->APfZerothLeft);
}
void ambipan_tilde_ztr(t_ambipan_tilde *x, t_floatarg APztr )
{
	x->APfZerothRight = APztr;
	x->APfZerothRight = (x->APfZerothRight<0)?0.0:(x->APfZerothRight>1)?1.0:x->APfZerothRight;
	if(x->APverbose == 1)post("Changed ztr to %f\n",x->APfZerothRight);
}

void ambipan_tilde_fil(t_ambipan_tilde *x, t_floatarg APfil )
{
	x->APfFirstLeft = APfil;
	x->APfFirstLeft = (x->APfFirstLeft<0)?0.0:(x->APfFirstLeft>1)?1.0:x->APfFirstLeft;
	if(x->APverbose == 1)post("Changed fil to %f\n",x->APfFirstLeft);
}
void ambipan_tilde_fir(t_ambipan_tilde *x, t_floatarg APfir )
{
	x->APfFirstRight = APfir;
	x->APfFirstRight = (x->APfFirstRight<0)?0.0:(x->APfFirstRight>1)?1.0:x->APfFirstRight;
	if(x->APverbose == 1)post("Changed fir to %f\n",x->APfFirstRight);
}
void ambipan_tilde_disl(t_ambipan_tilde *x, t_floatarg APdisl )
{
	x->APfDistanceLeft = APdisl;
	x->APfDistanceLeft = (x->APfDistanceLeft<0.01)?0.01:(x->APfDistanceLeft>1)?1.0:x->APfDistanceLeft;
	if(x->APverbose == 1)post("Changed disl to %f\n",x->APfDistanceLeft);
}
void ambipan_tilde_disr(t_ambipan_tilde *x, t_floatarg APdisr )
{
	x->APfDistanceRight = APdisr;
	x->APfDistanceRight = (x->APfDistanceRight<0.01)?0.01:(x->APfDistanceRight>1)?1.0:x->APfDistanceRight;
	if(x->APverbose == 1)post("Changed disr to %f\n",x->APfDistanceRight);
}		
void ambipan_tilde_cenl(t_ambipan_tilde *x, t_floatarg APcenl )
{
	x->APfCentreLeft = APcenl;
	x->APfCentreLeft = (x->APfCentreLeft<0.0001)?0.0001:(x->APfCentreLeft>1)?1.0:x->APfCentreLeft;
	if(x->APverbose == 1)post("Changed cenl to %f\n",x->APfCentreLeft);
}
void ambipan_tilde_cenr(t_ambipan_tilde *x, t_floatarg APcenr )
{
	x->APfCentreRight = APcenr;
	x->APfCentreRight = (x->APfCentreRight<0.0001)?0.0001:(x->APfCentreRight>1)?1.0:x->APfCentreRight;
	if(x->APverbose == 1)post("Changed cenr to %f\n",x->APfCentreRight);
}
void ambipan_tilde_dfacl(t_ambipan_tilde *x, t_floatarg APdfacl )
{
	x->APfDistanceFactorLeft = APdfacl;
	x->APfDistanceFactorLeft = (x->APfDistanceFactorLeft<0)?0.0:(x->APfDistanceFactorLeft>1)?1.0:x->APfDistanceFactorLeft;
	if(x->APverbose == 1)post("Changed dfacl to %f\n",x->APfDistanceFactorLeft);
}
void ambipan_tilde_dfacr(t_ambipan_tilde *x, t_floatarg APdfacr )
{
	x->APfDistanceFactorRight = APdfacr;
	x->APfDistanceFactorRight = (x->APfDistanceFactorRight<0)?0.0:(x->APfDistanceFactorRight>1)?1.0:x->APfDistanceFactorRight;
	if(x->APverbose == 1)post("Changed dfacr to %f\n",x->APfDistanceFactorRight);
}
void ambipan_tilde_voll(t_ambipan_tilde *x, t_floatarg APvoll )
{
	x->APfVolumeLeft = APvoll;
	x->APfVolumeLeft = (x->APfVolumeLeft<0)?0.0:(x->APfVolumeLeft>1)?1.0:x->APfVolumeLeft;
	if(x->APverbose == 1)post("Changed voll to %f\n",x->APfVolumeLeft);
}
void ambipan_tilde_volr(t_ambipan_tilde *x, t_floatarg APvolr )
{
	x->APfVolumeRight = APvolr;
	x->APfVolumeRight = (x->APfVolumeRight<0)?0.0:(x->APfVolumeRight>1)?1.0:x->APfVolumeRight;
	if(x->APverbose == 1)post("Changed volr to %f\n",x->APfVolumeRight);
}

t_int *ambipan_tilde_perform4(t_int *w)
{
	t_ambipan_tilde *x = (t_ambipan_tilde *)(w[1]);
	t_sample  *APin1 =    (t_sample *)(w[2]);
	t_sample  *APin2 =    (t_sample *)(w[3]);
	t_sample  *APout1 =    (t_sample *)(w[4]);
	t_sample  *APout2 =    (t_sample *)(w[5]);
	t_sample  *APout3 =    (t_sample *)(w[6]);
	t_sample  *APout4 =    (t_sample *)(w[7]);
	int          APn =           (int)(w[8]);
	
	x->APdAzLeft = x->APdALeft;
	x->APdAzRight = x->APdARight;
	x->APdElLeft = x->APdELeft;
	x->APdElRight = x->APdERight;
	x->APdDistanceRight = x->APfDistanceRight;
	x->APdDistanceLeft = x->APfDistanceLeft;
	x->APdCentreRight = x->APfCentreRight;
	x->APdCentreLeft = x->APfCentreLeft;
	x->APdVolumeLeft = gainLaw(x,x->APfVolumeLeft);
	x->APdVolumeRight = gainLaw(x,x->APfVolumeRight);
	x->APdFirstLeft = x->APfFirstLeft;
	x->APdZerothLeft = x->APfZerothLeft;
	x->APdFirstLeftDistance = 1.0;
	x->APdZerothLeftDistance = 1.0;
 	x->APdDistanceMultiplierLeft = 0.0;
	x->APdZerothRight = x->APfZerothRight;
	x->APdFirstRight = x->APfFirstRight;
	x->APdZerothRightDistance = 1.0;
	x->APdFirstRightDistance = 1.0;
	x->APdDistanceMultiplierRight = 0.0;
	
	if (x->APdCentreLeft < 0.001 )x->APdCentreLeft = 0.001;
	if (x->APdCentreRight < 0.001 )x->APdCentreRight = 0.001;
	
	x->APdXLeft = cos(x->APdAzLeft)* cos(x->APdELeft);
	x->APdXRight = cos(x->APdAzRight)* cos(x->APdERight);
	x->APdYLeft = sin(x->APdAzLeft)* cos(x->APdELeft);
	x->APdYRight = sin(x->APdAzRight)* cos(x->APdERight);
	x->APdZLeft = sin(x->APdELeft);
	x->APdZRight = sin(x->APdERight);
	
	if ( x->APdDistanceLeft >= x->APdCentreLeft ) 
	{
 		x->APdDistanceMultiplierLeft = distancingLeft(x,kdBFactor);	
 		x->APdZerothLeftDistance = x->APdVolumeLeft * x->APdZerothLeft * x->APdDistanceMultiplierLeft;
 		x->APdFirstLeftDistance =  x->APdVolumeLeft * x->APdFirstLeft * x->APdDistanceMultiplierLeft;
	} 
	else 
	{
 		x->APdZerothLeftDistance = x->APdVolumeLeft * x->APdZerothLeft * (2 - (x->APdDistanceLeft / x->APdCentreLeft));
 		x->APdFirstLeftDistance =  x->APdVolumeLeft * x->APdFirstLeft * (x->APdDistanceLeft / x->APdCentreLeft) ;
	}
	
	if ( x->APfDistanceRight >= x->APfCentreRight ) 
	{
		x->APdDistanceMultiplierRight = distancingRight(x,kdBFactor);
		x->APdZerothRightDistance = x->APdVolumeRight * x->APdZerothRight * x->APdDistanceMultiplierRight;
		x->APdFirstRightDistance = x->APdVolumeRight * x->APfFirstRight * x->APdDistanceMultiplierRight;
	} 
	else 
	{
		x->APdZerothRightDistance = x->APdVolumeRight * x->APdZerothRight * (2 - (x->APdDistanceRight / x->APdCentreRight));
		x->APdFirstRightDistance =  x->APdVolumeRight * x->APdFirstRight * (x->APdDistanceRight / x->APdCentreRight) ;
	}
	
 	x->APdWLeft = kAntiDenorm +x->APdZerothLeftDistance;
 	x->APdXLeft = kAntiDenorm + x->APdFirstLeftDistance * x->APdXLeft;
 	x->APdYLeft = kAntiDenorm + x->APdFirstLeftDistance * x->APdYLeft;
 	x->APdZLeft = kAntiDenorm + x->APdFirstLeftDistance * x->APdZLeft;
 	x->APdWRight = kAntiDenorm + x->APdZerothRightDistance;
 	x->APdXRight = kAntiDenorm + x->APdFirstRightDistance * x->APdXRight;
 	x->APdYRight = kAntiDenorm + x->APdFirstRightDistance * x->APdYRight;
 	x->APdZRight = kAntiDenorm + x->APdFirstRightDistance * x->APdZRight;
	
	while (APn--){
		x->APpansampleL = (*APin1++);
		x->APpansampleR = (*APin2++);
		
		
        x->APtempout1 =  kAntiDenorm + x->APpansampleL * x->APdWLeftOld;      
        x->APtempout2 =  kAntiDenorm + x->APpansampleL * x->APdXLeftOld;		
        x->APtempout3 =  kAntiDenorm + x->APpansampleL * x->APdYLeftOld;      
        x->APtempout4 =  kAntiDenorm + x->APpansampleL * x->APdZLeftOld;	
        x->APtempout5 =  kAntiDenorm + x->APpansampleR * x->APdWRightOld;	
        x->APtempout6 =  kAntiDenorm + x->APpansampleR * x->APdXRightOld;	
        x->APtempout7 =  kAntiDenorm + x->APpansampleR * x->APdYRightOld;	
        x->APtempout8 =  kAntiDenorm + x->APpansampleR * x->APdZRightOld;	
        (*APout1++) = x->APtempout1 + x->APtempout5;  
        (*APout2++) = x->APtempout2 + x->APtempout6;
        (*APout3++) = x->APtempout3 + x->APtempout7;
        (*APout4++) = x->APtempout4 + x->APtempout8;  
		
		
		
		x->APdWLeftOld	= (x->APdWLeftOld * kAvCoef1) + (kAvCoef2 * x->APdWLeft);
		x->APdXLeftOld	= (x->APdXLeftOld * kAvCoef1) + (kAvCoef2 * x->APdXLeft);
		x->APdYLeftOld	= (x->APdYLeftOld * kAvCoef1) + (kAvCoef2 * x->APdYLeft);
		x->APdZLeftOld	= (x->APdZLeftOld * kAvCoef1) + (kAvCoef2 * x->APdZLeft);
		x->APdWRightOld	= (x->APdWRightOld * kAvCoef1) + (kAvCoef2 * x->APdWRight);
		x->APdXRightOld	= (x->APdXRightOld * kAvCoef1) + (kAvCoef2 * x->APdXRight);
		x->APdYRightOld	= (x->APdYRightOld * kAvCoef1) + (kAvCoef2 * x->APdYRight);
		x->APdZRightOld	= (x->APdZRightOld * kAvCoef1) + (kAvCoef2 * x->APdZRight);
	}
	return (w+9);
	
}


void ambipan_tilde_dsp(t_ambipan_tilde *x, t_signal **sp)
{
	dsp_add(ambipan_tilde_perform4, 8, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec, sp[0]->s_n);
}

void *ambipan_tilde_new(void)
{
	t_ambipan_tilde *x = (t_ambipan_tilde *)pd_new(ambipan_tilde_class);
	ambipanProgram(x);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	return (void *)x;
}

void ambipan_tilde_setup(void) 
{
	ambipan_tilde_class = class_new(gensym("ambipan~"),(t_newmethod)ambipan_tilde_new,0, sizeof(t_ambipan_tilde),CLASS_DEFAULT,A_DEFFLOAT,0);
	class_addmethod(ambipan_tilde_class,
					(t_method)ambipan_tilde_dsp, gensym("dsp"), 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_azl,gensym("azl"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_azr,gensym("azr"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_ell,gensym("ell"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_elr,gensym("elr"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_ztl,gensym("ztl"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_ztr,gensym("ztr"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_fil,gensym("fil"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_fir,gensym("fir"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_disl,gensym("disl"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_disr,gensym("disr"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_cenl,gensym("cenl"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_cenr,gensym("cenr"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_dfacl,gensym("dfacl"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_dfacr,gensym("dfacr"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_voll,gensym("voll"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_volr,gensym("volr"), A_DEFFLOAT, 0);
	class_addmethod(ambipan_tilde_class, (t_method)ambipan_tilde_APverbose,gensym("verbose"), A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(ambipan_tilde_class, t_ambipan_tilde, APf);
}


/**********************************************ambidec~***************************************************/


static t_class *ambidec_tilde_class;
typedef struct _ambidec_tilde {
	t_object  x_obj;
	t_sample ADf;
	double ADcompensate;
	double ADaw;
	double ADax;
	double ADay;
	double ADaz;
	float ADfDecoder;	
	int ADoutcount;
} t_ambidec_tilde;

t_int *ambidec_tilde_perform0(t_int *w)
{
	t_ambidec_tilde *x = (t_ambidec_tilde *)(w[1]);
	t_sample  *ADin1 =    (t_sample *)(w[2]);
	t_sample  *ADin2 =    (t_sample *)(w[3]);
	t_sample  *ADin3 =    (t_sample *)(w[4]);
	t_sample  *ADin4 =    (t_sample *)(w[5]);
	t_sample  *ADout1 =    (t_sample *)(w[6]);
	t_sample  *ADout2 =    (t_sample *)(w[7]);
	t_sample  *ADout3 =    (t_sample *)(w[8]);
	t_sample  *ADout4 =    (t_sample *)(w[9]);
	t_sample  *ADout5 =    (t_sample *)(w[10]);
	t_sample  *ADout6 =    (t_sample *)(w[11]);
	t_sample  *ADout7 =    (t_sample *)(w[12]);
	t_sample  *ADout8 =    (t_sample *)(w[13]);
	int          ADn =           (int)(w[14]);
	
	while (ADn--){
		x->ADaw =  (*ADin1++);					
		x->ADax =  (*ADin2++);					
		x->ADay =  (*ADin3++);
		x->ADaz =  (*ADin4++);
		x->ADcompensate = 6.0255958607435774697130296573509 ;
		*ADout1++ = (float)(x->ADcompensate * ((0.17656719307784428776945098299128 * x->ADaw) + (x->ADax * 0.075) + (x->ADay * 0.075) - (x->ADaz * 0.075)));
		*ADout2++ = (float)(x->ADcompensate * ((0.17656719307784428776945098299128 * x->ADaw) + (x->ADax * 0.075) - (x->ADay * 0.075) - (x->ADaz * 0.075)));
		*ADout3++ = (float)(x->ADcompensate * ((0.17656719307784428776945098299128 * x->ADaw) - (x->ADax * 0.075) - (x->ADay * 0.075) - (x->ADaz * 0.075)));
		*ADout4++ = (float)(x->ADcompensate * ((0.17656719307784428776945098299128 * x->ADaw) - (x->ADax * 0.075) + (x->ADay * 0.075) - (x->ADaz * 0.075)));
		*ADout5++ = (float)(x->ADcompensate * ((0.17656719307784428776945098299128 * x->ADaw) + (x->ADax * 0.075) + (x->ADay * 0.075) + (x->ADaz * 0.075)));
		*ADout6++ = (float)(x->ADcompensate * ((0.17656719307784428776945098299128 * x->ADaw) + (x->ADax * 0.075) - (x->ADay * 0.075) + (x->ADaz * 0.075)));
		*ADout7++ = (float)(x->ADcompensate * ((0.17656719307784428776945098299128 * x->ADaw) - (x->ADax * 0.075) - (x->ADay * 0.075) + (x->ADaz * 0.075)));
		*ADout8++ = (float)(x->ADcompensate * ((0.17656719307784428776945098299128 * x->ADaw) - (x->ADax * 0.075) + (x->ADay * 0.075) + (x->ADaz * 0.075)));
	}
	return (w+15);
}
t_int *ambidec_tilde_perform1(t_int *w)
{
	t_ambidec_tilde *x = (t_ambidec_tilde *)(w[1]);
	t_sample  *ADin1 =    (t_sample *)(w[2]);
	t_sample  *ADin2 =    (t_sample *)(w[3]);
	t_sample  *ADin3 =    (t_sample *)(w[4]);
	t_sample  *ADin4 =    (t_sample *)(w[5]);
	t_sample  *ADout1 =    (t_sample *)(w[6]);
	t_sample  *ADout2 =    (t_sample *)(w[7]);
	t_sample  *ADout3 =    (t_sample *)(w[8]);
	t_sample  *ADout4 =    (t_sample *)(w[9]);
	int          ADn =           (int)(w[10]);
	
	while (ADn--){
		x->ADaw =  (*ADin1++);					
		x->ADax =  (*ADin2++);					
		x->ADay =  (*ADin3++);
		x->ADaz =  (*ADin4++);
		x->ADcompensate = 2.98538261891795961791327696252157 ;
		*ADout1++ = (float)(x->ADcompensate *((0.3531343861556885755389019659825 * x->ADaw) + (x->ADax * 0.17698644609603447770055373554725) + (x->ADay * 0.17698644609603447770055373554725)));
		*ADout2++ = (float)(x->ADcompensate *((0.3531343861556885755389019659825 * x->ADaw) + (x->ADax * 0.17698644609603447770055373554725) - (x->ADay * 0.17698644609603447770055373554725)));
		*ADout3++ = (float)(x->ADcompensate *((0.3531343861556885755389019659825 * x->ADaw) - (x->ADax * 0.17698644609603447770055373554725) - (x->ADay * 0.17698644609603447770055373554725)));
		*ADout4++ = (float)(x->ADcompensate *((0.3531343861556885755389019659825 * x->ADaw) - (x->ADax * 0.17698644609603447770055373554725) + (x->ADay * 0.17698644609603447770055373554725)));
	}
	return (w+11);
}
t_int *ambidec_tilde_perform2(t_int *w)
{
	t_ambidec_tilde *x = (t_ambidec_tilde *)(w[1]);
	t_sample  *ADin1 =    (t_sample *)(w[2]);
	t_sample  *ADin2 =    (t_sample *)(w[3]);
	t_sample  *ADin3 =    (t_sample *)(w[4]);
	t_sample  *ADin4 =    (t_sample *)(w[5]);
	t_sample  *ADout1 =    (t_sample *)(w[6]);
	t_sample  *ADout2 =    (t_sample *)(w[7]);
	t_sample  *ADout3 =    (t_sample *)(w[8]);
	t_sample  *ADout4 =    (t_sample *)(w[9]);
	int          ADn =           (int)(w[10]);
	
	while (ADn--){
		x->ADaw =  (*ADin1++);					
		x->ADax =  (*ADin2++);					
		x->ADay =  (*ADin3++);
		x->ADaz =  (*ADin4++);
		x->ADcompensate = 2.98538261891795961791327696252157 ;
		*ADout1++ = (float)(x->ADcompensate *((0.3531343861556885755389019659825 * x->ADaw) + (x->ADax * 0.17698644609603447770055373554725) + (x->ADay * 0.125)));
		*ADout2++ = (float)(x->ADcompensate *((0.3531343861556885755389019659825 * x->ADaw) + (x->ADax * 0.17698644609603447770055373554725) - (x->ADay * 0.125)));
		*ADout3++ = (float)(x->ADcompensate *((0.3531343861556885755389019659825 * x->ADaw) - (x->ADax * 0.17698644609603447770055373554725) - (x->ADay * 0.125)));
		*ADout4++ = (float)(x->ADcompensate *((0.3531343861556885755389019659825 * x->ADaw) - (x->ADax * 0.17698644609603447770055373554725) + (x->ADay * 0.125)));
	}
	return (w+11);
}
t_int *ambidec_tilde_perform3(t_int *w)
{
	t_ambidec_tilde *x = (t_ambidec_tilde *)(w[1]);
	t_sample  *ADin1 =    (t_sample *)(w[2]);
	t_sample  *ADin2 =    (t_sample *)(w[3]);
	t_sample  *ADin3 =    (t_sample *)(w[4]);
	t_sample  *ADin4 =    (t_sample *)(w[5]);
	t_sample  *ADout1 =    (t_sample *)(w[6]);
	t_sample  *ADout2 =    (t_sample *)(w[7]);
	t_sample  *ADout3 =    (t_sample *)(w[8]);
	t_sample  *ADout4 =    (t_sample *)(w[9]);
	t_sample  *ADout5 =    (t_sample *)(w[10]);
	t_sample  *ADout6 =    (t_sample *)(w[11]);
	int          ADn =           (int)(w[12]);
	
	while (ADn--){
		x->ADaw =  (*ADin1++);					
		x->ADax =  (*ADin2++);					
		x->ADay =  (*ADin3++);
		x->ADaz =  (*ADin4++);
		x->ADcompensate = 4.62381021399260298134654968972522;
		
		*ADout1++ = (float)(x->ADcompensate *((0.235422924103792383692601310655 * x->ADaw) + (x->ADax * 0.144333333333333333333333333333333) + (x->ADay * 0.0833333333333333333333333333333333)));
		*ADout4++ = (float)(x->ADcompensate *((0.235422924103792383692601310655 * x->ADaw) - (x->ADax * 0.144333333333333333333333333333333) - (x->ADay * 0.0833333333333333333333333333333333)));
		*ADout2++ = (float)(x->ADcompensate *((0.235422924103792383692601310655 * x->ADaw) + (x->ADay * 0.166666666666666666666666666666667)));
		*ADout5++ = (float)(x->ADcompensate *((0.235422924103792383692601310655 * x->ADaw) - (x->ADay * 0.166666666666666666666666666666667)));
		*ADout3++ = (float)(x->ADcompensate *((0.235422924103792383692601310655 * x->ADaw) - (x->ADax * 0.144333333333333333333333333333333) +  (x->ADay * 0.0833333333333333333333333333333333)));
		*ADout6++ = (float)(x->ADcompensate *((0.235422924103792383692601310655 * x->ADaw) + (x->ADax * 0.144333333333333333333333333333333) -  (x->ADay * 0.0833333333333333333333333333333333)));
	}
	return (w+13);
}
t_int *ambidec_tilde_perform4(t_int *w)
{
	t_ambidec_tilde *x = (t_ambidec_tilde *)(w[1]);
	t_sample  *ADin1 =    (t_sample *)(w[2]);
	t_sample  *ADin2 =    (t_sample *)(w[3]);
	t_sample  *ADin3 =    (t_sample *)(w[4]);
	t_sample  *ADin4 =    (t_sample *)(w[5]);
	t_sample  *ADout1 =    (t_sample *)(w[6]);
	t_sample  *ADout2 =    (t_sample *)(w[7]);
	t_sample  *ADout3 =    (t_sample *)(w[8]);
	t_sample  *ADout4 =    (t_sample *)(w[9]);
	t_sample  *ADout5 =    (t_sample *)(w[10]);
	t_sample  *ADout6 =    (t_sample *)(w[11]);
	t_sample  *ADout7 =    (t_sample *)(w[12]);
	t_sample  *ADout8 =    (t_sample *)(w[13]);
	int          ADn =           (int)(w[14]);
	
	while (ADn--){
		x->ADaw =  (*ADin1++);					
		x->ADax =  (*ADin2++);					
		x->ADay =  (*ADin3++);
		x->ADaz =  (*ADin4++);
		x->ADcompensate = 6.16595001861482166320348343878615;
		*ADout1++ = (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) + (0.1154875 * x->ADax) + (0.0478375 * x->ADay)));
		*ADout2++ = (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) + (0.0478375 * x->ADax) + (0.1154875 * x->ADay)));
		*ADout3++ = (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) - (0.0478375 * x->ADax) + (0.1154875 * x->ADay)));
		*ADout4++ = (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) - (0.1154875 * x->ADax) + (0.0478375 * x->ADay)));
		*ADout5++ = (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) - (0.1154875 * x->ADax) - (0.0478375 * x->ADay)));
		*ADout6++ = (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) - (0.0478375 * x->ADax) - (0.1154875 * x->ADay)));
		*ADout7++ = (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) + (0.0478375 * x->ADax) - (0.1154875 * x->ADay)));
		*ADout8++ = (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) + (0.1154875 * x->ADax) - (0.0478375 * x->ADay)));
	}
	return (w+15);
}
t_int *ambidec_tilde_perform5(t_int *w)
{
	t_ambidec_tilde *x = (t_ambidec_tilde *)(w[1]);
	t_sample  *ADin1 =    (t_sample *)(w[2]);
	t_sample  *ADin2 =    (t_sample *)(w[3]);
	t_sample  *ADin3 =    (t_sample *)(w[4]);
	t_sample  *ADin4 =    (t_sample *)(w[5]);
	t_sample  *ADout1 =    (t_sample *)(w[6]);
	t_sample  *ADout2 =    (t_sample *)(w[7]);
	t_sample  *ADout3 =    (t_sample *)(w[8]);
	t_sample  *ADout4 =    (t_sample *)(w[9]);
	t_sample  *ADout5 =    (t_sample *)(w[10]);
	t_sample  *ADout6 =    (t_sample *)(w[11]);
	t_sample  *ADout7 =    (t_sample *)(w[12]);
	t_sample  *ADout8 =    (t_sample *)(w[13]);
	int          ADn =           (int)(w[14]);
	
	while (ADn--){
		x->ADaw =  (*ADin1++);					
		x->ADax =  (*ADin2++);					
		x->ADay =  (*ADin3++);
		x->ADaz =  (*ADin4++);
		x->ADcompensate = 6.0255958607435774697130296573509;
		
		*ADout1++ =  (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) + (x->ADax * 0.125)));
		*ADout2++ =  (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) + (x->ADax * 0.0884932230480172388502768677736641) + (x->ADay *0.0884932230480172388502768677736641)));
		*ADout3++ =  (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) + (x->ADay *0.125)));
		*ADout4++ =  (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) - (x->ADax * 0.0884932230480172388502768677736641) + (x->ADay *0.0884932230480172388502768677736641)));
		*ADout5++ =  (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) - (x->ADax  *0.125)));
		*ADout6++ =  (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) - (x->ADax * 0.0884932230480172388502768677736641) - (x->ADay *0.0884932230480172388502768677736641)));
		*ADout7++ =  (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) - (x->ADay * 0.125)));
		*ADout8++ =  (float)(x->ADcompensate *((0.17656719307784428776945098299125 * x->ADaw) + (x->ADax * 0.0884932230480172388502768677736641) - (x->ADay *0.0884932230480172388502768677736641)));
		
	}
	return (w+15);
}
t_int *ambidec_tilde_perform6(t_int *w)
{
	t_ambidec_tilde *x = (t_ambidec_tilde *)(w[1]);
	t_sample  *ADin1 =    (t_sample *)(w[2]);
	t_sample  *ADin2 =    (t_sample *)(w[3]);
	t_sample  *ADin3 =    (t_sample *)(w[4]);
	t_sample  *ADin4 =    (t_sample *)(w[5]);
	t_sample  *ADout1 =    (t_sample *)(w[6]);
	t_sample  *ADout2 =    (t_sample *)(w[7]);
	int          ADn =           (int)(w[8]);
	x->ADcompensate =  1.54881661891248134467161784112307;
	while (ADn--){
		x->ADaw =  (*ADin1++);					
		x->ADax =  (*ADin2++);					
		x->ADay =  (*ADin3++);
		x->ADaz =  (*ADin4++);
		*ADout1++ = (float)(x->ADcompensate * ((0.707945784384137910802214942189313 * x->ADaw)  + (x->ADay * 0.5))) ;
		*ADout2++ = (float)(x->ADcompensate * ((0.707945784384137910802214942189313 * x->ADaw)  - (x->ADay * 0.5)));
	}
	return (w+9);
}
t_int *ambidec_tilde_perform7(t_int *w)
{
	t_ambidec_tilde *x = (t_ambidec_tilde *)(w[1]);
	t_sample  *ADin1 =    (t_sample *)(w[2]);
	t_sample  *ADin2 =    (t_sample *)(w[3]);
	t_sample  *ADin3 =    (t_sample *)(w[4]);
	t_sample  *ADin4 =    (t_sample *)(w[5]);
	t_sample  *ADout1 =    (t_sample *)(w[6]);
	t_sample  *ADout2 =    (t_sample *)(w[7]);
	int          ADn =           (int)(w[8]);
	
	while (ADn--){
		x->ADaw =  (*ADin1++);					
		x->ADax =  (*ADin2++);					
		x->ADay =  (*ADin3++);
		x->ADaz =  (*ADin4++);
		*ADout1++ = (float)x->ADaw;
		*ADout2++ = (float)x->ADaw;
	}
	return (w+9);
}


void ambidec_tilde_dsp(t_ambidec_tilde *x, t_signal **sp)
{
	if(x->ADfDecoder == 0)dsp_add(ambidec_tilde_perform0, 14, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec, sp[8]->s_vec, sp[9]->s_vec,sp[10]->s_vec,sp[11]->s_vec, sp[0]->s_n);
	if(x->ADfDecoder == 1)dsp_add(ambidec_tilde_perform1, 10, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec, sp[6]->s_vec,sp[7]->s_vec, sp[0]->s_n);
	if(x->ADfDecoder == 2)dsp_add(ambidec_tilde_perform2, 10, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec, sp[6]->s_vec,sp[7]->s_vec, sp[0]->s_n);
	if(x->ADfDecoder == 3)dsp_add(ambidec_tilde_perform3, 12, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec, sp[8]->s_vec, sp[9]->s_vec, sp[0]->s_n);
	if(x->ADfDecoder == 4)dsp_add(ambidec_tilde_perform4, 14, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec, sp[8]->s_vec, sp[9]->s_vec,sp[10]->s_vec,sp[11]->s_vec, sp[0]->s_n);
	if(x->ADfDecoder == 5)dsp_add(ambidec_tilde_perform5, 14, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec, sp[6]->s_vec, sp[7]->s_vec, sp[8]->s_vec, sp[9]->s_vec,sp[10]->s_vec,sp[11]->s_vec, sp[0]->s_n);
	if(x->ADfDecoder == 6)dsp_add(ambidec_tilde_perform6, 8, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec, sp[0]->s_n);
	if(x->ADfDecoder == 7)dsp_add(ambidec_tilde_perform7, 8, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec, sp[0]->s_n);
}

void *ambidec_tilde_new(t_floatarg ADdec)
{
	t_ambidec_tilde *x = (t_ambidec_tilde *)pd_new(ambidec_tilde_class);
	x->ADfDecoder = ADdec;
	if(x->ADfDecoder < 0) x->ADfDecoder = 0;
	if(x->ADfDecoder > 7) x->ADfDecoder = 7;
	
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	
	if(x->ADfDecoder == 0)
	{
		post("Cubic Decoder\n");
		for(x->ADoutcount = 0; x->ADoutcount < 8;x->ADoutcount ++)outlet_new(&x->x_obj, gensym ("signal"));
	}
	if(x->ADfDecoder == 1)
	{
		post("Square Decoder\n");
		for(x->ADoutcount = 0; x->ADoutcount < 4;x->ADoutcount ++)outlet_new(&x->x_obj, gensym ("signal"));
	}
	if(x->ADfDecoder == 2)
	{
		post("Rectangular Decoder\n");
		for(x->ADoutcount = 0; x->ADoutcount < 4;x->ADoutcount ++)outlet_new(&x->x_obj, gensym ("signal"));
	}
	if(x->ADfDecoder == 3)
	{
		post("Hexagonal Decoder\n");
		for(x->ADoutcount = 0; x->ADoutcount < 6;x->ADoutcount ++)outlet_new(&x->x_obj, gensym ("signal"));
	}
	if(x->ADfDecoder == 4)
	{
		post("Octagonal (type 1) Decoder\n");
		for(x->ADoutcount = 0; x->ADoutcount < 8;x->ADoutcount ++)outlet_new(&x->x_obj, gensym ("signal"));
	}
	if(x->ADfDecoder == 5)
	{
		post("Octagonal (type 2) Decoder\n");
		for(x->ADoutcount = 0; x->ADoutcount < 8;x->ADoutcount ++)outlet_new(&x->x_obj, gensym ("signal"));
	}
	if(x->ADfDecoder == 6)
	{
		post("Stereo Decoder\n");
		for(x->ADoutcount = 0; x->ADoutcount < 2;x->ADoutcount ++)outlet_new(&x->x_obj, gensym ("signal"));
	}
	if(x->ADfDecoder == 7)
	{
		post("Mono Decoder\n");
		for(x->ADoutcount = 0; x->ADoutcount < 2;x->ADoutcount ++)outlet_new(&x->x_obj, gensym ("signal"));
	}
	
	return (void *)x;
}

void ambidec_tilde_setup(void) 
{
	ambidec_tilde_class = class_new(gensym("ambidec~"),(t_newmethod)ambidec_tilde_new,0, sizeof(t_ambidec_tilde),CLASS_DEFAULT,A_DEFFLOAT,0);
	class_addmethod(ambidec_tilde_class,
					(t_method)ambidec_tilde_dsp, gensym("dsp"), 0);
	CLASS_MAINSIGNALIN(ambidec_tilde_class, t_ambidec_tilde, ADf);
}

/*****************************************ambimic~************************************************************/

static t_class *ambimic_tilde_class;
typedef struct _ambimic_tilde {
	t_object  x_obj;
	t_sample AMf;
	t_float AMverbose;
	double AMdAzimuth;
	double AMdElevation;
	double AMdAngle;
	double AMdPattern;
	
	double AMdCosAzLeft;
	double AMdCosElLeft;
	double AMdSinAzLeft;
	double AMdSinElLeft;
	
	double AMdCosAzRight;
	double AMdCosElRight;
	double AMdSinAzRight;
	double AMdSinElRight;
	
	double AMdZerothLevel;
	double AMdFirstLevel;
	double AMdXCoeffLeft;
	double AMdYCoeffLeft;
	double AMdZCoeffLeft;
	
	double AMdXCoeffRight;
	double AMdYCoeffRight;
	double AMdZCoeffRight;
	
	double AMdCosAzLeftOld;
	double AMdCosElLeftOld;
	double AMdSinAzLeftOld;
	double AMdSinElLeftOld;
	
	double AMdCosAzRightOld;
	double AMdCosElRightOld;
	double AMdSinAzRightOld;
	double AMdSinElRightOld;
	double AMdAngleOld;
	double AMdPatternOld;
	
	double AMdZerothLevelOld;
	double AMdFirstLevelOld;
	
	double AMdXCoeffLeftOld;
	double AMdYCoeffLeftOld;
	double AMdZCoeffLeftOld;
	
	
	double AMdXCoeffRightOld;
	double AMdYCoeffRightOld;
	double AMdZCoeffRightOld;
	
	
	double AMnewW;
	double AMnewX;
	double AMnewY;
	double AMnewZ;
	
	float AMfAzimuth;
	float AMfElevation;
	float AMfAngle;
	float AMfPattern;
	
	float AMain1;
	float AMain2;
	float AMain3;
	float AMain4;
	float AMtempazi;
	float AMtempele;
} t_ambimic_tilde;

void ambimicProgram(t_ambimic_tilde *x)
{
	x->AMdAzimuth= 0.0;
	x->AMdElevation= 0.25;
	
	x->AMdCosAzLeft 	= 1.0;
	x->AMdCosElLeft 	= 1.0;
	x->AMdSinAzLeft  = 0.0;
	x->AMdSinElLeft 	= 0.0;
	
	x->AMdCosAzRight = 1.0;
	x->AMdCosElRight = 1.0;
	x->AMdSinAzRight = 0.0;
	x->AMdSinElRight = 0.0;
	
	x->AMdZerothLevel = 1.0;
	x->AMdFirstLevel = 1.0;
	
	x->AMdXCoeffLeft = 1.0;
	x->AMdYCoeffLeft = 1.0;
	x->AMdZCoeffLeft = 0.0;
	
	
	x->AMdXCoeffRight = 1.0;
	x->AMdYCoeffRight = 1.0;
	x->AMdZCoeffRight = 0.0;
	
	
	x->AMdZerothLevelOld = 1.0;
	x->AMdFirstLevelOld = 1.0;
	x->AMdXCoeffLeftOld = 1.0;
	x->AMdYCoeffLeftOld = 1.0;
	x->AMdZCoeffLeftOld = 0.0;
	
	
	x->AMdXCoeffRightOld = 1.0;
	x->AMdYCoeffRightOld = 1.0;
	x->AMdZCoeffRightOld = 0.0;
	
	
	x->AMdAngle		= 0.1666666667;
	x->AMdAngleOld	= 0.1666666667;
	
	x->AMdPattern= 0.0;
	
	x->AMdPatternOld= 0.0;
	
	
	x->AMnewW= 0.0;
	x->AMnewX= 0.0;
	x->AMnewY= 0.0;
	x->AMnewZ= 0.0;
	
	x->AMdAzimuth	= 0.0;
	x->AMdElevation	= 0.25;
	x->AMdAngle		= (float)0.1666666667;
	x->AMdPattern	= 0.5;
}
t_int *ambimic_tilde_perform(t_int *w)
{
	t_ambimic_tilde *x = (t_ambimic_tilde *)(w[1]);
	t_sample  *AMin1 =    (t_sample *)(w[2]);
	t_sample  *AMin2 =    (t_sample *)(w[3]);
	t_sample  *AMin3 =    (t_sample *)(w[4]);
	t_sample  *AMin4 =    (t_sample *)(w[5]);
	t_sample  *AMout1 =    (t_sample *)(w[6]);
	t_sample  *AMout2 =    (t_sample *)(w[7]);
	int          AMn =           (int)(w[8]);
	
	x->AMdCosAzLeft 	= cos(x->AMdAzimuth + x->AMdAngle);
	x->AMdCosElLeft	= cos(x->AMdElevation);
	x->AMdSinAzLeft	= sin(x->AMdAzimuth + x->AMdAngle );
	x->AMdSinElLeft	= sin(x->AMdElevation);
	
	x->AMdCosAzRight	= cos(x->AMdAzimuth - x->AMdAngle);
	x->AMdCosElRight = cos(x->AMdElevation);
	x->AMdSinAzRight	= sin(x->AMdAzimuth - x->AMdAngle);
	x->AMdSinElRight	= sin(x->AMdElevation);
	
	x->AMdZerothLevel = 1 - x->AMdPattern;
	x->AMdFirstLevel =  0.707 * x->AMdPattern;
	
	x->AMdXCoeffLeft = x->AMdCosAzLeft * x->AMdCosElLeft * x->AMdFirstLevel;
	x->AMdYCoeffLeft = x->AMdSinAzLeft * x->AMdCosElLeft * x->AMdFirstLevel;
	x->AMdZCoeffLeft = x->AMdSinElLeft * x->AMdFirstLevel;
	
	x->AMdXCoeffRight = x->AMdCosAzRight * x->AMdCosElRight * x->AMdFirstLevel;
	x->AMdYCoeffRight = x->AMdSinAzRight * x->AMdCosElRight * x->AMdFirstLevel;
	x->AMdZCoeffRight = x->AMdSinElRight * x->AMdFirstLevel;
	while (AMn--){
		x->AMain1 = (*AMin1++);
		x->AMain2 = (*AMin2++);
		x->AMain3 = (*AMin3++);
		x->AMain4 = (*AMin4++);
		x->AMnewW = kAntiDenorm + x->AMdZerothLevelOld * (double)x->AMain1;
		x->AMnewX = kAntiDenorm + (double)x->AMain2;
		x->AMnewY = kAntiDenorm + (double)x->AMain3;
		x->AMnewZ = kAntiDenorm + (double)x->AMain4;
        *AMout1++ = kAntiDenorm + (float) (x->AMnewW + (x->AMdXCoeffLeftOld*x->AMnewX) + (x->AMdYCoeffLeftOld*x->AMnewY) + (x->AMdZCoeffLeftOld*x->AMnewZ));
        *AMout2++ = kAntiDenorm + (float) (x->AMnewW + (x->AMdXCoeffRightOld*x->AMnewX) + (x->AMdYCoeffRightOld*x->AMnewY) + (x->AMdZCoeffRightOld*x->AMnewZ));
		x->AMdZerothLevelOld	= (x->AMdZerothLevelOld * kAvCoef1) + (kAvCoef2 * x->AMdZerothLevel);
		
        x->AMdXCoeffLeftOld	= (x->AMdXCoeffLeftOld * kAvCoef1) + (kAvCoef2 * x->AMdXCoeffLeft);
		x->AMdYCoeffLeftOld	= (x->AMdYCoeffLeftOld * kAvCoef1) + (kAvCoef2 * x->AMdYCoeffLeft);
        x->AMdZCoeffLeftOld	= (x->AMdZCoeffLeftOld * kAvCoef1) + (kAvCoef2 * x->AMdZCoeffLeft);
		
        x->AMdXCoeffRightOld	= (x->AMdXCoeffRightOld * kAvCoef1) + (kAvCoef2 * x->AMdXCoeffRight);
		x->AMdYCoeffRightOld   = (x->AMdYCoeffRightOld * kAvCoef1) + (kAvCoef2 * x->AMdYCoeffRight);
        x->AMdZCoeffRightOld	= (x->AMdZCoeffRightOld * kAvCoef1) + (kAvCoef2 * x->AMdZCoeffRight);
	}
	return (w+9);
	
}
void ambimic_tilde_azimuth(t_ambimic_tilde *x, t_floatarg AMazi )
{
	x->AMtempazi = AMazi;
	x->AMtempazi = (x->AMtempazi<0)?0.0:(x->AMtempazi>1)?1.0:x->AMtempazi;
	x->AMdAzimuth = (-x->AMtempazi) * (2 * kPI);
	if(x->AMverbose == 1)post("Changed azimuth to %f\n",x->AMtempazi);
}

void ambimic_tilde_elevation(t_ambimic_tilde *x, t_floatarg AMele )
{
	x->AMtempele = AMele;
	x->AMtempele = (x->AMtempele<0)?0.0:(x->AMtempele>1)?1.0:x->AMtempele;
	x->AMdElevation = (x->AMtempele - 0.5) * kPI;
	if(x->AMverbose == 1)post("Changed elevation to %f\n",x->AMtempele);
}

void ambimic_tilde_angle(t_ambimic_tilde *x, t_floatarg AMang )
{
	x->AMdAngle = AMang;
	x->AMdAngle = (x->AMdAngle<0)?0.0:(x->AMdAngle>1)?1.0:x->AMdAngle;
	if(x->AMverbose == 1)post("Changed angle to %f\n",x->AMdAngle);
}

void ambimic_tilde_pattern(t_ambimic_tilde *x, t_floatarg AMpat )
{
	x->AMdPattern = AMpat;
	x->AMdPattern = (x->AMdPattern<0)?0.0:(x->AMdPattern>1)?1.0:x->AMdPattern;
	if(x->AMverbose == 1)post("Changed pattern to %f\n",x->AMdPattern);
}

void ambimic_tilde_verbose(t_ambimic_tilde *x, t_floatarg AMver )
{
	x->AMverbose = AMver;
	x->AMverbose = (x->AMverbose<0)?0.0:(x->AMverbose>1)?1.0:x->AMverbose;
}

void ambimic_tilde_dsp(t_ambimic_tilde *x, t_signal **sp)
{
	dsp_add(ambimic_tilde_perform, 8, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec, sp[0]->s_n);
}

void *ambimic_tilde_new(void)
{
	t_ambimic_tilde *x = (t_ambimic_tilde *)pd_new(ambimic_tilde_class);
	ambimicProgram(x);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	return (void *)x;
}

void ambimic_tilde_setup(void) 
{
	ambimic_tilde_class = class_new(gensym("ambimic~"),(t_newmethod)ambimic_tilde_new,0, sizeof(t_ambimic_tilde),CLASS_DEFAULT,A_DEFFLOAT,0);
	class_addmethod(ambimic_tilde_class,
					(t_method)ambimic_tilde_dsp, gensym("dsp"), 0);
	class_addmethod(ambimic_tilde_class, (t_method)ambimic_tilde_azimuth,gensym("azimuth"), A_DEFFLOAT, 0);
	class_addmethod(ambimic_tilde_class, (t_method)ambimic_tilde_elevation,gensym("elevation"), A_DEFFLOAT, 0);
	class_addmethod(ambimic_tilde_class, (t_method)ambimic_tilde_angle,gensym("angle"), A_DEFFLOAT, 0);
	class_addmethod(ambimic_tilde_class, (t_method)ambimic_tilde_pattern,gensym("pattern"), A_DEFFLOAT, 0);
	class_addmethod(ambimic_tilde_class, (t_method)ambimic_tilde_verbose,gensym("verbose"), A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(ambimic_tilde_class, t_ambimic_tilde, AMf);
}


/*******************************************ambiproc~****************************************************/

static t_class *ambiproc_tilde_class;
typedef struct _ambiproc_tilde {
	t_object  x_obj;
	t_sample APRf;
	t_float APRorder;
	t_float APRverbose;
	double APRrotate;
	double APRtilt;
	double APRtumble;
	double APRdSinRotateAngleOld;
	double APRdSinTiltAngleOld;
	double APRdSinTumbleAngleOld;
	double APRdCosRotateAngleOld;
	double APRdCosTiltAngleOld;
	double APRdCosTumbleAngleOld;
	double APRdSinRotateAngle;
	double APRdSinTiltAngle;
	double APRdSinTumbleAngle;
	double APRdCosRotateAngle;
	double APRdCosTiltAngle;
	double APRdCosTumbleAngle;
	
	double APRnewW;
	double APRnewX;
	double APRnewY;
	double APRnewZ;
	long  APRlOrderOfProcessing;
	
	float APRtemprotate;
	float APRain1;
	float APRain2;
	float APRain3;
	float APRain4;
	double APRdX1;
	double APRdY1;
	double APRdZ1;
	float APRtemptilt;
	float APRtemptumble;
} t_ambiproc_tilde;

enum 
{
	APRkRoTiTu = 0,
	APRkRoTuTi = 1,
	APRkTiRoTu = 2,
	APRkTiTuRo = 3,
	APRkTuRoTi = 4,
	APRkTuTiRo = 5
};

void ambiproc_Program(t_ambiproc_tilde *x)
{
	
	x->APRrotate		= 0.0;
	x->APRtilt		= 0.0;
	x->APRtumble		= 0.0;
	x->APRdSinRotateAngleOld		= 1.0;
	x->APRdSinTiltAngleOld	= 1.0;
	x->APRdSinTumbleAngleOld	= 1.0;
	x->APRdCosRotateAngleOld		= 0.0;
	x->APRdCosTiltAngleOld	= 0.0;
	x->APRdCosTumbleAngleOld	= 0.0;
}

t_int *ambiproc_tilde_perform(t_int *w)
{
	t_ambiproc_tilde *x = (t_ambiproc_tilde *)(w[1]);
	t_sample  *APRin1 =    (t_sample *)(w[2]);
	t_sample  *APRin2 =    (t_sample *)(w[3]);
	t_sample  *APRin3 =    (t_sample *)(w[4]);
	t_sample  *APRin4 =    (t_sample *)(w[5]);
	t_sample  *APRout1 =    (t_sample *)(w[6]);
	t_sample  *APRout2 =    (t_sample *)(w[7]);
	t_sample  *APRout3 =    (t_sample *)(w[8]);
	t_sample  *APRout4 =    (t_sample *)(w[9]);
	int          APRn =           (int)(w[10]);
	
	
	x->APRdSinRotateAngle	=  kAntiDenorm + sin(x->APRrotate);
	x->APRdSinTumbleAngle	=  kAntiDenorm + sin(x->APRtilt);
	x->APRdSinTiltAngle	=  kAntiDenorm + sin(x->APRtumble);
	x->APRdCosRotateAngle	=  kAntiDenorm + cos(x->APRrotate);
	x->APRdCosTumbleAngle	=  kAntiDenorm + cos(x->APRtilt);
	x->APRdCosTiltAngle	=  kAntiDenorm + cos(x->APRtumble);
	
	while (APRn--){
		x->APRain1 = (*APRin1++);
		x->APRain2 = (*APRin2++);
		x->APRain3 = (*APRin3++);
		x->APRain4 = (*APRin4++);
		
		switch ((int)x->APRorder){
				
			case APRkRoTuTi	:
				
				x->APRdY1 = kAntiDenorm + (x->APRain2 * x->APRdSinRotateAngleOld) + (x->APRain3 * x->APRdCosRotateAngleOld);
				x->APRdX1 = kAntiDenorm + (x->APRain2 * x->APRdCosRotateAngleOld) - (x->APRain3 * x->APRdSinRotateAngleOld);
				x->APRdZ1 = kAntiDenorm + (x->APRdX1 * x->APRdSinTumbleAngleOld) + (x->APRain4 * x->APRdCosTumbleAngleOld);
				
				*APRout1++ = x->APRain1;
				*APRout2++ = kAntiDenorm + (x->APRdX1 * x->APRdCosTumbleAngleOld) - (x->APRain4 * x->APRdSinTumbleAngleOld);
				*APRout3++ = kAntiDenorm + (x->APRdY1 * x->APRdCosTiltAngleOld) - (x->APRdZ1 * x->APRdSinTiltAngleOld);
				*APRout4++ = kAntiDenorm + (x->APRdY1 * x->APRdSinTiltAngleOld) + (x->APRdZ1 * x->APRdCosTiltAngleOld);
				break;
				
			case APRkTiRoTu	:
				x->APRdY1 = kAntiDenorm + (x->APRain3 * x->APRdCosTiltAngleOld) - (x->APRain4 * x->APRdSinTiltAngleOld);
				x->APRdZ1 = kAntiDenorm + (x->APRain3 * x->APRdSinTiltAngleOld) + (x->APRain4 * x->APRdCosTiltAngleOld);
				x->APRdX1 = kAntiDenorm + (x->APRain2 * x->APRdCosRotateAngleOld) - (x->APRdY1 * x->APRdSinRotateAngleOld);
				*APRout1++ = x->APRain1;
				*APRout2++ = kAntiDenorm + (x->APRdX1 * x->APRdCosTumbleAngleOld) - (x->APRdZ1 * x->APRdSinTumbleAngleOld);
				*APRout3++ = kAntiDenorm + (x->APRain2 * x->APRdSinRotateAngleOld) + (x->APRdY1 * x->APRdCosRotateAngleOld);
				*APRout4++ = kAntiDenorm + (x->APRdX1 * x->APRdSinTumbleAngleOld) + (x->APRdZ1 * x->APRdCosTumbleAngleOld);
				break;
				
			case APRkTiTuRo	:
				x->APRdY1 = kAntiDenorm + (x->APRain3 * x->APRdCosTiltAngleOld) - (x->APRain4 * x->APRdSinTiltAngleOld);
				x->APRdZ1 = kAntiDenorm + (x->APRain3 * x->APRdSinTiltAngleOld) + (x->APRain4 * x->APRdCosTiltAngleOld);
				x->APRdX1 = kAntiDenorm + (x->APRain2 * x->APRdCosTumbleAngleOld) - (x->APRdZ1 * x->APRdSinTumbleAngleOld);
				*APRout1++ = x->APRain1;
				*APRout2++ = kAntiDenorm + (x->APRdX1 * x->APRdCosRotateAngleOld) - (x->APRdY1 * x->APRdSinRotateAngleOld);
				*APRout3++ = kAntiDenorm + (x->APRdX1 * x->APRdSinRotateAngleOld) + (x->APRdY1 * x->APRdCosRotateAngleOld);
				*APRout4++ = kAntiDenorm + (x->APRain2 * x->APRdSinTumbleAngleOld) + (x->APRdZ1 * x->APRdCosTumbleAngleOld);
				
				break;
				
			case APRkTuRoTi	:
				x->APRdX1 = kAntiDenorm + (x->APRain2 * x->APRdCosTumbleAngleOld) - (x->APRain4 * x->APRdSinTumbleAngleOld);
				x->APRdZ1 = kAntiDenorm + (x->APRain2 * x->APRdSinTumbleAngleOld) + (x->APRain4 * x->APRdCosTumbleAngleOld);
				x->APRdY1 = kAntiDenorm + (x->APRdX1 * x->APRdSinRotateAngleOld) + (x->APRain3 * x->APRdCosRotateAngleOld);
				*APRout1++ = x->APRain1;
				*APRout2++ = kAntiDenorm + (x->APRdX1 * x->APRdCosRotateAngleOld) - (x->APRain3 * x->APRdSinRotateAngleOld);
				*APRout3++ = kAntiDenorm + (x->APRdY1 * x->APRdCosTiltAngleOld) - (x->APRdZ1 * x->APRdSinTiltAngleOld);
				*APRout4++ = kAntiDenorm + (x->APRdY1 * x->APRdSinTiltAngleOld) + (x->APRdZ1 * x->APRdCosTiltAngleOld);
				break;
			case APRkTuTiRo	:
				
				x->APRdX1 = kAntiDenorm + (x->APRain2 * x->APRdCosTumbleAngleOld) - (x->APRain4 * x->APRdSinTumbleAngleOld);
				x->APRdZ1 = kAntiDenorm + (x->APRain2 * x->APRdSinTumbleAngleOld) + (x->APRain4 * x->APRdCosTumbleAngleOld);
				x->APRdY1 = kAntiDenorm + (x->APRain3 * x->APRdCosTiltAngleOld) - (x->APRdZ1 * x->APRdSinTiltAngleOld);
				*APRout1++ = x->APRain1;
				*APRout2++ = kAntiDenorm + (x->APRdX1 * x->APRdCosRotateAngleOld) - (x->APRdY1 * x->APRdSinRotateAngleOld);
				*APRout3++ = kAntiDenorm + (x->APRdX1 * x->APRdSinRotateAngleOld) + (x->APRdY1 * x->APRdCosRotateAngleOld);
				*APRout4++ = kAntiDenorm + (x->APRain3 * x->APRdSinTiltAngleOld) + (x->APRdZ1 * x->APRdCosTiltAngleOld);
				
				break;
				
			case APRkRoTiTu	:
			default:
				x->APRdY1 = kAntiDenorm + (x->APRain2 * x->APRdSinRotateAngleOld) + (x->APRain3 * x->APRdCosRotateAngleOld);
				x->APRdX1 = kAntiDenorm + (x->APRain2 * x->APRdCosRotateAngleOld) - (x->APRain3 * x->APRdSinRotateAngleOld);
				x->APRdZ1 = kAntiDenorm + (x->APRdY1 * x->APRdSinTiltAngleOld) + (x->APRain4 * x->APRdCosTiltAngleOld);
				*APRout1++ = x->APRain1;
				*APRout2++ = kAntiDenorm + (x->APRdX1 * x->APRdCosTumbleAngleOld) - (x->APRdZ1 * x->APRdSinTumbleAngleOld);
				*APRout3++ = kAntiDenorm + (x->APRdY1 * x->APRdCosTiltAngleOld) - (x->APRain4 * x->APRdSinTiltAngleOld);
				*APRout4++ = kAntiDenorm + (x->APRdX1 * x->APRdSinTumbleAngleOld) + (x->APRdZ1 * x->APRdCosTumbleAngleOld);
				
				
		}
		
        
		x->APRdSinRotateAngleOld	= (x->APRdSinRotateAngleOld * kAvCoef1) + (kAvCoef2 * x->APRdSinRotateAngle);
		x->APRdSinTiltAngleOld	= (x->APRdSinTiltAngleOld * kAvCoef1) + (kAvCoef2 * x->APRdSinTiltAngle);
		x->APRdSinTumbleAngleOld	= (x->APRdSinTumbleAngleOld * kAvCoef1) + (kAvCoef2 * x->APRdSinTumbleAngle);
		x->APRdCosRotateAngleOld	= (x->APRdCosRotateAngleOld * kAvCoef1) + (kAvCoef2 * x->APRdCosRotateAngle);
		x->APRdCosTiltAngleOld	= (x->APRdCosTiltAngleOld * kAvCoef1) + (kAvCoef2 * x->APRdCosTiltAngle);
		x->APRdCosTumbleAngleOld	= (x->APRdCosTumbleAngleOld * kAvCoef1) + (kAvCoef2 * x->APRdCosTumbleAngle);
	}
	return (w+11);
	
}

void rotateField(t_ambiproc_tilde *x,double APRangle)
{
	double APRdX;
	double APRdY;
	APRdX = x->APRnewX;
	APRdY = x->APRnewY;
	x->APRnewY = (APRdX * sin(APRangle)) + (APRdY * cos(APRangle));
	x->APRnewX = (APRdX * cos(APRangle)) - (APRdY * sin(APRangle));
}



void tiltField(t_ambiproc_tilde *x,double APRangle)
{
	double APRdZ;
	double APRdY;
	APRdZ = x->APRnewZ;
	APRdY = x->APRnewY;
	x->APRnewY = (APRdY * cos(APRangle)) - (APRdZ * sin(APRangle));
	x->APRnewZ = (APRdY * sin(APRangle)) + (APRdZ * cos(APRangle));
	
}



void tumbleField(t_ambiproc_tilde *x,double APRangle)
{
	double APRdX;
	double APRdZ;
	APRdX = x->APRnewX;
	APRdZ = x->APRnewZ;
	x->APRnewX = (APRdX * cos(APRangle)) - (APRdZ * sin(APRangle));
	x->APRnewZ = (APRdX * sin(APRangle)) + (APRdZ * cos(APRangle));
	
}

void ambiproc_tilde_rotate(t_ambiproc_tilde *x, t_floatarg APRrot )
{
	x->APRtemprotate = APRrot;
	x->APRtemprotate = (x->APRtemprotate<0)?0.0:(x->APRtemprotate>1)?1.0:x->APRtemprotate;
	if(x->APRverbose == 1)post("Changing Rotate to %f\n",x->APRtemprotate);
	x->APRrotate 	= (double)(-x->APRtemprotate) * (2 * kPI);
}


void ambiproc_tilde_tilt(t_ambiproc_tilde *x, t_floatarg APRtil )
{
	x->APRtemptilt = APRtil;
	x->APRtemptilt = (x->APRtemptilt<0)?0.0:(x->APRtemptilt>1)?1.0:x->APRtemptilt;
	if(x->APRverbose == 1)post("Changing Tilt to %f\n",x->APRtemptilt);
	x->APRtilt 	= (double)(-x->APRtemptilt) * (2 * kPI);
}

void ambiproc_tilde_tumble(t_ambiproc_tilde *x, t_floatarg APRtum )
{
	x->APRtemptumble = APRtum;
	x->APRtemptumble = (x->APRtemptumble<0)?0.0:(x->APRtemptumble>1)?1.0:x->APRtemptumble;
	if(x->APRverbose == 1)post("Changing Tumble to %f\n",x->APRtemptumble);
	x->APRtumble 	= (double)(-x->APRtemptumble) * (2 * kPI);
}
void ambiproc_tilde_verbose(t_ambiproc_tilde *x, t_floatarg APRver )
{
	x->APRverbose = APRver;
	x->APRverbose = (x->APRverbose<0)?0.0:(x->APRverbose>1)?1.0:x->APRverbose;
}

void ambiproc_tilde_dsp(t_ambiproc_tilde *x, t_signal **sp)
{
	dsp_add(ambiproc_tilde_perform, 10, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec,sp[6]->s_vec,sp[7]->s_vec, sp[0]->s_n);
}

void *ambiproc_tilde_new(t_floatarg APRord)
{
	t_ambiproc_tilde *x = (t_ambiproc_tilde *)pd_new(ambiproc_tilde_class);
	x->APRorder = APRord;
	x->APRorder = (x->APRorder<0)?0.0:(x->APRorder>5)?5.0:x->APRorder;
	if(x->APRorder == 0)post("Rotate-Tilt-Tumble\n");
	if(x->APRorder == 1)post("Rotate-Tumble-Tilt\n");
	if(x->APRorder == 2)post("Tilt-Rotate-Tumble\n");
	if(x->APRorder == 3)post("Tilt-Tumble-Rotate\n");
	if(x->APRorder == 4)post("Tumble-Rotate-Tilt\n");
	if(x->APRorder == 5)post("Tumble-Tilt-Rotate\n");
	ambiproc_Program(x);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	return (void *)x;
}

void ambiproc_tilde_setup(void) 
{
	ambiproc_tilde_class = class_new(gensym("ambiproc~"),(t_newmethod)ambiproc_tilde_new,0, sizeof(t_ambiproc_tilde),CLASS_DEFAULT,A_DEFFLOAT,0);
	class_addmethod(ambiproc_tilde_class,
					(t_method)ambiproc_tilde_dsp, gensym("dsp"), 0);
	class_addmethod(ambiproc_tilde_class, (t_method)ambiproc_tilde_rotate,gensym("rotate"), A_DEFFLOAT, 0);
	class_addmethod(ambiproc_tilde_class, (t_method)ambiproc_tilde_tilt,gensym("tilt"), A_DEFFLOAT, 0);
	class_addmethod(ambiproc_tilde_class, (t_method)ambiproc_tilde_tumble,gensym("tumble"), A_DEFFLOAT, 0);
	class_addmethod(ambiproc_tilde_class, (t_method)ambiproc_tilde_verbose,gensym("verbose"), A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(ambiproc_tilde_class, t_ambiproc_tilde, APRf);
}

/**********************************************ambizoom~***************************************************/

static t_class *ambizoom_tilde_class;
typedef struct _ambizoom_tilde {
	t_object  x_obj;
	t_sample AZf;
	double AZdAzimuth;
	double AZdElevation;
	double AZdZoom;
	
	double AZdCosAzimuthPre;
	double AZdCosElevationPre;
	double AZdSinAzimuthPre;
	double AZdSinElevationPre;
	
	double AZdCosAzimuthPost;
	double AZdCosElevationPost;
	double AZdSinAzimuthPost;
	double AZdSinElevationPost;
	
	double AZdCosAzimuthPreOld;
	double AZdCosElevationPreOld;
	double AZdSinAzimuthPreOld;
	double AZdSinElevationPreOld;
	
	double AZdCosAzimuthPostOld;
	double AZdCosElevationPostOld;
	double AZdSinAzimuthPostOld;
	double AZdSinElevationPostOld;
	double AZdZoomOld;
	
	
	double AZnewW;
	double AZnewX;
	double AZnewY;
	double AZnewZ;
	
	float AZfAzimuth;
	float AZfElevation;
	float AZfZoom;
	
	float AZain1;
	float AZain2;
	float AZain3;
	float AZain4;
	
	float AZtempazi;
	float AZtempele;
	float AZtempzoom;	
	double AZdW;
	double AZdX;
	double AZdY;
	double AZoomYY,AZoomZZ, AZoomWW, AZoomXW, AZoomWX;
	float AZverbose;
} t_ambizoom_tilde;

void ambizoom_Program(t_ambizoom_tilde *x)
{
	x->AZdAzimuth= 0.0;
	x->AZdElevation= 0.25;
	
	x->AZdCosAzimuthPre 	 = 1.0;
	x->AZdCosElevationPre = 1.0;
	x->AZdSinAzimuthPre   = 0.0;
	x->AZdSinElevationPre = 0.0;
	
	x->AZdCosAzimuthPost   = 1.0;
	x->AZdCosElevationPost = 1.0;
	x->AZdSinAzimuthPost   = 0.0;
	x->AZdSinElevationPost = 0.0;
	
	x->AZdCosAzimuthPreOld   = 1.0;
	x->AZdCosElevationPreOld = 1.0;
	x->AZdSinAzimuthPreOld   = 0.0;
	x->AZdSinElevationPreOld = 0.0;
	
	x->AZdCosAzimuthPostOld   = 1.0;
	x->AZdCosElevationPostOld = 1.0;
	x->AZdSinAzimuthPostOld   = 0.0;
	x->AZdSinElevationPostOld = 0.0;
	
	x->AZdZoom= 0.0;
	x->AZdZoomOld= 0.0;
	
	
	x->AZnewW= 0.0;
	x->AZnewX= 0.0;
	x->AZnewY= 0.0;
	x->AZnewZ= 0.0;
	
	x->AZfAzimuth= 0.0;
	x->AZfElevation= 0.25;
	x->AZfZoom= 0.5;
}

t_int *ambizoom_tilde_perform(t_int *w)
{
	t_ambizoom_tilde *x = (t_ambizoom_tilde *)(w[1]);
	t_sample  *AZin1 =    (t_sample *)(w[2]);
	t_sample  *AZin2 =    (t_sample *)(w[3]);
	t_sample  *AZin3 =    (t_sample *)(w[4]);
	t_sample  *AZin4 =    (t_sample *)(w[5]);
	t_sample  *AZout1 =    (t_sample *)(w[6]);
	t_sample  *AZout2 =    (t_sample *)(w[7]);
	t_sample  *AZout3 =    (t_sample *)(w[8]);
	t_sample  *AZout4 =    (t_sample *)(w[9]);
	int          AZn =           (int)(w[10]);
	
	x->AZdCosAzimuthPre 		=  kAntiDenorm + cos(x->AZdAzimuth);
	x->AZdCosElevationPre	=  kAntiDenorm + cos(x->AZdElevation);
	x->AZdSinAzimuthPre		=  kAntiDenorm + sin(x->AZdAzimuth);
	x->AZdSinElevationPre	=  kAntiDenorm + sin(x->AZdElevation);
	
	x->AZdCosAzimuthPost		=  kAntiDenorm + cos(-x->AZdAzimuth);
	x->AZdCosElevationPost 	=  kAntiDenorm + cos(-x->AZdElevation);
	x->AZdSinAzimuthPost		=  kAntiDenorm + sin(-x->AZdAzimuth);
	x->AZdSinElevationPost	=  kAntiDenorm + sin(-x->AZdElevation);
	
	while (AZn--){
		x->AZain1 = (*AZin1++);
		x->AZain2 = (*AZin2++);
		x->AZain3 = (*AZin3++);
		x->AZain4 = (*AZin4++);
		x->AZnewW =  kAntiDenorm + (double)x->AZain1;
		x->AZnewX =  kAntiDenorm + (double)x->AZain2;
		x->AZnewY =  kAntiDenorm + (double)x->AZain3;
		x->AZnewZ =  kAntiDenorm + (double)x->AZain4;
		
		x->AZdY = (x->AZnewX * x->AZdSinAzimuthPreOld) + (x->AZnewY * x->AZdCosAzimuthPreOld);
		x->AZdX = (x->AZnewX * x->AZdCosAzimuthPreOld) - (x->AZnewY * x->AZdSinAzimuthPreOld);
		
		x->AZnewX = (x->AZdX * x->AZdCosElevationPreOld) - (x->AZnewZ * x->AZdSinElevationPreOld);
		x->AZnewY = x->AZdY;
		x->AZnewZ = (x->AZdX * x->AZdSinElevationPreOld) + (x->AZnewZ * x->AZdCosElevationPreOld);
		
		x->AZdW = x->AZnewW;
		x->AZdX = x->AZnewX;
		
		x->AZoomWW = 1.0 + (0.414* fabs(x->AZdZoomOld));	
		
		x->AZoomXW = x->AZdZoomOld/2;
		x->AZoomWX = x->AZdZoomOld;
		x->AZoomYY = sqrt(1 - x->AZdZoomOld*x->AZdZoomOld);
		x->AZoomZZ = x->AZoomYY;
		
		x->AZnewW = (x->AZoomWW * x->AZdW) + (x->AZoomXW * x->AZdX);
		x->AZnewX = x->AZdX + (x->AZoomWX * x->AZdW);
		x->AZnewY = x->AZnewY * x->AZoomYY;	
		x->AZnewZ = x->AZnewZ * x->AZoomZZ;	
		
		x->AZdX = (x->AZnewX * x->AZdCosElevationPostOld) - (x->AZnewZ * x->AZdSinElevationPostOld);
		x->AZnewZ = (x->AZnewX * x->AZdSinElevationPostOld) + (x->AZnewZ * x->AZdCosElevationPostOld);
		
		x->AZdY	 = x->AZnewY;
		x->AZnewY = (x->AZdX * x->AZdSinAzimuthPostOld) + (x->AZdY * x->AZdCosAzimuthPostOld);
		x->AZnewX = (x->AZdX * x->AZdCosAzimuthPostOld) - (x->AZdY * x->AZdSinAzimuthPostOld);
		
        *AZout1++ = (float)x->AZnewW;
        *AZout2++ = (float)x->AZnewX;
        *AZout3++ = (float)x->AZnewY;
        *AZout4++ = (float)x->AZnewZ;
        
        x->AZdCosAzimuthPreOld	= (x->AZdCosAzimuthPreOld * kAvCoef1) + (kAvCoef2 * x->AZdCosAzimuthPre);
		x->AZdCosElevationPreOld	= (x->AZdCosElevationPreOld * kAvCoef1) + (kAvCoef2 * x->AZdCosElevationPre);
        x->AZdSinAzimuthPreOld	= (x->AZdSinAzimuthPreOld * kAvCoef1) + (kAvCoef2 * x->AZdSinAzimuthPre);
		x->AZdSinElevationPreOld	= (x->AZdSinElevationPreOld * kAvCoef1) + (kAvCoef2 * x->AZdSinElevationPre);
        x->AZdCosAzimuthPostOld	= (x->AZdCosAzimuthPostOld * kAvCoef1) + (kAvCoef2 * x->AZdCosAzimuthPost);
		x->AZdCosElevationPostOld= (x->AZdCosElevationPostOld * kAvCoef1) + (kAvCoef2 * x->AZdCosElevationPost);
        x->AZdSinAzimuthPostOld	= (x->AZdSinAzimuthPostOld * kAvCoef1) + (kAvCoef2 * x->AZdSinAzimuthPost);
		x->AZdSinElevationPostOld= (x->AZdSinElevationPostOld * kAvCoef1) + (kAvCoef2 * x->AZdSinElevationPost);
 		x->AZdZoomOld			= (x->AZdZoomOld * kAvCoef1) + (kAvCoef2 * x->AZdZoom);
	}
	return (w+11);
}

void ambizoom_tilde_azimuth(t_ambizoom_tilde *x, t_floatarg AZazi )
{
	x->AZtempazi = AZazi;
	x->AZtempazi = (x->AZtempazi<0)?0.0:(x->AZtempazi>1)?1.0:x->AZtempazi;
	x->AZdAzimuth = (-x->AZtempazi) * (2 * kPI);
	if(x->AZverbose == 1)post("Changed azimuth to %f\n",x->AZtempazi);
}

void ambizoom_tilde_elevation(t_ambizoom_tilde *x, t_floatarg AZele )
{
	x->AZtempele = AZele;
	x->AZtempele = (x->AZtempele<0)?0.0:(x->AZtempele>1)?1.0:x->AZtempele;
	x->AZdElevation = (x->AZtempele - 0.5) * kPI;
	if(x->AZverbose == 1)post("Changed elevation to %f\n",x->AZtempele);
}

void ambizoom_tilde_zoom(t_ambizoom_tilde *x, t_floatarg AZzoo)
{
	x->AZtempzoom = AZzoo;
	x->AZtempzoom = (x->AZtempzoom<0)?0.0:(x->AZtempzoom>1)?1.0:x->AZtempzoom;
	x->AZdZoom	= (double)((0.5 - x->AZtempzoom) * 2);
	if(x->AZverbose == 1)post("Changed Zoom to %f\n",x->AZtempzoom);
}

void ambizoom_tilde_verbose(t_ambizoom_tilde *x, t_floatarg AZver )
{
	x->AZverbose = AZver;
	x->AZverbose = (x->AZverbose<0)?0.0:(x->AZverbose>1)?1.0:x->AZverbose;
}

void ambizoom_tilde_dsp(t_ambizoom_tilde *x, t_signal **sp)
{
	dsp_add(ambizoom_tilde_perform, 10, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec,sp[6]->s_vec,sp[7]->s_vec, sp[0]->s_n);
}

void *ambizoom_tilde_new(void)
{
	t_ambizoom_tilde *x = (t_ambizoom_tilde *)pd_new(ambizoom_tilde_class);
	ambizoom_Program(x);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	return (void *)x;
}

void ambizoom_tilde_setup(void) 
{
	ambizoom_tilde_class = class_new(gensym("ambizoom~"),(t_newmethod)ambizoom_tilde_new,0, sizeof(t_ambizoom_tilde),CLASS_DEFAULT,A_DEFFLOAT,0);
	class_addmethod(ambizoom_tilde_class,
					(t_method)ambizoom_tilde_dsp, gensym("dsp"), 0);
	class_addmethod(ambizoom_tilde_class, (t_method)ambizoom_tilde_azimuth,gensym("azimuth"), A_DEFFLOAT, 0);
	class_addmethod(ambizoom_tilde_class, (t_method)ambizoom_tilde_elevation,gensym("elevation"), A_DEFFLOAT, 0);
	class_addmethod(ambizoom_tilde_class, (t_method)ambizoom_tilde_zoom,gensym("zoom"), A_DEFFLOAT, 0);
	class_addmethod(ambizoom_tilde_class, (t_method)ambizoom_tilde_verbose,gensym("verbose"), A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(ambizoom_tilde_class, t_ambizoom_tilde, AZf);
}

/*******************************************ambiplane~*****************************************************/

static t_class *ambiplane_tilde_class;
typedef struct _ambiplane_tilde {
	t_object  x_obj;
	t_sample APLf;
	t_float APLverbose;
  	double APLdAzimuth;
	double APLdElevation;
	double APLdPlaneMirror;
	
	double APLdCosAzimuthPre;
	double APLdCosElevationPre;
	double APLdSinAzimuthPre;
	double APLdSinElevationPre;
	
	double APLdCosAzimuthPost;
	double APLdCosElevationPost;
	double APLdSinAzimuthPost;
	double APLdSinElevationPost;
	
	double APLdCosAzimuthPreOld;
	double APLdCosElevationPreOld;
	double APLdSinAzimuthPreOld;
	double APLdSinElevationPreOld;
	
	double APLdCosAzimuthPostOld;
	double APLdCosElevationPostOld;
	double APLdSinAzimuthPostOld;
	double APLdSinElevationPostOld;
	double APLdPlaneMirrorOld;
	
	
	double APLnewW;
	double APLnewX;
	double APLnewY;
	double APLnewZ;
	
	double APLdX;
	double APLdY;
	
	float APLtempazi;
	float APLtempele;
	float APLtempmir;
} t_ambiplane_tilde;

void ambiplane_Program(t_ambiplane_tilde *x)
{
	x->APLdAzimuth= 0.0;
	x->APLdElevation= 0.5;
	
	x->APLdCosAzimuthPre 	 = 1.0;
	x->APLdCosElevationPre = 1.0;
	x->APLdSinAzimuthPre   = 0.0;
	x->APLdSinElevationPre = 0.0;
	
	x->APLdCosAzimuthPost   = 1.0;
	x->APLdCosElevationPost = 1.0;
	x->APLdSinAzimuthPost   = 0.0;
	x->APLdSinElevationPost = 0.0;
	
	x->APLdCosAzimuthPreOld   = 1.0;
	x->APLdCosElevationPreOld = 1.0;
	x->APLdSinAzimuthPreOld   = 0.0;
	x->APLdSinElevationPreOld = 0.0;
	
	x->APLdCosAzimuthPostOld   = 1.0;
	x->APLdCosElevationPostOld = 1.0;
	x->APLdSinAzimuthPostOld   = 0.0;
	x->APLdSinElevationPostOld = 0.0;
	x->APLdPlaneMirror= 0.0;
	x->APLdPlaneMirrorOld= 0.0;
	x->APLdCosAzimuthPreOld 	= kAntiDenorm ;
	x->APLdCosElevationPreOld	= kAntiDenorm ;
	x->APLdSinAzimuthPreOld	= kAntiDenorm ;
	x->APLdSinElevationPreOld	= kAntiDenorm ;
	
	x->APLdCosAzimuthPostOld	= kAntiDenorm ;
	x->APLdCosElevationPostOld= kAntiDenorm ;
	x->APLdSinAzimuthPostOld	= kAntiDenorm ;
	x->APLdSinElevationPostOld= kAntiDenorm ;
	
	x->APLnewW= 0.0;
	x->APLnewX= 0.0;
	x->APLnewY= 0.0;
	x->APLnewZ= 0.0;
}

t_int *ambiplane_tilde_perform(t_int *w)
{
	t_ambiplane_tilde *x = (t_ambiplane_tilde *)(w[1]);
	t_sample  *APLin1 =    (t_sample *)(w[2]);
	t_sample  *APLin2 =    (t_sample *)(w[3]);
	t_sample  *APLin3 =    (t_sample *)(w[4]);
	t_sample  *APLin4 =    (t_sample *)(w[5]);
	t_sample  *APLout1 =    (t_sample *)(w[6]);
	t_sample  *APLout2 =    (t_sample *)(w[7]);
	t_sample  *APLout3 =    (t_sample *)(w[8]);
	t_sample  *APLout4 =    (t_sample *)(w[9]);
	int          APLn =           (int)(w[10]);
	
	x->APLdCosAzimuthPre 		= cos(x->APLdAzimuth);
	x->APLdCosElevationPre	= cos(x->APLdElevation);
	x->APLdSinAzimuthPre		= sin(x->APLdAzimuth);
	x->APLdSinElevationPre	= sin(x->APLdElevation);
	
	x->APLdCosAzimuthPost		= cos(-x->APLdAzimuth);
	x->APLdCosElevationPost 	= cos(-x->APLdElevation);
	x->APLdSinAzimuthPost		= sin(-x->APLdAzimuth);
	x->APLdSinElevationPost	= sin(-x->APLdElevation);
	
	while (APLn--){
		
		
		x->APLnewW = kAntiDenorm + (double)(*APLin1++);
		x->APLnewX = kAntiDenorm + (double)(*APLin2++);
		x->APLnewY = kAntiDenorm + (double)(*APLin3++);
		x->APLnewZ = kAntiDenorm + (double)(*APLin4++);
		
		
        x->APLdY = kAntiDenorm + (x->APLnewX * x->APLdSinAzimuthPreOld) + (x->APLnewY * x->APLdCosAzimuthPreOld);
		x->APLdX = kAntiDenorm + (x->APLnewX * x->APLdCosAzimuthPreOld) - (x->APLnewY * x->APLdSinAzimuthPreOld);
		// rotate field then
		
		x->APLnewX = kAntiDenorm + (x->APLdX * x->APLdCosElevationPreOld) - (x->APLnewZ * x->APLdSinElevationPreOld);
		x->APLnewY = kAntiDenorm + x->APLdY;
		x->APLnewZ = kAntiDenorm + (x->APLdX * x->APLdSinElevationPreOld) + (x->APLnewZ * x->APLdCosElevationPreOld);
		// tumble field which points the front at the desired point then
		
		x->APLnewX = kAntiDenorm + x->APLdPlaneMirrorOld * x->APLnewX;
		// do the mirror itself
		
		
		
		x->APLdX = kAntiDenorm + (x->APLnewX * x->APLdCosElevationPostOld) - (x->APLnewZ * x->APLdSinElevationPostOld);
		x->APLnewZ = kAntiDenorm + (x->APLnewX * x->APLdSinElevationPostOld) + (x->APLnewZ * x->APLdCosElevationPostOld);
		// tumble field to return to horizontal then
		
		x->APLdY	 = kAntiDenorm + x->APLnewY;
		x->APLnewY = kAntiDenorm + (x->APLdX * x->APLdSinAzimuthPostOld) + (x->APLdY * x->APLdCosAzimuthPostOld);
		x->APLnewX = kAntiDenorm + (x->APLdX * x->APLdCosAzimuthPostOld) - (x->APLdY * x->APLdSinAzimuthPostOld);
		// rotate field to et it back to he start
		
		
		
        (*APLout1++) = kAntiDenorm + (float) x->APLnewW;	// replacing
        (*APLout2++) = kAntiDenorm + (float) x->APLnewX;	// replacing
        (*APLout3++) = kAntiDenorm + (float) x->APLnewY;	// replacing
        (*APLout4++) = kAntiDenorm + (float) x->APLnewZ;	// replacing
        x->APLdCosAzimuthPreOld	= (x->APLdCosAzimuthPreOld * kAvCoef1) + (kAvCoef2 * x->APLdCosAzimuthPre);
		x->APLdCosElevationPreOld	= (x->APLdCosElevationPreOld * kAvCoef1) + (kAvCoef2 * x->APLdCosElevationPre);
        x->APLdSinAzimuthPreOld	= (x->APLdSinAzimuthPreOld * kAvCoef1) + (kAvCoef2 * x->APLdSinAzimuthPre);
		x->APLdSinElevationPreOld	= (x->APLdSinElevationPreOld * kAvCoef1) + (kAvCoef2 * x->APLdSinElevationPre);
		
        x->APLdCosAzimuthPostOld	= (x->APLdCosAzimuthPostOld * kAvCoef1) + (kAvCoef2 * x->APLdCosAzimuthPost);
		x->APLdCosElevationPostOld= (x->APLdCosElevationPostOld * kAvCoef1) + (kAvCoef2 * x->APLdCosElevationPost);
        x->APLdSinAzimuthPostOld	= (x->APLdSinAzimuthPostOld * kAvCoef1) + (kAvCoef2 * x->APLdSinAzimuthPost);
		x->APLdSinElevationPostOld= (x->APLdSinElevationPostOld * kAvCoef1) + (kAvCoef2 * x->APLdSinElevationPost);
		
 		x->APLdPlaneMirrorOld		= (x->APLdPlaneMirrorOld * kAvCoef1) + (kAvCoef2 * x->APLdPlaneMirror);
		
		
		
	}
	return (w+11);
}

void ambiplane_tilde_azimuth(t_ambiplane_tilde *x, t_floatarg APLazi )
{
	
	x->APLtempazi = APLazi;
	x->APLtempazi = (x->APLtempazi<0)?0.0:(x->APLtempazi>1)?1.0:x->APLtempazi;
	x->APLdAzimuth = (double)(x->APLtempazi * (2 * kPI));
	if(x->APLverbose == 1)post("Changed azimuth to %f\n",x->APLtempazi);
}

void ambiplane_tilde_elevation(t_ambiplane_tilde *x, t_floatarg APLele )
{
	x->APLtempele = APLele;
	x->APLtempele = (x->APLtempele<0)?0.0:(x->APLtempele>1)?1.0:x->APLtempele;
	x->APLdElevation = (x->APLtempele - 0.5) * kPI;
	if(x->APLverbose == 1)post("Changed elevation to %f\n",x->APLtempele);
}

void ambiplane_tilde_mirror(t_ambiplane_tilde *x, t_floatarg APLmir)
{
	x->APLtempmir = APLmir;
	x->APLtempmir = (x->APLtempmir<0)?0.0:(x->APLtempmir>1)?1.0:x->APLtempmir;
	x->APLdPlaneMirror	= (double)((0.5 - x->APLtempmir) * 2);
	if(x->APLverbose == 1)post("Changed Mirror to %f\n",x->APLtempmir);
	
}

void ambiplane_tilde_verbose(t_ambiplane_tilde *x, t_floatarg APLver )
{
	x->APLverbose = APLver;
	x->APLverbose = (x->APLverbose<0)?0.0:(x->APLverbose>1)?1.0:x->APLverbose;
}

void ambiplane_tilde_dsp(t_ambiplane_tilde *x, t_signal **sp)
{
	dsp_add(ambiplane_tilde_perform, 10, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec,sp[6]->s_vec,sp[7]->s_vec, sp[0]->s_n);
}

void *ambiplane_tilde_new(void)
{
	t_ambiplane_tilde *x = (t_ambiplane_tilde *)pd_new(ambiplane_tilde_class);
	ambiplane_Program(x);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	return (void *)x;
}

void ambiplane_tilde_setup(void) 
{
	ambiplane_tilde_class = class_new(gensym("ambiplane~"),(t_newmethod)ambiplane_tilde_new,0, sizeof(t_ambiplane_tilde),CLASS_DEFAULT,A_DEFFLOAT,0);
	class_addmethod(ambiplane_tilde_class,
					(t_method)ambiplane_tilde_dsp, gensym("dsp"), 0);
	class_addmethod(ambiplane_tilde_class, (t_method)ambiplane_tilde_azimuth,gensym("azimuth"), A_DEFFLOAT, 0);
	class_addmethod(ambiplane_tilde_class, (t_method)ambiplane_tilde_elevation,gensym("elevation"), A_DEFFLOAT, 0);
	class_addmethod(ambiplane_tilde_class, (t_method)ambiplane_tilde_mirror,gensym("mirror"), A_DEFFLOAT, 0);
	class_addmethod(ambiplane_tilde_class, (t_method)ambiplane_tilde_verbose,gensym("verbose"), A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(ambiplane_tilde_class, t_ambiplane_tilde, APLf);
}

/*****************************************ambi2s~************************************************************/

static t_class *ambi2s_tilde_class;
typedef struct _ambi2s_tilde {
	t_object  x_obj;
	t_sample f;
	t_float verbose;
	double dAngleFront;
	double dAngleRear;
	double dPatternFront;
	double dPatternRear;
	double dGainLeft;
	double dGainRight;
	double dGainCenter;
	double dGainLFE;
	double dGainSLeft;
	double dGainSRight;
	double dGainSCenter;
	
	
	double dCosAzFrontLeft;
	double dCosElFrontLeft;
	double dSinAzFrontLeft;
	double dSinElFrontLeft;
	double dCosAzFrontRight;
	double dCosElFrontRight;
	double dSinAzFrontRight;
	double dSinElFrontRight;
	
	double dZerothLevelFront;
	double dFirstLevelFront;
	
	double dXCoeffFrontLeft;
	double dYCoeffFrontLeft;
	double dZCoeffFrontLeft;
	double dXCoeffFrontRight;
	double dYCoeffFrontRight;
	double dZCoeffFrontRight;
	
	double dZerothLevelFrontOld;
	
	double dXCoeffFrontLeftOld;
	double dYCoeffFrontLeftOld;
	double dZCoeffFrontLeftOld;
	double dXCoeffFrontRightOld;
	double dYCoeffFrontRightOld;
	double dZCoeffFrontRightOld;
	
	double newWFront;
	
	
	double dCosAzRearLeft;
	double dCosElRearLeft;
	double dSinAzRearLeft;
	double dSinElRearLeft;
	double dCosAzRearRight;
	double dCosElRearRight;
	double dSinAzRearRight;
	double dSinElRearRight;
	
	double dZerothLevelRear;
	double dFirstLevelRear;
	
	double dXCoeffRearLeft;
	double dYCoeffRearLeft;
	double dZCoeffRearLeft;
	double dXCoeffRearRight;
	double dYCoeffRearRight;
	double dZCoeffRearRight;
	
	double dZerothLevelRearOld;
	
	double dXCoeffRearLeftOld;
	double dYCoeffRearLeftOld;
	double dZCoeffRearLeftOld;
	double dXCoeffRearRightOld;
	double dYCoeffRearRightOld;
	double dZCoeffRearRightOld;
	
	double newWRear;
	
	
	double dCosAzCenterLeft;
	double dCosElCenterLeft;
	double dSinAzCenterLeft;
	double dSinElCenterLeft;
	double dCosAzCenterRight;
	double dCosElCenterRight;
	double dSinAzCenterRight;
	double dSinElCenterRight;
	
	double dZerothLevelCenter;
	double dFirstLevelCenter;
	
	double dXCoeffCenterLeft;
	double dYCoeffCenterLeft;
	double dZCoeffCenterLeft;
	double dXCoeffCenterRight;
	double dYCoeffCenterRight;
	double dZCoeffCenterRight;
	
	double dZerothLevelCenterOld;
	
	double dXCoeffCenterLeftOld;
	double dYCoeffCenterLeftOld;
	double dZCoeffCenterLeftOld;
	double dXCoeffCenterRightOld;
	double dYCoeffCenterRightOld;
	double dZCoeffCenterRightOld;
	
	double newWCenter;
	
	
	
	double dCosAzRearCenterLeft;
	double dCosElRearCenterLeft;
	double dSinAzRearCenterLeft;
	double dSinElRearCenterLeft;
	double dCosAzRearCenterRight;
	double dCosElRearCenterRight;
	double dSinAzRearCenterRight;
	double dSinElRearCenterRight;
	
	double dZerothLevelRearCenter;
	double dFirstLevelRearCenter;
	
	double dXCoeffRearCenterLeft;
	double dYCoeffRearCenterLeft;
	double dZCoeffRearCenterLeft;
	double dXCoeffRearCenterRight;
	double dYCoeffRearCenterRight;
	double dZCoeffRearCenterRight;
	
	double dZerothLevelRearCenterOld;
	
	double dXCoeffRearCenterLeftOld;
	double dYCoeffRearCenterLeftOld;
	double dZCoeffRearCenterLeftOld;
	double dXCoeffRearCenterRightOld;
	double dYCoeffRearCenterRightOld;
	double dZCoeffRearCenterRightOld;
	
	double newWRearCenter;
	
	double newW;
	double newX;
	double newY;
	double newZ;
	
	
	double currentFrontLeft;
	double currentFrontRight;
	double currentRearLeft;
	double currentRearRight;
	double currentCenter;
	double currentRearCenter;
	
	
	float fAngleFront;
	float fAngleRear;
	float fPatternFront;
	float fPatternRear;
	float fGainLeft;
	float fGainRight;
	float fGainCenter;
	float fGainLFE;
	float fGainSLeft;
	float fGainSRight;
	float fGainSCenter;
	
	float tempfAngleFront;
	float tempfAngleRear;
	float tempfPatternFront;
	float tempfPatternRear;
	float tempfGainLeft;
	float tempfGainRight;
	float tempfGainCenter;
	float tempfGainLFE;
	float tempfGainSLeft;
	float tempfGainSRight;
	float tempfGainSCenter;
} t_ambi2s_tilde;

void ambi2sProgram(t_ambi2s_tilde *x)
{
	x->dCosAzFrontLeft 	 = 1.0;
	x->dCosElFrontLeft 	 = 1.0;
	x->dSinAzFrontLeft		 = 0.0;
	x->dSinElFrontLeft 	 = 0.0;
	x->dCosAzFrontRight	 = 1.0;
	x->dCosElFrontRight	 = 1.0;
	x->dSinAzFrontRight	 = 0.0;
	x->dSinElFrontRight	 = 0.0;
	x->dZerothLevelFront	 = 1.0;
	x->dFirstLevelFront	 = 1.0;
	x->dXCoeffFrontLeft	 = 1.0;
	x->dYCoeffFrontLeft	 = 1.0;
	x->dZCoeffFrontLeft	 = 0.0;
	x->dXCoeffFrontRight	 = 1.0;
	x->dYCoeffFrontRight	 = 1.0;
	x->dZCoeffFrontRight	 = 0.0;
	x->dZerothLevelFrontOld = 1.0;
	x->dXCoeffFrontLeftOld  = 1.0;
	x->dYCoeffFrontLeftOld  = 1.0;
	x->dZCoeffFrontLeftOld  = 0.0;
	x->dXCoeffFrontRightOld = 1.0;
	x->dYCoeffFrontRightOld = 1.0;
	x->dZCoeffFrontRightOld = 0.0;
	
	x->dCosAzRearLeft 		= 1.0;
	x->dCosElRearLeft 		= 1.0;
	x->dSinAzRearLeft		= 0.0;
	x->dSinElRearLeft 		= 0.0;
	x->dCosAzRearRight		= 1.0;
	x->dCosElRearRight		= 1.0;
	x->dSinAzRearRight		= 0.0;
	x->dSinElRearRight		= 0.0;
	x->dZerothLevelRear	= 1.0;
	x->dFirstLevelRear		= 1.0;
	x->dXCoeffRearLeft		= 1.0;
	x->dYCoeffRearLeft		= 1.0;
	x->dZCoeffRearLeft		= 0.0;
	x->dXCoeffRearRight	= 1.0;
	x->dYCoeffRearRight	= 1.0;
	x->dZCoeffRearRight	= 0.0;
	x->dZerothLevelRearOld = 1.0;
	x->dXCoeffRearLeftOld	= 1.0;
	x->dYCoeffRearLeftOld	= 1.0;
	x->dZCoeffRearLeftOld	= 0.0;
	x->dXCoeffRearRightOld = 1.0;
	x->dYCoeffRearRightOld = 1.0;
	x->dZCoeffRearRightOld = 0.0;
	
	x->dCosAzCenterLeft 		= 1.0;
	x->dCosElCenterLeft 		= 1.0;
	x->dSinAzCenterLeft		= 0.0;
	x->dSinElCenterLeft 		= 0.0;
	x->dCosAzCenterRight		= 1.0;
	x->dCosElCenterRight		= 1.0;
	x->dSinAzCenterRight		= 0.0;
	x->dSinElCenterRight		= 0.0;
	x->dZerothLevelCenter		= 1.0;
	x->dFirstLevelCenter		= 1.0;
	x->dXCoeffCenterLeft		= 1.0;
	x->dYCoeffCenterLeft		= 1.0;
	x->dZCoeffCenterLeft		= 0.0;
	x->dXCoeffCenterRight		= 1.0;
	x->dYCoeffCenterRight		= 1.0;
	x->dZCoeffCenterRight		= 0.0;
	x->dZerothLevelCenterOld	= 1.0;
	x->dXCoeffCenterLeftOld	= 1.0;
	x->dYCoeffCenterLeftOld	= 1.0;
	x->dZCoeffCenterLeftOld	= 0.0;
	x->dXCoeffCenterRightOld	= 1.0;
	x->dYCoeffCenterRightOld	= 1.0;
	x->dZCoeffCenterRightOld	= 0.0;
	
	x->dCosAzRearCenterLeft 		= 1.0;
	x->dCosElRearCenterLeft 		= 1.0;
	x->dSinAzRearCenterLeft		= 0.0;
	x->dSinElRearCenterLeft 		= 0.0;
	x->dCosAzRearCenterRight		= 1.0;
	x->dCosElRearCenterRight		= 1.0;
	x->dSinAzRearCenterRight		= 0.0;
	x->dSinElRearCenterRight		= 0.0;
	x->dZerothLevelRearCenter		= 1.0;
	x->dFirstLevelRearCenter		= 1.0;
	x->dXCoeffRearCenterLeft		= 1.0;
	x->dYCoeffRearCenterLeft		= 1.0;
	x->dZCoeffRearCenterLeft		= 0.0;
	x->dXCoeffRearCenterRight		= 1.0;
	x->dYCoeffRearCenterRight		= 1.0;
	x->dZCoeffRearCenterRight		= 0.0;
	x->dZerothLevelRearCenterOld	= 1.0;
	x->dXCoeffRearCenterLeftOld	= 1.0;
	x->dYCoeffRearCenterLeftOld	= 1.0;
	x->dZCoeffRearCenterLeftOld	= 0.0;
	x->dXCoeffRearCenterRightOld	= 1.0;
	x->dYCoeffRearCenterRightOld	= 1.0;
	x->dZCoeffRearCenterRightOld	= 0.0;
	
	x->dAngleFront		= 0.333333;		// 60 degrees at front (dgm changed for ITU5.1)
	x->dAngleRear		= 0.611111;		// 110 degrees at back (dgm changed for ITU5.1)
	x->dPatternFront	= 0.5;	// Cardioid pattern
	x->dPatternRear	= 0.4;	// subcardioid pattern (dgm changed for ITU5.1)
	x->dGainLeft		= 0.8;	// 0 dB gain
	x->dGainRight		= 0.8;	// 0 dB gain
	x->dGainCenter		= 0.5;	// -9 dB gain (allowance has been made for gainLaw, if this method is changed, change this (dgm changed for ITU5.1)
	x->dGainLFE		= 0.0;	// off (dgm changed for ITU5.1)
	x->dGainSLeft		= 0.75;	// -1.5 dB gain (dgm changed for ITU5.1)
	x->dGainSRight		= 0.75;	// -1.5 dB gain (dgm changed for ITU5.1)
	x->dGainSCenter	= 0.0;	// off (dgm changed for ITU5.1)
	
	x->newWFront		= 0.0;
	x->newWRear		= 0.0;
	x->newWCenter		= 0.0;
	x->newWRearCenter	= 0.0;
	x->newX			= 0.0;
	x->newY			= 0.0;
	x->newZ			= 0.0;
	
	x->fAngleFront		= (float) 0.333333;		//dgm 60 degrees at front for ITU5.1
	x->fAngleRear		= (float) 0.611111;		//dgm 110 degrees at back for ITU5.1
	x->fPatternFront	= (float) 0.5;	// Cardioid pattern
	x->fPatternRear	= (float) 0.4;	// subcardioid pattern (dgm changed for ITU5.1)
	x->fGainLeft		= (float) 0.8;	// 0 dB gain
	x->fGainRight		= (float) 0.8;	// 0 dB gain
	x->fGainCenter		= (float) 0.5;	// -9 dB gain (allowance has been made for gainLaw, if this method is changed, change this (dgm changed for ITU5.1)
	x->fGainLFE		= (float) 0.0;	// off (dgm changed for ITU5.1)
	x->fGainSLeft		= (float) 0.75;	// -1.5 dB gain (dgm changed for ITU5.1)
	x->fGainSRight		= (float) 0.75;	// -1.5 dB gain (dgm changed for ITU5.1)
	x->fGainSCenter	= (float) 0.0;	// off (dgm changed for ITU5.1)
}

t_int *ambi2s_tilde_perform(t_int *w)
{
	t_ambi2s_tilde *x = (t_ambi2s_tilde *)(w[1]);
	t_sample  *in1 =    (t_sample *)(w[2]);
	t_sample  *in2 =    (t_sample *)(w[3]);
	t_sample  *in3 =    (t_sample *)(w[4]);
	t_sample  *in4 =    (t_sample *)(w[5]);
	t_sample  *out1 =    (t_sample *)(w[6]);
	t_sample  *out2 =    (t_sample *)(w[7]);
	t_sample  *out3 =    (t_sample *)(w[8]);
	t_sample  *out4 =    (t_sample *)(w[9]);
	t_sample  *out5 =    (t_sample *)(w[10]);
	t_sample  *out6 =    (t_sample *)(w[11]);
	t_sample  *out7 =    (t_sample *)(w[12]);
	int          n =           (int)(w[13]);
	
	x->dZerothLevelFront = 1 - x->dPatternFront;
	x->dFirstLevelFront =  0.707 * x->dPatternFront; // .707 compensates for -3dB level of W in B Format
	
	x->dCosAzFrontLeft = cos(kAzimuthFront + x->dAngleFront);
	x->dCosElFrontLeft	= cos(kElevation);
	x->dSinAzFrontLeft	= sin(kAzimuthFront + x->dAngleFront);
	x->dSinElFrontLeft	= sin(kElevation);
	
	x->dCosAzFrontRight = cos(kAzimuthFront - x->dAngleFront);
	x->dCosElFrontRight = cos(kElevation);
	x->dSinAzFrontRight = sin(kAzimuthFront - x->dAngleFront);
	x->dSinElFrontRight = sin(kElevation);
	
	x->dXCoeffFrontLeft = x->dCosAzFrontLeft * x->dCosElFrontLeft * x->dFirstLevelFront;
	x->dYCoeffFrontLeft = x->dSinAzFrontLeft * x->dCosElFrontLeft * x->dFirstLevelFront;
	x->dZCoeffFrontLeft = x->dSinElFrontLeft * x->dFirstLevelFront;
	
	x->dXCoeffFrontRight = x->dCosAzFrontRight * x->dCosElFrontRight * x->dFirstLevelFront;
	x->dYCoeffFrontRight = x->dSinAzFrontRight * x->dCosElFrontRight * x->dFirstLevelFront;
	x->dZCoeffFrontRight = x->dSinElFrontRight * x->dFirstLevelFront;
	
	
	// Calculate the coefficients for the Rear L/R pair
	
	x->dZerothLevelRear = 1 - x->dPatternRear;
	x->dFirstLevelRear =  0.707 * x->dPatternRear; // .707 compensates for -3dB level of W in B Format
	
	x->dCosAzRearLeft  = cos(kAzimuthRear - x->dAngleRear);
	x->dCosElRearLeft	= cos(kElevation);
	x->dSinAzRearLeft	= sin(kAzimuthRear - x->dAngleRear);
	x->dSinElRearLeft	= sin(kElevation);
	
	x->dCosAzRearRight = cos(kAzimuthRear + x->dAngleRear);
	x->dCosElRearRight = cos(kElevation);
	x->dSinAzRearRight = sin(kAzimuthRear + x->dAngleRear);
	x->dSinElRearRight = sin(kElevation);
	
	x->dXCoeffRearLeft = x->dCosAzRearLeft * x->dCosElRearLeft * x->dFirstLevelRear;
	x->dYCoeffRearLeft = x->dSinAzRearLeft * x->dCosElRearLeft * x->dFirstLevelRear;
	x->dZCoeffRearLeft = x->dSinElRearLeft * x->dFirstLevelRear;
	
	x->dXCoeffRearRight = x->dCosAzRearRight * x->dCosElRearRight * x->dFirstLevelRear;
	x->dYCoeffRearRight = x->dSinAzRearRight * x->dCosElRearRight * x->dFirstLevelRear;
	x->dZCoeffRearRight = x->dSinElRearRight * x->dFirstLevelRear;
	
	
	// Calculate the coefficients for the Front center speaker
	// The pattern for this speaker is the same as the Front L/R pair
	
	x->dZerothLevelCenter = 1 - x->dPatternFront;
	x->dFirstLevelCenter =  0.707 * x->dPatternFront; // .707 compensates for -3dB level of W in B Format
	
	x->dCosAzCenterLeft   = cos(kAzimuthCenter + kAngleCenter);
	x->dCosElCenterLeft	= cos(kElevation);
	x->dSinAzCenterLeft	= sin(kAzimuthCenter + kAngleCenter);
	x->dSinElCenterLeft	= sin(kElevation);
	
	x->dCosAzCenterRight = cos(kAzimuthCenter - kAngleCenter);
	x->dCosElCenterRight = cos(kElevation);
	x->dSinAzCenterRight = sin(kAzimuthCenter - kAngleCenter);
	x->dSinElCenterRight = sin(kElevation);
	
	x->dXCoeffCenterLeft = x->dCosAzCenterLeft * x->dCosElCenterLeft * x->dFirstLevelCenter;
	x->dYCoeffCenterLeft = x->dSinAzCenterLeft * x->dCosElCenterLeft * x->dFirstLevelCenter;
	x->dZCoeffCenterLeft = x->dSinElCenterLeft * x->dFirstLevelCenter;
	
	x->dXCoeffCenterRight = x->dCosAzCenterRight * x->dCosElCenterRight * x->dFirstLevelCenter;
	x->dYCoeffCenterRight = x->dSinAzCenterRight * x->dCosElCenterRight * x->dFirstLevelCenter;
	x->dZCoeffCenterRight = x->dSinElCenterRight * x->dFirstLevelCenter;
	
	
	// Calculate the coefficients for the Rear center speaker
	// The pattern for this speaker is the same as the Rear L/R pair
	
	x->dZerothLevelRearCenter = 1 - x->dPatternRear;
	x->dFirstLevelRearCenter =  0.707 * x->dPatternRear; // .707 compensates for -3dB level of W in B Format
	
	x->dCosAzRearCenterLeft   = cos(kAzimuthRearCenter + kAngleRearCenter);
	x->dCosElRearCenterLeft	= cos(kElevation);
	x->dSinAzRearCenterLeft	= sin(kAzimuthRearCenter + kAngleRearCenter);
	x->dSinElRearCenterLeft	= sin(kElevation);
	
	x->dCosAzRearCenterRight = cos(kAzimuthRearCenter - kAngleRearCenter);
	x->dCosElRearCenterRight = cos(kElevation);
	x->dSinAzRearCenterRight = sin(kAzimuthRearCenter - kAngleRearCenter);
	x->dSinElRearCenterRight = sin(kElevation);
	
	x->dXCoeffRearCenterLeft = x->dCosAzRearCenterLeft * x->dCosElRearCenterLeft * x->dFirstLevelRearCenter;
	x->dYCoeffRearCenterLeft = x->dSinAzRearCenterLeft * x->dCosElRearCenterLeft * x->dFirstLevelRearCenter;
	x->dZCoeffRearCenterLeft = x->dSinElRearCenterLeft * x->dFirstLevelRearCenter;
	
	x->dXCoeffRearCenterRight = x->dCosAzRearCenterRight * x->dCosElRearCenterRight * x->dFirstLevelRearCenter;
	x->dYCoeffRearCenterRight = x->dSinAzRearCenterRight * x->dCosElRearCenterRight * x->dFirstLevelRearCenter;
	x->dZCoeffRearCenterRight = x->dSinElRearCenterRight * x->dFirstLevelRearCenter;
	
	while (n--){
        // Add antidenormalisation constant to incoming speaker feeds. This
		// stops most CPU's going into high precision mode
		x->newW = kAntiDenorm + (double) (*in1++);
		x->newX = kAntiDenorm + (double) (*in2++);
		x->newY = kAntiDenorm + (double) (*in3++);
		x->newZ = kAntiDenorm + (double) (*in4++);
		
		x->newWFront = kAntiDenorm +  kAntiDenorm + x->dZerothLevelFrontOld * x->newW;
		x->newWRear = kAntiDenorm +  kAntiDenorm + x->dZerothLevelRearOld * x->newW;
		x->newWCenter = kAntiDenorm +  kAntiDenorm + x->dZerothLevelCenterOld * x->newW;
		x->newWRearCenter = kAntiDenorm +  kAntiDenorm + x->dZerothLevelRearCenterOld * x->newW;
		
		// Calculate the current sample for each virtual microphone
		x->currentFrontLeft = (double) (kAntiDenorm +  x->newWFront + (x->dXCoeffFrontLeftOld*x->newX) + (x->dYCoeffFrontLeftOld*x->newY) + (x->dZCoeffFrontLeftOld*x->newZ));
		x->currentFrontRight = (double) (kAntiDenorm +  x->newWFront + (x->dXCoeffFrontRightOld*x->newX) + (x->dYCoeffFrontRightOld*x->newY) + (x->dZCoeffFrontRightOld*x->newZ));
		x->currentCenter = (double) (kAntiDenorm +  x->newWCenter + (x->dXCoeffCenterLeftOld*x->newX) + (x->dYCoeffCenterLeftOld*x->newY) + (x->dZCoeffCenterLeftOld*x->newZ));
		x->currentRearLeft = (double) (kAntiDenorm +  x->newWRear + (x->dXCoeffRearLeftOld*x->newX) + (x->dYCoeffRearLeftOld*x->newY) + (x->dZCoeffRearLeftOld*x->newZ));
		x->currentRearRight = (double) (kAntiDenorm +  x->newWRear + (x->dXCoeffRearRightOld*x->newX) + (x->dYCoeffRearRightOld*x->newY) + (x->dZCoeffRearRightOld*x->newZ));
		x->currentRearCenter = (double) (kAntiDenorm +  x->newWRearCenter + (x->dXCoeffRearCenterLeftOld*x->newX) + (x->dYCoeffRearCenterLeftOld*x->newY) + (x->dZCoeffRearCenterLeftOld*x->newZ));
		
		// Adjust each channel using gain parameter and create
		// the LFE (sub) channel by summing the other 6 channels
        (*out1++) = (float) (x->dGainLeft * x->currentFrontLeft);			// accumulating
        (*out2++) = (float) (x->dGainRight * x->currentFrontRight);		// accumulating
		(*out3++) = (float) (x->dGainCenter * x->currentCenter);			// accumulating
		(*out4++) = (float) (x->dGainLFE * ((x->currentFrontLeft/6) + (x->currentFrontRight/6) + (x->currentCenter/6) + (x->currentRearLeft/6) + (x->currentRearRight/6) + (x->currentRearCenter/6)));	// accumulating
		(*out5++) = (float) (x->dGainSLeft * x->currentRearLeft);			// accumulating
		(*out6++) = (float) (x->dGainSRight * x->currentRearRight);		// accumulating
		(*out7++) = (float) (x->dGainSCenter * x->currentRearCenter);		// accumulating
		
		// Calculate smoothing coefficients from this sample and the last sample for the front L/R pair
		x->dZerothLevelFrontOld	= (x->dZerothLevelFrontOld * kAvCoef1) + (kAvCoef2 * x->dZerothLevelFront);
        x->dXCoeffFrontLeftOld		= (x->dXCoeffFrontLeftOld * kAvCoef1) + (kAvCoef2 * x->dXCoeffFrontLeft);
		x->dYCoeffFrontLeftOld		= (x->dYCoeffFrontLeftOld * kAvCoef1) + (kAvCoef2 * x->dYCoeffFrontLeft);
        x->dZCoeffFrontLeftOld		= (x->dZCoeffFrontLeftOld * kAvCoef1) + (kAvCoef2 * x->dZCoeffFrontLeft);
		x-> dXCoeffFrontRightOld	= (x->dXCoeffFrontRightOld * kAvCoef1) + (kAvCoef2 * x->dXCoeffFrontRight);
		x->dYCoeffFrontRightOld	= (x->dYCoeffFrontRightOld * kAvCoef1) + (kAvCoef2 * x->dYCoeffFrontRight);
		x->dZCoeffFrontRightOld	= (x->dZCoeffFrontRightOld * kAvCoef1) + (kAvCoef2 * x->dZCoeffFrontRight);
		
		// Calculate smoothing coefficients from this sample and the last sample for the rear L/R pair
		x->dZerothLevelRearOld	= (x->dZerothLevelRearOld * kAvCoef1) + (kAvCoef2 * x->dZerothLevelRear);
		x->dXCoeffRearLeftOld	= (x->dXCoeffRearLeftOld * kAvCoef1) + (kAvCoef2 * x->dXCoeffRearLeft);
		x->dYCoeffRearLeftOld	= (x->dYCoeffRearLeftOld * kAvCoef1) + (kAvCoef2 * x->dYCoeffRearLeft);
        x->dZCoeffRearLeftOld	= (x->dZCoeffRearLeftOld * kAvCoef1) + (kAvCoef2 * x->dZCoeffRearLeft);
        x->dXCoeffRearRightOld	= (x->dXCoeffRearRightOld * kAvCoef1) + (kAvCoef2 * x->dXCoeffRearRight);
		x->dYCoeffRearRightOld	= (x->dYCoeffRearRightOld * kAvCoef1) + (kAvCoef2 * x->dYCoeffRearRight);
        x->dZCoeffRearRightOld	= (x->dZCoeffRearRightOld * kAvCoef1) + (kAvCoef2 * x->dZCoeffRearRight);
		
		// Calculate smoothing coefficients from this sample and the last sample for the front center speaker
		x->dZerothLevelCenterOld	= (x->dZerothLevelCenterOld * kAvCoef1) + (kAvCoef2 * x->dZerothLevelCenter);
        x->dXCoeffCenterLeftOld	= (x->dXCoeffCenterLeftOld * kAvCoef1) + (kAvCoef2 * x->dXCoeffCenterLeft);
		x->dYCoeffCenterLeftOld	= (x->dYCoeffCenterLeftOld * kAvCoef1) + (kAvCoef2 * x->dYCoeffCenterLeft);
        x->dZCoeffCenterLeftOld	= (x->dZCoeffCenterLeftOld * kAvCoef1) + (kAvCoef2 * x->dZCoeffCenterLeft);
        x->dXCoeffCenterRightOld	= (x->dXCoeffCenterRightOld * kAvCoef1) + (kAvCoef2 * x->dXCoeffCenterRight);
		x->dYCoeffCenterRightOld	= (x->dYCoeffCenterRightOld * kAvCoef1) + (kAvCoef2 * x->dYCoeffCenterRight);
        x->dZCoeffCenterRightOld	= (x->dZCoeffCenterRightOld * kAvCoef1) + (kAvCoef2 * x->dZCoeffCenterRight);
		
		// Calculate smoothing coefficients from this sample and the last sample for the rear center speaker
		x->dZerothLevelRearCenterOld	= (x->dZerothLevelRearCenterOld * kAvCoef1) + (kAvCoef2 * x->dZerothLevelRearCenter);
        x->dXCoeffRearCenterLeftOld	= (x->dXCoeffRearCenterLeftOld * kAvCoef1) + (kAvCoef2 * x->dXCoeffRearCenterLeft);
		x->dYCoeffRearCenterLeftOld	= (x->dYCoeffRearCenterLeftOld * kAvCoef1) + (kAvCoef2 * x->dYCoeffRearCenterLeft);
        x->dZCoeffRearCenterLeftOld	= (x->dZCoeffRearCenterLeftOld * kAvCoef1) + (kAvCoef2 * x->dZCoeffRearCenterLeft);
        x->dXCoeffRearCenterRightOld	= (x->dXCoeffRearCenterRightOld * kAvCoef1) + (kAvCoef2 * x->dXCoeffRearCenterRight);
		x->dYCoeffRearCenterRightOld	= (x->dYCoeffRearCenterRightOld * kAvCoef1) + (kAvCoef2 * x->dYCoeffRearCenterRight);
        x->dZCoeffRearCenterRightOld	= (x->dZCoeffRearCenterRightOld * kAvCoef1) + (kAvCoef2 * x->dZCoeffRearCenterRight);
		
	}
	return (w+14);
	
}
void ambi2s_tilde_angleFront(t_ambi2s_tilde *x, t_floatarg angF )
{
	x->tempfAngleFront = angF;
	x->tempfAngleFront = (x->tempfAngleFront<0)?0.0:(x->tempfAngleFront>1)?1.0:x->tempfAngleFront;
	x->fAngleFront = x->tempfAngleFront;
	x->dAngleFront	= (double)(x->fAngleFront * kPI/2);
	if(x->verbose == 1)post("Changed Angle Front to %f\n",x->fAngleFront);
}
void ambi2s_tilde_angleRear(t_ambi2s_tilde *x, t_floatarg angR )
{
	x->tempfAngleRear = angR;
	x->tempfAngleRear = (x->tempfAngleRear<0)?0.0:(x->tempfAngleRear>1)?1.0:x->tempfAngleRear;
	x->fAngleRear = x->tempfAngleRear;
	x->dAngleRear = (double)(x->fAngleRear * kPI/2);
	if(x->verbose == 1)post("Changed Angle Rear to %f\n",x->fAngleRear);
}
void ambi2s_tilde_patternFront(t_ambi2s_tilde *x, t_floatarg patF )
{
	x->tempfPatternFront = patF;
	x->tempfPatternFront = (x->tempfPatternFront<0)?0.0:(x->tempfPatternFront>1)?1.0:x->tempfPatternFront;
	x->fPatternFront = x->tempfPatternFront;
	x->dPatternFront	= (double)x->fPatternFront;
	if(x->verbose == 1)post("Changed Pattern Front to %f\n",(float)x->dPatternFront);
}

void ambi2s_tilde_patternRear(t_ambi2s_tilde *x, t_floatarg patR )
{
	x->tempfPatternRear = patR;
	x->tempfPatternRear = (x->tempfPatternRear<0)?0.0:(x->tempfPatternRear>1)?1.0:x->tempfPatternRear;
	x->fPatternRear = x->tempfPatternRear;
	x->dPatternRear	= (double)x->fPatternRear;
	if(x->verbose == 1)post("Changed Pattern Rear to %f\n",(float)x->dPatternRear);
}

void ambi2s_tilde_gainLeft(t_ambi2s_tilde *x, t_floatarg gainL )
{
	x->tempfGainLeft = gainL;
	x->tempfGainLeft = (x->tempfGainLeft<0)?0.0:(x->tempfGainLeft>1)?1.0:x->tempfGainLeft;
	x->fGainLeft = x->tempfGainLeft;
	if(x->fGainLeft == 0.0){ x->dGainLeft = 0; } else { x->dGainLeft = (double)(1/(pow(10,(fabs(((30 * x->fGainLeft)-24)-6)/20)))); };
	if(x->verbose == 1)post("Changed Gain Left to %f\n",(float)x->dGainLeft);
}

void ambi2s_tilde_gainRight(t_ambi2s_tilde *x, t_floatarg gainR )
{
	x->tempfGainRight = gainR;
	x->tempfGainRight = (x->tempfGainRight<0)?0.0:(x->tempfGainRight>1)?1.0:x->tempfGainRight;
	x->fGainRight = x->tempfGainRight;
	
	if(x->fGainRight == 0.0){ x->dGainRight = 0; } else { x->dGainRight = (double) (1/(pow(10,(fabs(((30 * x->fGainRight)-24)-6)/20)))); };
	if(x->verbose == 1)post("Changed Gain Right to %f\n",(float)x->dGainRight);
}

void ambi2s_tilde_gainCenter(t_ambi2s_tilde *x, t_floatarg gainC )
{
	x->tempfGainCenter = gainC;
	x->tempfGainCenter = (x->tempfGainCenter<0)?0.0:(x->tempfGainCenter>1)?1.0:x->tempfGainCenter;
	x->fGainCenter = x->tempfGainCenter;
	if(x->fGainCenter == 0.0){ x->dGainCenter = 0; } else { x->dGainCenter = (double) (1/(pow(10,(fabs(((30 * x->fGainCenter)-24)-6)/20)))); };
	if(x->verbose == 1)post("Changed Gain Center to %f\n",(float)x->dGainCenter);
}

void ambi2s_tilde_gainLFE(t_ambi2s_tilde *x, t_floatarg gainLFE )
{
	x->tempfGainLFE = gainLFE;
	x->tempfGainLFE = (x->tempfGainLFE<0)?0.0:(x->tempfGainLFE>1)?1.0:x->tempfGainLFE;
	x->fGainLFE = x->tempfGainLFE;
	if(x->fGainLFE == 0.0){ x->dGainLFE = 0; } else { x->dGainLFE = (double) (1/(pow(10,(fabs(((30 * x->fGainLFE)-24)-6)/20)))); };
	if(x->verbose == 1)post("Changed Gain LFE to %f\n",(float)x->dGainLFE);
}

void ambi2s_tilde_gainSLeft(t_ambi2s_tilde *x, t_floatarg gainSL )
{
	x->tempfGainSLeft = gainSL;
	x->tempfGainSLeft = (x->tempfGainSLeft<0)?0.0:(x->tempfGainSLeft>1)?1.0:x->tempfGainSLeft;
	x->fGainSLeft = x->tempfGainSLeft;
	if(x->fGainSLeft == 0.0){ x->dGainSLeft = 0; } else { x->dGainSLeft = (double) (1/(pow(10,(fabs(((30 * x->fGainSLeft)-24)-6)/20)))); };
	if(x->verbose == 1)post("Changed Gain SLeft to %f\n",(float)x->dGainSLeft);
}

void ambi2s_tilde_gainSRight(t_ambi2s_tilde *x, t_floatarg gainSR )
{
	x->tempfGainSRight = gainSR;
	x->tempfGainSRight = (x->tempfGainSRight<0)?0.0:(x->tempfGainSRight>1)?1.0:x->tempfGainSRight;
	x->fGainSRight = x->tempfGainSRight;
	if(x->fGainSRight == 0.0){ x->dGainSRight = 0; } else { x->dGainSRight = (double) (1/(pow(10,(fabs(((30 * x->fGainSRight)-24)-6)/20)))); };
	if(x->verbose == 1)post("Changed Gain SRight to %f\n",(float)x->dGainSRight);
}

void ambi2s_tilde_gainSCenter(t_ambi2s_tilde *x, t_floatarg gainSC )
{
	x->tempfGainSCenter = gainSC;
	x->tempfGainSCenter = (x->tempfGainSCenter<0)?0.0:(x->tempfGainSCenter>1)?1.0:x->tempfGainSCenter;
	x->fGainSCenter = x->tempfGainSCenter;
	if(x->fGainSCenter == 0.0){ x->dGainSCenter = 0; } else { x->dGainSCenter = (double) (1/(pow(10,(fabs(((30 * x->fGainSCenter)-24)-6)/20)))); };
	if(x->verbose == 1)post("Changed Gain SCenter to %f\n",(float)x->dGainSCenter);
}

void ambi2s_tilde_verbose(t_ambi2s_tilde *x, t_floatarg ver )
{
	x->verbose = ver;
	x->verbose = (x->verbose<0)?0.0:(x->verbose>1)?1.0:x->verbose;
}

void ambi2s_tilde_dsp(t_ambi2s_tilde *x, t_signal **sp)
{
	dsp_add(ambi2s_tilde_perform, 13, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,sp[4]->s_vec,sp[5]->s_vec,sp[6]->s_vec,sp[7]->s_vec,sp[8]->s_vec,sp[9]->s_vec,sp[10]->s_vec, sp[0]->s_n);
}

void *ambi2s_tilde_new(void)
{
	t_ambi2s_tilde *x = (t_ambi2s_tilde *)pd_new(ambi2s_tilde_class);
	ambi2sProgram(x);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal,&s_signal);
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	outlet_new(&x->x_obj, gensym ("signal"));
	return (void *)x;
}

void ambi2s_tilde_setup(void) 
{
	ambi2s_tilde_class = class_new(gensym("ambi2s~"),(t_newmethod)ambi2s_tilde_new,0, sizeof(t_ambi2s_tilde),CLASS_DEFAULT,A_DEFFLOAT,0);
	class_addmethod(ambi2s_tilde_class,
					(t_method)ambi2s_tilde_dsp, gensym("dsp"), 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_angleFront,gensym("aFront"), A_DEFFLOAT, 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_angleRear,gensym("aRear"), A_DEFFLOAT, 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_patternFront,gensym("patFront"), A_DEFFLOAT, 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_patternRear,gensym("patRear"), A_DEFFLOAT, 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_gainLeft,gensym("gainL"), A_DEFFLOAT, 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_gainRight,gensym("gainR"), A_DEFFLOAT, 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_gainCenter,gensym("gainC"), A_DEFFLOAT, 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_gainLFE,gensym("gainLFE"), A_DEFFLOAT, 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_gainSLeft,gensym("gainSL"), A_DEFFLOAT, 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_gainSRight,gensym("gainSR"), A_DEFFLOAT, 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_gainSCenter,gensym("gainSC"), A_DEFFLOAT, 0);
	class_addmethod(ambi2s_tilde_class, (t_method)ambi2s_tilde_verbose,gensym("verbose"), A_DEFFLOAT, 0);
	CLASS_MAINSIGNALIN(ambi2s_tilde_class, t_ambi2s_tilde, f);
}
void ambilib_tilde_setup(void) 
{
	ambipan_tilde_setup();
	ambidec_tilde_setup();
	ambimic_tilde_setup();
	ambiproc_tilde_setup();
	ambizoom_tilde_setup();
	ambiplane_tilde_setup();
	ambi2s_tilde_setup();
	post("Loaded ambilib v1.015 Copyright 2002-09 Dave Malham");
	post("Pd port by Matthew Paradis");
	post("Provided by the Music Research Centre");
	post("University of York, UK");
	post("http://music.york.ac.uk/mrc\n");
	
}
