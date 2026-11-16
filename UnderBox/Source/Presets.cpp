#include "Presets.h"
#include <cmath>

namespace
{
using Pr = std::pair<int, float>;
using Rc = std::vector<Pr>;
struct CatDef { const char* name; std::vector<Rc> rc; std::vector<const char*> names; };

std::vector<CatDef> makeCats()
{
    std::vector<CatDef> c;

    // ---------------- UNDER ----------------
    c.push_back({ "Rage", {
        { {O1Wave,1},{Uni,7},{UniDet,.45f},{O2Wave,2},{O2Lvl,.4f},{O2Fine,7},{Cut,6500},{Res,.2f},{FEnv,.35f},{FDec,.25f},{FSus,.3f},{ADec,.35f},{ASus,.6f},{ARel,.12f},{Drive,.55f},{Chorus,.2f},{RevMix,.15f},{DelMix,.1f} },
        { {O1Wave,0},{O2Wave,0},{O2Oct,2},{O2Lvl,.5f},{FM,.55f},{Uni,3},{UniDet,.1f},{Cut,9000},{ADec,.5f},{ASus,.1f},{ARel,.5f},{Drive,.3f},{RevMix,.3f},{DelMix,.25f} },
        { {O1Wave,1},{Uni,5},{UniDet,.3f},{Mono,1},{Glide,.35f},{Cut,5500},{Res,.3f},{ASus,1},{AAtk,.01f},{ARel,.2f},{LRate,6},{LPitch,.1f},{Drive,.6f},{DelMix,.2f},{RevMix,.2f} },
        { {O1Wave,2},{Uni,5},{UniDet,.35f},{Cut,2400},{FEnv,.5f},{FDec,.3f},{ADec,.6f},{ASus,.3f},{Drive,.45f},{Vox,.5f},{Vowel,.3f},{RevMix,.35f} } },
      { "Rage Stab","Sirena Rage","Pibe Rabioso","Moshpit","Ultra Rage","Zombie Stab","Cuchillo Neon","Sangre Glitter","Caos Total","Cripta Rave" } });

    c.push_back({ "Plugg", {
        { {O1Wave,3},{O2Wave,0},{O2Oct,1},{O2Lvl,.4f},{FM,.2f},{Cut,5000},{AAtk,.002f},{ADec,.45f},{ASus,0},{ARel,.6f},{Chorus,.35f},{DelMix,.3f},{DelFb,.45f},{RevMix,.4f} },
        { {O1Wave,1},{Uni,3},{UniDet,.2f},{Cut,2800},{FEnv,.5f},{FDec,.2f},{ADec,.3f},{ASus,0},{ARel,.4f},{Chorus,.4f},{RevMix,.35f},{DelMix,.25f} },
        { {O1Wave,0},{O2Wave,0},{O2Semi,7},{O2Lvl,.35f},{FM,.35f},{Cut,9000},{ADec,.7f},{ASus,0},{ARel,1.f},{RevMix,.5f},{DelMix,.3f} },
        { {O1Wave,3},{O2Wave,1},{O2Lvl,.25f},{O2Oct,1},{Cut,3500},{Res,.2f},{FEnv,.4f},{ADec,.5f},{ASus,0},{ARel,.8f},{Chorus,.3f},{RevMix,.45f} } },
      { "Plugg Nene","Burbuja","Gotita Slime","Cristal Barrio","Nube Rosa","Lluvia Fina","Mocosito","Pastel","Algodon","Neblina" } });

    c.push_back({ "Hyperpop", {
        { {O1Wave,1},{Uni,7},{UniDet,.65f},{O2Wave,2},{O2Oct,1},{O2Lvl,.3f},{Cut,12000},{AAtk,.004f},{ASus,1},{ARel,.2f},{Drive,.5f},{Crush,.15f},{Chorus,.3f},{RevMix,.25f},{DelMix,.2f} },
        { {O1Wave,2},{Uni,3},{FM,.5f},{O2Wave,0},{O2Oct,2},{O2Lvl,.5f},{Cut,10000},{ADec,.3f},{ASus,.1f},{Crush,.4f},{Rate,.25f},{Drive,.4f},{DelMix,.35f} },
        { {O1Wave,1},{Uni,5},{UniDet,.4f},{PEnv,12},{PTime,.12f},{Cut,9000},{ADec,.3f},{ASus,.5f},{Drive,.6f},{Crush,.2f},{RevMix,.3f} },
        { {O1Wave,2},{Uni,7},{UniDet,.5f},{Cut,7000},{FEnv,.3f},{FDec,.3f},{AAtk,.01f},{ASus,.7f},{Chorus,.5f},{Drive,.35f},{RevMix,.4f} } },
      { "Caramelo Roto","Glitter Bomba","Unicornio Zombi","Azucar Digital","Chicle Laser","Pixel Llorando","Mega Pila","Corazon 8bit","Tuki Tuki","Sonrisa Falsa" } });

    c.push_back({ "808s", {
        { {O1Wave,0},{O1Oct,-2},{Mono,1},{Glide,.15f},{PEnv,12},{PTime,.04f},{Cut,3000},{AAtk,.002f},{ADec,1.8f},{ASus,.5f},{ARel,.35f},{Drive,.35f},{RevMix,0} },
        { {O1Wave,0},{O1Oct,-2},{O2Wave,2},{O2Oct,-1},{O2Lvl,.12f},{Mono,1},{Glide,.2f},{PEnv,10},{PTime,.05f},{Cut,3500},{AAtk,.002f},{ADec,2.f},{ASus,.6f},{ARel,.3f},{Drive,.75f},{RevMix,0} },
        { {O1Wave,0},{O1Oct,-2},{Mono,1},{Glide,.4f},{Cut,2500},{AAtk,.003f},{ADec,2.5f},{ASus,.9f},{ARel,.6f},{Drive,.4f},{RevMix,0} },
        { {O1Wave,1},{O1Oct,-2},{Uni,3},{UniDet,.1f},{Mono,1},{Glide,.2f},{Cut,400},{Res,.2f},{ASus,.9f},{ARel,.3f},{Drive,.5f},{RevMix,0} } },
      { "808 Conurbano","808 Villa","Sub Zona Sur","Bombo Gordo","808 Cemento","Tubo Roto","808 Cueva","Subsuelo","808 Lluvia","Trueno" } });

    c.push_back({ "Cumbia", {
        { {O1Wave,2},{O2Wave,2},{O2Oct,1},{O2Lvl,.3f},{O2Fine,6},{Cut,3800},{AAtk,.01f},{ASus,.9f},{ARel,.12f},{LRate,5.5f},{LPitch,.06f},{DelMix,.2f},{RevMix,.25f},{Drive,.15f} },
        { {O1Wave,2},{O2Wave,0},{O2Oct,1},{O2Lvl,.5f},{Cut,5000},{ASus,1},{ARel,.08f},{LRate,6},{LAmp,.12f},{Chorus,.35f} },
        { {O1Wave,1},{O2Wave,2},{O2Fine,9},{Uni,2},{UniDet,.15f},{FType,2},{Cut,1800},{Res,.2f},{AAtk,.03f},{ASus,1},{ARel,.1f},{LRate,5},{LAmp,.15f},{RevMix,.25f} },
        { {O1Wave,4},{Pw,.3f},{Cut,3000},{FEnv,.4f},{ADec,.25f},{ASus,.2f},{ARel,.15f},{DelMix,.2f} } },
      { "Cumbia 420","Baile de Barrio","Teclado Gordo","Tropi Under","Lead Villero","Acordeon Rancio","Organo Bailable","Verano Pesado","Cuarteto Neon","Chamame Glitch" } });

    c.push_back({ "Phonk", {
        { {O1Wave,2},{O2Wave,2},{O2Semi,7},{O2Lvl,.9f},{FType,2},{Cut,1800},{Res,.4f},{ADec,.18f},{ASus,0},{ARel,.1f},{Drive,.45f},{RevMix,.1f} },
        { {O1Wave,0},{O2Wave,0},{O2Oct,1},{FM,.4f},{Cut,4000},{ADec,.9f},{ASus,0},{ARel,1.2f},{RevMix,.5f},{Drive,.2f},{DelMix,.3f} },
        { {O1Wave,1},{Uni,3},{Vox,.8f},{Vowel,.25f},{Cut,3500},{AAtk,.05f},{ASus,1},{ARel,.4f},{RevMix,.5f},{Chorus,.3f},{Drive,.2f} },
        { {O1Wave,2},{O1Oct,-1},{Sub,.4f},{Cut,900},{Res,.3f},{FEnv,.4f},{FDec,.2f},{ADec,.5f},{ASus,.6f},{Drive,.6f},{Mono,1},{Glide,.1f} } },
      { "Cencerro Drift","Campana Muerta","Coro Memphis","Noche Fria","Drift Villa","Fantasma","Pesadilla","Humo Negro","Cadena","Sirena Oscura" } });

    // ---------------- INSTRUMENTOS (como las fotos) ----------------
    c.push_back({ "Banjo", {
        { {O1Wave,1},{O2Wave,4},{Pw,.25f},{O2Lvl,.5f},{FType,2},{Cut,2200},{Res,.3f},{FEnv,.5f},{FDec,.15f},{AAtk,.001f},{ADec,.28f},{ASus,0},{ARel,.2f},{RevMix,.15f} },
        { {O1Wave,3},{O2Wave,1},{O2Lvl,.4f},{O2Fine,5},{Cut,3200},{Res,.2f},{FEnv,.5f},{FDec,.2f},{ADec,.35f},{ASus,0},{ARel,.3f},{DelMix,.15f} } },
      { "Banjo Pibe","Banjo Rural","Banjo Roto","Banjo Pampa","Banjo Seco","Banjo Brillante","Banjo Doble","Banjo Dulce","Banjo Lluvia","Banjo Gaucho" } });

    c.push_back({ "Guitarra", {
        { {O1Wave,1},{Uni,2},{UniDet,.12f},{Cut,2200},{Res,.15f},{FEnv,.45f},{FDec,.3f},{ADec,.8f},{ASus,.05f},{ARel,.3f},{Chorus,.25f},{RevMix,.2f} },
        { {O1Wave,3},{O2Wave,4},{O2Lvl,.3f},{Pw,.35f},{Cut,3000},{FEnv,.4f},{FDec,.25f},{ADec,.6f},{ASus,.1f},{ARel,.4f},{Chorus,.4f},{DelMix,.2f} },
        { {O1Wave,2},{Uni,2},{UniDet,.1f},{Cut,2600},{Drive,.7f},{ADec,.9f},{ASus,.15f},{ARel,.2f},{RevMix,.15f} } },
      { "Guitarra Nena","Criolla Neon","Guitarra Sucia","Electrica Rota","Arpegio Lluvia","Acustica Fria","Rasguido","Guitarra Slime","Bossa Chueca","Guitarra Under" } });

    c.push_back({ "Piano", {
        { {O1Wave,3},{O2Wave,0},{O2Oct,1},{O2Lvl,.3f},{Cut,4500},{FEnv,.3f},{FDec,.5f},{ADec,1.4f},{ASus,.05f},{ARel,.4f},{RevMix,.25f} },
        { {O1Wave,0},{O2Wave,0},{O2Oct,1},{FM,.3f},{O2Lvl,.15f},{Cut,5000},{ADec,1.8f},{ASus,.1f},{ARel,.5f},{Chorus,.4f},{RevMix,.25f} },
        { {O1Wave,3},{Cut,2200},{Crush,.35f},{Rate,.1f},{ADec,1.2f},{ASus,.05f},{ARel,.5f},{Chorus,.3f},{RevMix,.3f} } },
      { "Piano Nene","Piano Lluvia","Piano Roto","Piano Vitrola","Rhodes Barrio","Piano Madrugada","Piano Mojado","Piano Lofi","Piano Cajon","Piano Triste" } });

    c.push_back({ "Cajita", {
        { {O1Wave,0},{O2Wave,0},{O2Oct,2},{O2Lvl,.35f},{FM,.12f},{Cut,9000},{AAtk,.001f},{ADec,.9f},{ASus,0},{ARel,1.3f},{RevMix,.4f},{DelMix,.2f} },
        { {O1Wave,3},{O2Wave,0},{O2Oct,1},{O2Lvl,.4f},{Cut,7000},{ADec,.7f},{ASus,0},{ARel,1.f},{RevMix,.4f},{Chorus,.2f} } },
      { "Cajita Musical","Bailarina","Cajita Rota","Cajita Luna","Cajita Nene","Dulce Sueno","Cajita Vieja","Mecanismo","Cajita Mini","Cajita Hada" } });

    c.push_back({ "Xilofon", {
        { {O1Wave,0},{O2Wave,0},{O2Oct,1},{O2Semi,7},{O2Lvl,.3f},{Cut,9000},{ADec,.3f},{ASus,0},{ARel,.25f},{RevMix,.2f} },
        { {O1Wave,3},{O2Wave,0},{O2Oct,1},{O2Lvl,.25f},{FM,.15f},{Cut,8000},{ADec,.4f},{ASus,0},{ARel,.3f},{RevMix,.25f} } },
      { "Xilofon Escuela","Xilofon Madera","Marimba Seca","Xilofon Chicle","Vibrafono Nene","Xilofon Glitch","Xilofon Plastico","Marimba Lluvia","Xilofon Mini","Xilofon Rebote" } });

    c.push_back({ "Kalimba", {
        { {O1Wave,3},{O2Wave,0},{O2Oct,1},{O2Lvl,.3f},{FM,.25f},{Cut,3500},{ADec,.6f},{ASus,0},{ARel,.9f},{RevMix,.3f},{DelMix,.2f} },
        { {O1Wave,0},{O2Wave,0},{O2Oct,1},{FM,.35f},{Cut,5000},{ADec,.8f},{ASus,0},{ARel,1.f},{RevMix,.35f},{Chorus,.2f} } },
      { "Kalimba Luz","Kalimba Rio","Kalimba Seca","Kalimba Piedra","Kalimba Eco","Kalimba Nena","Kalimba Brisa","Kalimba Mate","Kalimba Sol","Kalimba Fria" } });

    c.push_back({ "Teclado", {
        { {O1Wave,2},{O2Wave,1},{O2Lvl,.4f},{O2Fine,7},{Cut,3200},{AAtk,.005f},{ASus,.8f},{ARel,.15f},{Chorus,.3f} },
        { {O1Wave,4},{Pw,.3f},{O2Wave,3},{O2Lvl,.5f},{O2Oct,1},{Cut,4000},{ASus,.9f},{ARel,.12f},{Chorus,.2f} } },
      { "Teclado Casio","Teclado Escuela","Organo Feria","Teclado Basico","Casio Rapido","Teclado Boliche","Teclado Pop","Teclado Fiesta","Casio Lofi","Teclado Tonto" } });

    c.push_back({ "Stylophone", {
        { {Mono,1},{O1Wave,1},{O2Wave,2},{O2Lvl,.4f},{O2Fine,4},{Glide,.05f},{Cut,2600},{Res,.5f},{AAtk,.003f},{ASus,1},{ARel,.05f},{LRate,6},{LPitch,.07f},{Drive,.25f} },
        { {Mono,1},{O1Wave,4},{Pw,.4f},{Cut,2000},{Res,.45f},{LRate,5},{LPitch,.05f},{Drive,.2f},{ASus,1},{ARel,.06f},{DelMix,.15f} } },
      { "Stylo Zumbido","Stylo Nene","Stylo Pila","Stylo Robot","Stylo Gato","Stylo Mosca","Stylo Radio","Stylo Chicharra","Stylo Verde","Stylo Roto" } });

    c.push_back({ "Violin", {
        { {O1Wave,1},{Uni,3},{UniDet,.18f},{Cut,3200},{Res,.15f},{AAtk,.12f},{ASus,1},{ARel,.3f},{LRate,5.5f},{LPitch,.07f},{RevMix,.3f},{Chorus,.15f} },
        { {O1Wave,1},{Uni,4},{UniDet,.25f},{Cut,2400},{FEnv,.3f},{FAtk,.15f},{FDec,.5f},{FSus,.7f},{AAtk,.2f},{ASus,1},{ARel,.5f},{LRate,5},{LPitch,.06f},{RevMix,.4f} } },
      { "Violin Triste","Cuerdas Nena","Violin Hielo","Violin Gato","Cuerdas Niebla","Violin Roto","Violin Tango","Cuerdas Lluvia","Violin Solo","Violin Cuarteto" } });

    c.push_back({ "Acordeon", {
        { {O1Wave,2},{O2Wave,1},{O2Fine,9},{Uni,2},{UniDet,.15f},{Cut,2600},{AAtk,.03f},{ASus,1},{ARel,.1f},{LRate,5},{LAmp,.2f},{RevMix,.2f} },
        { {O1Wave,1},{O2Wave,2},{O2Fine,8},{FType,2},{Cut,1800},{Res,.25f},{AAtk,.04f},{ASus,1},{ARel,.12f},{LAmp,.18f},{LRate,4.5f},{RevMix,.2f} } },
      { "Acordeon Tango","Acordeon Pibe","Fuelle Roto","Acordeon Feria","Acordeon Neon","Bandoneon Sucio","Acordeon Humo","Acordeon Sur","Fuelle Lento","Bandoneon Nene" } });

    c.push_back({ "Vox", {
        { {O1Wave,1},{Uni,3},{UniDet,.12f},{Vox,1},{Vowel,0},{Cantar,1},{Cut,6000},{AAtk,.03f},{ASus,1},{ARel,.25f},{LRate,5.5f},{LPitch,.05f},{RevMix,.3f} },
        { {O1Wave,1},{Uni,2},{Vox,1},{Vowel,.5f},{Glide,.08f},{Mono,1},{Cut,7000},{AAtk,.02f},{ASus,1},{ARel,.2f},{LRate,6},{LPitch,.08f},{Chorus,.3f},{RevMix,.3f},{Drive,.15f} },
        { {O1Wave,2},{Vox,.9f},{Cantar,1},{Crush,.3f},{Rate,.15f},{Cut,6000},{Drive,.3f},{AAtk,.01f},{ASus,1},{ARel,.1f},{DelMix,.2f} },
        { {O1Wave,1},{Uni,5},{UniDet,.3f},{Vox,1},{Vowel,.75f},{Cut,6000},{AAtk,.25f},{ASus,1},{ARel,.8f},{Chorus,.5f},{RevMix,.5f} } },
      { "Yaiii Nene","Coro Peluche","Ayayay Dos","Talkbox Nene","Robot Ei Ei","Uaaa Bebe","Nam Nam","Voz Slime","Oi Oi Pibe","Coro Barrio" } });

    c.push_back({ "Plucks", {
        { {O1Wave,1},{O2Wave,4},{O2Lvl,.3f},{Cut,2600},{Res,.25f},{FEnv,.6f},{FDec,.12f},{ADec,.2f},{ASus,0},{ARel,.2f},{DelMix,.2f},{RevMix,.2f} },
        { {O1Wave,2},{Uni,3},{UniDet,.2f},{Cut,3500},{FEnv,.5f},{FDec,.15f},{ADec,.25f},{ASus,0},{ARel,.3f},{Chorus,.3f},{RevMix,.3f} },
        { {O1Wave,0},{FM,.45f},{O2Wave,0},{O2Oct,1},{O2Lvl,.1f},{Cut,7000},{ADec,.2f},{ASus,0},{ARel,.3f},{DelMix,.25f} } },
      { "Pluck Seco","Pluck Goma","Pluck Latigo","Pluck Chicle","Pluck Burbuja","Pluck Mojado","Pluck Rapido","Pluck Hielo","Pluck Cuerda","Pluck Flaco" } });

    c.push_back({ "Leads", {
        { {O1Wave,1},{Uni,5},{UniDet,.3f},{Cut,5500},{Res,.2f},{AAtk,.004f},{ASus,.9f},{ARel,.2f},{Mono,1},{Glide,.06f},{Drive,.25f},{DelMix,.25f},{RevMix,.2f} },
        { {O1Wave,2},{O2Wave,1},{O2Lvl,.4f},{O2Fine,8},{Cut,4500},{ASus,1},{LRate,5.5f},{LPitch,.06f},{Mono,1},{Glide,.04f},{DelMix,.2f},{RevMix,.2f} },
        { {O1Wave,0},{O2Wave,0},{O2Oct,1},{FM,.6f},{Cut,9000},{ASus,1},{ARel,.2f},{Mono,1},{Glide,.05f},{DelMix,.3f} } },
      { "Lead Cohete","Lead Sierra","Lead Nene","Lead Laser","Lead Mosquito","Lead Sol","Lead Pitido","Lead Aguja","Lead Estrella","Lead Chillon" } });

    c.push_back({ "Sintes", {
        { {O1Wave,1},{Uni,4},{UniDet,.25f},{Cut,2800},{FEnv,.4f},{FDec,.6f},{FSus,.3f},{AAtk,.01f},{ADec,.6f},{ASus,.6f},{ARel,.3f},{Chorus,.4f},{RevMix,.25f} },
        { {O1Wave,2},{O2Wave,1},{O2Lvl,.5f},{O2Fine,10},{Uni,3},{Cut,2200},{Res,.3f},{FEnv,.5f},{FDec,.4f},{ASus,.7f},{ARel,.25f},{Chorus,.3f},{DelMix,.2f} },
        { {O1Wave,1},{O2Wave,1},{O2Oct,-1},{O2Lvl,.6f},{Cut,1800},{FEnv,.6f},{FDec,.5f},{ASus,.6f},{Drive,.2f} } },
      { "Sinte Neon","Sinte Autopista","Sinte Boliche","Sinte Vidrio","Sinte Radar","Sinte Rosa","Sinte Humo","Sinte Ruta 8","Sinte Nene","Sinte Faro" } });

    c.push_back({ "Bajos", {
        { {O1Wave,1},{O1Oct,-1},{Sub,.5f},{Mono,1},{Cut,700},{Res,.3f},{FEnv,.4f},{FDec,.25f},{ADec,.5f},{ASus,.7f},{ARel,.12f},{Drive,.2f},{RevMix,0} },
        { {O1Wave,1},{O1Oct,-1},{Uni,3},{UniDet,.2f},{O2Wave,1},{O2Oct,-1},{O2Fine,12},{O2Lvl,.6f},{Mono,1},{Cut,600},{Res,.25f},{LRate,.3f},{LCut,.2f},{ASus,1},{ARel,.2f},{Drive,.3f},{RevMix,0} },
        { {O1Wave,0},{O1Oct,-1},{Sub,.4f},{Mono,1},{Cut,1500},{ADec,1.f},{ASus,.8f},{ARel,.15f},{Drive,.15f},{Glide,.08f},{RevMix,0} } },
      { "Bajo Cemento","Bajo Gordo","Bajo Pantano","Bajo Reese","Bajo Redondo","Bajo Cuadrado","Bajo Rutero","Bajo Mojado","Bajo Piedra","Bajo Nene" } });

    c.push_back({ "Percusion", {
        { {O1Wave,0},{O1Oct,-1},{PEnv,24},{PTime,.07f},{Cut,1500},{AAtk,.001f},{ADec,.35f},{ASus,0},{ARel,.1f},{Drive,.3f},{RevMix,0} },
        { {O1Wave,3},{PEnv,6},{PTime,.05f},{Noise,.8f},{FType,2},{Cut,3000},{Res,.2f},{ADec,.22f},{ASus,0},{ARel,.1f},{RevMix,.15f} },
        { {O1Lvl,0},{Noise,1},{FType,1},{Cut,8500},{Res,.1f},{ADec,.05f},{ASus,0},{ARel,.03f},{RevMix,.05f} },
        { {O1Lvl,0},{Noise,1},{FType,2},{Cut,1500},{Res,.3f},{ADec,.14f},{ASus,0},{ARel,.1f},{RevMix,.25f} },
        { {O1Wave,0},{PEnv,10},{PTime,.12f},{Noise,.05f},{Cut,4000},{ADec,.4f},{ASus,0},{ARel,.15f},{RevMix,.1f} } },
      { "Bombo","Caja","Hi Hat","Clap","Tom Grave","Bombo Seco","Caja Rota","Hat Abierto","Clap Gordo","Tom Agudo" } });

    c.push_back({ "Juguetes", {
        { {O1Wave,4},{Pw,.125f},{Cut,6000},{Crush,.35f},{Rate,.3f},{ADec,.2f},{ASus,.3f},{ARel,.1f},{RevMix,.05f} },
        { {O1Wave,2},{O2Wave,4},{Pw,.25f},{O2Oct,1},{O2Lvl,.4f},{Crush,.4f},{Rate,.35f},{ADec,.15f},{ASus,.4f},{ARel,.08f},{Mono,1},{Glide,.02f},{RevMix,.05f} },
        { {O1Wave,0},{FM,.4f},{O2Wave,0},{O2Oct,2},{Crush,.3f},{Cut,7000},{ADec,.25f},{ASus,0},{ARel,.2f},{RevMix,.2f} } },
      { "Juguete Pito","Peluche Robot","Autito","Sonajero","Pelota Rota","Consola 8bit","Muneca Rara","Trencito","Burbuja Plastica","Juguete Chillon" } });

    c.push_back({ "Pads", {
        { {O1Wave,1},{Uni,5},{UniDet,.3f},{Cut,1600},{FEnv,.3f},{FAtk,.8f},{FDec,1.5f},{FSus,.5f},{AAtk,.9f},{ASus,1},{ARel,1.8f},{LRate,.2f},{LCut,.15f},{Chorus,.5f},{RevMix,.6f} },
        { {O1Wave,3},{O2Wave,1},{O2Lvl,.4f},{O2Fine,6},{Uni,3},{Cut,2200},{AAtk,1.2f},{ASus,1},{ARel,2.f},{Chorus,.4f},{RevMix,.6f} },
        { {O1Wave,1},{Uni,5},{UniDet,.3f},{Vox,.7f},{Vowel,.2f},{AAtk,1.f},{ASus,1},{ARel,2.f},{RevMix,.6f} } },
      { "Pad Niebla","Pad Nube","Pad Dormido","Pad Cielo","Pad Humo","Pad Panza","Pad Ola","Pad Sueno","Pad Lluvioso","Pad Atardecer" } });

    c.push_back({ "Raros", {
        { {O1Wave,2},{O2Wave,1},{FM,.7f},{O2Oct,1},{Cut,3500},{Res,.5f},{LRate,7},{LShape,3},{LCut,.6f},{Crush,.3f},{DelMix,.4f},{DelFb,.6f},{RevMix,.3f} },
        { {O1Wave,4},{Pw,.15f},{LRate,3},{LShape,1},{LPitch,.3f},{LCut,.4f},{Drive,.4f},{Rate,.3f},{DelMix,.4f},{DelFb,.6f} },
        { {O1Wave,1},{Uni,7},{UniDet,1},{PEnv,-12},{PTime,.4f},{Cut,2000},{Res,.6f},{LRate,.5f},{LCut,.5f},{AAtk,.3f},{ASus,1},{ARel,1},{Vox,.5f},{Vowel,.6f},{RevMix,.5f},{DelMix,.3f} } },
      { "Raro Moco","Raro Burbuja","Raro Ovni","Raro Tos","Raro Cucaracha","Raro Gelatina","Raro Escalera","Raro Hipo","Raro Alien","Raro Pantano" } });

    return c;
}

std::vector<Preset> build()
{
    auto cats = makeCats();
    const PDef* d = defs();
    std::vector<Preset> out;

    for (int ci = 0; ci < (int) cats.size(); ++ci)
    {
        auto& cd = cats[(size_t) ci];
        const juce::String cn(cd.name);
        const bool noSwap = cn == juce::String("Percusion") || cn == juce::String("808s") || cn == juce::String("Bajos")
                         || cn == juce::String("Cajita") || cn == juce::String("Xilofon") || cn == juce::String("Vox");
        const bool wild = cn == juce::String("Raros");

        for (int pass = 0; pass < 2; ++pass)
        for (int ni = 0; ni < (int) cd.names.size(); ++ni)
        {
            Preset p;
            p.cat = ci;
            p.name = juce::String(cd.names[(size_t) ni]) + (pass ? " 2" : "");
            auto& v = p.v;
            for (int k = 0; k < NumParams; ++k) v[(size_t) k] = d[k].def;
            for (auto& pr : cd.rc[(size_t) ni % cd.rc.size()]) v[(size_t) pr.first] = pr.second;

            juce::Random rng(ci * 1000 + ni * 10 + pass + 12345);
            const float s = pass ? 1.0f : 0.6f;
            auto jit = [&](float a) { return (rng.nextFloat() * 2.f - 1.f) * a; };

            v[Cut]     *= std::exp(jit(0.5f * s));
            v[Res]     += jit(0.08f * s);
            v[UniDet]  += jit(0.12f * s);
            v[O1Fine]  += jit(6.f * s);
            v[O2Fine]  += jit(5.f * s);
            v[ADec]    *= std::exp(jit(0.4f * s));
            v[ARel]    *= std::exp(jit(0.4f * s));
            v[FDec]    *= std::exp(jit(0.4f * s));
            v[Chorus]  += jit(0.15f * s);
            v[RevMix]  += jit(0.12f * s);
            v[DelMix]  += jit(0.1f * s);
            v[Drive]   += v[Drive] > 0.f ? jit(0.12f * s) : 0.f;
            v[Pw]      += jit(0.1f * s);
            v[Vowel]   += jit(0.3f * s);
            static const float dts[] = { .18f, .25f, .33f, .375f, .5f };
            v[DelTime] = dts[rng.nextInt(5)];

            if (! noSwap && (v[O1Wave] == 1 || v[O1Wave] == 2 || v[O1Wave] == 4) && rng.nextFloat() < 0.2f + 0.15f * (float) pass)
            { static const float w[] = { 1, 2, 4, 3 }; v[O1Wave] = w[rng.nextInt(4)]; }

            if (wild)
            { v[FM] = rng.nextFloat(); v[LRate] = 0.3f + rng.nextFloat() * 12.f; v[LShape] = (float) rng.nextInt(4); v[LPitch] += rng.nextFloat() * 0.3f; v[Rate] += rng.nextFloat() * 0.3f; }

            // melodia que toca el personaje
            if (cn == juce::String("Cumbia"))
            { static const float sc[] = { 0, 1, 6 }; v[Scale] = sc[rng.nextInt(3)]; v[Bpm] = 95.f + (float) rng.nextInt(3) * 5.f; v[Dens] = 0.5f + rng.nextFloat() * 0.3f; }
            else if (ci < 6)
            { static const float sc[] = { 1, 2, 3, 0, 6 }; v[Scale] = sc[rng.nextInt(5)]; v[Bpm] = 130.f + 5.f * (float) rng.nextInt(9); v[Dens] = 0.5f + rng.nextFloat() * 0.4f; }
            else
            { static const float sc[] = { 0, 1, 4, 5, 3 }; v[Scale] = sc[rng.nextInt(5)]; v[Bpm] = 100.f + 5.f * (float) rng.nextInt(9); v[Dens] = 0.35f + rng.nextFloat() * 0.35f; }

            for (int k = 0; k < NumPreset; ++k)
            {
                float x = juce::jlimit(d[k].mn, d[k].mx, v[(size_t) k]);
                if (d[k].step > 0.f) x = std::round(x / d[k].step) * d[k].step;
                v[(size_t) k] = x;
            }
            out.push_back(p);
        }
    }
    return out;
}
} // namespace

const std::vector<Preset>& factoryPresets() { static const std::vector<Preset> p = build(); return p; }

const juce::StringArray& categoryNames()
{
    static const juce::StringArray n = [] { juce::StringArray s; for (auto& c : makeCats()) s.add(c.name); s.add("User"); return s; }();
    return n;
}
