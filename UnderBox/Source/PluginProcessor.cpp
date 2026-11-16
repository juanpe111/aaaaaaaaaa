#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

namespace
{
const float FORM[5][3] = { {800,1150,2900}, {400,1600,2700}, {350,1700,2700}, {450,800,2830}, {325,700,2530} }; // A E I O U
const float FQ[3] = { 6.f, 9.f, 12.f };
const float FG[3] = { 1.f, 0.55f, 0.3f };
const int SCALES[7][7] = { {0,3,5,7,10,12,15}, {0,2,3,5,7,8,10}, {0,1,3,5,7,8,10}, {0,2,3,5,7,8,11}, {0,2,4,5,7,9,11}, {0,2,3,5,7,9,10}, {0,3,5,6,7,10,12} };
const int SLEN[7] = { 5, 7, 7, 7, 7, 7, 6 };
const int SYL[8] = { 0, 2, 0, 3, 1, 0, 2, 4 };     // "ya-i-ca-o-e..." para cantar
constexpr float twoPi = juce::MathConstants<float>::twoPi;

inline float blep(float t, float dt)
{
    if (t < dt) { t /= dt; return t + t - t * t - 1.0f; }
    if (t > 1.0f - dt) { t = (t - 1.0f) / dt; return t * t + t + t + 1.0f; }
    return 0.0f;
}
inline double midiHz(int n) { return 440.0 * std::pow(2.0, (n - 69) / 12.0); }
inline int scaleNote(int deg, int sc, int root)
{
    const int len = SLEN[sc];
    const int oct = (int) std::floor((float) deg / (float) len);
    return 60 + root + SCALES[sc][deg - oct * len] + 12 * oct;
}
} // namespace

UnderBoxProcessor::UnderBoxProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "STATE", makeLayout())
{
    for (int i = 0; i < NumParams; ++i) raw[i] = apvts.getRawParameterValue(defs()[i].id);
    for (int i = 0; i < 256; ++i)
        draw[(size_t) i].store(0.7f * std::sin(twoPi * (float) i / 256.f) + 0.3f * std::sin(3.f * twoPi * (float) i / 256.f));
    for (auto& s : scope) s.store(0.f);
    held[0].store(0); held[1].store(0);
    for (int i = 0; i < NumParams; ++i) cache[(size_t) i] = defs()[i].def;
    genMelody();
}

void UnderBoxProcessor::prepareToPlay(double s, int bs)
{
    sr = s;
    juce::dsp::ProcessSpec spec { s, (juce::uint32) juce::jmax(bs, 2048), 2 };
    for (auto& v : voices)
    {
        v.aenv.setSampleRate(s); v.fenv.setSampleRate(s);
        v.flt.prepare(spec); v.flt.reset(); v.on = false;
    }
    for (auto& f : form) { f.prepare(spec); f.reset(); f.setType(juce::dsp::StateVariableTPTFilterType::bandpass); }
    chorus.prepare(spec); chorus.reset();
    chorus.setRate(0.8f); chorus.setDepth(0.4f); chorus.setCentreDelay(8.f); chorus.setFeedback(0.f);
    reverb.setSampleRate(s); reverb.reset();
    dSize = (int) (s * 1.0) + 8;
    for (auto& b : dBuf) b.assign((size_t) dSize, 0.f);
    dPos = 0; dLp[0] = dLp[1] = 0.f;
}

// ------------------------------------------------------------------ osciladores
float UnderBoxProcessor::osc(int wave, float ph, float dt, float pw) const
{
    switch (wave)
    {
        case 0: return std::sin(ph * twoPi);
        case 1: return 2.0f * ph - 1.0f - blep(ph, dt);
        case 2: { float s = ph < 0.5f ? 1.f : -1.f; float p2 = ph + 0.5f; if (p2 >= 1.f) p2 -= 1.f; return s + blep(ph, dt) - blep(p2, dt); }
        case 3: return 4.0f * std::abs(ph - 0.5f) - 1.0f;
        case 4: { float s = ph < pw ? 1.f : -1.f; float p2 = ph - pw; if (p2 < 0.f) p2 += 1.f; return s + blep(ph, dt) - blep(p2, dt); }
        default: {
            const float x = ph * 256.f; const int i = (int) x & 255; const float fr = x - std::floor(x);
            const float a = draw[(size_t) i].load(std::memory_order_relaxed), b = draw[(size_t) ((i + 1) & 255)].load(std::memory_order_relaxed);
            return a + (b - a) * fr;
        }
    }
}

// ------------------------------------------------------------------ voces
void UnderBoxProcessor::noteOn(int note, float vel)
{
    const bool mono = cache[Mono] > 0.5f;
    Voice* v = nullptr;
    if (mono) v = &voices[0];
    else
    {
        for (auto& x : voices) if (! x.on) { v = &x; break; }
        if (v == nullptr) { v = &voices[0]; for (auto& x : voices) if (x.age > v->age) v = &x; }
    }
    const bool legato = mono && v->on && v->gate;
    v->note = note; v->ft = midiHz(note); v->vel = 0.35f + 0.65f * vel; v->age = 0;
    if (! legato)
    {
        if (! (mono && v->on)) v->f = v->ft;
        v->on = true; v->gate = true; v->pe = 1.f;
        const int nu = juce::jlimit(1, 7, (int) std::lround(cache[Uni]));
        for (int u = 0; u < 7; ++u) v->ph1[u] = nu == 1 ? 0.f : rnd.nextFloat();
        v->ph2 = 0.f; v->phSub = 0.f; v->lfoPh = 0.f;
        v->aenv.noteOn(); v->fenv.noteOn();
    }
    held[note >> 6].fetch_or(1ull << (note & 63));
    if (cache[Cantar] > 0.5f) vowelTarget = (float) SYL[syl++ & 7];   // cada nota = una silaba
}

void UnderBoxProcessor::noteOff(int note)
{
    held[note >> 6].fetch_and(~(1ull << (note & 63)));
    for (auto& v : voices)
        if (v.on && v.gate && v.note == note) { v.gate = false; v.aenv.noteOff(); v.fenv.noteOff(); }
}

void UnderBoxProcessor::allOff()
{
    for (auto& v : voices) if (v.on) { v.gate = false; v.aenv.noteOff(); v.fenv.noteOff(); }
    held[0].store(0); held[1].store(0);
}

void UnderBoxProcessor::handleMidi(const juce::MidiMessage& m)
{
    if (m.isNoteOn()) noteOn(m.getNoteNumber(), m.getFloatVelocity());
    else if (m.isNoteOff()) noteOff(m.getNoteNumber());
    else if (m.isAllNotesOff() || m.isAllSoundOff()) allOff();
    else if (m.isPitchWheel()) bend = (float) (m.getPitchWheelValue() - 8192) / 8192.f * 2.f;
}

void UnderBoxProcessor::renderVoice(Voice& v, float* L, float* R, int n)
{
    const auto& c = cache;
    const int nu = juce::jlimit(1, 7, (int) std::lround(c[Uni]));
    const int w1 = (int) c[O1Wave], w2 = (int) c[O2Wave];
    const float o1mul = std::exp2(c[O1Oct] + c[O1Fine] / 1200.f);
    const float o2mul = std::exp2(c[O2Oct] + (c[O2Semi] + c[O2Fine] / 100.f) / 12.f);
    const float lvl1 = c[O1Lvl], lvl2 = c[O2Lvl], sub = c[Sub], noise = c[Noise], fm = c[FM], pw = c[Pw];
    const float uniNorm = 1.f / std::sqrt((float) nu);
    const bool mono = c[Mono] > 0.5f;
    const float gt = 0.002f + c[Glide] * c[Glide] * 0.5f;
    const float glideCoef = 1.f - std::exp(-1.f / (gt * (float) sr));
    const float peCoef = std::exp(-1.f / (c[PTime] * (float) sr));
    const float lfoInc = c[LRate] / (float) sr;
    const int lshape = (int) c[LShape], ftype = (int) c[FType];
    const float vibDepth = c[LPitch] * 5.f, ampDep = c[LAmp];
    const float q = 0.6f + c[Res] * c[Res] * 14.f;
    const float bpGain = 1.5f / q;
    const float nyq = 0.45f * (float) sr;
    float detu[7];
    for (int u = 0; u < nu; ++u)
    {
        const float off = nu > 1 ? ((float) u / (float) (nu - 1)) * 2.f - 1.f : 0.f;
        detu[u] = std::exp2(off * c[UniDet] * 0.6f / 12.f);
    }

    v.aenv.setParameters({ c[AAtk], c[ADec], c[ASus], c[ARel] });
    v.fenv.setParameters({ c[FAtk], c[FDec], c[FSus], c[FRel] });
    v.flt.setType(ftype == 0 ? juce::dsp::StateVariableTPTFilterType::lowpass
                : ftype == 1 ? juce::dsp::StateVariableTPTFilterType::highpass
                             : juce::dsp::StateVariableTPTFilterType::bandpass);
    v.flt.setResonance(q);

    for (int i = 0; i < n; ++i)
    {
        if (mono) v.f += (v.ft - v.f) * glideCoef; else v.f = v.ft;
        v.pe *= peCoef;
        v.lfoPh += lfoInc;
        if (v.lfoPh >= 1.f) { v.lfoPh -= 1.f; v.sh = rnd.nextFloat() * 2.f - 1.f; }
        float l;
        switch (lshape)
        {
            case 0: l = std::sin(v.lfoPh * twoPi); break;
            case 1: l = 4.f * std::abs(v.lfoPh - 0.5f) - 1.f; break;
            case 2: l = v.lfoPh < 0.5f ? 1.f : -1.f; break;
            default: l = v.sh; break;
        }
        const float semis = bend + c[PEnv] * v.pe + l * vibDepth;
        const double fr = v.f * std::exp2((double) semis / 12.0);
        const float dt1 = juce::jmin(0.45f, (float) (fr * o1mul / sr));
        const float dt2 = juce::jmin(0.45f, (float) (fr * o2mul / sr));

        float o2 = 0.f;
        if (lvl2 > 0.f || fm > 0.f) { o2 = osc(w2, v.ph2, dt2, pw); v.ph2 += dt2; if (v.ph2 >= 1.f) v.ph2 -= 1.f; }

        float sl = 0.f, srr = 0.f;
        for (int u = 0; u < nu; ++u)
        {
            const float off = nu > 1 ? ((float) u / (float) (nu - 1)) * 2.f - 1.f : 0.f;
            const float dtu = juce::jmin(0.45f, dt1 * detu[u]);
            float p = v.ph1[u] + fm * o2;
            p -= std::floor(p);
            const float s = osc(w1, p, dtu, pw);
            v.ph1[u] += dtu; if (v.ph1[u] >= 1.f) v.ph1[u] -= 1.f;
            const float pan = off * 0.7f;
            sl += s * juce::jmin(1.f, 1.f - pan);
            srr += s * juce::jmin(1.f, 1.f + pan);
        }
        sl *= lvl1 * uniNorm; srr *= lvl1 * uniNorm;
        const float oo = o2 * lvl2; sl += oo; srr += oo;
        if (sub > 0.f) { const float sb = std::sin(v.phSub * twoPi) * sub; v.phSub += dt1 * 0.5f; if (v.phSub >= 1.f) v.phSub -= 1.f; sl += sb; srr += sb; }
        if (noise > 0.f) { const float nz = (rnd.nextFloat() * 2.f - 1.f) * noise; sl += nz; srr += nz; }

        const float ae = v.aenv.getNextSample();
        const float fe = v.fenv.getNextSample();
        if ((i & 7) == 0)
        {
            const float cut = c[Cut] * std::exp2(c[FEnv] * fe * 5.f + l * c[LCut] * 3.f + c[FKey] * (float) (v.note - 60) / 12.f);
            v.flt.setCutoffFrequency(juce::jlimit(20.f, nyq, cut));
        }
        sl = v.flt.processSample(0, sl); srr = v.flt.processSample(1, srr);
        if (ftype == 2) { sl *= bpGain; srr *= bpGain; }

        const float amp = ae * v.vel * (1.f - ampDep * 0.5f * (1.f - l)) * 0.4f;
        L[i] += sl * amp; R[i] += srr * amp;
    }
    v.age += (uint32_t) n;
    if (! v.aenv.isActive()) v.on = false;
}

void UnderBoxProcessor::renderRange(juce::AudioBuffer<float>& b, int start, int num)
{
    if (num <= 0) return;
    float* L = b.getWritePointer(0) + start; float* R = b.getWritePointer(1) + start;
    for (auto& v : voices) if (v.on) renderVoice(v, L, R, num);
}

// ------------------------------------------------------------------ melodia (el personaje toca)
void UnderBoxProcessor::genMelody()
{
    const int sc = juce::jlimit(0, 6, (int) raw[Scale]->load()), root = (int) raw[Root]->load();
    const float dens = raw[Dens]->load();
    juce::Random r(melodySeed.load() * 7919 + sc * 131 + 17);
    int deg = 2;
    static const int stepsD[] = { -2, -1, -1, 0, 1, 1, 2, 3, -3, 0 };
    static const int lens[] = { 1, 1, 2, 2, 3, 4 };
    auto makeStep = [&](int s)
    {
        const float prob = (s % 4 == 0) ? dens + 0.3f : (s % 2 == 0 ? dens : dens * 0.55f);
        if (r.nextFloat() < prob)
        {
            deg = juce::jlimit(-3, 9, deg + stepsD[r.nextInt(10)]);
            melNote[s] = scaleNote(deg, sc, root); melLen[s] = lens[r.nextInt(6)]; melVel[s] = 85 + r.nextInt(40);
        }
        else melNote[s] = -1;
    };
    for (int s = 0; s < 16; ++s) makeStep(s);
    for (int s = 16; s < 32; ++s)
    {
        if (r.nextFloat() < (s >= 28 ? 0.3f : 0.85f)) { melNote[s] = melNote[s - 16]; melLen[s] = melLen[s - 16]; melVel[s] = melVel[s - 16]; }
        else makeStep(s);
    }
}

void UnderBoxProcessor::runMelody(juce::MidiBuffer& midi, int n)
{
    if (cache[Play] < 0.5f)
    {
        if (melOn >= 0) { midi.addEvent(juce::MidiMessage::noteOff(1, melOn), 0); melOn = -1; }
        melStep = 0; melCount = 0.0;
        return;
    }
    const int key = melodySeed.load() * 131 + (int) cache[Scale] * 17 + (int) cache[Root] * 5 + (int) (cache[Dens] * 100.f);
    if (key != melKey) { melKey = key; genMelody(); }

    double bpm = cache[Bpm];
    if (auto* ph = getPlayHead())
        if (auto pos = ph->getPosition())
            if (pos->getIsPlaying())
                if (auto b = pos->getBpm()) bpm = *b;
    const double sps = sr * 60.0 / juce::jmax(30.0, bpm) / 4.0;   // semicorchea

    double t = melCount;
    while (t < (double) n)
    {
        const int at = (int) t;
        if (melOn >= 0 && --melLeft <= 0) { midi.addEvent(juce::MidiMessage::noteOff(1, melOn), at); melOn = -1; }
        const int s = melStep & 31;
        if (melNote[s] >= 0)
        {
            if (melOn >= 0) midi.addEvent(juce::MidiMessage::noteOff(1, melOn), at);
            midi.addEvent(juce::MidiMessage::noteOn(1, melNote[s], (juce::uint8) melVel[s]), at);
            melOn = melNote[s]; melLeft = melLen[s];
        }
        ++melStep; t += sps;
    }
    melCount = t - (double) n;
}

// ------------------------------------------------------------------ audio
void UnderBoxProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals nd;
    const int n = buffer.getNumSamples();
    if (buffer.getNumChannels() < 2) return;
    for (int i = 0; i < NumParams; ++i) cache[(size_t) i] = raw[i]->load();
    buffer.clear();

    runMelody(midi, n);
    keyState.processNextMidiBuffer(midi, 0, n, true);
    if (cache[Cantar] < 0.5f) vowelTarget = cache[Vowel] * 4.f;

    int pos = 0;
    for (const auto meta : midi)
    {
        const int t = juce::jlimit(pos, n, meta.samplePosition);
        renderRange(buffer, pos, t - pos);
        handleMidi(meta.getMessage());
        pos = t;
    }
    renderRange(buffer, pos, n - pos);
    post(buffer);
}

void UnderBoxProcessor::post(juce::AudioBuffer<float>& buffer)
{
    const int n = buffer.getNumSamples();
    float* L = buffer.getWritePointer(0); float* R = buffer.getWritePointer(1);
    const auto& c = cache;

    // --- Vox: banco de formantes (A E I O U) -> el personaje "canta"
    curVowel += (vowelTarget - curVowel) * (1.f - std::exp(-(float) n / ((float) sr * 0.06f)));
    vowelUI.store(curVowel);
    const float vox = c[Vox];
    if (vox > 0.001f)
    {
        const int i0 = juce::jlimit(0, 3, (int) curVowel);
        const int i1 = juce::jmin(4, i0 + 1);
        const float fr = juce::jlimit(0.f, 1.f, curVowel - (float) i0);
        for (int k = 0; k < 3; ++k)
        {
            form[k].setCutoffFrequency(FORM[i0][k] + (FORM[i1][k] - FORM[i0][k]) * fr);
            form[k].setResonance(FQ[k]);
        }
        for (int ch = 0; ch < 2; ++ch)
        {
            float* d = ch ? R : L;
            for (int i = 0; i < n; ++i)
            {
                const float x = d[i]; float y = 0.f;
                for (int k = 0; k < 3; ++k) y += FG[k] * form[k].processSample(ch, x) / FQ[k];
                d[i] = x * (1.f - vox) + y * vox * 3.5f;
            }
        }
    }

    // --- drive + bitcrush
    const float drive = c[Drive], crush = c[Crush], rate = c[Rate];
    if (drive > 0.001f || crush > 0.001f || rate > 0.001f)
    {
        const float g = 1.f + drive * 24.f, dm = juce::jmin(1.f, drive * 3.f);
        const int hold = 1 + (int) (rate * rate * 40.f);
        const float levels = std::pow(2.f, 15.f - crush * 12.f);
        const bool crushOn = crush > 0.001f || rate > 0.001f;
        for (int i = 0; i < n; ++i)
        {
            float l = L[i], r = R[i];
            if (drive > 0.001f) { l = l * (1.f - dm) + std::tanh(l * g) * 0.45f * dm; r = r * (1.f - dm) + std::tanh(r * g) * 0.45f * dm; }
            if (crushOn)
            {
                if (++crushCnt >= hold) { crushCnt = 0; heldL = std::round(l * levels) / levels; heldR = std::round(r * levels) / levels; }
                l = heldL; r = heldR;
            }
            L[i] = l; R[i] = r;
        }
    }

    // --- chorus
    if (c[Chorus] > 0.005f)
    {
        chorus.setMix(c[Chorus]);
        juce::dsp::AudioBlock<float> blk(buffer);
        juce::dsp::ProcessContextReplacing<float> ctx(blk);
        chorus.process(ctx);
    }

    // --- delay ping-pong
    if (c[DelMix] > 0.005f)
    {
        const float mix = c[DelMix], fb = c[DelFb];
        for (int i = 0; i < n; ++i)
        {
            curDelTime += (c[DelTime] - curDelTime) * 0.0005f;
            float rp = (float) dPos - curDelTime * (float) sr;
            if (rp < 0.f) rp += (float) dSize;
            const int i0 = juce::jlimit(0, dSize - 1, (int) rp);
            const int i1 = i0 + 1 >= dSize ? 0 : i0 + 1;
            const float fr = rp - (float) i0;
            const float dl = dBuf[0][(size_t) i0] * (1.f - fr) + dBuf[0][(size_t) i1] * fr;
            const float dr = dBuf[1][(size_t) i0] * (1.f - fr) + dBuf[1][(size_t) i1] * fr;
            dLp[0] += 0.45f * (dl - dLp[0]); dLp[1] += 0.45f * (dr - dLp[1]);
            dBuf[0][(size_t) dPos] = L[i] + dLp[1] * fb;
            dBuf[1][(size_t) dPos] = R[i] + dLp[0] * fb;
            L[i] += dl * mix; R[i] += dr * mix;
            if (++dPos >= dSize) dPos = 0;
        }
    }

    // --- reverb
    if (c[RevMix] > 0.005f)
    {
        juce::Reverb::Parameters rp;
        rp.roomSize = 0.3f + c[RevSize] * 0.65f; rp.damping = 0.45f; rp.wetLevel = c[RevMix] * 0.35f;
        rp.dryLevel = 1.f; rp.width = 1.f; rp.freezeMode = 0.f;
        reverb.setParameters(rp);
        reverb.processStereo(L, R, n);
    }

    // --- master + limitador suave + scope
    const float g = c[Vol] * c[Vol] * 1.6f;
    float sum = 0.f; int sp = scopePos.load(std::memory_order_relaxed);
    for (int i = 0; i < n; ++i)
    {
        L[i] = std::tanh(L[i] * g); R[i] = std::tanh(R[i] * g);
        const float m = 0.5f * (L[i] + R[i]);
        sum += m * m;
        scope[(size_t) (sp & 2047)].store(m, std::memory_order_relaxed); ++sp;
    }
    scopePos.store(sp, std::memory_order_release);
    const float rms = std::sqrt(sum / (float) juce::jmax(1, n));
    level.store(level.load() * 0.6f + rms * 0.4f);
}

// ------------------------------------------------------------------ presets / estado
void UnderBoxProcessor::applyPreset(const Preset& p)
{
    for (int i = 0; i < NumPreset; ++i)
        if (auto* prm = apvts.getParameter(defs()[i].id))
            prm->setValueNotifyingHost(prm->convertTo0to1(p.v[(size_t) i]));
    currentPresetName = p.name;
}

juce::File UnderBoxProcessor::userDir()
{
    auto d = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("UnderBox");
    d.createDirectory();
    return d;
}

juce::String UnderBoxProcessor::drawToString() const
{
    juce::String s;
    for (int i = 0; i < 256; ++i) s << juce::String(draw[(size_t) i].load(), 3) << " ";
    return s;
}

void UnderBoxProcessor::stringToDraw(const juce::String& s)
{
    auto t = juce::StringArray::fromTokens(s, " ", "");
    if (t.size() < 256) return;
    for (int i = 0; i < 256; ++i) draw[(size_t) i].store(juce::jlimit(-1.f, 1.f, t[i].getFloatValue()));
}

void UnderBoxProcessor::saveUserPreset(const juce::String& name)
{
    auto st = apvts.copyState();
    st.setProperty("draw", drawToString(), nullptr);
    st.setProperty("pname", name, nullptr);
    if (auto xml = st.createXml())
        xml->writeTo(userDir().getChildFile(juce::File::createLegalFileName(name) + ".ubp"));
    currentPresetName = name;
}

bool UnderBoxProcessor::loadUserPreset(const juce::File& f)
{
    auto xml = juce::parseXML(f);
    if (xml == nullptr || ! xml->hasTagName(apvts.state.getType())) return false;
    auto st = juce::ValueTree::fromXml(*xml);
    stringToDraw(st.getProperty("draw").toString());
    apvts.replaceState(st);
    currentPresetName = f.getFileNameWithoutExtension();
    return true;
}

void UnderBoxProcessor::getStateInformation(juce::MemoryBlock& dest)
{
    auto st = apvts.copyState();
    st.setProperty("draw", drawToString(), nullptr);
    st.setProperty("skin", skin.load(), nullptr);
    st.setProperty("pname", currentPresetName, nullptr);
    if (auto xml = st.createXml()) copyXmlToBinary(*xml, dest);
}

void UnderBoxProcessor::setStateInformation(const void* data, int size)
{
    if (auto xml = getXmlFromBinary(data, size))
        if (xml->hasTagName(apvts.state.getType()))
        {
            auto st = juce::ValueTree::fromXml(*xml);
            stringToDraw(st.getProperty("draw").toString());
            skin = (int) st.getProperty("skin", 0);
            currentPresetName = st.getProperty("pname", "Init").toString();
            apvts.replaceState(st);
        }
}

juce::AudioProcessorEditor* UnderBoxProcessor::createEditor() { return new UnderBoxEditor(*this); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new UnderBoxProcessor(); }
