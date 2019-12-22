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
			var.entity = Memes.readMem<unsigned int>(var.gameModule + offset.entityList + i * 0x10);
			if (var.entity != NULL) {
				entityPlayer = Memes.readMem<entityData_t>(var.entity);
				if (!entityPlayer.m_bSpotted) Memes.writeMem<bool>(var.entity + offset.spotted, true);
				if (i == 0) var.myName = playerInfo.name;
				DWORD x = Memes.readMem<DWORD>(Memes.readMem<DWORD>(var.UserInfoTable + 0x40) + 0xC);
				playerInfo = Memes.readMem<player_info_t>(Memes.readMem<DWORD>(x + 0x28 + 0x34 * i));
				prepareData();
			}
		}
		//system("CLS");
		//cout << json.dump(4) << "\n" << var.mapScale << " " << var.mapX << " " << var.mapY;
		//json.clear();
		return var.data;
	}

	static void flash()
	{
		var.flashDuration = Memes.readMem<int>(var.localPlayer + offset.flashDuration);
		if (var.flashDuration > 0) Memes.writeMem<int>(var.localPlayer + offset.flashDuration, 0);
	}

	static void trigger() //TODO:FIX TEAM COLOR - OUTDATED JSON (JUST CLEAN OR SOMETHING ELSE)
	{
		bool canShoot;
		while(true)
		{
			if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
			{
				int inCrossEntityId = Memes.readMem<int>(var.localPlayer + offset.crosshairId);
				Vector3_t velocity = Memes.readMem<Vector3_t>(var.localPlayer + offset.vecVelocity);
				if (abs(velocity.X) < 110 && abs(velocity.Z) < 110)
					canShoot = true;
				else
					canShoot = false;
				if (inCrossEntityId > 0 && inCrossEntityId <= var.maxPlayers && canShoot)
				{
					unsigned int inCrossEntityBase = Memes.readMem<unsigned int>(var.gameModule + offset.entityList + (inCrossEntityId - 1) * 0x10);
					int inCrossEntityTeam = Memes.readMem<int>(inCrossEntityBase + offset.iTeamNum);
					if (inCrossEntityTeam != Memes.readMem<int>(var.localPlayer + offset.iTeamNum))
					{
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
				}
			}
			Sleep(1);
		}
	}

	static void prepareData() //pack coords data
	{
		json[var.mapName][entityPlayer.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["X"] = int((var.mapX - entityPlayer.m_vecOrigin.X) / -var.mapScale);
		json[var.mapName][entityPlayer.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Z"] = int((var.mapZ - entityPlayer.m_vecOrigin.Z) / var.mapScale);
		json[var.mapName][entityPlayer.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Y"] = int(entityPlayer.m_vecOrigin.Y);
		json[var.mapName][entityPlayer.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Health"] = entityPlayer.m_iHealth; //TODO: c4owner, defusing
		json[var.mapName][entityPlayer.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["Dormant"] = entityPlayer.m_bDormant;
		json[var.mapName][entityPlayer.m_iTeamNum == 3 ? "Ct" : "T"][playerInfo.name]["SteamId"] = playerInfo.guid;
		json["softer"] = var.myName;
		var.data = json.dump();
	}
};
