#include "../c_hooks.h"
#include "../core/external_inclues.h"

void __fastcall c_hooks::OverrideView( void* ecx, void* edx, CViewSetup* pSetup )
{
	static auto oOverrideView = hooks.pClientModeHook->GetOriginal<OverrideView_t>( vtable_indexes::view );

	if (csgo_engine->IsConnected( ) && csgo_engine->in_game( ))
	{
		if (!g::pLocalEntity) return;
		if (!g::pLocalEntity->IsAlive( )) return;

		if (galaxy_vars.cfg.NoRecoil)
		{
			Vector viewPunch = g::pLocalEntity->GetViewPunchAngle( );
			Vector aimPunch = g::pLocalEntity->GetAimPunchAngle( );

			pSetup->angles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
			pSetup->angles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
			pSetup->angles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		}

		if (galaxy_vars.cfg.Fov != 0 && !g::pLocalEntity->IsScoped( ))
			pSetup->fov = galaxy_vars.cfg.Fov;

		if (galaxy_vars.cfg.NoZoom && g::pLocalEntity->IsScoped( ))
			pSetup->fov = (galaxy_vars.cfg.Fov == 0) ? 90 : galaxy_vars.cfg.Fov;
	}

	oOverrideView( ecx, edx, pSetup );
}
