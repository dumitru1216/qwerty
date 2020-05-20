#include "c_esp.h"
#include "..\Aimbot\c_ragebot.h"
#include "..\Aimbot\lag_compensation.h"
#include "..\..\valve_utils\Utils.h"
#include "..\..\valve_sdk\IVEngineClient.h"
#include "..\..\valve_sdk\PlayerInfo.h"
#include "..\..\valve_sdk\ISurface.h"
#include "..\..\valve_sdk\Hitboxes.h"
#include "..\..\valve_utils\Math.h"
#include "..\..\gui\menu_system.h"

c_esp esp;

void c_esp::RenderBox()
{
	csgo_surface->OutlinedRect( Box.left, Box.top, Box.right, Box.bottom, Color(galaxy_vars.cfg.box_color[0] * 255, galaxy_vars.cfg.box_color[1] * 255, galaxy_vars.cfg.box_color[2] * 255, 240 )); //main part
	csgo_surface->OutlinedRect( Box.left + 1, Box.top + 1, Box.right - 2, Box.bottom - 2, Color(0, 0, 0, 240)); //outlkine
	csgo_surface->OutlinedRect( Box.left - 1, Box.top - 1, Box.right + 2, Box.bottom + 2, Color(0, 0, 0, 240)); //outline
}

void c_esp::RenderName(C_BaseEntity* pEnt, int iterator)
{
    PlayerInfo_t pInfo;
    csgo_engine->GetPlayerInfo(iterator, &pInfo);

	auto name_color = galaxy_vars.cfg.name_color;

	if (galaxy_vars.cfg.name)
		csgo_surface->DrawT(Box.left + (Box.right / 2), Box.top - 9, Color(name_color[0] * 255, name_color[1] * 255, name_color[2] * 255, 240), font, true, pInfo.szName);
	
}

std::string clean_name( std::string name ) {
	std::string wep = name;
	if (wep[0] == 'W') wep.erase( wep.begin( ) );

	auto end_of_weapon = wep.find( "EAPON_" );
	if (end_of_weapon != std::string::npos)
		wep.erase( wep.begin( ) + end_of_weapon, wep.begin( ) + end_of_weapon + 6 );

	return wep;
}

float resolve_distance( Vector src, Vector dest )
{
	Vector delta = src - dest;
	float fl_dist = ::sqrtf( (delta.Length( )) );
	if (fl_dist < 1.0f) return 1.0f;
	return fl_dist;
}

void c_esp::RenderWeaponName(C_BaseEntity* pEnt)
{
    auto weapon = pEnt->GetActiveWeapon();
    if (!weapon) return;
     
	std::string wep_str = weapon->GetName( );
	Vector vecOrigin = pEnt->GetOrigin( ), vecOriginLocal = g::pLocalEntity->GetOrigin( );
	std::transform( wep_str.begin( ), wep_str.end( ), wep_str.begin( ), ::toupper );

	char ammo[519];
	char dist_to[32];
	sprintf_s( ammo, " - [ %d  /  %d ]", weapon->GetAmmo( ), weapon->m_iPrimaryReserveAmmoCount( ) );
	sprintf_s( dist_to, "%.0f ft", resolve_distance( vecOriginLocal, vecOrigin ) );
	std::string strWeaponName = std::string( weapon->GetCSWpnData( )->weapon_name ); //weapon->GetName();

    strWeaponName.erase(0, 7);

	if (galaxy_vars.cfg.weapon_name)
		csgo_surface->DrawT( Box.left + (Box.right / 2), Box.top + Box.bottom + 7, Color( 255, 255, 255, 240 ), font, true, dist_to );
		csgo_surface->DrawT( Box.left + (Box.right / 2), Box.top + Box.bottom + 15, Color( 255, 255, 255, 240 ), font, true, strWeaponName.c_str( ));

}

void c_esp::RenderAmmo( C_BaseEntity* pEnt )
{
	if (!pEnt->GetActiveWeapon( )->GetCSWpnData_2( )) return;
	int
		max_in_clip = pEnt->GetActiveWeapon( )->GetCSWpnData_2( )->iMaxClip1,
		ammo_in_clip = pEnt->GetActiveWeapon( )->GetAmmo( ),
		scaled_ammo = ammo_in_clip * Box.right / max_in_clip,
		shit = std::clamp( scaled_ammo, 0, Box.right );

	auto ammo_color = galaxy_vars.cfg.ammo_color;

	csgo_surface->FilledRect( Box.left + 2, Box.top + Box.bottom + 3, Box.right - 5, 2, Color( 20, 20, 20, 240 ) );
	csgo_surface->OutlinedRect( Box.left + 1, Box.top + Box.bottom + 2, Box.right - 3, 4, Color( 35, 35, 35, 240 ) );
	csgo_surface->FilledRect( Box.left + 2, Box.top + Box.bottom + 3, shit - 5, 2, Color( ammo_color[0] * 255, ammo_color[1] * 255, ammo_color[2] * 255, 240 ) );
	std::string ammo = "" + std::to_string( pEnt->GetActiveWeapon()->GetAmmo( ) );
	csgo_surface->DrawT( Box.left + (Box.right + 5), Box.top + Box.bottom - 1, Color( 255, 255, 255, 240 ), font, true, ammo.c_str( ) );


}
 //+ Box.right
void c_esp::RenderHealth( C_BaseEntity* pEnt )
{
	if (galaxy_vars.cfg.HealthBar)
	{
		//hp number
		std::string Health = "" + std::to_string( pEnt->GetHealth( ) );
		csgo_surface->DrawT( Box.left - 20, Box.top + (offsetY * 5), Color( 255, 255, 255, 255 ), font, false, Health.c_str( ) );
		offsetY += 1;

		csgo_surface->FilledRect( Box.left - 6, Box.top - 1, 4, Box.bottom + 2, Color( 0, 0, 0, 240 ) );
		int pixelValue = pEnt->GetHealth( ) * Box.bottom / 100;
		csgo_surface->FilledRect( Box.left - 5, Box.top + Box.bottom - pixelValue, 2, pixelValue, Color( 0, 255, 0, 250 ) );


	}
}

void c_esp::RenderFlags( C_BaseEntity* pEnt )
{
	int x = Box.right + Box.top + 2, y = Box.bottom - 1;

	auto drawFlag = [&]( const char* str, Color color, ... ) -> void
	{
		csgo_surface->DrawT( Box.right + Box.top + 2, Box.bottom - 1, color, g::CourierNew, false, str );
		y += 10;
	};

	Color colorwhiteflag;
	Color moneyflag;
	Color zoomFlag;

	if (!pEnt->IsDormant( )) {
		colorwhiteflag = Color( 255, 255, 255, 255 );
		moneyflag = Color( 21, 255, 0, 255 );
		zoomFlag = Color( 79, 111, 255, 255 );
	}
	else if (pEnt->IsDormant( )) {
		colorwhiteflag = Color( 144, 144, 144, 60 );
		moneyflag = Color( 144, 144, 144, 60 );
		zoomFlag = Color( 144, 144, 144, 60 );
	}

	const char* moonla;
	std::string moonla1 = "$" + std::to_string( pEnt->Money( ) );
	moonla = moonla1.c_str( );

	csgo_surface->DrawT( Box.left + Box.right + 5, Box.top + (offsetY * 11), moneyflag, g::CourierNew, false, moonla );

}

void c_esp::BoundBox(C_BaseEntity* pEnt)
{
	Box.bottom = 0;
	Box.top = 0;
	Box.left = 0;
	Box.right = 0;

	Vector2D w2sBottom, w2sTop;

	Utils::WorldToScreen(pEnt->GetOrigin() - Vector(0, 0, 8), w2sBottom); 
	Utils::WorldToScreen(pEnt->GetHitboxPosition(0, aimbot.Matrix[pEnt->EntIndex()]) + Vector(0, 0, 10), w2sTop);
	
	int Middle = w2sBottom.y - w2sTop.y;
	int Width = Middle / 3.f;

	Box.bottom = Middle;
	Box.top = w2sTop.y;
	Box.left = w2sBottom.x - Width;
	Box.right = Width * 2;
}

void c_esp::Render()
{
    if (!g::pLocalEntity || !csgo_engine->in_game() || !galaxy_vars.cfg.Esp)  return;
       
	(galaxy_vars.cfg.Font == 0) ? font = g::CourierNew : font = g::Tahoma;

	for (int i = 1; i < csgo_engine->GetMaxClients(); ++i)
    {
        C_BaseEntity* pPlayerEntity = csgo_entity_list->GetClientEntity(i);

        if (!pPlayerEntity
            || !pPlayerEntity->IsAlive()
            || pPlayerEntity->IsDormant()
            || pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
            continue;

		offsetY = 0;

		BoundBox(pPlayerEntity);

		if (Box.bottom == 0)
			continue;

        if (galaxy_vars.cfg.Box)
            RenderBox();

		if (galaxy_vars.cfg.flags)
			RenderFlags( pPlayerEntity );

        if (galaxy_vars.cfg.name)
            RenderName(pPlayerEntity, i);

        if (galaxy_vars.cfg.weapon_name)
            RenderWeaponName(pPlayerEntity);

		if (galaxy_vars.cfg.ammo_bar)
			RenderAmmo( pPlayerEntity );

		if (galaxy_vars.cfg.HealthBar)
			RenderHealth(pPlayerEntity);
    }
}