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
#include "DistrhoPlugin.hpp"
#include "blink/Biquad.h"
#include "caps/basics.h"
#include "caps/dsp/Oversampler.h"

class QuadrafuzzPlugin : public DISTRHO::Plugin
{
public:
    QuadrafuzzPlugin();

    const char *getLabel() const override;
    const char *getMaker() const override;
    const char *getLicense() const override;
    const char *getDescription() const override;
    const char *getHomePage() const override;
    uint32_t getVersion() const override;
    int64_t getUniqueId() const override;

    void initParameter(uint32_t index, Parameter &parameter) override;
    float getParameterValue(uint32_t index) const override;
    void setParameterValue(uint32_t index, float value) override;

    void run(const float *inputs[], float *outputs[], uint32_t frames) override;

private:
    template <class Oversampler> void runWithOversampler(Oversampler &os, const float *inputs[], float *outputs[], uint32_t frames);
    void runWithoutOversampler(const float *inputs[], float *outputs[], uint32_t frames);
    void setupFilters(unsigned over);
    void distort(float *inout, float gain, uint32_t frames);

private:
    bool fBypass = false;
    unsigned fOversampling = 0;
    float fInputGain = 0;
    float fInputGainLin = 0;
    float fOutputGain = 0;
    float fOutputGainLin = 0;
    float fDryGain = 0;
    float fDryGainLin = 0;
    float fWetGain = 0;
    float fWetGainLin = 0;
    float fLowDrive = 0;
    float fMidLowDrive = 0;
    float fMidHighDrive = 0;
    float fHighDrive = 0;

    unsigned fActiveOversampling = 0;

    enum { Bands = 4 };

    WebCore::Biquad fBiquad[Bands];
    DSP::Oversampler<2, 64> fOver2x;
    DSP::Oversampler<4, 64> fOver4x;
    DSP::Oversampler<8, 64> fOver8x;
};
