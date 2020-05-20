#pragma once
#include "..\..\valve_utils\GlobalVars.h"
#include "..\..\valve_sdk\CGlobalVarsBase.h"
#include "../../valve_sdk/IVModelInfo.h"
#include "Autowall.h"
#include "lag_compensation.h"
#include "..\AntiAim\c_antiaim.h"
#include "..\..\valve_utils\Utils.h"
#include "..\..\valve_sdk\IVEngineClient.h"
#include "..\..\valve_sdk\PlayerInfo.h"
#include "..\..\valve_sdk\ICvar.h"
#include "../../valve_sdk/IEngineTrace.h"
#include "..\..\gui\menu_system.h"
#include "..\..\valve_utils\Math.h"
#include "..\..\valve_sdk\Hitboxes.h"
#include <deque>

enum HitboxList
{
	Head = 0,
	Neck,
	Pelvis,
	Stomach,
	LowerChest,
	Chest,
	UpperChest,
	RightThigh,
	LeftThigh,
	RightShin,
	LeftShin,
	RightFoot,
	LeftFoot,
	RightHand,
	LeftHand,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm
};

class c_aimbot
{
public:
	matrix3x4_t Matrix[65][128];
	int Sequence;
	void OnCreateMove( );
	int bestEntDmg;
public:
	bool Backtrack[65];
	bool ShotBacktrack[65];
	Vector Hitscan( C_BaseEntity* pEnt );
	bool HitChance( C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance );
	void Autostop( );


};
extern c_aimbot aimbot;