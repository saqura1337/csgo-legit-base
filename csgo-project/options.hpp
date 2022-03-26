#pragma once

#include <Windows.h>
#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include "valve_sdk/Misc/Color.hpp"

#define A( s ) #s
#define OPTION(type, var, val) Var<type> var = {A(var), val}
template <typename T>
class ConfigValue
{
public:
	ConfigValue(std::string category_, std::string name_, T* value_)
	{
		category = category_;
		name = name_;
		value = value_;
	}

	std::string category, name;
	T* value;
};

struct legitbot_s
{
	bool enabled = false;
	int silent2 = false;

	bool flash_check = false;
	bool smoke_check = false;
	bool autopistol = false;

	float fov = 0.f;
	float silent_fov = 0.f;
	float smooth = 1.f;

	int shot_delay = 0;
	int kill_delay = 0;

	//shit
	bool deathmatch = false;
	//

	struct
	{
		bool head = true;
		bool chest = true;
		bool hands = true;
		bool legs = true;
	} hitboxes;

	struct
	{
		bool enabled = false;
		int type = 0;
		int x = 100;
		int y = 100;
		int start = 1;
	} rcs;
	struct
	{
		bool enabled = false;
		int hotkey = 0;
	} autofire;
};

struct weapons
{
	legitbot_s legit;
};

struct statrack_setting
{
	int definition_index = 1;
	struct
	{
		int counter = 0;
	}statrack_new;
};

struct item_setting
{
	char name[32] = "Default";
	//bool enabled = false;
	int stickers_place = 0;
	int definition_vector_index = 0;
	int definition_index = 0;
	bool enabled_stickers = 0;
	int paint_kit_vector_index = 0;
	int paint_kit_index = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	bool stat_trak = 0;
	float wear = FLT_MIN;
	char custom_name[32] = "";
};
class Options
{
public:
	std::map<short, weapons> weapons;
	struct
	{
		/*struct
		{
			std::map<int, profilechanger_settings> profile_items = { };
		}profile;*/
		struct
		{
			bool skin_preview = false;
			bool show_cur = true;

			std::map<int, statrack_setting> statrack_items = { };
			std::map<int, item_setting> m_items = { };
			std::map<std::string, std::string> m_icon_overrides = { };
		}skin;
	}changers;
	//
	// ESP
	//
//	bool esp_enabled = false;
	bool esp_enemies_only = false;
	bool esp_player_boxes = false;
	bool esp_player_boxesOccluded = false;
	bool esp_player_names = false;
	bool esp_player_health = false;
	bool esp_player_armour = false;
	bool esp_player_weapons = false;
	int  esp_player_weapons_type;
	bool esp_player_snaplines = false;
	bool esp_dropped_weapons = false;
	int  esp_dropped_weapons_type;
	bool esp_defuse_kit = false;
	bool esp_planted_c4 = false;
	bool esp_items = false;
	bool esp_player_flags = false;
	bool esp_flags_armor = false;
	bool esp_flags_kit = false;
	bool esp_flags_hostage = false;
	bool esp_flags_scoped = false;
	bool esp_flags_money = false;
	bool esp_flags_c4 = false;
	bool esp_flags_defusing = false;

	bool nightmode = false;
	//
	// GLOW
	//
	bool glow_enabled = false;
	bool glow_enemies_only = false;
	bool glow_players = false;
	bool glow_chickens = false;
	bool glow_c4_carrier = false;
	bool glow_planted_c4 = false;
	bool glow_defuse_kits = false;
	bool glow_weapons = false;
	bool glow_enemiesOC = false;

	//
	// CHAMS
	//
	bool chams_player_enabled = false;

	bool chams_player_enemies_only = false;
	bool chams_player_visible_only = false;

	int chams_type = 0;

	//
	// MISC
	//
	bool misc_bhop = false;
	bool misc_bhop2 = false;
	int playerModelT{ 0 };
	int playerModelCT{ 0 };

	bool misc_showranks = false;
	bool misc_watermark = false;
	bool clantag_spammer = false;
	bool velocity = false;
	bool velocity_pre = false;
	int velocity_offset = 140;
	bool showindicators = false;
	bool showkeys = false;
	int showkeys_pos = 0;
	bool no_flash = false;
	bool no_smoke = false;
	bool thirdperson = false;
	float thirdperson_dist = 10.f;
	int thirdperson_key;
	bool aspectratio = false;
	int aspectratio_amount = 0;
	bool disable_postprocessing;
	bool disable_shadows;
	bool disable_3dsky;
	bool sunset;
	int sunsetx;
	int sunsety;
	int sunsetz;
	bool skyboxchanger = false;
	int skyboxtype = 0;
	bool spectator_list = false;
	int autostrafe_key;
	bool autostrafe = false;

	bool edgejump = false;
	bool edge_jump_duck_in_air = false;
	int ej_hotkey = 0;

	bool edge_bug;
	int edge_bug_key;
	bool edgebugnotify;
	bool edgebugeffect;
	bool edgebugsound;
	bool jump_bug = false;
	int jump_bug_key;
	int glow_enemies_type;
	bool forcecrosshair = false;
	bool misc_autoaccept = false;

	//
	// COLORS
	//
	Color color_esp_ally_visible = { 255, 255, 255 };
	Color color_esp_enemy_visible = { 255, 255, 255 };
	Color color_esp_ally_occluded = { 255, 255, 255 };
	Color color_esp_enemy_occluded = { 255, 255, 255 };
	Color color_esp_defuse = { 255, 255, 255 };
	Color color_esp_c4 = { 255, 255, 255 };
	Color esp_player_weapons_color = { 255, 255, 255 };
	Color esp_dropped_weapons_color = { 255, 255, 255 };
	Color esp_player_health_color = { 255, 255, 255 };

	Color showkeys_color = { 255, 255, 255 };

	Color color_glow_enemy = { 255, 255, 255 };
	Color color_glow_enemyOC = { 255, 255, 255 };

	Color color_chams_player_enemy_visible = { 255, 255, 255 };
	Color color_chams_player_enemy_occluded = { 255, 255, 255 };

protected:
	//std::vector<ConfigValue<char>*> chars;
	std::vector<ConfigValue<int>*> ints;
	std::vector<ConfigValue<bool>*> bools;
	std::vector<ConfigValue<float>*> floats;
private:
	//	void SetupValue(char value, std::string category, std::string name);
	void SetupValue(int& value, std::string category, std::string name);
	void SetupValue(bool& value, std::string category, std::string name);
	void SetupValue(float& value, std::string category, std::string name);
	void SetupColor(Color& value, const std::string& name);
	void SetupWeapons();
	void SetupVisuals();
	void SetupMisc();
public:
	void Initialize();
	void LoadSettings(const std::string& szIniFile);
	void SaveSettings(const std::string& szIniFile);
	void DeleteSettings(const std::string& szIniFile);

	std::string folder;
};

inline Options g_Options;
inline bool   g_Unload;