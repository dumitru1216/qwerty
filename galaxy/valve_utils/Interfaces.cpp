#include "Interfaces.h"
#include "Utils.h"

#include "..\valve_sdk\IClientMode.h"
#include "..\valve_sdk\IBaseClientDll.h"
#include "..\valve_sdk\IClientEntityList.h"
#include "..\valve_sdk\IVEngineClient.h"
#include "..\valve_sdk\CPrediction.h"
#include "..\valve_sdk\IGameEvent.h"
#include "..\valve_sdk\ISurface.h"
#include "..\valve_sdk\IEngineTrace.h"
#include "..\valve_sdk\ISurfaceData.h"
#include "..\valve_sdk\ICvar.h"
#include "..\valve_sdk\IVModelInfo.h"
#include "..\valve_sdk\CModelRender.h"
#include "..\valve_sdk\IMaterial.h"
#include "..\valve_sdk\IVRenderView.h"

#include <fstream>

#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#define enc_wstr(s) std::wstring(enc_str(s).begin(), enc_str(s).end())
#define enc_wchar(s) enc_wstr(s).c_str()

/* Initializing global interfaces */
IBaseClientDLL*     csgo_client_dll = nullptr;
IClientMode*        csgo_client_mode = nullptr;
IClientEntityList*  csgo_entity_list = nullptr;
IVEngineClient*     csgo_engine = nullptr;
CPrediction*        csgo_prediction = nullptr;
IGameMovement*      csgo_movement = nullptr;
IMoveHelper*        csgo_move_helper = nullptr;
CGlobalVarsBase*    csgo_globals = nullptr;
IGameEventManager*  csgo_event_manager = nullptr;
ISurface*           csgo_surface = nullptr;
IEngineTrace*       csgo_trace = nullptr;
IPhysicsSurfaceProps* csgo_surface_data = nullptr;
ICVar*              csgo_cvar = nullptr;
IPanel*				csgo_panel = nullptr;
IVModelInfo*		csgo_model_info = nullptr;
CModelRender*       csgo_model_render = nullptr;
IMaterialSystem*    csgo_material_system = nullptr;
IVRenderView*       csgo_render_view = nullptr;
IMemAlloc*			csgo_mem_alloc = nullptr;

namespace interfaces
{
	template< class T >
	T* FindClass(std::string szModuleName, std::string szInterfaceName, bool bSkip = false)
	{
		if (szModuleName.empty() || szInterfaceName.empty())
			return nullptr;
		typedef PVOID(*CreateInterfaceFn)(const char* pszName, int* piReturnCode);
		CreateInterfaceFn hInterface = nullptr;
		while (!hInterface)
		{
			hInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(szModuleName.c_str()), "CreateInterface");
			Sleep(5);
		}

		char pszBuffer[256];
		for (int i = 0; i < 100; i++)
		{
			sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i);
			PVOID pInterface = hInterface(pszBuffer, nullptr);

			if (pInterface && pInterface != NULL)
			{
				if (bSkip)
					sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i + 1);

				Sleep(5);
				break;
			}
		}

		return (T*)hInterface(pszBuffer, nullptr);
	}

	template<typename T>
	T* CaptureInterface(const char* szModuleName, const char* szInterfaceVersion)
	{
		HMODULE moduleHandle = GetModuleHandleA(szModuleName);
		if (moduleHandle)   /* In case of not finding module handle, throw an error. */
		{
			CreateInterfaceFn pfnFactory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(moduleHandle, "CreateInterface"));
			return reinterpret_cast<T*>(pfnFactory(szInterfaceVersion, nullptr));
		}
		Utils::Log("Error getting interface %", szInterfaceVersion);
		return nullptr;
	}

    void Init()
    {
        csgo_client_dll    = FindClass<IBaseClientDLL>("client_panorama.dll", "VClient");
        csgo_client_mode   = **reinterpret_cast<IClientMode***>    ((*reinterpret_cast<uintptr_t**>( csgo_client_dll ))[10] + 0x5u);
        csgo_globals   = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>( csgo_client_dll ))[0]  + 0x1Bu);
        csgo_entity_list   = FindClass<IClientEntityList>("client_panorama.dll", "VClientEntityList");
        csgo_engine       = FindClass<IVEngineClient>("engine.dll", "VEngineClient");
        csgo_prediction   = FindClass<CPrediction>("client_panorama.dll", "VClientPrediction");
        csgo_movement     = FindClass<IGameMovement>("client_panorama.dll", "GameMovement");
        csgo_move_helper   = **reinterpret_cast<IMoveHelper***>((Utils::FindSignature("client_panorama.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 0x2));  
        csgo_event_manager = CaptureInterface<IGameEventManager>("engine.dll", "GAMEEVENTSMANAGER002");
        csgo_surface      = FindClass<ISurface>("vguimatsurface.dll", "VGUI_Surface");
		csgo_trace        = FindClass<IEngineTrace>("engine.dll", "EngineTraceClient");
		csgo_surface_data  = FindClass<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps");
		csgo_cvar         = FindClass<ICVar>("vstdlib.dll", "VEngineCvar");
		csgo_panel		= FindClass<IPanel>("vgui2.dll", "VGUI_Panel");
		csgo_model_info    = FindClass<IVModelInfo>("engine.dll", "VModelInfoClient");
		csgo_model_render  = FindClass<CModelRender>("engine.dll", "VEngineModel");
		csgo_material_system  = FindClass<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");
		csgo_render_view   = FindClass<IVRenderView>("engine.dll", "VEngineRenderView");
		csgo_mem_alloc = *reinterpret_cast<IMemAlloc**> (GetProcAddress( GetModuleHandleA( "tier0.dll" ), "g_pMemAlloc" ));

		
		std::ofstream( "csgo/materials/glowOverlay.vmt" ) << R"#("VertexLitGeneric" {
				"$additive" "1"
				"$envmap" "models/effects/cube_white"
				"$envmaptint" "[0.2 0.2 0.7]"
				"$envmapfresnel" "1"
				"$envmapfresnelminmaxexp" "[0 1 2]"
				"$alpha" "0.8"
			})#";

		std::ofstream( "csgo\\materials\\textured_virt.vmt" ) << R"("VertexLitGeneric"
				{
					"$basetexture"	"vgui/white"
					"$model"		"1"
					"$flat"			"0"
					"$nocull"		"1"
					"$halflambert"	"1"
					"$nofog"		"1"
					"$ignorez"		"0"
					"$znearer"		"0"
					"$wireframe"	"0"
				})";

    }
}