#include <thread>
#include "c_hooks.h"
#include "valve_utils\Utils.h"
#include "core\external_inclues.h"
#include "gui\menu_system.h"
#include "valve_sdk\Hitboxes.h"
#include "libloaderapi.h"
#include "hooking_mode/hooking_init.h"

Misc     g_Misc;
c_hooks    hooks;
Event    g_Event;

void c_hooks::vmt_hooks( )
{
	//dx9 interface
	DWORD shaderapidx9 = **(DWORD * *)(Utils::FindSignature( "shaderapidx9.dll", "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C" ) + 1);

	hooks.pClientHook = std::make_unique<VMTHook>( csgo_client_dll );
	hooks.pClientModeHook = std::make_unique<VMTHook>( csgo_client_mode );
	hooks.pSurfaceHook = std::make_unique<VMTHook>( csgo_surface );
	hooks.pPanelHook = std::make_unique<VMTHook>( csgo_panel );
	hooks.pModelHook = std::make_unique<VMTHook>( csgo_model_render );
	hooks.pRenderViewHook = std::make_unique<VMTHook>( csgo_render_view );
	hooks.D3DHook = std::make_unique<VMTHook>( (DWORD**)shaderapidx9 );
}
void c_hooks::table_functions( )
{
	hooks.pClientHook->Hook( vtable_indexes::frameStage, c_hooks::FrameStageNotify );
	hooks.pClientModeHook->Hook( vtable_indexes::createMove, c_hooks::CreateMove );
	hooks.pClientModeHook->Hook( vtable_indexes::view, c_hooks::OverrideView );
	hooks.pSurfaceHook->Hook( vtable_indexes::lockCursor, c_hooks::LockCursor );
	hooks.pPanelHook->Hook( vtable_indexes::paint, c_hooks::PaintTraverse );
	hooks.pModelHook->Hook( vtable_indexes::dme, c_hooks::DrawModelExecute );
	hooks.pRenderViewHook->Hook( vtable_indexes::sceneEnd, c_hooks::SceneEnd );
	hooks.D3DHook->Hook( vtable_indexes::end_scene, c_hooks::Hooked_EndScene );
	hooks.D3DHook->Hook( vtable_indexes::end_scene_reset, c_hooks::Hooked_EndScene_Reset );
}
void c_hooks::wnd_hook( )
{
	if (hooks.hCSGOWindow)        // Hook WNDProc to capture mouse / keyboard input
		hooks.pOriginalWNDProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr( hooks.hCSGOWindow, GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(hooks.WndProc) ));

}

void c_hooks::Init()
{
	c_hooking->window_handle( );

    interfaces::Init();                         
    g_pNetvars = std::make_unique<NetvarTree>();
	wnd_hook( );

	vmt_hooks( );
	table_functions( );
	g_Event.Init();
	HookPlayers( );
	c_hooking->init_fonts( );

    Utils::Log("Hooking completed!");
}

void c_hooks::Restore()
{
	Utils::Log("Unhooking in progress...");
    {   // Unhook every function we hooked and restore original one
		hooks.pClientHook->Unhook(vtable_indexes::frameStage);
        hooks.pClientModeHook->Unhook(vtable_indexes::createMove);
		hooks.pClientModeHook->Unhook(vtable_indexes::view);
        hooks.pSurfaceHook->Unhook(vtable_indexes::lockCursor);
		hooks.pPanelHook->Unhook(vtable_indexes::paint);
		hooks.pModelHook->Unhook(vtable_indexes::dme);

        SetWindowLongPtr(hooks.hCSGOWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks.pOriginalWNDProc));

        g_pNetvars.reset();   /* Need to reset by-hand, global pointer so doesnt go out-of-scope */
    }
    Utils::Log("Unhooking succeded!");
}