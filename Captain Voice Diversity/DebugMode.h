#pragma once

#include <Spore\BasicIncludes.h>

class DebugMode 
	: public ArgScript::ICommand
{
public:
	DebugMode();
	~DebugMode();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;

	static bool debugEnabled;
};

// We use this to identify mouth type for debug info. 
static hash_map<uint32_t, string8> mouthMap {
	// Insect voice
	{0x3C7E0F6E, "MCA"},
	{0x3C7E0F6D, "MCB"},
	{0x3C7E0F6C, "MCC"},
	{0x3C7E0F6B, "MCD"},
	
	
	{0x4072DC09, "ROA"},
	{0x4072DC0A, "ROB"},
	{0x4072DC0B, "ROC"},
	{0x4072DC0C, "ROD"},

	// Bird voice
	{0x1C99B34D, "BCA"},
	{0x1C99B34E, "BCB"},
	{0x1C99B34F, "BCC"},
	{0x1C99B348, "BCD"},
	{0x2799C41C, "BHA"},
	{0x2799C41F, "BHB"},
	{0x2799C41E, "BHC"},
	{0x2799C419, "BHD"},

	// Mammal voice
	{0x409C2A0E, "AOA"},
	{0x409C2A0D, "AOB"},
	{0x409C2A0C, "AOC"},
	{0x409C2A0B, "AOD"},
	{0x448F64A9, "FCA"},
	{0x448F64AA, "FCB"},
	{0x448F64AB, "FCC"},
	{0x448F64AC, "FCD"},
	{0x2C8644C5, "JCA"},
	{0x2C8644C6, "JCB"},
	{0x2C8644C7, "JCC"},
	{0x2C8644C0, "JCD"},
	{0x27863CA4, "JHA"},
	{0x27863CA7, "JHB"},
	{0x27863CA6, "JHC"},
	{0x27863CA1, "JHD"},
	{0x407759F7, "POA"},
	{0x407759F4, "POB"},
	{0x407759F5, "POC"},
	{0x407759F2, "POD"},
	{0x21706D65, "SHA"},
	{0x21706D66, "SHB"},
	{0x21706D67, "SHC"},
	{0x21706D60, "SHD"},
	
	// Null value, or N/A
	{0, "N/A"}
};

