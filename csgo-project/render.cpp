#include "render.hpp"

#include <mutex>

#include "features/visuals.hpp"
#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "helpers/math.hpp"

#include "bytes.h"

ImFont* SegoeUI_14px;
ImFont* SegoeUI_32px;
ImFont* SegoeUI_8px;
ImFont* g_pDefaultFont;
ImFont* g_VeloFont;
ImFont* g_SpectatorListFont;
ImFont* lucidaconsole = nullptr;
ImFont* g_Calibri;
ImFont* g_pWeaponsFont = nullptr;

ImDrawListSharedData _data;

std::mutex render_mutex;

void render::Initialize()
{
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(InputSys::Get().GetMainWindow());
	ImGui_ImplDX9_Init(g_D3DDevice9);

	draw_list = new ImDrawList(ImGui::GetDrawListSharedData());
	draw_list_act = new ImDrawList(ImGui::GetDrawListSharedData());
	draw_list_rendering = new ImDrawList(ImGui::GetDrawListSharedData());

	GetFonts();
}

void render::GetFonts()
{
	// menu font
	//ImGui::GetIO().Fonts->AddFontDefault(NULL);

	ImFontConfig font_config;
	font_config.OversampleH = 1; //or 2 is the same
	font_config.OversampleV = 1;
	font_config.PixelSnapH = 1;

	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x044F, // Cyrillic
		0,
	};
	ImGuiIO& io = ImGui::GetIO();
	g_SpectatorListFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\verdana.ttf", 14.0f, &font_config, ranges);

	// menu-font
	SegoeUI_14px = io.Fonts->AddFontFromMemoryTTF((void*)segoe, sizeof(segoe), 16.f, NULL, io.Fonts->GetGlyphRangesJapanese());
	SegoeUI_32px = io.Fonts->AddFontFromMemoryTTF((void*)segoe, sizeof(segoe), 30.f, NULL, io.Fonts->GetGlyphRangesJapanese());
	SegoeUI_8px = io.Fonts->AddFontFromMemoryTTF((void*)segoe, sizeof(segoe), 15.f, NULL, io.Fonts->GetGlyphRangesJapanese());

	// esp-font
	g_pDefaultFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(u8"C:\\Windows\\Fonts\\calibri.ttf", 25.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	// velocity-font
	g_VeloFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(u8"C:\\Windows\\Fonts\\verdanab.ttf", 30.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	// lucida-console
	lucidaconsole = io.Fonts->AddFontFromMemoryTTF((void*)lucidacon, sizeof(lucidacon), 16, &font_config, ranges);

	// calibri-font
	g_Calibri = ImGui::GetIO().Fonts->AddFontFromFileTTF(u8"C:\\Windows\\Fonts\\calibri.ttf", 16, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	// icons-font
	g_pWeaponsFont = io.Fonts->AddFontFromMemoryTTF((void*)iconfont, sizeof(iconfont), 16, NULL, io.Fonts->GetGlyphRangesCyrillic());
}

void render::ClearDrawList() {
	render_mutex.lock();
	draw_list_act->Clear();
	render_mutex.unlock();
}

void render::BeginScene() {
	draw_list->Clear();
	draw_list->PushClipRectFullScreen();


	if (g_EngineClient->IsInGame() && g_LocalPlayer)
		Visuals::Get().AddToDrawList();

	render_mutex.lock();
	*draw_list_act = *draw_list;
	render_mutex.unlock();
}

ImDrawList* render::RenderScene() {
	if (render_mutex.try_lock()) {
		*draw_list_rendering = *draw_list_act;
		render_mutex.unlock();
	}

	return draw_list_rendering;
}

float render::RenderText(const std::string& text, ImVec2 pos, float size, Color color, bool center, bool outline, ImFont* pFont, bool dropshadow)
{
	ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());
	if (!pFont->ContainerAtlas) return 0.f;
	draw_list->PushTextureID(pFont->ContainerAtlas->TexID);

	if (center)
		pos.x -= textSize.x / 2.0f;

	if (outline) {
		draw_list->AddText(pFont, size, ImVec2(pos.x + 1, pos.y + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x - 1, pos.y - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x + 1, pos.y - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2(pos.x - 1, pos.y + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
	}

	if (dropshadow)
		draw_list->AddText(pFont, size, ImVec2(pos.x + 1, pos.y + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());

	draw_list->AddText(pFont, size, pos, GetU32(color), text.c_str());

	draw_list->PopTextureID();

	return pos.y + textSize.y;
}

void render::RenderCircle3D(Vector position, float points, float radius, Color color)
{
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

		Vector start2d, end2d;
		if (g_DebugOverlay->ScreenPosition(start, start2d) || g_DebugOverlay->ScreenPosition(end, end2d))
			return;

		RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color);
	}
}