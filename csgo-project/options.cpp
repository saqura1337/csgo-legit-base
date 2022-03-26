#include <ShlObj.h>
#include <filesystem>
#include "options.hpp"
#include "valve_sdk/misc/Enums.hpp"
#include "valve_sdk/sdk.hpp"

const std::map<int, const char*> config_names = {
	{WEAPON_CZ75A, "CZ75 Auto"},
	{WEAPON_DEAGLE, "Desert Eagle"},
	{WEAPON_ELITE, "Dual Berettas"},
	{WEAPON_FIVESEVEN, "Five-SeveN"},
	{WEAPON_HKP2000, "P2000"},
	{WEAPON_P250, "P250"},
	{WEAPON_USP_SILENCER, "USP-S"},
	{WEAPON_TEC9, "Tec-9"},
	{WEAPON_REVOLVER, "R8 Revolver"},
	{WEAPON_GLOCK, "Glock-18"},

	{WEAPON_MAG7, "MAG-7"},
	{WEAPON_NOVA, "Nova"},
	{WEAPON_SAWEDOFF, "Sawed-Off"},
	{WEAPON_XM1014, "XM1014"},

	{WEAPON_UMP45, "UMP-45"},
	{WEAPON_P90, "P90"},
	{WEAPON_BIZON, "PP-Bizon"},
	{WEAPON_MAC10, "MAC-10"},
	{WEAPON_MP7, "MP7"},
	{WEAPON_MP9, "MP9"},
	{WEAPON_MP5, "MP5-SD"},

	{WEAPON_M249, "M249"},
	{WEAPON_NEGEV, "Negev"},

	{WEAPON_AK47, "AK-47"},
	{WEAPON_AUG, "AUG"},
	{WEAPON_GALILAR, "Galil AR"},
	{WEAPON_M4A1_SILENCER, "M4A1-S"},
	{WEAPON_M4A1, "M4A4"},
	{WEAPON_SG556, "SG 553"},
	{WEAPON_FAMAS, "FAMAS"},

	{WEAPON_AWP, "AWP"},
	{WEAPON_G3SG1, "G3SG1"},
	{WEAPON_SCAR20, "SCAR-20"},
	{WEAPON_SSG08, "SSG 08"},

	{WEAPON_KNIFE, "Knife"},
	{WEAPON_KNIFE_T, "TKnife"},

	{GLOVE_T_SIDE, "Glove"},
};

const std::map<int, const char*> k_weapon_names = {
	{WEAPON_KNIFE, "knife ct"},
	{WEAPON_KNIFE_T, "knife t"},
	{GLOVE_CT_SIDE, "glove ct"},
	{GLOVE_T_SIDE, "glove t"},
	{61, "usp"},
	{32, "p2000"},
	{4, "glock-18"},
	{2, "dual berettas"},
	{36, "p250"},
	{3, "five-seven"},
	{30, "tec-9"},
	{63, "cz75 auto"},
	{64, "r8 revolver"},
	{1, "deagle"},

	{34, "mp9"},
	{17, "mac-10"},
	{23, "mp5-sd"},
	{33, "mp7"},
	{24, "ump-45"},
	{19, "p90"},
	{26, "pp-bizon"},

	{7, "ak-47"},
	{60, "m4a1-s"},
	{16, "m4a4"},
	{8, "aug"},
	{39, "sg553"},
	{10, "famas"},
	{13, "galil"},

	{40, "ssg08"},
	{38, "scar-20"},
	{9, "awp"},
	{11, "g3sg1"},

	{14, "m249"},
	{28, "negev"},

	{27, "mag-7"},
	{35, "nova"},
	{29, "sawed-off"},
	{25, "xm1014"},
};

void Options::SetupValue(int& value, std::string category, std::string name)
{
	ints.push_back(new ConfigValue<int>(std::move(category), std::move(name), &value));
}
/*void Options::SetupValue(char value, std::string category, std::string name)
{
	chars.push_back(new ConfigValue<char>(std::move(category), std::move(name), &value));
}*/

void Options::SetupValue(bool& value, std::string category, std::string name)
{
	bools.push_back(new ConfigValue<bool>(std::move(category), std::move(name), &value));
}

void Options::SetupValue(float& value, std::string category, std::string name)
{
	floats.push_back(new ConfigValue<float>(std::move(category), std::move(name), &value));
}

void Options::SetupColor(Color& value, const std::string& name)
{
	SetupValue(value._CColor[0], "Colors", name + "_r");
	SetupValue(value._CColor[1], "Colors", name + "_g");
	SetupValue(value._CColor[2], "Colors", name + "_b");
	SetupValue(value._CColor[3], "Colors", name + "_a");
}

void Options::SetupWeapons()
{
	for (auto& [val, key] : config_names)
	{
		auto aimbot = &g_Options.weapons[val].legit;

		if (val != WEAPON_KNIFE && val != WEAPON_KNIFE_T && val != GLOVE_T_SIDE)
		{
			SetupValue(aimbot->enabled, key, "Enabled");
			SetupValue(aimbot->silent2, key, "Silent");

			SetupValue(aimbot->flash_check, key, "Flash Check");
			SetupValue(aimbot->smoke_check, key, "Smoke Check");
			SetupValue(aimbot->autopistol, key, "AutoPistol");

			SetupValue(aimbot->fov, key, "Fov");
			SetupValue(aimbot->silent_fov, key, "pSilent Fov");
			SetupValue(aimbot->smooth, key, "Smooth");

			SetupValue(aimbot->kill_delay, key, "Kill Delay");
			SetupValue(aimbot->shot_delay, key, "Shot Delay");

			SetupValue(aimbot->hitboxes.head, key, "Head Hitbox");
			SetupValue(aimbot->hitboxes.chest, key, "Chest Hitbox");
			SetupValue(aimbot->hitboxes.hands, key, "Hands Hitbox");
			SetupValue(aimbot->hitboxes.legs, key, "Legs Hitbox");

			SetupValue(aimbot->rcs.enabled, key, "RCS Enabled");
			SetupValue(aimbot->rcs.start, key, "RCS Start");
			SetupValue(aimbot->rcs.type, key, "RCS Type");
			SetupValue(aimbot->rcs.x, key, "RCS X");
			SetupValue(aimbot->rcs.y, key, "RCS Y");

			SetupValue(aimbot->autofire.enabled, key, "Autofire Enabled");
			SetupValue(aimbot->autofire.hotkey, key, "Autofire Hotkey");
		}
	}
	for (auto& [key, val] : k_weapon_names) {
		auto& option = g_Options.changers.skin.m_items[key];
		auto& option1 = g_Options.changers.skin.statrack_items[key];
		SetupValue(option.definition_vector_index, val, "d_vec_index");
		SetupValue(option.definition_index, val, "d_index");
		SetupValue(option.paint_kit_vector_index, val, "pk_vec_index");
		SetupValue(option.paint_kit_index, val, "pk_index");
		SetupValue(option.definition_override_index, val, "do_index");
		SetupValue(option.definition_override_vector_index, val, "do_vec_index");
		//
		SetupValue(option.seed, val, "seed");
		SetupValue(option.stat_trak, val, "stattrack");
		SetupValue(option1.statrack_new.counter, val, "stat_trak_val");
		SetupValue(option.wear, val, "wear");
		//
	}
}

void Options::SetupVisuals()
{
	SetupValue(g_Options.esp_player_boxes, "ESP", "Boxes");
	SetupValue(g_Options.esp_player_boxesOccluded, "ESP", "Occluded");
	SetupValue(g_Options.esp_player_names, "ESP", "Names");
	SetupValue(g_Options.esp_player_health, "ESP", "Health");
	SetupValue(g_Options.esp_player_weapons, "ESP", "Weapon");
	SetupValue(g_Options.esp_player_weapons_type, "ESP", "Weapon render type");
	SetupValue(g_Options.esp_dropped_weapons_type, "ESP", "Dropped weapons render type");
	SetupValue(g_Options.esp_player_flags, "ESP", "Flags");
	SetupValue(g_Options.esp_flags_armor, "ESP", "Flag armor");
	SetupValue(g_Options.esp_flags_kit, "ESP", "Flag kit");
	SetupValue(g_Options.esp_flags_hostage, "ESP", "Flag hostage");
	SetupValue(g_Options.esp_flags_scoped, "ESP", "Flag scoped");
	SetupValue(g_Options.esp_flags_money, "ESP", "Flag money");
	SetupValue(g_Options.esp_flags_c4, "ESP", "Flag c4");
	SetupValue(g_Options.esp_flags_defusing, "ESP", "Flag defusing");
	SetupValue(g_Options.esp_dropped_weapons, "ESP", "Dropped Weapons");
	SetupValue(g_Options.chams_player_enabled, "ESP", "Enabled ch");
	SetupValue(g_Options.chams_player_enemies_only, "ESP", "Chams enemies only");
	SetupValue(g_Options.chams_player_visible_only, "ESP", "Chams visible only");
	SetupValue(g_Options.glow_enabled, "ESP", "Enabled gl");
	SetupValue(g_Options.glow_enemiesOC, "ESP", "Occluded gl");
	SetupValue(g_Options.glow_enemies_type, "ESP", "glow_enemies_type");
	SetupValue(g_Options.chams_type, "ESP", "Chams type");
	SetupColor(g_Options.color_esp_enemy_visible, "Enemies Visible");
	SetupColor(g_Options.color_esp_enemy_occluded, "Enemies Occluded");
	SetupColor(g_Options.color_chams_player_enemy_visible, "Enemy Visible ch");
	SetupColor(g_Options.color_chams_player_enemy_occluded, "Enemy Occluded ch");
	SetupColor(g_Options.color_glow_enemy, "Enemy Visible");
	SetupColor(g_Options.color_glow_enemyOC, "Enemy Occluded");
	SetupColor(g_Options.esp_dropped_weapons_color, "Dropped weapons color");
	SetupColor(g_Options.esp_player_health_color, "Health color");
	SetupColor(g_Options.esp_player_weapons_color, "Enemy weapons color");

	SetupValue(g_Options.nightmode, "ESP", "Enabled nightmode");
}

void Options::SetupMisc()
{
	SetupColor(g_Options.showkeys_color, "Show keys color");
	SetupValue(g_Options.misc_showranks, "Misc", "Reveal ranks");
	SetupValue(g_Options.misc_watermark, "Misc", "Watermark");
	SetupValue(g_Options.clantag_spammer, "Misc", "Clantag");
	SetupValue(g_Options.spectator_list, "Misc", "Spectator list");
	SetupValue(g_Options.velocity, "Misc", "Velocity");
	SetupValue(g_Options.velocity_pre, "Misc", "Velocity pre");
	SetupValue(g_Options.velocity_offset, "Misc", "Velocity offset");
	SetupValue(g_Options.showindicators, "Misc", "Show binds");
	SetupValue(g_Options.showkeys, "Misc", "Show keys");
	SetupValue(g_Options.showkeys_pos, "Misc", "Pos");
	SetupValue(g_Options.no_flash, "Misc", "No flash");
	SetupValue(g_Options.no_smoke, "Misc", "No smoke");
	SetupValue(g_Options.misc_bhop, "Misc", "Bunnyhop");
	SetupValue(g_Options.autostrafe, "Misc", "Auto strafe");
	SetupValue(g_Options.autostrafe_key, "Misc", "AutoStrafe_key");
	SetupValue(g_Options.edge_bug, "Misc", "Edgebug");
	SetupValue(g_Options.edge_bug_key, "Misc", "edge_bug_key");
	SetupValue(g_Options.edgebugnotify, "Misc", "Edgebug detect");
	SetupValue(g_Options.edgebugeffect, "Misc", "Edgebug effect");
	SetupValue(g_Options.edgebugsound, "Misc", "Edgebug sound");
	SetupValue(g_Options.jump_bug, "Misc", "jump_bug");
	SetupValue(g_Options.jump_bug_key, "Misc", "jump_bug_key");
	SetupValue(g_Options.edgejump, "Misc", "Edge jump");
	SetupValue(g_Options.ej_hotkey, "Misc", "edgejump_key");
	SetupValue(g_Options.edge_jump_duck_in_air, "Misc", "Crouch in Air");
	SetupValue(g_Options.forcecrosshair, "Misc", "Force crosshair");
	SetupValue(g_Options.misc_autoaccept, "Misc", "Auto-accept");
	SetupValue(g_Options.thirdperson, "Misc", "Thirdperson");
	SetupValue(g_Options.thirdperson_key, "Misc", "Thirdperson key");
	SetupValue(g_Options.thirdperson_dist, "Misc", "Thirdperson distance");
	SetupValue(g_Options.aspectratio, "Visuals", "aspectratio");
	SetupValue(g_Options.aspectratio_amount, "Visuals", "aspectratio_amount");
	SetupValue(g_Options.disable_postprocessing, "Visuals", "disable_postprocessing");
	SetupValue(g_Options.disable_shadows, "Visuals", "disable_shadows");
	SetupValue(g_Options.disable_3dsky, "Visuals", "disable_3dsky");
	SetupValue(g_Options.skyboxchanger, "Misc", "Skybox changer");
	SetupValue(g_Options.skyboxtype, "Misc", "Skybox type");
	SetupValue(g_Options.playerModelCT, "Misc", "playerModelCT");
	SetupValue(g_Options.playerModelT, "Misc", "playerModelT");

}

void Options::Initialize()
{
	CHAR my_documents[MAX_PATH];
	SHGetFolderPathA(nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, my_documents);
	folder = my_documents + std::string("\\Cheat name\\");
	CreateDirectoryA(folder.c_str(), nullptr);
	SetupWeapons();
	SetupVisuals();
	SetupMisc();
}

void Options::SaveSettings(const std::string& szIniFile)
{
	std::string file = folder + szIniFile;

	CreateDirectoryA(folder.c_str(), nullptr);

	for (auto value : ints)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());

	for (auto value : floats)
		WritePrivateProfileStringA(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
}

void Options::LoadSettings(const std::string& szIniFile)
{
	std::string file = folder + szIniFile;

	CreateDirectoryA(folder.c_str(), nullptr);

	if (!std::filesystem::exists(file))
		return;

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}

	for (auto value : floats)
	{
		GetPrivateProfileStringA(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = float(atof(value_l));
	}
	g_ClientState->ForceFullUpdate();
}

void Options::DeleteSettings(const std::string& szIniFile)
{
	std::string file = folder + szIniFile;

	CreateDirectoryA(folder.c_str(), nullptr);

	if (!std::filesystem::exists(file))
		return;

	remove(file.c_str());
}