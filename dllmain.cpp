#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <cstring>
#include <iostream>
#include <windows.h>
#include <cstdio>


class MemoryAddress {
private:
    DWORD OldProtect;
    void* Addr;
    char isfree;
public:
    MemoryAddress(void* Address) {
        Addr = Address;
        VirtualProtect(Addr, sizeof(char), PAGE_EXECUTE_READWRITE, &OldProtect);
    }
    ~MemoryAddress() {
        if (!isfree)
            VirtualProtect(Addr, sizeof(char), OldProtect, &OldProtect);
    }
    void Free() {
        VirtualProtect(Addr, sizeof(char), OldProtect, &OldProtect);
        isfree = 1;
    }
};

#define CLASSNAME "Cheat"
#define TITLE "PVZ Trainer"

void* events[128];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_COMMAND:
        void (*eventvoid)();
        eventvoid = (void(*)())events[LOWORD(wParam)];
        eventvoid();
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND hwnd;

class CheatWindow {
private:
    WNDCLASS wc = {};
public:

    CheatWindow() {
       
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = CLASSNAME;
        RegisterClass(&wc);

        hwnd = CreateWindowEx(
            0, CLASSNAME, TITLE,
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
            NULL, NULL, wc.hInstance, NULL
        );
        



        if (hwnd == NULL) {
            return;
        }
    }

    void Button(int id,const char* title,int x,int y,int sizex,int sizey) {
        HWND hButton = CreateWindow(
            "BUTTON", title,
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            x, y, sizex, sizey,
            hwnd, (HMENU)id, wc.hInstance, NULL
        );
        NONCLIENTMETRICS ncm;
        ncm.cbSize = sizeof(NONCLIENTMETRICS);
        SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
        HFONT hFont = CreateFontIndirect(&ncm.lfMessageFont);
        SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
    }

    void WindowUpdate() {
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
    }

    void HandleWindow() {
        
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            WindowUpdate();
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    void NewEvent(int id,void* event) {
        events[id] = event;
    }

};

#define NOP(x) \
    MemoryAddress mem((void*)x); \
    *(BYTE*)x = 0x90; \
    mem.Free();
    
#define NOPN(x) \
    MemoryAddress mem_##x((void*)x); \
    *(BYTE*)x = 0x90; \
    mem_##x.Free();

#define INFO(msg) MessageBoxA(NULL, msg, TITLE, MB_OK | MB_ICONINFORMATION); 

#define SUN_SUB_BASE 0x0041F636 // 6 Bytes length
BYTE NoSubstractBackup[6];
BYTE NoSubstractEnabled;

void NoSubstract() {
    if(!NoSubstractEnabled) {
        for(int i =0; i < 6;i++) {
            int offset_base = SUN_SUB_BASE + i;
            NoSubstractBackup[i] = *(BYTE*)offset_base;
            NOP(offset_base);
        }
        NoSubstractEnabled = 1;
        INFO("NoSubstract enabled")
    }
    else {
        for(int i =0; i < 6;i++) {
            int offset_base = SUN_SUB_BASE + i;
            MemoryAddress mem((void*)offset_base); 
            *(BYTE*)offset_base = NoSubstractBackup[i];
            mem.Free();
        }
        NoSubstractEnabled = 0;
        INFO("NoSubstract disabled")
    }
}

#define SUN_SUB_INST_BASE 0x0041F634  // 2 Bytes length
BYTE SubToAddBackup[2];
BYTE SubToAddEnabled;

void SubToAdd() {
    if(!SubToAddEnabled) {
        SubToAddBackup[0] = *(BYTE*)(SUN_SUB_INST_BASE);
        SubToAddBackup[1] = *(BYTE*)(SUN_SUB_INST_BASE + 1);
        MemoryAddress mem((void*)SUN_SUB_INST_BASE); 
        MemoryAddress mem2((void*)(SUN_SUB_INST_BASE + 1)); 
        *(BYTE*)(SUN_SUB_INST_BASE) = 0x01;
        *(BYTE*)(SUN_SUB_INST_BASE + 1) = 0xDE;   
        mem.Free();
        mem2.Free();
        SubToAddEnabled = 1;
        INFO("SubToAdd enabled")
    }
    else {
        MemoryAddress mem((void*)SUN_SUB_INST_BASE); 
        MemoryAddress mem2((void*)(SUN_SUB_INST_BASE + 1)); 
        *(BYTE*)(SUN_SUB_INST_BASE) = SubToAddBackup[0];
        *(BYTE*)(SUN_SUB_INST_BASE + 1) = SubToAddBackup[1];
        mem.Free();
        mem2.Free();
        SubToAddEnabled = 0;
        INFO("SubToAdd Disabled")  
    }
}

#define NO_COOLDOWN_BASE 0x004958C5  // 2 Bytes length
BYTE NoCooldownBackup[2];
BYTE NoCooldownEnabled;

void NoCooldown() {
    if(!NoCooldownEnabled) {
        NoCooldownEnabled = 1;
        NoCooldownBackup[0] = *(BYTE*)NO_COOLDOWN_BASE;
        NoCooldownBackup[1] = *(BYTE*)(NO_COOLDOWN_BASE + 1);
        for(int i=0;i < 2;i++) {
            int offset_base = NO_COOLDOWN_BASE + i;
            NOP(offset_base);
        }
        INFO("NoCooldown Enabled");
    } else {
        NoCooldownEnabled = 0;
        MemoryAddress mem((void*)NO_COOLDOWN_BASE);
        MemoryAddress mem2((void*)(NO_COOLDOWN_BASE + 1));
        *(BYTE*)NO_COOLDOWN_BASE = NoCooldownBackup[0];
        *(BYTE*)(NO_COOLDOWN_BASE + 1) = NoCooldownBackup[1];
        mem.Free();
        mem2.Free();
        INFO("NoCooldown Disabled");
    }
}

#define MONEY_SUBTOADD_BASE 0x0049B62A  // 3 Bytes length
BYTE NoMoneyCheckBackup[3];
BYTE NoMoneyCheckEnable;

void MoneySubToAdd() {
    if(!NoMoneyCheckEnable) {
        MemoryAddress mem((void*)(MONEY_SUBTOADD_BASE));
        MemoryAddress mem2((void*)(MONEY_SUBTOADD_BASE+1));
        MemoryAddress mem3((void*)(MONEY_SUBTOADD_BASE+2));
        NoMoneyCheckBackup[0] = *(BYTE*)((MONEY_SUBTOADD_BASE));
        NoMoneyCheckBackup[1] = *(BYTE*)((MONEY_SUBTOADD_BASE + 1));
        NoMoneyCheckBackup[2] = *(BYTE*)((MONEY_SUBTOADD_BASE + 2));
        *(BYTE*)((MONEY_SUBTOADD_BASE)) = 0x01;
        *(BYTE*)((MONEY_SUBTOADD_BASE + 1)) = 0x41;
        *(BYTE*)((MONEY_SUBTOADD_BASE + 2)) = 0x54;
        mem.Free();
        mem2.Free();
        mem3.Free();
        NoMoneyCheckEnable = 1;
        INFO("NoMoneyCheck enabled")
    } 
    else {
        for(int i =0; i < 3;i++) {
            MemoryAddress mem((void*)(MONEY_SUBTOADD_BASE + i));
            *(BYTE*)(MONEY_SUBTOADD_BASE + i) = NoMoneyCheckBackup[i];
            mem.Free();
        }
        NoMoneyCheckEnable = 0;
        INFO("NoMoneyCheck disabled")
    }
}

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    CheatWindow cheat;
    cheat.Button(1,"NoSubstract",10,10,100,20);
    cheat.Button(2,"SubToAdd",10,50,100,20);
    cheat.Button(3,"NoCooldown",10,90,100,20);
    cheat.Button(4,"MoneySubToAdd",10,130,100,20);
    cheat.NewEvent(1,(void*)NoSubstract);
    cheat.NewEvent(2,(void*)SubToAdd);
    cheat.NewEvent(3,(void*)NoCooldown);
    cheat.NewEvent(4,(void*)MoneySubToAdd);    
    cheat.WindowUpdate();
    cheat.HandleWindow();
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        std::cout << "Injected.\n";

        HANDLE hThread = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
        return TRUE;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}