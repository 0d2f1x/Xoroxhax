#pragma once
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <ctime>
#include <string>
#include "manager.h"
#include "md5.h"
#include "tools.h"
#include "json.hpp"

manager Memes;

struct offsets
{
	unsigned int crosshairId = 0xB3D4;
	unsigned int entityList = 0x4D3C68C;
	unsigned int spotted = 0x93D;
	unsigned int localPlayer = 0xD28B1C;
	unsigned int flashDuration = 0xA410;
	unsigned int vecOrigin = 0x138;
	unsigned int vecVelocity = 0x114;
	unsigned int iTeamNum = 0xF4;
	unsigned int clientState = 0x588D9C;
	unsigned int clientStateMap = 0x28C;
	unsigned int isC4owner = 0x3A4290;
	unsigned int health = 0x100;
	unsigned int dormant = 0xED;
	unsigned int accountID = 0x2FC8;
	unsigned int radarBase = 0x51713EC; //for getting names
	unsigned int playerInfo = 0x52B8;
} offset;

struct variables
{
	//string streamUrl = "ws://localhost";
	string streamUrl = "wss://xorox.herokuapp.com";
	int streamDelay = 0;
	static const int maxPlayers = 64;
	int flashDuration = 0;
	int bufer = 0;
	string data = "";
	const int procID = Memes.getProcess(Tools::obfuscated::obfProcess());
	unsigned int gameModule = Memes.getModule(procID, Tools::obfuscated::obfClient());
	unsigned int gameEngine = Memes.getModule(procID, Tools::obfuscated::obfEngine());
	unsigned int clientState;
	unsigned int localPlayer;
	unsigned int UserInfoTable;
	unsigned int entity;
	int teamId;
	float allXYZ[maxPlayers][2];
	float currentXYZ[3];
	int mapX;
	int mapY;
	float mapScale; //5.86 (1.333 coefficient for primal map_scale)
	string mapName;
	string myName;
	string name = "player"; //For json sending
	string steamId;
	int health;
	bool dormant;
	unsigned int C4;
} var;

struct MapInfo
{
	MapInfo(string mapName, int offsetX, int offsetY, float scale)
	{
		this->mapName = mapName;
		this->offsetX = offsetX;
		this->offsetY = offsetY;
		this->scale = scale;//* 1.333; //768x768
	}

	string mapName;
	int offsetX;
	int offsetY;
	float scale;
};

map<int, MapInfo> maps =
{
	{ 0,  MapInfo(Tools::obfuscated::de_cache(), -2000, 3250, 5.5f)     },
	{ 1,  MapInfo(Tools::obfuscated::de_cbble(), -3840, 3072, 6.0f)     },
	{ 2,  MapInfo(Tools::obfuscated::de_dust2(), -2476, 3239, 4.4f)     },
	{ 3,  MapInfo(Tools::obfuscated::de_inferno(), -2087, 3870, 4.9f)   },
	{ 4,  MapInfo(Tools::obfuscated::de_mirage(), -3230, 1713, 5.0f)    },
	{ 5,  MapInfo(Tools::obfuscated::de_nuke(), -3453, 2887, 7.0f)      },
	{ 6,  MapInfo(Tools::obfuscated::de_overpass(), -4831, 1781, 5.2f)  },
	{ 7,  MapInfo(Tools::obfuscated::de_train(), -2477, 2392, 4.7f)     },
	{ 8,  MapInfo(Tools::obfuscated::cs_office(), -1838, 1858, 4.1f)    },
	{ 9,  MapInfo(Tools::obfuscated::dz_blacksite(), -8604, 8804, 17.0f)},
	{ 10, MapInfo(Tools::obfuscated::dz_sirocco(), -8604, 8804, 17.0f)  },
};

//static void GetPlayerName()
//{
//	unsigned int radarAddress = Memes.readMem<unsigned int>(offset.clientState + offset.radarBase);
//	radarAddress = Memes.readMem<unsigned int>(radarAddress + 0x3C);
//	cout << Memes.readMem<char>(radarAddress + (0x20 * (1 * 0x10)) + 0x3C);
//}


static void init()
{
	var.localPlayer = Memes.readMem<unsigned int>(var.gameModule + offset.localPlayer);
	var.clientState = Memes.readMem<unsigned int>(var.gameEngine + offset.clientState);
	var.mapName = Memes.readString(var.clientState + offset.clientStateMap);
	var.UserInfoTable = Memes.readMem<unsigned int>(var.clientState + offset.playerInfo);


	//struct radar_player_t {
	//	vector<double> origin; //0x0000
	//	vector<double> viewangles; //0x000C
	//	char pad_0018[56]; //0x0018
	//	uint32_t health; //0x0050
	//	char name[32]; //0x0054
	//	char pad_00D4[117]; //0x00D4
	//	uint8_t visible; //0x00E9
	//} radar; //Size: 0x0B32


	//auto radar_base = Memes.readMem<DWORD>(var.gameModule + offset.radarBase);
	//radar_base = Memes.readMem<DWORD>(radar_base + 0x78);
	//auto radar_player = Memes.readMem<radar_player_t>(radar_base + (0x174 * (10 + 1)) - 0x3C);

	//cout << radar.health;

	for (auto& x : maps) {
		if (x.second.mapName == var.mapName)
		{
			var.mapX = x.second.offsetX;
			var.mapY = x.second.offsetY;
			var.mapScale = x.second.scale;
		}
	}
}