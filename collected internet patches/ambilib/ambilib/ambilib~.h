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

#include <string.h>
#include "math.h"
#include "m_pd.h"
#include <stdio.h>

#ifndef kPI
#define kPI    3.14159265358979323846
#endif

const double kAntiDenorm = 1e-20;
const double kDtoR = kPI/180;
const double kRtoD = 180/kPI;
const double kLog2 = 0.6;
const double kdBFactor = (-6.0/20.0);
const float  k6dBPoint =  1 - (0.5/(20/6));										
const double kAvCoef1 = 0.999;			
const double kAvCoef2 = 0.001;	

const double kAzimuthFront = (0.0 * (kPI * 2));				// Front pair direction in radians
const double kAzimuthCenter = (0.0 * (kPI * 2));				// Center speaker direction in radians
const double kAzimuthRear = (0.5 * (kPI * 2));				// Rear pair direction in radians
const double kAzimuthRearCenter = (0.5 * (kPI * 2));			// Rear Center speaker direction in radians
const double kElevation = ((0.25) - 0.25) * ((kPI * 2));		// 0 degrees of elevations

const double kAngleCenter = (0.0 * (kPI/2));			// 0 degrees separation for front center pair
const double kAngleRearCenter = (0.0 * (kPI/2));		// 0 degrees separation for rear center pair


/*
void rotateField (double APRangle);
void tiltField (double APRangle);
void tumbleField (double APRangle);
void ambiproc_Program(void);
*/