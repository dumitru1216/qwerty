#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>
#include "config_system.h"

class c_menu
{
public:
	bool menuOpened = false;
	void Render( );
	void apply_fonts( );
	void run_popup( );
	void KeyStates( );
};
extern c_menu menu;