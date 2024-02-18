#ifndef UHUB_FUNC_H
#define UHUB_FUNC_H

#include <iostream>
#include <chrono>
#include <atomic>
#include <tuple>
#include <string>
#include <windows.h>
#include "lua.hpp"
#include "safe_queue.h"

class GlobalState {
    /**
     * Singleton class to store global state
     * launchTime: time of lua script launch, used for GetRunningTime
     * PrimaryMouseButtonEvents: whether to enable primary mouse button events, used for OnEvent
     * onEventQueue: queue of OnEvent events
     * mouseHook: handle to mouse hook, used for OnEvent
     */
    GlobalState();

    ~GlobalState() = default;

public:
    GlobalState(const GlobalState &) = delete;

    GlobalState &operator=(const GlobalState &) = delete;

    static GlobalState &getInstance();

    static void resetInstance();

    std::chrono::milliseconds launchTime{};
    std::atomic_bool PrimaryMouseButtonEvents = false;
    SafeQueue<std::tuple<std::string, int, std::string>> onEventQueue{};
    HHOOK mouseHook{};
};


// -- System Functions --

int Sleep(lua_State *L);

int OutputLogMessage(lua_State *L);

int OutputDebugMessage(lua_State *L);

int ClearLog(lua_State *L);

int GetRunningTime(lua_State *L);

int GetDate(lua_State *L);

// -- Callback Functions --

int EnablePrimaryMouseButtonEvents(lua_State *L);

// OnEvent (slave): partially implemented
//     PROFILE_ACTIVATED
//     PROFILE_DEACTIVATED
//     G_PRESSED (not implemented)
//     G_RELEASED (not implemented)
//     M_PRESSED (not implemented)
//     M_RELEASED (not implemented)
//     MOUSE_BUTTON_PRESSED: depending on PrimaryMouseButtonEvents (left click)
//     MOUSE_BUTTON_RELEASED: depending on PrimaryMouseButtonEvents (left click)
void RegisterMouseHook();
// Register WinAPI mouse hook for OnEvent
void UnregisterMouseHook();
// Unregister WinAPI mouse hook for OnEvent


// -- KB Functions --

int PressKey(lua_State *L);
int ReleaseKey(lua_State *L);
int PressAndReleaseKey(lua_State *L);
int IsModifierPressed(lua_State *L);
int IsKeyLockOn(lua_State *L);

// -- Mouse Functions --

int PressMouseButton(lua_State *L);
int ReleaseMouseButton(lua_State *L);
int PressAndReleaseMouseButton(lua_State *L);
int IsMouseButtonPressed(lua_State *L);
int MoveMouseTo(lua_State *L);
int MoveMouseToVirtual(lua_State *L);
int MoveMouseRelative(lua_State *L);
int MoveMouseWheel(lua_State *L);
int GetMousePosition(lua_State *L);

// -- Device Specific --
// These functions are not implemented

// GetMKeyState
// SetMKeyState
// SetMouseDPITable
// SetMouseDPITableIndex

// OutputLCDMessage
// ClearLCD
// SetBacklightColor
// PlayMacro
// AbortMacro
// SetSteeringWheelProperty
// (G13) SetMouseSpeed
// (G13) GetMouseSpeed
// (G13) IncrementMouseSpeed
// (G13) DecrementMouseSpeed


#endif //UHUB_FUNC_H
