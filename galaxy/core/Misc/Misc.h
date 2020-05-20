#pragma once

#include "..\Aimbot\Autowall.h"
#include "..\Aimbot\c_ragebot.h"
#include "..\Aimbot\lag_compensation.h"
#include "..\..\valve_utils\GlobalVars.h"
#include "..\..\valve_utils\Math.h"
#include "..\..\valve_sdk\ICvar.h"
#include "..\..\valve_sdk\CPrediction.h"
#include "..\..\gui\menu_system.h"
#include <iostream>
#include <algorithm>

// kinda just throw shit in here

#define _SOLVEY(a, b, c, d, e, f) ((c * b - d * a) / (c * f - d * e))
#define SOLVEY(...) _SOLVEY(?, ?, ?, ?, ?, ?)
#define SOLVEX(y, world, forward, right) ((world.x - right.x * y) / forward.x)

class Misc
{
public:
    void OnCreateMove()
    {
        this->pCmd   = g::pCmd;
        this->pLocal = g::pLocalEntity;
		this->DoBhop();
    };
	static void angle_vectorsss( const Vector& angles, Vector& forward, Vector& right, Vector& up )
	{
		const auto sp = sin( DEG2RAD( angles.x ) ), cp = cos( DEG2RAD( angles.x ) ),
			sy = sin( DEG2RAD( angles.y ) ), cy = cos( DEG2RAD( angles.y ) ),
			sr = sin( DEG2RAD( angles.z ) ), cr = cos( DEG2RAD( angles.z ) );

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;

		right.x = -1 * sr * sp * cy + -1 * cr * -sy;
		right.y = -1 * sr * sp * sy + -1 * cr * cy;
		right.z = -1 * sr * cp;

		up.x = cr * sp * cy + -sr * -sy;
		up.y = cr * sp * sy + -sr * cy;
		up.z = cr * cp;
	}
	void MovementFix( CUserCmd* cmd, Vector& wishangle ) 
	{
		Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
		angle_vectorsss( wishangle, view_fwd, view_right, view_up );
		angle_vectorsss( cmd->viewangles, cmd_fwd, cmd_right, cmd_up );

		const auto v8 = sqrtf( (view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y) );
		const auto v10 = sqrtf( (view_right.x * view_right.x) + (view_right.y * view_right.y) );
		const auto v12 = sqrtf( view_up.z * view_up.z );

		const Vector norm_view_fwd( (1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f );
		const Vector norm_view_right( (1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f );
		const Vector norm_view_up( 0.f, 0.f, (1.f / v12) * view_up.z );

		const auto v14 = sqrtf( (cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y) );
		const auto v16 = sqrtf( (cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y) );
		const auto v18 = sqrtf( cmd_up.z * cmd_up.z );

		const Vector norm_cmd_fwd( (1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f );
		const Vector norm_cmd_right( (1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f );
		const Vector norm_cmd_up( 0.f, 0.f, (1.f / v18) * cmd_up.z );

		const auto v22 = norm_view_fwd.x * cmd->forwardmove;
		const auto v26 = norm_view_fwd.y * cmd->forwardmove;
		const auto v28 = norm_view_fwd.z * cmd->forwardmove;
		const auto v24 = norm_view_right.x * cmd->sidemove;
		const auto v23 = norm_view_right.y * cmd->sidemove;
		const auto v25 = norm_view_right.z * cmd->sidemove;
		const auto v30 = norm_view_up.x * cmd->upmove;
		const auto v27 = norm_view_up.z * cmd->upmove;
		const auto v29 = norm_view_up.y * cmd->upmove;

		cmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
			+ (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
			+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
		cmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
			+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
			+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
		cmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
			+ (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
			+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

		const auto ratio = 2.f - fmaxf( fabsf( cmd->sidemove ), fabsf( cmd->forwardmove ) ) / 450.f;
		cmd->forwardmove *= ratio;
		cmd->sidemove *= ratio;

		wishangle = cmd->viewangles;
	}

	void ThirdPerson(ClientFrameStage_t curStage)
	{
		if (!csgo_engine->in_game() || !csgo_engine->IsConnected() || !g::pLocalEntity)
			return;
			
		static bool init = false;

		if (GetKeyState(VK_MBUTTON) && g::pLocalEntity->IsAlive())
		{
			if (init)
			{
				ConVar* sv_cheats = csgo_cvar->FindVar("sv_cheats");
				*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);
				csgo_engine->ExecuteClientCmd("thirdperson");
			}
			init = false;
		}
		else
		{
			if (!init)
			{
				ConVar* sv_cheats = csgo_cvar->FindVar("sv_cheats");
				*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);
				csgo_engine->ExecuteClientCmd("firstperson");
			}
			init = true;
		}

		if (curStage == FRAME_RENDER_START && GetKeyState(VK_MBUTTON) && g::pLocalEntity->IsAlive())
		{
			csgo_prediction->SetLocalViewAngles(Vector(g::RealAngle.x, g::RealAngle.y, 0)); // lol
		}
	}
	void Crosshair( )
	{
		if ( !g::pLocalEntity )
			return;

		if ( !g::pLocalEntity->IsAlive( ) )
			return;

		if ( g::pLocalEntity->IsScoped( ) && galaxy_vars.cfg.NoScope )
		{
			int Height, Width;
			csgo_engine->get_screen_size( Width, Height );

			Vector punchAngle = g::pLocalEntity->GetAimPunchAngle( );

			float x = Width / 2;
			float y = Height / 2;
			int dy = Height / 90;
			int dx = Width / 90;
			x -= ( dx*( punchAngle.y ) );
			y += ( dy*( punchAngle.x ) );

			Vector2D screenPunch = { x, y };

			csgo_surface->Line( 0, screenPunch.y, Width, screenPunch.y, Color( 0, 0, 0, 255 ) );
			csgo_surface->Line( screenPunch.x, 0, screenPunch.x, Height, Color( 0, 0, 0, 255 ) );
		}


	}

	void NormalWalk() // heh
	{
		g::pCmd->buttons &= ~IN_MOVERIGHT;
		g::pCmd->buttons &= ~IN_MOVELEFT;
		g::pCmd->buttons &= ~IN_FORWARD;
		g::pCmd->buttons &= ~IN_BACK;

		if (g::pCmd->forwardmove > 0.f)
			g::pCmd->buttons |= IN_FORWARD;
		else if (g::pCmd->forwardmove < 0.f)
			g::pCmd->buttons |= IN_BACK;
		if (g::pCmd->sidemove > 0.f)
		{
			g::pCmd->buttons |= IN_MOVERIGHT;
		}
		else if (g::pCmd->sidemove < 0.f)
		{
			g::pCmd->buttons |= IN_MOVELEFT;
		}
	}

private:
    CUserCmd*     pCmd;
    C_BaseEntity* pLocal;

	void DoAutostrafe( CUserCmd* cmd ) const
	{
		static const auto cl_sidespeed = csgo_cvar->FindVar( ("cl_sidespeed") );
		float yaw_add = 0.f;
		float yaw_add_s = 0.f;

		if (!g::pLocalEntity) return;
		if (!csgo_engine->IsConnected( ) || !csgo_engine->in_game( ) || !galaxy_vars.cfg.AutoStrafe) return;
		if (!g::pLocalEntity->IsAlive( )) return;
			
		if (cmd->buttons & IN_BACK) {
			yaw_add = -180.f;
			if (cmd->buttons & IN_MOVERIGHT)
				yaw_add = -125;
			else if (cmd->buttons & IN_MOVELEFT)
				yaw_add = 125;
		}
		else if (cmd->buttons & IN_FORWARD) {
			yaw_add = 0.f;
			if (cmd->buttons & IN_MOVERIGHT)
				yaw_add = -45;
			else if (cmd->buttons & IN_MOVELEFT)
				yaw_add = 45;
		}
		else if (cmd->buttons & IN_MOVERIGHT)
			yaw_add = -90.f;
		else if (cmd->buttons & IN_MOVELEFT)
			yaw_add = 90.f;

		g::OriginalView.y += yaw_add;

		cmd->forwardmove = 0.f;
		cmd->sidemove = 0.f;

		const auto delta = g_Math.NormalizeYaw( g::OriginalView.y - RAD2DEG( atan2( g::pLocalEntity->get_velocity( ).y, g::pLocalEntity->get_velocity( ).x ) ) );
		g::pCmd->sidemove = delta > 0.f ? -cl_sidespeed->GetFloat( ) : cl_sidespeed->GetFloat( );

		g::OriginalView.y = g_Math.NormalizeYaw( g::OriginalView.y - delta );
	}

    void DoBhop() const
    {
 
		if (!galaxy_vars.cfg.Bhop && !galaxy_vars.cfg.AutoStrafe)
			return;

		static auto last_jumped = false;
		static auto should_fake = false;

		const auto move_type = g::pLocalEntity->GetMoveType( );
		const auto flags = g::pLocalEntity->get_flags( );

		if (move_type != MoveType_t::movetype_ladder && move_type != MoveType_t::movetype_noclip &&
		!(flags & 1 << 9))// in_water
		{
			if (!last_jumped && should_fake)
			{
				should_fake = false;
				if (galaxy_vars.cfg.Bhop)
					g::pCmd->buttons |= IN_JUMP;
			}
			else if (g::pCmd->buttons & IN_JUMP)
			{
				DoAutostrafe( g::pCmd );

				if (flags & fl_onground)
				{
					last_jumped = true;
					should_fake = true;
				}
				else
				{
					if (galaxy_vars.cfg.Bhop)
						g::pCmd->buttons &= ~IN_JUMP;
					last_jumped = false;
				}
			}
			else
			{
				last_jumped = false;
				should_fake = false;
			}
		}
    }

};

extern Misc g_Misc;