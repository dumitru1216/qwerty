#include "../c_hooks.h"
#include "../core/external_inclues.h"

extern LRESULT ImGui_ImplDX9_WndProcHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
LRESULT c_hooks::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static bool pressed = false;

	if (!pressed && GetAsyncKeyState( VK_INSERT )) {
		pressed = true;
	}
	else if (pressed && !GetAsyncKeyState( VK_INSERT )) {
		pressed = false;

		menu.menuOpened = !menu.menuOpened;
	}

	if (menu.menuOpened && ImGui_ImplDX9_WndProcHandler( hWnd, uMsg, wParam, lParam ))
		return true;


	return CallWindowProcA( hooks.pOriginalWNDProc, hWnd, uMsg, wParam, lParam );
}
