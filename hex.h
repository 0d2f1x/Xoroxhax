#pragma once
#include "vars.h"

nlohmann::json json;

class Hex
{
public:
	static string rad()
	{
		for (short i = 0; i < var.maxPlayers; i++) {
			DWORD entity = Memes.RPM<DWORD>(var.gameModule + offset.entityList + i * 0x10);
			if (entity != NULL) {
				var.viewAngle = Memes.RPM<float>(entity + offset.angEyeAnglesY);
				entityData = Memes.RPM<entityData_t>(entity);
				if (!entityData.m_bSpotted) Memes.WPM<bool>(entity + offset.spotted, true);
				DWORD x = Memes.RPM<DWORD>(Memes.RPM<DWORD>(var.UserInfoTable + 0x40) + 0xC);
				playerInfo = Memes.RPM<player_info_t>(Memes.RPM<DWORD>(x + 0x28 + 0x34 * i));
				if (i == 0) 
					var.myName = playerInfo.name;
				prepareData();
			}
		}
		//system("CLS");
		//cout << json.dump();
		json.clear(); //do not remove
		return var.data;
	}

	static void flash()
	{
		DWORD flashDuration = Memes.RPM<int>(var.localPlayer + offset.flashDuration);
		if (flashDuration > 0) Memes.WPM<int>(var.localPlayer + offset.flashDuration, 0);
	}

	static void trigger()
	{
		while (true) {
			if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0 && (GetKeyState(VK_LBUTTON) & 0x100) == 0) {
				const int inCrossEntityId = Memes.RPM<int>(var.localPlayer + offset.crosshairId);
				if (inCrossEntityId > 0 && inCrossEntityId <= var.maxPlayers && Speed(Memes.RPM<vector3>(var.localPlayer + offset.vecVelocity)) < 125) {
					const unsigned int inCrossEntityBase = Memes.RPM<unsigned int>(var.gameModule + offset.entityList + (inCrossEntityId - 1) * 0x10);
					if (Memes.RPM<int>(inCrossEntityBase + offset.iTeamNum) != Memes.RPM<int>(var.localPlayer + offset.iTeamNum)) {
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
				}
			}
			Sleep(1);
		}
	}

	static void headWalker() //TODO: DEFINE FOR MEMES //
	{
		DWORD targetEntity = NULL;
		while (true) {
			if ((GetKeyState(VK_MBUTTON) & 0x100) != 0) { // select target
				const int inCrossEntityId = Memes.RPM<int>(var.localPlayer + offset.crosshairId);
				if (inCrossEntityId > 0 && inCrossEntityId <= var.maxPlayers)
					targetEntity = Memes.RPM<DWORD>(var.gameModule + offset.entityList + (inCrossEntityId - 1) * 0x10);
			}
			if ((GetKeyState(VK_F3) & 0x100) != 0) //turn off forward when we ON or OFF
				Memes.WPM<int>(var.gameModule + offset.dwForceForward, 0);
			if ((GetKeyState(VK_F3) & 0x0001) != 0 && targetEntity != NULL) { //main toggle
				vector3 localPlayerOrigin = Memes.RPM<vector3>(var.localPlayer + offset.vecOrigin);
				vector3 targetPlayerOrigin = Memes.RPM<vector3>(targetEntity + offset.vecOrigin);
				if (Speed(Memes.RPM<vector3>(targetEntity + offset.vecVelocity)) > 0) {
					Memes.WPM<float>(var.clientState + offset.ViewAngles + 0x4, calcAngle(localPlayerOrigin, targetPlayerOrigin));
					Memes.WPM<int>(var.gameModule + offset.dwForceForward, 1);
				}
				else
					Memes.WPM<int>(var.gameModule + offset.dwForceForward, 0);
			}
			Sleep(1);
		}
	}

	static void prepareData() //pack coords data
	{
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["X"] = int((var.mapX - entityData.m_vecOrigin.X) / -var.mapScale);
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Y"] = int((var.mapY - entityData.m_vecOrigin.Y) / var.mapScale);
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Z"] = int(entityData.m_vecOrigin.Z);
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Health"] = entityData.m_iHealth; //TODO: c4owner, defusing
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Dormant"] = entityData.m_bDormant;
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["ViewAngle"] = var.viewAngle;
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["SteamId"] = playerInfo.guid;
		json["softer"] = var.myName;
		var.data = json.dump();
	}
};
