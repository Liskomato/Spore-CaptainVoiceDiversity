#include "stdafx.h"
#include "SporepediaStateListener.h"

// Define the static variables first.
bool SporepediaStateListener::IsSporepediaOpen = false;
uint32_t SporepediaStateListener::previewCreatureMouthID = 0x0;

SporepediaStateListener::SporepediaStateListener()
{
}


SporepediaStateListener::~SporepediaStateListener()
{
}

// For internal use, do not modify.
/*int SporepediaStateListener::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int SporepediaStateListener::Release()
{
	return DefaultRefCounted::Release();
}*/


// The method that receives the message. The first thing you should do is checking what ID sent this message...
bool SporepediaStateListener::HandleMessage(uint32_t messageID, void* message)
{
	if (messageID == id("SporepediaState")) {
		IsSporepediaOpen = *((bool*)message);
	}
	if (messageID == id("CreatureMouthID")) {
		previewCreatureMouthID = *((uint32_t*)message);
	}

	// Return true if the message has been handled. Other listeners will receive the message regardless of the return value.
	return true;
}


bool SporepediaStateListener::GetSporepediaState()
{
	return IsSporepediaOpen;
}

uint32_t SporepediaStateListener::GetMouthID()
{
	return previewCreatureMouthID;
}

void SporepediaStateListener::SetSporepediaState(bool state) {

	IsSporepediaOpen = state;

}

void SporepediaStateListener::SetMouthID(uint32_t mouthID) {

	previewCreatureMouthID = mouthID;

}