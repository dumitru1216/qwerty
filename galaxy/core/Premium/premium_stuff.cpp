#include "premium_stuff.h"
#include "../../c_hooks.h"
#include "../../valve_utils/GlobalVars.h"
#include "..\Aimbot\c_ragebot.h"
#include "..\Aimbot\lag_compensation.h"
#include "..\..\valve_utils\Utils.h"
#include "..\..\valve_sdk\IVEngineClient.h"
#include "..\..\valve_sdk\PlayerInfo.h"
#include "..\..\valve_sdk\ISurface.h"
#include "..\..\valve_sdk\Hitboxes.h"
#include "..\..\valve_utils\Math.h"
#include "..\..\gui\menu_system.h"


c_stuff stuff;

void c_stuff::back_trails( )
{
	if (!galaxy_vars.cfg.local_trails || !g::pLocalEntity)
		return;

	static Vector old_origin = g::pLocalEntity->GetOrigin( );
	auto origin = g::pLocalEntity->GetOrigin( ) + Vector( 0, 0, 5 );

	auto start = old_origin, end = origin;
	//Color color = config->Colors.trails_color;
	constexpr auto width = 1.6f;

	//if (old_origin.Length2D( ) != origin.Length2D( ))
		//draw_beam( start, end, color, width );

	old_origin = origin;
}

