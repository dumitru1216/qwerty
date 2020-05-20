#include "c_antiaim.h"
#include "..\Aimbot\Autowall.h"
#include "..\..\valve_utils\Utils.h"
#include "..\..\valve_sdk\IVEngineClient.h"
#include "..\..\valve_sdk\PlayerInfo.h"
#include "..\..\valve_utils\Math.h"
#include "..\..\gui\menu_system.h"
#include <array>
#include "../../valve_sdk/ICvar.h"
#include "../Aimbot/lag_compensation.h"

c_antiaim antiaim;

void FreeStanding( ) // cancer frrestand
{
	static float FinalAngle;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;
	for (int i = 1; i <= csgo_engine->GetMaxClients( ); ++i)
	{
		C_BaseEntity* pPlayerEntity = csgo_entity_list->GetClientEntity( i );

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive( )
			|| pPlayerEntity->IsDormant( )
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam( ) == g::pLocalEntity->GetTeam( ))
			continue;

		float angToLocal = g_Math.CalcAngle( g::pLocalEntity->GetOrigin( ), pPlayerEntity->GetOrigin( ) ).y;
		Vector ViewPoint = pPlayerEntity->GetOrigin( ) + Vector( 0, 0, 90 );

		Vector2D Side1 = { (45 * sin( g_Math.GRD_TO_BOG( angToLocal ) )),(45 * cos( g_Math.GRD_TO_BOG( angToLocal ) )) };
		Vector2D Side2 = { (45 * sin( g_Math.GRD_TO_BOG( angToLocal + 180 ) )) ,(45 * cos( g_Math.GRD_TO_BOG( angToLocal + 180 ) )) };

		Vector2D Side3 = { (50 * sin( g_Math.GRD_TO_BOG( angToLocal ) )),(50 * cos( g_Math.GRD_TO_BOG( angToLocal ) )) };
		Vector2D Side4 = { (50 * sin( g_Math.GRD_TO_BOG( angToLocal + 180 ) )) ,(50 * cos( g_Math.GRD_TO_BOG( angToLocal + 180 ) )) };

		Vector Origin = g::pLocalEntity->GetOrigin( );

		Vector2D OriginLeftRight[] = { Vector2D( Side1.x, Side1.y ), Vector2D( Side2.x, Side2.y ) };

		Vector2D OriginLeftRightLocal[] = { Vector2D( Side3.x, Side3.y ), Vector2D( Side4.x, Side4.y ) };

		for (int side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };
			Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

			if (g_Autowall.CanHitFloatingPoint( OriginAutowall, ViewPoint ))
			{
				if (side == 0)
				{
					bside1 = true;
					FinalAngle = angToLocal + 90;
				}
				else if (side == 1)
				{
					bside2 = true;
					FinalAngle = angToLocal - 90;
				}
				autowalld = true;
			}
			else
			{
				for (int side222 = 0; side222 < 2; side222++)
				{
					Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 80 };

					if (g_Autowall.CanHitFloatingPoint( OriginAutowall222, OriginAutowall2 ))
					{
						if (side222 == 0)
						{
							bside1 = true;
							FinalAngle = angToLocal + 90;
						}
						else if (side222 == 1)
						{
							bside2 = true;
							FinalAngle = angToLocal - 90;
						}
						autowalld = true;
					}
				}
			}
		}
	}

	if (!autowalld || (bside1 && bside2))
		g::pCmd->viewangles.y += 180;
	else
		g::pCmd->viewangles.y = FinalAngle;

}

//desync chams
bool m_should_update_fake = false;
std::array< AnimationLayer, 15 > m_fake_layers; // 13
std::array< float, 24 > m_fake_poses; // 20
CBasePlayerAnimState* m_fake_state = nullptr;
bool init_fake_anim = false;
float m_fake_spawntime = 0.f;
matrix3x4_t m_fake_position_matrix[128];
void c_antiaim::desyncchams( )
{
	if (!g::pLocalEntity || !g::pLocalEntity->IsAlive( )) {
		m_should_update_fake = true;
		return;
	}

	if (m_fake_spawntime != g::pLocalEntity->m_flSpawnTime( ) || m_should_update_fake)
	{
		init_fake_anim = false;
		m_fake_spawntime = g::pLocalEntity->m_flSpawnTime( );
		m_should_update_fake = false;
	}

	if (!init_fake_anim)
	{
		m_fake_state = reinterpret_cast<CBasePlayerAnimState*> (csgo_mem_alloc->Alloc( sizeof( CBasePlayerAnimState ) ));

		if (m_fake_state != nullptr)
			g::pLocalEntity->create_animation_state( m_fake_state );

		init_fake_anim = true;
	}

	if (g::bSendPacket)
	{
		int OldFrameCount = csgo_globals->framecount;
		int OldTickCount = csgo_globals->tick_cound;
		static auto host_timescale = csgo_cvar->FindVar( ("host_timescale") );

		csgo_globals->framecount = TIME_TO_TICKS( g::pLocalEntity->GetSimulationTime( ) );
		csgo_globals->tick_cound = TIME_TO_TICKS( g::pLocalEntity->GetSimulationTime( ) );

		std::memcpy( m_fake_layers.data( ), g::pLocalEntity->GetAnimOverlays( ), sizeof( m_fake_layers ) );
		std::memcpy( m_fake_poses.data( ), g::pLocalEntity->m_flPoseParameter( ).data( ), sizeof( m_fake_poses ) );

		g::pLocalEntity->UpdateAnimationState( m_fake_state, g::pCmd->viewangles );

		const auto backup_absangles = g::pLocalEntity->GetAbsAngles( );

		/* invalidate bone cache */
		g::pLocalEntity->GetMostRecentModelBoneCounter( ) = 0;
		g::pLocalEntity->GetLastBoneSetupTime( ) = -FLT_MAX;

		g::m_got_fake_matrix = g::pLocalEntity->SetupBones( g::m_fake_matrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING & ~BONE_USED_BY_ATTACHMENT, csgo_globals->cur_time );

		memcpy( m_fake_position_matrix, g::m_fake_matrix, sizeof( m_fake_position_matrix ) );

		const auto org_tmp = g::pLocalEntity->GetRenderOrigin( );

		if (g::m_got_fake_matrix)
		{
			for (auto& i : g::m_fake_matrix)
			{
				i[0][3] -= org_tmp.x;
				i[1][3] -= org_tmp.y;
				i[2][3] -= org_tmp.z;
			}
		}

		g::pLocalEntity->SetAbsAngles( backup_absangles ); // restore real abs rotation

		/* invalidate bone cache */
		g::pLocalEntity->GetMostRecentModelBoneCounter( ) = 0;
		g::pLocalEntity->GetLastBoneSetupTime( ) = -FLT_MAX;

		std::memcpy( g::pLocalEntity->GetAnimOverlays( ), m_fake_layers.data( ), sizeof( m_fake_layers ) );
		std::memcpy( g::pLocalEntity->m_flPoseParameter( ).data( ), m_fake_poses.data( ), sizeof( m_fake_poses ) );

		csgo_globals->framecount = OldFrameCount;
		csgo_globals->tick_cound = OldTickCount;
	}
}

//onetap lby braker right hre
bool LbyUpdate( ) {

	auto speed = g::pLocalEntity->get_velocity( ).Length2D( );
	static float next_lby = 0.00f;
	float curtime = csgo_globals->cur_time;

	if (!(g::pLocalEntity->get_flags( ) & fl_onground ))
		return false;

	if (speed > 0.1f)
		next_lby = curtime + 0.22;

	if (next_lby < curtime)
	{
		next_lby = curtime + 1.1;
		return true;
	}
	else
		return false;
}

float C_BaseEntity::MaxDesyncDelta( ) {

	
	if (g::pLocalEntity) return false;

	auto animstate = uintptr_t( g::pLocalEntity->GetPlayerAnimState( ) );

	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = std::fmax( 0, std::fmin( *reinterpret_cast<float*>(animstate + 0xF8), 1 ) );

	float speedfactor = std::fmax( 0, std::fmin( 1, *reinterpret_cast<float*> (animstate + 0xFC) ) );

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0) {

		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

	}

	unk3 = *(float*)(animstate + 0x334) * unk2;

	return unk3;

}

void DoFake( CUserCmd* userCMD )
{
	static bool inverter = false;


	static size_t lastTime = 0;
	if (GetAsyncKeyState( galaxy_vars.cfg.desync_swap ))
	{
		if (GetTickCount( ) > lastTime) {
			inverter = !inverter;
			lastTime = GetTickCount( ) + 450;
		}
	}

	if (inverter)
	{
		userCMD->viewangles.y -= (galaxy_vars.cfg.body_lean - 100);
		if (g::bSendPacket)
		{
			userCMD->viewangles.y -= g::pLocalEntity->MaxDesyncDelta( );
		}
		else
		{
			if (LbyUpdate( ))
				userCMD->viewangles.y += (g::pLocalEntity->MaxDesyncDelta( ) * 2);
			else
				userCMD->viewangles.y -= 119.f;
		}
	}
	else
	{
		userCMD->viewangles.y += (galaxy_vars.cfg.body_invert - 100);
		if (g::bSendPacket)
		{
			userCMD->viewangles.y += g::pLocalEntity->MaxDesyncDelta( );
		}
		else
		{
			if (LbyUpdate( ))
				userCMD->viewangles.y -= (g::pLocalEntity->MaxDesyncDelta( ) * 2);
			else
				userCMD->viewangles.y += 119.f;
		}

	}

}
void c_antiaim::yaw_desync( )
{
	switch (galaxy_vars.cfg.yaw_desync)
	{
		case 0: break; // thats turn off case
		case 1: g::pCmd->viewangles.y += 180; break; //backwards
		case 2: DoFake( g::pCmd );
		
	}


}

inline float FastSqrt222( float x )
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
#define square( x ) ( x * x )
void ClampMovement( CUserCmd* pCommand, float fMaxSpeed )
{
	if (fMaxSpeed <= 0.f)
		return;
	float fSpeed = (float)(FastSqrt222( square( pCommand->forwardmove ) + square( pCommand->sidemove ) + square( pCommand->upmove ) ));
	if (fSpeed <= 0.f)
		return;
	if (pCommand->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f;
	if (fSpeed <= fMaxSpeed)
		return;
	float fRatio = fMaxSpeed / fSpeed;
	pCommand->forwardmove *= fRatio;
	pCommand->sidemove *= fRatio;
	pCommand->upmove *= fRatio;
}

void slowwalk( )
{
	if (g::pCmd)
	{
		if (galaxy_vars.cfg.slowwalk)
		{
			if (GetAsyncKeyState( VK_SHIFT ))
				if (g::pLocalEntity->get_flags( ) & fl_onground)
					ClampMovement( g::pCmd, galaxy_vars.cfg.slowspeed * 2 );
		}
	}

}

void c_antiaim::create_move( )
{
	if (!csgo_engine->in_game( ) || !galaxy_vars.cfg.Antiaim) return;
	if (!g::pLocalEntity->IsAlive( )) return;
	if (!g::pLocalEntity->GetActiveWeapon( ) || g::pLocalEntity->IsKnifeorNade( )) return;
	float flServerTime = g::pLocalEntity->GetTickBase( ) * csgo_globals->interval_per_tick;
	bool canShoot = (g::pLocalEntity->GetActiveWeapon( )->GetNextPrimaryAttack( ) <= flServerTime);
	if (g::pCmd->buttons & IN_USE) return;

	if (canShoot && (g::pCmd->buttons & IN_ATTACK)) return;

	//automatically pitch down
	g::pCmd->viewangles.x = 89.9f;

	yaw_desync( );
	slowwalk( );

	if (g::pLocalEntity->GetMoveType( ) != MoveType_t::movetype_ladder)
		g::pCmd->buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT);
}

void c_antiaim::smallmovements( )
{
	auto NetChannel = csgo_engine->get_network_channel( );
	if (!NetChannel) return;
		

	if (galaxy_vars.cfg.Antiaim && g::pCmd)
	{


		if (!(g::pLocalEntity->GetMoveType( ) == MoveType_t::movetype_ladder))
		{
			if (g::pLocalEntity->get_flags( ) & fl_onground )
			{
				if (NetChannel->m_nChokedPackets) {
					static bool flip_movement = false;

					if (g::pCmd->sidemove == 0.0f && !(g::pCmd->buttons & (int)IN_JUMP))
						g::pCmd->sidemove = flip_movement ? -1.011f : 1.011f;

					flip_movement = !flip_movement;
				}

				if (NetChannel->m_nChokedPackets) {
					static bool flip_movement = false;

					if (g::pCmd->forwardmove == 0.0f && !(g::pCmd->buttons & (int)IN_JUMP))
						g::pCmd->forwardmove = flip_movement ? -1.011f : 1.011f;

					flip_movement = !flip_movement;
				}

			}

		}



	}

}