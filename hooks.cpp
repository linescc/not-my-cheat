#include "hooks.hpp"
#include <intrin.h>  

#include "menu.hpp"
#include "options.hpp"
#include "helpers/input.hpp"
#include "helpers/utils.hpp"
#include "features/misc.hpp"
#include "features/chams.hpp"
#include "features/visuals.hpp"
#include "features/Skinchanger.h"
#include "helpers/math.hpp"
#include "SpoofedConvar.h"

#pragma intrinsic(_ReturnAddress)  
vgui::HFont font;




namespace Hooks
{
	vfunc_hook hlclient_hook;
	vfunc_hook direct3d_hook;
	vfunc_hook vguipanel_hook;
	vfunc_hook vguisurf_hook;
	vfunc_hook mdlrender_hook;
	vfunc_hook clientmode_hook;
	vfunc_hook sv_cheats;
	vfunc_hook render_view;

	void Initialize( )
	{
		hlclient_hook.setup( g_CHLClient, "client_panorama.dll" );
		direct3d_hook.setup( g_D3DDevice9, "shaderapidx9.dll" );
		vguipanel_hook.setup( g_VGuiPanel );
		vguisurf_hook.setup( g_VGuiSurface );
		mdlrender_hook.setup( g_MdlRender, "engine.dll" );
		clientmode_hook.setup( g_ClientMode, "client_panorama.dll" );
		ConVar* sv_cheats_con = g_CVar->FindVar( "sv_cheats" );
		sv_cheats.setup( sv_cheats_con );
		render_view.setup( g_RenderView );


		hlclient_hook.hook_index( 37, hkFrameStageNotify );
	
		hlclient_hook.hook_index( 22, hkCreateMove_Proxy );
		vguipanel_hook.hook_index( 41, hkPaintTraverse );
		mdlrender_hook.hook_index( index::DrawModelExecute, hkDrawModelExecute );
		clientmode_hook.hook_index( index::OverrideView, hkOverrideView );
		clientmode_hook.hook_index( 35, hkGetViewModelFOV );
		render_view.hook_index(9, hkSceneEnd);
	

		vguisurf_hook.hook_index( 67, hkLockCursor );


		sv_cheats.hook_index( index::SvCheatsGetBool, hkSvCheatsGetBool );

		Visuals::CreateFonts( );

		direct3d_hook.hook_index( index::EndScene, hkEndScene );
		direct3d_hook.hook_index( index::Reset, hkReset );

		g_EngineClient->vClientCmd_Unrestricted("con_filter_enable 1;con_filter_text_out failed;");

	}
	//-------------------------------------------------------------------------------

	void Shutdown( )
	{
		hlclient_hook.unhook_all( );
		direct3d_hook.unhook_all( );
		vguipanel_hook.unhook_all( );
		vguisurf_hook.unhook_all( );
		mdlrender_hook.unhook_all( );
		clientmode_hook.unhook_all( );
		render_view.unhook_all( );
	}



	typedef void( __thiscall* LockCursor )( void* );
	LockCursor oLockCursor;

	void __stdcall hkLockCursor( )
	{
		oLockCursor = Hooks::vguisurf_hook.get_original<LockCursor>( 67 );

		if ( Menu::Get( ).IsVisible( ) )
		{
			g_VGuiSurface->UnlockCursor( );
			return;
		}

		oLockCursor( g_VGuiSurface );
	}

	void __fastcall hkSceneEnd( void* thisptr , void* edx )
	{
		auto ofunc = render_view.get_original<SceneEndFn>( 9 );

		bool update = false;

		if ( !g_LocalPlayer || !g_EngineClient->IsConnected( ) || !g_EngineClient->IsInGame( ) )
		{
			update = true;
			return ofunc( thisptr );
		}



		if ( g_Options.update_world)
		{

			static auto fog_enableskybox = g_CVar->FindVar("fog_enableskybox");
			static auto fog_override = g_CVar->FindVar("fog_override");
			static auto fog_enable = g_CVar->FindVar("fog_enable");
			static auto draw_specific_static_prop = g_CVar->FindVar("r_DrawSpecificStaticProp");


			// set values
			fog_enable->SetValue(0.0f);
			fog_enableskybox->SetValue(0.0f);
			fog_override->SetValue(1.0f);
			draw_specific_static_prop->SetValue(1.0f);

			for ( MaterialHandle_t i = g_MatSystem->FirstMaterial( ); i != g_MatSystem->InvalidMaterial( ); i = g_MatSystem->NextMaterial( i ) )
			{
				IMaterial *pMaterial = g_MatSystem->GetMaterial( i );
				if ( !pMaterial || pMaterial->IsErrorMaterial( ) )
					continue;

				if ( g_Options.misc_col_sky )
				{
					if ( strstr( pMaterial->GetTextureGroupName( ) , TEXTURE_GROUP_SKYBOX ) )
					{
						pMaterial->ColorModulate( g_Options.color_sky.r( ) / 255.f ,
												  g_Options.color_sky.g( ) / 255.f ,
												  g_Options.color_sky.b( ) / 255.f );
					}

				}
				else
				{
					if ( !g_Options.misc_nightmode )
					{
						if ( strstr( pMaterial->GetTextureGroupName( ) , TEXTURE_GROUP_SKYBOX ) )
						{
							pMaterial->ColorModulate( 1 , 1 , 1 );
						}
					}
				}
				if ( g_Options.misc_nightmode )
				{
					if ( !g_Options.misc_col_sky )
					{
						if ( strstr( pMaterial->GetTextureGroupName( ) , TEXTURE_GROUP_SKYBOX ) )
						{
							pMaterial->ColorModulate( g_Options.color_nightmode.r( ) / 255.f ,
													  g_Options.color_nightmode.g( ) / 255.f ,
													  g_Options.color_nightmode.b( ) / 255.f );
						}
					}

					if ( strstr( pMaterial->GetTextureGroupName( ) , "World textures" ) )
					{
						pMaterial->ColorModulate( g_Options.color_nightmode.r( ) / 255.f ,
												  g_Options.color_nightmode.g( ) / 255.f ,
												  g_Options.color_nightmode.b( ) / 255.f );
					}
					if ( strstr( pMaterial->GetTextureGroupName( ) , "StaticProp" ) )
					{
						pMaterial->ColorModulate( g_Options.color_nightmode.r( ) / 255.f ,
												  g_Options.color_nightmode.g( ) / 255.f ,
												  g_Options.color_nightmode.b( ) / 255.f );
					}
					if ( strstr( pMaterial->GetTextureGroupName( ) , "models/props/de_dust/palace_bigdome" ) )
					{
						pMaterial->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW , true );
					}
					if ( strstr( pMaterial->GetTextureGroupName( ) , "models/props/de_dust/palace_pillars" ) )
					{
						pMaterial->ColorModulate( g_Options.color_nightmode.r( ) / 255.f ,
												  g_Options.color_nightmode.g( ) / 255.f ,
												  g_Options.color_nightmode.b( ) / 255.f );
					}

					if ( strstr( pMaterial->GetTextureGroupName( ) , "Particle textures" ) )
					{
						pMaterial->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW , true );
					}

				}
				else
				{
					if ( !g_Options.misc_col_sky )
					{
						if ( strstr( pMaterial->GetTextureGroupName( ) , TEXTURE_GROUP_SKYBOX ) )
						{
							pMaterial->ColorModulate( 1 , 1 , 1 );
						}
					}

					if ( strstr( pMaterial->GetTextureGroupName( ) , "World textures" ) )
					{
						pMaterial->ColorModulate( 1 , 1 , 1 );
					}
					if ( strstr( pMaterial->GetTextureGroupName( ) , "StaticProp" ) )
					{
						pMaterial->ColorModulate( 1 , 1 , 1 );
					}
					if ( strstr( pMaterial->GetTextureGroupName( ) , "models/props/de_dust/palace_bigdome" ) )
					{
						pMaterial->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW , false );
					}
					if ( strstr( pMaterial->GetTextureGroupName( ) , "models/props/de_dust/palace_pillars" ) )
					{
						pMaterial->ColorModulate( 1 , 1 , 1 );
					}

					if ( strstr( pMaterial->GetTextureGroupName( ) , "Particle textures" ) )
					{
						pMaterial->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW , false );
					}
				}

			}
			update = false;
			g_Options.update_world = false;
		}

		ofunc( thisptr );
	}

	void __fastcall hkDrawWorldLists( IVRenderView * ecx , uintptr_t edx , void * mat_render_context , void * render_list , unsigned long flags , float water_adjust )
	{
		auto ofunc = render_view.get_original<DrawWorldList>( 12 );

		ofunc( g_RenderView , edx , mat_render_context , render_list , flags , water_adjust );
	}

	long __stdcall hkEndScene( IDirect3DDevice9* device )
	{
		auto oEndScene = direct3d_hook.get_original<EndScene>( index::EndScene );

		static uintptr_t gameoverlay_return_address = 0;

		if ( !gameoverlay_return_address ) {
			MEMORY_BASIC_INFORMATION info;
			VirtualQuery( _ReturnAddress( ), &info, sizeof( MEMORY_BASIC_INFORMATION ) );

			char mod[MAX_PATH];
			GetModuleFileNameA( ( HMODULE ) info.AllocationBase, mod, MAX_PATH );

			if ( strstr( mod, "gameoverlay" ) )
				gameoverlay_return_address = ( uintptr_t ) ( _ReturnAddress( ) );
		}



		DWORD colorwrite , srgbwrite;
		device->GetRenderState( D3DRS_COLORWRITEENABLE , &colorwrite );
		device->GetRenderState( D3DRS_SRGBWRITEENABLE , &srgbwrite );

		//fix drawing without calling engine functons/cl_showpos
		device->SetRenderState( D3DRS_COLORWRITEENABLE , 0xffffffff );
		//removes the source engine color correction
		device->SetRenderState( D3DRS_SRGBWRITEENABLE , false );

		IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
		device->GetVertexDeclaration( &vertDec );
		device->GetVertexShader( &vertShader );

		Menu::Get( ).Render( );

		device->SetRenderState( D3DRS_COLORWRITEENABLE , colorwrite );
		device->SetRenderState( D3DRS_SRGBWRITEENABLE , srgbwrite );
		device->SetVertexDeclaration( vertDec );
		device->SetVertexShader( vertShader );



		return oEndScene( device );
	}
	//--------------------------------------------------------------------------------
	long __stdcall hkReset( IDirect3DDevice9* device , D3DPRESENT_PARAMETERS* pPresentationParameters )
	{
		auto oReset = direct3d_hook.get_original<Reset>( index::Reset );

		Menu::Get( ).OnDeviceLost( );

		auto hr = oReset( device , pPresentationParameters );

		if ( hr >= 0 )
		{
			Menu::Get( ).OnDeviceReset( );
			Visuals::CreateFonts( );
		}

		return hr;
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkCreateMove( int sequence_number , float input_sample_frametime , bool active , bool& bSendPacket )
	{
		auto oCreateMove = hlclient_hook.get_original<CreateMove>( 22 );

		oCreateMove( g_CHLClient , sequence_number , input_sample_frametime , active );

		auto cmd = g_Input->GetUserCmd( sequence_number );
		auto verified = g_Input->GetVerifiedCmd( sequence_number );

		if ( !cmd || !cmd->command_number )
			return;


		if ( g_LocalPlayer && !g_Options.PlayeName )
			g_Options.PlayeName = g_LocalPlayer->GetPlayerInfo( ).szName;

		if ( g_LocalPlayer )
		{
			if ( InputSys::Get( ).IsKeyDown( VK_TAB ) && g_Options.misc_rankreveal )
				Utils::RankRevealAll( );

			C_Misc::Get( ).OnCreateMove( cmd );

		}


		verified->m_cmd = *cmd;
		verified->m_crc = cmd->GetChecksum( );


	}
	//--------------------------------------------------------------------------------
	__declspec( naked ) void __stdcall hkCreateMove_Proxy( int sequence_number , float input_sample_frametime , bool active )
	{
		__asm
		{
			push ebp
			mov  ebp , esp
			push ebx
			lea  ecx , [esp]
			push ecx
			push dword ptr[active]
			push dword ptr[input_sample_frametime]
			push dword ptr[sequence_number]
			call Hooks::hkCreateMove
			pop  ebx
			pop  ebp
			retn 0Ch
		}
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkPaintTraverse( vgui::VPANEL panel , bool forceRepaint , bool allowForce )
	{
		static auto panelId = vgui::VPANEL { 0 };
		static auto oPaintTraverse = vguipanel_hook.get_original<PaintTraverse>( 41 );

		oPaintTraverse( g_VGuiPanel , panel , forceRepaint , allowForce );

		if ( !panelId )
		{
			const auto panelName = g_VGuiPanel->GetName( panel );
			if ( !strcmp( panelName , "FocusOverlayPanel" ) )
			{
				panelId = panel;
			}
		}
		else if ( panelId == panel )
		{

			if ( !g_EngineClient->IsInGame( ) )
				g_Options.CurrentPlayer = 0;



			g_InputSystem->EnableInput( !Menu::Get( ).IsVisible( ) );

			if ( g_EngineClient->IsInGame( ) )
			{

				if ( !g_LocalPlayer )
					return;




				if ( g_Options.esp_enabled || g_Options.misc_radar )
				{
					for ( auto i = 0; i <= g_EntityList->GetHighestEntityIndex( ); ++i )
					{
						C_BasePlayer* entity = ( C_BasePlayer* ) g_EntityList->GetClientEntity( i );

						if ( !entity )
							continue;

						if ( entity == g_LocalPlayer )
							continue;

						if ( !entity->IsDormant( ) )
						{

							if ( g_Options.misc_radar && entity->IsAlive( ) )
							{
								static auto m_bSpotted = NetvarSys::Get( ).GetOffset( "DT_BaseEntity" , "m_bSpotted" );

								*( char* ) ( ( DWORD ) ( entity ) +m_bSpotted ) = 1;
							}

							if ( g_Options.esp_planted_c4 && entity->GetClientClass( )->m_ClassID == ClassId::ClassId_CPlantedC4 )
								Visuals::Misc::RenderPlantedC4( entity );

							if ( !entity->IsAlive( ) ) continue;

							if ( g_Options.esp_enabled )
							{
								if ( entity->IsPlayer( ) )
								{

									bool can_draw = true;



									if ( Visuals::Player::Begin( entity ) )
									{

										if (can_draw)
										{
											if (g_Options.esp_player_snaplines) Visuals::Player::RenderSnapline();
											if (g_Options.esp_player_boxes)     Visuals::Player::RenderBox();
											if (g_Options.esp_player_weapons)   Visuals::Player::RenderWeapon();
											if (g_Options.esp_player_names)     Visuals::Player::RenderName();
											if (g_Options.esp_player_health)    Visuals::Player::RenderHealth();
											if (g_Options.esp_skeleton)			Visuals::Player::RenderSkeleton();
										}

									}
								}

								if ( g_Options.esp_dropped_weapons && entity->IsWeapon( ) )
								{
									Visuals::Misc::RenderWeapon( ( C_BaseCombatWeapon* ) entity );
								}
								if ( g_Options.esp_defuse_kit && entity->IsDefuseKit( ) )
								{
									Visuals::Misc::RenderDefuseKit( entity );
								}
								if (g_Options.esp_nade_esp)
									Visuals::Misc::RenderNadeESP((C_BaseCombatWeapon*)entity);
							}
						}
					}
				}

			}
		}
	}
	//--------------------------------------------------------------------------------
	typedef float( __thiscall *get_fov_t )( void* );
	float __fastcall hkGetViewModelFOV( void* ecx , void* edx )
	{

		static auto ofunc = clientmode_hook.get_original<get_fov_t>( 35 );
		float viewmodelFOV = ofunc( ecx );



		if ( !g_Options.misc_viewmodel_fov || !g_LocalPlayer )
			return 68;
		else
		{
			if ( !g_LocalPlayer->m_bIsScoped( ) )
				return g_Options.misc_viewmodel_fov;
			else
				return 68;
		}

		return 68;
	}


	//--------------------------------------------------------------------------------

	void __stdcall hkFrameStageNotify( ClientFrameStage_t stage )
	{

		static auto ofunc = hlclient_hook.get_original<FrameStageNotify>( 37 );

		if ( stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START )
		{
			auto local_id = g_EngineClient->GetLocalPlayer( );
			if ( local_id )
			{
				C_BasePlayer* localplayer = ( C_BasePlayer* ) g_EntityList->GetClientEntity( local_id );

				if ( localplayer && localplayer->IsAlive( ) )
				{
					C_Skins::Get( ).Skinchanger( );
					C_Skins::Get( ).Glovechanger( );
				}
			}


		}



		ofunc( g_CHLClient , stage );
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkOverrideView( CViewSetup* vsView )
	{

		static auto ofunc = clientmode_hook.get_original<OverrideView>( index::OverrideView );

		if (g_EngineClient->IsInGame())
		{
			auto xd = g_CVar->FindVar("mat_postprocess_enable");
			xd->SetValue(0);


		}


		if ( g_LocalPlayer )
		{
			if ( !g_LocalPlayer->m_bIsScoped( ))
			{
				static auto viewmodel = g_CVar->FindVar( "viewmodel_fov" );

				if ( g_Options.misc_viewmodel_fov )
					viewmodel->SetValue( g_Options.misc_viewmodel_fov );
				else
					viewmodel->SetValue( viewmodel->GetDefault( ) );

				if ( g_Options.misc_fov )
					vsView->fov = g_Options.misc_fov;
			}


		}

		ofunc( g_ClientMode , vsView );
	}
	//--------------------------------------------------------------------------------
	void __stdcall hkDrawModelExecute( IMatRenderContext* ctx , const DrawModelState_t& state , const ModelRenderInfo_t& pInfo , matrix3x4_t* pCustomBoneToWorld )
	{
		static auto ofunc = mdlrender_hook.get_original<DrawModelExecute>( index::DrawModelExecute );
		Chams::Get().OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
		ofunc( g_MdlRender , ctx , state , pInfo , pCustomBoneToWorld );
		g_MdlRender->ForcedMaterialOverride( nullptr );
	}

	auto dwCAM_Think = Utils::PatternScan( GetModuleHandleW( L"client_panorama.dll") , "85 C0 75 30 38 86" );
	typedef bool( __thiscall *svc_get_bool_t )( PVOID );
	bool __fastcall hkSvCheatsGetBool( PVOID pConVar , void* edx )
	{
		static auto ofunc = sv_cheats.get_original<svc_get_bool_t>( 13 );
		if ( !ofunc )
			return false;

		if ( reinterpret_cast< DWORD >( _ReturnAddress( ) ) == reinterpret_cast< DWORD >( dwCAM_Think ) )
			return true;
		return ofunc( pConVar );
	}
}