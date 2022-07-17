#pragma once

#include <Spore\BasicIncludes.h>

#define SporepediaStateListenerPtr intrusive_ptr<SporepediaStateListener>

class SporepediaStateListener 
	: public App::IUnmanagedMessageListener
{
public:
	static const uint32_t TYPE = id("SporepediaStateListener");

	SporepediaStateListener();
	~SporepediaStateListener();

	
	// This is the function you have to implement, called when a message you registered to is sent.
	bool HandleMessage(uint32_t messageID, void* message) override;
	static bool IsSporepediaOpen;			// Property to see if a large Sporepedia card is open. Default state is false.
	static uint32_t previewCreatureMouthID; // Variable that receives the animated creature's mouthID from a message with ID "CreatureMouthID".

	static bool GetSporepediaState();		// Returns value for variable IsSporepediaOpen.
	static uint32_t GetMouthID();			// Returns the mouthID the listener class received earlier.

	static void SetSporepediaState(bool state);
	static void SetMouthID(uint32_t mouthID);
};
