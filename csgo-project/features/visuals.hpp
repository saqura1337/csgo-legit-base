#pragma once

#include "../singleton.hpp"

#include "../render.hpp"
#include "../helpers/math.hpp"
#include "../valve_sdk/csgostructs.hpp"

class CHudChat
{
public:
	void ChatPrintf(int iPlayerIndex, int iFilter, const char* fmt, ...)
	{
		CallVFunction<void(__cdecl*)(void*, int, int, const char*, ...)>(this, 27)(this, iPlayerIndex, iFilter, fmt);
	}
};

class Visuals : public Singleton<Visuals>
{
	friend class Singleton<Visuals>;

	CRITICAL_SECTION cs;

	Visuals();
	~Visuals();
public:
	class Player
	{
	public:
		struct
		{
			C_BasePlayer* pl;
			bool          is_enemy;
			bool          is_visible;
			Color         clr;
			Vector        head_pos;
			Vector        feet_pos;
			RECT          bbox;
		} ctx;

		bool Begin(C_BasePlayer * pl);
		void RenderBox(C_BaseEntity* pl);
		void RenderName(C_BaseEntity* pl);
		void RenderWeaponName(C_BaseEntity* pl);
		void RenderHealth(C_BaseEntity* pl);
		void RenderArmour();
		void RenderSnapline();
		void Flags(C_BaseEntity* pl);
	};
	void RenderWeapon(C_BaseCombatWeapon* ent);
	void RenderDefuseKit(C_BaseEntity* ent);
	void RenderPlantedC4(C_BaseEntity* ent);
	void thirdperson();
	void edgebug_notify(float unpred_z, int unpred_flags);
	void RenderBinds();
	void RenderKeystrokes();
	void modelChanger(ClientFrameStage_t stage);
	void disablePostprocessing();
	void disableShadows();
	void disable3dSky();
	void Sunset();
	void Skybox();
public:
	void AddToDrawList();
	void Render();
};
