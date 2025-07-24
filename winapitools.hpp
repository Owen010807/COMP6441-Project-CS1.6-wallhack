#include <iostream>
#include <windows.h>
#include <Tlhelp32.h>
#pragma warning(disable : 4996)

/*
    Get the process ID through the window
*/
DWORD get_procid(const char* process_name) {
    //Get the window handle
    HWND game_hwnd = FindWindowA(nullptr, process_name);
    DWORD proc_id = 0;
    //Get the process id through the window handle
    GetWindowThreadProcessId(game_hwnd, &proc_id);
    return proc_id;
}

/*
    Get the module base address based on the process id and module name
*/
DWORD get_module_address(DWORD process_id, const char* module_name) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
    MODULEENTRY32 module_info;
    ZeroMemory(&module_info, sizeof(module_info));
    module_info.dwSize = sizeof(module_info);

    char module_name_up[1024];
    ZeroMemory(module_name_up, 1024);
    strncpy(module_name_up, module_name, strlen(module_name));
    _strupr(module_name_up);
    BOOL state = Module32First(snap, &module_info);
    char szModuleUP[1024];
    while (state)
    {
        /*printf("module name:%s\n", module_info.szModule);
        printf("module address:%x\n", module_info.modBaseAddr);*/

        // wcstombs(szModuleUP, module_info.szModule, strlen(module_name));
        strncpy(szModuleUP, module_info.szModule, strlen(module_info.szModule));
        _strupr(szModuleUP);
        if (strncmp(module_name_up, szModuleUP, strlen(module_name)) == 0) {
            CloseHandle(snap);
            return (DWORD)module_info.modBaseAddr;
        }
        state = Module32Next(snap, &module_info);

    }
    CloseHandle(snap);
    return 0;
}

/*
    Read DWORD type 4byte
*/
DWORD readDWORD(HANDLE process_handle, DWORD address) {
    DWORD value = 0;
    DWORD read_size = 0;
    ReadProcessMemory(process_handle, (LPCVOID)address, &value, 4, &read_size);
    return value;
}

/*
    Read float type 4byte
*/
float readFloat(HANDLE process_handle, DWORD address) {
    float value = 0;
    DWORD read_size = 0;
    ReadProcessMemory(process_handle, (LPCVOID)address, &value, 4, &read_size);
    return value;
}

/*
    Read char* type 4byte
*/
char* readCharArray(HANDLE process_handle, DWORD address, DWORD len) {
    char bootName[24];
    DWORD read_size = 0;
    ReadProcessMemory(process_handle, (LPCVOID)address, &bootName, len, &read_size);
    return bootName;
}
