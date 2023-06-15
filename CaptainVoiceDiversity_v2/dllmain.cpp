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

member_detour(captainvoicediversity, CaptainVoiceDiversity, void(void*, char8_t*, char8_t*, int32_t)) {
	void detoured(void* p1, char8_t* p2, char8_t* p3, int32_t p4) {
		
		
		original_function(this,p1, p2, p3, p4);
	}
};


void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
	captainvoicediversity::attach(Address(ModAPI::ChooseAddress(0xa3b200, 0xa3b350)));


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

