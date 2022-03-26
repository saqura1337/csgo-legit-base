#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace ImGui {
    bool tab(const char* label, bool selected);
    bool subtab(const char* label, bool selected);
    void beginchild(const char* name, ImVec2 size);
    void endchild();
}
