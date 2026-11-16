# Under Box (VST3)

Sinte con 500 presets (Rage, Plugg, Hyperpop, 808s, Cumbia, Phonk + instrumentos raros) y un personaje que toca o canta las melodias.

## Como conseguir el .vst3 (sin instalar nada)
1. Crea un repositorio nuevo en github.com y subi toda esta carpeta (con `.github`).
2. Anda a la pestana **Actions** -> "Build Under Box" -> cuando termine (10-15 min) baja el artefacto **UnderBox-Windows** (o macOS).
3. Windows: descomprimi y copia la carpeta `Under Box.vst3` a `C:\Program Files\Common Files\VST3\`.
   macOS: copia `Under Box.vst3` a `/Library/Audio/Plug-Ins/VST3/` (si no abre: `xattr -cr "Under Box.vst3"`).
4. En FL Studio: Options > Manage plugins > Find installed plugins.

## Compilar en tu PC
Necesitas CMake + Visual Studio (Windows) o Xcode (macOS):
```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --target UnderBox_VST3
```
El plugin queda en `build/UnderBox_artefacts/Release/VST3/`. Tambien genera un `.exe` Standalone para probarlo sin DAW.

## Uso
- **PRESETS**: categorias a la izquierda, lista a la derecha. GUARDAR guarda el sonido actual en Documentos/UnderBox (categoria User). MI CARPETA abre esa carpeta.
- **SONIDO**: perillas rapidas. **MOTOR**: osciladores, unison, FM, filtro, envolventes, LFO. **FX**: drive, bitcrush, chorus, delay, reverb.
- **DIBUJAR**: dibuja tu propia onda con el mouse (se usa como Onda 1).
- **NARU**: TOCA = el personaje toca una melodia solo (usa el BPM del DAW si esta en play). CANTA = cada nota es una silaba con voz de formantes (Vox). NUEVA MELODIA genera otra; escala, nota base, BPM y densidad cambian el estilo.
- **SKIN**: cambia los colores del personaje. Para usar tu propio dibujo guardalo como `personaje.png` en Documentos/UnderBox y aparece como skin extra.
