#include "CInput.hpp"
#include "../theme/CThemeDefaults.hpp"
#include "../render/commands/CDrawRectCommand.hpp"

#include <algorithm>

namespace Hyprlooks {

    CInput::CInput() {
        m_style = CThemeDefaults::inputStyle();
        updateDisplay();
    }

    void CInput::setValue(const std::string& v) {
        m_value = v;
        updateDisplay();
        markDirty();
    }

    const std::string& CInput::value() const {
        return m_value;
    }

    void CInput::setPlaceholder(const std::string& p) {
        m_placeholder = p;
        updateDisplay();
        markDirty();
    }

    void CInput::setInputWidth(float w) {
        m_inputWidth = w;
        markDirty();
    }

    void CInput::setEditing(bool editing) {
        if (m_editing == editing)
            return;
        m_editing = editing;
        updateDisplay();
        markDirty();
    }

    void CInput::onChange(ChangeCallback cb) {
        m_onChange = std::move(cb);
    }

    void CInput::onSubmit(ChangeCallback cb) {
        m_onSubmit = std::move(cb);
    }

    void CInput::insertText(const std::string& utf8) {
        m_value += utf8;
        updateDisplay();
        if (m_onChange)
            m_onChange(m_value);
        markDirty();
    }

    void CInput::backspace() {
        if (m_value.empty())
            return;

        // erase the last UTF-8 codepoint (skip continuation bytes 10xxxxxx)
        size_t i = m_value.size();
        do {
            --i;
        } while (i > 0 && (static_cast<unsigned char>(m_value[i]) & 0xC0) == 0x80);
        m_value.erase(i);

        updateDisplay();
        if (m_onChange)
            m_onChange(m_value);
        markDirty();
    }

    void CInput::submit() {
        if (m_onSubmit)
            m_onSubmit(m_value);
    }

    eWidgetType CInput::type() const {
        return eWidgetType::INPUT;
    }

    const char* CInput::typeName() const {
        return "CInput";
    }

    bool CInput::interactive() const {
        return true;
    }

    Hyprutils::Math::Vector2D CInput::measure() const {
        const double h = m_style.fontSize * 1.6 + m_style.padding.y * 2.0;
        return {m_inputWidth, h};
    }

    void CInput::updateDisplay() {
        // while editing, show the raw buffer (empty is fine, caret sits at start);
        // otherwise fall back to the placeholder when the buffer is empty.
        const std::string disp = m_editing ? m_value : (m_value.empty() ? m_placeholder : m_value);
        setText(disp); // CLabel::setText invalidates the text cache as needed
    }

    IWidget::RenderCommandList CInput::buildRenderCommands(float monitorScale) {
        RenderCommandList commands;

        const auto       fieldBox = m_geometry.box;
        const CHyprColor bg       = (m_editing && m_style.hoverBgColor.a > 0.F) ? m_style.hoverBgColor : m_style.bgColor;

        if (bg.a > 0.F) {
            commands.push_back(makeUnique<CDrawRectCommand>(CDrawRectCommand::SParams{
                .box           = fieldBox,
                .color         = bg,
                .round         = sc<int>(m_style.borderRadius.topLeft),
                .roundingPower = m_style.roundingPower,
            }));
        }

        // render the text inset by padding and vertically centred. CLabel draws
        // at m_geometry.box, so temporarily point it at the inset box.
        ensureCache(monitorScale);

        Hyprutils::Math::CBox textBox = fieldBox;
        textBox.x                     = fieldBox.x + m_style.padding.x;
        textBox.y                     = fieldBox.y + (fieldBox.h - m_textSize.y) / 2.0;

        const auto savedBox = m_geometry.box;
        m_geometry.box      = textBox;
        auto textCmds       = CLabel::buildRenderCommands(monitorScale);
        m_geometry.box      = savedBox;
        for (auto& cmd : textCmds)
            commands.push_back(std::move(cmd));

        // caret at the end of the buffer while focused
        if (m_editing) {
            const double caretH = std::max(sc<double>(m_style.fontSize), m_textSize.y);
            const double caretX = textBox.x + m_textSize.x + 1.0;
            const double caretY = fieldBox.y + (fieldBox.h - caretH) / 2.0;
            commands.push_back(makeUnique<CDrawRectCommand>(CDrawRectCommand::SParams{
                .box   = {caretX, caretY, 2.0, caretH},
                .color = m_style.fgColor,
            }));
        }

        return commands;
    }

}
