#define NOMINMAX
#include <Windows.h>

#include "valve_sdk/sdk.hpp"
#include "helpers/utils.hpp"
#include "helpers/input.hpp"
#include "helpers/Config.h"
#include "hooks.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "Seq.h"
#include "imgui/imgui.h"
#include "NetVarHooks.h"

#include <wininet.h>
#include <string.h>
# include <ctime>
#pragma comment(lib, "wininet")



DWORD WINAPI OnDllAttach( LPVOID base )
{

	if (Utils::WaitForModules(10000, { L"client_panorama.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT) {
		// waiting for shit to load nigga
		return FALSE;
    }

#ifdef _DEBUG
	Utils::AttachConsole( );
#endif

	try {
		Utils::ConsolePrint( "Initializing...\n" );



		SHGetFolderPathA( NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, ( LPSTR ) ConfigSys::Get( ).my_documents_folder );

		CreateDirectoryA( std::string( ConfigSys::Get( ).my_documents_folder + std::string( "\\Euphoria\\" ) ).c_str( ), NULL );



		Interfaces::Initialize( );



		NetvarSys::Get( ).Initialize( );
		InputSys::Get( ).Initialize( );
		Menu::Get( ).Initialize( );

		Hooks::Initialize( );
		NetvarHook( );





		InputSys::Get( ).RegisterHotkey( VK_F9, [base] ( ) {
			if ( Menu::Get( ).IsVisible( ) )
				Menu::Get( ).Toggle( );
			g_Unload = true;
		} );

		// Menu Toggle
		InputSys::Get( ).RegisterHotkey( VK_END, [base] ( ) {
			Menu::Get( ).Toggle( );
		} );

		Utils::ConsolePrint( "Finished.\n" );

		while ( !g_Unload )
			Sleep( 1000 );


#ifdef _DEBUG
		Utils::DetachConsole( );
#endif
		Hooks::Shutdown( );
		Menu::Get( ).Shutdown( );
		UnloadProxy( );

		FreeLibraryAndExitThread( static_cast< HMODULE >( base ), 1 );

	}
	catch ( const std::exception& ex ) {
		Utils::ConsolePrint( "An error occured during initialization:\n" );
		Utils::ConsolePrint( "%s\n", ex.what( ) );
		Utils::ConsolePrint( "Press any key to exit.\n" );
		Utils::ConsoleReadKey( );
		Utils::DetachConsole( );

		FreeLibraryAndExitThread( static_cast< HMODULE >( base ), 1 );
	}


}

BOOL WINAPI OnDllDetach()
{
#ifdef _DEBUG
	Utils::DetachConsole();
#endif
	Hooks::Shutdown();
	Menu::Get().Shutdown();
	UnloadProxy(); //is this for the skinchanger animnation meme? yes show me
    return TRUE;
}
BOOL WINAPI DllMain(
    _In_      HINSTANCE hinstDll,
    _In_      DWORD     fdwReason,
    _In_opt_  LPVOID    lpvReserved
)
{
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDll);
            CreateThread(nullptr, 0, OnDllAttach, hinstDll, 0, nullptr);
            return TRUE;
        case DLL_PROCESS_DETACH:
			if (lpvReserved == nullptr)
				return OnDllDetach();
            return TRUE;
        default:
            return TRUE;
    }
}
