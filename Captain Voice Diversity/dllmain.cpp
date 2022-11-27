// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "captainvoicediversity.h"
#include "SporepediaStateListener.h"
#include "DebugMode.h"
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
	
// Uncomment ManualBreakpoint() when you want to debug this program.
//	ManualBreakpoint();


// 	In case listeners are needed again, uncomment the lines below.
// 
//  auto listener = new SporepediaStateListener();
//	MessageManager.AddUnmanagedListener(listener, id("SporepediaState"));   // Boolean value for checking if a large Sporepedia preview is opened.
//	MessageManager.AddUnmanagedListener(listener, id("CreatureMouthID"));   // For passing over animated creature's mouth type when not an adventure avatar.
			
// Adds the "switch" cheat for the mod's debugging mode.
	CheatManager.AddCheat("CVDEnableDebug", new DebugMode());
}

using namespace Anim;

// Looks through the anim_block vector to see if there exists a mouth and its voice ID. Returns said voice ID as uint32_t variable.
uint32_t GetMouthType(eastl::vector<anim_block> blocks) {

	// Declaring variables
	uint32_t mouthID = 0;
	uint32_t i = 0;
	
	
	// Cycle through the blocks of the vector until a member variable soundIDMouth is found.
	for (i = 0; i < blocks.size(); i++) {
		// If mouth ID is found, assign it to variable mouthID and break the loop.
		if (blocks[i].pBlock->soundIDMouth) {
			mouthID = blocks[i].pBlock->soundIDMouth;
			break;
		}
	};
	

	// Return the member variable soundIDMouth as a value if found.
	if (mouthID != 0) {
		return mouthID;
	}


	// Return 0 as function value if no mouth part is found.
	if (DebugMode::debugEnabled) {
		App::ConsolePrintF("Captain Voice Diversity DEBUG: No mouth part could be found for creature. This likely means the creature is mouthless, so returning 0 as value.");
	}
	return 0;
													
};

// Function for printing debug information to the console.
void PrintDebugVoiceInfo(uint32_t mouthID, string8 sentientVoiceType) {
	// We need to first convert our two relevant strings to C-style strings, so they show properly in the console.
	string mouth, sentient;
	mouth.assign_convert(mouthMap[mouthID]);
	sentient.assign_convert(sentientVoiceType);
	App::ConsolePrintF("Captain Voice Diversity DEBUG - Mouth ID: 0x%X, Mouth type: %s, Sentient voice type: %s",mouthID,mouth,sentient);
}


// 
// The main detour of the mod. It detours a function in address 0xa3b350 (March 2017) / 0xa138b0 (Disc) that determines the voice
// of creatures depending on a hash map that gathers its information from audio_mouthmap~!base.soundProp file.
// 
// For the purposes of this mod, we want to change the value of the resource key with the hash 0x0B65639D (defaultIn in SporeModderFX), 
// since that is the resource key that determines the voices of all civilized, space and captain creatures, along with being the fallback
// for the case where certain audio types lack their own voice file variants. 
// 
member_detour(captainvoicediversity, CaptainVoiceDiversity, bool(uint32_t, uint8_t, uint32_t, uint32_t)) {
	bool detoured(uint32_t a1, uint8_t a2, uint32_t a3, uint32_t a4) {
		
		//
		// For reference, these two sets (InsA_set and BirA_set) are for when we want to match the creature's mouthID with them to determine if
		//  we should give them a bird voice (BirA) or an insect voice (InsA).
		//
		// These are the "real" names of the hashes in question, as referenced from audio_mouthmap~!base.soundProp
		// 
		//									Mandible mouths (carnivore)
		//							  MCA			MCB			 MCC		MCD
		set<uint32_t> InsA_set = { 0x3C7E0F6E , 0x3C7E0F6D , 0x3C7E0F6C , 0x3C7E0F6B ,
								   0x4072DC09 , 0x4072DC0A , 0x4072DC0B , 0x4072DC0C };
		//							  ROA			ROB			 ROC		 ROD
		//									Radial mouths (omnivore)
		// 
		//									   Bird beaks (omnivore)
		//							  BCA			BCB			BCC			  BCD
		set<uint32_t> BirA_set = { 0x1C99B34D , 0x1C99B34E , 0x1C99B34F , 0x1C99B348 ,
								   0x2799C41C , 0x2799C41F , 0x2799C41E , 0x2799C419 };
		//							  BHA			BHB			BHC			  BHD
		//									   Bird beaks (herbivore)
		// 
		//
		// 
		// 
		// SporepediaStateListener::GetSporepediaState() returns true if boolean value IsSporepediaOpen is true. 
		// Based on that, we can give priority to Sporepedia preview creatures when a large Sporepedia card is open.
		//
		if (SporepediaStateListener::GetSporepediaState()) {
		
			// Gets the creature mouth ID that has been loaded in memory, to value SporepediaStateListener::previewCreatureMouthID.
			uint32_t previewMouthType = SporepediaStateListener::GetMouthID();

			// Compare the received mouth ID with those on the two sets we defined before. If equal to one of the components in InsA_set, for example, 
			// define defaultIn as "InsA".
			//
			if (InsA_set.count(previewMouthType)) {
				this->voiceMap[0x0B65639D] = "InsA";
			}
			else if (BirA_set.count(previewMouthType)) {
				this->voiceMap[0x0B65639D] = "BirA";
			}
			else {
				this->voiceMap[0x0B65639D] = "MamA";
			}
		
			// In other cases we set defaultIn into its vanilla behaviour, in which it has the value "MamA".
			// 
			// 
			// Next, if the debug cheat is on, we send the debug information.
			if (DebugMode::debugEnabled) PrintDebugVoiceInfo(previewMouthType,this->voiceMap[0x0B65639D]);

			// After that we return the original function.
			//
			return original_function(this, a1, a2, a3, a4);
		
		}
		

			// Otherwise we do not want the mod to trigger in the overall campaign, with the exception of editors and adventures, of course.
		    //
		if (!(Simulator::IsSpaceGame() || Simulator::IsCivGame() || Simulator::IsTribeGame() || Simulator::IsCreatureGame())) {
			//
			// Since the captain is arguably the biggest user of defaultIn as a value in adventures, we want to prioritize getting their
			// voice right in adventures. Hence the following conditions.
			//
			if (Simulator::IsScenarioMode() && GameNounManager.GetAvatar()) {
			
					//	Get the AnimatedCreature pointer for the avatar/captain. 
					AnimatedCreaturePtr avatarAnimated = GameNounManager.GetAvatar()->mpAnimatedCreature.get();
			
					//	Get the avatar's mouth type with a special function.
					uint32_t mouthType = GetMouthType(avatarAnimated->p_cid->blocks);

					// And at the end, compare it with the existing sets we set up at the start of this function.
					if (InsA_set.count(mouthType)) {
						this->voiceMap[0x0B65639D] = "InsA";
					}
					else if (BirA_set.count(mouthType)) {
						this->voiceMap[0x0B65639D] = "BirA";
					}
					else {
						this->voiceMap[0x0B65639D] = "MamA";
					}
					if (DebugMode::debugEnabled) PrintDebugVoiceInfo(mouthType, this->voiceMap[0x0B65639D]);
			}
			else {
				
			//  
			// Usually this part of the function triggers when we're in the editors, since normally the creatures in them don't change
			// their voice with the exception of the Tribal Outfitter (and tribal stage in general, for that matter).
			//
				uint32_t mouthType = SporepediaStateListener::GetMouthID();
				
				if (InsA_set.count(mouthType)) {
					this->voiceMap[0x0B65639D] = "InsA";
				}
				else if (BirA_set.count(mouthType)) {
					this->voiceMap[0x0B65639D] = "BirA";
				}
				else {
					this->voiceMap[0x0B65639D] = "MamA";
				}
				if (DebugMode::debugEnabled) PrintDebugVoiceInfo(mouthType, this->voiceMap[0x0B65639D]);
			}
		}
		else {
			//
			// When in the campaign, we use default vanilla behaviour since the other voice types lack the audio files 
			// "MamA" has for the later stages.
			//
			this->voiceMap[0x0B65639D] = "MamA";
		}
		//
		// At the end, we return the original function here. If desired, we could also "turn off" the detour's functions so it all 
		// returns to vanilla behaviour entirely.
		//
		 bool ret = original_function(this, a1, a2, a3, a4);
		 
//		 this->voiceMap[0x0B65639D] = "MamA";
		 return ret;
	}

};

//
// The detour for when we want to load creatures for the editor previews. Unlike LoadAnimation(), this function should only
// trigger once per creature.
//
member_detour(LoadCreature_detour, IAnimWorld, AnimatedCreature*(void*,void*,uint32_t,void*,void*,void*, void*)) {
	AnimatedCreature* detoured(void* a1, void* a2, uint32_t a3, void* a4, void* a5, void* a6, void* a7) {
		
		//
		// We first want to get the return value so we execute the original function first.
		// We will assign it to variable "creature".
		//
		auto creature = original_function(this, a1, a2, a3, a4, a5, a6, a7);

		//
		// We want to make sure the right conditions are set first before we do anything else.
		// For this, we've set our condition to be that GetSporepediaState() returns true.
		//

		if (SporepediaStateListener::GetSporepediaState()) {
			
		//
		// If conditions are right, we get the now-loaded creature's mouth type and assign it to SporepediaStateListener::previewCreatureMouthID
		// immediately after.
		//

			uint32_t mouthID = 0;
			mouthID = GetMouthType(creature->p_cid->blocks);
			SporepediaStateListener::SetMouthID(mouthID);

	// There is also debug console output and message to be sent if they are needed.
	// 
	//		App::ConsolePrintF("Captain Voice Diversity: Mouth ID is 0x%X",mouthID);
	//		MessageManager.PostMSG(id("CreatureMouthID"), &mouthID);
			
		}

	// At the end, we return the "creature" variable, as we are now done.
		return creature;
	
	}

};


//
// For editors, we can detour LoadAnimation safely since usually we only need to deal with one type of creature at once. 
// This will also prevent previewed creatures' voices "sticking" to the main edited creature while browsing through Sporepedia.
//
virtual_detour(LoadAnimation_detour, AnimatedCreature, AnimatedCreature, void(uint32_t, int*)) {
	void detoured(uint32_t animID, int* pChoice) {

		if ((Editors::GetEditor() && !Simulator::IsScenarioMode()) && !SporepediaStateListener::GetSporepediaState()) {

			uint32_t mouthID = 0;
			mouthID = GetMouthType(this->p_cid->blocks);
			SporepediaStateListener::SetMouthID(mouthID);
			//		App::ConsolePrintF("Captain Voice Diversity: Mouth ID is 0x%X",mouthID);
			//		MessageManager.PostMSG(id("CreatureMouthID"), &mouthID);

		}

		return original_function(this, animID, pChoice);

	}

};


	//
	// By detouring ItemViewer's HandleUIMessage function, we can set IsSporepediaOpen's boolean value when certain conditions are right.
	//
	virtual_detour(ItemViewer_HandleUIMessage_Detour, Palettes::AdvancedItemViewer, UTFWin::IWinProc, bool(UTFWin::IWindow*,const UTFWin::Message&)) 
	{
		bool detoured(UTFWin::IWindow* pWindow, const UTFWin::Message& message) {
			
		// Debugging information. Uncomment if you want.
		//	App::ConsolePrintF("UI event: Window ID = 0x%X, messageType = 0x%X",pWindow->GetControlID(),message.eventType);

			if (pWindow->GetControlID() == 0xF3C6D819 && message.eventType == UTFWin::kMsgWinProcAdded) {

				SporepediaStateListener::SetSporepediaState(true);

				if (DebugMode::debugEnabled) {
					App::ConsolePrintF("Captain Voice Diversity DEBUG: Large Sporepedia preview is open. Set state to true.");
					//	App::ConsolePrintF("Captain Voice Diversity DEBUG: Sent message to SporepediaStateListener to set boolean true.");
				}
			}
			else if (pWindow->GetControlID() == 0xF3C6D819 && message.eventType == UTFWin::kMsgWinProcRemoved) {
			
				SporepediaStateListener::SetSporepediaState(false);

				if (DebugMode::debugEnabled) {
					App::ConsolePrintF("Captain Voice Diversity DEBUG: Large Sporepedia preview is closed. Set state to false.");
					//	App::ConsolePrintF("Captain Voice Diversity DEBUG: Sent message to SporepediaStateListener to set boolean false.");
				}
			}

			return original_function(this, pWindow, message);
		}
	};



void Dispose()
{
	// This method is called when the game is closing

	// If you add listeners to this mod again, don't forget to uncomment the line below.
	// delete listener;
}

void AttachDetours()
{
	captainvoicediversity::attach(Address(ModAPI::ChooseAddress(0xa138b0, 0xa3b350)));

	LoadCreature_detour::attach(Address(ModAPI::ChooseAddress(0xa0b1c0, 0xa0b1c0)));

	LoadAnimation_detour::attach(Address(ModAPI::ChooseAddress(0xa0c5d0, 0xa0c5d0)));

	ItemViewer_HandleUIMessage_Detour::attach(GetAddress(Palettes::AdvancedItemViewer, HandleUIMessage));

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

