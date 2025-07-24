#include "winapitools.hpp"

struct GamePlayer
{
	char name[24];
	float x;
	float y;
	float z;
	float blood;
	DWORD camp;
};
DWORD max_playerCount = 64;
DWORD g_real_playerCount = 1;
struct GamePlayer g_game_player[64];
HWND g_game_hwnd;
DWORD g_base_address;
HANDLE g_hprocess;
// Global Matrix Information 
float g_matrix[4][4];
int	g_game_x, g_game_y, g_game_w, g_game_h;

/*
	Initialize player name
*/
void init_name() {
    for (int i = 0; i < max_playerCount; i++) {
        DWORD nameAddress = i * 0x24C + g_base_address + 0x1B5A6C8;
        char* name = readCharArray(g_hprocess, nameAddress, 24);
        if (name[0] == '\0') {
            g_real_playerCount = i;
            break;
        }
        strncpy(g_game_player[i].name, name, 24);
    }
}

/*
    Initialize coordinates xyz
*/
void init_xyz() {
    DWORD playerBaseAddress = g_base_address + 0x11069BC;
    DWORD playerBaseValue = readDWORD(g_hprocess, playerBaseAddress);

    for (DWORD i = 0; i < g_real_playerCount; i++) {
        DWORD zAddress = playerBaseValue + (0x324 * i) + 0x90;
        float zValue = readFloat(g_hprocess, zAddress);
        g_game_player[i].z = zValue;
        DWORD yAddress = playerBaseValue + (0x324 * i) + 0x8c;
        float yValue = readFloat(g_hprocess, yAddress);
        g_game_player[i].y = yValue;
        DWORD xAddress = playerBaseValue + (0x324 * i) + 0x88;
        float xValue = readFloat(g_hprocess, xAddress);
        g_game_player[i].x = xValue;
    }
}

/*
    Initialize player blood
*/
void init_blood() {
    DWORD playerBaseAddress = g_base_address + 0x11069BC;
    DWORD playerBaseValue = readDWORD(g_hprocess, playerBaseAddress);
    for (DWORD i = 0; i < g_real_playerCount; i++) {
        DWORD playerBloodAddress = playerBaseValue + (0x324 * i) + 0x1E0;
        float bloodValue = readFloat(g_hprocess, playerBloodAddress);
        g_game_player[i].blood = bloodValue;
    }
}

/*
    Initialize team
*/
void init_camp() {
    DWORD playerBaseAddress = g_base_address + 0x11069BC;
    DWORD playerBaseValue = readDWORD(g_hprocess, playerBaseAddress);
    for (DWORD i = 0; i < g_real_playerCount; i++) {
        DWORD player7cAddress = playerBaseValue + (0x324 * i) + 0x7C;
        DWORD base7cValue = readDWORD(g_hprocess, player7cAddress);
        DWORD campAddress = base7cValue + 0x1C8;
        DWORD campValue = readDWORD(g_hprocess, campAddress);
        g_game_player[i].camp = campValue;
    }
}

/*
    Initialize matrix
*/
void init_matrix() {
    //cstrike.exe + 1820100
    DWORD matrixAddress = g_base_address + 0x1820100;
    DWORD read_size = 0;
    ReadProcessMemory(g_hprocess, (LPCVOID)matrixAddress, g_matrix, sizeof(float) * 4 * 4, &read_size);

    //for (int i = 0; i < 4;i++) {
    //    for (int j = 0; j < 4;j++) {
    //        printf("%f\t", g_matrix[i][j]);
    //    }
    //    printf("\n");
    //}
}

void init_game_data() {
    init_name();
    init_xyz();
    init_blood();
    init_camp();
    init_matrix();
}

/*
    Print player information
*/
void print_player_info() {
    for (DWORD i = 0; i < g_real_playerCount; i++) {
        printf("Player name:%s\n", g_game_player[i].name);
        printf("Player coordinates x:%f\n", g_game_player[i].x);
        printf("Player coordinates y:%f\n", g_game_player[i].y);
        printf("Player coordinates z:%f\n", g_game_player[i].z);
        printf("Player blood:%f\n", g_game_player[i].blood);
        printf("Player team:%d\n", g_game_player[i].camp);
    }
}
