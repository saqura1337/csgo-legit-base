#pragma once

#include "../singleton.hpp"

#include "../render.hpp"
#include "../helpers/math.hpp"
#include "../valve_sdk/csgostructs.hpp"

class movement : public Singleton<movement>
{
public:
	//void edgebug(CUserCmd* cmd);
	void jumpbug(CUserCmd* cmd);
};