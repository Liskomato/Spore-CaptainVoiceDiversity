// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "captainvoicediversity.h"
#include "captainvoice_editorpreview.h"
#include "SporepediaStateListener.h"
#include <Spore\Properties.h>
#include <Spore\Palettes\AdvancedItemViewer.h>

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
//	CheatManager.AddCheat("functionFinder", new FunctionFinder());
	
	ManualBreakpoint();
//	MessageManager.AddUnmanagedListener(new SporepediaStateListener(), id("SporepediaState"));   // Boolean value for checking if a large Sporepedia preview is opened.
//	MessageManager.AddUnmanagedListener(new SporepediaStateListener(), id("CreatureMouthID"));   // For passing over animated creature's mouth type when not an adventure avatar.
//	MessageManager.AddUnmanagedListener(new SporepediaStateListener(), UTFWin::MessageType::kMsgButtonClick);   // Close button for large Sporepedia previews.
			
}

using namespace Anim;

// Looks through the anim_block vector to see if there exists a mouth and its voice ID. Returns said voice ID as uint32_t variable.
uint32_t GetMouthType(eastl::vector<anim_block> blocks) {

	// Declaring variables
	uint32_t mouthID;
	int i = 0;
	
	// Cycle through the blocks of the vector until a member variable soundIDMouth is found.
	for (i = 0; !blocks[i].pBlock->soundIDMouth; i++) {
		// Break loop if no mouth ID is found.
		if (i == blocks.size() && !blocks[i].pBlock->soundIDMouth) {
			break;
		}
	};
	
	// Return 0 as function value if no mouth part is found.
	if (!blocks[i].pBlock->soundIDMouth) {
		App::ConsolePrintF("Captain Voice Diversity: Error! No mouth part could be found for creature. Returning 0 as value.");
		return 0;
	}

	// Return the member variable soundIDMouth as a value if found.
	mouthID = blocks[i].pBlock->soundIDMouth;
	return mouthID;												
};




// DETOUR	
member_detour(captainvoicediversity, CaptainVoiceDiversity, bool(uint32_t, uint8_t, uint32_t, uint32_t)) {
	bool detoured(uint32_t a1, uint8_t a2, uint32_t a3, uint32_t a4) {
		
		set<uint32_t> InsA_set = { 0x3C7E0F6E , 0x3C7E0F6D , 0x3C7E0F6C , 0x3C7E0F6B ,
								   0x4072DC09 , 0x4072DC0A , 0x4072DC0B , 0x4072DC0C };
		set<uint32_t> BirA_set = { 0x1C99B34D , 0x1C99B34E , 0x1C99B34F , 0x1C99B348 ,
								   0x2799C41C , 0x2799C41F , 0x2799C41E , 0x2799C419 };
		
		
		if (SporepediaStateListener::GetSporepediaState()) {
		
			uint32_t previewMouthType = SporepediaStateListener::GetMouthID();

			if (previewMouthType) {
				if (InsA_set.count(previewMouthType)) {
					this->voiceMap[0x0B65639D] = "InsA";
				}
				else if (BirA_set.count(previewMouthType)) {
					this->voiceMap[0x0B65639D] = "BirA";
				}
				else {
					this->voiceMap[0x0B65639D] = "MamA";
				}
			}
			else {
				App::ConsolePrintF("Captain Voice Diversity: Error! Null pointer exception. Setting value to default behaviour.");
				this->voiceMap[0x0B65639D] = "MamA";
			}
			return original_function(this, a1, a2, a3, a4);
		
		}
		
		if (!(Simulator::IsSpaceGame() || Simulator::IsCivGame())) {
			if (Simulator::IsScenarioMode() && GameNounManager.GetAvatar()) {
			
					AnimatedCreaturePtr avatarAnimated = GameNounManager.GetAvatar()->mpAnimatedCreature.get();

					uint32_t mouthType = GetMouthType(avatarAnimated->p_cid->blocks);

			//		bool SporepediaClosed = false;

					if (InsA_set.count(mouthType)) {
						this->voiceMap[0x0B65639D] = "InsA";
					}
					else if (BirA_set.count(mouthType)) {
						this->voiceMap[0x0B65639D] = "BirA";
					}
					else {
						this->voiceMap[0x0B65639D] = "MamA";
					}
					
			//		MessageManager.PostMSG(id("SporepediaState"), &SporepediaClosed);

			}
			else {
				uint32_t mouthType = SporepediaStateListener::GetMouthID();
				
				if (mouthType) {
					if (InsA_set.count(mouthType)) {
						this->voiceMap[0x0B65639D] = "InsA";
					}
					else if (BirA_set.count(mouthType)) {
						this->voiceMap[0x0B65639D] = "BirA";
					}
					else {
						this->voiceMap[0x0B65639D] = "MamA";
					}
				}
				else {
					App::ConsolePrintF("Captain Voice Diversity: Error! Null pointer exception. Setting value to default behaviour.");
					this->voiceMap[0x0B65639D] = "MamA";
				}
			}
		}
		else {
			this->voiceMap[0x0B65639D] = "MamA";
		}
		 bool ret = original_function(this, a1, a2, a3, a4);
//		 this->voiceMap[0x0B65639D] = "MamA";
		 return ret;
	}

};

/*virtual_detour(captainTypeEdit, cAnimWorld, IAnimWorld, AnimatedCreaturePtr(ResourceKey&, int, Vector3&, Vector3&, bool)) {
	
	AnimatedCreaturePtr detoured(ResourceKey & key, int a2, Vector3 & a3, Vector3 & a4, bool a5) {

		if (Simulator::IsScenarioMode && key == GameNounManager.GetAvatar()->GetModelKey()) {
			
			PropertyListPtr keyPropList;
			PropManager.GetPropertyList(key.instanceID,key.groupID,keyPropList);

			
			

		//	App::Property* avatarModelType;
		//	avatarPropList->GetProperty(0xB0351B13, avatarModelType);			

		}
		return original_function(this,key, a2, a3, a4, a5);
	}
	
};*/

/*static_detour(Create_Detour, Simulator::cCreatureAnimal* (const Vector3&, Simulator::cSpeciesProfile*, int, Simulator::cHerd*, bool, bool)) {
	Simulator::cCreatureAnimal* detoured(const Vector3 & pPosition, Simulator::cSpeciesProfile * pSpecies, int age, Simulator::cHerd * pHerd, bool bIsAvatar, bool unk) {
		if (bIsAvatar == true) {
			;
			return original_function(pPosition, pSpecies/*SpeciesManager.GetSpeciesProfile({0x066B8241, TypeIDs::crt, GroupIDs::CreatureModels}), age, pHerd, bIsAvatar, unk);
		}
		else {
			return original_function(pPosition, pSpecies, age, pHerd, bIsAvatar, unk);
		}
	}
};*/

virtual_detour(LoadAnimation_detour, AnimatedCreature, AnimatedCreature, void(uint32_t, int*)) {
	void detoured(uint32_t animID, int* pChoice) {

		if ((Editors::GetEditor() && !Simulator::IsScenarioMode()) || SporepediaStateListener::GetSporepediaState()) {
			
			uint32_t mouthID = 0;
			mouthID = GetMouthType(this->p_cid->blocks);
			SporepediaStateListener::SetMouthID(mouthID);
			App::ConsolePrintF("Captain Voice Diversity: Mouth ID is 0x%X",mouthID);
	//		MessageManager.PostMSG(id("CreatureMouthID"), &mouthID);
			
		}

		return original_function(this, animID, pChoice);
	
	}

};

/*member_detour(SporepediaShow_detour, Sporepedia::ShopperRequest, void(Sporepedia::ShopperRequest&)) {

	void detoured(ShopperRequest &request) {
		
		bool SporepediaOpened = true;
		MessageManager.PostMSG(id("SporepediaState"),&SporepediaOpened);
		original_function(this, request);
		
	}

};*/
namespace Palettes {
/*	member_detour(ItemViewerInitialize_detour, AdvancedItemViewer, void(const ResourceKey&, IWindow*, IWindow*, uint32_t, PaletteItem*, PaletteInfo*, bool))
	{
		void detoured(const ResourceKey & nameKey, IWindow * pWindow, IWindow * pParentWindow,
			uint32_t messageID, PaletteItem * pItem, PaletteInfo * pPaletteInfo, bool b) {
			original_function(this, nameKey, pWindow, pParentWindow, messageID, pItem, pPaletteInfo, b);

			bool SporepediaOpen = true;
			MessageManager.PostMSG(id("SporepediaState"), &SporepediaOpen);
		}

	};*/

	virtual_detour(ItemViewer_HandleUIMessage_Detour, AdvancedItemViewer, IWinProc, bool(UTFWin::IWindow*,const UTFWin::Message&)) 
	{
		bool detoured(IWindow* pWindow, const UTFWin::Message& message) {
			
		// Debugging information. Uncomment if you want.
			App::ConsolePrintF("UI event: Window ID = 0x%X, messageType = 0x%X",pWindow->GetControlID(),message.eventType);

			if (pWindow->GetControlID() == 0xF3C6D819 && message.eventType == kMsgWinProcAdded) {

				SporepediaStateListener::SetSporepediaState(true);

				App::ConsolePrintF("Captain Voice Diversity: Large Sporepedia preview is open. Set state to true.");
		//		App::ConsolePrintF("Captain Voice Diversity DEBUG: Sent message to SporepediaStateListener to set boolean true.");
			}
			else if (pWindow->GetControlID() == 0xF3C6D819 && message.eventType == kMsgWinProcRemoved) {
			
				SporepediaStateListener::SetSporepediaState(false);

				App::ConsolePrintF("Captain Voice Diversity: Large Sporepedia preview is closed. Set state to false.");
		//		App::ConsolePrintF("Captain Voice Diversity DEBUG: Sent message to SporepediaStateListener to set boolean false.");
			}

			return original_function(this, pWindow, message);
		}
	};

}

/*virtual_detour(ItemViewerOpen_detour, Palettes::AdvancedItemViewer, Palettes::ItemViewer, void(ResourceKey&)) {

	void detoured(ResourceKey& t) {
	
		bool SporepediaOpen = true;
		MessageManager.PostMSG(id("SporepediaState"), &SporepediaOpen);

//		uint32_t mouthID = GetMouthType(this->GetAnimatedCreature()->p_cid->blocks);
//		MessageManager.PostMSG(id("CreatureMouthID"),&mouthID);
		
		original_function(this, t);

	}

};*/

/*virtual_detour(ItemViewerClosed_detour, Palettes::AdvancedItemViewer, Palettes::ItemViewer, void(void)) {

	void detoured() {

		bool SporepediaOpen = false;
		uint32_t mouthID = 0;

		MessageManager.PostMSG(id("SporepediaState"), &SporepediaOpen);
		MessageManager.PostMSG(id("CreatureMouthID"), &mouthID);

		original_function(this);

	}

};*/

void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
	captainvoicediversity::attach(Address(ModAPI::ChooseAddress(0xa138b0, 0xa3b350)));

	LoadAnimation_detour::attach(Address(ModAPI::ChooseAddress(0xa0c5d0, 0xa0c5d0)));

	Palettes::ItemViewer_HandleUIMessage_Detour::attach(GetAddress(Palettes::AdvancedItemViewer, HandleUIMessage));

	// UNUSED DETOURS
	//	captainTypeEdit::attach(GetAddress(cAnimWorld,LoadCreature));
	//	Create_Detour::attach(GetAddress(Simulator::cCreatureAnimal, Create));
	//	SporepediaShow_detour::attach(GetAddress(Sporepedia::ShopperRequest, Show));
	//	ItemViewerOpen_detour::attach(GetAddress(Palettes::AdvancedItemViewer, Load));
	//	ItemViewerClosed_detour::attach(GetAddress(Palettes::AdvancedItemViewer, OnOutside));
	//	Palettes::ItemViewerInitialize_detour::attach(GetAddress(Palettes::AdvancedItemViewer, Initialize));

	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain(HMODULE hModule,
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

