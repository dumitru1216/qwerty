#pragma once
#include "..\..\valve_utils\GlobalVars.h"
#include "..\..\valve_sdk\CGlobalVarsBase.h"
#include "..\..\valve_sdk\IClientMode.h"
#include <deque>

class Resolver
{
public:
	bool UseFreestandAngle[65];
	float FreestandAngle[65];
	Vector absOriginBackup;
	float pitchHit[65];

	void OnCreateMove();
	void FrameStage(ClientFrameStage_t stage);
private:
	void AnimationFix(C_BaseEntity* pEnt);
};
extern Resolver g_Resolver;