#include "PluginEditor.h"
#include <cmath>

using juce::Colour;

//==============================================================================  LOOK
UBLook::UBLook()
{
    setColour(juce::ComboBox::backgroundColourId, Colour(0xffffefc2));
    setColour(juce::ComboBox::textColourId, Colour(ub::ink));
    setColour(juce::ComboBox::outlineColourId, Colour(ub::ink));
    setColour(juce::ComboBox::arrowColourId, Colour(ub::ink));
    setColour(juce::PopupMenu::backgroundColourId, Colour(0xfffff3d0));
    setColour(juce::PopupMenu::textColourId, Colour(ub::ink));
    setColour(juce::PopupMenu::highlightedBackgroundColourId, Colour(ub::gold));
    setColour(juce::PopupMenu::highlightedTextColourId, Colour(ub::ink));
    setColour(juce::TextButton::buttonColourId, Colour(0xffff6fae));
    setColour(juce::TextButton::buttonOnColourId, Colour(ub::gold));
    setColour(juce::TextButton::textColourOffId, Colour(ub::ink));
    setColour(juce::TextButton::textColourOnId, Colour(ub::ink));
    setColour(juce::ListBox::backgroundColourId, juce::Colours::transparentBlack);
    setColour(juce::Label::textColourId, Colour(ub::ink));
}

void UBLook::drawRotarySlider(juce::Graphics& g, int x, int y, int w, int h, float pos, float a0, float a1, juce::Slider&)
{
    auto b = juce::Rectangle<float>((float) x, (float) y, (float) w, (float) h).reduced(3.f);
    const auto c = b.getCentre();
    const float r = juce::jmin(b.getWidth(), b.getHeight()) * 0.5f;
    g.setColour(Colour(0xff2a1a12)); g.fillEllipse(b.expanded(2.f));
    juce::Path track; track.addCentredArc(c.x, c.y, r, r, 0.f, a0, a1, true);
    g.setColour(Colour(0x44ffffff)); g.strokePath(track, juce::PathStrokeType(2.5f));
    const float ang = a0 + pos * (a1 - a0);
    juce::Path val; val.addCentredArc(c.x, c.y, r, r, 0.f, a0, ang, true);
    g.setColour(Colour(0xffb6ff3b)); g.strokePath(val, juce::PathStrokeType(3.f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    auto kb = b.reduced(5.f);
    g.setGradientFill(juce::ColourGradient(Colour(0xfffff3cf), c.x - r * .4f, c.y - r * .5f, Colour(0xffe0a040), c.x + r * .6f, c.y + r * .7f, true));
    g.fillEllipse(kb);
    g.setColour(Colour(ub::ink));
    g.drawLine(c.x, c.y, c.x + std::sin(ang) * r * 0.62f, c.y - std::cos(ang) * r * 0.62f, 3.f);
}

void UBLook::drawButtonBackground(juce::Graphics& g, juce::Button& b, const Colour&, bool over, bool down)
{
    auto r = b.getLocalBounds().toFloat().reduced(1.5f);
    Colour c = b.getToggleState() ? b.findColour(juce::TextButton::buttonOnColourId) : b.findColour(juce::TextButton::buttonColourId);
    if (down) c = c.darker(0.25f); else if (over) c = c.brighter(0.15f);
    g.setColour(c); g.fillRoundedRectangle(r, r.getHeight() * 0.5f);
    g.setColour(Colour(0xff2a1a12)); g.drawRoundedRectangle(r, r.getHeight() * 0.5f, 2.f);
}

void UBLook::drawButtonText(juce::Graphics& g, juce::TextButton& b, bool, bool)
{
    g.setFont(ub::F(12.f));
    g.setColour(Colour(ub::ink));
    g.drawText(b.getButtonText(), b.getLocalBounds(), juce::Justification::centred);
}

void UBLook::drawToggleButton(juce::Graphics& g, juce::ToggleButton& b, bool over, bool)
{
    auto r = b.getLocalBounds().toFloat().reduced(1.5f);
    Colour c = b.getToggleState() ? Colour(0xffa6e03a) : Colour(0xffd9c9a0);
    if (over) c = c.brighter(0.15f);
    g.setColour(c); g.fillRoundedRectangle(r, r.getHeight() * 0.5f);
    g.setColour(Colour(0xff2a1a12)); g.drawRoundedRectangle(r, r.getHeight() * 0.5f, 2.f);
    g.setColour(Colour(ub::ink)); g.setFont(ub::F(11.5f));
    g.drawText(b.getButtonText(), b.getLocalBounds(), juce::Justification::centred);
}

//==============================================================================  CTL
Ctl::Ctl(juce::AudioProcessorValueTreeState& apvts, int p, int tabIndex) : tab(tabIndex), type(defs()[p].type)
{
    const auto& d = defs()[p];
    auto* prm = apvts.getParameter(d.id);
    if (type == 0)
    {
        sl.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        sl.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        sl.setRotaryParameters(juce::MathConstants<float>::pi * 1.2f, juce::MathConstants<float>::pi * 2.8f, true);
        sl.setPopupDisplayEnabled(true, true, nullptr);
        addAndMakeVisible(sl);
        a1 = std::make_unique<juce::SliderParameterAttachment>(*prm, sl);
        sl.setDoubleClickReturnValue(true, d.def);
    }
    else if (type == 1)
    {
        if (auto* ch = dynamic_cast<juce::AudioParameterChoice*>(prm)) cb.addItemList(ch->choices, 1);
        addAndMakeVisible(cb);
        a2 = std::make_unique<juce::ComboBoxParameterAttachment>(*prm, cb);
    }
    else
    {
        tg.setButtonText(d.name);
        addAndMakeVisible(tg);
        a3 = std::make_unique<juce::ButtonParameterAttachment>(*prm, tg);
    }
    if (type != 2)
    {
        lab.setText(d.name, juce::dontSendNotification);
        lab.setFont(ub::F(10.5f)); lab.setJustificationType(juce::Justification::centred);
        lab.setColour(juce::Label::textColourId, Colour(ub::ink));
        lab.setInterceptsMouseClicks(false, false);
        addAndMakeVisible(lab);
    }
}

void Ctl::resized()
{
    if (type == 0) { sl.setBounds(6, 0, 44, 44); lab.setBounds(0, 46, getWidth(), 13); }
    else if (type == 1) { cb.setBounds(2, 10, getWidth() - 4, 22); lab.setBounds(0, 46, getWidth(), 13); }
    else if (getHeight() < 40) tg.setBounds(getLocalBounds());
    else tg.setBounds(2, 10, getWidth() - 4, 22);
}

//==============================================================================  DRAW PAD / SCOPE
void DrawPad::paint(juce::Graphics& g)
{
    auto b = getLocalBounds().toFloat();
    g.setColour(Colour(0xff1b1530)); g.fillRoundedRectangle(b, 8.f);
    g.setColour(Colour(0x22ffffff));
    for (int i = 1; i < 8; ++i) g.drawVerticalLine((int) (b.getWidth() * (float) i / 8.f), 0.f, b.getHeight());
    g.drawHorizontalLine((int) b.getCentreY(), 0.f, b.getWidth());
    juce::Path p;
    for (int i = 0; i < 256; ++i)
    {
        const float x = b.getWidth() * (float) i / 255.f, y = b.getCentreY() - proc.draw[(size_t) i].load() * b.getHeight() * 0.45f;
        if (i == 0) p.startNewSubPath(x, y); else p.lineTo(x, y);
    }
    g.setColour(Colour(0xffb6ff3b)); g.strokePath(p, juce::PathStrokeType(2.5f));
}

void DrawPad::setAt(const juce::MouseEvent& e)
{
    auto b = getLocalBounds().toFloat();
    const int i = juce::jlimit(0, 255, (int) (e.position.x / b.getWidth() * 256.f));
    const float v = juce::jlimit(-1.f, 1.f, (b.getCentreY() - e.position.y) / (b.getHeight() * 0.45f));
    if (lastI >= 0 && lastI != i)
    {
        int a = lastI, c = i; float va = lastV, vc = v;
        if (a > c) { std::swap(a, c); std::swap(va, vc); }
        for (int k = a; k <= c; ++k) proc.draw[(size_t) k].store(va + (vc - va) * (float) (k - a) / (float) (c - a));
    }
    else proc.draw[(size_t) i].store(v);
    lastI = i; lastV = v; repaint();
}

void DrawPad::useDrawn()
{
    if (auto* prm = proc.apvts.getParameter("o1wave")) prm->setValueNotifyingHost(prm->convertTo0to1(5.f));
}

void ScopeView::paint(juce::Graphics& g)
{
    auto b = getLocalBounds().toFloat();
    g.setColour(Colour(0xff17301f)); g.fillRoundedRectangle(b, 8.f);
    g.setColour(Colour(0xff6b4a2a)); g.drawRoundedRectangle(b.reduced(1.f), 8.f, 3.f);
    const int pos = proc.scopePos.load(std::memory_order_acquire);
    const int start = pos - 1200;
    int off = 0;
    for (int i = 0; i < 600; ++i)
        if (proc.scope[(size_t) ((start + i) & 2047)].load() < 0.f && proc.scope[(size_t) ((start + i + 1) & 2047)].load() >= 0.f) { off = i; break; }
    float peak = 0.1f;
    for (int i = 0; i < 512; ++i) peak = juce::jmax(peak, std::abs(proc.scope[(size_t) ((start + off + i) & 2047)].load()));
    juce::Path p;
    auto in = b.reduced(8.f, 6.f);
    for (int i = 0; i < 512; ++i)
    {
        const float s = proc.scope[(size_t) ((start + off + i) & 2047)].load() / peak;
        const float x = in.getX() + in.getWidth() * (float) i / 511.f, y = in.getCentreY() - s * in.getHeight() * 0.45f;
        if (i == 0) p.startNewSubPath(x, y); else p.lineTo(x, y);
    }
    g.setColour(Colour(0xff55ff7a)); g.strokePath(p, juce::PathStrokeType(2.f));
}

//==============================================================================  STAGE (personaje)
namespace
{
struct SkinCol { juce::uint32 hood, acc, bg1, bg2; };
const SkinCol SK[5] = {
    { 0xffff5fa2, 0xffb6ff3b, 0xff1b1f4b, 0xff4a1f6d },
    { 0xff8ae234, 0xffff4fa0, 0xff2d1b4b, 0xff0e0a1a },
    { 0xff2a2a35, 0xffff3b3b, 0xff5a1020, 0xff12060a },
    { 0xff4cc9f0, 0xffffd60a, 0xff0b2a5b, 0xff0e6f7a },
    { 0xfff1f1f1, 0xff9b5cff, 0xffff8ac6, 0xff6a3fd8 } };
const float WS[5] = { 1.0f, 1.25f, 1.4f, 0.8f, 0.6f }, HS[5] = { 1.0f, 0.55f, 0.4f, 0.9f, 0.6f };
}

Stage::Stage(UnderBoxProcessor& p) : proc(p)
{
    voxP = p.apvts.getRawParameterValue("vox");
    cantarP = p.apvts.getRawParameterValue("cantar");
    // Si queres tu propio dibujo, guardalo como  Documentos/UnderBox/personaje.png
    custom = juce::ImageFileFormat::loadFrom(UnderBoxProcessor::userDir().getChildFile("personaje.png"));
    startTimerHz(30);
}

void Stage::timerCallback()
{
    t += 1.f / 30.f;
    lvl += (proc.level.load() - lvl) * 0.4f;
    vowel += (proc.vowelUI.load() - vowel) * 0.4f;
    singing = voxP->load() > 0.3f || cantarP->load() > 0.5f;

    uint64_t cur[2] = { proc.held[0].load(), proc.held[1].load() };
    heldCount = 0; lowNote = -1; highNote = -1;
    const float cx = (float) getWidth() * 0.5f;
    for (int n = 0; n < 128; ++n)
    {
        const bool on = (cur[n >> 6] >> (n & 63)) & 1ull, was = (prevHeld[n >> 6] >> (n & 63)) & 1ull;
        if (on) { ++heldCount; if (lowNote < 0) lowNote = n; highNote = n; }
        if (on && ! was && notes.size() < 24)
            notes.push_back({ singing ? cx + 16.f : cx - 80.f + 160.f * (float) (juce::jlimit(36, 96, n) - 36) / 60.f,
                              singing ? (float) getHeight() * 0.38f - 4.f : (float) getHeight() - 90.f,
                              (rnd.nextFloat() - 0.5f) * 1.2f, 1.f, rnd.nextInt(4) });
    }
    prevHeld[0] = cur[0]; prevHeld[1] = cur[1];

    auto mapX = [&](int n) { return cx - 80.f + 160.f * (float) (juce::jlimit(36, 96, n) - 36) / 60.f; };
    const float tr = heldCount ? mapX(highNote) : cx + 48.f;
    const float tl = heldCount > 1 ? mapX(lowNote) : cx - 48.f;
    rhx += (tr - rhx) * 0.35f; lhx += (tl - lhx) * 0.35f;
    rpress += ((heldCount > 0 ? 1.f : 0.f) - rpress) * 0.4f;
    lpress += ((heldCount > 1 ? 1.f : 0.f) - lpress) * 0.4f;
    open += (juce::jlimit(0.f, 1.f, lvl * 7.f) - open) * 0.5f;

    blink -= 1.f / 30.f;
    if (blink <= 0.f && rnd.nextFloat() < 0.012f) blink = 0.12f;

    for (auto& n : notes) { n.y -= 1.4f; n.x += n.vx + std::sin(n.y * 0.08f) * 0.5f; n.life -= 0.018f; }
    notes.erase(std::remove_if(notes.begin(), notes.end(), [](const Note& n) { return n.life <= 0.f; }), notes.end());
    repaint();
}

void Stage::drawMouth(juce::Graphics& g, float cx, float my)
{
    const int i0 = juce::jlimit(0, 3, (int) vowel), i1 = juce::jmin(4, i0 + 1);
    const float fr = juce::jlimit(0.f, 1.f, vowel - (float) i0);
    const float ws = WS[i0] + (WS[i1] - WS[i0]) * fr, hs = HS[i0] + (HS[i1] - HS[i0]) * fr;
    if (open < 0.07f)
    {
        juce::Path p; p.startNewSubPath(cx - 10.f, my); p.quadraticTo(cx, my + 9.f, cx + 10.f, my);
        g.setColour(Colour(0xff3a1a1a));
        g.strokePath(p, juce::PathStrokeType(2.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        return;
    }
    const float mw = 20.f * ws, mh = (5.f + 26.f * open) * hs;
    juce::Path mp; mp.addEllipse(cx - mw * 0.5f, my - 2.f, mw, mh);
    g.setColour(Colour(0xff5a0f1e)); g.fillPath(mp);
    g.saveState(); g.reduceClipRegion(mp);
    g.setColour(Colour(0xffff7a93)); g.fillEllipse(cx - mw * 0.28f, my + mh * 0.5f, mw * 0.56f, mh * 0.5f);
    g.restoreState();
}

void Stage::paint(juce::Graphics& g)
{
    const float w = (float) getWidth(), h = (float) getHeight(), cx = w * 0.5f;
    const int si = juce::jlimit(0, 5, proc.skin.load());
    const bool useImg = si == 5 && custom.isValid();
    const SkinCol& sk = SK[useImg ? 0 : juce::jmin(si, 4)];
    const Colour hood(sk.hood), acc(sk.acc), cream(0xfffff0d8);

    g.setGradientFill(juce::ColourGradient(Colour(sk.bg1), 0.f, 0.f, Colour(sk.bg2), 0.f, h, false));
    g.fillRoundedRectangle(0.f, 0.f, w, h, 10.f);
    g.setColour(juce::Colours::white.withAlpha(0.06f));
    for (int y = 0; y < (int) h; y += 26)
        for (int x = ((y / 26) % 2) * 13; x < (int) w; x += 26) g.fillEllipse((float) x, (float) y, 9.f, 9.f);

    // logo
    static const juce::uint32 lc[] = { 0xffff6fae, 0xffb6ff3b, 0xffffd60a, 0xff4cc9f0 };
    const juce::String logo("UNDER BOX");
    g.setFont(ub::F(26.f));
    const float x0 = cx - (float) logo.length() * 10.f;
    for (int i = 0; i < logo.length(); ++i)
    {
        g.setColour(Colour(lc[i & 3]));
        g.drawText(juce::String::charToString(logo[i]), (int) (x0 + (float) i * 20.f), 6 + (int) (std::sin(t * 3.f + (float) i) * 2.f), 22, 30, juce::Justification::centred);
    }
    // globito con el nombre del preset
    juce::Rectangle<float> bub(cx - 105.f, 42.f, 210.f, 22.f);
    g.setColour(Colour(ub::paper)); g.fillRoundedRectangle(bub, 4.f);
    g.setColour(Colour(ub::ink)); g.setFont(ub::F(12.f));
    g.drawText(proc.currentPresetName, bub.toNearestInt(), juce::Justification::centred);

    const float bob = std::sin(t * 7.f) * (1.f + 5.f * juce::jmin(1.f, lvl * 5.f));
    const float hy = h * 0.40f + bob, R = 56.f, keyTop = h - 70.f;

    if (useImg)
    {
        g.saveState();
        g.addTransform(juce::AffineTransform::scale(1.f + lvl * 0.4f, 1.f - lvl * 0.2f, cx, h - 20.f));
        g.drawImage(custom, juce::Rectangle<float>(cx - 110.f, 70.f + bob, 220.f, h - 100.f), juce::RectanglePlacement::centred);
        g.restoreState();
    }
    else
    {
        // mini teclado de utileria
        if (! singing)
        {
            g.setColour(Colour(0xff2a1a12)); g.fillRoundedRectangle(cx - 96.f, keyTop, 192.f, 40.f, 6.f);
            g.setColour(Colour(0xfff5f0e0)); g.fillRoundedRectangle(cx - 90.f, keyTop + 4.f, 180.f, 32.f, 3.f);
            g.setColour(Colour(0xff2a1a12));
            for (int i = 1; i < 18; ++i) g.drawVerticalLine((int) (cx - 90.f + 10.f * (float) i), keyTop + 4.f, keyTop + 36.f);
        }
        // cuerpo
        const float torsoTop = hy + R - 8.f;
        g.setColour(hood); g.fillRoundedRectangle(cx - 36.f, torsoTop, 72.f, 70.f, 22.f);
        g.setColour(acc); g.fillRect(cx - 36.f, torsoTop + 34.f, 72.f, 8.f);
        // brazos
        auto arm = [&](float sx, float sy, float hx, float hy2, float side)
        {
            juce::Path p; p.startNewSubPath(sx, sy);
            p.quadraticTo((sx + hx) * 0.5f + side * 18.f, (sy + hy2) * 0.5f + 14.f, hx, hy2);
            g.setColour(hood); g.strokePath(p, juce::PathStrokeType(12.f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            g.setColour(cream); g.fillEllipse(hx - 8.f, hy2 - 8.f, 16.f, 16.f);
            g.setColour(Colour(0xff2a1a12)); g.drawEllipse(hx - 8.f, hy2 - 8.f, 16.f, 16.f, 1.5f);
        };
        const float shy = torsoTop + 14.f;
        float rx, ry, lx, ly;
        if (singing)
        {
            rx = cx + 40.f; ry = hy + 62.f;
            lx = cx - 64.f; ly = hy + 18.f + std::sin(t * 9.f) * 8.f * (0.3f + juce::jmin(1.f, lvl * 6.f));
        }
        else
        {
            rx = rhx; ry = keyTop - 12.f + 9.f * rpress;
            lx = lhx; ly = keyTop - 12.f + 9.f * lpress;
        }
        arm(cx - 32.f, shy, lx, ly, -1.f);
        arm(cx + 32.f, shy, rx, ry, 1.f);

        // cabeza
        g.setColour(hood); g.fillEllipse(cx - 40.f - 8.f, hy - R - 8.f, 28.f, 28.f); g.fillEllipse(cx + 20.f + 8.f, hy - R - 8.f, 28.f, 28.f);
        g.fillEllipse(cx - R, hy - R, R * 2.f, R * 2.f);
        g.setColour(Colour(0xff2a1a12)); g.drawEllipse(cx - R, hy - R, R * 2.f, R * 2.f, 2.f);
        g.setColour(cream); g.fillEllipse(cx - 44.f, hy - 28.f, 88.f, 76.f);
        // vincha + colitas
        g.saveState();
        juce::Path hc; hc.addEllipse(cx - R, hy - R, R * 2.f, R * 2.f); g.reduceClipRegion(hc);
        g.setColour(acc); g.fillRect(cx - R, hy - 42.f, R * 2.f, 14.f);
        g.restoreState();
        juce::Path tail; tail.startNewSubPath(cx + R - 8.f, hy - 38.f);
        tail.lineTo(cx + R + 24.f + std::sin(t * 5.f) * 6.f, hy - 48.f + std::sin(t * 4.f + 1.f) * 6.f);
        tail.lineTo(cx + R + 6.f, hy - 30.f); tail.closeSubPath();
        tail.startNewSubPath(cx + R - 8.f, hy - 34.f);
        tail.lineTo(cx + R + 18.f + std::sin(t * 5.f + 2.f) * 5.f, hy - 24.f + std::sin(t * 3.f) * 5.f);
        tail.lineTo(cx + R + 2.f, hy - 28.f); tail.closeSubPath();
        g.setColour(acc); g.fillPath(tail);
        // ojos
        for (int s = -1; s <= 1; s += 2)
        {
            const float ex = cx + (float) s * 21.f, ey = hy - 4.f;
            if (blink > 0.f) { g.setColour(Colour(0xff2a1a12)); g.drawLine(ex - 8.f, ey, ex + 8.f, ey, 3.f); }
            else
            {
                g.setColour(Colour(0xff1b1020)); g.fillEllipse(ex - 9.f, ey - 12.f, 18.f, 24.f);
                g.setColour(juce::Colours::white); g.fillEllipse(ex - 5.f, ey - 8.f, 7.f, 7.f); g.fillEllipse(ex + 1.f, ey + 2.f, 4.f, 4.f);
            }
            g.setColour(Colour(0xffff8fb0).withAlpha(0.7f)); g.fillEllipse(ex + (float) s * 8.f - 7.f, hy + 12.f, 14.f, 9.f);
        }
        drawMouth(g, cx, hy + 24.f);
        // microfono
        if (singing)
        {
            g.setColour(Colour(0xff555566)); g.drawLine(cx + 20.f, hy + 36.f, rx, ry, 5.f);
            g.setColour(Colour(0xffb0b0c0)); g.fillEllipse(cx + 12.f, hy + 26.f, 16.f, 16.f);
            g.setColour(Colour(0xff555566)); g.drawEllipse(cx + 12.f, hy + 26.f, 16.f, 16.f, 1.5f);
            arm(cx + 32.f, shy, rx, ry, 1.f);
        }
    }

    // notitas flotando
    static const juce::uint32 nc[] = { 0xffff6fae, 0xffb6ff3b, 0xffffd60a, 0xff4cc9f0 };
    for (auto& n : notes)
    {
        g.setColour(Colour(nc[n.col]).withAlpha(juce::jlimit(0.f, 1.f, n.life)));
        g.fillEllipse(n.x - 4.f, n.y - 3.f, 8.f, 6.f);
        g.drawLine(n.x + 3.5f, n.y - 1.f, n.x + 3.5f, n.y - 15.f, 1.8f);
        g.drawLine(n.x + 3.5f, n.y - 15.f, n.x + 9.f, n.y - 10.f, 1.8f);
    }
    g.setColour(Colour(0xff2a1a12)); g.drawRoundedRectangle(0.5f, 0.5f, w - 1.f, h - 1.f, 10.f, 2.f);
}

//==============================================================================  MODELOS DE LISTA
int CatModel::getNumRows() { return categoryNames().size(); }
void CatModel::selectedRowsChanged(int row) { if (row >= 0) ed.setCategory(row); }
void CatModel::paintListBoxItem(int row, juce::Graphics& g, int w, int h, bool sel)
{
    static const juce::uint32 cols[] = { 0xffff5fa2, 0xffb6ff3b, 0xff4cc9f0, 0xffffd60a, 0xff9b5cff, 0xffff8a3d };
    if (row < 0 || row >= categoryNames().size()) return;
    auto r = juce::Rectangle<float>(3.f, 1.f, (float) w - 14.f, (float) h - 2.f);
    g.setColour(Colour(cols[row % 6]).withAlpha(sel ? 1.f : 0.78f)); g.fillRoundedRectangle(r, 3.f);
    juce::Path tip; tip.addTriangle(r.getRight(), r.getY(), r.getRight(), r.getBottom(), r.getRight() + 8.f, r.getCentreY());
    g.setColour(Colour(0xfff0d8a8)); g.fillPath(tip);
    if (sel) { g.setColour(juce::Colours::white); g.drawRoundedRectangle(r, 3.f, 1.5f); }
    g.setColour(Colour(ub::ink)); g.setFont(ub::F(10.5f));
    g.drawText(categoryNames()[row], 10, 0, w - 24, h, juce::Justification::centredLeft);
}

int PresModel::getNumRows() { return ed.visNames.size(); }
void PresModel::selectedRowsChanged(int row) { if (row >= 0) ed.loadVisible(row); }
void PresModel::paintListBoxItem(int row, juce::Graphics& g, int w, int h, bool sel)
{
    if (row < 0 || row >= ed.visNames.size()) return;
    if (sel) { g.setColour(Colour(0xfff0b840).withAlpha(0.8f)); g.fillRoundedRectangle(26.f, 1.f, (float) w - 30.f, (float) h - 2.f, 3.f); }
    g.setColour(Colour(0x553a6ea5)); g.drawHorizontalLine(h - 1, 24.f, (float) w - 6.f);
    g.setColour(Colour(0xff6b5a50)); g.fillEllipse(8.f, (float) h * 0.5f - 3.f, 6.f, 6.f);
    g.setColour(Colour(ub::ink)); g.setFont(ub::F(13.f, false));
    g.drawText(ed.visNames[row], 32, 0, w - 40, h, juce::Justification::centredLeft);
}

//==============================================================================  EDITOR
void UnderBoxEditor::addRow(int t, int row, const char* title, std::initializer_list<int> ps)
{
    titles.push_back({ t, row, title });
    int col = 0;
    for (int p : ps)
    {
        auto c = std::make_unique<Ctl>(proc.apvts, p, t);
        c->setBounds(20 + 90 + col * 58, 66 + row * 64, 56, 60);
        c->setVisible(false);
        addChildComponent(*c);
        ctls.push_back(std::move(c));
        ++col;
    }
}

UnderBoxEditor::UnderBoxEditor(UnderBoxProcessor& p)
    : AudioProcessorEditor(&p), proc(p), pad(p), scopeV(p), stage(p),
      kb(p.keyState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setLookAndFeel(&look);
    setSize(900, 560);

    static const char* names[6] = { "PRESETS", "SONIDO", "MOTOR", "FX", "DIBUJAR", "NARU" };
    for (int i = 0; i < 6; ++i)
    {
        auto* b = tabBtns.add(new juce::TextButton(names[i]));
        b->setRadioGroupId(77); b->setClickingTogglesState(true);
        b->setColour(juce::TextButton::buttonColourId, Colour(0xffc79a3c));
        b->setColour(juce::TextButton::buttonOnColourId, Colour(0xfffff0a0));
        b->setBounds(20 + i * 96, 12, 90, 32);
        b->onClick = [this, i] { setTab(i); };
        addAndMakeVisible(b);
    }
    tabBtns[0]->setToggleState(true, juce::dontSendNotification);

    addRow(1, 0, "TIMBRE", { Cut, Res, FEnv, Uni, UniDet, Drive });
    addRow(1, 1, "FORMA", { AAtk, ARel, Glide, PEnv, LPitch, Vol });
    addRow(1, 2, "VOZ", { Vox, Vowel, Cantar, Chorus, DelMix, RevMix });
    addRow(2, 0, "OSC 1", { O1Wave, O1Oct, O1Fine, O1Lvl, Uni, UniDet, Pw });
    addRow(2, 1, "OSC 2", { O2Wave, O2Oct, O2Semi, O2Fine, O2Lvl, Sub, Noise, FM });
    addRow(2, 2, "FILTRO", { FType, Cut, Res, FEnv, FKey, Mono, Glide });
    addRow(2, 3, "ENVS", { AAtk, ADec, ASus, ARel, FAtk, FDec, FSus, FRel });
    addRow(2, 4, "PITCH / LFO", { PEnv, PTime, LRate, LShape, LPitch, LCut, LAmp });
    addRow(3, 0, "DISTORSION", { Drive, Crush, Rate });
    addRow(3, 1, "ESPACIO", { Chorus, DelMix, DelTime, DelFb, RevMix, RevSize });
    addRow(3, 2, "SALIDA", { Vol });
    addRow(5, 0, "NARU", { Play, Cantar, Scale, Root, Bpm, Dens, Vowel });

    // presets
    catList.setModel(&catModel); catList.setRowHeight(13);
    presList.setModel(&presModel); presList.setRowHeight(22);
    catList.setBounds(20, 62, 130, 334); presList.setBounds(158, 62, 430, 334);
    catList.setColour(juce::ListBox::backgroundColourId, Colour(0xff4a3328));
    addChildComponent(catList); addChildComponent(presList);

    // dibujar
    pad.setBounds(26, 70, 556, 250); addChildComponent(pad);
    static const char* dn[5] = { "Seno", "Sierra", "Cuadrada", "Azar", "Suavizar" };
    for (int i = 0; i < 5; ++i)
    {
        auto* b = drawBtns.add(new juce::TextButton(dn[i]));
        b->setBounds(26 + i * 100, 332, 92, 28);
        b->onClick = [this, i]
        {
            juce::Random r;
            std::array<float, 256> tmp;
            for (size_t k = 0; k < 256; ++k) tmp[k] = proc.draw[k].load();
            for (size_t k = 0; k < 256; ++k)
            {
                const float ph = (float) k / 256.f;
                if (i == 0) tmp[k] = std::sin(ph * juce::MathConstants<float>::twoPi);
                else if (i == 1) tmp[k] = 2.f * ph - 1.f;
                else if (i == 2) tmp[k] = ph < 0.5f ? 0.9f : -0.9f;
                else if (i == 3) tmp[k] = (k % 16 == 0) ? r.nextFloat() * 2.f - 1.f : tmp[k - (k % 16)];
                else tmp[k] = 0.25f * tmp[(k + 255) & 255] + 0.5f * tmp[k] + 0.25f * tmp[(k + 1) & 255];
            }
            for (size_t k = 0; k < 256; ++k) proc.draw[k].store(tmp[k]);
            pad.repaint(); pad.useDrawn();
        };
        addChildComponent(b);
    }

    // naru
    newMelBtn.setBounds(20 + 90, 66 + 70, 160, 30);
    newMelBtn.onClick = [this] { proc.melodySeed++; };
    addChildComponent(newMelBtn);

    // fila de abajo
    saveBtn.setBounds(20, 408, 112, 26); saveBtn.onClick = [this] { askSaveName(); }; addAndMakeVisible(saveBtn);
    folderBtn.setBounds(140, 408, 120, 26);
    folderBtn.setColour(juce::TextButton::buttonColourId, Colour(0xffb6ff3b));
    folderBtn.onClick = [] { UnderBoxProcessor::userDir().startAsProcess(); };
    addAndMakeVisible(folderBtn);
    skinBtn.setBounds(20, 518, 120, 26);
    skinBtn.setColour(juce::TextButton::buttonColourId, Colour(0xfff48fb1));
    skinBtn.onClick = [this] { proc.skin = (proc.skin.load() + 1) % stage.getSkinCount(); };
    addAndMakeVisible(skinBtn);
    scopeV.setBounds(20, 442, 120, 70); addAndMakeVisible(scopeV);
    stage.setBounds(604, 54, 282, 346); addAndMakeVisible(stage);

    playBottom = std::make_unique<Ctl>(proc.apvts, Play, -1);
    cantarBottom = std::make_unique<Ctl>(proc.apvts, Cantar, -1);
    playBottom->setBounds(610, 408, 130, 26); cantarBottom->setBounds(750, 408, 130, 26);
    addAndMakeVisible(*playBottom); addAndMakeVisible(*cantarBottom);

    kb.setBounds(150, 440, 736, 104);
    kb.setAvailableRange(36, 96);
    kb.setLowestVisibleKey(36);
    kb.setKeyWidth(736.f / 36.f);
    kb.setWantsKeyboardFocus(false);
    kb.setColour(juce::MidiKeyboardComponent::whiteNoteColourId, Colour(0xfff8f2e2));
    kb.setColour(juce::MidiKeyboardComponent::blackNoteColourId, Colour(0xff2a2230));
    kb.setColour(juce::MidiKeyboardComponent::keySeparatorLineColourId, Colour(0xff3a2b22));
    kb.setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId, Colour(0xffff7a3d));
    kb.setColour(juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, Colour(0x33ffd60a));
    addAndMakeVisible(kb);

    catList.selectRow(0);
    setTab(0);
}

UnderBoxEditor::~UnderBoxEditor() { setLookAndFeel(nullptr); }

void UnderBoxEditor::setTab(int t)
{
    tab = t;
    for (auto& c : ctls) c->setVisible(c->tab == t);
    catList.setVisible(t == 0); presList.setVisible(t == 0);
    pad.setVisible(t == 4);
    for (auto* b : drawBtns) b->setVisible(t == 4);
    newMelBtn.setVisible(t == 5);
    repaint();
}

void UnderBoxEditor::setCategory(int c)
{
    curCat = c;
    visNames.clear(); visIdx.clear(); userFiles.clear();
    if (c < categoryNames().size() - 1)
    {
        const auto& all = factoryPresets();
        for (int i = 0; i < (int) all.size(); ++i)
            if (all[(size_t) i].cat == c) { visNames.add(all[(size_t) i].name); visIdx.push_back(i); }
    }
    else
    {
        userFiles = UnderBoxProcessor::userDir().findChildFiles(juce::File::findFiles, false, "*.ubp");
        for (auto& f : userFiles) visNames.add(f.getFileNameWithoutExtension());
    }
    presList.updateContent();
    presList.deselectAllRows();
    presList.scrollToEnsureRowIsOnscreen(0);
}

void UnderBoxEditor::loadVisible(int row)
{
    if (row < 0 || row >= visNames.size()) return;
    if (curCat < categoryNames().size() - 1) proc.applyPreset(factoryPresets()[(size_t) visIdx[(size_t) row]]);
    else proc.loadUserPreset(userFiles[row]);
}

void UnderBoxEditor::askSaveName()
{
    auto* w = new juce::AlertWindow("Guardar preset", "Ponele nombre a tu sonido:", juce::MessageBoxIconType::NoIcon);
    w->addTextEditor("n", proc.currentPresetName);
    w->addButton("Guardar", 1, juce::KeyPress(juce::KeyPress::returnKey));
    w->addButton("Cancelar", 0, juce::KeyPress(juce::KeyPress::escapeKey));
    juce::Component::SafePointer<UnderBoxEditor> safe(this);
    w->enterModalState(true, juce::ModalCallbackFunction::create([safe, w](int r)
    {
        if (r != 1 || safe == nullptr) return;
        auto n = w->getTextEditorContents("n").trim();
        if (n.isEmpty()) return;
        safe->proc.saveUserPreset(n);
        safe->catList.selectRow(categoryNames().size() - 1);
        safe->setCategory(categoryNames().size() - 1);
        safe->catList.repaint();
    }), true);
}

void UnderBoxEditor::paint(juce::Graphics& g)
{
    g.setGradientFill(juce::ColourGradient(Colour(0xff5a3b2e), 0.f, 0.f, Colour(0xff2a1c17), 0.f, (float) getHeight(), false));
    g.fillAll();
    g.setColour(Colour(0x10ffffff));
    for (int y = 0; y < getHeight(); y += 6) g.drawHorizontalLine(y, 0.f, (float) getWidth());

    juce::Rectangle<float> lp(14.f, 54.f, 580.f, 346.f);
    g.setColour(Colour(0xff1d1a3a)); g.fillRoundedRectangle(lp.expanded(4.f), 12.f);
    g.setColour(Colour(ub::paper)); g.fillRoundedRectangle(lp, 8.f);
    g.setColour(Colour(0x2290632f));
    for (float y = lp.getY() + 24.f; y < lp.getBottom() - 4.f; y += 22.f) g.drawHorizontalLine((int) y, lp.getX() + 6.f, lp.getRight() - 6.f);
    g.setColour(Colour(0xff6b5a50));
    for (float x = lp.getX() + 10.f; x < lp.getRight() - 6.f; x += 18.f) { g.fillEllipse(x, lp.getY() + 3.f, 4.f, 4.f); g.fillEllipse(x, lp.getBottom() - 8.f, 4.f, 4.f); }

    g.setColour(Colour(ub::ink)); g.setFont(ub::F(12.f));
    for (auto& t : titles)
        if (t.tab == tab) g.drawText(t.text, 24, 66 + t.row * 64 + 14, 86, 16, juce::Justification::centredLeft);

    g.setFont(ub::F(12.f, false));
    if (tab == 4)
    {
        g.drawText("Dibuja tu onda con el mouse. Queda como Onda 1 (\"Dibujo\") y se guarda en el preset.", 26, 366, 556, 20, juce::Justification::centredLeft);
    }
    if (tab == 5)
    {
        g.drawText("TOCA: el personaje toca una melodia solo (sigue el BPM del DAW si esta en play).", 24, 190, 560, 18, juce::Justification::centredLeft);
        g.drawText("CANTA: cada nota es una silaba (a-i-a-o-e...) con la voz del motor Vox.", 24, 210, 560, 18, juce::Justification::centredLeft);
        g.drawText("NUEVA MELODIA genera otra. Escala, nota base, BPM y densidad cambian como suena.", 24, 230, 560, 18, juce::Justification::centredLeft);
        g.drawText("Skin propio: guarda tu dibujo como personaje.png en Documentos/UnderBox y elegilo con SKIN.", 24, 250, 560, 18, juce::Justification::centredLeft);
    }
    g.setColour(Colour(0xffe8d9b0)); g.setFont(ub::F(11.f, false));
    g.drawText(juce::String((int) factoryPresets().size()) + " presets de fabrica", 268, 408, 330, 26, juce::Justification::centredLeft);
}
