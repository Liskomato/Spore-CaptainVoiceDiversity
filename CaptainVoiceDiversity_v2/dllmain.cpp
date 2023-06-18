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

/// The original main function of the mod. Placed here as context of what came before.
/*
member_detour(captainvoicediversity, CaptainVoiceDiversity, void(void*, char8_t*, char8_t*, int32_t)) {
	void detoured(void* p1, char8_t* p2, char8_t* p3, int32_t p4) {
		
		
		original_function(this,p1, p2, p3, p4);
	}
};
*/

///
/// Function parameters
/// int id:  ID of the property function reads from
/// int* target_data: pointer to the value being written to.
///
member_detour(cvd_assigndata, CaptainVoiceDiversity, void(int, int*)) {
	void detoured(int id, int* target_data) {
		// We will first allow the original function to run.
		original_function(this, id, target_data);

		// Then check if ID matches and if target integer is above 5.
		if (id == 0x39f9e08 && *target_data > 5) {
			/// If target_data is above 5, change it to 5. The value represents creature type.
			/// 
			/// 4 and below is creature, 5 is tribal, and 6 and above are civilian, colonist, etc.
			/// function at address 0xa3b200 (Disc) / 0xa3b350 (March 2017) checks if target_data is 5. If above 6, it will always run the same voice for every creature.
			/// 
			/// In this case, we do not want to have target_data above 5, so we change it to 5 if it goes over.
			///
			*target_data = 5;
		}
	}
};

void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
//	captainvoicediversity::attach(Address(ModAPI::ChooseAddress(0xa3b200, 0xa3b350)));
	cvd_assigndata::attach(Address(ModAPI::ChooseAddress(0xa0fa40,0xa0fa30)));

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

