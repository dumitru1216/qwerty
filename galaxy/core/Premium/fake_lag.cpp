#include "fakelag.h"

/* variables */
int csgo_chocke = 1;

/* extern of class */
fake_lag fakelag;

int fake_lag::fakelag_factor( )
{
	/* get local player and return it */
	auto local_player = g::pLocalEntity;
	if ( !local_player ) return 0;

	/* get velocity */
	auto csgo_velocity = local_player->get_velocity( );
	
	/* get speed */
	auto csgo_speed = csgo_velocity.Length( );

	/* get distance */
	auto distance_per_tick = csgo_speed * csgo_globals->interval_per_tick;

	/* return */
	return 0;
}

void fake_lag::do_fakelag( )
{
	/* get local player */
	auto local = g::pLocalEntity;
	if ( !local ) return;

	/* fix crash when voice recording */
	if ( csgo_engine->is_voice_recording( ) ) return;

	/* fakelag on shoot : mb layer */


	/* net channel */
	auto csgo_net_channel = csgo_engine->get_network_channel( );
	if ( !csgo_net_channel ) return;

	/* flags */
	bool standing = local->get_velocity( ).Length2D( ) <= 0.1f;
	bool accelerating = local->get_velocity( ).Length2D( ) >= 0.1f && local->get_velocity( ).Length2D( ) <= 150.f;
	bool onhighspeed = local->get_velocity( ).Length2D( ) >= 265.f;
	bool onground = local->get_flags( ) & fl_onground;
	bool running = onground && accelerating || local->get_velocity( ).Length2D( ) >= 175.f;

	/* variables for flags */
	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;
	bool flag4 = false;

	/* enable flags */
	if ( galaxy_vars.cfg.fake_lag && ( onground && standing && accelerating && onhighspeed && running) )
	{
		flag1 = true;
		flag2 = true;
		flag3 = true;
		flag4 = true;
	}

	/* wehn fakeduck set chockes to 14 */
	/* not now */

	//for down thing /* fix flags */ ( if dont work without)
	/*  
	
		else
		return;

	*/

	/* fix flags */
	if ( flag1 || flag2 || flag3 || flag4 )
		csgo_chocke = galaxy_vars.cfg.Fakelag;

	/* finish fakelag */
	if ( galaxy_vars.cfg.fakelag_type == 1 )
	{
		if ( csgo_net_channel->m_nChokedPackets < csgo_chocke )
		{
			/* send packet off */
			g::bSendPacket = false;
		}
		else
		{
			/* send packet on */
			g::bSendPacket = true;
		}
	}
	else if ( galaxy_vars.cfg.fakelag_type == 2 )
	{
		/* get velocity */
		auto csgo_velocity = local->get_velocity( );

		/* get speed */
		auto csgo_speed = csgo_velocity.Length( );

		/* get distance */
		auto distance_per_tick = csgo_speed * csgo_globals->interval_per_tick;

		/* im dont gonna force chockes for fakeduck */
		if ( csgo_velocity.Length2D( ) > 0 )
			csgo_chocke = std::min<int>( std::ceilf( teleport_distance / distance_per_tick ), galaxy_vars.cfg.Fakelag );
		else
			csgo_chocke = galaxy_vars.cfg.Fakelag;

		/* fix net channel */
		if ( csgo_net_channel->m_nChokedPackets < csgo_chocke )
		{
			/* send packet off */
			g::bSendPacket = false;
		}
		else
		{
			/* send packet on */
			g::bSendPacket = true;
		}
	}
	else if ( galaxy_vars.cfg.fakelag_type )
	{
		/* next fakelag update time */
		static float next_update_time = 0;

		/* random bool */
		bool yo = false;

		/* fix packets */
		if ( next_update_time < csgo_globals->cur_time )
		{
			next_update_time = csgo_globals->cur_time + 1.1f;
			yo = true;
		}

		/* finish packets */
		if ( csgo_net_channel->m_nChokedPackets < csgo_chocke && yo )
		{
			/* send packet off */
			g::bSendPacket = false;
		}
		else
		{
			/* send packet on */
			g::bSendPacket = true;
		}
	}
}

void fake_lag::new_fakelag( )
{
	/* calls */
	if ( !csgo_engine->IsConnected( ) || !csgo_engine->in_game( ) || galaxy_vars.cfg.Fakelag == 0  ) return;
	if ( !g::pLocalEntity->IsAlive( ) ) return;
	if ( g::pLocalEntity->IsKnifeorNade( ) ) return;
	auto NetChannel = csgo_engine->get_network_channel( );
	if ( !NetChannel ) return;
	

	static float maxSpeed = 320.f;
	static float Acceleration = 5.5f;
	float maxAccelspeed = Acceleration * maxSpeed * csgo_globals->interval_per_tick;

	float TicksToStop = g::pLocalEntity->get_velocity( ).Length( ) / maxAccelspeed;

	bool canHit = false;
	static bool init = false;
	static bool stop = true;
	static bool stop2 = true;
	bool skip = false;

	if ( galaxy_vars.cfg.FakeLagOnPeek )
	{
		for ( int i = 1; i < csgo_engine->GetMaxClients( ); ++i )
		{
			C_BaseEntity* pPlayerEntity = csgo_entity_list->GetClientEntity( i );

			if ( !pPlayerEntity
				|| !pPlayerEntity->IsAlive( )
				|| pPlayerEntity->IsDormant( )
				|| pPlayerEntity == g::pLocalEntity
				|| pPlayerEntity->GetTeam( ) == g::pLocalEntity->GetTeam( ) )
				continue;

			Vector EnemyHead = { pPlayerEntity->GetOrigin( ).x, pPlayerEntity->GetOrigin( ).y, ( pPlayerEntity->GetHitboxPosition( 0, aimbot.Matrix[ pPlayerEntity->EntIndex( ) ] ).z + 10.f ) };

			Vector Head = { g::pLocalEntity->GetOrigin( ).x, g::pLocalEntity->GetOrigin( ).y, ( g::pLocalEntity->GetHitboxPosition( 0, aimbot.Matrix[ pPlayerEntity->EntIndex( ) ] ).z + 10.f ) };
			Vector HeadExtr = ( Head + ( g::pLocalEntity->get_velocity( ) * 0.203125f ) );
			Vector OriginExtr = ( ( g::pLocalEntity->GetOrigin( ) + ( g::pLocalEntity->get_velocity( ) * 0.21875f ) ) + Vector( 0, 0, 8 ) );

			float dmg;

			if ( abs( g::pLocalEntity->get_velocity( ).Length2D( ) ) > 50.f && ( g_Autowall.CanHitFloatingPoint( HeadExtr, EnemyHead ) || g_Autowall.CanHitFloatingPoint( OriginExtr, EnemyHead ) ) )
			{
				canHit = true;
			}
		}

		if ( canHit )
		{
			if ( stop2 )
			{
				init = true;
				stop2 = false;
			}
		}
		else
			stop2 = true;

		if ( init )
		{
			if ( !stop )
			{
				g::bSendPacket = true;
				g::LagPeek = true;
				stop = true;
				skip = true;
			}
		}

		if ( !skip )
		{
			if ( g::LagPeek )
			{
				if ( NetChannel->m_nChokedPackets < 13 )
					g::bSendPacket = false;
				else
					g::LagPeek = false;
			}
			else
			{
				g::bSendPacket = ( NetChannel->m_nChokedPackets >= galaxy_vars.cfg.Fakelag );
				stop = false;
				init = false;
			}
		}
	}
	else
	{
		init = false;
		stop = true;
		stop2 = true;
		g::LagPeek = false;

		g::bSendPacket = ( NetChannel->m_nChokedPackets >= galaxy_vars.cfg.Fakelag );

		if ( GetAsyncKeyState( VK_SHIFT ) )
			g::bSendPacket = ( NetChannel->m_nChokedPackets >= 13 );
	}
}

