#include <thread>

#include "func.h"
#include "define.h"

using namespace std;
using namespace chrono;

GlobalState::GlobalState() {
    launchTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    PrimaryMouseButtonEvents = false;
    onEventQueue.clear();
}

GlobalState &GlobalState::getInstance() {
    static GlobalState instance;
    return instance;
}

void GlobalState::resetInstance() {
    GlobalState::getInstance().launchTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    GlobalState::getInstance().PrimaryMouseButtonEvents = false;
    GlobalState::getInstance().onEventQueue.clear();
}

// -- System Functions --

int Sleep(lua_State *L) {
    int64_t ms = luaL_checkinteger(L, 1);
    this_thread::sleep_for(milliseconds(ms));
    return 0;
}

void outputMessage(lua_State *L, ostream &os) {
    int numArgs = lua_gettop(L);
    for (int i = 1; i <= numArgs; i++) {
        if (lua_isstring(L, i)) {
            os << lua_tostring(L, i);
        } else if (lua_isnumber(L, i)) {
            os << lua_tonumber(L, i);
        } else if (lua_isboolean(L, i)) {
            os << lua_toboolean(L, i);
        } else if (lua_isnil(L, i)) {
            os << "nil";
        } else {
            os << lua_typename(L, lua_type(L, i));
        }
        os << '\t';
    }
    os << flush;
}

int OutputLogMessage(lua_State *L) {
    outputMessage(L, cout);
    return 0;
}

int OutputDebugMessage(lua_State *L) {
    outputMessage(L, cerr);
    return 0;
}

int ClearLog(lua_State *L) {
    system("cls");
    return 0;
}


int GetRunningTime(lua_State *L) {
    auto currentTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    int64_t runningTime = currentTime.count() - GlobalState::getInstance().launchTime.count();
    lua_pushinteger(L, runningTime);
    return 1;
}


int GetDate(lua_State *L) {
    // param1: format (optional): same as strftime, default: "%a %b %-d %H:%M:%S %Y"
    // param2: time (optional): lua time
    // return: formatted date string
    const char *format = luaL_optstring(L, 1, "%a %b %e %H:%M:%S %Y");
    time_t t = luaL_optinteger(L, 2, system_clock::to_time_t(system_clock::now()));
    char buffer[80];
    tm timeinfo{};
    localtime_s(&timeinfo, &t);
    strftime(buffer, 80, format, &timeinfo);
    lua_pushstring(L, buffer);
    return 1;
}

// -- Callback Functions --

int EnablePrimaryMouseButtonEvents(lua_State *L) {
    bool enable = lua_toboolean(L, 1);
    GlobalState::getInstance().PrimaryMouseButtonEvents = enable;
    return 0;
}


LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        switch (wParam) {
            case WM_LBUTTONDOWN:
                if (GlobalState::getInstance().PrimaryMouseButtonEvents)
                    GlobalState::getInstance().onEventQueue.emplace("MOUSE_BUTTON_PRESSED", 1, "mouse");
                PostMessage(nullptr, WM_USER + 1, 0, 0);
                break;
            case WM_LBUTTONUP:
                if (GlobalState::getInstance().PrimaryMouseButtonEvents)
                    GlobalState::getInstance().onEventQueue.emplace("MOUSE_BUTTON_RELEASED", 1, "mouse");
                PostMessage(nullptr, WM_USER + 1, 0, 0);
                break;
            case WM_MBUTTONDOWN:
                GlobalState::getInstance().onEventQueue.emplace("MOUSE_BUTTON_PRESSED", 3, "mouse");
                PostMessage(nullptr, WM_USER + 1, 0, 0);
                break;
            case WM_MBUTTONUP:
                GlobalState::getInstance().onEventQueue.emplace("MOUSE_BUTTON_RELEASED", 3, "mouse");
                PostMessage(nullptr, WM_USER + 1, 0, 0);
                break;
            case WM_RBUTTONDOWN:
                GlobalState::getInstance().onEventQueue.emplace("MOUSE_BUTTON_PRESSED", 2, "mouse");
                PostMessage(nullptr, WM_USER + 1, 0, 0);
                break;
            case WM_RBUTTONUP:
                GlobalState::getInstance().onEventQueue.emplace("MOUSE_BUTTON_RELEASED", 2, "mouse");
                PostMessage(nullptr, WM_USER + 1, 0, 0);
                break;
            default:
                break;
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam); // Pass event to other hooks
}

void RegisterMouseHook() {
    GlobalState::getInstance().mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, nullptr, 0);
}

void UnregisterMouseHook() {
    UnhookWindowsHookEx(GlobalState::getInstance().mouseHook);
}

// -- KB Functions --

int operateKey(lua_State *L, int flag) {
    int numArgs = lua_gettop(L);
    for (int i = 1; i <= numArgs; i++) {
        int scanCode = -1;
        if (lua_isnumber(L, i)) {
            scanCode = int(lua_tointeger(L, i));
        } else if (lua_isstring(L, i)) {
            auto it = KEY_SC.find(lua_tostring(L, i));
            if (it != KEY_SC.end()) {
                scanCode = it->second;
            }
        }
        if (scanCode != -1) {
            INPUT input{};
            input.type = INPUT_KEYBOARD;
            input.ki.dwFlags = flag | KEYEVENTF_SCANCODE;
            input.ki.wScan = scanCode;
            SendInput(1, &input, sizeof(INPUT));
        }
    }
    return 0;
}

int PressKey(lua_State *L) {
    operateKey(L, 0);
    return 0;
}

int ReleaseKey(lua_State *L) {
    operateKey(L, KEYEVENTF_KEYUP);
    return 0;
}

int PressAndReleaseKey(lua_State *L) {
    PressKey(L);
    ReleaseKey(L);
    return 0;
}

int IsModifierPressed(lua_State *L) {
    bool isPressed = false;
    auto it = MODIFIER_VK.find(lua_tostring(L, 1));
    if (it != MODIFIER_VK.end()) {
        for (const auto key: it->second)
            isPressed |= bool(GetAsyncKeyState(key) & 0x8000);

    }
    lua_pushboolean(L, isPressed);
    return 1;
}

int IsKeyLockOn(lua_State *L) {
    bool isOn = false;
    auto it = KEYLOCK_VK.find(lua_tostring(L, 1));
    if (it != KEYLOCK_VK.end())
        isOn = GetKeyState(it->second) & 0x0001;
    lua_pushboolean(L, isOn);
    return 1;
}

// -- Mouse Functions --

int OperateButton(lua_State *L, bool down) {
    auto map = down ? MOUSEDOWN_SC : MOUSEUP_SC;
    auto it = map.find(int(lua_tointeger(L, 1)));
    if (it != map.end()) {
        cout << it->second.first << " " << it->second.second << endl;
        INPUT input{};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = it->second.first;
        input.mi.mouseData = it->second.second;
        SendInput(1, &input, sizeof(INPUT));
    }
    return 0;
}

int PressMouseButton(lua_State *L) {
    OperateButton(L, true);
    return 0;
}

int ReleaseMouseButton(lua_State *L) {
    OperateButton(L, false);
    return 0;
}

int PressAndReleaseMouseButton(lua_State *L) {
    PressMouseButton(L);
    ReleaseMouseButton(L);
    return 0;
}

int IsMouseButtonPressed(lua_State *L) {
    bool isPressed = false;
    auto it = MOUSE_VK.find(int(lua_tointeger(L, 1)));
    if (it != MOUSE_VK.end())
        isPressed = GetAsyncKeyState(it->second) & 0x8000;
    lua_pushboolean(L, isPressed);
    return 1;
}

int MoveMouseTo(lua_State *L) {
    int x = int(lua_tointeger(L, 1));
    int y = int(lua_tointeger(L, 2));

    // Move the mouse
    INPUT input{};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    input.mi.dx = LONG(x);
    input.mi.dy = LONG(y);
    SendInput(1, &input, sizeof(INPUT));

    return 0;
}

int MoveMouseToVirtual(lua_State *L) {
    int x = int(lua_tointeger(L, 1));
    int y = int(lua_tointeger(L, 2));

    // Move the mouse
    INPUT input{};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE;
    input.mi.dx = LONG(x);
    input.mi.dy = LONG(y);
    SendInput(1, &input, sizeof(INPUT));

    return 0;
}

int MoveMouseRelative(lua_State *L) {
    int x = int(lua_tointeger(L, 1));
    int y = int(lua_tointeger(L, 2));

    // Move the mouse
    INPUT input{};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = x;
    input.mi.dy = y;
    SendInput(1, &input, sizeof(INPUT));

    return 0;
}

int MoveMouseWheel(lua_State *L) {
    int click = int(lua_tointeger(L, 1));

    INPUT input{};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = click * WHEEL_DELTA;
    SendInput(1, &input, sizeof(INPUT));

    return 0;
}

int GetMousePosition(lua_State *L) {
    // ~ 1 pixel error on virtual screen
    POINT p;
    GetPhysicalCursorPos(&p);

    int currentMonitorWidth = GetSystemMetrics(SM_CXSCREEN);
    int currentMonitorHeight = GetSystemMetrics(SM_CYSCREEN);

    int64_t x = (p.x * 65535 + currentMonitorWidth / 2) / (currentMonitorWidth - 1);
    int64_t y = (p.y * 65535 + currentMonitorHeight / 2) / (currentMonitorHeight - 1);

    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    return 2;
}
