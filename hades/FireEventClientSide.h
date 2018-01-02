#pragma once
#include "HookIncludes.h"
#include "SkinChanger.h"
typedef bool(__thiscall *FireEventClientSideFn)(IGameEvent*);
FireEventClientSideFn FireEventClientSide;
inline bool hkFireEventClientSide(IGameEvent *Event);

int Kills2 = 0;
int Kills = 0;
bool RoundInfo = false;
size_t Delay = 0;



inline bool hkFireEventClientSide(IGameEvent *Event) {
	// Get the user ID of the attacker.
	int UserID = Event->GetInt("attacker");

	if (!UserID)
		return false;

	// Only continue if we were the attacker.
	if (g_Engine->GetPlayerForUserID(UserID) != g_Engine->GetLocalPlayer())
		return false;

	// Get the original weapon used to kill.
	const char* szWeapon = Event->GetString("weapon");
	/*
	for (auto ReplacementIcon : killIcons) {
		// Search for a valid replacement.
		if (!strcmp(szWeapon, ReplacementIcon.first)) {
			// Replace with user defined value.
			Event->SetString("weapon", ReplacementIcon.second);
			break;
		}
		}
		*/
	
	return true;


}

