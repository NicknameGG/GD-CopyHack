#include <iostream>

#include <windows.h>
#include <TlHelp32.h>
#include<conio.h>
#include <string>
#include <string_view>


const inline auto get_proc_pid(const std::string_view proc_name)
{
    DWORD pid;

    const auto hwnd = FindWindowA(nullptr, proc_name.data());

    return GetWindowThreadProcessId(hwnd, &pid) ? pid : 0;
}

const uintptr_t get_module_base(const uintptr_t pid, const std::wstring_view module_name)
{
    MODULEENTRY32W me32 = {};
    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);


    me32.dwSize = sizeof(MODULEENTRY32W); // decltype(me32)

    if (Module32First(snapshot, &me32))
    {
        do
        {
            if (!module_name.compare(me32.szModule))
            {
                CloseHandle(snapshot);
                return reinterpret_cast<uintptr_t>(me32.modBaseAddr);
            }
        } while (Module32Next(snapshot, &me32));
    }

    CloseHandle(snapshot);
    return 0;
}

static constexpr auto p_name = "Geometry Dash"; // probably

int main()
{
    system("title Copy Hack for Geometry Dash - NicknameGG");

     
    std::cout << "Waiting for Geometry Dash...\n";
    while (!FindWindowA(NULL, "Geometry Dash")) {
        Sleep(250);
    }

    DWORD oldp = 0;
    byte change[] = {
    0x90,0x90,0x90,0x90,0x90
    };
    byte original[] = {
        0xE9,0xDE,0x00,0x00,0x00
    };
    bool hack = false;
    const auto pid = get_proc_pid(p_name);
    DWORD base = get_module_base(pid, L"GeometryDash.exe");
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    VirtualProtectEx(process, (LPVOID)(base + 0x1763AB), sizeof(change), PAGE_EXECUTE_READWRITE, &oldp);


    if (process == INVALID_HANDLE_VALUE) {
        std::cout << "Couldn't attach to Geometry Dash...\n";
            system("pause");
    }

    system("cls");




    std::cout << "Hack loaded... \nPress space to toggle hack";
 while (true) {
     Sleep(10);

    if (GetKeyState(VK_SPACE) & 0x8000 && hack == false) {
        system("cls");
        system("color a");
        WriteProcessMemory(process, (LPVOID)(base + 0x1763AB), change, sizeof(change), NULL);
        std::cout << "Copy hack enabled!";
        hack = true;
        Sleep(40);
    }
    else if (GetKeyState(VK_SPACE) & 0x8000 && hack == true) {
        system("cls");
        system("color c");
        WriteProcessMemory(process, (LPVOID)(base + 0x1763AB), original, sizeof(original), NULL);
        std::cout << "Copy hack disabled!";
        hack = false;
        Sleep(40);
    }
}



    return 0;
}
//offset 0x1763AB


//00C863AB | E9 DE000000              | jmp geometrydash.C8648E                 |