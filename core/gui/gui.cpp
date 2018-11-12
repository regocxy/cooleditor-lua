#include "gui.h"
#include <vector>
#include <iostream>
namespace gui
{
    void TextWrapped(const char *str)
    {
        bool need_wrap = (GImGui->CurrentWindow->DC.TextWrapPos < 0.0f);    // Keep existing wrap position is one ia already set
        if (need_wrap) PushTextWrapPos(0.0f);
        TextUnformatted(str);
        if (need_wrap) PopTextWrapPos();
    }
}
