#pragma once

#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "embedded/icons_font_awesome.h"
#include "imgui/imgui_internal.h"
#include "imgui_user/imgui_tabs.h"
#include "imgui_user/imgui_date_picker.h"
#include "imgui_user/imgui_user.h"
#include "imguizmo/imguizmo.h"
#include <memory>
namespace gui
{
    using namespace ImGui;
    void TextWrapped(const char *str);
    enum ImGuiDragDropFlagsEx {
        ImGuiDragDropFlags_AcceptNoUnderLine = 1 << 12
    };
}


