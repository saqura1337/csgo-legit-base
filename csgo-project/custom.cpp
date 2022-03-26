#include "custom.h"

using namespace ImGui;

extern ImFont* SegoeUI_14px;

bool ImGui::tab(const char* label, bool selected) {
    auto window = ImGui::GetCurrentWindow();

    PushFont(SegoeUI_14px);
    auto label_size = CalcTextSize(label);
    PopFont();
    auto id = GetID(label);
    ImVec2 size(89, 11 + label_size.y);
    ImRect total_bb(window->DC.CursorPos, { window->DC.CursorPos.x + size.x, window->DC.CursorPos.y + size.y });

    ItemSize(size);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);

    if (selected)
        window->DrawList->AddRectFilled({ total_bb.Min.x, total_bb.Max.y - 2 }, total_bb.Max, ImColor(155, 158, 255));
    //window->DrawList->AddRectFilledMultiColor({ total_bb.Min.x, total_bb.Max.y - 2 }, total_bb.Max, ImColor(155, 158, 255), ImColor(105, 115, 151), ImColor(105, 115, 151), ImColor(155, 158, 255));

    PushFont(SegoeUI_14px);
    RenderText({ total_bb.Min.x + size.x / 2 - label_size.x / 2, total_bb.Min.y }, label);
    PopFont();

    return pressed;
}

bool ImGui::subtab(const char* label, bool selected) {
    auto window = ImGui::GetCurrentWindow();

    PushFont(SegoeUI_14px);
    auto label_size = CalcTextSize(label);
    PopFont();
    auto id = GetID(label);
    ImVec2 size(10 + label_size.x, 24);
    ImRect total_bb(window->DC.CursorPos, { window->DC.CursorPos.x + size.x, window->DC.CursorPos.y + size.y });

    ItemSize(size);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);

    if (selected)
        window->DrawList->AddRectFilled(total_bb.Min, { total_bb.Min.x + 2, total_bb.Max.y }, ImColor(155, 158, 255));
    //window->DrawList->AddRectFilledMultiColor(total_bb.Min, { total_bb.Min.x + 2, total_bb.Max.y }, ImColor(155, 158, 255), ImColor(155, 158, 255), ImColor(105, 115, 151), ImColor(105, 115, 151));

    PushFont(SegoeUI_14px);
    RenderText({ total_bb.Min.x + 10, total_bb.Min.y + size.y / 2 - label_size.y / 2 }, label);
    PopFont();

    return pressed;
}

void ImGui::beginchild(const char* label, ImVec2 size)
{
    BeginChild(label, size);
    SetCursorPos(GImGui->Style.WindowPadding);
    BeginChild(label, { size.x - GImGui->Style.WindowPadding.x, size.y - GImGui->Style.WindowPadding.y }, false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
}

void ImGui::endchild() {
    EndChild();
    EndChild();
}
