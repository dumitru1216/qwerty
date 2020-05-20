#include "../c_hooks.h"
#include "../core/external_inclues.h"

void Event::FireGameEvent( IGameEvent* event )
{
	if (!event) return;
	if (!g::pLocalEntity) return;

	auto attacker = csgo_entity_list->GetClientEntity( csgo_engine->GetPlayerForUserID( event->GetInt( "attacker" ) ) );
	if (!attacker) return;
	if (attacker != g::pLocalEntity) return;

	int index = csgo_engine->GetPlayerForUserID( event->GetInt( "userid" ) );

	PlayerInfo_t pInfo;
	csgo_engine->GetPlayerInfo( index, &pInfo );

	g::Hit[index] = true;
};