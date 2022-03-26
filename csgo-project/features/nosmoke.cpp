
#include "nosmoke.hpp"
#include "../options.hpp"

void ResetData()
{
	int g_nTickbase = 0;
	int g_nTicks = 0;
	bool m_bSwap = false;
	int m_iMaxProcessTicks = 0;

	g_nTickbase = 0;
	m_iMaxProcessTicks = 0;
	m_bSwap = false;
}

void NoSmoke::OnFrameStageNotify()
{
	static bool set = true;
	static std::vector<const char*> smoke_materials =
	{
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	if (!g_Options.no_smoke)
	{
		if (set)
		{
			for (auto material_name : smoke_materials)
			{
				IMaterial* mat = g_MatSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			}
			set = false;
		}
		return;
	}

	set = true;
	for (auto material_name : smoke_materials)
	{
		IMaterial* mat = g_MatSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
	}

	static std::string old_Skyname = "";
	static bool OldNightmode;
	static int OldSky;
	static int amount;
	if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected() || !g_LocalPlayer)
	{
		old_Skyname = "";
		OldNightmode = false;
		OldSky = 0;
		amount = 0;
		return;
	}

	if (g_LocalPlayer->IsAlive())
	{

		static ConVar* r_DrawSpecificStaticProp;
		if (OldNightmode || !&g_Options.nightmode)
		{
			if (!r_DrawSpecificStaticProp)
				r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
			r_DrawSpecificStaticProp->SetValue(0);
			for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i)) {
				auto material = g_MatSystem->GetMaterial(i);

				if (!material)
					continue;

				if (strstr(material->GetTextureGroupName(), ("World"))) {
					material->ColorModulate(0.025f, 0.025f, 0.025f);
				}
				else if (strstr(material->GetTextureGroupName(), ("StaticProp"))) {
					material->ColorModulate(0.1f, 0.1f, 0.1f);
				}
			}
			OldNightmode = &g_Options.nightmode;
		}
	}
}
