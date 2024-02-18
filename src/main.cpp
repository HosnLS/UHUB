
#include <memory>
#include <thread>
#include <conio.h>
#include "lua.hpp"
#include "func.h"

using namespace std;

atomic_bool Stop = false;

void HookThread() {
    RegisterMouseHook();
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (Stop) break;
    }
    UnregisterMouseHook();
    cout << "HookThread exit" << endl;
}

void EventThread(lua_State *L) {
    std::tuple<std::string, int, std::string> event;
    while (true) {
        GlobalState::getInstance().onEventQueue.wait_pop(event);
        lua_getglobal(L, "OnEvent");
        lua_pushstring(L, std::get<0>(event).c_str());
        lua_pushinteger(L, std::get<1>(event));
        lua_pushstring(L, std::get<2>(event).c_str());
        lua_pcall(L, 3, 0, 0);
        if (Stop) break;
    }
    cout << "EventThread exit" << endl;
}

int main() {
    // Set the process priority to REALTIME_PRIORITY_CLASS
    HANDLE hProcess = GetCurrentProcess();
    BOOL result = SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
    if (result == 0)
        cerr << "SetPriorityClass failed: " << GetLastError() << endl;

    // Set the window to topmost
//    HWND hWnd = ::GetForegroundWindow();
//    ::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE);

    // Set the console to UTF-8
    system("chcp 65001");

    cout << "退出：输入 q，回车，随后点击鼠标中键/右键。或直接关闭窗口" << endl;
    cout << "Exit: input q, Enter, then click the middle/right mouse button. Or close the window directly" << endl;
    cout << endl;

    // Create a new Lua state
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // Register the functions
    lua_register(L, "Sleep", Sleep);
    lua_register(L, "OutputLogMessage", OutputLogMessage);
    lua_register(L, "OutputDebugMessage", OutputDebugMessage);
    lua_register(L, "ClearLog", ClearLog);
    lua_register(L, "GetRunningTime", GetRunningTime);
    lua_register(L, "GetDate", GetDate);

    lua_register(L, "EnablePrimaryMouseButtonEvents", EnablePrimaryMouseButtonEvents);

    lua_register(L, "PressKey", PressKey);
    lua_register(L, "ReleaseKey", ReleaseKey);
    lua_register(L, "PressAndReleaseKey", PressAndReleaseKey);
    lua_register(L, "IsModifierPressed", IsModifierPressed);
    lua_register(L, "IsKeyLockOn", IsKeyLockOn);

    lua_register(L, "PressMouseButton", PressMouseButton);
    lua_register(L, "ReleaseMouseButton", ReleaseMouseButton);
    lua_register(L, "PressAndReleaseMouseButton", PressAndReleaseMouseButton);
    lua_register(L, "IsMouseButtonPressed", IsMouseButtonPressed);
    lua_register(L, "MoveMouseTo", MoveMouseTo);
    lua_register(L, "MoveMouseToVirtual", MoveMouseToVirtual);
    lua_register(L, "MoveMouseRelative", MoveMouseRelative);
    lua_register(L, "MoveMouseWheel", MoveMouseWheel);
    lua_register(L, "GetMousePosition", GetMousePosition);

    // Initialize the global state
    GlobalState::resetInstance();

    // Load the script
    if (luaL_dofile(L, "jit/script.lua") != LUA_OK) {
        cerr << "Error Occurred!: " << lua_tostring(L, -1) << endl;
        lua_pop(L, 1);
    }

    // OnEvent: PROFILE_ACTIVATED
    lua_getglobal(L, "OnEvent");
    lua_pushstring(L, "PROFILE_ACTIVATED");
    lua_pcall(L, 1, 0, 0);

    // OnEvent: running
    // start a mew thread to monitor message from windows mouse hook
    thread hookThread(HookThread);

    // start a new thread to monitor the event queue
    thread eventThread(EventThread, L);

    while (true) {
        char c = char(getchar());
        if ('q' == c || 'Q' == c) {
            Stop = true;
            break;
        }
    }
    hookThread.join();
    eventThread.join();

    // OnEvent: PROFILE_DEACTIVATED
    lua_getglobal(L, "OnEvent");
    lua_pushstring(L, "PROFILE_DEACTIVATED");
    lua_call(L, 1, 0);

    // Close the Lua state
    lua_close(L);

    cout << "Main exit" << endl;

    return 0;
}