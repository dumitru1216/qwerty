#include "c_ragebot.h"

c_aimbot aimbot;

void c_aimbot::Autostop( )
{
	if (!galaxy_vars.cfg.Autostop) return;

	Vector Velocity = g::pLocalEntity->get_velocity( );
	static float Speed = 450.f;

	Vector Direction;
	Vector RealView;
	g_Math.VectorAngles( Velocity, Direction );
	csgo_engine->GetViewAngles( RealView );
	Direction.y = RealView.y - Direction.y;

	Vector Forward;
	g_Math.AngleVectors( Direction, &Forward );
	Vector NegativeDirection = Forward * -Speed;

	g::pCmd->forwardmove = NegativeDirection.x;
	g::pCmd->sidemove = NegativeDirection.y;
}

bool c_aimbot::HitChance( C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance )
{

	float Seeds = (galaxy_vars.cfg.Hitchance == 1) ? 356.f : 256.f;

	Angle -= (g::pLocalEntity->GetAimPunchAngle( ) * csgo_cvar->FindVar( "weapon_recoil_scale" )->GetFloat( ));

	Vector forward, right, up;

	g_Math.AngleVectors( Angle, &forward, &right, &up );

	int Hits = 0, neededHits = (Seeds * (chance / 100.f));

	float weapSpread = pWeapon->GetSpread( ), weapInaccuracy = pWeapon->GetInaccuracy( );

	for (int i = 0; i < Seeds; i++)
	{
		float Inaccuracy = g_Math.RandomFloat( 0.f, 1.f ) * weapInaccuracy;
		float Spread = g_Math.RandomFloat( 0.f, 1.f ) * weapSpread;

		Vector spreadView( (cos( g_Math.RandomFloat( 0.f, 2.f * M_PI ) ) * Inaccuracy) + (cos( g_Math.RandomFloat( 0.f, 2.f * M_PI ) ) * Spread), (sin( g_Math.RandomFloat( 0.f, 2.f * M_PI ) ) * Inaccuracy) + (sin( g_Math.RandomFloat( 0.f, 2.f * M_PI ) ) * Spread), 0 ), direction;
		direction = Vector( forward.x + (spreadView.x * right.x) + (spreadView.y * up.x), forward.y + (spreadView.x * right.y) + (spreadView.y * up.y), forward.z + (spreadView.x * right.z) + (spreadView.y * up.z) ).Normalize( );

		Vector viewanglesSpread, viewForward;

		g_Math.VectorAngles( direction, up, viewanglesSpread );
		g_Math.NormalizeAngles( viewanglesSpread );

		g_Math.AngleVectors( viewanglesSpread, &viewForward );
		viewForward.NormalizeInPlace( );

		viewForward = g::pLocalEntity->GetEyePosition( ) + (viewForward * pWeapon->GetCSWpnData( )->range);

		C_Trace Trace;

		csgo_trace->ClipRayToEntity( C_Ray( g::pLocalEntity->GetEyePosition( ), viewForward ), mask_shot | contents_grate, pEnt, &Trace );

		if (Trace.m_pEnt == pEnt)
			Hits++;

		if (((Hits / Seeds) * 100.f) >= chance)
			return true;

		if ((Seeds - i + Hits) < neededHits)
			return false;
	}

	return false;
}

Vector c_aimbot::Hitscan( C_BaseEntity* pEnt ) {
	/* variables */
	float DamageArray[28];
	float tempDmg = 0.f;
	Vector tempHitbox = { 0,0,0 };
	static int HitboxForMuti[] = { 2,2,4,4,6,6 };
	float angToLocal = g_Math.CalcAngle( g::pLocalEntity->GetOrigin( ), pEnt->GetOrigin( ) ).y;

	/* vectoring multipoint */
	Vector2D mutipointXY = { (sin( g_Math.GRD_TO_BOG( angToLocal ) )),(cos( g_Math.GRD_TO_BOG( angToLocal ) )) };
	Vector2D mutipointXY180 = { (sin( g_Math.GRD_TO_BOG( angToLocal + 180 ) )) ,(cos( g_Math.GRD_TO_BOG( angToLocal + 180 ) )) };
	Vector2D mutipoint[] = { Vector2D( mutipointXY.x, mutipointXY.y ), Vector2D( mutipointXY180.x, mutipointXY180.y ) };
	std::vector<int> Scan;
	int HeadHeight = 0;

	/* velocity shot improving accuracy */
	float csgo_velocity = abs( pEnt->get_velocity( ).Length2D( ) );
	if ( !galaxy_vars.cfg.DelayShot && csgo_velocity > 29.f )
		csgo_velocity = 30.f;

	/* hitscan */
	if ( csgo_velocity <= 215.f) {
		/* hitscan type */
		switch ( galaxy_vars.cfg.hitscan_type )
		{
			/* head only */
			case 0: { 
				Scan.push_back( ( int )HitboxList::Head );
			}
			/* hitscan without head aim only */
			case 1: {
				Scan.push_back( ( int )HitboxList::Pelvis );
				Scan.push_back( ( int )HitboxList::Stomach );
				Scan.push_back( ( int )HitboxList::Chest );
				Scan.push_back( ( int )HitboxList::UpperChest );
				Scan.push_back( ( int )HitboxList::LeftUpperArm );
				Scan.push_back( ( int )HitboxList::RightUpperArm );
				Scan.push_back( ( int )HitboxList::LeftLowerArm );
				Scan.push_back( ( int )HitboxList::RightLowerArm );
				Scan.push_back( ( int )HitboxList::Neck );
				Scan.push_back( ( int )HitboxList::LeftThigh );
				Scan.push_back( ( int )HitboxList::RightThigh );
				Scan.push_back( ( int )HitboxList::LeftFoot );
				Scan.push_back( ( int )HitboxList::RightFoot );
				Scan.push_back( ( int )HitboxList::LeftShin );
				Scan.push_back( ( int )HitboxList::RightShin );
			}
			/* normal hitscan */
			case 2: {
				if ( galaxy_vars.cfg.MultiPoint )
				{
					if ( galaxy_vars.cfg.head )
					{
						Scan.push_back( ( int )HitboxList::Head );
					}
					if ( galaxy_vars.cfg.body )
					{
						Scan.push_back( ( int )HitboxList::Pelvis );
						Scan.push_back( ( int )HitboxList::Stomach );
						Scan.push_back( ( int )HitboxList::Chest );
						Scan.push_back( ( int )HitboxList::UpperChest );
					}
					if ( galaxy_vars.cfg.arms )
					{
						Scan.push_back( ( int )HitboxList::LeftUpperArm );
						Scan.push_back( ( int )HitboxList::RightUpperArm );
						Scan.push_back( ( int )HitboxList::LeftLowerArm );
						Scan.push_back( ( int )HitboxList::RightLowerArm );
					}
					if ( galaxy_vars.cfg.neck )
					{
						Scan.push_back( ( int )HitboxList::Neck );
					}
					if ( galaxy_vars.cfg.legs )
					{
						Scan.push_back( ( int )HitboxList::LeftThigh );
						Scan.push_back( ( int )HitboxList::RightThigh );
						Scan.push_back( ( int )HitboxList::LeftFoot );
						Scan.push_back( ( int )HitboxList::RightFoot );
						Scan.push_back( ( int )HitboxList::LeftShin );
						Scan.push_back( ( int )HitboxList::RightShin );
					}

					if ( !GetAsyncKeyState( galaxy_vars.cfg.body_key ) )
					{
						Scan.push_back( ( int )HitboxList::Pelvis );
						Scan.push_back( ( int )HitboxList::Stomach );
						Scan.push_back( ( int )HitboxList::Chest );
						Scan.push_back( ( int )HitboxList::UpperChest );
					}

				}
			}
		}


		HeadHeight = galaxy_vars.cfg.HeadScale;
	}

	Vector Hitbox;
	int bestHitboxint = 0;

	for (int hitbox : Scan)
	{
		if (hitbox < 19)
			Hitbox = pEnt->GetHitboxPosition( hitbox, Matrix[pEnt->EntIndex( )] );
		else if (hitbox > 18 && hitbox < 25)
		{
			float Radius = 0;
			Hitbox = pEnt->GetHitboxPosition( HitboxForMuti[hitbox - 19], Matrix[pEnt->EntIndex( )], &Radius );
			Radius *= (galaxy_vars.cfg.BodyScale / 100.f);
			Hitbox = Vector( Hitbox.x + (Radius * mutipoint[((hitbox - 19) % 2)].x), Hitbox.y - (Radius * mutipoint[((hitbox - 19) % 2)].y), Hitbox.z );
		}
		else if (hitbox > 24 && hitbox < 28)
		{
			float Radius = 0;
			Hitbox = pEnt->GetHitboxPosition( 0, Matrix[pEnt->EntIndex( )], &Radius );
			Radius *= (HeadHeight / 100.f);
			if (hitbox != 27)
				Hitbox = Vector( Hitbox.x + (Radius * mutipoint[((hitbox - 25) % 2)].x), Hitbox.y - (Radius * mutipoint[((hitbox - 25) % 2)].y), Hitbox.z );
			else
				Hitbox += Vector( 0, 0, Radius );
		}

		float Damage = g_Autowall.Damage( Hitbox );

		if (Damage > 0.f)
			DamageArray[hitbox] = Damage;
		else
			DamageArray[hitbox] = 0;

		if (DamageArray[hitbox] > tempDmg)
		{
			tempHitbox = Hitbox;
			bestHitboxint = hitbox;
			tempDmg = DamageArray[hitbox];
		}

		g::AimbotHitbox[pEnt->EntIndex( )][hitbox] = Hitbox;
	}

	PlayerRecords pPlayerEntityRecord = g_LagComp.PlayerRecord[pEnt->EntIndex( )].at( 0 );

	Backtrack[pEnt->EntIndex( )] = false;
	ShotBacktrack[pEnt->EntIndex( )] = false;

	if (tempDmg >= galaxy_vars.cfg.Mindmg)
	{
		bestEntDmg = tempDmg;

		if ((bestHitboxint == 25 || bestHitboxint == 26 || bestHitboxint == 27) && abs( DamageArray[HITBOX_HEAD] - DamageArray[bestHitboxint] ) <= 10.f)
			return pEnt->GetHitboxPosition( HITBOX_HEAD, Matrix[pEnt->EntIndex( )] );
		else if ((bestHitboxint == 19 || bestHitboxint == 20) && DamageArray[HITBOX_PELVIS] > 30)
			return pEnt->GetHitboxPosition( HITBOX_PELVIS, Matrix[pEnt->EntIndex( )] );
		else if ((bestHitboxint == 21 || bestHitboxint == 22) && DamageArray[HITBOX_THORAX] > 30)
			return pEnt->GetHitboxPosition( HITBOX_THORAX, Matrix[pEnt->EntIndex( )] );
		else if ((bestHitboxint == 23 || bestHitboxint == 24) && DamageArray[HITBOX_UPPER_CHEST] > 30)
			return pEnt->GetHitboxPosition( HITBOX_UPPER_CHEST, Matrix[pEnt->EntIndex( )] );

		return tempHitbox;
	}

	return Vector( 0, 0, 0 );
}

void c_aimbot::OnCreateMove( ) {
	/* variables & calls */
	if (!csgo_engine->in_game( )) return;
		
	Vector Aimpoint = { 0,0,0 };
	C_BaseEntity* Target = nullptr;
	int targetID = 0;
	int tempDmg = 0;
	static bool shot = false;

	for (int i = 1; i <= csgo_engine->GetMaxClients( ); ++i)
	{
		C_BaseEntity* pPlayerEntity = csgo_entity_list->GetClientEntity( i );

		if (!pPlayerEntity || !pPlayerEntity->IsAlive( ) || pPlayerEntity->IsDormant( )) {
			g_LagComp.ClearRecords( i );
			continue;		
		}

		g_LagComp.StoreRecord( pPlayerEntity );

		if (pPlayerEntity == g::pLocalEntity || pPlayerEntity->GetTeam( ) == g::pLocalEntity->GetTeam( )) continue;
			
		/* original enemy angles */
		g::EnemyEyeAngs[i] = pPlayerEntity->GetEyeAngles( );

		if (g_LagComp.PlayerRecord[i].size( ) == 0 || !g::pLocalEntity->IsAlive( ) || !galaxy_vars.cfg.Aimbot || galaxy_vars.cfg.LegitBacktrack) continue;
		if (!g::pLocalEntity->GetActiveWeapon( ) || g::pLocalEntity->IsKnifeorNade( )) continue;
			

		bestEntDmg = 0;

		Vector Hitbox = Hitscan( pPlayerEntity );

		if (Hitbox != Vector( 0, 0, 0 ) && tempDmg <= bestEntDmg)
		{
			Aimpoint = Hitbox;
			Target = pPlayerEntity;
			targetID = Target->EntIndex( );
			tempDmg = bestEntDmg;
		}
	}

	if (!g::pLocalEntity->IsAlive( )) {
		shot = false;
		return;
	}

	if (!g::pLocalEntity->GetActiveWeapon( ) || g::pLocalEntity->IsKnifeorNade( ) || g::pLocalEntity->IsKnife_fix( )) {
		shot = false;
		return;
	}

	if (shot) {
		g::bSendPacket = true;
		antiaim.create_move( );
		shot = false;
	}

	float flServerTime = g::pLocalEntity->GetTickBase( ) * csgo_globals->interval_per_tick;
	bool canShoot = (g::pLocalEntity->GetActiveWeapon( )->GetNextPrimaryAttack( ) <= flServerTime);

	if (Target) {
		g::TargetIndex = targetID;

		float SimulationTime = 0.f;

		if (Backtrack[targetID])
			SimulationTime = g_LagComp.PlayerRecord[targetID].at( 0 ).SimTime;
		else
			SimulationTime = g_LagComp.PlayerRecord[targetID].at( g_LagComp.PlayerRecord[targetID].size( ) - 1 ).SimTime;

		if (ShotBacktrack[targetID])
			SimulationTime = g_LagComp.PlayerRecord[targetID].at( g_LagComp.ShotTick[targetID] ).SimTime;

		Vector Angle = g_Math.CalcAngle( g::pLocalEntity->GetEyePosition( ), Aimpoint );

		if (g::pLocalEntity->get_velocity( ).Length( ) >= (g::pLocalEntity->GetActiveWeapon( )->GetCSWpnData( )->max_speed_alt * .34f) - 5 && !GetAsyncKeyState( VK_SPACE ))
			Autostop( );

		if (!(g::pCmd->buttons & IN_ATTACK) && canShoot && HitChance( Target, g::pLocalEntity->GetActiveWeapon( ), Angle, Aimpoint, galaxy_vars.cfg.HitchanceValue )) {

			if (!Backtrack[targetID] && !ShotBacktrack[targetID])
				g::Shot[targetID] = true;


			g::bSendPacket = true;
			shot = true;

			g::pCmd->viewangles = Angle - (g::pLocalEntity->GetAimPunchAngle( ) * csgo_cvar->FindVar( "weapon_recoil_scale" )->GetFloat( ));
			g::pCmd->buttons |= IN_ATTACK;
			g::pCmd->tick_count = TIME_TO_TICKS( SimulationTime + g_LagComp.LerpTime( ) );
		}
	}
}