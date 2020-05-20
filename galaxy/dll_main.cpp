#include <thread>
#include "c_hooks.h"
#include "valve_utils\Utils.h"
#include "valve_utils\GlobalVars.h"
#include "gui/config_system.h"


HINSTANCE HThisModule;

DWORD WINAPI attach( void* instance ) {
	while (!GetModuleHandleW( L"serverbrowser.dll" ))
		Sleep( 200 );


	try {
		
		c_hooks::Init( );
		
	}

	catch (const std::runtime_error & err) {
		MessageBoxA( NULL, err.what( ), "mortiilui error!", MB_OK | MB_ICONERROR );
		
		FreeLibraryAndExitThread( static_cast<HMODULE>(instance), 0 );
	}

	galaxy_vars.run( "deathrow" );



	while (!GetAsyncKeyState( VK_HOME ))
		std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );

	FreeLibraryAndExitThread( static_cast<HMODULE>(instance), 0 );
}

BOOL WINAPI detach( ) {
	c_hooks::Restore( );


	return TRUE;
}

BOOL APIENTRY DllMain( void* instance, uintptr_t reason, void* reserved ) {
	DisableThreadLibraryCalls( static_cast<HMODULE>(instance) );

	switch (reason) {
	case DLL_PROCESS_ATTACH:
		CreateThread( nullptr, NULL, attach, instance, NULL, nullptr );
		break;

	case DLL_PROCESS_DETACH:
		detach( );
		break;
	}

	return true;
}