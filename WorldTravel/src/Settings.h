#pragma once
#include <Windows.h>
#include <string>

class Settings
{
public:
    static bool EnableLogging;
    static bool FastTravel;
    static int TeleportKey;
    static bool EnableCayoPerico;
    static bool EnableNorthYankton;
	static bool EnableLibertyCityLODLights;
	static bool EnableLCOnStartup;
    static char iniFilePath[MAX_PATH];

    static void Load();

    static std::string GetLang(const char* key, const char* defaultValue);

private:
    static bool LoadBoolSetting(const char* section, const char* key, bool defaultValue, const char* iniFilePath);
    static int LoadIntSetting(const char* section, const char* key, int defaultValue, const char* iniFilePath);
};
