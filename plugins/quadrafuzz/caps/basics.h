/*
	basics.h
	
	Copyright 2004-12 Tim Goetze <tim@quitte.de>
	
	http://quitte.de/dsp/

	Common constants, typedefs, utility functions 
	and simplified LADSPA #defines.

*/
/*
	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
	02111-1307, USA or point your web browser to http://www.gnu.org.
*/

#ifndef BASICS_H
#define BASICS_H

#define _GNU_SOURCE 1
#define _USE_GNU 1

/* unlocking some standard math calls. */
#define __USE_ISOC99 1
#define __USE_ISOC9X 1
#define _ISOC99_SOURCE 1
#define _ISOC9X_SOURCE 1

#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <float.h>

#include <assert.h>
#include <stdio.h>

typedef __int8_t			int8;
typedef __uint8_t			uint8;
typedef __int16_t			int16;
typedef __uint16_t		uint16;
typedef __int32_t			int32;
typedef __uint32_t		uint32;
typedef __int64_t			int64;
typedef __uint64_t		uint64;

#define MIN_GAIN 1e-6 /* -120 dB */
/* smallest non-denormal 32 bit IEEE float is 1.18e-38 */
#define NOISE_FLOOR 1e-20 /* -400 dB */

/* //////////////////////////////////////////////////////////////////////// */

typedef float sample_t;
typedef unsigned int uint;
typedef unsigned long ulong;

/* prototype that takes a sample and yields a sample */
typedef sample_t (*clip_func_t) (sample_t);

#ifndef max
template <class X, class Y> X min (X x, Y y) { return x < (X)y ? x : (X)y; }
template <class X, class Y> X max (X x, Y y) { return x > (X)y ? x : (X)y; }
#endif /* ! max */

template <class T>
T clamp (T value, T lower, T upper)
{
	if (value < lower) return lower;
	if (value > upper) return upper;
	return value;
}

static inline float frandom() { return (float) random() / (float) RAND_MAX; }

/* NB: also true if 0  */
inline bool 
is_denormal (float & f)
{
	int32 i = *((int32 *) &f);
	return ((i & 0x7f800000) == 0);
}

/* not used, check validity before using */
inline bool 
is_denormal (double & f)
{
	int64 i = *((int64 *) &f);
	return ((i & 0x7fe0000000000000ll) == 0);
}

/* lovely algorithm from 
  http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2Float
*/
inline uint 
next_power_of_2 (uint n)
{
	assert (n <= 0x40000000);

	--n;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;

	return ++n;
}

inline double db2lin (double db) { return pow(10, .05*db); }
inline double lin2db (double lin) { return 20*log10(lin); }

#if defined(__i386__) || defined(__amd64__)
	#define TRAP asm ("int $3;")
#else
	#define TRAP
#endif

#endif /* BASICS_H */
