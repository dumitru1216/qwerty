#include "hooking_init.h"
#include "../c_hooks.h"
#include <thread>
#include "..\valve_utils\Utils.h"
#include "../core/external_inclues.h"
#include "..\gui\menu_system.h"
#include "..\valve_sdk\Hitboxes.h"
#include "libloaderapi.h"

hooking* c_hooking;

void hooking::window_handle( )
{
	// Get window handle
	while (!(hooks.hCSGOWindow = FindWindowA( "Valve001", nullptr )))
	{
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for( 50ms );
	}
}

void hooking::init_fonts( )
{
	//create font
	g::CourierNew = csgo_surface->FontCreate( );
	g::Tahoma = csgo_surface->FontCreate( );
	g::weapon_font = csgo_surface->FontCreate( );
	g::watermark_font = csgo_surface->FontCreate( );

	//render font
	csgo_surface->SetFontGlyphSet( g::CourierNew, "hooge 05_55 Cyr2", 10, 300, 0, 0, FONTFLAG_OUTLINE );
	csgo_surface->SetFontGlyphSet( g::Tahoma, "Tahoma", 12, 700, 0, 0, FONTFLAG_DROPSHADOW );
	csgo_surface->SetFontGlyphSet( g::weapon_font, "hooge 05_55 Cyr2", 10, 300, 0, 0, FONTFLAG_OUTLINE );
	csgo_surface->SetFontGlyphSet( g::watermark_font, "hooge 05_55 Cyr2", 12, 300, 0, 0, FONTFLAG_OUTLINE );
		
}

