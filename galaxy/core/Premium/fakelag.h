#pragma once
#include "../../c_hooks.h"
#include "../../valve_utils/GlobalVars.h"
#include "../../gui/config_system.h"
#include "../Aimbot/Autowall.h"
#include "../Aimbot/c_ragebot.h"

static constexpr int max_chocke = 14;
static constexpr float teleport_distance = 64.f;

class fake_lag
{
public:
	int fakelag_factor( );
	void do_fakelag(  );
	void new_fakelag( );
};
extern fake_lag fakelag;