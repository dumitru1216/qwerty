#include "EnginePrediction.h"
#include "..\..\valve_sdk\CInput.h"
#include "..\..\valve_sdk\CEntity.h"
#include "..\..\valve_utils\GlobalVars.h"
#include "..\..\valve_sdk\CPrediction.h"
#include "..\..\valve_sdk\CGlobalVarsBase.h"
#include "..\..\gui\menu_system.h"

//outlassn https://www.unknowncheats.me/forum/1842735-post1.html and vrc https://www.unknowncheats.me/forum/2296204-post1.html

float flOldCurtime;
float flOldFrametime;
int *m_nPredictionRandomSeed;
int *m_pSetPredictionPlayer;
static bool bInit = false;

void engine_prediction::RunEnginePred()
{
	if (!g::pLocalEntity->IsAlive() || !csgo_move_helper || !galaxy_vars.cfg.Aimbot || galaxy_vars.cfg.LegitBacktrack)
		return;

	if (!m_nPredictionRandomSeed || !m_pSetPredictionPlayer) {
		m_nPredictionRandomSeed = *reinterpret_cast<int**>(Utils::FindSignature("client_panorama.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);
		m_pSetPredictionPlayer = *reinterpret_cast<int**>(Utils::FindSignature("client_panorama.dll", "89 35 ? ? ? ? F3 0F 10 46") + 2);
	}

	CMoveData data;
	memset(&data, 0, sizeof(CMoveData));
	g::pLocalEntity->SetCurrentCommand(g::pCmd);
	csgo_move_helper->SetHost(g::pLocalEntity);

	*m_nPredictionRandomSeed = g::pCmd->random_seed;
	*m_pSetPredictionPlayer = uintptr_t(g::pLocalEntity);

	*reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(g::pLocalEntity) + 0x3314) = reinterpret_cast<uint32_t>(g::pCmd); // lol
	*reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(g::pLocalEntity) + 0x326C) = reinterpret_cast<uint32_t>(g::pCmd); // lol

	flOldCurtime = csgo_globals->cur_time;
	flOldFrametime = csgo_globals->frame_time;

	g::uRandomSeed = *m_nPredictionRandomSeed;
	csgo_globals->cur_time = g::pLocalEntity->GetTickBase() * csgo_globals->interval_per_tick;
	csgo_globals->frame_time = csgo_globals->interval_per_tick;

	csgo_movement->StartTrackPredictionErrors(g::pLocalEntity);

	csgo_prediction->SetupMove(g::pLocalEntity, g::pCmd, csgo_move_helper, &data);
	csgo_movement->ProcessMovement(g::pLocalEntity, &data);
	csgo_prediction->FinishMove(g::pLocalEntity, g::pCmd, &data);

	if (g::pLocalEntity->GetActiveWeapon())
		g::pLocalEntity->GetActiveWeapon()->AccuracyPenalty();
}

void engine_prediction::EndEnginePred()
{
	if (!g::pLocalEntity->IsAlive() || !csgo_move_helper || !galaxy_vars.cfg.Aimbot || galaxy_vars.cfg.LegitBacktrack)
		return;

	csgo_movement->FinishTrackPredictionErrors(g::pLocalEntity);
	csgo_move_helper->SetHost(nullptr);

	if (m_nPredictionRandomSeed || m_pSetPredictionPlayer)
	{
		*m_nPredictionRandomSeed = -1;
		*m_pSetPredictionPlayer = 0;
	}

	csgo_globals->cur_time = flOldCurtime;
	csgo_globals->frame_time = flOldFrametime;

	g::pLocalEntity->SetCurrentCommand(NULL);
}