#pragma once

#include <string>
#include "singleton.hpp"
#include "imgui/imgui.h"

struct IDirect3DDevice9;

template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(Utils::PatternScan2("client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::PatternScan2("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}

class Menu
	: public Singleton<Menu>
{
public:
	void Initialize();
	void Shutdown();

	void OnDeviceLost();
	void OnDeviceReset();
	

	void Watermark();
	void SpectatorList();

	void Render();
	void UpdateStyle();
	void Toggle();

	bool IsVisible() const { return _visible; }

private:
    void CreateStyle();


	ImGuiStyle        _style;
	bool              _visible;
};