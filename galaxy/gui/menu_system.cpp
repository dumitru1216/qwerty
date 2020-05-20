#include "menu_system.h"
#include "config_system.h"
#include "..\valve_sdk\Vector.h"
#include "..\valve_sdk\ISurface.h"
#include "..\valve_utils\Color.h"
#include "..\valve_utils\GlobalVars.h"
#include "..\valve_utils\XorStr.h"
#include "../c_hooks.h"
#include "../hooking_mode/directx_font.h"

c_menu menu;

ImFont* Main;
ImFont* MainCaps;
bool show_popup = false;
bool save_config = false;
bool load_config = false;
bool reverse = false;
int offset = 0;

namespace ImGui
{
long get_mils( ) {
	auto duration = std::chrono::system_clock::now( ).time_since_epoch( );
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count( );
}

void begin_popup( const char* text, int on_screen_mils, bool* done ) {
	if (!done)
		show_popup = true;

	ImGuiIO& io = ImGui::GetIO( );
	ImGuiStyle& style = ImGui::GetStyle( );
	int width = io.DisplaySize.x;
	static long old_time = -1;
	ImGui::SetNextWindowPos( ImVec2( width - offset, 100 ) );
	style.WindowMinSize = ImVec2( 100.f, 20.f );
	ImGui::Begin( "##PopUpWindow", &show_popup, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize );
	ImVec2 p = ImGui::GetCursorScreenPos( );

	ImGui::GetWindowDrawList( )->AddRectFilledMultiColor( ImVec2( p.x - 15, p.y - 13 ), ImVec2( p.x + ImGui::GetWindowWidth( ), p.y - 16 ), ImColor( 20, 20, 20, 255 ), ImColor( 255, 255, 255, 255 ), ImColor( 255, 255, 255, 255 ), ImColor( 20, 20, 20, 255 ) );
	long current_time_ms = get_mils( );

	ImVec2 text_size = ImGui::CalcTextSize( text );
	ImGui::SetCursorPosY( ImGui::GetWindowHeight( ) / 2 - text_size.y / 2 );
	ImGui::Text( text );

	if (!reverse) {
		if (offset < ImGui::GetWindowWidth( ))
			offset += (ImGui::GetWindowWidth( ) + 5) * ((1000.0f / ImGui::GetIO( ).Framerate) / 100);

		if (offset >= ImGui::GetWindowWidth( ) && old_time == -1) {
			old_time = current_time_ms;
		}
	}

	if (current_time_ms - old_time >= on_screen_mils && old_time != -1)
		reverse = true;

	if (reverse) {
		if (offset > 0)
			offset -= (ImGui::GetWindowWidth( ) + 5) * ((1000.0f / ImGui::GetIO( ).Framerate) / 100);
		if (offset <= 0) {
			offset = 0;
			reverse = false;
			*done = true;
			old_time = -1;
			show_popup = false;
		}
	}

	ImGui::End( );
}

}


void c_menu::apply_fonts( )
{
	ImGui::CreateContext( );

	Main = ImGui::GetIO( ).Fonts->AddFontFromMemoryCompressedTTF( SmallestPixel_compressed_data, SmallestPixel_compressed_size, 10 );
	MainCaps = ImGui::GetIO( ).Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\segoeuil.ttf", 24 );

}

void legit_tab( ) {
	ImGui::Columns( 2, NULL, false );

	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "main 1", ImVec2( 269, 201 ), true );
	{
	



	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "main 2", ImVec2( 269, 152 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::NextColumn( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "main 3", ImVec2( 289, 152 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::Spacing( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

	ImGui::BeginChild( "main 4", ImVec2( 289, 201 ), true );
	{
	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Columns( );
}

void rage_tab( ) {
	ImGui::Columns( 2, NULL, false );

	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "ragebot [ main ]", ImVec2( 269, 201 ), true );
	{
		ImGui::Checkbox( "enable aimbot", &galaxy_vars.cfg.Aimbot );
		ImGui::Checkbox( "automatic stop", &galaxy_vars.cfg.Autostop );


	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );
	ImGui::BeginChild( "ragebot [ settings ]", ImVec2( 269, 152 ), true );
	{
		ImGui::SliderInt( "hitchance", &galaxy_vars.cfg.HitchanceValue, 0, 100, "%.f%%" );
		ImGui::SliderInt( "minimum damage", &galaxy_vars.cfg.Mindmg, 0, 100, "%.f%%" );
		



	}
	ImGui::EndChild( true );

	ImGui::NextColumn( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "ragebot [ multipoint ]", ImVec2( 289, 152 ), true ); {
		const char* hitscan_type[ ] = { "head only", "body only", "full hitscan" };
		ImGui::Combo( "hitscan mode", &galaxy_vars.cfg.hitscan_type, hitscan_type, IM_ARRAYSIZE(hitscan_type) );
		if ( galaxy_vars.cfg.hitscan_type == 2 ) {
			ImGui::Checkbox( "multipoint", &galaxy_vars.cfg.MultiPoint );
			if ( galaxy_vars.cfg.MultiPoint ) {
				ImGui::Checkbox( "head", &galaxy_vars.cfg.head );
				ImGui::SameLine( );
				ImGui::Checkbox( "body", &galaxy_vars.cfg.body );
				ImGui::SameLine( );
				ImGui::Checkbox( "neck", &galaxy_vars.cfg.neck );
				ImGui::Checkbox( "legs", &galaxy_vars.cfg.legs );
				ImGui::SameLine( );
				ImGui::Checkbox( "arms", &galaxy_vars.cfg.arms );
			}
			ImGui::TextColored( ImColor( 255, 0, 0, 255 ), "multipoint scale" );
			ImGui::SliderInt( "head scale", &galaxy_vars.cfg.HeadScale, 0, 100, "%.f%%" );
			ImGui::SliderInt( "body scale", &galaxy_vars.cfg.BodyScale, 0, 100, "%.f%%" );
		}	

		ImGui::TextColored(ImColor(255, 0, 0, 255), "force body" );	
		ImGui::Hotkey( "force bodyaim", &galaxy_vars.cfg.body_key, ImVec2( 30, 40 ) );
	}
	ImGui::EndChild( true );

	ImGui::Spacing( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

	ImGui::BeginChild( "ragebot [ anti-aim ]", ImVec2( 289, 201 ), true ); {
		const char* antiaim_deysnc_yaw[] = { "off", "backwars", "static desync" };
		const char* fakelag_type[ ] = { "off", "always", "teleport", "switch packets" };

		ImGui::Checkbox( "enable anti-aim", &galaxy_vars.cfg.Antiaim );
		ImGui::Combo( "yaw/desync", &galaxy_vars.cfg.yaw_desync, antiaim_deysnc_yaw, IM_ARRAYSIZE(antiaim_deysnc_yaw) );
		if (galaxy_vars.cfg.yaw_desync == 2) {
			ImGui::Hotkey( "desync swap", &galaxy_vars.cfg.desync_swap, ImVec2(30, 40) );
			ImGui::SliderInt( "body lean", &galaxy_vars.cfg.body_lean, 0, 300, "%.f%%" );
			ImGui::SliderInt( "body invert", &galaxy_vars.cfg.body_invert, 0, 300, "%.f%%" );
		}
		ImGui::Checkbox( "fakelag", &galaxy_vars.cfg.fake_lag );
		if ( galaxy_vars.cfg.fake_lag ) {
			ImGui::SliderInt( "fakelag ticks", &galaxy_vars.cfg.Fakelag, 0, 16, "%.f%%" );
			ImGui::Checkbox( "ddos on peek", &galaxy_vars.cfg.FakeLagOnPeek );
			ImGui::TextColored( ImColor( 255, 0, 0, 255 ), "teleport fakelag disabled" );
		}
	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Columns( );
}

void visuals_tab( ) {
	ImGui::Columns( 2, NULL, false );

	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "visuals [ players ]", ImVec2( 269, 201 ), true ); {
	
		ImGui::Checkbox( "enable visuals", &galaxy_vars.cfg.Esp );
		ImGui::Checkbox( "bounding box", &galaxy_vars.cfg.Box );
		ImGui::SameLine( );
		ImGui::ColorEdit4( "###box_color", galaxy_vars.cfg.box_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
		ImGui::Checkbox( "name", &galaxy_vars.cfg.name );
		ImGui::SameLine( );
		ImGui::ColorEdit4( "###name_color", galaxy_vars.cfg.name_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
		ImGui::Checkbox( "health bar", &galaxy_vars.cfg.HealthBar );
		ImGui::Checkbox( "weapon", &galaxy_vars.cfg.weapon_name );
		ImGui::Checkbox( "ammo bar", &galaxy_vars.cfg.ammo_bar );
		ImGui::SameLine( );
		ImGui::ColorEdit4( "###ammo_color", galaxy_vars.cfg.ammo_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
		ImGui::Checkbox( "flags", &galaxy_vars.cfg.flags );


	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "visuals [ chams ]", ImVec2( 269, 152 ), true );
	{
		static int chams_sub = 0;
		if (ImGui::Button( "enemy", ImVec2( 50, 15 ) ) )
		{
			chams_sub = 0;
		}
		ImGui::SameLine( );
		if (ImGui::Button( "local", ImVec2( 50, 15 ) ))
		{
			chams_sub = 1;
		}
		ImGui::SameLine( );
		if (ImGui::Button( "other", ImVec2( 50, 15 ) ))
		{
			chams_sub = 2;
		}
		ImGui::SameLine( );
		if (ImGui::Button( "glow", ImVec2( 50, 15 ) ))
		{
			chams_sub = 3;
		}

		const char* chams_material[] = { "textured", "eso", "glow" };

		if (chams_sub == 0)
		{
			ImGui::Checkbox( "enemy chams", &galaxy_vars.cfg.chams_enemy );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###normal_chamsenem", galaxy_vars.cfg.enemy_visible_colors, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Checkbox( "enemy chams invisible", &galaxy_vars.cfg.enemy_invisible );
			ImGui::Combo( "enemy materials [ normal ]", &galaxy_vars.cfg.normal_enemy_material, chams_material, IM_ARRAYSIZE( chams_material ) );
			ImGui::Checkbox( "enemy chams overide", &galaxy_vars.cfg.enemy_overide_chams );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###overidecolorenemy", galaxy_vars.cfg.enemy_overide_colors, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Combo( "enemy materials [ overide ]", &galaxy_vars.cfg.overide_enemy_material, chams_material, IM_ARRAYSIZE( chams_material ) );


		}

		if (chams_sub == 1)
		{
			ImGui::Checkbox( "local chams", &galaxy_vars.cfg.local_chams );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###noremal_locachams", galaxy_vars.cfg.local_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Combo( "local materials [ normal ]", &galaxy_vars.cfg.local_materials, chams_material, IM_ARRAYSIZE( chams_material ) );
			ImGui::Checkbox( "local chams overide", &galaxy_vars.cfg.local_chams_overide );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###localcamsoveride", galaxy_vars.cfg.local_overide_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Combo( "local materials [ overide ]", &galaxy_vars.cfg.local_overide_materials, chams_material, IM_ARRAYSIZE( chams_material ) );

			ImGui::Checkbox( "desync chams", &galaxy_vars.cfg.desync_chams );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###desyncncamayte", galaxy_vars.cfg.desync_chams_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Combo( "desync materials [ normal ]", &galaxy_vars.cfg.desync_chams_material, chams_material, IM_ARRAYSIZE( chams_material ) );
			ImGui::Checkbox( "desync chams overide", &galaxy_vars.cfg.deysnc_Overid );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###desocverdidechams", galaxy_vars.cfg.desync_chams_overide, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Combo( "desync materials [ overide ]", &galaxy_vars.cfg.desync_chams_overide_material, chams_material, IM_ARRAYSIZE( chams_material ) );


		}

		if (chams_sub == 2)
		{
			ImGui::Checkbox( "hand chams", &galaxy_vars.cfg.hand_chams );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###handnormalcaolor", galaxy_vars.cfg.hand_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Combo( "hand materials [ normal ]", &galaxy_vars.cfg.hand_materials, chams_material, IM_ARRAYSIZE( chams_material ) );
			ImGui::Checkbox( "hand chams overide", &galaxy_vars.cfg.hand_chams_overide );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###handoveridecolor", galaxy_vars.cfg.hand_overide_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
			ImGui::Combo( "hand materials [ overide ]", &galaxy_vars.cfg.hand_materials_overide, chams_material, IM_ARRAYSIZE( chams_material ) );


		}

		if (chams_sub == 3)
		{

		}



	}
	ImGui::EndChild( true );

	ImGui::NextColumn( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "visuals [ other ]", ImVec2( 289, 152 ), true );
	{
		ImGui::Checkbox( "world modulation", &galaxy_vars.cfg.NightMode );
		ImGui::SliderInt( "nighmode scale", &galaxy_vars.cfg.NightModeSlider, 0, 100, "%.f%%" );
		ImGui::Text( "sky color" );
		ImGui::SameLine( );
		ImGui::ColorEdit4( "###skycolor", galaxy_vars.cfg.sky_box_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs );
		ImGui::Checkbox( "top bar", &galaxy_vars.cfg.top_bar_enable );
		ImGui::Checkbox( "spectators list", &galaxy_vars.cfg.specator_list );

	}
	ImGui::EndChild( true );

	ImGui::Spacing( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

	ImGui::BeginChild( "Effects", ImVec2( 289, 201 ), true );
	{
		ImGui::Checkbox( "remove scope", &galaxy_vars.cfg.NoScope );
		ImGui::Checkbox( "remove visual recoil", &galaxy_vars.cfg.NoRecoil );
		ImGui::Checkbox( "remove zoom", &galaxy_vars.cfg.NoZoom );
		ImGui::Checkbox( "remove smoke", &galaxy_vars.cfg.no_smoke );
		
	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Columns( );
}

void misc_tab( ) {
	ImGui::Columns( 2, NULL, false );

	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "misc [ main ]", ImVec2( 269, 361 ), true );
	{
		ImGui::Checkbox( "automatic bhop", &galaxy_vars.cfg.Bhop );
		ImGui::Checkbox( "automatic strafe", &galaxy_vars.cfg.AutoStrafe );
	}
	ImGui::EndChild( true );
	ImGui::NextColumn( );
	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "misc [ other ]", ImVec2( 289, 152 ), true );
	{
		ImGui::Text( "menu color" );
		ImGui::SameLine( );
		ImGui::ColorEdit4( "###mmen0", galaxy_vars.cfg.menu_color, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs );
		ImGui::Checkbox( "slowwalk", &galaxy_vars.cfg.slowwalk );
		if (galaxy_vars.cfg.slowwalk)
		{
			ImGui::SliderInt( "slowwalk speed", &galaxy_vars.cfg.slowspeed, 0, 100, "%.f%%" );
		}
		ImGui::Checkbox( "server hitboxes", &galaxy_vars.cfg.server_hitobx );
	}
	ImGui::EndChild( true );

	ImGui::Spacing( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

	ImGui::BeginChild( "misc [ premium ]", ImVec2( 289, 201 ), true );
	{
		ImGui::Checkbox( "airstuck exploit", &galaxy_vars.cfg.air_stuck );
		if (galaxy_vars.cfg.air_stuck)
		{
			ImGui::Hotkey( "airstuck key", &galaxy_vars.cfg.air_key, ImVec2( 30, 40 ) );
		}
		ImGui::Checkbox( "air desync exploit", &galaxy_vars.cfg.air_desync );
	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Columns( );
}

void config_tab( ) {
	ImGui::Columns( 2, NULL, false );

	ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
	ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

	ImGui::BeginChild( "configs [ configs ]", ImVec2( 269, 361 ), true );
	{
		constexpr auto& config_items = galaxy_vars.get_configs( );
		static int current_config = -1;

		if (static_cast<size_t>(current_config) >= config_items.size( ))
			current_config = -1;

		static char buffer[16];

		if (ImGui::ListBox( "", &current_config, []( void* data, int idx, const char** out_text ) {
			auto& vector = *static_cast<std::vector<std::string>*>(data);
			*out_text = vector[idx].c_str( );
			return true;
		}, &config_items, config_items.size( ), 5 ) && current_config != -1)
			strcpy( buffer, config_items[current_config].c_str( ) );

		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 18 );
		ImGui::PushID( 0 );
		ImGui::PushItemWidth( 178 );
		if (ImGui::InputText( "", buffer, IM_ARRAYSIZE( buffer ), ImGuiInputTextFlags_EnterReturnsTrue )) {
			if (current_config != -1)
				galaxy_vars.rename( current_config, buffer );
		}
		ImGui::PopID( );
		ImGui::NextColumn( );

		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 18 );
		if (ImGui::Button( ("create"), ImVec2( 85, 20 ) )) {
			galaxy_vars.add( buffer );
		}

		ImGui::SameLine( );

		if (ImGui::Button( ("reset"), ImVec2( 85, 20 ) )) {
			galaxy_vars.reset( );
		}

		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 18 );
		if (current_config != -1) {
			if (ImGui::Button( ("load"), ImVec2( 85, 20 ) )) {
				galaxy_vars.load( current_config );

				load_config = true;

			}

			ImGui::SameLine( );

			if (ImGui::Button( ("save"), ImVec2( 85, 20 ) )) {
				galaxy_vars.save( current_config );

				save_config = true;

			}

			ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 18 );
			if (ImGui::Button( ("remove"), ImVec2( 85, 20 ) )) {
				galaxy_vars.remove( current_config );
			}
		}
	}
	ImGui::EndChild( true );
	ImGui::NextColumn( );
	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

	ImGui::BeginChild( "config [ lua -> presets ]", ImVec2( 289, 152 ), true );
	{

	}
	ImGui::EndChild( true );

	ImGui::Spacing( );

	ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
	ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

	ImGui::BeginChild( "config [ lua -> custom ]", ImVec2( 289, 201 ), true );
	{

	}
	ImGui::EndChild( true );

	ImGui::PopStyleVar( );
	ImGui::Columns( );
}

void c_menu::Render( )
{
	ImGui::GetIO( ).MouseDrawCursor = menuOpened;

	static int page = 0;

	if (menuOpened) {

		

		ImGui::GetStyle( ).Colors[ImGuiCol_CheckMark] = ImVec4( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2], 1.f );
		ImGui::GetStyle( ).Colors[ImGuiCol_SliderGrab] = ImVec4( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2], 1.f );
		ImGui::GetStyle( ).Colors[ImGuiCol_SliderGrabActive] = ImVec4( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2], 1.f );

		ImGui::SetNextWindowSize( ImVec2( 600, 455 ), ImGuiSetCond_FirstUseEver );

		ImGui::Begin( "angeldust", &menuOpened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar ); {
			ImVec2 p = ImGui::GetCursorScreenPos( );
			ImColor c = ImColor( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2] );

			ImGui::GetWindowDrawList( )->AddRectFilled( ImVec2( p.x, p.y + 3 ), ImVec2( p.x + ImGui::GetWindowWidth( ), p.y + -3 ), ImColor( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2] ) );
			ImGui::GetWindowDrawList( )->AddRectFilledMultiColor( ImVec2( p.x, p.y + 3 ), ImVec2( p.x + ImGui::GetWindowWidth( ) / 2, p.y + -3 ), ImColor( 0, 0, 0, 125 ), ImColor( 0, 0, 0, 15 ), ImColor( 0, 0, 0, 15 ), ImColor( 0, 0, 0, 125 ) );
			ImGui::GetWindowDrawList( )->AddRectFilledMultiColor( ImVec2( p.x + ImGui::GetWindowWidth( ) / 2, p.y + 3 ), ImVec2( p.x + ImGui::GetWindowWidth( ), p.y + -3 ), ImColor( 0, 0, 0, 15 ), ImColor( 0, 0, 0, 125 ), ImColor( 0, 0, 0, 125 ), ImColor( 0, 0, 0, 15 ) );
			ImGui::GetWindowDrawList( )->AddLine( ImVec2( p.x, p.y + 2 ), ImVec2( p.x + ImGui::GetWindowWidth( ), p.y + 2 ), ImColor( 0, 0, 0, 150 ) );

		

			ImGui::PushFont( MainCaps );
			ImGui::Dummy( ImVec2( 250, 0 ) ); ImGui::SameLine( );
			ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 7 );
			ImGui::TextColored( ImVec4( galaxy_vars.cfg.menu_color[0], galaxy_vars.cfg.menu_color[1], galaxy_vars.cfg.menu_color[2], 1.f ), "deathrow" );
			ImGui::PopFont( );

			ImGui::PushFont( Main );
			ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
			ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

			ImGui::BeginChild( "tabs", ImVec2( 568, 30 ), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar );
			{
				if (ImGui::ButtonT( "ragebot", ImVec2( 95, -1 ), page, 0, c, false )) page = 0; ImGui::SameLine( 0, 0 );
				if (ImGui::ButtonT( "legitbot", ImVec2( 95, -1 ), page, 1, c, false )) page = 1; ImGui::SameLine( 0, 0 );
				if (ImGui::ButtonT( "visuals", ImVec2( 95, -1 ), page, 2, c, false )) page = 2; ImGui::SameLine( 0, 0 );
				if (ImGui::ButtonT( "misc", ImVec2( 95, -1 ), page, 3, c, false )) page = 3; ImGui::SameLine( 0, 0 );
				if (ImGui::ButtonT( "config & lua", ImVec2( 95, -1 ), page, 4, c, false )) page = 4; ImGui::SameLine( 0, 0 );
				if (ImGui::ButtonT( "skins", ImVec2( 95, -1 ), page, 5, c, false )) page = 5; ImGui::SameLine( 0, 0 );
			}
			ImGui::EndChild( );
			ImGui::PopFont( );

			ImGui::PushFont( Main );

			switch (page) {
			case 0: rage_tab( );
				break;
			case 1: legit_tab( );
				break;
			case 2: visuals_tab( );
				break;
			case 3: misc_tab( );
				break;
			case 4: config_tab( );
				break;
			}

			ImGui::PopFont( );
		}
		ImGui::End( );

	}
}

void c_menu::run_popup( )
{
	ImGui::PushFont( Main );
	ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );
	ImGui::PushStyleColor( ImGuiCol_ChildWindowBg, ImVec4( 30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f ) );
	ImGui::PushStyleColor( ImGuiCol_Border, ImVec4( 0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f ) );

	if (save_config) {
		bool done = false;
		ImGui::begin_popup( "config saved.", 2000, &done );
		if (done)
			save_config = false;
	}

	if (load_config) {
		bool done = false;
		ImGui::begin_popup( "config loaded.", 2000, &done );
		if (done)
			load_config = false;
	}

	ImGui::PopStyleColor( );
	ImGui::PopStyleColor( );
	ImGui::PopStyleVar( );
	ImGui::PopFont( );
}
