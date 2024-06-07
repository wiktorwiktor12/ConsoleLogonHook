#pragma once

namespace DirectUI {

    enum CheckedStateFlags {
        CheckedStateFlags_NONE,
        CheckedStateFlags_CHECKED
    };

    struct UILIB_API TouchCheckBox : public TouchButton {
        CheckedStateFlags GetCheckedState();
        HRESULT SetCheckedState(CheckedStateFlags state);

        bool GetToggleOnClick();
        HRESULT SetToggleOnClick(bool toggle);

    };
}
