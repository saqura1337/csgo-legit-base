#include "chams.hpp"
#include <fstream>

#include "../valve_sdk/csgostructs.hpp"
#include "../options.hpp"
#include "../hooks.hpp"
#include "../helpers/input.hpp"
#include "../helpers/math.hpp"

Chams::Chams() {
	KeyValues* Overlay = new KeyValues("Overlay");
	KeyValues* Animated = new KeyValues("Animated");
	Overlay->LoadFromBuffer(Overlay, "Overlay", R"#("VertexLitGeneric" {
 
	"$additive" "1"
	"$envmap" "models/effects/cube_white"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "0.6"
	})#");
	Animated->LoadFromBuffer(Animated, "Animated", R"#("VertexLitGeneric" {
    "$basetexture" "VGUI/white_additive"
    "$bumpmap" "de_nuke/hr_nuke/pool_water_normals_002"
    "$bumptransform" "center 0.5 0.5 scale 0.25 0.25 rotate 0.0 translate 0.0 0.0"
	"$ignorez" "0"
	"$nofog" "1"
	"$model" "1"
	"$color2" "[1.0, 1.0, 1.0]"
	"$halflambert" "1"
	"$envmap" "env_cubemap"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1.0"
	"$envmapfresnelminmaxexp" "[1.0, 1.0, 1.0]"
	"$phong" "1"
	"$phongexponent" "1024"
	"$phongboost" "4.0"
	"$phongfresnelranges" "[1.0, 1.0, 1.0]"
	"$rimlight" "1"
	"$rimlightexponent" "4.0"
	"$rimlightboost" "2.0"
	"Proxies"
        {
            "TextureScroll"
            {
                "textureScrollVar" "$bumptransform"
                "textureScrollRate" "0.25"
                "textureScrollAngle" "0.0"
            }
        }
	})#");
	materialRegular = g_MatSystem->FindMaterial("debug/debugambientcube", "Model textures");
	materialFlat = g_MatSystem->FindMaterial("debug/debugdrawflat", "Model textures");
	materialGlass = g_MatSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", "Model textures");
	materialGlow = g_MatSystem->FindMaterial("dev/glow_armsrace", "Model textures");
	materialOverlay = g_MatSystem->CreateMaterial("Overlay", Overlay);
	materialOverlay->IncrementReferenceCount();
	materialAnimated = g_MatSystem->CreateMaterial("Animated", Animated);
	materialAnimated->IncrementReferenceCount();
}

Chams::~Chams() {
}


void Chams::OverrideMaterial(bool ignorez, int type, const Color& rgba)
{
	IMaterial* material = nullptr;
	switch (type)
	{
	case 0: material = materialRegular; break;
	case 1: material = materialFlat; break;
	case 2: material = materialGlass; break;
	case 3: material = materialGlow; break;
	case 4: material = materialOverlay; break;
	case 5: material = materialAnimated; break;
	}
	bool bFound = false;
	IMaterialVar* pMatVar = material->FindVar("$envmaptint", &bFound);
	if (bFound)
		pMatVar->SetVecValue(rgba.r() / 255.f, rgba.g() / 255.f, rgba.b() / 255.f);
	material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignorez);
	material->ColorModulate(
		rgba.r() / 255.0f,
		rgba.g() / 255.0f,
		rgba.b() / 255.0f);
	g_MdlRender->ForcedMaterialOverride(material);
}

void Chams::OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix) 
{
	bool is_player = strstr(info.pModel->szName, "models/player") != nullptr;
	bool is_arm = strstr(info.pModel->szName, "arms") != nullptr;
	bool is_sleeve = strstr(info.pModel->szName, "sleeve") != nullptr;
	bool is_weapon = strstr(info.pModel->szName, "models/weapons/v_") != nullptr;
	if (is_player
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/ctm_sas.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/ctm_sas_variantf.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/ctm_fbi_variantb.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/ctm_fbi_variantf.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/ctm_fbi_variantg.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/ctm_fbi_varianth.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/ctm_st6_variantg.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/ctm_st6_variante.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/ctm_st6_varianti.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/ctm_st6_variantk.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/ctm_st6_variantm.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_leet_variantf.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_leet_variantg.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_leet_varianth.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_leet_varianti.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_phoenix.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_phoenix_variantf.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_phoenix_variantg.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_phoenix_varianth.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_balkan_variantf.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_balkan_variantg.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_balkan_varianth.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_balkan_varianti.mdl")
		|| strstr(info.pModel->szName, "models/player/custom_player/legacy/tm_balkan_variantj.mdl")
		)
	{
		if (g_Options.chams_player_enabled) {
			C_BasePlayer* ent = (C_BasePlayer*)g_EntityList->GetClientEntity(info.entity_index);
			if (!ent || ent->IsDormant() || !ent->IsPlayer()) return;

			if (ent && ent->IsPlayer() && ent->IsAlive()) {
				const auto enemy = ent->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();

				if (!enemy && g_Options.chams_player_enemies_only)
					return;

				const auto clr_front = Color(g_Options.color_chams_player_enemy_visible);
				const auto clr_back = Color(g_Options.color_chams_player_enemy_occluded);
				if (!g_Options.chams_player_visible_only) {
					OverrideMaterial(true, g_Options.chams_type, clr_back);
					Hooks::dme::o_dme(g_MdlRender, 0, ctx, state, info, matrix);
					OverrideMaterial(false, g_Options.chams_type, clr_front);
				}
				else {
					OverrideMaterial(false, g_Options.chams_type, clr_front);
				}
			}
		}
		else if (is_arm)
		{
			auto material = g_MatSystem->FindMaterial(info.pModel->szName, "Model textures");

			if (!material)
				return;
		}
	}
}