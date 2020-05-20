#include "../c_hooks.h"
#include "../core/external_inclues.h"
#include "../core/Animations System/animation_system.h"

std::vector<const char*> vistasmoke_mats = //b1g nosmoke
{
"particle/vistasmokev1/vistasmokev1_smokegrenade",
"particle/vistasmokev1/vistasmokev1_emods",
"particle/vistasmokev1/vistasmokev1_emods_impactdust",
};
auto smoke_count = *(DWORD*)(Utils::FindSignature( "client_panorama", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0" ) + 0x8);

C_BaseEntity* playerIndex( int index ) {
	typedef C_BaseEntity* (__fastcall * PlayerByIndex)(int);
	static PlayerByIndex UTIL_PlayerByIndex = reinterpret_cast<PlayerByIndex>(Utils::PatternScan( GetModuleHandleW( L"server.dll" ), "85 C9 7E 2A A1" ));

	if (!UTIL_PlayerByIndex)
		return false;

	return UTIL_PlayerByIndex( index );
}

void no_smoke( )
{
	if ( galaxy_vars.cfg.no_smoke )
	{
		for (auto matName : vistasmoke_mats)
		{
			*(int*)smoke_count = 0;
			IMaterial* mat = csgo_material_system->FindMaterial( matName, "Other textures" );
			mat->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, true );
		}
	}
}

void server_hitbox( )
{
	if (g::pLocalEntity && g::pLocalEntity->IsAlive( ))
	{
		static auto util_playerbyindex = Utils::FindSignature( "server.dll", "85 C9 7E 2A A1" );
		static auto draw_server_hitboxes = Utils::FindSignature( "server.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE" );

		auto t = -1.0f;

		auto idx = csgo_engine->GetLocalPlayer( );

		__asm {
			mov ecx, idx
			call util_playerbyindex
			cmp eax, 0
			je gay
			pushad
			movss xmm1, t
			push 0
			mov ecx, eax
			call draw_server_hitboxes
			popad
		}

	gay:
		{

		}
	}
}

void __stdcall c_hooks::FrameStageNotify( ClientFrameStage_t curStage )
{
	static auto oFrameStage = hooks.pClientHook->GetOriginal<FrameStageNotify_t>( vtable_indexes::frameStage );

	if (csgo_engine->in_game( ) && csgo_engine->IsConnected( ) && g::pLocalEntity)
	{
		//hooking features
		g_Misc.ThirdPerson( curStage );
		g_Resolver.FrameStage( curStage );
		no_smoke( );
		//proper hooking nightmode
		c_other_esp.night_mode( );

		if (curStage == FRAME_RENDER_START)
		{
			if ( galaxy_vars.cfg.server_hitobx )
			{
				server_hitbox( );
			}

		
			c_animfix.LocalAnimFix( g::pLocalEntity );
		}
	}

	oFrameStage( curStage );
}
