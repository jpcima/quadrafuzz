/*
Copyright (c) 2019 Jean Pierre Cimalando

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice (including the next
paragraph) shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#define DISTRHO_PLUGIN_BRAND           "Jean Pierre Cimalando"
#define DISTRHO_PLUGIN_NAME            "Quadrafuzz"
#define DISTRHO_PLUGIN_URI             "http://jpcima.sdf1.org/lv2/quadrafuzz"
#define DISTRHO_PLUGIN_HOMEPAGE        "https://github.com/jpcima/quadrafuzz"
#define DISTRHO_PLUGIN_UNIQUE_ID       'q','d','f','z'
#define DISTRHO_PLUGIN_VERSION         0,0,0
#define DISTRHO_PLUGIN_LABEL           "Quadrafuzz"
#define DISTRHO_PLUGIN_LICENSE         "http://spdx.org/licenses/GPL-3.0-or-later"
#define DISTRHO_PLUGIN_MAKER           "Jean Pierre Cimalando"
#define DISTRHO_PLUGIN_DESCRIPTION     "Multi-band fuzz distortion"
#define DISTRHO_PLUGIN_NUM_INPUTS      1
#define DISTRHO_PLUGIN_NUM_OUTPUTS     1
#define DISTRHO_PLUGIN_IS_SYNTH        0
#define DISTRHO_PLUGIN_HAS_UI          0
#define DISTRHO_PLUGIN_HAS_EMBED_UI    0
#define DISTRHO_PLUGIN_HAS_EXTERNAL_UI 0
#define DISTRHO_PLUGIN_IS_RT_SAFE      1
#define DISTRHO_PLUGIN_WANT_PROGRAMS   0
#define DISTRHO_PLUGIN_WANT_STATE      0
#define DISTRHO_PLUGIN_WANT_FULL_STATE 0
#define DISTRHO_PLUGIN_NUM_PROGRAMS    0

enum {
    /* parameter IDs */
    pIdBypass,
    pIdInputGain,
    pIdOutputGain,
    pIdDryGain,
    pIdWetGain,
    pIdLowDrive,
    pIdMidLowDrive,
    pIdMidHighDrive,
    pIdHighDrive,
    pIdOversampling,

    Parameter_Count
};

enum {
    /* state IDs */
    State_Count
};
