#include "../../helpers/utils.hpp"

KeyValues* KeyValues::FindKey(const char* keyName, bool bCreate)
{
	static auto key_values_find_key = reinterpret_cast<KeyValues * (__thiscall*)(void*, const char*, bool)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 EC 1C 53 8B D9 85 DB"));
	return key_values_find_key(this, keyName, bCreate);
}

const char* KeyValues::GetString(KeyValues* pThis, const char* keyName, const char* defaultValue)
{
	static auto key_values_get_string = reinterpret_cast<const char* (__thiscall*)(void*, const char*, const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08"));
	return key_values_get_string(pThis, keyName, defaultValue);
}
const char* KeyValues::GetStringxD(const char* keyName, const char* defaultValue)
{
	static auto key_values_get_string = reinterpret_cast<const char* (__thiscall*)(void*, const char*, const char*)>(Utils::PatternScan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08"));
	return key_values_get_string(this, keyName, defaultValue);
}
bool KeyValues::LoadFromBuffer(KeyValues* pThis, const char* pszFirst, const char* pszSecond, PVOID pSomething, PVOID pAnother, PVOID pLast, PVOID pAnother2)
{
	//.text : 1041E409                 call    LoadFromBuffer2 <----- here
	//.text : 1041E40E                 call    sub_10402010
	//.text : 1041E413                 push    offset FileName
	//.text : 1041E418                 push    offset aGame_1; "game"
	//.text : 1041E41D                 mov     ecx, [eax + 200h]
	//.text : 1041E423                 call    sub_10898B00
	//.text : 1041E428                 push    offset FileName
	//.text : 1041E42D                 push    offset aGame_1; "game"

	static auto callLoadBuff = reinterpret_cast<bool(__thiscall*)(KeyValues*, const char*, const char*, PVOID, PVOID, PVOID, PVOID)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04"));

	return callLoadBuff(pThis, pszFirst, pszSecond, pSomething, pAnother, pLast, pAnother2);
}

void KeyValues::SetString(const char* name, const char* value)
{
	auto keyvalues = FindKey(name, 1);

	static auto fnSetString = reinterpret_cast<void(__thiscall*)(KeyValues*, const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01"));
	if (keyvalues)
		fnSetString(keyvalues, value);
}
void KeyValues::SetInt(const char* name, int value)
{
	auto keyvalues = FindKey(name, 1);

	if (keyvalues)
	{
		m_iValue = value;
		m_iDataType = 2;
	}
}

void KeyValues::SetFloat(const char* name, float value)
{
	auto keyvalues = FindKey(name, 1);

	if (keyvalues)
	{
		m_flValue = value;
		m_iDataType = 3;
	}
}