#ifndef UHUB_DEFINE_H
#define UHUB_DEFINE_H

#include <unordered_map>
#include <vector>

std::unordered_map<std::string, int> KEY_SC = {
        {"escape",       0x01},
        {"f1",           0x3b},
        {"f2",           0x3c},
        {"f3",           0x3d},
        {"f4",           0x3e},
        {"f5",           0x3f},
        {"f6",           0x40},
        {"f7",           0x41},
        {"f8",           0x42},
        {"f9",           0x43},
        {"f10",          0x44},
        {"f11",          0x57},
        {"f12",          0x58},
        {"f13",          0x64},
        {"f14",          0x65},
        {"f15",          0x66},
        {"f16",          0x67},
        {"f17",          0x68},
        {"f18",          0x69},
        {"f19",          0x6a},
        {"f20",          0x6b},
        {"f21",          0x6c},
        {"f22",          0x6d},
        {"f23",          0x6e},
        {"f24",          0x76},
        {"printscreen",  0x137},
        {"scrolllock",   0x46},
        {"pause",        0x146},
        {"tilde",        0x29},
        {"1",            0x02},
        {"2",            0x03},
        {"3",            0x04},
        {"4",            0x05},
        {"5",            0x06},
        {"6",            0x07},
        {"7",            0x08},
        {"8",            0x09},
        {"9",            0x0a},
        {"0",            0x0b},
        {"minus",        0x0c},
        {"equal",        0x0d},
        {"backspace",    0x0e},
        {"tab",          0x0f},
        {"q",            0x10},
        {"w",            0x11},
        {"e",            0x12},
        {"r",            0x13},
        {"t",            0x14},
        {"y",            0x15},
        {"u",            0x16},
        {"I",            0x17},
        {"o",            0x18},
        {"p",            0x19},
        {"lbracket",     0x1a},
        {"rbracket",     0x1b},
        {"backslash",    0x2b},
        {"capslock",     0x3a},
        {"a",            0x1e},
        {"s",            0x1f},
        {"d",            0x20},
        {"f",            0x21},
        {"g",            0x22},
        {"h",            0x23},
        {"j",            0x24},
        {"k",            0x25},
        {"l",            0x26},
        {"semicolon",    0x27},
        {"quote",        0x28},
        {"enter",        0x1c},
        {"lshift",       0x2a},
        {"non_us_slash", 0x56},
        {"z",            0x2c},
        {"x",            0x2d},
        {"c",            0x2e},
        {"v",            0x2f},
        {"b",            0x30},
        {"n",            0x31},
        {"m",            0x32},
        {"comma",        0x33},
        {"period",       0x34},
        {"slash",        0x35},
        {"rshift",       0x36},
        {"lctrl",        0x1d},
        {"lgui",         0x15b},
        {"lalt",         0x38},
        {"spacebar",     0x39},
        {"ralt",         0x138},
        {"rgui",         0x15c},
        {"appkey",       0x15d},
        {"rctrl",        0x11d},
        {"insert",       0x152},
        {"home",         0x147},
        {"pageup",       0x149},
        {"delete",       0x153},
        {"end",          0x14f},
        {"pagedown",     0x151},
        {"up",           0x148},
        {"left",         0x14b},
        {"down",         0x150},
        {"right",        0x14d},
        {"numlock",      0x45},
        {"numslash",     0x135},
        {"numminus",     0x4a},
        {"num7",         0x47},
        {"num8",         0x48},
        {"num9",         0x49},
        {"numplus",      0x4e},
        {"num4",         0x4b},
        {"num5",         0x4c},
        {"num6",         0x4d},
        {"num1",         0x4f},
        {"num2",         0x50},
        {"num3",         0x51},
        {"numenter",     0x11c},
        {"num0",         0x52},
        {"numperiod",    0x53},
};

std::unordered_map<std::string, std::vector<int>> MODIFIER_VK = {
        {"lctrl",  {VK_LCONTROL}},
        {"rctrl",  {VK_RCONTROL}},
        {"ctrl",   {VK_LCONTROL, VK_RCONTROL}},
        {"lalt",   {VK_LMENU}},
        {"ralt",   {VK_RMENU}},
        {"alt",    {VK_LMENU, VK_RMENU}},
        {"lshift", {VK_LSHIFT}},
        {"rshift", {VK_RSHIFT}},
        {"shift",  {VK_LSHIFT, VK_RSHIFT}},
};

std::unordered_map<std::string, int> KEYLOCK_VK = {
        {"scrolllock", VK_SCROLL},
        {"capslock", VK_CAPITAL},
        {"numlock",  VK_NUMLOCK},
};

std::unordered_map<int, std::pair<int, int>> MOUSEDOWN_SC = {
        {1, {MOUSEEVENTF_LEFTDOWN, 0}},
        {2, {MOUSEEVENTF_MIDDLEDOWN, 0}},
        {3, {MOUSEEVENTF_RIGHTDOWN, 0}},
        {4, {MOUSEEVENTF_XDOWN, XBUTTON1}},
        {5, {MOUSEEVENTF_XDOWN, XBUTTON2}},
};

std::unordered_map<int, std::pair<int, int>> MOUSEUP_SC = {
        {1, {MOUSEEVENTF_LEFTUP, 0}},
        {2, {MOUSEEVENTF_MIDDLEUP, 0}},
        {3, {MOUSEEVENTF_RIGHTUP, 0}},
        {4, {MOUSEEVENTF_XUP, XBUTTON1}},
        {5, {MOUSEEVENTF_XUP, XBUTTON2}},
};

std::unordered_map<int, int> MOUSE_VK = {
        {1, VK_LBUTTON},
        {2, VK_MBUTTON},
        {3, VK_RBUTTON},
        {4, VK_XBUTTON1},
        {5, VK_XBUTTON2},
};

#endif //UHUB_DEFINE_H
