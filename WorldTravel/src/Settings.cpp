#include "Settings.h"

bool Settings::EnableLogging = true;
bool Settings::FastTravel = true;
int Settings::TeleportKey = VK_F11;
bool Settings::EnableCayoPerico = true;
bool Settings::EnableNorthYankton = true;
bool Settings::EnableLibertyCityLODLights = true;
bool Settings::EnableLCOnStartup = true;

char Settings::iniFilePath[MAX_PATH];


void Settings::Load()
{
    GetFullPathName("WorldTravel.ini", MAX_PATH, iniFilePath, nullptr);

    EnableLogging = LoadBoolSetting("WorldTravel", "EnableLogging", true, iniFilePath);
    FastTravel = LoadBoolSetting("WorldTravel", "FastTravel", true, iniFilePath);
    TeleportKey = LoadIntSetting("WorldTravel", "TeleportKey", VK_F11, iniFilePath);
    EnableCayoPerico = LoadBoolSetting("WorldTravel", "EnableCayoPerico", true, iniFilePath);
    EnableNorthYankton = LoadBoolSetting("WorldTravel", "EnableNorthYankton", true, iniFilePath);
	EnableLibertyCityLODLights = LoadBoolSetting("WorldTravel", "EnableLibertyCityLODLights", true, iniFilePath);
	EnableLCOnStartup = LoadBoolSetting("WorldTravel", "EnableLCOnStartup", false, iniFilePath);
}

// Settings helper functions
bool Settings::LoadBoolSetting(const char* section, const char* key, bool defaultValue, const char* iniFilePath)
{
    UINT value = GetPrivateProfileInt(section, key, defaultValue ? 1 : 0, iniFilePath);
    return value != 0;
}

int Settings::LoadIntSetting(const char* section, const char* key, int defaultValue, const char* iniFilePath)
{
    return GetPrivateProfileInt(section, key, defaultValue, iniFilePath);
}

std::string Settings::GetLang(const char* key, const char* defaultValue) {
	char buffer[256];
	GetPrivateProfileString("Languages", key, defaultValue, buffer, sizeof(buffer), iniFilePath);
	return std::string(buffer);
}
