#pragma once
#include "..\..\valve_utils\GlobalVars.h"
#include "..\..\valve_sdk\CGlobalVarsBase.h"

class c_antiaim
{
public:
	void create_move();
	void desyncchams();
	void yaw_desync( );
	void smallmovements( );
};
extern c_antiaim antiaim;