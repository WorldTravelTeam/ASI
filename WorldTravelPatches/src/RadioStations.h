#pragma once
#include <cstdint>
#include <string>
#include "Hooking.Patterns.h"
#include <MinHook.h>
#include "Hooking.h"

class RadioStations
{
public:
	static void Patch()
	{
		uint8_t* maxRadioStations = hook::get_pattern<uint8_t>("8D 5E 60 89 35", 2); // audRadioStation::InitClass
		DWORD oldProtect;
		VirtualProtect(maxRadioStations, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
		*maxRadioStations = 127;
		VirtualProtect(maxRadioStations, 1, oldProtect, &oldProtect);
		uint32_t* radioStationsMemSize = (uint32_t*)(maxRadioStations + 0x26);
		VirtualProtect(radioStationsMemSize, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
		*radioStationsMemSize = 127 * 8;
		VirtualProtect(radioStationsMemSize, 4, oldProtect, &oldProtect);


		uint8_t* maxRadioStations2 = hook::get_pattern<uint8_t>("BD 60 00 00 00 E9", 1);
		VirtualProtect(maxRadioStations2, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
		*maxRadioStations2 = 127;
		VirtualProtect(maxRadioStations2, 1, oldProtect, &oldProtect);
		uint32_t* radioStationsMemSize2 = (uint32_t*)(maxRadioStations2 + 0x43);
		VirtualProtect(radioStationsMemSize2, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
		*radioStationsMemSize2 = 127 * 8;
		VirtualProtect(radioStationsMemSize2, 4, oldProtect, &oldProtect);


		uint8_t* maxRadioStations3 = hook::get_pattern<uint8_t>("45 8D 67 ? 8B C6", 3); // audRadioStation::RemoveRadioStationList
		VirtualProtect(maxRadioStations3, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
		*maxRadioStations3 = 127;
		VirtualProtect(maxRadioStations3, 1, oldProtect, &oldProtect);
		uint32_t* radioStationsMemSize3 = (uint32_t*)(maxRadioStations3 + 0x141);
		VirtualProtect(radioStationsMemSize3, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
		*radioStationsMemSize3 = 127 * 8;
		VirtualProtect(radioStationsMemSize3, 4, oldProtect, &oldProtect);
	}
};