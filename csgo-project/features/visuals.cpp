#include <algorithm>

#include "visuals.hpp"

#include "../options.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../menu.hpp"

auto GetGunName = [](short cw)
{
	switch (cw)
	{
		case WEAPON_KNIFE:
		case WEAPON_KNIFE_T:
		case 500:
		case 505:
		case 506:
		case 507:
		case 508:
		case 509:
		case 512:
		case 514:
		case 515:
		case 516:
		case 519:
		case 520:
		case 522:
		case 523:
			return "Knife";
		case WEAPON_DEAGLE:
			return "Deagle";
		case WEAPON_ELITE:
			return "Dual Berettas";
		case WEAPON_FIVESEVEN:
			return "Five-Seven";
		case WEAPON_GLOCK:
			return "Glock-18";
		case WEAPON_HKP2000:
			return "P2000";
		case WEAPON_P250:
			return "P250";
		case WEAPON_USP_SILENCER:
			return "USP-S";
		case WEAPON_TEC9:
			return "Tec-9";
		case WEAPON_CZ75A:
			return "CZ75 Auto";
		case WEAPON_REVOLVER:
			return "R8 Revolver";
		case WEAPON_MAC10:
			return "MAC-10";
		case WEAPON_UMP45:
			return "UMP-45";
		case WEAPON_BIZON:
			return "PP-Bizon";
		case WEAPON_MP5:
			return "MP5";
		case WEAPON_MP7:
			return "MP7";
		case WEAPON_MP9:
			return "MP9";
		case WEAPON_P90:
			return "P90";
		case WEAPON_GALILAR:
			return "Galil AR";
		case WEAPON_FAMAS:
			return "FAMAS";
		case WEAPON_M4A1_SILENCER:
			return "M4A1-S";
		case WEAPON_M4A1:
			return "M4A4";
		case WEAPON_AUG:
			return "AUG";
		case WEAPON_SG556:
			return "SG 553";
		case WEAPON_AK47:
			return "AK-47";
		case WEAPON_G3SG1:
			return "G3SG1";
		case WEAPON_SCAR20:
			return "SCAR-20";
		case WEAPON_AWP:
			return "AWP";
		case WEAPON_SSG08:
			return "SSG 08";
		case WEAPON_XM1014:
			return "XM1014";
		case WEAPON_SAWEDOFF:
			return "Sawed-Off";
		case WEAPON_MAG7:
			return "MAG-7";
		case WEAPON_NOVA:
			return "Nova";
		case WEAPON_NEGEV:
			return "Negev";
		case WEAPON_M249:
			return "M249";
		case WEAPON_TASER:
			return "Zeus";
		case WEAPON_FLASHBANG:
			return "Flashbang";
		case WEAPON_HEGRENADE:
			return "HE Grenade";
		case WEAPON_SMOKEGRENADE:
			return "Smoke";
		case WEAPON_MOLOTOV:
			return "Molotov";
		case WEAPON_DECOY:
			return "Decoy";
		case WEAPON_INCGRENADE:
			return "Inc Grenade";
		case WEAPON_C4:
			return "C4";
		default:
			return "";
	}
};

auto GetGunIcon = [](short cw)
{
	switch (cw)
	{
		case WEAPON_KNIFE:
		case WEAPON_KNIFE_T:
		case 500:
		case 505:
		case 506:
		case 507:
		case 508:
		case 509:
		case 512:
		case 514:
		case 515:
		case 516:
		case 519:
		case 520:
		case 522:
		case 523:
			return "]";
		case WEAPON_DEAGLE:
			return "A";
		case WEAPON_ELITE:
			return "B";
		case WEAPON_FIVESEVEN:
			return "C";
		case WEAPON_GLOCK:
			return "D";
		case WEAPON_HKP2000:
			return "E";
		case WEAPON_P250:
			return "F";
		case WEAPON_USP_SILENCER:
			return "G";
		case WEAPON_TEC9:
			return "H";
		case WEAPON_CZ75A:
			return "I";
		case WEAPON_REVOLVER:
			return "J";
		case WEAPON_MAC10:
			return "K";
		case WEAPON_UMP45:
			return "L";
		case WEAPON_BIZON:
			return "M";
		case WEAPON_MP7:
			return "N";
		case WEAPON_MP9:
			return "O";
		case WEAPON_P90:
			return "P";
		case WEAPON_GALILAR:
			return "Q";
		case WEAPON_FAMAS:
			return "R";
		case WEAPON_M4A1_SILENCER:
			return "T";
		case WEAPON_M4A1:
			return "S";
		case WEAPON_AUG:
			return "U";
		case WEAPON_SG556:
			return "V";
		case WEAPON_AK47:
			return "W";
		case WEAPON_G3SG1:
			return "X";
		case WEAPON_SCAR20:
			return "Y";
		case WEAPON_AWP:
			return "Z";
		case WEAPON_SSG08:
			return "a";
		case WEAPON_XM1014:
			return "b";
		case WEAPON_SAWEDOFF:
			return "c";
		case WEAPON_MAG7:
			return "d";
		case WEAPON_NOVA:
			return "e";
		case WEAPON_NEGEV:
			return "f";
		case WEAPON_M249:
			return "g";
		case WEAPON_TASER:
			return "h";
		case WEAPON_FLASHBANG:
			return "i";
		case WEAPON_HEGRENADE:
			return "j";
		case WEAPON_SMOKEGRENADE:
			return "k";
		case WEAPON_MOLOTOV:
			return "l";
		case WEAPON_DECOY:
			return "m";
		case WEAPON_INCGRENADE:
			return "n";
		case WEAPON_C4:
			return "o";
		default:
			return "";
	}
};

RECT GetBBox(C_BaseEntity* ent)
{
	RECT rect{};
	auto collideable = ent->GetCollideable();

	if (!collideable)
		return rect;

	auto min = collideable->OBBMins();
	auto max = collideable->OBBMaxs();

	const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

	Vector points[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++) {
		Math::VectorTransform(points[i], trans, pointsTransformed[i]);
	}

	Vector screen_points[8] = {};

	for (int i = 0; i < 8; i++) {
		if (!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
			return rect;
	}

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++) {
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}

Visuals::Visuals()
{
	InitializeCriticalSection(&cs);
}

Visuals::~Visuals() {
	DeleteCriticalSection(&cs);
}

//--------------------------------------------------------------------------------
void Visuals::Render() {
}
//--------------------------------------------------------------------------------
int flPlayerAlpha[64];
int flAlphaFade = 5.f;

bool Visuals::Player::Begin(C_BasePlayer* pl)
{
	if (!pl->IsAlive())
		return false;

	if (pl->IsDormant() && flPlayerAlpha[pl->EntIndex()] > 0)
	{
		flPlayerAlpha[pl->EntIndex()] -= flAlphaFade;
	}
	else if (flPlayerAlpha[pl->EntIndex()] < 255 && !(pl->IsDormant()))
	{
		flPlayerAlpha[pl->EntIndex()] += flAlphaFade;
	}
	if (flPlayerAlpha <= 0 && pl->IsDormant())
		return false;
	ctx.pl = pl;
	ctx.is_enemy = g_LocalPlayer->m_iTeamNum() != pl->m_iTeamNum();
	ctx.is_visible = g_LocalPlayer->CanSeePlayer(pl, HITBOX_CHEST);

	if (!ctx.is_enemy)
		return false;
	//if (!ctx.is_visible && /*g_Options.esp_player_boxesOccluded*/)
		//return false;
	//ctx.clr = ctx.is_enemy ? (ctx.is_visible ? g_Options.color_esp_enemy_visible : g_Options.color_esp_enemy_occluded) : (ctx.is_visible ? g_Options.color_esp_ally_visible : g_Options.color_esp_ally_occluded);

	auto head = pl->GetHitboxPos(HITBOX_HEAD);
	auto origin = pl->m_vecOrigin();

	if (!Math::WorldToScreen(head, ctx.head_pos) ||
		!Math::WorldToScreen(origin, ctx.feet_pos))
		return false;

	ctx.bbox = GetBBox(pl);

	std::swap(ctx.bbox.top, ctx.bbox.bottom);

	return !(!ctx.bbox.left || !ctx.bbox.top || !ctx.bbox.right || !ctx.bbox.bottom);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Visuals::Player::RenderBox(C_BaseEntity* pl) {
	if (!ctx.is_visible && g_Options.esp_player_boxesOccluded) {
		render::Get().RenderBoxByType(ctx.bbox.left, ctx.bbox.top, ctx.bbox.right, ctx.bbox.bottom, Color(g_Options.color_esp_enemy_occluded[0], g_Options.color_esp_enemy_occluded[1], g_Options.color_esp_enemy_occluded[2], flPlayerAlpha[pl->EntIndex()]), 1);
		render::Get().RenderBoxByType(ctx.bbox.left - 1, ctx.bbox.top - 1, ctx.bbox.right + 1, ctx.bbox.bottom + 1, Color(0, 0, 0, flPlayerAlpha[pl->EntIndex()]), 1);
		render::Get().RenderBoxByType(ctx.bbox.left + 1, ctx.bbox.top + 1, ctx.bbox.right - 1, ctx.bbox.bottom - 1, Color(0, 0, 0, flPlayerAlpha[pl->EntIndex()]), 1);
	}
	if (ctx.is_visible) {
		render::Get().RenderBoxByType(ctx.bbox.left, ctx.bbox.top, ctx.bbox.right, ctx.bbox.bottom, Color(g_Options.color_esp_enemy_visible[0], g_Options.color_esp_enemy_visible[1], g_Options.color_esp_enemy_visible[2], flPlayerAlpha[pl->EntIndex()]), 1);
		render::Get().RenderBoxByType(ctx.bbox.left - 1, ctx.bbox.top - 1, ctx.bbox.right + 1, ctx.bbox.bottom + 1, Color(0, 0, 0, flPlayerAlpha[pl->EntIndex()]), 1);
		render::Get().RenderBoxByType(ctx.bbox.left + 1, ctx.bbox.top + 1, ctx.bbox.right - 1, ctx.bbox.bottom - 1, Color(0, 0, 0, flPlayerAlpha[pl->EntIndex()]), 1);
	}
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderName(C_BaseEntity* pl)
{
	player_info_t info = ctx.pl->GetPlayerInfo();
	auto sz = g_pDefaultFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, info.szName);
	render::Get().RenderText(info.szName, ctx.bbox.left + (ctx.bbox.right - ctx.bbox.left - sz.x) / 2, (ctx.bbox.top - sz.y - 1), 12.f, Color(255, 255, 255, flPlayerAlpha[pl->EntIndex()]), false);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderHealth(C_BaseEntity* pl)
{
	auto hp = ctx.pl->m_iHealth();
	float box_h = (float)fabs(ctx.bbox.bottom - ctx.bbox.top);
	float off = 8;

	int height = (box_h * hp) / 100;

	int x = ctx.bbox.left - off;
	int y = ctx.bbox.top;
	int w = 4;
	int h = box_h;
	std::string text = std::to_string(hp);

	/*background*/
	render::Get().RenderBox(x, y - 1, x + w, y + h + 1, Color(0, 0, 0, flPlayerAlpha[pl->EntIndex()]));
	/*box*/
	render::Get().RenderBox(x + 1, y + h - height, x + w - 1, y + h, Color(g_Options.esp_player_health_color.r(), g_Options.esp_player_health_color.g(), g_Options.esp_player_health_color.b(), flPlayerAlpha[pl->EntIndex()]));
	/*hp text*/
	render::Get().RenderText(text, x - 20, y + h - height, 12.f, Color(255, 255, 255, flPlayerAlpha[pl->EntIndex()]));

}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderArmour()
{
	auto  armour = ctx.pl->m_ArmorValue();
	float box_h = (float)fabs(ctx.bbox.bottom - ctx.bbox.top);
	//float off = (box_h / 6.f) + 5;
	float off = 4;

	int height = (((box_h * armour) / 100));

	int x = ctx.bbox.right + off;
	int y = ctx.bbox.top;
	int w = 4;
	int h = box_h;

	render::Get().RenderBox(x, y, x + w, y + h, Color::Black, 1.f, true);
	render::Get().RenderBox(x + 1, y + 1, x + w - 1, y + height - 2, Color(0, 50, 255, 255), 1.f, true);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderWeaponName(C_BaseEntity* pl)
{
	auto weapon = ctx.pl->m_hActiveWeapon().Get();

	if (!weapon) return;
	if (!weapon->GetCSWeaponData()) return;

	auto gun_name = std::string(GetGunName(weapon->m_Item().m_iItemDefinitionIndex()));
	auto gun_icon = std::string(GetGunIcon(weapon->m_Item().m_iItemDefinitionIndex()));
	auto sz = g_pDefaultFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, gun_name.c_str());
	auto sz_icon = g_pWeaponsFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, gun_icon.c_str());

	if (g_Options.esp_player_weapons_type == 0)
		render::Get().RenderText(gun_name, ImVec2(ctx.bbox.left + (ctx.bbox.right - ctx.bbox.left - sz.x) / 2, ctx.bbox.bottom + 1), 12.f, Color(g_Options.esp_player_weapons_color.r(), g_Options.esp_player_weapons_color.g(), g_Options.esp_player_weapons_color.b(), flPlayerAlpha[pl->EntIndex()]), false);
	else if (g_Options.esp_player_weapons_type == 1)
		render::Get().RenderText(gun_icon, ImVec2(ctx.bbox.left + (ctx.bbox.right - ctx.bbox.left - sz_icon.x) / 2, ctx.bbox.bottom + 1), 14.f, Color(g_Options.esp_player_weapons_color.r(), g_Options.esp_player_weapons_color.g(), g_Options.esp_player_weapons_color.b(), flPlayerAlpha[pl->EntIndex()]), false, true, g_pWeaponsFont);
}
//--------------------------------------------------------------------------------
void Visuals::Player::RenderSnapline()
{

	int screen_w, screen_h;
	g_EngineClient->GetScreenSize(screen_w, screen_h);

	render::Get().RenderLine(screen_w / 2.f, (float)screen_h,
		ctx.feet_pos.x, ctx.feet_pos.y, Color::Red);
}
//--------------------------------------------------------------------------------
void Visuals::RenderWeapon(C_BaseCombatWeapon* ent)
{
	auto clean_item_name = [](const char* name) -> const char* {
		if (name[0] == 'C')
			name++;

		auto start = strstr(name, "Weapon");
		if (start != nullptr)
			name = start + 6;

		return name;
	};

	// We don't want to Render weapons that are being held
	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	auto name = std::string(GetGunName(ent->m_Item().m_iItemDefinitionIndex()));
	auto name_icon = std::string(GetGunIcon(ent->m_Item().m_iItemDefinitionIndex()));
	auto size = g_pDefaultFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, name.c_str());
	auto size_icon = g_pWeaponsFont->CalcTextSizeA(14.f, FLT_MAX, 0.0f, name_icon.c_str());
	int w = bbox.right - bbox.left;

	if (g_Options.esp_dropped_weapons_type == 0)
		render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - size.x * 0.5f, bbox.bottom + 1), 11.f, g_Options.esp_dropped_weapons_color);
	else if (g_Options.esp_dropped_weapons_type == 1)
		render::Get().RenderText(name_icon, ImVec2((bbox.left + w * 0.5f) - size_icon.x * 0.5f, bbox.bottom + 1), 14.f, g_Options.esp_dropped_weapons_color, false, true, g_pWeaponsFont);
}
//--------------------------------------------------------------------------------
void Visuals::RenderDefuseKit(C_BaseEntity* ent)
{
	if (ent->m_hOwnerEntity().IsValid())
		return;

	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;

	render::Get().RenderBox(bbox, g_Options.color_esp_defuse);

	auto name = "Defuse Kit";
	auto sz = g_pDefaultFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, name);
	int w = bbox.right - bbox.left;
	render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 12.f, Color::White);
}
//--------------------------------------------------------------------------------
void Visuals::RenderPlantedC4(C_BaseEntity* ent)
{
	auto bbox = GetBBox(ent);

	if (bbox.right == 0 || bbox.bottom == 0)
		return;


	render::Get().RenderBox(bbox, g_Options.color_esp_c4);


	int bombTimer = std::ceil(ent->m_flC4Blow() - g_GlobalVars->curtime);
	std::string timer = std::to_string(bombTimer);

	auto name = (bombTimer < 0.f) ? "Bomb" : timer;
	auto sz = g_pDefaultFont->CalcTextSizeA(12.f, FLT_MAX, 0.0f, name.c_str());
	int w = bbox.right - bbox.left;

	render::Get().RenderText(name, ImVec2((bbox.left + w * 0.5f) - sz.x * 0.5f, bbox.bottom + 1), 12.f, g_Options.color_esp_c4);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#define FLAG_MACRO std::pair<std::string, Color>
#define FLAG(string, color) vecFlags.push_back(FLAG_MACRO(string, color)) 

void Visuals::Player::Flags(C_BaseEntity* pl)
{
	if (!g_LocalPlayer || !g_EngineClient->IsConnected() || !g_EngineClient->IsInGame())
		return;

	std::vector<FLAG_MACRO> vecFlags;

	if (g_Options.esp_flags_armor)
		if (ctx.pl->m_ArmorValue() > 0)
			FLAG(ctx.pl->GetArmorName(), Color(240, 240, 240, flPlayerAlpha[pl->EntIndex()]));

	if (g_Options.esp_flags_scoped)
		if (ctx.pl->m_bIsScoped())
			FLAG("ZOOM", Color(30, 120, 235, flPlayerAlpha[pl->EntIndex()]));

	if (g_Options.esp_flags_money)
		if (ctx.pl->m_iAccount())
			FLAG(std::string("$").append(std::to_string(ctx.pl->m_iAccount())), Color(170, 190, 80, flPlayerAlpha[pl->EntIndex()]));

	if (g_Options.esp_flags_defusing)
		if (ctx.pl->m_bIsDefusing())
			FLAG("DEFUSING", Color(200, 0, 0, flPlayerAlpha[pl->EntIndex()]));

	if (g_Options.esp_flags_c4)
		if (ctx.pl->HasC4())
			FLAG("C4", Color(200, 0, 0, flPlayerAlpha[pl->EntIndex()]));

	if (g_Options.esp_flags_hostage)
		if (ctx.pl->m_hCarriedHostage() || ctx.pl->m_bIsGrabbingHostage())
			FLAG("HOSTAGE", ctx.pl->m_bIsGrabbingHostage() ? Color(200, 0, 0, flPlayerAlpha[pl->EntIndex()]) : Color(30, 120, 235, flPlayerAlpha[pl->EntIndex()]));

	if (g_Options.esp_flags_kit)
		if (ctx.pl->m_bHasDefuser())
			FLAG("KIT", Color(30, 120, 235, flPlayerAlpha[pl->EntIndex()]));

	int offset = 0;
	for (auto Text : vecFlags)
	{
		render::Get().RenderText(Text.first, ctx.bbox.right + 4 + ((ctx.pl->m_ArmorValue() > 0 && g_Options.esp_player_armour) ? 5 : 0), ctx.bbox.top - 2 + offset, 11.f, Text.second);
		offset += 12;
	}
}
//
bool lastvelsaved = false; //saver 
int lastjump, lastvel, lasttick = 0; // last vel holder 
std::string drawvel; //text drawer holder 
std::string drawvel2;
void render_speed()
{
	if (!g_Options.velocity)
		return;

	if (!g_LocalPlayer)
		return;

	int screenWidth, screenHeight;
	g_EngineClient->GetScreenSize(screenWidth, screenHeight);

	auto local_player = g_LocalPlayer;

	Vector speed = local_player->m_vecVelocity();
	int intspeed = round(speed.Length2D());

	const float delta = intspeed - lastvel;

	std::string vel = std::to_string(intspeed);

	if (local_player->m_fFlags() & FL_ONGROUND)
	{
		if (lastvelsaved)
		{
			lastvelsaved = false;
		}

		drawvel = vel;
	}
	else
	{
		if (!lastvelsaved)
		{
			lastjump = intspeed;
			lastvelsaved = true;
		}
		drawvel = vel + " (" + std::to_string(lastjump) + ")";
	}
	drawvel2 = "(" + std::to_string(lastjump) + ")";

	Color c = vel == std::to_string(lastjump) ? Color(255, 199, 89) : vel < std::to_string(lastjump) ? Color(255, 119, 119) : Color(30, 255, 109);

	if (local_player->m_nMoveType() == MOVETYPE_NOCLIP) {
		render::Get().RenderText("noclip", screenWidth / 2, screenHeight - g_Options.velocity_offset, 27.f, Color(255, 255, 255, 255), false, false, g_VeloFont, true);
	}
	else
	{
		render::Get().RenderText(std::to_string(intspeed), screenWidth / 2 - 48, screenHeight - g_Options.velocity_offset, 27.f, c, false, false, g_VeloFont, true);

		if (!(local_player->m_fFlags() & FL_ONGROUND)) {
			if (lastjump >= 100 && g_Options.velocity_pre)
			{
				render::Get().RenderText(drawvel2, screenWidth / 2 + 2, screenHeight - g_Options.velocity_offset, 27.f, c, false, false, g_VeloFont, true);
			}
		}
	}
}

void Visuals::thirdperson()
{
	if (!g_LocalPlayer)
		return;

	if (!g_Options.thirdperson && !g_LocalPlayer->IsAlive())
		return;

	if (GetKeyState(g_Options.thirdperson_key))
	{
		if (!g_Input->m_fCameraInThirdPerson)
		{
			g_Input->m_fCameraInThirdPerson = true;
		}

		if (!g_LocalPlayer->IsAlive())
			g_Input->m_fCameraInThirdPerson = false;

		float dist = g_Options.thirdperson_dist;

		QAngle* view = g_LocalPlayer->GetVAngles();
		trace_t tr;
		Ray_t ray;

		Vector desiredCamOffset = Vector(cos(DEG2RAD(view->yaw)) * dist,
			sin(DEG2RAD(view->yaw)) * dist,
			sin(DEG2RAD(-view->pitch)) * dist
		);

		ray.Init(g_LocalPlayer->GetEyePos(), (g_LocalPlayer->GetEyePos() - desiredCamOffset));
		CTraceFilter traceFilter;
		traceFilter.pSkip = g_LocalPlayer;
		g_EngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

		Vector diff = g_LocalPlayer->GetEyePos() - tr.endpos;

		float distance2D = sqrt(abs(diff.x * diff.x) + abs(diff.y * diff.y));

		bool horOK = distance2D > (dist - 2.0f);
		bool vertOK = (abs(diff.z) - abs(desiredCamOffset.z) < 3.0f);

		float cameraDistance;

		if (horOK && vertOK)
		{
			cameraDistance = dist;
		}
		else
		{
			if (vertOK)
			{
				cameraDistance = distance2D * 0.95f;
			}
			else
			{
				cameraDistance = abs(diff.z) * 0.95f;
			}
		}
		g_Input->m_fCameraInThirdPerson = true;

		g_Input->m_vecCameraOffset.z = cameraDistance;
	}
	else
	{
		g_Input->m_fCameraInThirdPerson = false;
	}
}

void Visuals::RenderBinds()
{
	if (!g_Options.showindicators)
		return;

	int w, h;
	g_EngineClient->GetScreenSize(w, h);
	if (GetAsyncKeyState(g_Options.edge_bug_key) && !GetAsyncKeyState(g_Options.jump_bug_key) && !GetAsyncKeyState(g_Options.ej_hotkey))
		render::Get().RenderText("eb", ImVec2(w / 2, h - h / 6), 25.f, Color(255, 255, 255, 255), false, false, g_VeloFont);

	if (GetAsyncKeyState(g_Options.jump_bug_key) && !GetAsyncKeyState(g_Options.edge_bug_key) && !GetAsyncKeyState(g_Options.ej_hotkey))
		render::Get().RenderText("jb", ImVec2(w / 2, h - h / 6), 25.f, Color(255, 255, 255, 255), false, false, g_VeloFont);

	if (GetAsyncKeyState(g_Options.ej_hotkey) && !GetAsyncKeyState(g_Options.edge_bug_key) && !GetAsyncKeyState(g_Options.jump_bug_key))
		render::Get().RenderText("ej", ImVec2(w / 2, h - h / 6), 25.f, Color(255, 255, 255, 255), false, false, g_VeloFont);
}

void Visuals::RenderKeystrokes()
{
	if (!g_Options.showkeys)
		return;

	int w, h;
	g_EngineClient->GetScreenSize(w, h);
	int position = g_Options.showkeys_pos;

	if (GetAsyncKeyState(VK_CONTROL)) //crouch
		render::Get().RenderText("c", ImVec2(w / 2 - 18 - 2 + 2, position - 15), 14.f, g_Options.showkeys_color, false, false, g_Calibri);

	if (GetAsyncKeyState(0x57)) //w
		render::Get().RenderText("w", ImVec2(w / 2 - 2 + 2 - 2, position - 15), 14.f, g_Options.showkeys_color, false, false, g_Calibri);

	if (GetAsyncKeyState(VK_SPACE)) //jump
		render::Get().RenderText("j", ImVec2(w / 2 + 18 - 2 + 3, position - 17), 14.f, g_Options.showkeys_color, false, false, g_Calibri);

	if (GetAsyncKeyState(0x41)) //a
		render::Get().RenderText("a", ImVec2(w / 2 - 18 - 2 + 2, position - 15 + 15), 14.f, g_Options.showkeys_color, false, false, g_Calibri);

	if (GetAsyncKeyState(0x53)) //s
		render::Get().RenderText("s", ImVec2(w / 2 - 2 + 2, position - 15 + 15), 14.f, g_Options.showkeys_color, false, false, g_Calibri);

	if (GetAsyncKeyState(0x44)) //d
		render::Get().RenderText("d", ImVec2(w / 2 + 18 - 2 + 2, position - 15 + 15), 14.f, g_Options.showkeys_color, false, false, g_Calibri);
}

void Visuals::Skybox()
{
	if (!g_Options.skyboxchanger)
		return;

	auto LoadSkyBox = reinterpret_cast<void(__fastcall*)(const char*)>(Utils::PatternScan(("engine.dll"), ("55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45")));
	switch (g_Options.skyboxtype)
	{
		case 0:
			LoadSkyBox("cs_tibet");
			break;
		case 1:
			LoadSkyBox("cs_baggage_skybox_");
			break;
		case 2:
			LoadSkyBox("embassy");
			break;
		case 3:
			LoadSkyBox("italy");
			break;
		case 4:
			LoadSkyBox("jungle");
			break;
		case 5:
			LoadSkyBox("office");
			break;
		case 6:
			LoadSkyBox("sky_cs15_daylight01_hdr");
			break;
		case 7:
			LoadSkyBox("vertigoblue_hdr");
			break;
		case 8:
			LoadSkyBox("sky_cs15_daylight02_hdr");
			break;
		case 9:
			LoadSkyBox("vertigo");
			break;
		case 10:
			LoadSkyBox("sky_day02_05_hdr");
			break;
		case 11:
			LoadSkyBox("nukeblank");
			break;
		case 12:
			LoadSkyBox("sky_venice");
			break;
		case 13:
			LoadSkyBox("sky_cs15_daylight03_hdr");
			break;
		case 14:
			LoadSkyBox("sky_cs15_daylight04_hdr");
			break;
		case 15:
			LoadSkyBox("sky_csgo_cloudy01");
			break;
		case 16:
			LoadSkyBox("sky_csgo_night02");
			break;
		case 17:
			LoadSkyBox("sky_csgo_night02b");
			break;
		case 18:
			LoadSkyBox("sky_csgo_night_flat");
			break;
		case 19:
			LoadSkyBox("sky_dust");
			break;
		case 20:
			LoadSkyBox("vietnam");
			break;
	}
}

void Visuals::modelChanger(ClientFrameStage_t stage)
{
	static int originalIdx = 0;

	if (!g_LocalPlayer) {
		originalIdx = 0;
		return;
	}

	constexpr auto getModel = [](int team) constexpr noexcept -> const char* {
		constexpr std::array models{
			"models/player/custom_player/legacy/ctm_diver_varianta.mdl", // Cmdr. Davida 'Goggles' Fernandez | SEAL Frogman
			"models/player/custom_player/legacy/ctm_diver_variantb.mdl", // Cmdr. Frank 'Wet Sox' Baroud | SEAL Frogman
			"models/player/custom_player/legacy/ctm_diver_variantc.mdl", // Lieutenant Rex Krikey | SEAL Frogman
			"models/player/custom_player/legacy/ctm_fbi_varianth.mdl", // Michael Syfers | FBI Sniper
			"models/player/custom_player/legacy/ctm_fbi_variantf.mdl", // Operator | FBI SWAT
			"models/player/custom_player/legacy/ctm_fbi_variantb.mdl", // Special Agent Ava | FBI
			"models/player/custom_player/legacy/ctm_fbi_variantg.mdl", // Markus Delrow | FBI HRT
			"models/player/custom_player/legacy/ctm_gendarmerie_varianta.mdl", // Sous-Lieutenant Medic | Gendarmerie Nationale
			"models/player/custom_player/legacy/ctm_gendarmerie_variantb.mdl", // Chem-Haz Capitaine | Gendarmerie Nationale
			"models/player/custom_player/legacy/ctm_gendarmerie_variantc.mdl", // Chef d'Escadron Rouchard | Gendarmerie Nationale
			"models/player/custom_player/legacy/ctm_gendarmerie_variantd.mdl", // Aspirant | Gendarmerie Nationale
			"models/player/custom_player/legacy/ctm_gendarmerie_variante.mdl", // Officer Jacques Beltram | Gendarmerie Nationale
			"models/player/custom_player/legacy/ctm_sas_variantg.mdl", // D Squadron Officer | NZSAS
			"models/player/custom_player/legacy/ctm_sas_variantf.mdl", // B Squadron Officer | SAS
			"models/player/custom_player/legacy/ctm_st6_variante.mdl", // Seal Team 6 Soldier | NSWC SEAL
			"models/player/custom_player/legacy/ctm_st6_variantg.mdl", // Buckshot | NSWC SEAL
			"models/player/custom_player/legacy/ctm_st6_varianti.mdl", // Lt. Commander Ricksaw | NSWC SEAL
			"models/player/custom_player/legacy/ctm_st6_variantj.mdl", // 'Blueberries' Buckshot | NSWC SEAL
			"models/player/custom_player/legacy/ctm_st6_variantk.mdl", // 3rd Commando Company | KSK
			"models/player/custom_player/legacy/ctm_st6_variantl.mdl", // 'Two Times' McCoy | TACP Cavalry
			"models/player/custom_player/legacy/ctm_st6_variantm.mdl", // 'Two Times' McCoy | USAF TACP
			"models/player/custom_player/legacy/ctm_st6_variantn.mdl", // Primeiro Tenente | Brazilian 1st Battalion
			"models/player/custom_player/legacy/ctm_swat_variante.mdl", // Cmdr. Mae 'Dead Cold' Jamison | SWAT
			"models/player/custom_player/legacy/ctm_swat_variantf.mdl", // 1st Lieutenant Farlow | SWAT
			"models/player/custom_player/legacy/ctm_swat_variantg.mdl", // John 'Van Healen' Kask | SWAT
			"models/player/custom_player/legacy/ctm_swat_varianth.mdl", // Bio-Haz Specialist | SWAT
			"models/player/custom_player/legacy/ctm_swat_varianti.mdl", // Sergeant Bombson | SWAT
			"models/player/custom_player/legacy/ctm_swat_variantj.mdl", // Chem-Haz Specialist | SWAT
			//"models/player/custom_player/legacy/ctm_swat_variantk.mdl" // Lieutenant 'Tree Hugger' Farlow | SWAT
			"models/player/custom_player/legacy/tm_professional_varj.mdl", // Getaway Sally | The Professionals
			"models/player/custom_player/legacy/tm_professional_vari.mdl", // Number K | The Professionals
			"models/player/custom_player/legacy/tm_professional_varh.mdl", // Little Kev | The Professionals
			"models/player/custom_player/legacy/tm_professional_varg.mdl", // Safecracker Voltzmann | The Professionals
			"models/player/custom_player/legacy/tm_professional_varf5.mdl", // Bloody Darryl The Strapped | The Professionals
			"models/player/custom_player/legacy/tm_professional_varf4.mdl", // Sir Bloody Loudmouth Darryl | The Professionals
			"models/player/custom_player/legacy/tm_professional_varf3.mdl", // Sir Bloody Darryl Royale | The Professionals
			"models/player/custom_player/legacy/tm_professional_varf2.mdl", // Sir Bloody Skullhead Darryl | The Professionals
			"models/player/custom_player/legacy/tm_professional_varf1.mdl", // Sir Bloody Silent Darryl | The Professionals
			"models/player/custom_player/legacy/tm_professional_varf.mdl", // Sir Bloody Miami Darryl | The Professionals
			"models/player/custom_player/legacy/tm_phoenix_varianti.mdl", // Street Soldier | Phoenix
			"models/player/custom_player/legacy/tm_phoenix_varianth.mdl", // Soldier | Phoenix
			"models/player/custom_player/legacy/tm_phoenix_variantg.mdl", // Slingshot | Phoenix
			"models/player/custom_player/legacy/tm_phoenix_variantf.mdl", // Enforcer | Phoenix
			"models/player/custom_player/legacy/tm_leet_variantj.mdl", // Mr. Muhlik | Elite Crew
			"models/player/custom_player/legacy/tm_leet_varianti.mdl", // Prof. Shahmat | Elite Crew
			"models/player/custom_player/legacy/tm_leet_varianth.mdl", // Osiris | Elite Crew
			"models/player/custom_player/legacy/tm_leet_variantg.mdl", // Ground Rebel | Elite Crew
			"models/player/custom_player/legacy/tm_leet_variantf.mdl", // The Elite Mr. Muhlik | Elite Crew
			"models/player/custom_player/legacy/tm_jungle_raider_variantf2.mdl", // Trapper | Guerrilla Warfare
			"models/player/custom_player/legacy/tm_jungle_raider_variantf.mdl", // Trapper Aggressor | Guerrilla Warfare
			"models/player/custom_player/legacy/tm_jungle_raider_variante.mdl", // Vypa Sista of the Revolution | Guerrilla Warfare
			"models/player/custom_player/legacy/tm_jungle_raider_variantd.mdl", // Col. Mangos Dabisi | Guerrilla Warfare
			"models/player/custom_player/legacy/tm_jungle_raider_variantñ.mdl", // Arno The Overgrown | Guerrilla Warfare
			"models/player/custom_player/legacy/tm_jungle_raider_variantb2.mdl", // 'Medium Rare' Crasswater | Guerrilla Warfare
			"models/player/custom_player/legacy/tm_jungle_raider_variantb.mdl", // Crasswater The Forgotten | Guerrilla Warfare
			"models/player/custom_player/legacy/tm_jungle_raider_varianta.mdl", // Elite Trapper Solman | Guerrilla Warfare
			"models/player/custom_player/legacy/tm_balkan_varianth.mdl", // 'The Doctor' Romanov | Sabre
			"models/player/custom_player/legacy/tm_balkan_variantj.mdl", // Blackwolf | Sabre
			"models/player/custom_player/legacy/tm_balkan_varianti.mdl", // Maximus | Sabre
			"models/player/custom_player/legacy/tm_balkan_variantf.mdl", // Dragomir | Sabre
			"models/player/custom_player/legacy/tm_balkan_variantg.mdl", // Rezan The Ready | Sabre
			"models/player/custom_player/legacy/tm_balkan_variantk.mdl", // Rezan the Redshirt | Sabre
			"models/player/custom_player/legacy/tm_balkan_variantl.mdl" // Dragomir | Sabre Footsoldier
		};

		switch (team) {
			case 2: return static_cast<std::size_t>(g_Options.playerModelT - 1) < models.size() ? models[g_Options.playerModelT - 1] : nullptr;
			case 3: return static_cast<std::size_t>(g_Options.playerModelCT - 1) < models.size() ? models[g_Options.playerModelCT - 1] : nullptr;
		default: return nullptr;
		}
	};

	if (const auto model = getModel(g_LocalPlayer->m_iTeamNum())) {
		if (stage == FRAME_RENDER_START)
			originalIdx = g_LocalPlayer->m_nModelIndex();

		const auto idx = stage == FRAME_RENDER_END && originalIdx ? originalIdx : g_MdlInfo->GetModelIndex(model);

		g_LocalPlayer->SetModelIndex(idx);

		if (const auto ragdoll = g_LocalPlayer->get_entity_from_handle(g_LocalPlayer->m_hRagdoll()))
			ragdoll->SetModelIndex(idx);
	}
}

void Visuals::AddToDrawList() {
	if (g_Options.showindicators)
		RenderBinds();
	if (g_Options.showkeys)
		RenderKeystrokes();
	if (g_Options.velocity)
		render_speed();

	for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i) {
		auto entity = C_BaseEntity::GetEntityByIndex(i);

		if (!entity)
			continue;

		if (entity == g_LocalPlayer && !g_Input->m_fCameraInThirdPerson)
			continue;

		if (i <= g_GlobalVars->maxClients) {
			auto player = Player();
			if (player.Begin((C_BasePlayer*)entity)) {
				//if (g_Options.esp_player_snaplines) player.RenderSnapline();
				if (g_Options.esp_player_boxes)     player.RenderBox(entity);
				if (g_Options.esp_player_weapons)   player.RenderWeaponName(entity);
				if (g_Options.esp_player_names)     player.RenderName(entity);
				if (g_Options.esp_player_health)    player.RenderHealth(entity);
				//if (g_Options.esp_player_armour)    player.RenderArmour();
				if (g_Options.esp_player_flags)		player.Flags(entity);
			}
		}
		if (g_Options.esp_dropped_weapons && entity->IsWeapon())
			RenderWeapon(static_cast<C_BaseCombatWeapon*>(entity));
		if (g_Options.esp_dropped_weapons && entity->IsDefuseKit())
			RenderDefuseKit(entity);
		if (entity->IsPlantedC4() && g_Options.esp_planted_c4)
			RenderPlantedC4(entity);
	}
}

/*credits: https://yougame.biz/threads/216013/ */
bool did_eb = false;

void Visuals::edgebug_notify(float unpred_z, int unpred_flags)
{
	float z_velocity = floor(g_LocalPlayer->m_vecVelocity().z);

	if (unpred_z >= -7 || !g_LocalPlayer || z_velocity != -7 || unpred_flags & FL_ONGROUND || g_LocalPlayer->m_nMoveType() == MOVETYPE_NOCLIP)
		did_eb = false;
	else
	{
		did_eb = true;

		auto g_ChatElement = FindHudElement<CHudChat>("CHudChat");

		if (g_Options.edge_bug && g_Options.edgebugnotify) {
			g_ChatElement->ChatPrintf(0, 0, std::string("").
				append(" \x01").
				append("[Cheat name]").
				append(" \x01").
				append("edgebug").c_str());
		}

		if (g_Options.edge_bug && g_Options.edgebugeffect)
			g_LocalPlayer->m_flHealthShotBoost() = g_GlobalVars->curtime + 0.5f;

		if (g_Options.edge_bug && g_Options.edgebugsound)
			g_VGuiSurface->PlaySound_("survival\\money_collect_01.wav");
	}
}

void Visuals::disablePostprocessing() {
	static auto mat_postprocess_enable = g_CVar->FindVar("mat_postprocess_enable");

	if (g_Options.disable_postprocessing) {
		if (mat_postprocess_enable->GetInt() != 0)
			mat_postprocess_enable->SetValue(0);
	}
	else {
		if (mat_postprocess_enable->GetInt() != 1)
			mat_postprocess_enable->SetValue(1);
	}
}

void Visuals::disableShadows() {
	static ConVar* shadows = g_CVar->FindVar("cl_csm_enabled");

	if (g_Options.disable_shadows) {
		if (shadows->GetInt() != false)
			shadows->SetValue(false);
	}
	else {
		if (shadows->GetInt() != true)
			shadows->SetValue(true);
	}
}

void Visuals::disable3dSky() {
	static ConVar* sky = g_CVar->FindVar("r_3dsky");

	if (g_Options.disable_3dsky) {
		if (sky->GetInt() != false)
			sky->SetValue(false);
	}
	else {
		if (sky->GetInt() != true)
			sky->SetValue(true);
	}
}

void Visuals::Sunset() {
	if (&g_Options.sunset)
	{
		g_CVar->FindVar("cl_csm_rot_override")->SetValue(1);
		g_CVar->FindVar("cl_csm_rot_x")->SetValue(g_Options.sunsetx);
		g_CVar->FindVar("cl_csm_rot_y")->SetValue(g_Options.sunsety);
		g_CVar->FindVar("cl_csm_rot_z")->SetValue(g_Options.sunsetz);
	}
}