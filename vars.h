#pragma once
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <ctime>
#include <string>
#include "manager.h"
#include "md5.h"
#include "tools.h"
#include "json.hpp"
#define _USE_MATH_DEFINES
#define RAD 180 / M_PI
#include <math.h>

manager Memes;

//static BYTE scan_code(DWORD pKey)
//{
//	const DWORD result = MapVirtualKey(pKey, MAPVK_VK_TO_VSC);
//	return static_cast<BYTE>(result);
//}
//
//static void press_key(DWORD pKey)
//{
//	keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), 0, 0);
//}
//
//static void release_key(DWORD pKey)
//{
//	keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), KEYEVENTF_KEYUP, 0);
//}
//#define PRESS(x) press_key(x)
//#define RELEASE(x) release_key(x)

typedef struct Vector3_s
{
	float X;
	float Y;
	float Z;
} vector3;
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
struct entityData_t
{
	char pad1[0xED];
	bool m_bDormant;
	char pad2[0x6];
	int  m_iTeamNum;
	char pad3[0x8];
	int  m_iHealth;
	int  m_fFlags;
	vector3 m_vecViewOffset;
	char pad4[0x24];
	vector3 m_vecOrigin;
	char pad5[0x11B];
	bool m_bLifeState;
	char pad6[0x6DD];
	bool m_bSpotted;
	char pad7[0x207E];
	bool m_bShouldGlow;
	char pad8[0x52F];
	DWORD_PTR m_hActiveWeapon;
	char pad9[0x9C];
	int m_iItemDefinitionIndex;
	char pad10[0x8C];
	vector3 m_aimPunchAngle;
	char pad11[0x1A4];
	float m_flNextPrimaryAttack;
	char pad12[0x244];
	int m_nTickBase;
	char pad13[0x480];
	bool m_bIsDefusing;
	char pad14[0x6A63];
	float m_flFlashDuration;
	char pad15[0x6E8];
	int m_iClass;
	char pad16[0x5C];
	int m_iCrosshairId;
};
static entityData_t entityData;
static player_info_t playerInfo;

struct offsets
{
	unsigned int dwForceForward = 0x316DDAC;
	unsigned int m_vecViewOffset = 0x108;
	unsigned int ViewAngles = 0x4D88;
	unsigned int angEyeAnglesY = 0xB370;
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
	string data = "";
	const int procID = Memes.getProcess(Tools::obfuscated::obfProcess());
	unsigned int gameModule = Memes.getModule(procID, Tools::obfuscated::obfClient());
	unsigned int gameEngine = Memes.getModule(procID, Tools::obfuscated::obfEngine());
	unsigned int clientState;
	unsigned int localPlayer;
	unsigned int UserInfoTable;
	int mapX;
	int mapY;
	float mapScale; //5.86 (1.333 coefficient for primal map_scale)
	string mapName;
	string myName;
	float viewAngle;
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
	{ 11, MapInfo(Tools::obfuscated::dz_junglety(), -8504, 8741, 17.0f) }
};

static void init()
{
	var.localPlayer = Memes.RPM<unsigned int>(var.gameModule + offset.localPlayer);
	var.clientState = Memes.RPM<unsigned int>(var.gameEngine + offset.clientState);
	var.mapName = Memes.RS(var.clientState + offset.clientStateMap);
	var.UserInfoTable = Memes.RPM<unsigned int>(var.clientState + offset.playerInfo);

	for (auto& x : maps) {
		if (x.second.mapName == var.mapName)
		{
			var.mapX = x.second.offsetX;
			var.mapY = x.second.offsetY;
			var.mapScale = x.second.scale;
		}
	}
}

float Speed(vector3 velocity)
{
	return sqrt(velocity.X * velocity.X + velocity.Y * velocity.Y + velocity.Z * velocity.Z);
}

static float calcAngle(vector3 src, vector3 dst)
{
	vector3 resultVector = { (src.X - dst.X), (src.Y - dst.Y), (src.Z - dst.Z) };
	float yaw = atan(resultVector.Y / resultVector.X) * RAD; // 1 RAD = 57.2958 DEGREE
	if (resultVector.X >= 0.0f)
		yaw += 180.0f;
	return yaw;
}