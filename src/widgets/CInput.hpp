#pragma once

#include "CLabel.hpp"

#include <functional>
#include <string>

namespace Hyprlooks {

    // A single-line, focusable text field. Reuses CLabel's cached text texture
    // for rendering and adds an editable buffer, a caret, and focus styling.
    //
    // v1 scope: caret is fixed at the end (append + backspace, no mid-string
    // editing or arrow keys). Layout follows the system default xkb layout via
    // CKeyboardTranslator, not per-window Hyprland layout switches.
    class CInput : public CLabel {
      public:
        using ChangeCallback = std::function<void(const std::string&)>;

        CInput();
        ~CInput() override = default;

        void                              setValue(const std::string& v);
        const std::string&                value() const;
        void                              setPlaceholder(const std::string& p);
        void                              setInputWidth(float w);
        void                              setEditing(bool editing);
        void                              onChange(ChangeCallback cb);
        void                              onSubmit(ChangeCallback cb);

        // editing operations, driven by the input manager on key events
        void                              insertText(const std::string& utf8);
        void                              backspace();
        void                              submit();

        virtual eWidgetType               type() const override;
        virtual const char*               typeName() const override;
        virtual bool                      interactive() const override;
        virtual Hyprutils::Math::Vector2D measure() const override;
        virtual RenderCommandList         buildRenderCommands(float monitorScale) override;

      private:
        void           updateDisplay();

        std::string    m_value;
        std::string    m_placeholder;
        float          m_inputWidth = 220.F;
        bool           m_editing    = false;
        ChangeCallback m_onChange;
        ChangeCallback m_onSubmit;
    };

}
