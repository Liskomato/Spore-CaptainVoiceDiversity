#pragma once

class CaptainVoiceDiversity					// Defines the class.
{
public:
	char padding[0x18];						// Ignore these bytes.
	hash_map<uint32_t, string8> voiceMap;   // This hash map will allow us to swap the captain voice.

};