#include "stdafx.h"
#include "DebugMode.h"

bool DebugMode::debugEnabled = false;

DebugMode::DebugMode()
{
}


DebugMode::~DebugMode()
{
}


void DebugMode::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.

	bool value;

	try {
		value = mpFormatParser->ParseBool(line.GetArgumentAt(1));
	}
	catch (ArgScript::ArgScriptException) {
		App::ConsolePrintF("Error! Expected a boolean value (true/false).");
		return;
	}

	if (debugEnabled == value) {
		if (debugEnabled) App::ConsolePrintF("Captain Voice Diversity: Debug mode is already enabled.");
		else if (!debugEnabled) App::ConsolePrintF("Captain Voice Diversity: Debug mode is already disabled.");
		return;
	}

	debugEnabled = value;

	if (value) {
		App::ConsolePrintF("Captain Voice Diversity: Debug mode enabled.");
	}
	else if (value == false) {
		App::ConsolePrintF("Captain Voice Diversity: Debug mode disabled.");
	}


}

const char* DebugMode::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Enables debug messages for Captain Voice Diversity.";
	}
	else {
		return "CVDEnableDebug: Turns on debugging messages in the cheat console for the Captain Voice Diversity mod.";
	}
}
