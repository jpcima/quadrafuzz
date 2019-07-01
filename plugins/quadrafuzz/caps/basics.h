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

#include "ladspa.h"

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

#define HARD_RT  LADSPA_PROPERTY_HARD_RT_CAPABLE

/* some LADSPA_DEFINES_THAT_COME_WITH_LOTS_OF_CHARACTERS */
#define INPUT   LADSPA_PORT_INPUT
#define OUTPUT  LADSPA_PORT_OUTPUT
#define CONTROL LADSPA_PORT_CONTROL
#define AUDIO   LADSPA_PORT_AUDIO

#define AUDIO_IN  AUDIO|INPUT
#define AUDIO_OUT AUDIO|OUTPUT
#define CTRL_IN  	CONTROL|INPUT
#define CTRL_OUT 	CONTROL|OUTPUT

/* extending LADSPA_PORT_* */
#define LADSPA_PORT_GROUP (AUDIO<<1) /* 16 */
#define GROUP    LADSPA_PORT_GROUP

/* more LADSPA_DEFINES_THAT_REALLY_COME_WITH_LOTS_OF_CHARACTERS */
#define BOUNDED  (LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE)
#define INTEGER  LADSPA_HINT_INTEGER
#define LOG      LADSPA_HINT_LOGARITHMIC
#define TOGGLE   LADSPA_HINT_TOGGLED

#define DEFAULT_0     LADSPA_HINT_DEFAULT_0
#define DEFAULT_1     LADSPA_HINT_DEFAULT_1
#define DEFAULT_100   LADSPA_HINT_DEFAULT_100
#define DEFAULT_440   LADSPA_HINT_DEFAULT_440
#define DEFAULT_MIN   LADSPA_HINT_DEFAULT_MINIMUM
#define DEFAULT_LOW   LADSPA_HINT_DEFAULT_LOW
#define DEFAULT_MID   LADSPA_HINT_DEFAULT_MIDDLE
#define DEFAULT_HIGH  LADSPA_HINT_DEFAULT_HIGH
#define DEFAULT_MAX   LADSPA_HINT_DEFAULT_MAXIMUM

/* //////////////////////////////////////////////////////////////////////// */

typedef struct {
	const char * name;
	LADSPA_PortDescriptor descriptor;
	LADSPA_PortRangeHint range;
	const char * meta;
} PortInfo;

typedef LADSPA_Data sample_t;
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

/* //////////////////////////////////////////////////////////////////////// */

#define CAPS "C* "

class Plugin 
{
	public:
		float fs, over_fs; /* sample rate and 1/fs */
		float adding_gain; /* for run_adding() */

		int first_run; /* 1st block after activate(), do no parameter smoothing */
		sample_t normal; /* renormal constant */

		sample_t ** ports;
		LADSPA_PortRangeHint * ranges; /* for getport() below */

	public:
		/* get port value, mapping inf or nan to 0 */
		inline sample_t getport_unclamped (int i)
			{
				sample_t v = *ports[i];
				return (isinf (v) || isnan(v)) ? 0 : v;
			}

		/* get port value and clamp to port range */
		inline sample_t getport (int i)
			{
				LADSPA_PortRangeHint & r = ranges[i];
				sample_t v = getport_unclamped (i);
				return clamp (v, r.LowerBound, r.UpperBound);
			}
};

#endif /* BASICS_H */
