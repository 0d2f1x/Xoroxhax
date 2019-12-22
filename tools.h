#pragma once
#include "vars.h"

using namespace std;

class Tools
{
public:
	static void checkRun() // Only one window allowed
	{
		HW_PROFILE_INFO hwProfileInfo;
		GetCurrentHwProfile(&hwProfileInfo);
		std::string hwid = md5(hwProfileInfo.szHwProfileGuid);
		HANDLE Mutex = OpenMutex(MUTEX_ALL_ACCESS, 0, hwid.c_str());	 //MUTEX = md5 hwid
		if (!Mutex) Mutex = CreateMutex(0, 0, hwid.c_str()); else exit(0);
	}

	class Junk
	{
	public:
		static const unsigned char BLACK = 219;

		/* type of each pixel */
		typedef struct {
			unsigned char elem;
			unsigned char color;
		} SCREENELEMENT;

		static BOOL gotoxy(const WORD x, const WORD y) {
			COORD xy;
			xy.X = x;
			xy.Y = y;
			return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
		}

		/* print the screen to console */
		inline
			static void printScreen(int width, int height, SCREENELEMENT** screen) {
			gotoxy(1, 1); // top-left corner
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					SCREENELEMENT* cur = &screen[i][j];
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cur->color);
					printf("%c", cur->elem); // may be faster than cout
				}
			}
		}

		/* initialize the screen to element se */
		inline
			static void initScreen(int width, int height, SCREENELEMENT** screen, SCREENELEMENT se) {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					screen[i][j] = se;
				}
			}
		}

		/* draw a rectangle to (x, y), size (szx, szy) */
		inline
			static void putRectangle(int x, int y, int szx, int szy, int color, int height, SCREENELEMENT** screen) {
			for (int i = y; i < y + szy; i++) {
				for (int j = x; j < x + szx; j++) {
					screen[i][j].color = color;
				}
			}
		}

		static void screenSaver()
		{
			SetConsoleOutputCP(437);
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			int ret;
			/* get the width of the console */
			ret = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			int width = 80;
			if (ret) {
				width = csbi.dwSize.X;
			}
			int height = csbi.srWindow.Bottom - csbi.srWindow.Top;
			SCREENELEMENT** s = new SCREENELEMENT * [height];
			for (int i = 0; i < height; i++) {
				s[i] = new SCREENELEMENT[width];
			}
			SCREENELEMENT se;
			se.elem = BLACK;
			se.color = 0;
			initScreen(width, height, s, se);
			se.color = 15;
			srand(time(NULL));
			for (int i = 0; i < 10; i++) {
				int x = rand() % width;
				int y = rand() % height;
				int szx = rand() % (width - x);
				int szy = rand() % (height - y);
				/* color from 0 to 15 */
				int c = rand() % 15 + 1;
				/* put a random rectangle */
				putRectangle(x, y, szx, szy, c, height, s);
				printScreen(width, height, s);
				//Sleep(20); // optional interval
			}
			for (int i = 0; i < height; i++) {
				delete[] s[i];
			}
			delete[] s;
			//return 0; /* use Ctrl+C, never returns */
		}
	};

	class obfuscated
	{
	public:
		static char* obfProcess()
		{
			char process[16];
			process[9] = L'I' + 41;
			process[3] = L'r' - 3;
			process[14] = L'z' - 10;
			process[4] = 46 + 0;
			process[1] = 121 - 6;
			process[12] = 103;
			process[6] = L'S' + 37;
			process[2] = L'L' + 27;
			process[0] = L'h' - 5;
			process[11] = L'v' - 5;
			process[7] = L's' - 14;
			process[13] = 53 + 46;
			process[8] = 0;
			process[15] = 84 + 37;
			process[10] = 115;
			process[5] = L'R' + 19;
			return process;
		}

		static char* obfClient()
		{
			char client[38];
			client[35] = L'r' - 15;
			client[19] = 5 - 5;
			client[28] = L'v' - 2;
			client[11] = L'0' + 66;
			client[18] = L':' + 50;
			client[20] = L'r' - 12;
			client[1] = 108;
			client[15] = L'<' - 14;
			client[21] = L'n' + 2;
			client[13] = 116 - 7;
			client[17] = 49 + 59;
			client[4] = 101 + 9;
			client[33] = L'z' - 4;
			client[36] = L'6' + 58;
			client[22] = L'i' + 4;
			client[10] = 90 + 21;
			client[34] = 48 + 74;
			client[12] = 51 + 46;
			client[0] = 99;
			client[6] = L'w' - 24;
			client[3] = 49 + 52;
			client[14] = 97;
			client[7] = L';' + 53;
			client[30] = 121 - 0;
			client[26] = 100 + 6;
			client[23] = L'H' + 47;
			client[25] = 95 + 6;
			client[2] = L'1' + 56;
			client[37] = 100;
			client[27] = L'E' + 38;
			client[31] = 74 + 42;
			client[5] = L'5' + 63;
			client[8] = 109 - 12;
			client[16] = L'X' + 12;
			client[29] = L'g' + 8;
			client[32] = 109;
			client[9] = 98 + 12;
			client[24] = L'J' + 36;
			return client;
		}

		static char* obfEngine()
		{
			char engine[20];
			engine[2] = 103;
			engine[8] = L'w' - 11;
			engine[6] = 85 - 39;
			engine[0] = L'9' + 44;
			engine[4] = L'v' - 8;
			engine[5] = 49 + 52;
			engine[10] = 93 - 93;
			engine[9] = 76 + 32;
			engine[1] = L'0' + 62;
			engine[13] = 107;
			engine[3] = 59 + 46;
			engine[14] = 114;
			engine[16] = L's' - 4;
			engine[11] = 102;
			engine[19] = 97;
			engine[17] = 122;
			engine[18] = 107;
			engine[15] = 112;
			engine[12] = 95 + 17;
			engine[7] = L'<' + 40;
			return engine;
		}

		static char* de_cache()
		{
			char de_cache[16];
			de_cache[1] = 101;
			de_cache[5] = L'n' - 11;
			de_cache[13] = L'I' + 30;
			de_cache[10] = L'?' + 38;
			de_cache[11] = L'w' - 2;
			de_cache[0] = L'h' - 4;
			de_cache[14] = 103;
			de_cache[9] = 92 + 9;
			de_cache[8] = 0;
			de_cache[7] = 101;
			de_cache[6] = L'w' - 15;
			de_cache[2] = L'T' + 11;
			de_cache[4] = L'?' + 34;
			de_cache[15] = 52 + 46;
			de_cache[3] = L'h' - 5;
			de_cache[12] = L'm' - 1;
			return de_cache;
		}

		static char* de_cbble()
		{
			char de_cbble[16];
			de_cbble[1] = 102 - 1;
			de_cbble[6] = 56 + 52;
			de_cbble[14] = L'y' - 5;
			de_cbble[0] = L't' - 16;
			de_cbble[12] = 73 + 31;
			de_cbble[9] = 106;
			de_cbble[15] = L'X' + 33;
			de_cbble[2] = 56 + 39;
			de_cbble[5] = 54 + 44;
			de_cbble[11] = 113 - 5;
			de_cbble[4] = 88 + 10;
			de_cbble[7] = 49 + 52;
			de_cbble[8] = 53 - 53;
			de_cbble[10] = L'R' + 39;
			de_cbble[3] = 99;
			de_cbble[13] = L'x' - 8;
			return de_cbble;
		}

		static char* de_dust2()
		{
			char de_dust2[16];
			de_dust2[14] = L'b' + 8;
			de_dust2[2] = L'z' - 27;
			de_dust2[0] = L'f' - 2;
			de_dust2[6] = 120 - 4;
			de_dust2[4] = 117;
			de_dust2[15] = 69 + 36;
			de_dust2[9] = L'Y' + 17;
			de_dust2[1] = L'@' + 37;
			de_dust2[13] = 51 + 71;
			de_dust2[3] = 95 + 5;
			de_dust2[12] = 117;
			de_dust2[8] = 52 - 52;
			de_dust2[11] = 100 - 2;
			de_dust2[7] = L'f' - 52;
			de_dust2[5] = 54 + 61;
			de_dust2[10] = 122 - 5;
			return de_dust2;
		}

		static char* de_inferno()
		{
			char de_inferno[20];
			de_inferno[4] = 110;
			de_inferno[19] = 98;
			de_inferno[12] = L'>' + 60;
			de_inferno[0] = 89 + 11;
			de_inferno[7] = L':' + 56;
			de_inferno[6] = L':' + 43;
			de_inferno[10] = 0 + 0;
			de_inferno[17] = 122 - 6;
			de_inferno[15] = 72 + 29;
			de_inferno[16] = L'k' - 1;
			de_inferno[3] = 85 + 20;
			de_inferno[18] = L'x' - 4;
			de_inferno[9] = 63 + 48;
			de_inferno[2] = 81 + 14;
			de_inferno[5] = 80 + 22;
			de_inferno[14] = 119 - 2;
			de_inferno[8] = L'z' - 12;
			de_inferno[1] = L'k' - 6;
			de_inferno[11] = 120 - 3;
			de_inferno[13] = 99;
			return de_inferno;
		}

		static char* de_mirage()
		{
			char de_mirage[18];
			de_mirage[17] = L'x' - 3;
			de_mirage[2] = L'n' - 15;
			de_mirage[12] = L'4' + 46;
			de_mirage[13] = 115;
			de_mirage[6] = 98 - 1;
			de_mirage[1] = L'u' - 16;
			de_mirage[14] = L'v' - 6;
			de_mirage[0] = L'f' - 2;
			de_mirage[5] = L'r' - 0;
			de_mirage[9] = 0;
			de_mirage[15] = L'?' + 41;
			de_mirage[7] = 70 + 33;
			de_mirage[8] = 101;
			de_mirage[11] = 70 + 38;
			de_mirage[16] = L'H' + 49;
			de_mirage[10] = L'u' - 0;
			de_mirage[3] = L'G' + 38;
			de_mirage[4] = 105;
			return de_mirage;
		}

		static char* de_nuke()
		{
			char de_nuke[14];
			de_nuke[13] = L'w' - 17;
			de_nuke[10] = L'u' - 15;
			de_nuke[9] = 113;
			de_nuke[12] = L'o' - 11;
			de_nuke[0] = 78 + 22;
			de_nuke[8] = 121 - 4;
			de_nuke[1] = 59 + 42;
			de_nuke[5] = L'o' - 4;
			de_nuke[11] = L'l' - 9;
			de_nuke[7] = 112 - 112;
			de_nuke[6] = L'K' + 26;
			de_nuke[3] = 88 + 22;
			de_nuke[4] = 59 + 58;
			de_nuke[2] = 83 + 12;
			return de_nuke;
		}

		static char* de_overpass()
		{
			char de_overpass[22];
			de_overpass[9] = 115 + 0;
			de_overpass[16] = L'o' + 9;
			de_overpass[5] = 69 + 32;
			de_overpass[15] = L'z' - 1;
			de_overpass[19] = 100 + 13;
			de_overpass[13] = L'?' + 53;
			de_overpass[10] = 91 + 24;
			de_overpass[4] = 120 - 2;
			de_overpass[18] = L'z' - 0;
			de_overpass[12] = 79 + 21;
			de_overpass[8] = 97;
			de_overpass[7] = L's' - 3;
			de_overpass[14] = L'q' - 6;
			de_overpass[3] = L'v' - 7;
			de_overpass[6] = L'U' + 29;
			de_overpass[1] = 101;
			de_overpass[21] = L'5' + 61;
			de_overpass[20] = 49 + 54;
			de_overpass[17] = 79 + 29;
			de_overpass[11] = 0;
			de_overpass[0] = 114 - 14;
			de_overpass[2] = L'Z' + 5;
			return de_overpass;
		}

		static char* de_train()
		{
			char de_train[16];
			de_train[4] = 105 + 9;
			de_train[12] = 111;
			de_train[11] = 74 + 24;
			de_train[8] = L'"' - 34;
			de_train[15] = 119 - 3;
			de_train[3] = 108 + 8;
			de_train[2] = 95;
			de_train[1] = L'j' - 5;
			de_train[14] = L'y' - 1;
			de_train[13] = 106 + 8;
			de_train[0] = L'G' + 29;
			de_train[6] = L'm' - 4;
			de_train[9] = L'v' - 6;
			de_train[5] = L'O' + 18;
			de_train[10] = L'i' - 5;
			de_train[7] = 110;
			return de_train;
		}

		static char* cs_office()
		{
			char cs_office[18];
			cs_office[15] = 87 + 28;
			cs_office[14] = 74 + 33;
			cs_office[3] = L'u' - 6;
			cs_office[9] = L'_' - 95;
			cs_office[13] = L'i' - 1;
			cs_office[10] = L'o' - 3;
			cs_office[16] = L'g' - 2;
			cs_office[12] = 86 + 24;
			cs_office[6] = 110 - 5;
			cs_office[4] = 102;
			cs_office[8] = 101;
			cs_office[0] = L'9' + 42;
			cs_office[2] = 95;
			cs_office[5] = L'k' - 5;
			cs_office[1] = L'`' + 19;
			cs_office[17] = L'z' - 3;
			cs_office[11] = 103 - 3;
			cs_office[7] = L'=' + 38;
			return cs_office;
		}

		static char* dz_blacksite()
		{
			char dz_blacksite[24];
			dz_blacksite[0] = L'\\' + 8;
			dz_blacksite[19] = L's' - 10;
			dz_blacksite[23] = 56 + 64;
			dz_blacksite[21] = 112;
			dz_blacksite[6] = 66 + 33;
			dz_blacksite[17] = 92 + 12;
			dz_blacksite[13] = L'k' + 12;
			dz_blacksite[10] = 116;
			dz_blacksite[5] = L'B' + 31;
			dz_blacksite[8] = 115;
			dz_blacksite[3] = 49 + 49;
			dz_blacksite[7] = 97 + 10;
			dz_blacksite[20] = L'z' - 0;
			dz_blacksite[22] = L'w' - 2;
			dz_blacksite[15] = L'7' + 43;
			dz_blacksite[16] = 117 - 15;
			dz_blacksite[12] = 0;
			dz_blacksite[14] = L'w' - 9;
			dz_blacksite[18] = 65 + 32;
			dz_blacksite[1] = 122 - 0;
			dz_blacksite[4] = L'E' + 39;
			dz_blacksite[11] = L'v' - 17;
			dz_blacksite[2] = L'2' + 45;
			dz_blacksite[9] = 105;
			return dz_blacksite;
		}

		static char* dz_sirocco()
		{
			char dz_sirocco[20];
			dz_sirocco[1] = L'?' + 59;
			dz_sirocco[7] = L'E' + 30;
			dz_sirocco[14] = 100 + 7;
			dz_sirocco[0] = L'i' - 5;
			dz_sirocco[9] = 65 + 46;
			dz_sirocco[4] = L'\\' + 13;
			dz_sirocco[18] = 122;
			dz_sirocco[2] = 100 - 5;
			dz_sirocco[5] = L't' - 2;
			dz_sirocco[11] = L'o' - 11;
			dz_sirocco[15] = L'w' - 0;
			dz_sirocco[16] = L'x' - 15;
			dz_sirocco[13] = L'>' + 45;
			dz_sirocco[12] = 116 - 17;
			dz_sirocco[6] = 54 + 57;
			dz_sirocco[19] = 117 - 6;
			dz_sirocco[3] = 79 + 36;
			dz_sirocco[8] = 99;
			dz_sirocco[17] = 111;
			dz_sirocco[10] = L'`' - 96;
			return dz_sirocco;
		}
	};
};