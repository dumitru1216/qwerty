#include "../c_hooks.h"
#include "../core/external_inclues.h"

void __fastcall c_hooks::DoExtraBonesProcessing( void * ECX, void * EDX, void * unkn1, void * unkn2, void * unkn3, void * unkn4, CBoneBitList & unkn5, void * unkn6 )
{
	C_BaseEntity* pPlayerEntity = (C_BaseEntity*)ECX;

	if (!pPlayerEntity || pPlayerEntity == nullptr) return;
	if (!pPlayerEntity->IsAlive( ) || pPlayerEntity->IsDormant( )) return;
	if (!pPlayerEntity->AnimState( )) return;

	auto oDoExtraBonesProcessing = hooks.pPlayerHook[pPlayerEntity->EntIndex( )]->GetOriginal<ExtraBoneProcess_t>( vtable_indexes::extraBonePro );

	if (!pPlayerEntity) {
		oDoExtraBonesProcessing( ECX, unkn1, unkn2, unkn3, unkn4, unkn5, unkn6 );
		return;
	}

	auto animstate = pPlayerEntity->GetPlayerAnimState( );
	if (!animstate || !animstate->pBaseEntity) {
		oDoExtraBonesProcessing( ECX, unkn1, unkn2, unkn3, unkn4, unkn5, unkn6 );
		return;
	}

	bool enablefixfit = true;
	if (!enablefixfit) {
		oDoExtraBonesProcessing( ECX, unkn1, unkn2, unkn3, unkn4, unkn5, unkn6 );
		return;
	}

	auto o_on_ground = animstate->m_bOnGround;
	animstate->m_bOnGround = false;
	oDoExtraBonesProcessing( ECX, unkn1, unkn2, unkn3, unkn4, unkn5, unkn6 );
	animstate->m_bOnGround = o_on_ground;

}

void c_hooks::HookPlayers( )
{
	static bool Init[65];
	static bool Hooked[65];

	for (int i = 1; i < csgo_engine->GetMaxClients( ); ++i)
	{
		C_BaseEntity* pPlayerEntity = csgo_entity_list->GetClientEntity( i );

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive( )
			|| pPlayerEntity->IsDormant( ))
		{
			if (Hooked[i])
				hooks.pPlayerHook[i]->Unhook( vtable_indexes::extraBonePro );

			Hooked[i] = false;
			continue;
		}

		if (!Init[i])
		{
			hooks.pPlayerHook[i] = std::make_unique<ShadowVTManager>( );
			Init[i] = true;
		}

		if (Hooked[i])
			hooks.pPlayerHook[i]->Unhook( vtable_indexes::extraBonePro );

		if (!Hooked[i])
		{
			hooks.pPlayerHook[i]->Setup( pPlayerEntity );
			hooks.pPlayerHook[i]->Hook( vtable_indexes::extraBonePro, c_hooks::DoExtraBonesProcessing );

			Hooked[i] = true;
		}
	}
}
