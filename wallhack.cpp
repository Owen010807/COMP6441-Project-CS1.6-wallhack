#include <iostream>
#include "transparent.hpp"

int main()
{
    //Get the module base address
    DWORD proc_id = get_procid("Counter-Strike");
    printf("ProcID:%d\n", proc_id);

    g_hprocess =     OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_id);
    char botName[24];
    DWORD read_size = 0;
    g_base_address = get_module_address(proc_id, "cstrike.exe");
    printf("Module base address:%x\n", g_base_address);
    init_game_data();
    print_player_info();
    init_matrix();
    //Get the window handle
    g_game_hwnd = FindWindowA(nullptr, "Counter-Strike");
    HWND transparent = creat_transparent_window(g_game_hwnd);
    init_d3d(transparent);
    message_handle(g_game_hwnd, transparent);
}
