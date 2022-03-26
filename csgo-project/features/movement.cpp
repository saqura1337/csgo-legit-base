#include <algorithm>

#include "movement.hpp"

#include "../options.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../menu.hpp"

/*void movement::edgebug(CUserCmd* cmd)
{
	if (g_Options.edge_bug && GetAsyncKeyState(g_Options.edge_bug_key))
	{
		float max_radias = D3DX_PI * 2;
		float step = max_radias / 128;
		float xThick = 23;
		bool did_jump;
		bool edgebugz;
		bool unduck;
		Vector pos = g_LocalPlayer->abs_origin();

		for (float a = 0.f; a < max_radias; a += step)
		{
			Vector pt;
			pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
			pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
			pt.z = pos.z;

			Vector pt2 = pt;
			pt2.z -= 6;

			trace_t edgebug;
			trace_t fag;

			Ray_t ray;
			ray.Init(pt, pt2);

			CTraceFilter flt;
			flt.pSkip = g_LocalPlayer;
			g_EngineTrace->TraceRay(ray, MASK_PLAYERSOLID, &flt, &fag);

			if (ray.m_IsRay != 1.f && edgebug.fraction != 0.f)
			{
				did_jump = true;
				cmd->buttons |= in_duck;
				unduck = true;
				edgebugz = true;
				//return true;
			}
		}
	}
}*/

void movement::jumpbug(CUserCmd* cmd)
{
	float max_radias = D3DX_PI * 2;
	float step = max_radias / 128;
	float xThick = 23;

	if (g_Options.jump_bug && GetAsyncKeyState(g_Options.jump_bug_key)) {
		if (g_LocalPlayer->m_fFlags() & FL_ONGROUND) {
			g_Options.misc_bhop2 = false;
			bool unduck = cmd->buttons &= ~in_duck;
			if (unduck) {
				cmd->buttons &= ~in_duck; // duck
				cmd->buttons |= in_jump; // jump
				unduck = false;
			}
			Vector pos = g_LocalPlayer->GetAbsOrigin();
			for (float a = 0.f; a < max_radias; a += step) {
				Vector pt;
				pt.x = (xThick * cos(a)) + pos.x;
				pt.y = (xThick * sin(a)) + pos.y;
				pt.z = pos.z;

				Vector pt2 = pt;
				pt2.z -= 8192;

				trace_t fag;

				Ray_t ray;
				ray.Init(pt, pt2);

				CTraceFilter flt;
				flt.pSkip = g_LocalPlayer;
				g_EngineTrace->TraceRay(ray, MASK_PLAYERSOLID, &flt, &fag);

				if (fag.fraction != 1.f && fag.fraction != 0.f) {
					cmd->buttons |= in_duck; // duck
					cmd->buttons &= ~in_jump; // jump
					unduck = true;
				}
			}
			for (float a = 0.f; a < max_radias; a += step) {
				Vector pt;
				pt.x = ((xThick - 2.f) * cos(a)) + pos.x;
				pt.y = ((xThick - 2.f) * sin(a)) + pos.y;
				pt.z = pos.z;

				Vector pt2 = pt;
				pt2.z -= 8192;

				trace_t fag;

				Ray_t ray;
				ray.Init(pt, pt2);

				CTraceFilter flt;
				flt.pSkip = g_LocalPlayer;
				g_EngineTrace->TraceRay(ray, MASK_PLAYERSOLID, &flt, &fag);

				if (fag.fraction != 1.f && fag.fraction != 0.f) {
					cmd->buttons |= in_duck; // duck
					cmd->buttons &= ~in_jump; // jump
					unduck = true;
				}
			}
			for (float a = 0.f; a < max_radias; a += step) {
				Vector pt;
				pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
				pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
				pt.z = pos.z;

				Vector pt2 = pt;
				pt2.z -= 8192;

				trace_t fag;

				Ray_t ray;
				ray.Init(pt, pt2);

				CTraceFilter flt;
				flt.pSkip = g_LocalPlayer;
				g_EngineTrace->TraceRay(ray, MASK_PLAYERSOLID, &flt, &fag);

				if (fag.fraction != 1.f && fag.fraction != 0.f) {
					cmd->buttons |= in_duck; // duck
					cmd->buttons &= ~in_jump; // jump
					unduck = true;
				}
			}
		}
	}
	else g_Options.misc_bhop2 = true;
}