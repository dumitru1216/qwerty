#pragma once
#include "..\valve_sdk\CInput.h"
#include "..\valve_sdk\CEntity.h"

namespace g
{
    extern CUserCmd*      pCmd;
    extern C_BaseEntity*  pLocalEntity;
    extern std::uintptr_t uRandomSeed;
	extern Vector         OriginalView;
	extern bool           bSendPacket;
	extern bool			  LagPeek;
	extern int            TargetIndex;
	extern Vector         EnemyEyeAngs[65];
	extern Vector         AimbotHitbox[65][28];
	extern Vector         RealAngle;
	extern Vector         FakeAngle;
	extern bool           Shot[65];
	extern bool           Hit[65];
	extern int            MissedShots[65];

	extern matrix3x4_t m_fake_matrix[128];
	extern bool m_got_fake_matrix;

	//fonts
	extern DWORD weapon_font;
	extern DWORD watermark_font;
	extern DWORD CourierNew;
	extern DWORD Tahoma;
}