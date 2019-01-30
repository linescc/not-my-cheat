#pragma once

#include "valve_sdk/csgostructs.hpp"
#include "helpers/vfunc_hook.hpp"
#include <d3d9.h>
#include <sys/types.h>
namespace index
{
    constexpr auto EndScene                 = 42;
    constexpr auto Reset                    = 16;
    constexpr auto PaintTraverse            = 41;
    constexpr auto CreateMove               = 21;
    constexpr auto FrameStageNotify         = 37;
    constexpr auto DrawModelExecute         = 21;
	constexpr auto SvCheatsGetBool          = 13;
	constexpr auto OverrideView             = 18;
}

namespace Hooks
{
	void Initialize ( );
	void Shutdown ( );

	extern vfunc_hook hlclient_hook;
	extern vfunc_hook direct3d_hook;
	extern vfunc_hook vguipanel_hook;
	extern vfunc_hook vguisurf_hook;
	extern vfunc_hook mdlrender_hook;
	extern vfunc_hook viewrender_hook;

	using EndScene = long ( __stdcall * )( IDirect3DDevice9* );
	using Reset = long ( __stdcall * )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
	using CreateMove = void ( __thiscall* )( IBaseClientDLL*, int, float, bool );
	using PaintTraverse = void ( __thiscall* )( IPanel*, vgui::VPANEL, bool, bool );
	using FrameStageNotify = void ( __thiscall* )( IBaseClientDLL*, ClientFrameStage_t );
	using DrawModelExecute = void ( __thiscall* )( IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t* );
	using OverrideView = void ( __thiscall* )( IClientMode*, CViewSetup* );
	using SceneEndFn = void ( __thiscall* )( void* );
	using DrawWorldList = void ( __thiscall* )( IVRenderView*, uintptr_t, void*, void*, unsigned long, float );


	long __stdcall hkEndScene ( IDirect3DDevice9* device );
	long __stdcall hkReset ( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters );
	void __stdcall hkCreateMove ( int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket );
	void __stdcall hkCreateMove_Proxy ( int sequence_number, float input_sample_frametime, bool active );
	void __stdcall hkPaintTraverse ( vgui::VPANEL panel, bool forceRepaint, bool allowForce );
	void __stdcall hkDrawModelExecute ( IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld );
	void __stdcall hkFrameStageNotify ( ClientFrameStage_t stage );
	void __stdcall hkOverrideView ( CViewSetup * vsView );
	bool __fastcall hkSvCheatsGetBool ( PVOID pConVar, void* edx );
	void __stdcall hkLockCursor ( );
	float __fastcall hkGetViewModelFOV ( void* ecx, void* edx );
	void __fastcall hkSceneEnd ( void* thisptr, void* edx );
	void __fastcall hkDrawWorldLists ( IVRenderView* ecx, uintptr_t edx, void* mat_render_context, void* render_list, unsigned long flags, float water_adjust );
}
