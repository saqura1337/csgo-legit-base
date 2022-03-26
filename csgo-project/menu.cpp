#include "Menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <chrono>

#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "options.hpp"
#include "ui.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"
#include "features/item_definitions.h"
#include "features/kit_parser.h"
#include "features/skins.h"
#include "render.hpp"
#include "custom.h"
void ReadDirectory(const std::string& name, std::vector<std::string>& v)
{
	auto pattern(name);
	pattern.append("\\*.cfg");
	WIN32_FIND_DATAA data;
	HANDLE hFind;
	if ((hFind = FindFirstFileA(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			v.emplace_back(data.cFileName);
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}
}
struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};

namespace ImGuiEx
{
	inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
	{
		float clr[4] = {
			v->r() / 255.0f,
			v->g() / 255.0f,
			v->b() / 255.0f,
			v->a() / 255.0f
		};
		//clr[3]=255;
		if (ImGui::ColorEdit4(label, clr, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar)) {
			v->SetColor(clr[0], clr[1], clr[2], clr[3]);
			return true;
		}
		return false;
	}
	inline bool ColorEdit4a(const char* label, Color* v, bool show_alpha = true)
	{
		float clr[4] = {
			v->r() / 255.0f,
			v->g() / 255.0f,
			v->b() / 255.0f,
			v->a() / 255.0f
		};
		//clr[3]=255;
		if (ImGui::ColorEdit4(label, clr, show_alpha | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar)) {
			v->SetColor(clr[0], clr[1], clr[2], clr[3]);
			return true;
		}
		return false;
	}

	inline bool ColorEdit3(const char* label, Color* v)
	{
		return ColorEdit4(label, v, false);
	}
}

void Menu::Initialize()
{
	CreateStyle();

	_visible = true;
}

void Menu::Shutdown()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Menu::OnDeviceLost()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

std::string comp_name() {
	char buff[MAX_PATH];
	GetEnvironmentVariableA("USERNAME", buff, MAX_PATH);

	return std::string(buff);
}

void Menu::Watermark()
{
	if (!g_Options.misc_watermark)
		return;

	std::string Cheatname = ("Cheat name");

	auto watermark = Cheatname + (" | ") + comp_name() + (" | ");

	if (g_EngineClient->IsInGame())
	{
		auto nci = g_EngineClient->GetNetChannelInfo();

		auto net_channel = g_EngineClient->GetNetChannelInfo();

		auto local_player = reinterpret_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));
		std::string outgoing = local_player ? std::to_string((int)(net_channel->GetLatency(FLOW_OUTGOING) * 1000)) : "0";

		if (nci)
		{
			auto server = nci->GetAddress();

			if (!strcmp(server, ("loopback")))
				server = ("local server");
			else
				server = ("valve server");

			auto tickrate = std::to_string((int)(1.0f / g_GlobalVars->interval_per_tick));

			watermark = Cheatname + (" | ") + comp_name() + (" | ") + server + (" | delay: ") + outgoing.c_str() + (" ms | ") + tickrate + (" tick | ");
		}
	}
	ImVec2 p, s;
	ImGui::PushFont(g_SpectatorListFont);
	auto size_text = ImGui::CalcTextSize(watermark.c_str());
	ImGui::PopFont();
	ImGui::SetNextWindowSize(ImVec2(size_text.x + (Menu::Get().IsVisible() ? 24 : 14), 20));

	ImGui::Begin("watermark", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_::ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_::ImGuiWindowFlags_NoNav);
	{
		auto d = ImGui::GetWindowDrawList();
		p = ImGui::GetWindowPos();
		s = ImGui::GetWindowSize();
		ImGui::PushFont(g_SpectatorListFont);
		ImGui::SetWindowSize(ImVec2(s.x, 21 + 18));
		//
		d->AddRectFilled(p, p + ImVec2(s.x, 21), ImColor(39, 39, 39, int(50 * 1)));
		auto main_colf = ImColor(39, 39, 39, int(255 * 1));
		auto main_coll = ImColor(39, 39, 39, int(140 * 1));
		d->AddRectFilledMultiColor(p, p + ImVec2(s.x / 2, 20), main_coll, main_colf, main_colf, main_coll);
		d->AddRectFilledMultiColor(p + ImVec2(s.x / 2, 0), p + ImVec2(s.x, 20), main_colf, main_coll, main_coll, main_colf);
		//
		auto main_colf2 = ImColor(39, 39, 39, 255);
		d->AddRectFilledMultiColor(p, p + ImVec2(s.x / 2, 20), main_coll, main_colf2, main_colf2, main_coll);
		d->AddRectFilledMultiColor(p + ImVec2(s.x / 2, 0), p + ImVec2(s.x, 20), main_colf2, main_coll, main_coll, main_colf2);
		auto line_colf = ImColor(126, 131, 219, 200);
		auto line_coll = ImColor(126, 131, 219, 255);
		d->AddRectFilledMultiColor(p, p + ImVec2(s.x / 2, 2), line_coll, line_colf, line_colf, line_coll);
		d->AddRectFilledMultiColor(p + ImVec2(s.x / 2, 0), p + ImVec2(s.x, 2), line_colf, line_coll, line_coll, line_colf);
		d->AddText(p + ImVec2((Menu::Get().IsVisible() ? s.x - 10 : s.x) / 2 - size_text.x / 2, (20) / 2 - size_text.y / 2), ImColor(250, 250, 250, 255), watermark.c_str());
	}
	ImGui::End();
}

void Menu::SpectatorList()
{
	if (!g_Options.spectator_list)
		return;

	std::string spectators;

	if (g_EngineClient->IsInGame() && g_LocalPlayer)
	{
		for (int i = 1; i <= g_GlobalVars->maxClients; i++)
		{
			auto ent = C_BasePlayer::GetPlayerByIndex(i);

			if (!ent || ent->IsAlive() || ent->IsDormant())
				continue;

			auto target = (C_BasePlayer*)ent->m_hObserverTarget();

			if (!target || target != g_LocalPlayer)
				continue;

			if (ent == target)
				continue;

			auto info = ent->GetPlayerInfo();

			spectators += std::string(info.szName) + u8"\n";
		}
	}

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(65, 8));
	if (ImGui::Begin("spectators:", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | (_visible ? NULL : ImGuiWindowFlags_NoMove)))
	{
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::PushFont(g_SpectatorListFont);

		//ImGui::Text("spectators:");

		ImGui::Text(spectators.c_str());
		ImGui::PopFont();
	}
	ImGui::End();
}

void Menu::Render()
{
	ImGui::GetIO().MouseDrawCursor = _visible;
	SpectatorList();
	if (!_visible)
		return;

	ImGui::SetNextWindowSize({ 680, 330 }); /*menu size*/

	static float f = 0.0f;
	static int counter = 0;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("csgo-sdk", 0, ImGuiWindowFlags_NoDecoration);
	{
		ImGui::PopStyleVar();
		ImVec2 p = ImGui::GetWindowPos();
		ImDrawList* draw = ImGui::GetWindowDrawList();

		draw->AddRectFilled({ p.x + 131, p.y + 40 }, { p.x + ImGui::GetCurrentWindow()->Size.x, p.y + ImGui::GetCurrentWindow()->Size.y - 19 }, ImColor(36, 36, 36));
		draw->AddCircleFilled({ p.x + 23, p.y + 20 }, 3.5f, ImColor(155, 158, 255));
		ImGui::PushFont(SegoeUI_14px);
		draw->AddText({ p.x + 33, p.y + 20 - ImGui::CalcTextSize("Cheat name").y / 2 - 1 }, ImGui::GetColorU32(ImGuiCol_Text), "Cheat name");
		ImGui::PopFont();

		static int tab = 0;
		static int subtab = 0;

		ImGui::SetCursorPos({ 249, 13 });
		ImGui::BeginGroup();
		{
			if (ImGui::tab("Legitbot", tab == 0)) { tab = 0; subtab = 0; } ImGui::SameLine();
			if (ImGui::tab("Visuals", tab == 1)) { tab = 1; subtab = 1; } ImGui::SameLine();
			if (ImGui::tab("Misc", tab == 2)) { tab = 2; subtab = 3; } ImGui::SameLine();
			if (ImGui::tab("Skins", tab == 3)) { tab = 3; subtab = 5; }
		}
		ImGui::EndGroup();

		if (tab == 0)
		{
			static int definition_index = WEAPON_INVALID;

			auto localPlayer = C_BasePlayer::GetPlayerByIndex(g_EngineClient->GetLocalPlayer());
			if (g_EngineClient->IsInGame() && localPlayer && localPlayer->IsAlive() && localPlayer->m_hActiveWeapon() && localPlayer->m_hActiveWeapon()->IsGun())
				definition_index = localPlayer->m_hActiveWeapon()->m_Item().m_iItemDefinitionIndex();
			else
				definition_index = WEAPON_INVALID;
			if (definition_index == WEAPON_INVALID)definition_index = WEAPON_DEAGLE;

			ImGui::SetCursorPos({ 0, 60 });
			ImGui::BeginGroup();
			{
				if (ImGui::subtab("General", subtab == 0)) subtab = 0;
			}
			ImGui::EndGroup();

			if (subtab == 0)
			{
				ImGui::SetCursorPos({ 147, 52 });
				ImGui::beginchild("General", { 255, 245 });
				{
					auto settings = &g_Options.weapons[definition_index].legit;

					ImGui::Checkbox("Enabled", &settings->enabled);

					ImGui::Combo("Silent type", &settings->silent2, "Off\0Silent \0Perfect silent\0");

					if (settings->silent2)
						ImGui::SliderFloat("Silent FOV", &settings->silent_fov, 0.f, 20.f, "%.f");

					ImGui::SliderFloat("Field of view", &settings->fov, 0.f, 20.f, "%.f");
					ImGui::SliderFloat("Smooth", &settings->smooth, 1.f, 20.f, "%.f");
					ImGui::Checkbox("Flash check", &settings->flash_check);
					ImGui::Checkbox("Smoke check", &settings->smoke_check);
					ImGui::Checkbox("Auto-pistol", &settings->autopistol);

					if (ImGui::BeginCombo("Hitboxes", "...", ImGuiComboFlags_NoArrowButton))
					{
						ImGui::Selectable("Head", &settings->hitboxes.head, ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable("Chest", &settings->hitboxes.chest, ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable("Hands", &settings->hitboxes.hands, ImGuiSelectableFlags_DontClosePopups);
						ImGui::Selectable("Legs", &settings->hitboxes.legs, ImGuiSelectableFlags_DontClosePopups);
						ImGui::EndCombo();
					}

					if (!settings->silent2)
						ImGui::SliderInt("Shot delay", &settings->shot_delay, 0, 1000, "%i");

					ImGui::SliderInt("Kill delay", &settings->kill_delay, 0, 1000, " % i");
				}
				ImGui::endchild();

				ImGui::SetCursorPos({ 417, 52 });
				ImGui::beginchild("Weapons", { 255, 245 });
				{
					auto settings = &g_Options.weapons[definition_index].legit;

					ImGui::Checkbox("Enable RCS", &settings->rcs.enabled);

					const char* rcs_types[] = { "Standalone", "Aim" };
					if (ImGui::BeginCombo("RCS type", rcs_types[settings->rcs.type], ImGuiComboFlags_NoArrowButton))
					{
						for (int i = 0; i < IM_ARRAYSIZE(rcs_types); i++)
						{
							if (ImGui::Selectable(rcs_types[i], i == settings->rcs.type))
								settings->rcs.type = i;
						}

						ImGui::EndCombo();
					};
					ImGui::SliderInt("RCS x", &settings->rcs.x, 0, 100, "%i");
					ImGui::SliderInt("RCS y", &settings->rcs.y, 0, 100, "%i");
				}
				ImGui::endchild();
			}
		}
		if (tab == 1)
		{
			ImGui::SetCursorPos({ 0, 60 });
			ImGui::BeginGroup();
			{
				if (ImGui::subtab("General", subtab == 1)) subtab = 1;
				if (ImGui::subtab("World", subtab == 2)) subtab = 2;
			}
			ImGui::EndGroup();

			if (subtab == 1)
			{
				ImGui::SetCursorPos({ 147, 52 });
				ImGui::beginchild("General", { 255, 245 });
				{
					ImGui::Checkbox("Enable boxes", &g_Options.esp_player_boxes); ImGui::SameLine(); ImGuiEx::ColorEdit4("Enemies Visible   ", &g_Options.color_esp_enemy_visible);
					ImGui::Checkbox("Enable occluded boxes", &g_Options.esp_player_boxesOccluded); ImGui::SameLine(); ImGuiEx::ColorEdit4("Enemies Occluded      ", &g_Options.color_esp_enemy_occluded);
					ImGui::Checkbox("Enemy name", &g_Options.esp_player_names);
					ImGui::Checkbox("Enemy health", &g_Options.esp_player_health); ImGui::SameLine(); ImGuiEx::ColorEdit4("Health color  ", &g_Options.esp_player_health_color);
					ImGui::Checkbox("Enemy weapon", &g_Options.esp_player_weapons); ImGui::SameLine(); ImGuiEx::ColorEdit4("Enemy weapon color  ", &g_Options.esp_player_weapons_color);

					const char* enemy_weapon_types[] = { "Text", "Icon" };
					if (g_Options.esp_player_weapons)
						if (ImGui::BeginCombo("Enemy weapon type", enemy_weapon_types[g_Options.esp_player_weapons_type], ImGuiComboFlags_NoArrowButton))
						{
							for (int i = 0; i < IM_ARRAYSIZE(enemy_weapon_types); i++)
							{
								if (ImGui::Selectable(enemy_weapon_types[i], i == g_Options.esp_player_weapons_type))
									g_Options.esp_player_weapons_type = i;
							}
							ImGui::EndCombo();
						}

					ImGui::Checkbox("Enemy flags", &g_Options.esp_player_flags);
					if (g_Options.esp_player_flags) {
						if (ImGui::BeginCombo("Flags", "..."))
						{
							ImGui::Selectable(("Money"), &g_Options.esp_flags_money, ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(("Defusing"), &g_Options.esp_flags_defusing, ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(("Defuse kit"), &g_Options.esp_flags_kit, ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(("Armor"), &g_Options.esp_flags_armor, ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(("Scoped"), &g_Options.esp_flags_scoped, ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(("Hostage carrier"), &g_Options.esp_flags_hostage, ImGuiSelectableFlags_DontClosePopups);
							ImGui::Selectable(("Bomb carrier"), &g_Options.esp_flags_c4, ImGuiSelectableFlags_DontClosePopups);
							ImGui::EndCombo();
						}
					}
					ImGui::Checkbox("Dropped weapons", &g_Options.esp_dropped_weapons); ImGui::SameLine(); ImGuiEx::ColorEdit4("Dropped weapons color  ", &g_Options.esp_dropped_weapons_color);

					const char* dropped_weapon_types[] = { "Text", "Icon" };
					if (g_Options.esp_dropped_weapons)
						if (ImGui::BeginCombo("Dropped weapons type", dropped_weapon_types[g_Options.esp_dropped_weapons_type], ImGuiComboFlags_NoArrowButton))
						{
							for (int i = 0; i < IM_ARRAYSIZE(dropped_weapon_types); i++)
							{
								if (ImGui::Selectable(dropped_weapon_types[i], i == g_Options.esp_dropped_weapons_type))
									g_Options.esp_dropped_weapons_type = i;
							}
							ImGui::EndCombo();
						}

					ImGui::Checkbox("Enable glow", &g_Options.glow_enabled); ImGui::SameLine(); ImGuiEx::ColorEdit4a("##Enemy   ", &g_Options.color_glow_enemy);

					const char* glow_enemies_type[] = { "Basic", "Pulse", "Outline inner" };
					if (g_Options.glow_enabled) {
						if (ImGui::BeginCombo("Glow type", glow_enemies_type[g_Options.glow_enemies_type], ImGuiComboFlags_NoArrowButton))
						{
							for (int i = 0; i < IM_ARRAYSIZE(glow_enemies_type); i++)
							{
								if (ImGui::Selectable(glow_enemies_type[i], i == g_Options.glow_enemies_type))
									g_Options.glow_enemies_type = i;
							}
							ImGui::EndCombo();
						}
					}
					ImGui::Checkbox("Enable occluded glow", &g_Options.glow_enemiesOC); ImGui::SameLine(); ImGuiEx::ColorEdit4a("##color_glow_enemiesOC   ", &g_Options.color_glow_enemyOC);
				}
				ImGui::endchild();

				ImGui::SetCursorPos({ 417, 52 });
				ImGui::beginchild("Colored models", { 255, 245 });
				{
					ImGui::Checkbox("Enable chams", &g_Options.chams_player_enabled); ImGui::SameLine(); ImGuiEx::ColorEdit4a("Chams color", &g_Options.color_chams_player_enemy_visible); ImGuiEx::ColorEdit4a("Chams color", &g_Options.color_chams_player_enemy_occluded);

					const char* chams[] = { "Regular", "Flat", "Glass", "Glow", "Overlay", "Animated" };
					if (g_Options.chams_player_enabled)
						if (ImGui::BeginCombo("Chams material", chams[g_Options.chams_type], ImGuiComboFlags_NoArrowButton))
						{
							for (int i = 0; i < IM_ARRAYSIZE(chams); i++)
							{
								if (ImGui::Selectable(chams[i], i == g_Options.chams_type))
									g_Options.chams_type = i;
							}
							ImGui::EndCombo();
						}

					ImGui::Checkbox("Enemies only", &g_Options.chams_player_enemies_only);
					ImGui::Checkbox("Visible only", &g_Options.chams_player_visible_only);
				}
				ImGui::endchild();
			}
			else if (subtab == 2) {
				ImGui::SetCursorPos({ 147, 52 });
				ImGui::beginchild("World", { 255, 245 });
				{
					ImGui::Checkbox("Third person", &g_Options.thirdperson); ImGui::SameLine();          ImGui::Hotkey("                    ", &g_Options.thirdperson_key, ImVec2(50, 15));

					if (g_Options.thirdperson)
						ImGui::SliderFloat("Distance", &g_Options.thirdperson_dist, 10.f, 250.f);

					ImGui::Checkbox("No flash effect", &g_Options.no_flash);
					ImGui::Checkbox("No smoke effect", &g_Options.no_smoke);

					ImGui::Checkbox("Aspect ratio", &g_Options.aspectratio);

					if (g_Options.aspectratio)
						ImGui::SliderInt("Amount", &g_Options.aspectratio_amount, 0, 200);

					ImGui::Checkbox("Disable post-processing", &g_Options.disable_postprocessing);
					ImGui::Checkbox("Disable shadows", &g_Options.disable_shadows);
					ImGui::Checkbox("Disable 3d sky", &g_Options.disable_3dsky);

					ImGui::Checkbox("Enable Sunset mode", &g_Options.sunset);
					ImGui::SliderInt("Sunset x", &g_Options.sunsetx, -100, 100);
					ImGui::SliderInt("Sunset y", &g_Options.sunsety, -100, 100);
					ImGui::SliderInt("Sunset z", &g_Options.sunsetz, -100, 100);

					ImGui::Checkbox("Nightmode", &g_Options.nightmode);

					ImGui::Checkbox("Skybox changer", &g_Options.skyboxchanger);

					if (g_Options.skyboxchanger)
						ImGui::Combo("Skybox", &g_Options.skyboxtype, "cs_tibet\0cs_baggage\0embassy\0italy\0jungle\0office\0sky_daylight01\0vertigoblue\0sky_daylight02\0vertigo\0sky_day02\0nukeblank\0sky_venice\0sky_daylight03\0sky_daylight03\0sky_daylight04\0sky_cloudy\0sky_night2\0sky_night2b\0sky_night_flat\0sky_dust\0vietnam");
				}
				ImGui::endchild();
			}
		}
		if (tab == 2)
		{
			ImGui::SetCursorPos({ 0, 60 });
			ImGui::BeginGroup();
			{
				if (ImGui::subtab("General", subtab == 3)) subtab = 3;
				if (ImGui::subtab("Settings", subtab == 4)) subtab = 4;
			}
			ImGui::EndGroup();

			if (subtab == 3)
			{
				ImGui::SetCursorPos({ 147, 52 });
				ImGui::beginchild("General", { 255, 245 });
				{
					ImGui::Checkbox("Reveal ranks", &g_Options.misc_showranks);
					ImGui::Checkbox("Watermark##hc", &g_Options.misc_watermark);
					ImGui::Checkbox("Clantag", &g_Options.clantag_spammer);
					ImGui::Checkbox("Spectator list", &g_Options.spectator_list);
					ImGui::Checkbox("Velocity", &g_Options.velocity);

					if (g_Options.velocity) {
						ImGui::SliderInt("Offset", &g_Options.velocity_offset, 0, 500);
						ImGui::Checkbox("Velocity pre", &g_Options.velocity_pre);
					}

					ImGui::Checkbox("Show binds", &g_Options.showindicators);
					ImGui::Checkbox("Show keys", &g_Options.showkeys); ImGui::SameLine(); ImGuiEx::ColorEdit4a("Show keys col", &g_Options.showkeys_color);

					if (g_Options.showkeys)
						ImGui::SliderInt("Pos", &g_Options.showkeys_pos, 0, 1000);

					ImGui::Checkbox("Force crosshair", &g_Options.forcecrosshair);
					ImGui::Checkbox("Auto-accept", &g_Options.misc_autoaccept);
				}
				ImGui::endchild();

				ImGui::SetCursorPos({ 417, 52 });
				ImGui::beginchild("Movement", { 255, 245 });
				{
					ImGui::Checkbox("Auto bhop", &g_Options.misc_bhop);
					ImGui::Checkbox("Auto strafe", &g_Options.autostrafe); ImGui::SameLine();     ImGui::Hotkey(" ", &g_Options.autostrafe_key, ImVec2(50, 15));
					ImGui::Checkbox("Edge bug", &g_Options.edge_bug); ImGui::SameLine();          ImGui::Hotkey("  ", &g_Options.edge_bug_key, ImVec2(50, 15));
					if (g_Options.edge_bug)
					{
						ImGui::Checkbox("Edge bug notify", &g_Options.edgebugnotify);
						ImGui::Checkbox("Edge bug effect", &g_Options.edgebugeffect);
						ImGui::Checkbox("Edge bug sound", &g_Options.edgebugsound);
					}
					ImGui::Checkbox("Jump bug", &g_Options.jump_bug); ImGui::SameLine();          ImGui::Hotkey("   ", &g_Options.jump_bug_key, ImVec2(50, 15));
					ImGui::Checkbox("Edge jump", &g_Options.edgejump); ImGui::SameLine(); ImGui::Hotkey("    ", &g_Options.ej_hotkey, ImVec2(50, 15));

					if (g_Options.edgejump)
						ImGui::Checkbox("Crouch in air", &g_Options.edge_jump_duck_in_air);
				}
				ImGui::endchild();
			}
			else if (subtab == 4) {
				ImGui::SetCursorPos({ 147, 52 });
				ImGui::beginchild("Settings", { 255, 245 });
				{
					static int selected = 0;
					static char cfgName[64];

					std::vector<std::string> cfgList;
					ReadDirectory(g_Options.folder, cfgList);
					ImGui::PushItemWidth(150.f);
					if (!cfgList.empty())
					{
						if (ImGui::BeginCombo("Config", cfgList[selected].c_str(), ImGuiComboFlags_NoArrowButton))
						{
							for (size_t i = 0; i < cfgList.size(); i++)
							{
								if (ImGui::Selectable(cfgList[i].c_str(), i == selected))
									selected = i;
							}
							ImGui::EndCombo();
						}
						if (ImGui::Button("Save config"))
							g_Options.SaveSettings(cfgList[selected]);
						if (ImGui::Button("Load config"))
							g_Options.LoadSettings(cfgList[selected]);
						if (ImGui::Button("Delete config"))
						{
							g_Options.DeleteSettings(cfgList[selected]);
							selected = 0;
						}
					}
					ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
					ImGui::InputText("##configname", cfgName, 24);
					if (ImGui::Button("Create config"))
					{
						if (strlen(cfgName))
							g_Options.SaveSettings(cfgName + std::string(".cfg"));
					}
					ImGui::PopItemWidth();
				}
				ImGui::endchild();
			}
		}
		if (tab == 3)
		{
			ImGui::SetCursorPos({ 0, 60 });
			ImGui::BeginGroup();
			{
				if (ImGui::subtab("General", subtab == 5)) subtab = 5;
				if (ImGui::subtab("Models", subtab == 6)) subtab = 6;
			}
			ImGui::EndGroup();

			if (subtab == 5)
			{
				auto& entries = g_Options.changers.skin.m_items;
				static auto definition_vector_index = 0;
				auto& selected_entry = entries[k_weapon_names[definition_vector_index].definition_index];
				auto& satatt = g_Options.changers.skin.statrack_items[k_weapon_names[definition_vector_index].definition_index];
				selected_entry.definition_index = k_weapon_names[definition_vector_index].definition_index;
				selected_entry.definition_vector_index = definition_vector_index;
				ImGui::SetCursorPos({ 147, 52 });
				ImGui::beginchild("Weapon", { 255, 245 });
				{
					ImGui::PushItemWidth(-1);;
					for (size_t w = 0; w < k_weapon_names.size(); w++) {
						if (ImGui::Selectable(k_weapon_names[w].name, definition_vector_index == w)) {
							definition_vector_index = w;
						}
					}
					ImGui::PopItemWidth();
				}
				ImGui::endchild();
				ImGui::SetCursorPos({ 417, 52 });
				ImGui::beginchild("Settings", { 255, 245 });
				{
					ImGui::Checkbox("Enable stattrak", &selected_entry.stat_trak);
					ImGui::Spacing();

					if (selected_entry.stat_trak)
						ImGui::InputInt("Stattrak", &satatt.statrack_new.counter);

					ImGui::Spacing();
					ImGui::InputInt("Seed", &selected_entry.seed);
					ImGui::Spacing();
					ImGui::SliderFloat("Wear", &selected_entry.wear, FLT_MIN, 1.f, "%.10f", 5);
					ImGui::Spacing();
					ImGui::InputText("Custom name", selected_entry.custom_name, 32);

					if (selected_entry.definition_index != GLOVE_T_SIDE &&
						selected_entry.definition_index != GLOVE_CT_SIDE &&
						selected_entry.definition_index != WEAPON_KNIFE &&
						selected_entry.definition_index != WEAPON_KNIFE_T) {
						ImGui::Combo("Paint kit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text) {
							*out_text = k_skins[idx].name.c_str();
							return true;
							}, nullptr, k_skins.size(), 20);
						selected_entry.paint_kit_index = k_skins[selected_entry.paint_kit_vector_index].id;
					}
					else if (selected_entry.definition_index == WEAPON_KNIFE || selected_entry.definition_index == WEAPON_KNIFE_T) {
						ImGui::Combo("Knife", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
							*out_text = k_knife_names.at(idx).name;
							return true;
							}, nullptr, k_knife_names.size(), 10);
						selected_entry.definition_override_index = k_knife_names.at(selected_entry.definition_override_vector_index).definition_index;

						ImGui::Combo("Knife paint kit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text) {
							*out_text = k_skins[idx].name.c_str();
							return true;
							}, nullptr, k_skins.size(), 20);
						selected_entry.paint_kit_index = k_skins[selected_entry.paint_kit_vector_index].id;
					}
					else if (selected_entry.definition_index == GLOVE_T_SIDE || selected_entry.definition_index == GLOVE_CT_SIDE) {
						ImGui::Combo("Glove", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text) {
							*out_text = k_glove_names.at(idx).name;
							return true;
							}, nullptr, k_glove_names.size(), 10);
						selected_entry.definition_override_index = k_glove_names.at(selected_entry.definition_override_vector_index).definition_index;

						ImGui::Combo("Glove paint kit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text) {
							*out_text = k_gloves[idx].name.c_str();
							return true;
							}, nullptr, k_gloves.size(), 20);
						selected_entry.paint_kit_index = k_gloves[selected_entry.paint_kit_vector_index].id;
					}
					else {
						static auto unused_value = 0;
						selected_entry.definition_override_vector_index = 0;
					}
					ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
					if (ImGui::Button("force update")) {
						static auto clear_hud_weapon_icon_fn =
							reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
								Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39"));

						auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

						if (!element)
							return;

						auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xa0);
						if (hud_weapons == nullptr)
							return;

						if (!*hud_weapons->get_weapon_count())
							return;

						for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
							i = clear_hud_weapon_icon_fn(hud_weapons, i);

						typedef void(*ForceUpdate) (void);
						static ForceUpdate FullUpdate = (ForceUpdate)Utils::PatternScan(GetModuleHandleA("engine.dll"), "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
						FullUpdate();
					}
				}
				ImGui::endchild();
			}
			else if (subtab == 6)
			{
				ImGui::SetCursorPos({ 147, 52 });
				ImGui::beginchild("Player models", { 255, 245 });
				{
					ImGui::Combo("Counter-Terrorist", &g_Options.playerModelCT, "Default\0Cmdr. Davida 'Goggles' Fernandez | SEAL Frogman\0Cmdr. Frank 'Wet Sox' Baroud | SEAL Frogman\0Lieutenant Rex Krikey | SEAL Frogman\0Michael Syfers | FBI Sniper\0Operator | FBI SWAT\0Special Agent Ava | FBI\0Markus Delrow | FBI HRT\0Sous-Lieutenant Medic | Gendarmerie Nationale\0Chem-Haz Capitaine | Gendarmerie Nationale\0Chef d'Escadron Rouchard | Gendarmerie Nationale\0Aspirant | Gendarmerie Nationale\0Officer Jacques Beltram | Gendarmerie Nationale\0D Squadron Officer | NZSAS\0B Squadron Officer | SAS\0Seal Team 6 Soldier | NSWC SEAL\0Buckshot | NSWC SEAL\0Lt. Commander Ricksaw | NSWC SEAL\0'Blueberries' Buckshot | NSWC SEAL\0third Commando Company | KSK\0'Two Times' McCoy | TACP Cavalry\0'Two Times' McCoy | USAF TACP\0Primeiro Tenente | Brazilian 1st Battalion\0Cmdr. Mae 'Dead Cold' Jamison | SWAT\0first Lieutenant Farlow | SWAT\0John 'Van Healen' Kask | SWAT\0Bio-Haz Specialist | SWAT\0Sergeant Bombson | SWAT\0Chem-Haz Specialist | SWAT\0Getaway Sally | The Professionals\0Number K | The Professionals\0Little Kev | The Professionals\0Safecracker Voltzmann | The Professionals\0Bloody Darryl The Strapped | The Professionals\0Sir Bloody Loudmouth Darryl | The Professionals\0Sir Bloody Darryl Royale | The Professionals\0Sir Bloody Skullhead Darryl | The Professionals\0Sir Bloody Silent Darryl | The Professionals\0Sir Bloody Miami Darryl | The Professionals\0Street Soldier | Phoenix\0Soldier | Phoenix\0Slingshot | Phoenix\0Enforcer | Phoenix\0Mr. Muhlik | Elite Crew\0Prof. Shahmat | Elite Crew\0Osiris | Elite Crew\0Ground Rebel | Elite Crew\0The Elite Mr. Muhlik | Elite Crew\0Trapper | Guerrilla Warfare\0Trapper Aggressor | Guerrilla Warfare\0Vypa Sista of the Revolution | Guerrilla Warfare\0Col. Mangos Dabisi | Guerrilla Warfare\0Arno The Overgrown | Guerrilla Warfare\0'Medium Rare' Crasswater | Guerrilla Warfare\0Crasswater The Forgotten | Guerrilla Warfare\0Elite Trapper Solman | Guerrilla Warfare\0'The Doctor' Romanov | Sabre\0Blackwolf | Sabre\0Maximus | Sabre\0Dragomir | Sabre\0Rezan The Ready | Sabre\0Rezan the Redshirt | Sabre\0Dragomir | Sabre Footsoldier\0");
					ImGui::Combo("Terrorist", &g_Options.playerModelT, "Default\0Cmdr. Davida 'Goggles' Fernandez | SEAL Frogman\0Cmdr. Frank 'Wet Sox' Baroud | SEAL Frogman\0Lieutenant Rex Krikey | SEAL Frogman\0Michael Syfers | FBI Sniper\0Operator | FBI SWAT\0Special Agent Ava | FBI\0Markus Delrow | FBI HRT\0Sous-Lieutenant Medic | Gendarmerie Nationale\0Chem-Haz Capitaine | Gendarmerie Nationale\0Chef d'Escadron Rouchard | Gendarmerie Nationale\0Aspirant | Gendarmerie Nationale\0Officer Jacques Beltram | Gendarmerie Nationale\0D Squadron Officer | NZSAS\0B Squadron Officer | SAS\0Seal Team 6 Soldier | NSWC SEAL\0Buckshot | NSWC SEAL\0Lt. Commander Ricksaw | NSWC SEAL\0'Blueberries' Buckshot | NSWC SEAL\0third Commando Company | KSK\0'Two Times' McCoy | TACP Cavalry\0'Two Times' McCoy | USAF TACP\0Primeiro Tenente | Brazilian 1st Battalion\0Cmdr. Mae 'Dead Cold' Jamison | SWAT\0first Lieutenant Farlow | SWAT\0John 'Van Healen' Kask | SWAT\0Bio-Haz Specialist | SWAT\0Sergeant Bombson | SWAT\0Chem-Haz Specialist | SWAT\0Getaway Sally | The Professionals\0Number K | The Professionals\0Little Kev | The Professionals\0Safecracker Voltzmann | The Professionals\0Bloody Darryl The Strapped | The Professionals\0Sir Bloody Loudmouth Darryl | The Professionals\0Sir Bloody Darryl Royale | The Professionals\0Sir Bloody Skullhead Darryl | The Professionals\0Sir Bloody Silent Darryl | The Professionals\0Sir Bloody Miami Darryl | The Professionals\0Street Soldier | Phoenix\0Soldier | Phoenix\0Slingshot | Phoenix\0Enforcer | Phoenix\0Mr. Muhlik | Elite Crew\0Prof. Shahmat | Elite Crew\0Osiris | Elite Crew\0Ground Rebel | Elite Crew\0The Elite Mr. Muhlik | Elite Crew\0Trapper | Guerrilla Warfare\0Trapper Aggressor | Guerrilla Warfare\0Vypa Sista of the Revolution | Guerrilla Warfare\0Col. Mangos Dabisi | Guerrilla Warfare\0Arno The Overgrown | Guerrilla Warfare\0'Medium Rare' Crasswater | Guerrilla Warfare\0Crasswater The Forgotten | Guerrilla Warfare\0Elite Trapper Solman | Guerrilla Warfare\0'The Doctor' Romanov | Sabre\0Blackwolf | Sabre\0Maximus | Sabre\0Dragomir | Sabre\0Rezan The Ready | Sabre\0Rezan the Redshirt | Sabre\0Dragomir | Sabre Footsoldier\0");
				}
				ImGui::endchild();
			}
		}
	}
	ImGui::End();
}

void Menu::Toggle()
{
	_visible = !_visible;
}

void Menu::CreateStyle()
{
	ImGui::StyleColorsDark();
}