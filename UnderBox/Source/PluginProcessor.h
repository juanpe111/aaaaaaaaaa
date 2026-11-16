#pragma once
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_dsp/juce_dsp.h>
#include <array>
#include <atomic>
#include "Params.h"
#include "Presets.h"

class UnderBoxProcessor : public juce::AudioProcessor
{
public:
    UnderBoxProcessor();
    ~UnderBoxProcessor() override = default;

    void prepareToPlay(double sampleRate, int blockSize) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& l) const override { return l.getMainOutputChannelSet() == juce::AudioChannelSet::stereo(); }
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "Under Box"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 4.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    // ---- compartido con la interfaz ----
    juce::AudioProcessorValueTreeState apvts;
    juce::MidiKeyboardState keyState;
    std::array<std::atomic<float>, 256> draw;        // onda dibujada
    std::array<std::atomic<float>, 2048> scope;      // osciloscopio
    std::atomic<int> scopePos { 0 };
    std::atomic<float> level { 0.f };                // volumen de salida (boca del personaje)
    std::atomic<float> vowelUI { 0.f };              // vocal actual 0..4 (A E I O U)
    std::atomic<uint64_t> held[2];                   // notas sonando (bitmask 128)
    std::atomic<int> melodySeed { 1 };
    std::atomic<int> skin { 0 };
    juce::String currentPresetName { "Init" };

    void applyPreset(const Preset& p);
    void saveUserPreset(const juce::String& name);
    bool loadUserPreset(const juce::File& f);
    static juce::File userDir();
    juce::String drawToString() const;
    void stringToDraw(const juce::String& s);

private:
    struct Voice
    {
        bool on = false, gate = false;
        int note = 60; float vel = 1.f; uint32_t age = 0;
        double f = 440.0, ft = 440.0;
        float pe = 0.f, ph1[7] = {}, ph2 = 0.f, phSub = 0.f, lfoPh = 0.f, sh = 0.f;
        juce::ADSR aenv, fenv;
        juce::dsp::StateVariableTPTFilter<float> flt;
    };

    float osc(int wave, float ph, float dt, float pw) const;
    void renderVoice(Voice& v, float* L, float* R, int n);
    void renderRange(juce::AudioBuffer<float>& b, int start, int num);
    void post(juce::AudioBuffer<float>& b);
    void handleMidi(const juce::MidiMessage& m);
    void noteOn(int note, float vel);
    void noteOff(int note);
    void allOff();
    void runMelody(juce::MidiBuffer& midi, int n);
    void genMelody();

    double sr = 44100.0;
    std::atomic<float>* raw[NumParams];
    std::array<float, NumParams> cache {};
    std::array<Voice, 8> voices;
    juce::Random rnd;
    float bend = 0.f;

    // vox
    juce::dsp::StateVariableTPTFilter<float> form[3];
    float vowelTarget = 0.f, curVowel = 0.f; int syl = 0;

    // fx
    juce::dsp::Chorus<float> chorus;
    juce::Reverb reverb;
    std::vector<float> dBuf[2];
    int dSize = 1, dPos = 0; float dLp[2] = { 0, 0 }, curDelTime = 0.3f;
    float heldL = 0, heldR = 0; int crushCnt = 0;

    // melodia
    int melNote[32], melLen[32], melVel[32];
    int melStep = 0, melOn = -1, melLeft = 0, melKey = -1;
    double melCount = 0.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnderBoxProcessor)
};
