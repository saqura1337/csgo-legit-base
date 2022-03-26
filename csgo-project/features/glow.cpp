#include "glow.hpp"

#include "../valve_sdk/csgostructs.hpp"
#include "../options.hpp"

Glow::Glow()
{
}

Glow::~Glow()
{
    // We cannot call shutdown here unfortunately.
    // Reason is not very straightforward but anyways:
    // - This destructor will be called when the dll unloads
    //   but it cannot distinguish between manual unload 
    //   (pressing the Unload button or calling FreeLibrary)
    //   or unload due to game exit.
    //   What that means is that this destructor will be called
    //   when the game exits.
    // - When the game is exiting, other dlls might already 
    //   have been unloaded before us, so it is not safe to 
    //   access intermodular variables or functions.
    //   
    //   Trying to call Shutdown here will crash CSGO when it is
    //   exiting (because we try to access g_GlowObjManager).
    //
}

void Glow::Shutdown()
{
    // Remove glow from all entities
    for(auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++) {
        auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
        auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

        if(glowObject.IsUnused())
            continue;

        if(!entity || entity->IsDormant())
            continue;

        glowObject.m_flAlpha = 0.0f;
    }
}

void Glow::Run()
{
    for(auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++) {
        auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
        auto entity = reinterpret_cast<C_BasePlayer*>(glowObject.m_pEntity);

        if(glowObject.IsUnused())
            continue;

        if(!entity || entity->IsDormant())
            continue;

        auto class_id = entity->GetClientClass()->m_ClassID;

            if (class_id== ClassId_CCSPlayer)
            {
                auto is_enemy = entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();

                if(!g_Options.glow_enabled || !entity->IsAlive())
                    continue;

                if(!is_enemy)
                    continue;

                if (!g_LocalPlayer->CanSeePlayer(entity, HITBOX_CHEST) && g_Options.glow_enemiesOC) {
                    glowObject.m_flRed = g_Options.color_glow_enemyOC[0] / 255.0f;
                    glowObject.m_flGreen = g_Options.color_glow_enemyOC[1] / 255.0f;
                    glowObject.m_flBlue = g_Options.color_glow_enemyOC[2] / 255.0f;
                    glowObject.m_flAlpha = g_Options.color_glow_enemyOC[3] / 255.0f;

                }
                if (g_LocalPlayer->CanSeePlayer(entity, HITBOX_CHEST)) {
                    glowObject.m_flRed = g_Options.color_glow_enemy[0] / 255.0f;
                    glowObject.m_flGreen = g_Options.color_glow_enemy[1] / 255.0f;
                    glowObject.m_flBlue = g_Options.color_glow_enemy[2] / 255.0f;
                    glowObject.m_flAlpha = g_Options.color_glow_enemy[3] / 255.0f;

                    glowObject.m_nGlowStyle = g_Options.glow_enemies_type;
                }
            }
            else {
                glowObject.m_flRed = 0.0f;
                glowObject.m_flGreen = 0.0f;
                glowObject.m_flBlue = 0.0f;
                glowObject.m_flAlpha = 0.0f;
            }

        glowObject.m_bRenderWhenOccluded = true;
        glowObject.m_bRenderWhenUnoccluded = false;
    }
}
