#include "../c_hooks.h"
#include "../core/external_inclues.h"

void modulate( const float* color, IMaterial* material )
{
	if (material)
	{
		material->AlphaModulate( color[3] );
		material->ColorModulate( color[0], color[1], color[2] );
		bool found_tint;
		const auto tint = material->FindVar( "$envmaptint", &found_tint );
		if (found_tint)
			tint->set_vector( Vector( color[0], color[1], color[2] ) );
	}

}

//get hand
bool isHand( std::string modelName ) {
	if ((modelName.find( "arms" ) != std::string::npos || modelName.find( "v_models" ) != std::string::npos) && !(modelName.find( "uid" ) != std::string::npos || modelName.find( "stattrack" ) != std::string::npos)) {
		return true;
	}

	return false;
}

static IMaterial* glowOverlay = nullptr;
static IMaterial* materialRegular = nullptr;
static IMaterial* rifk_glow = nullptr;
void __fastcall c_hooks::DrawModelExecute( void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix )
{
	static auto oDrawModelExecute = hooks.pModelHook->GetOriginal<DrawModelExecute_t>( vtable_indexes::dme );
	const char* ModelName = csgo_model_info->GetModelName( (model_t*)info.pModel );
	C_BaseEntity* pPlayerEntity = csgo_entity_list->GetClientEntity( info.index );

	//enemy struct
	static IMaterial* normal_enemy;
	static IMaterial* overide_enemy;

	//local struct
	static IMaterial* normal_local;
	static IMaterial* overide_local;

	//desync
	static IMaterial* desync_material;
	static IMaterial* desync_overide;

	//hands
	static IMaterial* hand_normal;
	static IMaterial* hand_overide;


	glowOverlay = csgo_material_system->FindMaterial( "glowOverlay.vmt", TEXTURE_GROUP_OTHER );
	materialRegular = csgo_material_system->FindMaterial( "textured_virt", TEXTURE_GROUP_MODEL );
	rifk_glow = csgo_material_system->FindMaterial( "dev/glow_armsrace", TEXTURE_GROUP_OTHER );

	//enemy normal
	switch (galaxy_vars.cfg.normal_enemy_material)
	{
		case 0:
			normal_enemy = materialRegular;
			break;
		//---------------------------------
		case 1:
			normal_enemy = glowOverlay;
			break;
		//---------------------------------
		case 2:
			normal_enemy = rifk_glow;
			break;
		//---------------------------------
	}
	//enemy overide
	switch (galaxy_vars.cfg.overide_enemy_material)
	{
	case 0:
		overide_enemy = materialRegular;
		break;
		//---------------------------------
	case 1:
		overide_enemy = glowOverlay;
		break;
		//---------------------------------
	case 2:
		overide_enemy = rifk_glow;
		break;
		//---------------------------------
	}
	//local normal
	switch (galaxy_vars.cfg.local_materials)
	{
	case 0:
		normal_local = materialRegular;
		break;
		//---------------------------------
	case 1:
		normal_local = glowOverlay;
		break;
		//---------------------------------
	case 2:
		normal_local = rifk_glow;
		break;
		//---------------------------------
	}
	//local overide
	switch (galaxy_vars.cfg.local_overide_materials)
	{
	case 0:
		overide_local = materialRegular;
		break;
		//---------------------------------
	case 1:
		overide_local = glowOverlay;
		break;
		//---------------------------------
	case 2:
		overide_local = rifk_glow;
		break;
		//---------------------------------
	}
	//desync normal
	switch (galaxy_vars.cfg.desync_chams_material)
	{
	case 0:
		desync_material = materialRegular;
		break;
		//---------------------------------
	case 1:
		desync_material = glowOverlay;
		break;
		//---------------------------------
	case 2:
		desync_material = rifk_glow;
		break;
		//---------------------------------
	}
	//desync overide
	switch (galaxy_vars.cfg.desync_chams_overide_material)
	{
	case 0:
		desync_overide = materialRegular;
		break;
		//---------------------------------
	case 1:
		desync_overide = glowOverlay;
		break;
		//---------------------------------
	case 2:
		desync_overide = rifk_glow;
		break;
		//---------------------------------
	}
	//hand normal
	switch (galaxy_vars.cfg.hand_materials)
	{
	case 0:
		hand_normal = materialRegular;
		break;
		//---------------------------------
	case 1:
		hand_normal = glowOverlay;
		break;
		//---------------------------------
	case 2:
		hand_normal = rifk_glow;
		break;
		//---------------------------------
	}
	//hand overide
	switch (galaxy_vars.cfg.hand_materials_overide)
	{
	case 0:
		hand_overide = materialRegular;
		break;
		//---------------------------------
	case 1:
		hand_overide = glowOverlay;
		break;
		//---------------------------------
	case 2:
		hand_overide = rifk_glow;
		break;
		//---------------------------------
	}

	//entity modelname
	if (pPlayerEntity && strstr( ModelName, "models/player" ))
	{
		if (info.index == csgo_engine->GetLocalPlayer( ) && strstr( ModelName, "models/player" ))
		{
			if (g::m_got_fake_matrix)
			{
				if (galaxy_vars.cfg.desync_chams) { // enabled
					for (auto& i : g::m_fake_matrix)
					{
						i[0][3] += info.origin.x;
						i[1][3] += info.origin.y;
						i[2][3] += info.origin.z;
					}
					{

						desync_material->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
						modulate( galaxy_vars.cfg.desync_chams_color, desync_material );
						csgo_model_render->ForcedMaterialOverride( desync_material );
						oDrawModelExecute( csgo_model_render, context, state, info, g::m_fake_matrix ); // 0

						if (galaxy_vars.cfg.deysnc_Overid)
						{
							desync_overide->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
							modulate( galaxy_vars.cfg.desync_chams_overide, desync_overide );
							csgo_model_render->ForcedMaterialOverride( desync_overide );
							oDrawModelExecute( csgo_model_render, context, state, info, g::m_fake_matrix ); // 0
						}

					}
					for (auto& i : g::m_fake_matrix)
					{
						i[0][3] -= info.origin.x;
						i[1][3] -= info.origin.y;
						i[2][3] -= info.origin.z;
					}
					csgo_model_render->ForcedMaterialOverride( nullptr );
				}
			}

			if (galaxy_vars.cfg.local_chams)
			{
				normal_local->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
				modulate( galaxy_vars.cfg.local_color, normal_local );
				csgo_model_render->ForcedMaterialOverride( normal_local );
				oDrawModelExecute( ecx, context, state, info, matrix );

				if (galaxy_vars.cfg.local_chams_overide)
				{
					overide_local->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
					modulate( galaxy_vars.cfg.local_overide_color, overide_local );
					csgo_model_render->ForcedMaterialOverride( overide_local );
					oDrawModelExecute( ecx, context, state, info, matrix );
				}
			}
		}

		if (pPlayerEntity != nullptr && pPlayerEntity != g::pLocalEntity && pPlayerEntity->GetTeam( ) != g::pLocalEntity->GetTeam( ) && strstr( ModelName, "models/player" ))
		{
			if (galaxy_vars.cfg.chams_enemy)
			{
				normal_enemy->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
				modulate( galaxy_vars.cfg.enemy_visible_colors, normal_enemy );
				csgo_model_render->ForcedMaterialOverride( normal_enemy );
				oDrawModelExecute( ecx, context, state, info, matrix );	

				if (galaxy_vars.cfg.enemy_invisible)
				{
					normal_enemy->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
					normal_enemy->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
					modulate( galaxy_vars.cfg.enemy_visible_colors, normal_enemy );
					csgo_model_render->ForcedMaterialOverride( normal_enemy );
					oDrawModelExecute( ecx, context, state, info, matrix );
				}

				if (galaxy_vars.cfg.enemy_overide_chams)
				{
					overide_enemy->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
					overide_enemy->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
					modulate( galaxy_vars.cfg.enemy_overide_colors, overide_enemy );
					csgo_model_render->ForcedMaterialOverride( overide_enemy );
					oDrawModelExecute( ecx, context, state, info, matrix );

				}
			}		
		
		}


	}

	if (isHand(ModelName))
	{
		if (galaxy_vars.cfg.hand_chams)
		{
			hand_normal->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );			
			modulate( galaxy_vars.cfg.hand_color, hand_normal );
			csgo_model_render->ForcedMaterialOverride( hand_normal );
			oDrawModelExecute( ecx, context, state, info, matrix );

			if (galaxy_vars.cfg.hand_chams_overide)
			{
				hand_overide->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );
				modulate( galaxy_vars.cfg.hand_overide_color, hand_overide );
				csgo_model_render->ForcedMaterialOverride( hand_overide );
				oDrawModelExecute( ecx, context, state, info, matrix );
			}
		}
	}




	
	oDrawModelExecute( ecx, context, state, info, matrix );
	csgo_model_render->ForcedMaterialOverride( nullptr );
}