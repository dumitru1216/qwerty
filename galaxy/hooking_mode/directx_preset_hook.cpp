#include "../c_hooks.h"
#include "../core/external_inclues.h"
#include "directx_font.h"

IDirect3DStateBlock9* m_pStateBlockDraw;
IDirect3DStateBlock9* m_pStateBlockText;
IDirect3DStateBlock9 *state_block;
DWORD dwOld_D3DRS_COLORWRITEENABLE;
static bool initialized = false;

void __stdcall create_objects( IDirect3DDevice9* device )  //creating imgui objects
{
	if (hooks.hCSGOWindow)
		ImGui_ImplDX9_CreateDeviceObjects( );
}

void __stdcall invalidate_objects( ) //invalidate imgui objects
{
	ImGui_ImplDX9_InvalidateDeviceObjects( );
}

void __stdcall setup_resent( IDirect3DDevice9* device ) //setup imgui colors / fonts / style
{
	ImGui_ImplDX9_Init( hooks.hCSGOWindow, device );


	ImGuiStyle& style = ImGui::GetStyle( );
	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2( 0, 0 );
	style.WindowMinSize = ImVec2( 32, 32 );
	style.WindowRounding = 0.0f;
	style.WindowTitleAlign = ImVec2( 0.0f, 0.5f );
	style.ChildWindowRounding = 0.0f;
	style.FramePadding = ImVec2( 4, 3 );
	style.FrameRounding = 0.0f;
	style.ItemSpacing = ImVec2( 8, 8 );
	style.ItemInnerSpacing = ImVec2( 8, 8 );
	style.TouchExtraPadding = ImVec2( 0, 0 );
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 0.0f;
	style.ScrollbarSize = 6.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 0.0f;
	style.ButtonTextAlign = ImVec2( 0.0f, 0.5f );
	style.DisplayWindowPadding = ImVec2( 22, 22 );
	style.DisplaySafeAreaPadding = ImVec2( 4, 4 );
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = false;
	style.CurveTessellationTol = 1.f;

	ImVec4* colors = ImGui::GetStyle( ).Colors;
	colors[ImGuiCol_Text] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
	colors[ImGuiCol_TextDisabled] = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
	colors[ImGuiCol_WindowBg] = ImVec4( 25 / 255.f, 25 / 255.f, 25 / 255.f, 1.0f );
	colors[ImGuiCol_PopupBg] = ImVec4( 0.08f, 0.08f, 0.08f, 0.94f );
	colors[ImGuiCol_Border] = ImVec4( 0, 0, 0, 1 );
	colors[ImGuiCol_BorderShadow] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
	colors[ImGuiCol_ChildWindowBg] = ImVec4( 35 / 255.f, 35 / 255.f, 35 / 255.f, 1 );
	colors[ImGuiCol_FrameBg] = ImVec4( 0.22f, 0.22f, 0.22f, 1.0f );
	colors[ImGuiCol_FrameBgHovered] = ImVec4( 0.15f, 0.15f, 0.15f, 1.0f );
	colors[ImGuiCol_FrameBgActive] = ImVec4( 0.08f, 0.08f, 0.08f, 1.00f );
	colors[ImGuiCol_TitleBgActive] = ImVec4( 35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f );
	colors[ImGuiCol_TitleBg] = ImVec4( 35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f );
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4( 35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f );
	colors[ImGuiCol_MenuBarBg] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );
	colors[ImGuiCol_ScrollbarBg] = ImVec4( 0.17f, 0.17f, 0.17f, 1.00f );
	colors[ImGuiCol_ScrollbarGrab] = ImVec4( 0.25f, 0.25f, 0.25f, 1.00f );
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 0.25f, 0.25f, 0.25f, 1.00f );
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( 0.25f, 0.25f, 0.25f, 1.00f );
	colors[ImGuiCol_Button] = ImVec4( 135 / 255.f, 135 / 255.f, 135 / 255.f, 1.0f );
	colors[ImGuiCol_ButtonHovered] = ImVec4( 151 / 255.f, 151 / 255.f, 151 / 255.f, 1.0f );
	colors[ImGuiCol_ButtonActive] = ImVec4( 135 / 255.f, 135 / 255.f, 135 / 255.f, 1.0f );
	colors[ImGuiCol_Header] = ImVec4( 35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f );
	colors[ImGuiCol_HeaderHovered] = ImVec4( 35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f );
	colors[ImGuiCol_HeaderActive] = ImVec4( 35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f );
	colors[ImGuiCol_Separator] = ImVec4( 0, 0, 0, 1 );
	colors[ImGuiCol_SeparatorHovered] = ImVec4( 0, 0, 0, 1 );
	colors[ImGuiCol_SeparatorActive] = ImVec4( 0, 0, 0, 1 );
	colors[ImGuiCol_ResizeGrip] = ImVec4( 0.26f, 0.59f, 0.98f, 0.25f );
	colors[ImGuiCol_ResizeGripHovered] = ImVec4( 0.26f, 0.59f, 0.98f, 0.67f );
	colors[ImGuiCol_ResizeGripActive] = ImVec4( 0.26f, 0.59f, 0.98f, 0.95f );
	colors[ImGuiCol_PlotLines] = ImVec4( 0.61f, 0.61f, 0.61f, 1.00f );
	colors[ImGuiCol_PlotLinesHovered] = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
	colors[ImGuiCol_PlotHistogram] = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
	colors[ImGuiCol_TextSelectedBg] = ImVec4( 0.26f, 0.59f, 0.98f, 0.35f );
	colors[ImGuiCol_CloseButton] = ImVec4( 0, 0, 0, 0 );
	colors[ImGuiCol_CloseButtonHovered] = ImVec4( 0, 0, 0, 0 );

	create_objects( device );
}

void __stdcall end_present( IDirect3DDevice9* device ) {
	if (menu.menuOpened) {
		if (ImGui::GetStyle( ).Alpha > 1.f)
			ImGui::GetStyle( ).Alpha = 1.f;
		else if (ImGui::GetStyle( ).Alpha != 1.f)
			ImGui::GetStyle( ).Alpha += 0.05f;
	}
	else {
		if (ImGui::GetStyle( ).Alpha < 0.f)
			ImGui::GetStyle( ).Alpha = 0.f;
		else if (ImGui::GetStyle( ).Alpha != 0.f)
			ImGui::GetStyle( ).Alpha -= 0.01f;
	}


	ImGui::Render( );

	state_block->Apply( );
	state_block->Release( );
}

void __stdcall pre_render( IDirect3DDevice9* device ) {
	D3DVIEWPORT9 d3d_viewport;
	device->GetViewport( &d3d_viewport );

	device->CreateStateBlock( D3DSBT_ALL, &state_block );
	state_block->Capture( );

	device->SetVertexShader( nullptr );
	device->SetPixelShader( nullptr );
	device->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

	device->SetRenderState( D3DRS_LIGHTING, FALSE );
	device->SetRenderState( D3DRS_FOGENABLE, FALSE );
	device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	device->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	device->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
	device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	device->SetRenderState( D3DRS_STENCILENABLE, FALSE );

	device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
	device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, TRUE );

	device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	device->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	device->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
	device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	device->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
	device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	device->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );

	device->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
	device->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
}

void __stdcall post_render( ) {
	ImGui_ImplDX9_NewFrame( );
}

void __stdcall c_hooks::Hooked_EndScene( IDirect3DDevice9* pDevice )
{
	static auto oEndScene = hooks.D3DHook->GetOriginal<EndSceneFn>( vtable_indexes::end_scene );

	if (!initialized) {
		menu.apply_fonts( );
		setup_resent( pDevice );
		initialized = true;
	}
	if (initialized) {
		pre_render( pDevice );
		post_render( );

		menu.run_popup( );
		menu.Render( );
		end_present( pDevice );
	}



	oEndScene( pDevice );
}

void __stdcall c_hooks::Hooked_EndScene_Reset( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters )
{
	static auto oEndSceneReset = hooks.D3DHook->GetOriginal<EndSceneResetFn>( vtable_indexes::end_scene_reset );

	if (!initialized)
		oEndSceneReset( pDevice, pPresentationParameters );

	invalidate_objects( );
	auto hr = oEndSceneReset;
	create_objects( pDevice );


	return oEndSceneReset( pDevice, pPresentationParameters );
}