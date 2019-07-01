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

#include "QuadrafuzzPlugin.hpp"
#include <array>
#include <cmath>

QuadrafuzzPlugin::QuadrafuzzPlugin()
    : Plugin(Parameter_Count, DISTRHO_PLUGIN_NUM_PROGRAMS, State_Count)
{
    for (unsigned p = 0; p < Parameter_Count; ++p) {
        Parameter param;
        initParameter(p, param);
        setParameterValue(p, param.ranges.def);
    }
}

const char *QuadrafuzzPlugin::getLabel() const
{
    return DISTRHO_PLUGIN_LABEL;
}

const char *QuadrafuzzPlugin::getMaker() const
{
    return DISTRHO_PLUGIN_MAKER;
}

const char *QuadrafuzzPlugin::getLicense() const
{
    return DISTRHO_PLUGIN_LICENSE;
}

const char *QuadrafuzzPlugin::getDescription() const
{
    return DISTRHO_PLUGIN_DESCRIPTION;
}

const char *QuadrafuzzPlugin::getHomePage() const
{
    return DISTRHO_PLUGIN_HOMEPAGE;
}

uint32_t QuadrafuzzPlugin::getVersion() const
{
    return d_version(DISTRHO_PLUGIN_VERSION);
}

int64_t QuadrafuzzPlugin::getUniqueId() const
{
    return d_cconst(DISTRHO_PLUGIN_UNIQUE_ID);
}

void QuadrafuzzPlugin::initParameter(uint32_t index, Parameter &parameter)
{
    parameter.hints = kParameterIsAutomable;

    switch (index) {
    case pIdBypass:
        parameter.designation = kParameterDesignationBypass;
        break;
    case pIdInputGain:
        parameter.symbol = "InputGain";
        parameter.name = "Input Gain";
        parameter.unit = "dB";
        parameter.ranges = ParameterRanges(0, -40, +10);
        break;
    case pIdOutputGain:
        parameter.symbol = "OutputGain";
        parameter.name = "Output Gain";
        parameter.unit = "dB";
        parameter.ranges = ParameterRanges(0, -40, +10);
        break;
    case pIdDryGain:
        parameter.symbol = "DryGain";
        parameter.name = "Dry Gain";
        parameter.unit = "dB";
        parameter.ranges = ParameterRanges(-40, -40, +10);
        break;
    case pIdWetGain:
        parameter.symbol = "WetGain";
        parameter.name = "Wet Gain";
        parameter.unit = "dB";
        parameter.ranges = ParameterRanges(0, -40, +10);
        break;
    case pIdLowDrive:
        parameter.symbol = "LowDrive";
        parameter.name = "Low Drive";
        parameter.ranges = ParameterRanges(0.6, 0.0, 1.0);
        break;
    case pIdMidLowDrive:
        parameter.symbol = "MidLowDrive";
        parameter.name = "Mid-Low Drive";
        parameter.ranges = ParameterRanges(0.8, 0.0, 1.0);
        break;
    case pIdMidHighDrive:
        parameter.symbol = "MidHighDrive";
        parameter.name = "Mid-High Drive";
        parameter.ranges = ParameterRanges(0.5, 0.0, 1.0);
        break;
    case pIdHighDrive:
        parameter.symbol = "HighDrive";
        parameter.name = "High Drive";
        parameter.ranges = ParameterRanges(0.6, 0.0, 1.0);
        break;
    case pIdOversampling: {
        std::array<std::pair<int, const char *>, 4> ev =
            {{{1, "none"}, {2, "2x"}, {4, "4x"}, {8, "8x"}}};
        ParameterEnumerationValue *enumValues = new ParameterEnumerationValue[ev.size()];
        parameter.enumValues.values = enumValues;
        parameter.enumValues.count = ev.size();
        parameter.enumValues.restrictedMode = true;
        for (size_t i = 0; i < ev.size(); ++i) {
            enumValues[i].value = ev[i].first;
            enumValues[i].label = ev[i].second;
        }
        parameter.symbol = "Oversampling";
        parameter.name = "Oversampling";
        parameter.ranges = ParameterRanges(1, 1, ev.back().first);
        parameter.hints = kParameterIsInteger;
        break;
    }
    default:
        DISTRHO_SAFE_ASSERT(false);
    }
}

float QuadrafuzzPlugin::getParameterValue(uint32_t index) const
{
    switch (index) {
    case pIdBypass:
        return fBypass;
    case pIdInputGain:
        return fInputGain;
    case pIdOutputGain:
        return fOutputGain;
    case pIdDryGain:
        return fDryGain;
    case pIdWetGain:
        return fWetGain;
    case pIdLowDrive:
        return fLowDrive;
    case pIdMidLowDrive:
        return fMidLowDrive;
    case pIdMidHighDrive:
        return fMidHighDrive;
    case pIdHighDrive:
        return fHighDrive;
    case pIdOversampling:
        return fOversampling;
    default:
        DISTRHO_SAFE_ASSERT_RETURN(false, 0);
    }
}

void QuadrafuzzPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index) {
    case pIdBypass:
        fBypass = value > 0.5f;
        break;
    case pIdInputGain:
        fInputGain = value;
        fInputGainLin = std::pow(10.0f, 0.05f * value);
        break;
    case pIdOutputGain:
        fOutputGain = value;
        fOutputGainLin = std::pow(10.0f, 0.05f * value);
        break;
    case pIdDryGain:
        fDryGain = value;
        fDryGainLin = std::pow(10.0f, 0.05f * value);
        break;
    case pIdWetGain:
        fWetGain = value;
        fWetGainLin = std::pow(10.0f, 0.05f * value);
        break;
    case pIdLowDrive:
        fLowDrive = value;
        break;
    case pIdMidLowDrive:
        fMidLowDrive = value;
        break;
    case pIdMidHighDrive:
        fMidHighDrive = value;
        break;
    case pIdHighDrive:
        fHighDrive = value;
        break;
    case pIdOversampling:
        fOversampling = (unsigned)value;
        break;
    default:
        DISTRHO_SAFE_ASSERT(false);
    }
}

void QuadrafuzzPlugin::run(const float *inputs[], float *outputs[], uint32_t frames)
{
    switch (fOversampling) {
    default:
        DISTRHO_SAFE_ASSERT(false);
        /* fall through */
    case 1:
        runWithoutOversampler(inputs, outputs, frames);
        break;
    case 2:
        runWithOversampler(fOver2x, inputs, outputs, frames);
        break;
    case 4:
        runWithOversampler(fOver4x, inputs, outputs, frames);
        break;
    case 8:
        runWithOversampler(fOver8x, inputs, outputs, frames);
        break;
    }
}

template <class Oversampler> void QuadrafuzzPlugin::runWithOversampler(Oversampler &os, const float *inputs[], float *outputs[], uint32_t frames)
{
    const float *input = inputs[0];
    float *output = outputs[0];

    if (fBypass) {
        memcpy(output, input, frames * sizeof(float));
        return;
    }

    constexpr uint32_t over = Oversampler::Ratio;
    if (fActiveOversampling != over) {
        setupFilters(over);
        fActiveOversampling = over;
    }

    float inputGain = fInputGainLin;
    float outputGain = fOutputGainLin;
    float dryGain = fDryGainLin;
    float wetGain = fWetGainLin;
    float drive[Bands] = { fLowDrive, fMidLowDrive, fMidHighDrive, fHighDrive };

    constexpr uint32_t maxFrames = 64;

    while (frames > 0) {
        uint32_t framesCurrent = (frames < maxFrames) ? frames : maxFrames;

        // add dry signal
        for (uint32_t i = 0; i < framesCurrent; ++i) {
            float in = inputGain * input[i];
            output[i] = dryGain * in;
        }

        // compute oversampled input
        float bandIn[maxFrames * over];
        for (uint32_t i = 0; i < framesCurrent; ++i) {
            bandIn[over * i] = os.upsample(wetGain * inputGain * input[i]);
            for (uint32_t o = 1; o < over; ++o)
                bandIn[over * i + o] = os.uppad(o);
        }

        // compute oversampled output
        float bandOut[Bands][maxFrames * over];
        for (unsigned b = 0; b < Bands; ++b) {
            fBiquad[b].process(bandIn, bandOut[b], over * framesCurrent);
            distort(bandOut[b], drive[b], over * framesCurrent);
        }
        for (uint32_t i = 0; i < framesCurrent; ++i) {
            float sumBands = 0;
            for (unsigned b = 0; b < Bands; ++b)
                sumBands += bandOut[b][over * i];
            output[i] += outputGain * os.downsample(sumBands);
            for (uint32_t o = 1; o < over; ++o) {
                sumBands = 0;
                for (unsigned b = 0; b < Bands; ++b)
                    sumBands += bandOut[b][over * i + o];
                os.downstore(sumBands);
            }
        }

        input += framesCurrent;
        output += framesCurrent;
        frames -= framesCurrent;
    }
}

void QuadrafuzzPlugin::runWithoutOversampler(const float *inputs[], float *outputs[], uint32_t frames)
{
    DSP::NoOversampler os;
    runWithOversampler(os, inputs, outputs, frames);
}

void QuadrafuzzPlugin::setupFilters(unsigned over)
{
    double fs = getSampleRate() * over;
    double fnorm = 1.0 / (0.5 * fs);

    fBiquad[0].setLowpassParams(147.0 * fnorm, M_SQRT1_2);
    fBiquad[1].setBandpassParams(587.0 * fnorm, M_SQRT1_2);
    fBiquad[2].setBandpassParams(2490.0 * fnorm, M_SQRT1_2);
    fBiquad[3].setHighpassParams(4980.0 * fnorm, M_SQRT1_2);

    for (unsigned nf = 0; nf < 4; ++nf) {
        WebCore::Biquad &filter = fBiquad[nf];
        filter.reset();
    }

    switch (over) {
    default:
        DISTRHO_SAFE_ASSERT(false);
        /* fall through */
    case 1:
        break;
    case 2:
        fOver2x.reset();
        break;
    case 4:
        fOver4x.reset();
        break;
    case 8:
        fOver8x.reset();
        break;
    }
}

void QuadrafuzzPlugin::distort(float *inout, float gain, uint32_t frames)
{
    float pi = M_PI;
    gain *= 150;

    for (uint32_t i = 0; i < frames; ++i) {
        float x = inout[i];
        x = (3 + gain) * (20 * pi / 180.0) * x / (pi + gain * std::fabs(x));
        inout[i] = x;
    }
}

///
namespace DISTRHO {

Plugin *createPlugin()
{
    return new QuadrafuzzPlugin;
}

} // namespace DISTRHO
