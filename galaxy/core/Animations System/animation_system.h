#pragma once
#include "../../valve_sdk/CEntity.h"

class animations_system
{
public:
	bool setup_bones( C_BaseEntity* target, int boneMask, float currentTime, matrix3x4_t* pBoneToWorldOut );
	bool HandleBoneSetup( C_BaseEntity* target, matrix3x4_t* pBoneToWorldOut, int boneMask, float currentTime );
	void build_server_bones( C_BaseEntity* player, matrix3x4_t* pBoneToWorldOut );
	void EnemyAnimFix( C_BaseEntity* entity );
	void LocalAnimFix( C_BaseEntity* entity );
	void build_matrix( C_BaseEntity* pl, matrix3x4_t mat[128] );

};
extern animations_system c_animfix;

