#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

// Todos los parametros del sinte, en orden. Los presets guardan [0, NumPreset).
enum P {
    O1Wave, O1Oct, O1Fine, O1Lvl, Uni, UniDet, Pw,
    O2Wave, O2Oct, O2Semi, O2Fine, O2Lvl, Sub, Noise, FM,
    FType, Cut, Res, FEnv, FKey,
    AAtk, ADec, ASus, ARel, FAtk, FDec, FSus, FRel,
    PEnv, PTime, LRate, LShape, LPitch, LCut, LAmp,
    Mono, Glide, Vox, Vowel, Cantar,
    Drive, Crush, Rate, Chorus, DelMix, DelTime, DelFb, RevMix, RevSize, Vol,
    Scale, Bpm, Dens, Root, Play,
    NumParams,
    NumPreset = Root
};

// type: 0 = float, 1 = choice (opciones separadas por |), 2 = on/off
struct PDef { const char* id; const char* name; float mn, mx, def, step, skew; int type; const char* choices; };

inline const PDef* defs()
{
    static const PDef d[] = {
        {"o1wave","Onda 1",0,5,1,1,1,1,"Seno|Sierra|Cuadrada|Triangulo|Pulso|Dibujo"},
        {"o1oct","Octava 1",-2,2,0,1,1,0,nullptr},
        {"o1fine","Fino 1",-100,100,0,0,1,0,nullptr},
        {"o1lvl","Nivel 1",0,1,0.8f,0,1,0,nullptr},
        {"uni","Unison",1,7,1,1,1,0,nullptr},
        {"unidet","Desafine",0,1,0.25f,0,1,0,nullptr},
        {"pw","Ancho",0.05f,0.95f,0.5f,0,1,0,nullptr},
        {"o2wave","Onda 2",0,5,1,1,1,1,"Seno|Sierra|Cuadrada|Triangulo|Pulso|Dibujo"},
        {"o2oct","Octava 2",-2,2,0,1,1,0,nullptr},
        {"o2semi","Semitono",-12,12,0,1,1,0,nullptr},
        {"o2fine","Fino 2",-100,100,0,0,1,0,nullptr},
        {"o2lvl","Nivel 2",0,1,0,0,1,0,nullptr},
        {"sub","Sub",0,1,0,0,1,0,nullptr},
        {"noise","Ruido",0,1,0,0,1,0,nullptr},
        {"fm","FM",0,1,0,0,1,0,nullptr},
        {"ftype","Filtro",0,2,0,1,1,1,"LP|HP|BP"},
        {"cut","Corte",20,20000,8000,0,0.3f,0,nullptr},
        {"res","Reso",0,0.95f,0.15f,0,1,0,nullptr},
        {"fenv","Env Filtro",-1,1,0,0,1,0,nullptr},
        {"fkey","Key Track",0,1,0,0,1,0,nullptr},
        {"aatk","Ataque",0.001f,5,0.005f,0,0.3f,0,nullptr},
        {"adec","Decay",0.005f,5,0.4f,0,0.3f,0,nullptr},
        {"asus","Sustain",0,1,0.8f,0,1,0,nullptr},
        {"arel","Release",0.005f,8,0.25f,0,0.3f,0,nullptr},
        {"fatk","F Ataque",0.001f,5,0.005f,0,0.3f,0,nullptr},
        {"fdec","F Decay",0.005f,5,0.4f,0,0.3f,0,nullptr},
        {"fsus","F Sustain",0,1,0,0,1,0,nullptr},
        {"frel","F Release",0.005f,8,0.3f,0,0.3f,0,nullptr},
        {"penv","Pitch Env",-24,24,0,0,1,0,nullptr},
        {"ptime","Pitch Time",0.005f,1,0.08f,0,0.4f,0,nullptr},
        {"lrate","LFO Rate",0.1f,20,5,0,0.4f,0,nullptr},
        {"lshape","LFO Onda",0,3,0,1,1,1,"Seno|Tri|Cuad|Azar"},
        {"lpitch","LFO Pitch",0,1,0,0,1,0,nullptr},
        {"lcut","LFO Corte",0,1,0,0,1,0,nullptr},
        {"lamp","LFO Vol",0,1,0,0,1,0,nullptr},
        {"mono","MONO",0,1,0,1,1,2,nullptr},
        {"glide","Glide",0,1,0,0,1,0,nullptr},
        {"vox","Vox",0,1,0,0,1,0,nullptr},
        {"vowel","Vocal",0,1,0,0,1,0,nullptr},
        {"cantar","CANTA",0,1,0,1,1,2,nullptr},
        {"drive","Drive",0,1,0,0,1,0,nullptr},
        {"crush","Bits",0,1,0,0,1,0,nullptr},
        {"rate","Rate",0,1,0,0,1,0,nullptr},
        {"chorus","Chorus",0,1,0,0,1,0,nullptr},
        {"delmix","Delay",0,1,0,0,1,0,nullptr},
        {"deltime","D Tiempo",0.05f,0.8f,0.3f,0,1,0,nullptr},
        {"delfb","D Feedback",0,0.9f,0.35f,0,1,0,nullptr},
        {"revmix","Reverb",0,1,0.1f,0,1,0,nullptr},
        {"revsize","R Tamano",0,1,0.5f,0,1,0,nullptr},
        {"vol","Volumen",0,1,0.7f,0,1,0,nullptr},
        {"scale","Escala",0,6,0,1,1,1,"Menor Pent|Menor|Frigia|Menor Arm|Mayor|Dorica|Blues"},
        {"bpm","BPM",60,200,140,1,1,0,nullptr},
        {"dens","Densidad",0,1,0.6f,0,1,0,nullptr},
        {"root","Nota Base",0,11,0,1,1,1,"Do|Do#|Re|Re#|Mi|Fa|Fa#|Sol|Sol#|La|La#|Si"},
        {"play","TOCA",0,1,0,1,1,2,nullptr},
    };
    static_assert(sizeof(d) / sizeof(d[0]) == NumParams, "tabla de parametros desalineada con el enum");
    return d;
}

inline juce::AudioProcessorValueTreeState::ParameterLayout makeLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout l;
    for (int i = 0; i < NumParams; ++i)
    {
        const auto& d = defs()[i];
        juce::ParameterID id { d.id, 1 };
        if (d.type == 1)
            l.add(std::make_unique<juce::AudioParameterChoice>(id, d.name, juce::StringArray::fromTokens(d.choices, "|", ""), (int) d.def));
        else if (d.type == 2)
            l.add(std::make_unique<juce::AudioParameterBool>(id, d.name, d.def > 0.5f));
        else
            l.add(std::make_unique<juce::AudioParameterFloat>(id, d.name, juce::NormalisableRange<float>(d.mn, d.mx, d.step, d.skew), d.def));
    }
    return l;
}
