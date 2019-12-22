#pragma once
#include "vars.h"
#include <chrono> 
#include <algorithm> 

nlohmann::json json;

typedef struct player_info_s
{
	char pad[0x10];
	char name[128];
	int	 userID;
	char guid[33];
	unsigned long friendsID;
	char friendsName[128];
	bool fakeplayer;
	bool ishltv;
	unsigned long customFiles[4];
	unsigned char filesDownloaded;
	byte buffer[200];
} player_info_t;


class Entity {
public:
	int m_iID;
	int m_iBase;
	int m_iTeam;
};

class Vector3
{
public:
	vector<float> value; //only for velocity
};

class Hex
{
public:
	static string rad()
	{
		for (short i = 0; i < var.maxPlayers; i++)
		{
			var.entity = Memes.readMem<unsigned int>(var.gameModule + offset.entityList + i * 0x10);
			if (var.entity != NULL) {
				Memes.writeMem<bool>(var.entity + offset.spotted, true);
				var.teamId = Memes.readMem<int>(var.entity + offset.iTeamNum);
				var.health = Memes.readMem<int>(var.entity + offset.health);
				var.dormant = Memes.readMem<bool>(var.entity + offset.dormant);
				DWORD x = Memes.readMem<DWORD>(Memes.readMem<DWORD>(var.UserInfoTable + 0x40) + 0xC);
				player_info_t p = Memes.readMem<player_info_t>(Memes.readMem<DWORD>(x + 0x28 + 0x34 * i));
				var.name = p.name;
				var.steamId = p.guid;
				if (i == 0) var.myName = p.name;
				for (short j = 0, cordOffset = 0; j < 3; j++, cordOffset += 4)
				{
					var.allXYZ[i][j] = Memes.readMem<float>(var.entity + offset.vecOrigin + cordOffset);
					prepareData(var.allXYZ, i, j);
				}
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

	static void trigger()
	{
		Vector3 localVelocity;
		Entity InCrossEntity;
		bool canShoot;
		while(true)
		{
			//auto start = chrono::high_resolution_clock::now();
			//cout << localVelocity.value[0] << " " << localVelocity.value[1] << "\n";
			if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
			{
				InCrossEntity.m_iID = Memes.readMem<int>(var.localPlayer + offset.crosshairId);
				for (short j = 0, cordOffset = 0; j < 3; j++, cordOffset += 4)
					localVelocity.value.push_back(abs(Memes.readMem<float>(var.localPlayer + offset.vecVelocity + cordOffset)));
				if (localVelocity.value[0] < 110 && localVelocity.value[1] < 110)
					canShoot = true;
				else
					canShoot = false;
				localVelocity.value.clear();
				if (InCrossEntity.m_iID > 0 && InCrossEntity.m_iID <= var.maxPlayers && canShoot)
				{
					InCrossEntity.m_iBase = Memes.readMem<unsigned int>(var.gameModule + offset.entityList + (InCrossEntity.m_iID - 1) * 0x10);
					InCrossEntity.m_iTeam = Memes.readMem<int>(InCrossEntity.m_iBase + offset.iTeamNum);
					int localTeam = Memes.readMem<int>(var.localPlayer + offset.iTeamNum);
					if (InCrossEntity.m_iTeam != localTeam)
					{
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
				}
			}
			//auto stop = chrono::high_resolution_clock::now();
			//auto duration = stop - start;
			//cout << duration.count();
			Sleep(1);
		}
	}

	static void prepareData(float coords[var.maxPlayers][2], short i, short j) //pack coords data
	{
		if (var.bufer > 2) var.bufer = 0;
		var.currentXYZ[var.bufer] = coords[i][j];
		if (var.bufer == 2) {
			json[var.mapName][var.teamId-2 == 1 ? "Ct" : "T"][var.name]["X"] = int((var.mapX - var.currentXYZ[0]) / -var.mapScale); //map scale, in future upgrade to auto pars map
			json[var.mapName][var.teamId-2 == 1 ? "Ct" : "T"][var.name]["Y"] = int((var.mapY - var.currentXYZ[1]) / var.mapScale);
			json[var.mapName][var.teamId-2 == 1 ? "Ct" : "T"][var.name]["Z"] = int(var.currentXYZ[2]);
			json[var.mapName][var.teamId-2 == 1 ? "Ct" : "T"][var.name]["Health"] = var.health; //TODO: c4owner, defusing, planting 
			json[var.mapName][var.teamId-2 == 1 ? "Ct" : "T"][var.name]["Dormant"] = var.dormant;
			json[var.mapName][var.teamId-2 == 1 ? "Ct" : "T"][var.name]["SteamId"] = var.steamId;

			json["batya"] = var.myName;
			//json[teamId == 1 ? "Enemy" : "Team"][var.name + to_string(i)]["C4"] = var.C4;
			var.data = json.dump();
		}
		var.bufer++;
	}
};
