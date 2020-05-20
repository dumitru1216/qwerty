#include "../c_hooks.h"
#include "../core/external_inclues.h"
void __fastcall c_hooks::SceneEnd( void *ecx, void *edx )
{
	static auto oSceneEnd = hooks.pRenderViewHook->GetOriginal<SceneEnd_t>( vtable_indexes::sceneEnd );
	oSceneEnd( ecx, edx );


}
