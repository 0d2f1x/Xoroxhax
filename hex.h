#pragma once
#include "vars.h"

nlohmann::json json;

class Hex
{
public:

	static string rad()
	{
		for (short i = 0; i < var.maxPlayers; i++)
		{
			DWORD entity = Memes.RPM<DWORD>(var.gameModule + offset.entityList + i * 0x10);
			if (entity != NULL) {
				var.viewAngle = Memes.RPM<float>(entity + offset.angEyeAnglesY);
				entityData = Memes.RPM<entityData_t>(entity);
				if (!entityData.m_bSpotted) Memes.WPM<bool>(entity + offset.spotted, true);
				DWORD x = Memes.RPM<DWORD>(Memes.RPM<DWORD>(var.UserInfoTable + 0x40) + 0xC);
				playerInfo = Memes.RPM<player_info_t>(Memes.RPM<DWORD>(x + 0x28 + 0x34 * i));
				if (i == 0) {
					var.myName = playerInfo.name;
					var.localPlayerOrigin = entityData.m_vecOrigin;
					//headWalker();
				}
				if (i != 0 && abs(entityData.m_vecOrigin.X - var.localPlayerOrigin.X) < 50 && abs(entityData.m_vecOrigin.Z - var.localPlayerOrigin.Z) < 50)
					headWalker(entityData.m_vecOrigin);
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
		while(true)
		{
			if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0 && (GetKeyState(VK_LBUTTON) & 0x100) == 0) //if caps ON and LBUTTON not pressed
			{
				int inCrossEntityId = Memes.RPM<int>(var.localPlayer + offset.crosshairId);
				Vector3_t velocity = Memes.RPM<Vector3_t>(var.localPlayer + offset.vecVelocity);
				if (inCrossEntityId > 0 && inCrossEntityId <= var.maxPlayers && (abs(velocity.X) < 110 && abs(velocity.Z) < 110))
				{
					const unsigned int inCrossEntityBase = Memes.RPM<unsigned int>(var.gameModule + offset.entityList + (inCrossEntityId - 1) * 0x10);
					if (Memes.RPM<int>(inCrossEntityBase + offset.iTeamNum) != Memes.RPM<int>(var.localPlayer + offset.iTeamNum))
					{
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
				}
			}
			Sleep(1);
		}
	}

	static void headWalker(Vector3_t closestPlayerOrigin)
	{
		keybd_event('A', 0, 0, NULL);
		Sleep(100);
		keybd_event('A', 0, KEYEVENTF_KEYUP, NULL);
		Sleep(100);
		keybd_event('D', 0, 0, NULL);
		Sleep(100);
		keybd_event('D', 0, KEYEVENTF_KEYUP, NULL);
	}

	static void prepareData() //pack coords data
	{
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["X"] = int((var.mapX - entityData.m_vecOrigin.X) / -var.mapScale);
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Z"] = int((var.mapZ - entityData.m_vecOrigin.Z) / var.mapScale);
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Y"] = int(entityData.m_vecOrigin.Y);
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Health"] = entityData.m_iHealth; //TODO: c4owner, defusing
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Dormant"] = entityData.m_bDormant;
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["ViewAngle"] = var.viewAngle;
		json[var.mapName][entityData.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["SteamId"] = playerInfo.guid;
		json["softer"] = var.myName;
		var.data = json.dump();
	}
};
