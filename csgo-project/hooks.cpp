#include "hooks.hpp"
#include <intrin.h>  

#include "render.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "helpers/prediction.hpp"
#include "helpers/input.hpp"
#include "helpers/utils.hpp"
#include "features/aimbot.hpp"
#include "features/bhop.hpp"
#include "features/chams.hpp"
#include "features/visuals.hpp"
#include "features/glow.hpp"
#include "features/skins.h"
#include "features/nosmoke.hpp"
#include "features/movement.hpp"

#include "minhook.hpp"
#pragma comment(lib, "minhook.lib")

#pragma intrinsic(_ReturnAddress)
//--------------------------------------------------------------------------------
void Hooks::Initialize() {
	ConVar* sv_cheats_con = g_CVar->FindVar("sv_cheats");
	auto rpe_targ = (void*)((uintptr_t)g_ClientState + 0x8);

	const auto gv_endscene = reinterpret_cast<void*>(get_virtual(g_D3DDevice9, index::EndScene));
	const auto gv_reset = reinterpret_cast<void*>(get_virtual(g_D3DDevice9, index::Reset));
	const auto gv_crmove = reinterpret_cast<void*>(get_virtual(g_ClientMode, index::CreateMove));
	const auto gv_painttr = reinterpret_cast<void*>(get_virtual(g_VGuiPanel, index::PaintTraverse));
	const auto gv_fsn = reinterpret_cast<void*>(get_virtual(g_CHLClient, index::FrameStageNotify));
	const auto gv_sound = reinterpret_cast<void*>(get_virtual(g_EngineSound, index::EmitSound1));
	const auto gv_lcursor = reinterpret_cast<void*>(get_virtual(g_VGuiSurface, index::LockCursor));
	const auto gv_dme = reinterpret_cast<void*>(get_virtual(g_MdlRender, index::DrawModelExecute));
	const auto gv_pse = reinterpret_cast<void*>(get_virtual(g_ClientMode, index::DoPostScreenSpaceEffects));
	const auto gv_ovview = reinterpret_cast<void*>(get_virtual(g_ClientMode, index::OverrideView));
	const auto gv_event = reinterpret_cast<void*>(get_virtual(g_GameEvents, index::FireEvent));
	const auto gv_cheats = reinterpret_cast<void*>(get_virtual(sv_cheats_con, index::SvCheatsGetBool));
	const auto gv_aspect_ratio = reinterpret_cast<void*>(get_virtual(g_EngineClient, index::AspectRatio));
	const auto gv_suppress_list = reinterpret_cast<void*>(get_virtual(g_SpatialPartition, index::SuppressLists));
	const auto gv_rpe = reinterpret_cast<void*>(get_virtual(rpe_targ, index::ReadPacketEntities));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize hooks.");

	MH_CreateHook(gv_endscene, &end_scene::hook, reinterpret_cast<void**>(&end_scene::o_end_scene));
	MH_CreateHook(gv_reset, &reset::hook, reinterpret_cast<void**>(&reset::o_reset));
	MH_CreateHook(gv_crmove, &createmove::hook, reinterpret_cast<void**>(&createmove::o_crmove));
	MH_CreateHook(gv_painttr, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse::o_paint));
	MH_CreateHook(gv_fsn, &fsn::hook, reinterpret_cast<void**>(&fsn::o_fsn));
	MH_CreateHook(gv_sound, &emit_sound::hook, reinterpret_cast<void**>(&emit_sound::o_sound));
	MH_CreateHook(gv_lcursor, &lock_cursor::hook, reinterpret_cast<void**>(&lock_cursor::o_lock));
	MH_CreateHook(gv_dme, &dme::hook, reinterpret_cast<void**>(&dme::o_dme));
	MH_CreateHook(gv_pse, &dpse::hook, reinterpret_cast<void**>(&dpse::o_effects));
	MH_CreateHook(gv_ovview, &override_view::hook, reinterpret_cast<void**>(&override_view::o_ovview));
	MH_CreateHook(gv_event, &fire_event::hook, reinterpret_cast<void**>(&fire_event::o_events));
	MH_CreateHook(gv_cheats, &cheats::hook, reinterpret_cast<void**>(&cheats::o_cheats));
	MH_CreateHook(gv_aspect_ratio, &aspect_ratio::hook, reinterpret_cast<void**>(&aspect_ratio::o_aspect_ratio));
	MH_CreateHook(gv_suppress_list, &suppress_list::hook, reinterpret_cast<void**>(&suppress_list::o_suppress_list));
	MH_CreateHook(gv_rpe, &rpe::hook, reinterpret_cast<void**>(&rpe::o_rpe));

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	sequence_hook = new recv_prop_hook(C_BaseViewModel::m_nSequence(), hkRecvProxy);
}
//--------------------------------------------------------------------------------
void Hooks::Shutdown() {
	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);

	sequence_hook->~recv_prop_hook();
	m_pFlashDuration->~recv_prop_hook();
	Glow::Get().Shutdown();
}
//--------------------------------------------------------------------------------
long __stdcall Hooks::end_scene::hook(IDirect3DDevice9* device) {
	IDirect3DVertexDeclaration9* vertDec;
	IDirect3DVertexShader9* vertShader;
	device->GetVertexDeclaration(&vertDec);
	device->GetVertexShader(&vertShader);

	DWORD colorwrite, srgbwrite;
	IDirect3DVertexDeclaration9* vert_dec = nullptr;
	IDirect3DVertexShader9* vert_shader = nullptr;
	DWORD dwOld_D3DRS_COLORWRITEENABLE = NULL;
	device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
	device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

	device->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	device->GetVertexDeclaration(&vert_dec);
	device->GetVertexShader(&vert_shader);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto esp_drawlist = render::Get().RenderScene();
	Menu::Get().Render();
	ImGui::Render(esp_drawlist);

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	device->SetVertexDeclaration(vertDec);
	device->SetVertexShader(vertShader);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
	device->SetVertexDeclaration(vert_dec);
	device->SetVertexShader(vert_shader);

	return o_end_scene(device);
}
//--------------------------------------------------------------------------------
long __stdcall Hooks::reset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters) {
	Menu::Get().OnDeviceLost();

	auto hr = o_reset(device, presentation_parameters);
	if (hr >= 0) Menu::Get().OnDeviceReset();

	return hr;
}
//--------------------------------------------------------------------------------
bool __stdcall Hooks::createmove::hook(float input_sample_frametime, CUserCmd* cmd) {
	o_crmove(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number || g_Unload) return false;

	if (Menu::Get().IsVisible())
		cmd->buttons &= ~IN_ATTACK;

	if (g_Options.misc_bhop)
		BunnyHop::OnCreateMove(cmd);

	if (GetAsyncKeyState(g_Options.autostrafe_key))
		BunnyHop::AutoStafe(cmd);

	if (g_Options.no_flash)
		g_LocalPlayer->m_flFlashDuration() = 0.f;

	if (g_Options.forcecrosshair && !g_LocalPlayer->m_bIsScoped())
		g_CVar->FindVar("weapon_debug_spread_show")->SetValue(3);
	else
		g_CVar->FindVar("weapon_debug_spread_show")->SetValue(0);

	static std::string tag = " thr1lltr1p ";
	if (g_LocalPlayer && g_Options.clantag_spammer)
		Utils::SetClantag(tag.data());
	else
		Utils::SetClantag(" ");

	if (g_Options.misc_showranks && InputSys::Get().IsKeyDown(VK_TAB)) // rank revealer will work even after unhooking, idk how to "hide" ranks  again
		g_CHLClient->DispatchUserMessage(CS_UM_ServerRankRevealAll, 0, 0, nullptr);

	static auto prediction = new PredictionSystem();
	auto flags = g_LocalPlayer->m_fFlags();
	float eb = floor(g_LocalPlayer->m_vecVelocity().z);

	prediction->StartPrediction(cmd);
	{
		g_Legitbot->Run(cmd);
		Visuals::Get().edgebug_notify(eb, flags);

		if (g_Options.edgejump && GetAsyncKeyState(g_Options.ej_hotkey))
		{
			if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND) && (flags & FL_ONGROUND))
				cmd->buttons |= IN_JUMP;

			if (!(g_LocalPlayer->m_fFlags() & FL_ONGROUND) && g_Options.edge_jump_duck_in_air && !(cmd->buttons |= IN_DUCK))
				cmd->buttons |= IN_DUCK;
		}

		if ((g_LocalPlayer->m_fFlags() & FL_ONGROUND) && g_LocalPlayer->IsAlive()) if (g_Options.edge_bug && GetAsyncKeyState(g_Options.edge_bug_key)) cmd->buttons = 4;
		movement::Get().jumpbug(cmd);
	}
	prediction->EndPrediction();

	return false;
}
//--------------------------------------------------------------------------------
bool __fastcall Hooks::fire_event::hook(void* ecx, void* edx, IGameEvent* event) {
	if (strstr(event->GetName(), "player_death")) {
		if (g_EngineClient->GetPlayerForUserID(event->GetInt("attacker")) == g_EngineClient->GetLocalPlayer()) {
			/*auto& weapon = g_LocalPlayer->m_hActiveWeapon();
			if (weapon) {
				auto& skin_data = Cfg.skins.m_items[weapon->m_Item().m_iItemDefinitionIndex()];
				auto& stat_track = Cfg.skins.statrack_items[weapon->m_Item().m_iItemDefinitionIndex()];
				if (skin_data.stat_trak) {
					stat_track.statrack_new.counter++;
					weapon->m_nFallbackStatTrak() = stat_track.statrack_new.counter;
					weapon->GetClientNetworkable()->PostDataUpdate(0);
					weapon->GetClientNetworkable()->OnDataChanged(0);
				}
			}

			const auto icon_override = Skins::Get().GetIconOverride(event->GetString("weapon"));
			if (icon_override) event->GetString("weapon", icon_override);*/
		}
	}
	if (!strcmp(event->GetName(), "game_newmap")) {
		//ConVar* sv_skyname = g_CVar->FindVar("sv_skyname");
		//Misc::Get().OldSkyname = sv_skyname->GetString();
		//Misc::Get().NightmodeDone = false;
	}
	return o_events(ecx, edx, event);
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::paint_traverse::hook(void* _this, int edx, vgui::VPANEL panel, bool force_repaint, bool allow_force) {
	static auto panelId = vgui::VPANEL{ 0 };

	o_paint(g_VGuiPanel, edx, panel, force_repaint, allow_force);

	if (!panelId) {
		const auto panelName = g_VGuiPanel->GetName(panel);
		if (!strcmp(panelName, "FocusOverlayPanel"))
			panelId = panel;
	}
	else if (panelId == panel) {
		static bool bSkip = false;
		bSkip = !bSkip;

		if (bSkip) return;
		
		render::Get().BeginScene();
		Menu::Get().Watermark();
	}
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::fsn::hook(void* _this, int edx, ClientFrameStage_t stage) {
	if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		o_fsn(g_CHLClient, edx, stage);

	if (stage == FRAME_RENDER_START) {
		Visuals::Get().disablePostprocessing();
		Visuals::Get().disableShadows();
		Visuals::Get().disable3dSky();
		Visuals::Get().Sunset();
	}

	Visuals::Get().modelChanger(stage);

	o_fsn(g_CHLClient, edx, stage);
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::emit_sound::hook(void* _this, int edx, IRecipientFilter& filter, int ent_index, int channel, const char* sound_entry, unsigned int sound_entry_hash, const char* sample, float volume, int seed, float attenuation, int flags, int pitch, const Vector* origin, const Vector* direction, void* utl_vec_origins, bool update_positions, float sound_time, int speaker_entity, int unk) {

	if (!g_Options.misc_autoaccept)
		o_sound(g_EngineSound, edx, filter, ent_index, channel, sound_entry, sound_entry_hash, sample, volume, seed, attenuation, flags, pitch, origin, direction, utl_vec_origins, update_positions, sound_time, speaker_entity, unk);

	if (!strcmp(sound_entry, "UIPanorama.popup_accept_match_beep")) {
		static auto fnAccept = reinterpret_cast<bool(__stdcall*)(const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));

		if (fnAccept) {
			fnAccept("");
			auto window = FindWindowW(L"Valve001", NULL);
			FLASHWINFO flash{ sizeof(FLASHWINFO), window, FLASHW_TRAY | FLASHW_TIMERNOFG, 0, 0 };
			FlashWindowEx(&flash);
		}
	}
	o_sound(g_EngineSound, edx, filter, ent_index, channel, sound_entry, sound_entry_hash, sample, volume, seed, attenuation, flags, pitch, origin, direction, utl_vec_origins, update_positions, sound_time, speaker_entity, unk);
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::lock_cursor::hook(void* _this) {
	if (Menu::Get().IsVisible()) {
		g_VGuiSurface->UnlockCursor();
		g_InputSystem->ResetInputState();
		return;
	}
	o_lock(g_VGuiSurface);
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::dme::hook(void* _this, int edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* custom_bone_to_world) {
	if (g_MdlRender->IsForcedMaterialOverride())
		return o_dme(_this, edx, ctx, state, info, custom_bone_to_world);

	Chams::Get().OnDrawModelExecute(ctx, state, info, custom_bone_to_world);
	o_dme(_this, edx, ctx, state, info, custom_bone_to_world);
	g_MdlRender->ForcedMaterialOverride(nullptr);
}
//--------------------------------------------------------------------------------
int __fastcall Hooks::dpse::hook(void* _this, int edx, int a1) {
	if (g_LocalPlayer && g_Options.glow_enabled)
		Glow::Get().Run();

	return o_effects(g_ClientMode, edx, a1);
}
//--------------------------------------------------------------------------------
void __fastcall Hooks::override_view::hook(void* _this, int edx, CViewSetup* vs_view) {
	if (g_EngineClient->IsInGame() && vs_view) {
		Visuals::Get().thirdperson();
	}

	o_ovview(g_ClientMode, edx, vs_view);
}
//--------------------------------------------------------------------------------
auto dwCAM_Think = Utils::PatternScan(GetModuleHandleW(L"client.dll"), "85 C0 75 30 38 86");
typedef bool(__thiscall* svc_get_bool_t)(PVOID);
bool __fastcall Hooks::cheats::hook(PVOID convar, void* edx) {
	if (!o_cheats) return false;

	if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think)) return true;
	return o_cheats(convar, edx);
}
//--------------------------------------------------------------------------------	
float __fastcall Hooks::aspect_ratio::hook(void* ecx, void* edx, int32_t width, int32_t height) {
	if (!g_Options.aspectratio) o_aspect_ratio(ecx, edx, width, height);
	else return ((float)g_Options.aspectratio_amount / 10);

	o_aspect_ratio(ecx, edx, width, height);
}
//--------------------------------------------------------------------------------
static auto random_sequence(const int low, const int high) -> int {
	return rand() % (high - low + 1) + low;
}
//--------------------------------------------------------------------------------
static auto fix_animation(const char* model, const int sequence) -> int {
	enum ESequence {
		SEQUENCE_DEFAULT_DRAW = 0,
		SEQUENCE_DEFAULT_IDLE1 = 1,
		SEQUENCE_DEFAULT_IDLE2 = 2,
		SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
		SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
		SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
		SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
		SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
		SEQUENCE_DEFAULT_LOOKAT01 = 12,
		SEQUENCE_BUTTERFLY_DRAW = 0,
		SEQUENCE_BUTTERFLY_DRAW2 = 1,
		SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
		SEQUENCE_BUTTERFLY_LOOKAT03 = 15,
		SEQUENCE_FALCHION_IDLE1 = 1,
		SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
		SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
		SEQUENCE_FALCHION_LOOKAT01 = 12,
		SEQUENCE_FALCHION_LOOKAT02 = 13,
		SEQUENCE_DAGGERS_IDLE1 = 1,
		SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
		SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
		SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
		SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,
		SEQUENCE_BOWIE_IDLE1 = 1,
	};
	if (strstr(model, "models/weapons/v_knife_butterfly.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
		default: return sequence + 1;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_falchion_advanced.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random_sequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default: return sequence - 1;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_push.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
			return random_sequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return random_sequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT01:
			return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default: return sequence + 2;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_survival_bowie.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_BOWIE_IDLE1;
		default: return sequence - 1;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_ursus.mdl") || strstr(model, "models/weapons/v_knife_canis.mdl") || strstr(model, "models/weapons/v_knife_outdoor.mdl") || strstr(model, "models/weapons/v_knife_cord.mdl") || strstr(model, "models/weapons/v_knife_skeleton.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
			return random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
		default: return sequence + 1;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_widowmaker.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(14, 15);
		default: return sequence;
		}
	}
	else if (strstr(model, "models/weapons/v_knife_stiletto.mdl")) {
		switch (sequence) {
		case SEQUENCE_DEFAULT_LOOKAT01:
			return random_sequence(12, 13);
		default: return sequence;
		}
	}
	else return sequence;
}
//--------------------------------------------------------------------------------
void Hooks::hkRecvProxy(const CRecvProxyData* pData, void* entity, void* output) {
	static auto original_fn = Hooks::sequence_hook->get_original_function();
	const auto local = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));
	if (local && local->IsAlive()) {
		const auto proxy_data = const_cast<CRecvProxyData*>(pData);
		const auto view_model = static_cast<C_BaseViewModel*>(entity);
		if (view_model && view_model->m_hOwner() && view_model->m_hOwner().IsValid()) {
			const auto owner = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntityFromHandle(view_model->m_hOwner()));
			if (owner == g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer())) {
				const auto view_model_weapon_handle = view_model->m_hWeapon();
				if (view_model_weapon_handle.IsValid()) {
					const auto view_model_weapon = static_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(view_model_weapon_handle));
					if (view_model_weapon) {
						if (k_weapon_info.count(view_model_weapon->m_Item().m_iItemDefinitionIndex())) {
							auto original_sequence = proxy_data->m_Value.m_Int;
							const auto override_model = k_weapon_info.at(view_model_weapon->m_Item().m_iItemDefinitionIndex()).model;
							proxy_data->m_Value.m_Int = fix_animation(override_model, proxy_data->m_Value.m_Int);
						}
					}
				}
			}
		}
	}
	original_fn(pData, entity, output);
}
//--------------------------------------------------------------------------------
void __stdcall Hooks::suppress_list::hook(int a2, bool a3) {
	static auto OnRenderStart_Return = Utils::PatternScan(GetModuleHandleA("client.dll"), "FF 50 40 8B 1D ? ? ? ?") + 0x3;
	static auto FrameNetUpdateEnd_Return = Utils::PatternScan(GetModuleHandleA("client.dll"), "5F 5E 5D C2 04 00 83 3D ? ? ? ? ?");

	if (g_LocalPlayer && g_LocalPlayer->IsAlive()) {
		if (_ReturnAddress() == OnRenderStart_Return) {
		}
		else if (_ReturnAddress() == FrameNetUpdateEnd_Return) {
			skins::on_frame_stage_notify(true);
		}
		else if (ClientFrameStage_t::FRAME_NET_UPDATE_START)
			NoSmoke::Get().OnFrameStageNotify();
	}

	o_suppress_list(g_SpatialPartition, a2, a3);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void __fastcall Hooks::rpe::hook(void* client_state, void* edx, void* ent_info) {
	o_rpe(client_state, ent_info);
	skins::on_frame_stage_notify(false);
}
//--------------------------------------------------------------------------------	