#pragma once
#include "PluginProcessor.h"

namespace ub
{
    constexpr juce::uint32 ink = 0xff3a2b22, paper = 0xfff3e5b8, gold = 0xffe8b23a;
    inline juce::Font F(float h, bool bold = true) { return juce::Font(juce::FontOptions(h, bold ? juce::Font::bold : juce::Font::plain)); }
}

//==============================================================================
class UBLook : public juce::LookAndFeel_V4
{
public:
    UBLook();
    void drawRotarySlider(juce::Graphics&, int x, int y, int w, int h, float pos, float a0, float a1, juce::Slider&) override;
    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour&, bool, bool) override;
    void drawButtonText(juce::Graphics&, juce::TextButton&, bool, bool) override;
    void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool, bool) override;
    juce::Font getComboBoxFont(juce::ComboBox&) override { return ub::F(11.f); }
    juce::Font getTextButtonFont(juce::TextButton&, int) override { return ub::F(12.f); }
};

//==============================================================================
// Un control de parametro (perilla, combo o boton) con su etiqueta
class Ctl : public juce::Component
{
public:
    Ctl(juce::AudioProcessorValueTreeState& apvts, int p, int tabIndex);
    void resized() override;
    int tab;
private:
    juce::Slider sl; juce::ComboBox cb; juce::ToggleButton tg; juce::Label lab;
    std::unique_ptr<juce::SliderParameterAttachment> a1;
    std::unique_ptr<juce::ComboBoxParameterAttachment> a2;
    std::unique_ptr<juce::ButtonParameterAttachment> a3;
    int type;
};

//==============================================================================
class DrawPad : public juce::Component
{
public:
    explicit DrawPad(UnderBoxProcessor& p) : proc(p) {}
    void paint(juce::Graphics&) override;
    void mouseDown(const juce::MouseEvent& e) override { lastI = -1; setAt(e); }
    void mouseDrag(const juce::MouseEvent& e) override { setAt(e); }
    void mouseUp(const juce::MouseEvent&) override { lastI = -1; useDrawn(); }
    void useDrawn();
private:
    void setAt(const juce::MouseEvent&);
    UnderBoxProcessor& proc; int lastI = -1; float lastV = 0.f;
};

class ScopeView : public juce::Component, private juce::Timer
{
public:
    explicit ScopeView(UnderBoxProcessor& p) : proc(p) { startTimerHz(30); }
    ~ScopeView() override { stopTimer(); }
    void paint(juce::Graphics&) override;
private:
    void timerCallback() override { repaint(); }
    UnderBoxProcessor& proc;
};

//==============================================================================
// El personaje: toca el teclado o canta con el microfono segun el preset
class Stage : public juce::Component, private juce::Timer
{
public:
    explicit Stage(UnderBoxProcessor& p);
    ~Stage() override { stopTimer(); }
    void paint(juce::Graphics&) override;
    int getSkinCount() const { return custom.isValid() ? 6 : 5; }
private:
    void timerCallback() override;
    void drawMouth(juce::Graphics&, float cx, float my);
    struct Note { float x, y, vx, life; int col; };
    UnderBoxProcessor& proc;
    std::atomic<float>* voxP; std::atomic<float>* cantarP;
    juce::Image custom;
    std::vector<Note> notes;
    float t = 0, lvl = 0, vowel = 0, rhx = 0, lhx = 0, rpress = 0, lpress = 0, blink = 0, open = 0;
    uint64_t prevHeld[2] = { 0, 0 };
    int heldCount = 0, lowNote = 60, highNote = 60;
    bool singing = false;
    juce::Random rnd;
};

//==============================================================================
class UnderBoxEditor;
class CatModel : public juce::ListBoxModel
{
public:
    explicit CatModel(UnderBoxEditor& e) : ed(e) {}
    int getNumRows() override;
    void paintListBoxItem(int, juce::Graphics&, int, int, bool) override;
    void selectedRowsChanged(int) override;
    UnderBoxEditor& ed;
};
class PresModel : public juce::ListBoxModel
{
public:
    explicit PresModel(UnderBoxEditor& e) : ed(e) {}
    int getNumRows() override;
    void paintListBoxItem(int, juce::Graphics&, int, int, bool) override;
    void selectedRowsChanged(int) override;
    UnderBoxEditor& ed;
};

//==============================================================================
class UnderBoxEditor : public juce::AudioProcessorEditor
{
public:
    explicit UnderBoxEditor(UnderBoxProcessor&);
    ~UnderBoxEditor() override;
    void paint(juce::Graphics&) override;

    void setCategory(int c);
    void loadVisible(int row);
    juce::StringArray visNames;

private:
    struct Title { int tab, row; juce::String text; };
    void addRow(int tab, int row, const char* title, std::initializer_list<int> ps);
    void setTab(int t);
    void askSaveName();

    UnderBoxProcessor& proc;
    UBLook look;
    int tab = 0, curCat = 0;
    std::vector<int> visIdx;
    juce::Array<juce::File> userFiles;

    juce::OwnedArray<juce::TextButton> tabBtns;
    std::vector<std::unique_ptr<Ctl>> ctls;
    std::vector<Title> titles;
    CatModel catModel { *this };
    PresModel presModel { *this };
    juce::ListBox catList, presList;
    juce::TextButton saveBtn { "GUARDAR" }, folderBtn { "MI CARPETA" }, skinBtn { "SKIN" }, newMelBtn { "NUEVA MELODIA" };
    juce::OwnedArray<juce::TextButton> drawBtns;
    std::unique_ptr<Ctl> playBottom, cantarBottom;
    DrawPad pad;
    ScopeView scopeV;
    Stage stage;
    juce::MidiKeyboardComponent kb;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnderBoxEditor)
};
