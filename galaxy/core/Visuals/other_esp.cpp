#include "other_esp.h"
#include "..\Aimbot\c_ragebot.h"
#include "..\Aimbot\lag_compensation.h"
#include "..\..\valve_utils\Utils.h"
#include "..\..\valve_sdk\IVEngineClient.h"
#include "..\..\valve_sdk\PlayerInfo.h"
#include "..\..\valve_sdk\ISurface.h"
#include "..\..\valve_sdk\Hitboxes.h"
#include "..\..\valve_utils\Math.h"
#include "..\..\gui\menu_system.h"
#include "../../valve_sdk/ICvar.h"
#include "../../c_hooks.h"

other_esp c_other_esp;

void other_esp::night_mode( )
{
	//starting a proper nighmode
	static bool OldNightmode;

	int OldNightmodeValue = galaxy_vars.cfg.NightModeSlider;

	float nightamount = galaxy_vars.cfg.NightModeSlider / 100.f;

	if (OldNightmode != galaxy_vars.cfg.NightMode || OldNightmodeValue != galaxy_vars.cfg.NightModeSlider)
	{
		ConVar* r_DrawSpecificStaticProp = csgo_cvar->FindVar( "r_DrawSpecificStaticProp" );
		*(int*)((DWORD)& r_DrawSpecificStaticProp->fnChangeCallback + 0xC) = 0; // ew
		r_DrawSpecificStaticProp->SetValue( 0 );

		for (auto i = csgo_material_system->FirstMaterial( ); i != csgo_material_system->InvalidMaterial( ); i = csgo_material_system->NextMaterial( i ))
		{
			IMaterial* pMaterial = csgo_material_system->GetMaterial( i );

			if (!pMaterial)
				continue;

			if (strstr( pMaterial->GetTextureGroupName( ), "World" ) || strstr( pMaterial->GetTextureGroupName( ), "StaticProp" ))
			{
				if (galaxy_vars.cfg.NightMode)
				{
					if (strstr( pMaterial->GetTextureGroupName( ), "StaticProp" ))
						pMaterial->ColorModulate( nightamount, nightamount, nightamount );
					else
						pMaterial->ColorModulate( nightamount, nightamount, nightamount );
				}
				else
					pMaterial->ColorModulate( 1.0f, 1.0f, 1.0f );
			}

			if (strstr( pMaterial->GetTextureGroupName( ), "SkyBox" )) {
				if (galaxy_vars.cfg.NightMode)
				{
					auto sky_color = galaxy_vars.cfg.sky_box_color;

					pMaterial->ColorModulate( sky_color[0], sky_color[1], sky_color[2] );
				}
			}


			

		}

		OldNightmode = galaxy_vars.cfg.NightMode;
	}
}

void GradientH( int x, int y, int w, int h, Color c1, Color c2 )
{
	csgo_surface->FilledRect( x, y, w, h, c1 );
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		csgo_surface->FilledRect( x + i, y, 1, h, Color( first, second, third, ia ) );
	}
}

void other_esp::watermak( )
{
	
}

void other_esp::top_bar( )
{
	if (!galaxy_vars.cfg.top_bar_enable) return;

	int width, height;
	csgo_engine->get_screen_size( width, height );

	auto menu_color = galaxy_vars.cfg.menu_color;

	GradientH( 0, 0, width / 2, 3, Color( 56, 56, 56, 255 ), Color( menu_color[0] * 255, menu_color[1] * 255, menu_color[2] * 255, 255 ) );
	GradientH( width / 2, 0, width / 2, 3, Color( menu_color[0] * 255, menu_color[1] * 255, menu_color[2] * 255, 255 ), Color( 56, 56, 56, 255 ) );

}

void other_esp::spec_list( )
{
	if (!galaxy_vars.cfg.specator_list) return;

	if (csgo_engine->IsConnected( )) {
		int pos_x = galaxy_vars.cfg.pos_x;
		int pos_y = galaxy_vars.cfg.pos_y;
		static int _drag_x = 300;
		static int _drag_y = 300;

		static bool _dragging = false;
		bool _click = false;

		if (GetAsyncKeyState( VK_LBUTTON ))
			_click = true;

		Vector2D _mouse_pos = csgo_surface->GetMousePosition( );

		if (_dragging && !_click)
			_dragging = false;

		if (_dragging&& _click)
		{
			pos_x = _mouse_pos.x - _drag_x;
			pos_y = _mouse_pos.y - _drag_y;
			galaxy_vars.cfg.pos_x = pos_x;
			galaxy_vars.cfg.pos_y = pos_y;
		}

		int spec_width = 160;
		int spec_height = 160;

		if ((csgo_surface->MouseInRegion( pos_x + 400, pos_y, spec_width, spec_height - 140 ))) {
			_dragging = true;
			_drag_x = _mouse_pos.x - pos_x;
			_drag_y = _mouse_pos.y - pos_y;
		}

		int wa, ha;
		csgo_engine->get_screen_size( wa, ha );
	
		csgo_surface->FilledRect( pos_x + 400, pos_y, spec_width, 20, Color( 0, 0, 0, 240 ) );
		GradientH( pos_x + 400, pos_y - 1, 79, 2, Color( 56, 56, 56, 255 ), Color( 255, 255, 255, 255 ) );
		GradientH( pos_x + 479, pos_y - 1, 81, 2, Color( 255, 255, 255, 255 ), Color( 56, 56, 56, 255 ) );
		int loop = 0;

		for (int i = 0; i <= 64; i++) {
			C_BaseEntity* e = (C_BaseEntity*)csgo_entity_list->GetClientEntity( i );
			PlayerInfo_t pinfo;

			if (e&& e != g::pLocalEntity && !e->IsDormant( )) {
				csgo_engine->GetPlayerInfo( i, &pinfo );
				uintptr_t obs = e->observer_target( );
				if (!obs) continue;

				C_BaseEntity * spec = (C_BaseEntity*)csgo_entity_list->GetClientEntityFromHandle( obs );
				if (spec == nullptr) continue;
				PlayerInfo_t spec_info;
				csgo_engine->GetPlayerInfo( i, &spec_info );
				char buf[255]; sprintf_s( buf, "%s", pinfo.szName );
				if (spec->EntIndex( ) == g::pLocalEntity->EntIndex( )) {
					
					csgo_surface->FilledRect( pos_x + 400, (pos_y + (18 * loop)) + 20, 160, 20, Color( 30, 30, 30, 240 ) );
					csgo_surface->DrawT( pos_x + 408, (pos_y + (18 * loop)) + 22, Color( 180, 180, 180 ), g::CourierNew, false, buf );

					loop++;
				}
			}
		}
		char loop_str[24];
	
		csgo_surface->DrawT( pos_x + 408, pos_y + 4, Color( 180, 180, 180 ), g::CourierNew, false, "Spectators" );
		
	
	}
}

void other_esp::paint( )
{
	/* do calls now : better method */
	c_other_esp.spec_list( );
	c_other_esp.top_bar( );
}
