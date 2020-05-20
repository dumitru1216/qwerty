#pragma once
#include "..\..\valve_utils\GlobalVars.h"
#include "..\..\valve_sdk\CGlobalVarsBase.h"
#include <deque>

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / csgo_globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( csgo_globals->interval_per_tick *( t ) )

struct PlayerRecords
{
	matrix3x4_t Matrix[128];
	float Velocity;
	float SimTime;
	bool Shot;
};

class LagComp
{
public:
	int ShotTick[65];
	std::deque<PlayerRecords> PlayerRecord[65] = {  };
	void StoreRecord(C_BaseEntity* pEnt);
	void ClearRecords(int i);
	float LerpTime();
	bool ValidTick(int tick);

	template<class T, class U>
	T clamp(T in, U low, U high);
private:
};
extern LagComp g_LagComp;