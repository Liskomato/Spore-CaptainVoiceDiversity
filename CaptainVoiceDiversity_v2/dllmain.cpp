// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "captainvoicediversity.h"

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
}

member_detour(captainvoicediversity, CaptainVoiceDiversity, void(uint32_t, char8_t*, char8_t*, int32_t)) {
	void detoured(uint32_t param_2, char8_t* param_3, char8_t* param_4, int32_t param_5) {
		
		if (param_2 == 1) param_2 = 0;
		original_function(this,param_2, param_3, param_4, param_5);
	}
};
void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
	captainvoicediversity::attach(Address(ModAPI::ChooseAddress(0xa138b0, 0xa3b350)));

	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

