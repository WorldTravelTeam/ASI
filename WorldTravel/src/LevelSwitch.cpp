#include "LevelSwitch.h"
#include <fstream>  
#include <windows.h>
#include <string>
#include <ctime>
#include <iostream>
#include <vector>
#include "HelpText.h"
#include "MpMap.h"
#include "PlayerLocation.h"
#include "Farlods.h"
#include "PathNodeState.h"
#include <unordered_map>
#include <functional>
#include "IPL.h"
#include "Settings.h"

namespace levelSwitch
{
	// Streaming Data Index
	int LibertyCityScenariosIndex;
	int LibertyCityZonesIndex;
	int LibertyCityIPLsIndex;
	int LosSantosScenariosIndex;
	int LosSantosZonesIndex;
	int LosSantosIPLsIndex;
	int ModIPLsIndex;

	// Streaming Data References
	std::vector<std::string> libertyScenarios;
	std::vector<std::string> libertyZones;
	std::vector<std::string> libertyAmbientZones;
	std::vector<std::string> libertyIpls;
	std::vector<std::string> libertySpIpls;
	std::vector<std::string> libertyMpIpls;
	std::vector<std::string> libertyLODLightIpls;
	std::vector<std::string> libertyPeds;
	std::vector<std::string> libertyVehicles;
	std::vector<std::string> libertyRadio;
	std::vector<std::string> santosScenarios;
	std::vector<std::string> santosZones;
	std::vector<std::string> santosAmbientZones;
	std::vector<std::string> santosIpls;
	std::vector<std::string> santosSpIpls;
	std::vector<std::string> santosMpIpls;
	std::vector<std::string> santosMpForcedIpls;
	std::vector<std::string> santosPeds;
	std::vector<std::string> santosVehicles;
	std::vector<std::string> santosRadio;
	std::vector<std::string> yanktonZones;
	std::vector<std::string> yanktonAmbientZones;
	std::vector<std::string> yanktonIpls;
	std::vector<std::string> cayoScenarios;
	std::vector<std::string> cayoZones;
	std::vector<std::string> cayoAmbientZones;
	std::vector<std::string> cayoMpIpls;
	std::vector<std::string> cayoInteriors = { "h4_islandx_mansion_vault",
												"h4_islandx_mansion_lockup_03",
												"h4_islandx_mansion_lockup_02",
												"h4_islandx_mansion_lockup_01",
												"h4_islandx_mansion_office",
												"h4_mph4_airstrip_interior_0_airstrip_hanger" };
	std::vector<std::string> cayoInstancePlacement = { "h4_mph4_terrain_01_grass_0",
														"h4_mph4_terrain_01_grass_1",
														"h4_mph4_terrain_02_grass_0",
														"h4_mph4_terrain_02_grass_1",
														"h4_mph4_terrain_02_grass_2",
														"h4_mph4_terrain_02_grass_3",
														"h4_mph4_terrain_04_grass_0",
														"h4_mph4_terrain_04_grass_1",
														"h4_mph4_terrain_05_grass_0",
														"h4_mph4_terrain_06_grass_0"};
	std::vector<std::string> cayoOccl = { "h4_mph4_terrain_occ_00",
											"h4_mph4_terrain_occ_01",
											"h4_mph4_terrain_occ_02",
											"h4_mph4_terrain_occ_03",
											"h4_mph4_terrain_occ_04",
											"h4_mph4_terrain_occ_05",
											"h4_mph4_terrain_occ_06",
											"h4_mph4_terrain_occ_07",
											"h4_mph4_terrain_occ_08",
											"h4_mph4_terrain_occ_09" };
	std::vector<std::string> modIpls;
	std::vector<bool> santosIplState;
	std::vector<bool> santosSpIplState;
	std::vector<bool> santosMpIplState;
	std::vector<bool> santosScenariosState;
	std::vector<bool> santosAmbientZonesState;

	//WeatherTypes
	std::vector<std::string> weatherTypes =
	{
		"CLEAR",
		"EXTRASUNNY",
		"CLOUDS",
		"OVERCAST",
		"RAIN",
		"CLEARING",
		"THUNDER",
		"SMOG",
		"CLEAR",
		"EXTRASUNNY",
		"CLOUDS",
		"SMOG"
	};
	std::vector<std::string> yanktonWeatherTypes =
	{
		"CLEAR",
		"EXTRASUNNY",
		"CLOUDS",
		"OVERCAST",
		"EXTRASUNNY",
		"XMAS",
		"SNOW",
		"SNOWLIGHT",
		"BLIZZARD",
		"XMAS",
		"SNOW",
		"SNOWLIGHT",
		"BLIZZARD"
	};

	int LCEnabled = 0;
	//int playerLocationID = 0; // 0 = LS    1 = LC    2 = NY    3 = CP

	// Player Objects
	Player player;
	Ped playerPed;
	Vehicle playerVehicle;
	Vehicle yanktonVehicle;
	Hash yanktonVehicleHash;
	BOOL bPlayerExists;
	Vector3 PlayerPosition;

	// Blip Objects
	Blip AirportBlip;
	Blip DocksBlip;
	Blip FlyToLCBlip;
	Blip FlyToLSBlip;
	Blip FlyToCPBlip;
	bool airportBlipVisible = false;
	bool docksBlipVisible = false;
	bool flyToLCBlipVisible = false;
	bool flyToLSBlipVisible = false;
	bool flyToCPBlipVisible = false;
	bool airportBlipFarDist = false;

	// Marker Cords
	std::vector<float> LibertyCityOffsetPos =		{ 5188.185f, -3256.298f, 0.0f };
	std::vector<float> LosSantosIntAirport =		{ -1050.53f, -2741.48f, 14.60f };
	std::vector<float> LosSantosIntAirportArrival = { -1037.446f, -2737.821f, 20.168f };
	std::vector<float> LosSantosDocks =				{ 1170.40f, -2973.52f, 5.13f };
	std::vector<float> LosSantosDocksArrival =		{ 1143.96f, -2953.18f, 5.13f };
	std::vector<float> FrancisIntAirport =			{ 7553.86f, -2879.39f, 6.08f };
	std::vector<float> FrancisIntAirportArrival =	{ 7552.72f, -2851.3f, 6.08f };
	std::vector<float> LibertyCityDocks =			{ 5935.99f, -3531.89f, 5.05f };
	std::vector<float> LibertyCityDocksArrival =	{ 5941.92f, -3481.14f, 5.05f };
	std::vector<float> NorthYanktonAirport =		{ 15873.73f, -5158.65f, 83.85f };
	std::vector<float> NorthYanktonAirportArrival = { 5550.18f, -5131.62f, 78.55f };
	std::vector<float> CayoPericoAirport =			{ 4496.84f, -4521.04f, 4.41f };
	std::vector<float> TempLocation =				{ 0.0f, 0.0f, 100.0f };

	// Bounds
	float LibertyCityNorthBound = 1200.0f;
	float LibertyCitySouthBound = -8000.0f;
	float LibertyCityEastBound = 9900.0f;
	float LibertyCityWestBound = 1600.0f;

	float CayoPericoNorthBound = -3150.0f;
	float CayoPericoSouthBound = -7150.0f;
	float CayoPericoEastBound = 6700.0f;
	float CayoPericoWestBound = 2700.0f;
	float CayoPericoMPDestHeading = 35.0f;

	// Flight Trigger Cords
	std::vector<float> FlyToLCFromLSCoords =	{ 9500.0f, 9500.0f, 0.0f };
	std::vector<float> FlyToLSFromLCCoords =	{ -9500.0f, -9500.0f, 0.0f };
	std::vector<float> FlyToCPFromLSCoords =	{ 9500.0f, -9500.0f, 0.0f };
	std::vector<float> FlyToLCFromCPCoords =	{ -4000.0f, 9500.0f, 0.0f };
	std::vector<float> FlyToCPFromLCCoordsMP =	{ 3700.0f, -3200.0f, 0.0f };
	std::vector<float> FlyToLSFromCPCoords =	{ -9500.0f, 9500.0f, 0.0f };
	std::vector<float> FlyToCPFromLSCoordsMP =	{ 2750.0f, -3200.0f, 0.0f };
	std::vector<float> FlyToCPFromLCCoords =	{ 10500.0f, -13500.0f, 0.0f };

	// Interior Cords
	std::vector<float> heist_police_dlc =		{ 442.4296f, -985.067f, 29.88529f };
	std::vector<float> gr_grdlc_int_01 =		{ 1103.562f, -3000.0f, -40.0f };
	std::vector<float> xm_x17dlc_int_01 =		{ 520.0f, 4750.0f, -70.0f };
	std::vector<float> xm_x17dlc_int_sub =		{ 512.7686f, 4851.981f, -62.90245f };
	std::vector<float> ba_dlc_int_03_ba =		{ -1421.015f, -3012.587f, -80.0f };
	std::vector<float> h4_interior_0_int_sub =	{ 1560.0f, 400.0f, -50.0f };
	std::vector<float> dlc_int_01_xm3 =			{ 485.0f, -2625.0f, -50.0f };
	bool tempWaterSwapActive = false;
	bool hasCayoLoadedExternally = false;

	void readFileToVector(const std::string& filePath, std::vector<std::string>& targetVector) {
		std::ifstream file(filePath);
		std::string line;

		if (file.is_open()) {
			while (std::getline(file, line)) {
				if (!line.empty()) {
					targetVector.push_back(line);
				}
			}
			file.close();
		}
	}

	// Generalized function for all locations
	void readCityFiles(const std::string& city, const std::unordered_map<std::string, std::vector<std::string>*>& fileMap) {
		for (const auto& [fileSuffix, targetVector] : fileMap) {
			std::string filePath = "./Levels/" + city + "/" + fileSuffix + ".txt";
			readFileToVector(filePath, *targetVector);
		}
	}

	// Location specific readers
	void readLSFiles() {
		std::unordered_map<std::string, std::vector<std::string>*> lsFiles = {
			{"Scenarios", &santosScenarios},
			{"Zones", &santosZones},
			{"AmbientZones", &santosAmbientZones},
			{"IPLs", &santosIpls},
			{"IPLsSP", &santosSpIpls},
			{"IPLsMP", &santosMpIpls},
			{"IPLsMPForced", &santosMpForcedIpls},
			{"ZonedPeds", &santosPeds},
			{"ZonedVehicles", &santosVehicles},
			{"ZonedRadioStations", &santosRadio},
			{"Mods/ModIPLs", &modIpls}
		};
		readCityFiles("Los Santos", lsFiles);
	}

	void readNYFiles() {
		std::unordered_map<std::string, std::vector<std::string>*> nyFiles = {
			{"Zones", &yanktonZones},
			{"AmbientZones", &yanktonAmbientZones},
			{"IPLs", &yanktonIpls}
		};
		readCityFiles("North Yankton", nyFiles);
	}

	void readCPFiles() {
		std::unordered_map<std::string, std::vector<std::string>*> cpFiles = {
			{"Scenarios", &cayoScenarios},
			{"Zones", &cayoZones},
			{"AmbientZones", &cayoAmbientZones},
			{"IPLsMP", &cayoMpIpls}
		};
		readCityFiles("Cayo Perico", cpFiles);
	}

	void readLCFiles() {
		std::unordered_map<std::string, std::vector<std::string>*> lcFiles = {
			{"Scenarios", &libertyScenarios},
			{"Zones", &libertyZones},
			{"AmbientZones", &libertyAmbientZones},
			{"IPLs", &libertyIpls},
			{"IPLsSP", &libertySpIpls},
			{"IPLsMP", &libertyMpIpls},
			{"IPLsLODLights", &libertyLODLightIpls},
			{"ZonedPeds", &libertyPeds},
			{"ZonedVehicles", &libertyVehicles},
			{"ZonedRadioStations", &libertyRadio}
		};
		readCityFiles("Liberty City", lcFiles);
	}


	void RequestScript(PCHAR scriptName, int stackSize)
	{
		if (SCRIPT::_GET_NUMBER_OF_INSTANCES_OF_SCRIPT_WITH_NAME_HASH(GAMEPLAY::GET_HASH_KEY(scriptName)) == 0 && SCRIPT::DOES_SCRIPT_EXIST(scriptName))
		{
			SCRIPT::REQUEST_SCRIPT(scriptName);

			for (ULONGLONG timeOut = GetTickCount64() + 5000; GetTickCount64() < timeOut;)
			{
				if (SCRIPT::HAS_SCRIPT_LOADED(scriptName))
					break;
				WAIT(0);
			}

			SYSTEM::START_NEW_SCRIPT(scriptName, stackSize); // 1024 on console
			SCRIPT::SET_SCRIPT_AS_NO_LONGER_NEEDED(scriptName);
		}
	}

	// Initialize Liberty City
	void initialize()
	{
		readLSFiles();
		readLCFiles();
		readNYFiles();
		readCPFiles();

		for (int i = 0; i < libertyScenarios.size(); i++)
		{
			AI::SET_SCENARIO_GROUP_ENABLED((char*)libertyScenarios[i].c_str(), false);
		}

		for (int i = 0; i < libertyZones.size(); i++)
		{
			int zoneId = ZONE::GET_ZONE_FROM_NAME_ID((char*)libertyZones[i].c_str());
			ZONE::SET_ZONE_ENABLED(zoneId, false);
		}

		for (int i = 0; i < libertyAmbientZones.size(); i++)
		{
			AUDIO::SET_AMBIENT_ZONE_STATE_PERSISTENT(const_cast<char*>(libertyAmbientZones[i].c_str()), false, true);
		}

		for (int i = 0; i < libertyPeds.size(); i++)
		{
			PED::SET_PED_MODEL_IS_SUPPRESSED(GAMEPLAY::GET_HASH_KEY((char*)libertyPeds[i].c_str()), true);
		}

		for (int i = 0; i < libertyVehicles.size(); i++)
		{
			VEHICLE::SET_VEHICLE_MODEL_IS_SUPPRESSED(GAMEPLAY::GET_HASH_KEY((char*)libertyVehicles[i].c_str()), true);
		}

		for (int i = 0; i < libertyRadio.size(); i++)
		{
			AUDIO::LOCK_RADIO_STATION((char*)libertyRadio[i].c_str(), true);
		}

		for (int i = 0; i < santosRadio.size(); i++)
		{
			AUDIO::LOCK_RADIO_STATION((char*)santosRadio[i].c_str(), false);
		}

		for (int i = 0; i < cayoInteriors.size(); i++)
		{
			STREAMING::REMOVE_IPL(const_cast<char*>(cayoInteriors[i].c_str()));
		}

		for (int i = 0; i < cayoInstancePlacement.size(); i++)
		{
			STREAMING::REMOVE_IPL(const_cast<char*>(cayoInstancePlacement[i].c_str()));
		}
		
		for (int i = 0; i < cayoOccl.size(); i++)
		{
			STREAMING::REMOVE_IPL(const_cast<char*>(cayoOccl[i].c_str()));
		}

		for (int i = 0; i < cayoZones.size(); i++)
		{
			int zoneId = ZONE::GET_ZONE_FROM_NAME_ID((char*)cayoZones[i].c_str());
			ZONE::SET_ZONE_ENABLED(zoneId, false);
		}
	}

	void CreateBlips()
	{
		if (!UI::DOES_BLIP_EXIST(AirportBlip))
		{
			if (worldtravel::IsLosSantos())
				AirportBlip = UI::ADD_BLIP_FOR_COORD(LosSantosIntAirport[0], LosSantosIntAirport[1], LosSantosIntAirport[2]);

			else if (worldtravel::IsLibertyCity())
				AirportBlip = UI::ADD_BLIP_FOR_COORD(FrancisIntAirport[0], FrancisIntAirport[1], FrancisIntAirport[2]);

			else if (worldtravel::IsNorthYankton())
				AirportBlip = UI::ADD_BLIP_FOR_COORD(NorthYanktonAirport[0], NorthYanktonAirport[1], NorthYanktonAirport[2]);

			UI::SET_BLIP_SCALE(AirportBlip, 1);
			UI::SET_BLIP_SPRITE(AirportBlip, 90);
			UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
            UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const_cast<char*>(Settings::GetLang("Lng29", "Airport").c_str()));
			UI::END_TEXT_COMMAND_SET_BLIP_NAME(AirportBlip);
			UI::SET_BLIP_COLOUR(AirportBlip, 0);
			UI::SET_BLIP_FLASHES(AirportBlip, false);

			if (!worldtravel::IsCayoPerico())
				UI::SET_BLIP_DISPLAY(AirportBlip, 2);
			else
				UI::SET_BLIP_DISPLAY(AirportBlip, 0);
			UI::SET_BLIP_AS_SHORT_RANGE(AirportBlip, true);
			UI::SET_BLIP_CATEGORY(AirportBlip, 1);
		}

		if (!UI::DOES_BLIP_EXIST(DocksBlip))
		{
			if (worldtravel::IsLosSantos())
				DocksBlip = UI::ADD_BLIP_FOR_COORD(LosSantosDocks[0], LosSantosDocks[1], LosSantosDocks[2]);

			else if (worldtravel::IsLibertyCity())
				DocksBlip = UI::ADD_BLIP_FOR_COORD(LibertyCityDocks[0], LibertyCityDocks[1], LibertyCityDocks[2]);

			UI::SET_BLIP_SCALE(DocksBlip, 1);
			UI::SET_BLIP_SPRITE(DocksBlip, 677);
			UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const_cast<char*>(Settings::GetLang("Lng30", "Docks").c_str()));
			UI::END_TEXT_COMMAND_SET_BLIP_NAME(DocksBlip);
			UI::SET_BLIP_COLOUR(DocksBlip, 0);
			UI::SET_BLIP_FLASHES(DocksBlip, false);

			if (worldtravel::IsLosSantos() || worldtravel::IsLibertyCity())
				UI::SET_BLIP_DISPLAY(DocksBlip, 2);
			else
				UI::SET_BLIP_DISPLAY(DocksBlip, 0);
			UI::SET_BLIP_AS_SHORT_RANGE(DocksBlip, true);
			UI::SET_BLIP_CATEGORY(DocksBlip, 1);
		}

		if (!UI::DOES_BLIP_EXIST(FlyToLCBlip))
		{
			if (worldtravel::IsLosSantos())
				FlyToLCBlip = UI::ADD_BLIP_FOR_COORD(FlyToLCFromLSCoords[0], FlyToLCFromLSCoords[1], 1200.0f);

			else if (worldtravel::IsCayoPerico())
				FlyToLCBlip = UI::ADD_BLIP_FOR_COORD(FlyToLCFromCPCoords[0], FlyToLCFromCPCoords[1], 1200.0f);

			else
				FlyToLCBlip = UI::ADD_BLIP_FOR_COORD(0.0f, 0.0f, 1200.0f);

			UI::SET_BLIP_SCALE(FlyToLCBlip, 1);
			UI::SET_BLIP_SPRITE(FlyToLCBlip, 307);
			UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const_cast<char*>(Settings::GetLang("Lng31", "Fly to Liberty City").c_str()));
			UI::END_TEXT_COMMAND_SET_BLIP_NAME(FlyToLCBlip);
			UI::SET_BLIP_COLOUR(FlyToLCBlip, 0);
			UI::SET_BLIP_FLASHES(FlyToLCBlip, false);
			flyToLCBlipVisible = false;
			UI::SET_BLIP_DISPLAY(FlyToLCBlip, 0);
			UI::SET_BLIP_AS_SHORT_RANGE(FlyToLCBlip, false);
			UI::SET_BLIP_CATEGORY(FlyToLCBlip, 1);
		}

		if (!UI::DOES_BLIP_EXIST(FlyToLSBlip))
		{
			if (worldtravel::IsLibertyCity())
				FlyToLSBlip = UI::ADD_BLIP_FOR_COORD(FlyToLSFromLCCoords[0], FlyToLSFromLCCoords[1], 1200.0f);

			else if (worldtravel::IsCayoPerico())
				FlyToLSBlip = UI::ADD_BLIP_FOR_COORD(FlyToLSFromCPCoords[0], FlyToLSFromCPCoords[1], 1200.0f);

			else
				FlyToLSBlip = UI::ADD_BLIP_FOR_COORD(0.0f, 0.0f, 1200.0f);

			UI::SET_BLIP_SCALE(FlyToLSBlip, 1);
			UI::SET_BLIP_SPRITE(FlyToLSBlip, 307);
			UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const_cast<char*>(Settings::GetLang("Lng32", "Fly to Los Santos").c_str()));
			UI::END_TEXT_COMMAND_SET_BLIP_NAME(FlyToLSBlip);
			UI::SET_BLIP_COLOUR(FlyToLSBlip, 0);
			UI::SET_BLIP_FLASHES(FlyToLSBlip, false);
			flyToLSBlipVisible = false;
			UI::SET_BLIP_DISPLAY(FlyToLSBlip, 0);
			UI::SET_BLIP_AS_SHORT_RANGE(FlyToLSBlip, false);
			UI::SET_BLIP_CATEGORY(FlyToLSBlip, 1);
		}

		if (!UI::DOES_BLIP_EXIST(FlyToCPBlip))
		{
			if (worldtravel::IsLosSantos())
				FlyToCPBlip = UI::ADD_BLIP_FOR_COORD(FlyToCPFromLSCoords[0], FlyToCPFromLSCoords[1], 1200.0f);

			else if (worldtravel::IsLibertyCity())
				FlyToCPBlip = UI::ADD_BLIP_FOR_COORD(FlyToCPFromLCCoords[0], FlyToCPFromLCCoords[1], 1200.0f);

			else
				FlyToCPBlip = UI::ADD_BLIP_FOR_COORD(0.0f, 0.0f, 1200.0f);

			UI::SET_BLIP_SCALE(FlyToCPBlip, 1);
			UI::SET_BLIP_SPRITE(FlyToCPBlip, 307);
			UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
			UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const_cast<char*>(Settings::GetLang("Lng33", "Fly to Cayo Perico").c_str()));
			UI::END_TEXT_COMMAND_SET_BLIP_NAME(FlyToCPBlip);
			UI::SET_BLIP_COLOUR(FlyToCPBlip, 0);
			UI::SET_BLIP_FLASHES(FlyToCPBlip, false);
			flyToCPBlipVisible = false;
			UI::SET_BLIP_DISPLAY(FlyToCPBlip, 0);
			UI::SET_BLIP_AS_SHORT_RANGE(FlyToCPBlip, false);
			UI::SET_BLIP_CATEGORY(FlyToCPBlip, 1);
		}
	}

	// Fix to force disable IPLs in MP
	void KeepLosSantosIplsDisabled()
	{
		if (!worldtravel::IsLosSantos())
		{
			if (worldtravel::MpMap::IsMPMapActive())
			{
				for (int i = 0; i < santosMpForcedIpls.size(); i++)
				{
					if (STREAMING::IS_IPL_ACTIVE((char*)santosMpForcedIpls[i].c_str()))
						STREAMING::REMOVE_IPL((char*)santosMpForcedIpls[i].c_str());
				}
			}
		}
	}

	// Controls time of day
	void NetworkClockController(bool forward)
	{
		int hours = TIME::GET_CLOCK_HOURS();
		int minutes = TIME::GET_CLOCK_MINUTES();
		int seconds = TIME::GET_CLOCK_SECONDS();
		if (forward)
		{
			hours += 12;
			if (hours > 23)
			{
				hours -= 24;
			}
		}
		else
		{
			hours -= 12;
			if (hours < 0)
			{
				hours += 24;
			}
		}
		NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(hours, minutes, seconds);
	}

	// Fix for Cayo Perico Island Hopper not functioning correctly.
	void CayoPericoIslandHopperHelper()
	{
		if (!worldtravel::IsCayoPerico())
		{
			if (!hasCayoLoadedExternally && STREAMING::IS_IPL_ACTIVE("h4_islandairstrip_slod"))
			{
				for (int i = 0; i < cayoInteriors.size(); i++)
				{
					if (!STREAMING::IS_IPL_ACTIVE(const_cast<char*>(cayoInteriors[i].c_str())))
					{
						STREAMING::REQUEST_IPL(const_cast<char*>(cayoInteriors[i].c_str()));
					}
				}
				for (int i = 0; i < cayoInstancePlacement.size(); i++)
				{
					if (!STREAMING::IS_IPL_ACTIVE(const_cast<char*>(cayoInstancePlacement[i].c_str())))
					{
						STREAMING::REQUEST_IPL(const_cast<char*>(cayoInstancePlacement[i].c_str()));
					}
				}
				for (int i = 0; i < cayoOccl.size(); i++)
				{
					if (!STREAMING::IS_IPL_ACTIVE(const_cast<char*>(cayoOccl[i].c_str())))
					{
						STREAMING::REQUEST_IPL(const_cast<char*>(cayoOccl[i].c_str()));
					}
				}
				hasCayoLoadedExternally = true;
			}
			else if (hasCayoLoadedExternally && !STREAMING::IS_IPL_ACTIVE("h4_islandairstrip_slod"))
			{
				for (int i = 0; i < cayoInteriors.size(); i++)
				{
					if (STREAMING::IS_IPL_ACTIVE(const_cast<char*>(cayoInteriors[i].c_str())))
					{
						STREAMING::REMOVE_IPL(const_cast<char*>(cayoInteriors[i].c_str()));
					}
				}
				for (int i = 0; i < cayoInstancePlacement.size(); i++)
				{
					if (STREAMING::IS_IPL_ACTIVE(const_cast<char*>(cayoInstancePlacement[i].c_str())))
					{
						STREAMING::REMOVE_IPL(const_cast<char*>(cayoInstancePlacement[i].c_str()));
					}
				}
				for (int i = 0; i < cayoOccl.size(); i++)
				{
					if (STREAMING::IS_IPL_ACTIVE(const_cast<char*>(cayoOccl[i].c_str())))
					{
						STREAMING::REMOVE_IPL(const_cast<char*>(cayoOccl[i].c_str()));
					}
				}
				hasCayoLoadedExternally = false;
			}
		}
	}

	// Fix for interiors being flooded
	void InteriorWaterFix()
	{
		int playerInteriorId = INTERIOR::GET_INTERIOR_FROM_ENTITY(player);
		if (!worldtravel::IsLosSantos())
		{
			if ((playerInteriorId == INTERIOR::GET_INTERIOR_AT_COORDS(gr_grdlc_int_01[0], gr_grdlc_int_01[1], gr_grdlc_int_01[2]) ||
				playerInteriorId == INTERIOR::GET_INTERIOR_AT_COORDS(xm_x17dlc_int_01[0], xm_x17dlc_int_01[1], xm_x17dlc_int_01[2]) ||
				playerInteriorId == INTERIOR::GET_INTERIOR_AT_COORDS(xm_x17dlc_int_sub[0], xm_x17dlc_int_sub[1], xm_x17dlc_int_sub[2]) ||
				playerInteriorId == INTERIOR::GET_INTERIOR_AT_COORDS(ba_dlc_int_03_ba[0], ba_dlc_int_03_ba[1], ba_dlc_int_03_ba[2]) ||
				playerInteriorId == INTERIOR::GET_INTERIOR_AT_COORDS(h4_interior_0_int_sub[0], h4_interior_0_int_sub[1], h4_interior_0_int_sub[2]) ||
				playerInteriorId == INTERIOR::GET_INTERIOR_AT_COORDS(dlc_int_01_xm3[0], dlc_int_01_xm3[1], dlc_int_01_xm3[2])) &&
				playerInteriorId != 0 &&
				!tempWaterSwapActive)
			{
				STREAMING::LOAD_GLOBAL_WATER_FILE(0);
				PED::CLEAR_PED_WETNESS(playerPed);
				tempWaterSwapActive = true;
			}
			else if (tempWaterSwapActive && playerInteriorId == 0)
			{
				//WAIT(1000);
				STREAMING::LOAD_GLOBAL_WATER_FILE(2);
				tempWaterSwapActive = false;
				if (NETWORK::NETWORK_IS_IN_SESSION())
				{
					NETWORK::NETWORK_SET_PROPERTY_ID(100);
				}
			}
		}
	}


	// handles blip visibility for all maps
	void BlipVisibilityController()
	{
		Entity player = playerPed;
		if (worldtravel::IsLosSantos())
		{
			if (!airportBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(AirportBlip, 2);
				airportBlipVisible = true;
			}
			if (!docksBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(DocksBlip, 2);
				docksBlipVisible = true;
			}
			if (PED::IS_PED_IN_FLYING_VEHICLE(player))
			{
				if (!flyToLCBlipVisible)
				{
					UI::SET_BLIP_DISPLAY(FlyToLCBlip, 2);
					flyToLCBlipVisible = true;
				}
				if (!flyToCPBlipVisible)
				{
					if (!NETWORK::NETWORK_IS_IN_SESSION())
					{
						UI::SET_BLIP_DISPLAY(FlyToCPBlip, 2);
						flyToCPBlipVisible = true;
					}
				}
				if (!airportBlipFarDist)
				{
					UI::SET_BLIP_AS_SHORT_RANGE(AirportBlip, false);
					airportBlipFarDist = true;
				}
			}
			else
			{
				if (flyToLCBlipVisible)
				{
					UI::SET_BLIP_DISPLAY(FlyToLCBlip, 0);
					flyToLCBlipVisible = false;
				}
				if (flyToCPBlipVisible)
				{
					UI::SET_BLIP_DISPLAY(FlyToCPBlip, 0);
					flyToCPBlipVisible = false;
				}
				if (airportBlipFarDist)
				{
					UI::SET_BLIP_AS_SHORT_RANGE(AirportBlip, true);
					airportBlipFarDist = false;
				}
			}
			if (flyToLSBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(FlyToLSBlip, 0);
				flyToLSBlipVisible = false;
			}
		}
		else if (worldtravel::IsLibertyCity())
		{
			if (!airportBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(AirportBlip, 2);
				airportBlipVisible = true;
			}
			if (!docksBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(DocksBlip, 2);
				docksBlipVisible = true;
			}
			if (PED::IS_PED_IN_FLYING_VEHICLE(player))
			{
				if (!flyToLSBlipVisible)
				{
					UI::SET_BLIP_DISPLAY(FlyToLSBlip, 2);
					flyToLSBlipVisible = true;
				}
				if (!flyToCPBlipVisible)
				{
					if (!NETWORK::NETWORK_IS_IN_SESSION())
					{
						UI::SET_BLIP_DISPLAY(FlyToCPBlip, 2);
						flyToCPBlipVisible = true;
					}
				}
				if (!airportBlipFarDist)
				{
					UI::SET_BLIP_AS_SHORT_RANGE(AirportBlip, false);
					airportBlipFarDist = true;
				}
			}
			else
			{
				if (flyToLSBlipVisible)
				{
					UI::SET_BLIP_DISPLAY(FlyToLSBlip, 0);
					flyToLSBlipVisible = false;
				}
				if (flyToCPBlipVisible)
				{
					UI::SET_BLIP_DISPLAY(FlyToCPBlip, 0);
					flyToCPBlipVisible = false;
				}
				if (airportBlipFarDist)
				{
					UI::SET_BLIP_AS_SHORT_RANGE(AirportBlip, true);
					airportBlipFarDist = false;
				}
			}
			if (flyToLCBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(FlyToLCBlip, 0);
				flyToLCBlipVisible = false;
			}
		}
		else if (worldtravel::IsNorthYankton())
		{
			if (!airportBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(AirportBlip, 2);
				airportBlipVisible = true;
			}
			if (docksBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(DocksBlip, 0);
				docksBlipVisible = false;
			}
			if (flyToLSBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(FlyToLSBlip, 0);
				flyToLSBlipVisible = false;
			}
			if (flyToLCBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(FlyToLCBlip, 0);
				flyToLCBlipVisible = false;
			}
			if (flyToCPBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(FlyToCPBlip, 0);
				flyToCPBlipVisible = false;
			}
			if (!airportBlipFarDist)
			{
				UI::SET_BLIP_AS_SHORT_RANGE(AirportBlip, false);
				airportBlipFarDist = true;
			}
		}
		else if (worldtravel::IsCayoPerico())
		{
			if (!airportBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(AirportBlip, 2);
				airportBlipVisible = true;
			}
			if (docksBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(DocksBlip, 0);
				docksBlipVisible = false;
			}
			if (PED::IS_PED_IN_FLYING_VEHICLE(player))
			{
				if (!flyToLSBlipVisible)
				{
					UI::SET_BLIP_DISPLAY(FlyToLSBlip, 2);
					flyToLSBlipVisible = true;
				}
				if (!flyToLCBlipVisible)
				{
					UI::SET_BLIP_DISPLAY(FlyToLCBlip, 2);
					flyToLCBlipVisible = true;
				}
				if (!airportBlipFarDist)
				{
					UI::SET_BLIP_AS_SHORT_RANGE(AirportBlip, false);
					airportBlipFarDist = true;
				}
			}
			else
			{
				if (flyToLSBlipVisible)
				{
					UI::SET_BLIP_DISPLAY(FlyToLSBlip, 0);
					flyToLSBlipVisible = false;
				}
				if (flyToLCBlipVisible)
				{
					UI::SET_BLIP_DISPLAY(FlyToLCBlip, 0);
					flyToLCBlipVisible = false;
				}
				if (airportBlipFarDist)
				{
					UI::SET_BLIP_AS_SHORT_RANGE(AirportBlip, true);
					airportBlipFarDist = false;
				}
			}
			if (flyToCPBlipVisible)
			{
				UI::SET_BLIP_DISPLAY(FlyToCPBlip, 0);
				flyToCPBlipVisible = false;
			}
		}
	}


	void SetBlipsLocation(int blipLocationID)
	{
		if (blipLocationID == 0)
		{
			UI::SET_BLIP_COORDS(AirportBlip, LosSantosIntAirport[0], LosSantosIntAirport[1], LosSantosIntAirport[2]);
			UI::SET_BLIP_COORDS(DocksBlip, LosSantosDocks[0], LosSantosDocks[1], LosSantosDocks[2]);
			UI::SET_BLIP_COORDS(FlyToLCBlip, FlyToLCFromLSCoords[0], FlyToLCFromLSCoords[1], 1200.0f);
			UI::SET_BLIP_COORDS(FlyToCPBlip, FlyToCPFromLSCoords[0], FlyToCPFromLSCoords[1], 1200.0f);
			UI::SET_BLIP_COORDS(FlyToLSBlip, 0.0f, 0.0f, 1200.0f);
		}
		else if (blipLocationID == 1)
		{
			UI::SET_BLIP_COORDS(AirportBlip, FrancisIntAirport[0], FrancisIntAirport[1], FrancisIntAirport[2]);
			UI::SET_BLIP_COORDS(DocksBlip, LibertyCityDocks[0], LibertyCityDocks[1], LibertyCityDocks[2]);
			UI::SET_BLIP_COORDS(FlyToLSBlip, FlyToLSFromLCCoords[0], FlyToLSFromLCCoords[1], 1200.0f);
			UI::SET_BLIP_COORDS(FlyToCPBlip, FlyToCPFromLCCoords[0], FlyToCPFromLCCoords[1], 1200.0f);
			UI::SET_BLIP_COORDS(FlyToLCBlip, 0.0f, 0.0f, 1200.0f);
		}
		else if (blipLocationID == 2)
		{
			UI::SET_BLIP_COORDS(AirportBlip, NorthYanktonAirport[0], NorthYanktonAirport[1], NorthYanktonAirport[2]);
			UI::SET_BLIP_COORDS(DocksBlip, 0.0f, 0.0f, 0.0f);
			UI::SET_BLIP_COORDS(FlyToLSBlip, 0.0f, 0.0f, 1200.0f);
			UI::SET_BLIP_COORDS(FlyToCPBlip, 0.0f, 0.0f, 1200.0f);
			UI::SET_BLIP_COORDS(FlyToLCBlip, 0.0f, 0.0f, 1200.0f);
		}
		else if (blipLocationID == 3)
		{
			UI::SET_BLIP_COORDS(AirportBlip, CayoPericoAirport[0], CayoPericoAirport[1], CayoPericoAirport[2]);
			UI::SET_BLIP_COORDS(DocksBlip, 0.0f, 0.0f, 0.0f);
			UI::SET_BLIP_COORDS(FlyToLSBlip, FlyToLSFromCPCoords[0], FlyToLSFromCPCoords[1], 1200.0f);
			UI::SET_BLIP_COORDS(FlyToLCBlip, FlyToLCFromCPCoords[0], FlyToLCFromCPCoords[1], 1200.0f);
			UI::SET_BLIP_COORDS(FlyToCPBlip, 0.0f, 0.0f, 1200.0f);
		}
	}



	// Blocks NPCs from spawning in unloaded areas
	void NpcSpawnBlocker()
	{
		//block traffic and peds around Los Santos if it is not loaded
		if (!STREAMING::IS_IPL_ACTIVE("cs5_lod"))
		{
			if (!ENTITY::IS_ENTITY_IN_AREA(playerPed, 2600.0f, -6600.0f, -10000.0f, 8300.0f, -2500.0f, 10000.0f, false, false, false) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, 3000.0f, -2500.0f, -10000.0f, 8300.0f, -1700.0f, 10000.0f, false, false, false) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, 3400.0f, -1700.0f, -10000.0f, 8300.0f, -900.0f, 10000.0f, false, false, false))
			{
				//worldtravel::HelpText::DisplayHelpText("NPCs blocked in this area when Los Santos is disabled.");
				VEHICLE::SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				VEHICLE::SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				VEHICLE::SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				PED::SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				PED::SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f, 0.0f);
			}
		}

		//block traffic and peds around North Yankton/Cayo Perico if they're both not loaded
		if (!STREAMING::IS_IPL_ACTIVE("prologuerd") && !STREAMING::IS_IPL_ACTIVE("h4_islandx"))
		{
			if (ENTITY::IS_ENTITY_IN_AREA(playerPed, 2200.0f, -5600.0f, -10000.0f, 4100.0f, -4000.0f, 10000.0f, false, false, false) ||
				ENTITY::IS_ENTITY_IN_AREA(playerPed, 4100.0f, -5600.0f, -10000.0f, 6500.0f, -4600.0f, 10000.0f, false, false, false))
			{
				//worldtravel::HelpText::DisplayHelpText("NPCs blocked in this area when North Yankton and Cayo Perico are disabled.");
				VEHICLE::SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				VEHICLE::SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				VEHICLE::SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				PED::SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				PED::SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f, 0.0f);
			}
		}

		//block traffic and peds around Liberty City if it's not loaded
		if (!STREAMING::IS_IPL_ACTIVE("manhat06_slod"))
		{
			if (ENTITY::IS_ENTITY_IN_AREA(playerPed, 2600.0f, -4200.0f, -10000.0f, 8300.0f, -2500.0f, 10000.0f, false, false, false) ||
				ENTITY::IS_ENTITY_IN_AREA(playerPed, 3000.0f, -2500.0f, -10000.0f, 8300.0f, -1700.0f, 10000.0f, false, false, false) ||
				ENTITY::IS_ENTITY_IN_AREA(playerPed, 3400.0f, -1700.0f, -10000.0f, 8300.0f, -900.0f, 10000.0f, false, false, false))
			{
				//worldtravel::HelpText::DisplayHelpText("NPCs blocked in this area when Liberty City is disabled.");
				VEHICLE::SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				VEHICLE::SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				VEHICLE::SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				PED::SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
				PED::SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f, 0.0f);
			}
		}
	}

	/////////////////////////////////////////////////////////
	////             Loading Helper Functions            ////
	/////////////////////////////////////////////////////////


	void requestIpls(const std::vector<std::string>& iplList)
	{
		for (const auto& ipl : iplList)
		{
			STREAMING::REQUEST_IPL(const_cast<char*>(ipl.c_str()));
		}
	}

	void requestIpls(const std::vector<std::string>& iplList, std::vector<bool>& iplState)
	{
		int index = 0;
		for (const auto& ipl : iplList)
		{
			if (iplState[index])
				STREAMING::REQUEST_IPL(const_cast<char*>(ipl.c_str()));
			index++;
		}
	}

	void enableScenarios(const std::vector<std::string>& scenarioList)
	{
		for (const auto& scenario : scenarioList)
		{
			AI::SET_SCENARIO_GROUP_ENABLED(const_cast<char*>(scenario.c_str()), true);
		}
	}

	void enableScenarios(const std::vector<std::string>& scenarioList, std::vector<bool>& scenarioState)
	{
		int index = 0;
		for (const auto& scenario : scenarioList)
		{
			if (scenarioState[index])
				AI::SET_SCENARIO_GROUP_ENABLED(const_cast<char*>(scenario.c_str()), true);
			index++;
		}
	}

	void enableZones(const std::vector<std::string>& zoneList)
	{
		for (const auto& zoneName : zoneList)
		{
			int zoneId = ZONE::GET_ZONE_FROM_NAME_ID(const_cast<char*>(zoneName.c_str()));
			ZONE::SET_ZONE_ENABLED(zoneId, true);
		}
	}

	void enableAmbientZones(const std::vector<std::string>& ambientZoneList)
	{
		for (const auto& ambientZoneName : ambientZoneList)
		{
			AUDIO::SET_AMBIENT_ZONE_STATE_PERSISTENT(const_cast<char*>(ambientZoneName.c_str()), true, true);
		}
	}

	void enableAmbientZones(const std::vector<std::string>& ambientZoneList, std::vector<bool>& ambientZoneState)
	{
		int index = 0;
		for (const auto& ambientZoneName : ambientZoneList)
		{
			if (ambientZoneState[index])
				AUDIO::SET_AMBIENT_ZONE_STATE_PERSISTENT(const_cast<char*>(ambientZoneName.c_str()), true, true);
			index++;
		}
	}

	void unlockModels(const std::vector<std::string>& modelList, bool isPed = true)
	{
		for (const auto& model : modelList)
		{
			if (isPed)
				PED::SET_PED_MODEL_IS_SUPPRESSED(GAMEPLAY::GET_HASH_KEY(const_cast<char*>(model.c_str())), false);
			else
				VEHICLE::SET_VEHICLE_MODEL_IS_SUPPRESSED(GAMEPLAY::GET_HASH_KEY(const_cast<char*>(model.c_str())), false);
		}
	}

	void unlockRadioStations(const std::vector<std::string>& radioList)
	{
		for (const auto& radioStation : radioList)
		{
			AUDIO::LOCK_RADIO_STATION(const_cast<char*>(radioStation.c_str()), false);
		}
	}

	void setTrainTracksAndFrequency(const std::vector<int>& trackIds, int frequency = 120000)
	{
		for (int trackId : trackIds)
		{
			VEHICLE::SWITCH_TRAIN_TRACK(trackId, true);
			VEHICLE::SET_TRAIN_TRACK_SPAWN_FREQUENCY(trackId, frequency);
		}
	}

	void loadMapLiberty(const std::vector<std::string>& scenarios,
		const std::vector<std::string>& zones,
		const std::vector<std::string>& ambientZones,
		const std::vector<std::string>& pedModels,
		const std::vector<std::string>& vehicleModels,
		const std::vector<std::string>& radioStations,
		const std::vector<int>& trainTracks,
		int trainFrequency = 120000)
	{
		enableScenarios(scenarios);
		enableZones(zones);
		enableAmbientZones(ambientZones);
		unlockModels(pedModels, true);
		unlockModels(vehicleModels, false);
		unlockRadioStations(radioStations);
		setTrainTracksAndFrequency(trainTracks, trainFrequency);
	}

	void loadMapSantos(const std::vector<std::string>& scenarios,
		const std::vector<std::string>& zones,
		const std::vector<std::string>& ambientZones,
		const std::vector<std::string>& pedModels,
		const std::vector<std::string>& vehicleModels,
		const std::vector<std::string>& radioStations,
		const std::vector<int>& trainTracks,
		std::vector<bool>& scenarioState,
		std::vector<bool>& ambientZoneState,
		int trainFrequency = 120000)
	{
		enableScenarios(scenarios, scenarioState);
		enableZones(zones);
		enableAmbientZones(ambientZones, ambientZoneState);
		unlockModels(pedModels, true);
		unlockModels(vehicleModels, false);
		unlockRadioStations(radioStations);
		setTrainTracksAndFrequency(trainTracks, trainFrequency);
	}


	/////////////////////////////////////////////////////////
	////             Loading Helper Functions            ////
	/////////////////////////////////////////////////////////

	// Load the Cayo Perico Island
	void loadCayo()
	{
		/*if (!worldtravel::MpMap::IsMPMapActive())
		{
			DLC2::_LOAD_MP_DLC_MAPS();
			while (DLC2::GET_IS_LOADING_SCREEN_ACTIVE())
			{
				WAIT(0); // avoid freezing the game
			}
			WAIT(1000); // 1 Second
			requestIpls(santosMpIpls);
		}*/
		if (!NETWORK::NETWORK_IS_IN_SESSION())
		{
			STREAMING::SET_ISLAND_ENABLED(const_cast<char*>("HeistIsland"), true);
		}

		enableScenarios(cayoScenarios);
		requestIpls(cayoMpIpls);
		enableZones(cayoZones);
		enableAmbientZones(cayoAmbientZones);

		UI::SET_USE_ISLAND_MAP(true);

		int weatherID = std::rand() % weatherTypes.size();
		if (!NETWORK::NETWORK_IS_IN_SESSION())
		{
			GAMEPLAY::SET_WEATHER_TYPE_NOW(const_cast<char*>(weatherTypes[weatherID].c_str()));
		}

		worldtravel::PathNodeState::SetPathNodeState(2);
		STREAMING::LOAD_GLOBAL_WATER_FILE(1);
		SetBlipsLocation(3);
	}

	// Load Liberty City
	void loadLiberty()
	{
		//STREAMING::SET_ISLAND_ENABLED(const_cast<char*>("LibertyCity"), true);
		requestIpls(libertyIpls);
		// Request MP or SP IPLs based on session status
		if (worldtravel::MpMap::IsMPMapActive())
		{
			requestIpls(libertyMpIpls);
		}
		else
		{
			requestIpls(libertySpIpls);
		}
		if (Settings::EnableLibertyCityLODLights)
		{
			requestIpls(libertyLODLightIpls);
		}

		loadMapLiberty(libertyScenarios, libertyZones, libertyAmbientZones, libertyPeds, libertyVehicles, libertyRadio, { 12, 13, 14, 16, 17, 18 });
		
		int weatherID = std::rand() % weatherTypes.size();
		if (!NETWORK::NETWORK_IS_IN_SESSION())
		{
			GAMEPLAY::SET_WEATHER_TYPE_NOW(const_cast<char*>(weatherTypes[weatherID].c_str()));
		}
		
		worldtravel::PathNodeState::SetPathNodeState(1);
		GRAPHICS::DISABLE_VEHICLE_DISTANTLIGHTS(true);
		STREAMING::LOAD_GLOBAL_WATER_FILE(2);
		SetBlipsLocation(1);
	}

	// Load Los Santos
	void loadSantos()
	{
		patches::farlods::SetVisible(true);

		requestIpls(santosIpls, santosIplState);
		// Request MP or SP IPLs based on session status
		if (worldtravel::MpMap::IsMPMapActive())
		{
			requestIpls(santosMpIpls, santosMpIplState);
		}
		else
		{
			requestIpls(santosSpIpls, santosSpIplState);
		}

		requestIpls(modIpls);

		loadMapSantos(santosScenarios, santosZones, santosAmbientZones, santosPeds, santosVehicles, santosRadio, { 0, 3 }, santosScenariosState, santosAmbientZonesState);
		worldtravel::PathNodeState::SetPathNodeState(0);
		GRAPHICS::DISABLE_VEHICLE_DISTANTLIGHTS(false);
		STREAMING::LOAD_GLOBAL_WATER_FILE(0);
		SetBlipsLocation(0);
		STREAMING::_SET_MAPDATACULLBOX_ENABLED((char*)"HeistIsland", false);

		// if in singleplayer
		if (!NETWORK::NETWORK_IS_IN_SESSION())
		{
			UI::_SET_MINIMAP_REVEALED(false);
			for (int i = 0; i < 5; i++)
			{
				GAMEPLAY::DISABLE_HOSPITAL_RESTART(i, false);
			}
			for (int i = 0; i < 7; i++)
			{
				GAMEPLAY::DISABLE_POLICE_RESTART(i, false);
			}
		}

		int weatherID = std::rand() % weatherTypes.size();
		// if in multiplayer
		if (!NETWORK::NETWORK_IS_IN_SESSION())
		{
			GAMEPLAY::SET_WEATHER_TYPE_NOW(const_cast<char*>(weatherTypes[weatherID].c_str()));
		}

		if (!NETWORK::NETWORK_IS_IN_SESSION())
		{
			// enable the following scripts in singleplayer that we disabled when not in LS
			RequestScript("blip_controller", 1424);        // responsible for blips
			RequestScript("forsalesigns", 1424);           // responsible for property for sale signs
			RequestScript("respawn_controller", 1424);     // responsible for respawning the player
			RequestScript("restrictedareas", 1424);        // responsible for restricted areas
			RequestScript("vehicle_gen_controller", 1424); // responsible for player businesses / vehicle blips
		}
	}

	// Load North Yankton
	void loadYankton()
	{
		requestIpls(yanktonIpls);
		enableZones(yanktonZones);
		enableAmbientZones(yanktonAmbientZones);
		UI::SET_MINIMAP_IN_PROLOGUE(true);

		int weatherID = std::rand() % yanktonWeatherTypes.size();
		// if in multiplayer
		if (NETWORK::NETWORK_IS_IN_SESSION())
		{
			GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST(const_cast<char*>(yanktonWeatherTypes[weatherID].c_str()));
		}
		else
		{
			GAMEPLAY::SET_WEATHER_TYPE_PERSIST(const_cast<char*>(yanktonWeatherTypes[weatherID].c_str()));
		}
		PATHFIND::SET_ROADS_IN_ANGLED_AREA(5526.24f, -5137.23f, 61.78925f, 3679.327f, -4973.879f, 125.0828f, 192, false, true, true);
		PATHFIND::SET_ROADS_IN_ANGLED_AREA(3691.211f, -4941.24f, 94.59368f, 3511.115f, -4869.191f, 126.7621f, 16, false, true, true);
		PATHFIND::SET_ROADS_IN_ANGLED_AREA(3510.004f, -4865.81f, 94.69557f, 3204.424f, -4833.817f, 126.8152f, 16, false, true, true);
		PATHFIND::SET_ROADS_IN_ANGLED_AREA(3186.534f, -4832.798f, 109.8148f, 3202.187f, -4833.993f, 114.815f, 16, false, true, true);
		GAMEPLAY::SET_OVERRIDE_WEATHER(const_cast<char*>(yanktonWeatherTypes[weatherID].c_str()));
		worldtravel::PathNodeState::SetPathNodeState(1);
		SetBlipsLocation(2);
	}

	/////////////////////////////////////////////////////////
	////           Unloading Helper Functions            ////
	/////////////////////////////////////////////////////////

	void removeIpls(const std::vector<std::string>& ipls)
	{
		for (const auto& ipl : ipls)
		{
			STREAMING::REMOVE_IPL(const_cast<char*>(ipl.c_str()));
		}
	}

	void removeIpls(const std::vector<std::string>& ipls, std::vector<bool>& iplState)
	{
		for (const auto& ipl : ipls)
		{
			iplState.push_back(STREAMING::IS_IPL_ACTIVE(const_cast<char*>(ipl.c_str())));
			STREAMING::REMOVE_IPL(const_cast<char*>(ipl.c_str()));
		}
	}

	void disableScenarios(const std::vector<std::string>& scenarios)
	{
		for (const auto& scenario : scenarios)
		{
			AI::SET_SCENARIO_GROUP_ENABLED(const_cast<char*>(scenario.c_str()), false);
		}
	}

	void disableScenarios(const std::vector<std::string>& scenarios, std::vector<bool>& scenarioState)
	{
		for (const auto& scenario : scenarios)
		{
			scenarioState.push_back(AI::IS_SCENARIO_GROUP_ENABLED(const_cast<char*>(scenario.c_str())));
			AI::SET_SCENARIO_GROUP_ENABLED(const_cast<char*>(scenario.c_str()), false);
		}
	}

	void disableZones(const std::vector<std::string>& zones)
	{
		for (const auto& zoneName : zones)
		{
			int zoneId = ZONE::GET_ZONE_FROM_NAME_ID(const_cast<char*>(zoneName.c_str()));
			ZONE::SET_ZONE_ENABLED(zoneId, false);
		}
	}

	void disableAmbientZones(const std::vector<std::string>& ambientZoneList)
	{
		for (const auto& ambientZoneName : ambientZoneList)
		{
			AUDIO::SET_AMBIENT_ZONE_STATE_PERSISTENT(const_cast<char*>(ambientZoneName.c_str()), false, true);
		}
	}

	void disableAmbientZones(const std::vector<std::string>& ambientZoneList, std::vector<bool>& ambientZoneState)
	{
		int index = 0;
		for (const auto& ambientZoneName : ambientZoneList)
		{
			ambientZoneState.push_back(AUDIO::IS_AMBIENT_ZONE_ENABLED(const_cast<char*>(ambientZoneName.c_str())));
			AUDIO::SET_AMBIENT_ZONE_STATE_PERSISTENT(const_cast<char*>(ambientZoneName.c_str()), false, true);
			index++;
		}
	}

	void suppressModels(const std::vector<std::string>& models, bool isVehicle = false)
	{
		for (const auto& model : models)
		{
			auto modelHash = GAMEPLAY::GET_HASH_KEY(const_cast<char*>(model.c_str()));
			if (isVehicle)
				VEHICLE::SET_VEHICLE_MODEL_IS_SUPPRESSED(modelHash, true);
			else
				PED::SET_PED_MODEL_IS_SUPPRESSED(modelHash, true);
		}
	}

	void lockRadioStations(const std::vector<std::string>& radioStations)
	{
		for (const auto& station : radioStations)
		{
			AUDIO::LOCK_RADIO_STATION(const_cast<char*>(station.c_str()), true);
		}
	}

	void switchTrainTracks(const std::vector<int>& trackIds, bool state)
	{
		for (const auto& trackId : trackIds)
		{
			VEHICLE::SWITCH_TRAIN_TRACK(trackId, state);
		}
	}

	void unloadMapLiberty(const std::vector<std::string>& scenarios,
		const std::vector<std::string>& zones, const std::vector<std::string>& ambientZones, const std::vector<std::string>& peds,
		const std::vector<std::string>& vehicles, const std::vector<std::string>& radios,
		const std::vector<int>& trainTracks)
	{
		disableScenarios(scenarios);
		disableZones(zones);
		disableAmbientZones(ambientZones);
		suppressModels(peds);
		suppressModels(vehicles, true);
		lockRadioStations(radios);
		switchTrainTracks(trainTracks, false);
	}

	void unloadMapSantos(const std::vector<std::string>& scenarios,
		const std::vector<std::string>& zones, const std::vector<std::string>& ambientZones, const std::vector<std::string>& peds,
		const std::vector<std::string>& vehicles, const std::vector<std::string>& radios,
		const std::vector<int>& trainTracks, std::vector<bool>& scenarioState, std::vector<bool>& ambientZoneState)
	{
		disableScenarios(scenarios, scenarioState);
		disableZones(zones);
		disableAmbientZones(ambientZones, ambientZoneState);
		suppressModels(peds);
		suppressModels(vehicles, true);
		lockRadioStations(radios);
		switchTrainTracks(trainTracks, false);
	}

	/////////////////////////////////////////////////////////
	////           Unloading Helper Functions            ////
	/////////////////////////////////////////////////////////

	// Unload the Cayo Perico Island
	void unloadCayo()
	{
		removeIpls(cayoMpIpls);
		/*if (!worldtravel::MpMap::IsMPMapActive())
		{
			DLC2::_UNLOAD_MP_DLC_MAPS();
			while (DLC2::GET_IS_LOADING_SCREEN_ACTIVE())
			{
				WAIT(0); // avoid freezing the game
			}

			removeIpls(santosIpls);
			removeIpls(santosSpIpls);
			WAIT(1000); // 1 Second
		}*/

		// if in singleplayer
		if (!NETWORK::NETWORK_IS_IN_SESSION())
		{
			STREAMING::SET_ISLAND_ENABLED("HeistIsland", false);
		}
		WAIT(1000);

		disableScenarios(cayoScenarios);
		disableZones(cayoZones);
		disableAmbientZones(cayoAmbientZones);

		UI::SET_USE_ISLAND_MAP(false);
	}

	// Unload Liberty City
	void unloadLiberty()
	{
		//STREAMING::SET_ISLAND_ENABLED(const_cast<char*>("LibertyCity"), false);
		WAIT(1000);

		removeIpls(libertyIpls);
		// Remove MP or SP IPLs based on session status
		if (worldtravel::MpMap::IsMPMapActive())
			removeIpls(libertyMpIpls);
		else
			removeIpls(libertySpIpls);


		if (Settings::EnableLibertyCityLODLights)
		{
			removeIpls(libertyLODLightIpls);
		}

		unloadMapLiberty(libertyScenarios, libertyZones, libertyAmbientZones, libertyPeds, libertyVehicles, libertyRadio, { 12, 13, 14, 16, 17, 18 });
	}

	// Unload Los Santos
	void unloadSantos()
	{
		patches::farlods::SetVisible(false);

		removeIpls(santosIpls, santosIplState);
		// Remove MP or SP IPLs based on session status
		if (worldtravel::MpMap::IsMPMapActive())
			removeIpls(santosMpIpls, santosMpIplState);
		else
			removeIpls(santosSpIpls, santosSpIplState);

		unloadMapSantos(santosScenarios, santosZones, santosAmbientZones, santosPeds, santosVehicles, santosRadio, { 0, 3 }, santosScenariosState, santosAmbientZonesState);

		// if in singleplayer
		if (!NETWORK::NETWORK_IS_IN_SESSION())
		{
			UI::_SET_MINIMAP_REVEALED(true);
			for (int i = 0; i < 5; i++)
				GAMEPLAY::DISABLE_HOSPITAL_RESTART(i, true);
			for (int i = 0; i < 7; i++)
				GAMEPLAY::DISABLE_POLICE_RESTART(i, true);
		}

		STREAMING::_SET_MAPDATACULLBOX_ENABLED((char*)"HeistIsland", true);

		if (!NETWORK::NETWORK_IS_IN_SESSION())
		{
			// disable the following scripts in singleplayer to remove blips and respawn points when not in LS
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME((char*)"blip_controller");          // responsible for blips
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME((char*)"forsalesigns");			   // responsible for property for sale signs
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME((char*)"respawn_controller");	   // responsible for respawning the player
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME((char*)"restrictedareas");		   // responsible for restricted areas
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME((char*)"vehicle_gen_controller");   // responsible for player businesses / vehicle blips
		}
	}

	// Unload North Yankton
	void unloadYankton()
	{
		removeIpls(yanktonIpls);
		disableZones(yanktonZones);
		disableAmbientZones(yanktonAmbientZones);
		UI::SET_MINIMAP_IN_PROLOGUE(false);
		PATHFIND::SET_ROADS_IN_ANGLED_AREA(5526.24f, -5137.23f, 61.78925f, 3679.327f, -4973.879f, 125.0828f, 192, false, false, true);
		PATHFIND::SET_ROADS_IN_ANGLED_AREA(3691.211f, -4941.24f, 94.59368f, 3511.115f, -4869.191f, 126.7621f, 16, false, false, true);
		PATHFIND::SET_ROADS_IN_ANGLED_AREA(3510.004f, -4865.81f, 94.69557f, 3204.424f, -4833.817f, 126.8152f, 16, false, false, true);
		PATHFIND::SET_ROADS_IN_ANGLED_AREA(3186.534f, -4832.798f, 109.8148f, 3202.187f, -4833.993f, 114.815f, 16, false, false, true);
		int weatherID = std::rand() % yanktonWeatherTypes.size();
		// if in multiplayer
		if (NETWORK::NETWORK_IS_IN_SESSION())
		{
			GAMEPLAY::CLEAR_WEATHER_TYPE_NOW_PERSIST_NETWORK(1);
		}
		else
		{
			GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
		}
		GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
	}

	// used to handle manual flight between maps
	void FlightController()
	{
		Entity player = playerPed;

		if (PED::IS_PED_IN_ANY_VEHICLE(player, 0))
			player = PED::GET_VEHICLE_PED_IS_USING(player);
		float playerAltitude = ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(player);
		Vector3 playerVelocity = ENTITY::GET_ENTITY_VELOCITY(player);
		float playerVelocityX = playerVelocity.x;
		float playerVelocityY = playerVelocity.y;
		float playerVelocityZ = playerVelocity.z;
		if (worldtravel::IsLosSantos())
		{
			if (ENTITY::IS_ENTITY_AT_COORD(player, FlyToLCFromLSCoords[0], FlyToLCFromLSCoords[1], playerAltitude, 1500.0f, 1500.0f, 1500.0f, 0, 1, 0))
			{
				CAM::DO_SCREEN_FADE_OUT(800);
				WAIT(800);
				worldtravel::SetPlayerLocationID(-1);
				loadLiberty();
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToLSFromLCCoords[0] + 1500.00f, FlyToLSFromLCCoords[1] + 1500.00f, playerAltitude, 0, 0, 1);
				ENTITY::SET_ENTITY_HEADING(player, 300.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);
				ENTITY::SET_ENTITY_VELOCITY(player, playerVelocityX, playerVelocityY, playerVelocityZ);
				if (!NETWORK::NETWORK_IS_IN_SESSION())
				{
					TIME::ADD_TO_CLOCK_TIME(9, 0, 0);
				}
				else
				{
					NetworkClockController(true);
				}
				unloadSantos();
				worldtravel::SetPlayerLocationID(1);
				CAM::DO_SCREEN_FADE_IN(800);
			}

			if (ENTITY::IS_ENTITY_AT_COORD(player, FlyToCPFromLSCoords[0], FlyToCPFromLSCoords[1], playerAltitude, 1500.0f, 1500.0f, 1500.0f, 0, 1, 0) &&
				!NETWORK::NETWORK_IS_IN_SESSION())
			{
				CAM::DO_SCREEN_FADE_OUT(800);
				WAIT(800);
				worldtravel::SetPlayerLocationID(-1);
				loadCayo();

				if (!NETWORK::NETWORK_IS_IN_SESSION())
				{
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToLSFromCPCoords[0] + 1500.00f, FlyToLSFromCPCoords[1] - 1500.00f, playerAltitude, 0, 0, 1);
				}
				else
				{
					if (playerAltitude < 400.0f)
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToCPFromLSCoordsMP[0], FlyToCPFromLSCoordsMP[1], 200.0f, 0, 0, 1);
					}
					else
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToCPFromLSCoordsMP[0], FlyToCPFromLSCoordsMP[1], playerAltitude, 0, 0, 1);
					}
				}
				ENTITY::SET_ENTITY_HEADING(player, 240.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);
				ENTITY::SET_ENTITY_VELOCITY(player, playerVelocityX, playerVelocityY, playerVelocityZ);
				if (!NETWORK::NETWORK_IS_IN_SESSION())
				{
					TIME::ADD_TO_CLOCK_TIME(14, 0, 0);
				}
				else
				{
					NetworkClockController(true);
				}
				unloadSantos();
				worldtravel::SetPlayerLocationID(3);
				CAM::DO_SCREEN_FADE_IN(800);
			}
		}
		else if (worldtravel::IsLibertyCity())
		{
			if (ENTITY::IS_ENTITY_AT_COORD(player, FlyToLSFromLCCoords[0], FlyToLSFromLCCoords[1], playerAltitude, 1500.0f, 1500.0f, 1500.0f, 0, 1, 0))
			{

				CAM::DO_SCREEN_FADE_OUT(800);
				WAIT(800);
				worldtravel::SetPlayerLocationID(-1);
				loadSantos();
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToLCFromLSCoords[0] - 1500.00f, FlyToLCFromLSCoords[1] - 1500.00f, playerAltitude, 0, 0, 1);
				ENTITY::SET_ENTITY_HEADING(player, 130.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);
				ENTITY::SET_ENTITY_VELOCITY(player, playerVelocityX, playerVelocityY, playerVelocityZ);
				if (!NETWORK::NETWORK_IS_IN_SESSION())
				{
					TIME::ADD_TO_CLOCK_TIME(3, 0, 0);
				}
				else
				{
					NetworkClockController(false);
					NETWORK::NETWORK_CLEAR_CLOCK_TIME_OVERRIDE();
				}
				unloadLiberty();
				worldtravel::SetPlayerLocationID(0);
				CAM::DO_SCREEN_FADE_IN(800);

			}

			if (ENTITY::IS_ENTITY_AT_COORD(player, FlyToCPFromLCCoords[0], FlyToCPFromLCCoords[1], playerAltitude, 1500.0f, 1500.0f, 1500.0f, 0, 1, 0) &&
				!NETWORK::NETWORK_IS_IN_SESSION())
			{

				CAM::DO_SCREEN_FADE_OUT(800);
				WAIT(800);
				worldtravel::SetPlayerLocationID(-1);
				unloadLiberty();
				if (!NETWORK::NETWORK_IS_IN_SESSION())
				{
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToLCFromCPCoords[0] + 1500.00f, FlyToLCFromCPCoords[1] - 1500.00f, playerAltitude, 0, 0, 1);
				}
				else
				{
					if (playerAltitude < 400.0f)
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToCPFromLCCoordsMP[0], FlyToCPFromLCCoordsMP[1], 200.0f, 0, 0, 1);
					}
					else
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToCPFromLCCoordsMP[0], FlyToCPFromLCCoordsMP[1], playerAltitude, 0, 0, 1);
					}
				}
				ENTITY::SET_ENTITY_HEADING(player, 220.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);
				ENTITY::SET_ENTITY_VELOCITY(player, playerVelocityX, playerVelocityY, playerVelocityZ);
				if (!NETWORK::NETWORK_IS_IN_SESSION())
				{
					TIME::ADD_TO_CLOCK_TIME(6, 0, 0);
				}
				loadCayo();
				worldtravel::SetPlayerLocationID(3);
				CAM::DO_SCREEN_FADE_IN(800);

			}
		}
		else if (worldtravel::IsCayoPerico())
		{
			int destination = -1;
			if (!NETWORK::NETWORK_IS_IN_SESSION())
			{
				if (ENTITY::IS_ENTITY_AT_COORD(player, FlyToLSFromCPCoords[0], FlyToLSFromCPCoords[1], playerAltitude, 1500.0f, 1500.0f, 1500.0f, 0, 1, 0))
				{
					destination = 0;
				}

				if (ENTITY::IS_ENTITY_AT_COORD(player, FlyToLCFromCPCoords[0], FlyToLCFromCPCoords[1], playerAltitude, 1500.0f, 1500.0f, 1500.0f, 0, 1, 0))
				{
					destination = 1;
				}
			}
			else
			{
				if (!ENTITY::IS_ENTITY_DEAD(playerPed) &&
					!ENTITY::IS_ENTITY_IN_AREA(playerPed, CayoPericoWestBound, CayoPericoEastBound, -10000.0f, CayoPericoSouthBound, CayoPericoNorthBound, 10000.0f, false, false, false))
				{
					float playerHeading = ENTITY::GET_ENTITY_HEADING(player);
					if ((playerHeading >= CayoPericoMPDestHeading) && playerHeading <= 180.0f + CayoPericoMPDestHeading)
					{
						destination = 0;
					}
					else
					{
						destination = 1;
					}
				}
			}

			if (destination == 0)
			{
				CAM::DO_SCREEN_FADE_OUT(800);
				WAIT(800);
				worldtravel::SetPlayerLocationID(-1);
				unloadCayo();
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToCPFromLSCoords[0] - 1500.00f, FlyToCPFromLSCoords[1] + 1500.00f, playerAltitude, 0, 0, 1);
				ENTITY::SET_ENTITY_HEADING(player, 45.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);
				ENTITY::SET_ENTITY_VELOCITY(player, playerVelocityX, playerVelocityY, playerVelocityZ);
				if (!NETWORK::NETWORK_IS_IN_SESSION())
				{
					TIME::ADD_TO_CLOCK_TIME(6, 0, 0);
				}
				else
				{
					NetworkClockController(false);
					NETWORK::NETWORK_CLEAR_CLOCK_TIME_OVERRIDE();
				}
				loadSantos();
				worldtravel::SetPlayerLocationID(0);
				CAM::DO_SCREEN_FADE_IN(800);
			}
			else if (destination == 1)
			{
				CAM::DO_SCREEN_FADE_OUT(800);
				WAIT(800);
				worldtravel::SetPlayerLocationID(-1);
				unloadCayo();
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToCPFromLCCoords[0] - 1500.00f, FlyToCPFromLCCoords[1] + 1500.00f, playerAltitude, 0, 0, 1);
				ENTITY::SET_ENTITY_HEADING(player, 10.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
				CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);
				ENTITY::SET_ENTITY_VELOCITY(player, playerVelocityX, playerVelocityY, playerVelocityZ);
				if (!NETWORK::NETWORK_IS_IN_SESSION())
				{
					TIME::ADD_TO_CLOCK_TIME(4, 0, 0);
				}
				loadLiberty();
				worldtravel::SetPlayerLocationID(1);
				CAM::DO_SCREEN_FADE_IN(800);
			}
		}
	}

	// Airport Travel from Los Santos
	void AirportTravelLS()
	{
		if (!ENTITY::IS_ENTITY_DEAD(playerPed) && ENTITY::IS_ENTITY_AT_COORD(playerPed, LosSantosIntAirport[0], LosSantosIntAirport[1], LosSantosIntAirport[2], 150.0f, 150.0f, 150.0f, 0, 1, 0) && !PED::IS_PED_IN_ANY_VEHICLE(player, 0))
		{
			GRAPHICS::DRAW_MARKER(1, -1050.53f, -2741.48f, 13.60f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6f, 0.6f, 0.5f, 240, 200, 80, 150, false, false, 2, false, false, false, 0);
			if (ENTITY::IS_ENTITY_AT_COORD(playerPed, LosSantosIntAirport[0], LosSantosIntAirport[1], LosSantosIntAirport[2], 1.0f, 1.0f, 1.0f, 0, 1, 0))
			{
				if (PLAYER::GET_PLAYER_WANTED_LEVEL(playerPed) == 0)
				{
					worldtravel::HelpText::DisplayHelpText(Settings::GetLang("Lng34", "Press ~INPUT_CONTEXT~ to fly to Liberty City or ~INPUT_CONTEXT_SECONDARY~ to North Yankton for $350."));
					if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 51) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 52))
					{
						int destination = 0;
						if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 51))
						{
							destination = 1;
						}
						else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 52))
						{
							destination = 2;
						}
						CAM::DO_SCREEN_FADE_OUT(800);
						WAIT(800);
						worldtravel::SetPlayerLocationID(-1);
						int maxWanted = PLAYER::GET_MAX_WANTED_LEVEL();
						PLAYER::SET_MAX_WANTED_LEVEL(0);
						CUTSCENE::REQUEST_CUTSCENE((char*)"hs4_lsa_take_nimb2", 8);
						CUTSCENE::SET_CUTSCENE_PED_COMPONENT_VARIATION_FROM_PED("MP_1", playerPed, 0);
						CUTSCENE::REGISTER_ENTITY_FOR_CUTSCENE(playerPed, "MP_1", 0, 0, 64);
						CUTSCENE::SET_CUTSCENE_FADE_VALUES(true, true, true, true);
						CUTSCENE::START_CUTSCENE(0);
						ENTITY::FREEZE_ENTITY_POSITION(playerPed, true);
						if (NETWORK::NETWORK_IS_IN_SESSION())
						{
							NETWORK::NETWORK_SET_IN_MP_CUTSCENE(true, true);
						}
						while (CUTSCENE::IS_CUTSCENE_ACTIVE())
						{
							WAIT(0);
						}
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToLCFromLSCoords[0] - 1500.00f, FlyToLCFromLSCoords[1] - 1500.00f, FlyToLCFromLSCoords[2] + 1500.00f, 0, 0, 1);
						if (destination == 1)
							loadLiberty();
						else if (destination == 2)
							loadYankton();
						CUTSCENE::REMOVE_CUTSCENE();
						CAM::DO_SCREEN_FADE_OUT(0);
						unloadSantos();
						WAIT(100);
						if (destination == 1)
						{
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, FrancisIntAirportArrival[0], FrancisIntAirportArrival[1], FrancisIntAirportArrival[2], 0, 0, 1);
							ENTITY::SET_ENTITY_HEADING(playerPed, 97.4f);
						}
						else if (destination == 2)
						{
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, NorthYanktonAirportArrival[0], NorthYanktonAirportArrival[1], NorthYanktonAirportArrival[2], 0, 0, 1);
							ENTITY::SET_ENTITY_HEADING(playerPed, 86.0f);
							yanktonVehicleHash = GAMEPLAY::GET_HASH_KEY("asea2");
							ENTITY::SET_ENTITY_HEADING(playerPed, 86.0f);
							STREAMING::REQUEST_MODEL(yanktonVehicleHash);
							while (!STREAMING::HAS_MODEL_LOADED(yanktonVehicleHash))
							{
								WAIT(0);
							}
							yanktonVehicle = VEHICLE::CREATE_VEHICLE(yanktonVehicleHash, NorthYanktonAirportArrival[0], NorthYanktonAirportArrival[1], NorthYanktonAirportArrival[2], 86.0f, true, true, false);
							AI::TASK_WARP_PED_INTO_VEHICLE(playerPed, yanktonVehicle, -1);
							STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(yanktonVehicleHash);
						}
						CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
						CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);
						if (!NETWORK::NETWORK_IS_IN_SESSION())
						{
							if (destination == 1)
								TIME::ADD_TO_CLOCK_TIME(13, 0, 0);
							else if (destination == 2)
								TIME::ADD_TO_CLOCK_TIME(12, 0, 0);
						}
						else
						{
							NetworkClockController(true);
						}
						if (destination == 1)
						{
							CUTSCENE::REQUEST_CUTSCENE_WITH_PLAYBACK_LIST((char*)"lc_lca_land_nimb", 1, 24);
							CUTSCENE::SET_CUTSCENE_PED_COMPONENT_VARIATION_FROM_PED("MP_1", playerPed, 0);
							CUTSCENE::REGISTER_ENTITY_FOR_CUTSCENE(playerPed, "MP_1", 0, 0, 64);
							CUTSCENE::SET_CUTSCENE_FADE_VALUES(true, true, true, true);
							CUTSCENE::START_CUTSCENE(0);
							CAM::DO_SCREEN_FADE_IN(250);
							while (CUTSCENE::IS_CUTSCENE_ACTIVE())
							{
								WAIT(0);
							}
							CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
							CUTSCENE::REMOVE_CUTSCENE();
						}
						if (NETWORK::NETWORK_IS_IN_SESSION())
						{
							NETWORK::NETWORK_SET_IN_MP_CUTSCENE(false, false);
						}
						ENTITY::FREEZE_ENTITY_POSITION(playerPed, false);
						CAM::DO_SCREEN_FADE_OUT(0);
						if (destination == 1)
							ENTITY::SET_ENTITY_HEADING(playerPed, 97.4f);
						else if (destination == 2)
							ENTITY::SET_ENTITY_HEADING(playerPed, 86.0f);
						PLAYER::SET_MAX_WANTED_LEVEL(maxWanted);
						worldtravel::SetPlayerLocationID(destination);
						CAM::DO_SCREEN_FADE_IN(800);
					}
				}
				else
				{
					worldtravel::HelpText::DisplayHelpText(Settings::GetLang("Lng35", "You cannot buy a plane ticket while wanted by the cops!"));
				}
			}
		}
	}

	// Airport Travel from Liberty City
	void AirportTravelLC()
	{
		if (!ENTITY::IS_ENTITY_DEAD(playerPed) && ENTITY::IS_ENTITY_AT_COORD(playerPed, FrancisIntAirport[0], FrancisIntAirport[1], FrancisIntAirport[2], 150.0f, 150.0f, 150.0f, 0, 1, 0) && !PED::IS_PED_IN_ANY_VEHICLE(player, 0))
		{
			GRAPHICS::DRAW_MARKER(1, 7553.86f, -2879.39f, 5.08f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6f, 0.6f, 0.5f, 240, 200, 80, 150, false, false, 2, false, false, false, 0);
			if (ENTITY::IS_ENTITY_AT_COORD(playerPed, FrancisIntAirport[0], FrancisIntAirport[1], FrancisIntAirport[2], 1.0f, 1.0f, 1.0f, 0, 1, 0))
			{
				if (PLAYER::GET_PLAYER_WANTED_LEVEL(playerPed) == 0)
				{
					worldtravel::HelpText::DisplayHelpText(Settings::GetLang("Lng36", "Press ~INPUT_CONTEXT~ to fly to Los Santos or ~INPUT_CONTEXT_SECONDARY~ to North Yankton for $350."));
					if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 51) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 52))
					{
						int destination = 1;
						if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 51))
						{
							destination = 0;
						}
						else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 52))
						{
							destination = 2;
						}
						CAM::DO_SCREEN_FADE_OUT(800);
						WAIT(800);
						worldtravel::SetPlayerLocationID(-1);
						int maxWanted = PLAYER::GET_MAX_WANTED_LEVEL();
						PLAYER::SET_MAX_WANTED_LEVEL(0);
						CUTSCENE::REQUEST_CUTSCENE((char*)"lc_lca_take_nimb2", 8);
						CUTSCENE::SET_CUTSCENE_PED_COMPONENT_VARIATION_FROM_PED("MP_1", playerPed, 0);
						CUTSCENE::REGISTER_ENTITY_FOR_CUTSCENE(playerPed, "MP_1", 0, 0, 64);
						CUTSCENE::SET_CUTSCENE_FADE_VALUES(true, true, true, true);
						ENTITY::FREEZE_ENTITY_POSITION(playerPed, true);
						CUTSCENE::START_CUTSCENE(0);
						if (NETWORK::NETWORK_IS_IN_SESSION())
						{
							NETWORK::NETWORK_SET_IN_MP_CUTSCENE(true, true);
						}
						while (CUTSCENE::IS_CUTSCENE_ACTIVE())
						{
							WAIT(0);
						}
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToLSFromLCCoords[0] + 1500.00f, FlyToLSFromLCCoords[1] + 1500.00f, FlyToLSFromLCCoords[2] + 1500.00f, 0, 0, 1);
						if (destination == 0)
							loadSantos();
						else if (destination == 2)
							loadYankton();
						CUTSCENE::REMOVE_CUTSCENE();
						CAM::DO_SCREEN_FADE_OUT(0);
						unloadLiberty();
						WAIT(100);
						if (destination == 0)
						{
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, LosSantosIntAirportArrival[0], LosSantosIntAirportArrival[1], LosSantosIntAirportArrival[2], 0, 0, 1);
							ENTITY::SET_ENTITY_HEADING(playerPed, 341.24f);
						}
						else if (destination == 2)
						{
							ENTITY::FREEZE_ENTITY_POSITION(playerPed, false);
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, NorthYanktonAirportArrival[0], NorthYanktonAirportArrival[1], NorthYanktonAirportArrival[2], 0, 0, 1);
							yanktonVehicleHash = GAMEPLAY::GET_HASH_KEY("asea2");
							ENTITY::SET_ENTITY_HEADING(playerPed, 86.0f);
							STREAMING::REQUEST_MODEL(yanktonVehicleHash);
							while (!STREAMING::HAS_MODEL_LOADED(yanktonVehicleHash))
							{
								WAIT(0);
							}
							yanktonVehicle = VEHICLE::CREATE_VEHICLE(yanktonVehicleHash, NorthYanktonAirportArrival[0], NorthYanktonAirportArrival[1], NorthYanktonAirportArrival[2], 86.0f, true, true, false);
							AI::TASK_WARP_PED_INTO_VEHICLE(playerPed, yanktonVehicle, -1);
							STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(yanktonVehicleHash);
						}
						CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
						CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);

						if (!NETWORK::NETWORK_IS_IN_SESSION())
						{
							if (destination == 0)
								TIME::ADD_TO_CLOCK_TIME(3, 0, 0);
							else if (destination == 2)
								TIME::ADD_TO_CLOCK_TIME(7, 0, 0);
						}
						else
						{
							if (destination == 0)
							{
								NetworkClockController(false);
								NETWORK::NETWORK_CLEAR_CLOCK_TIME_OVERRIDE();
							}
						}
						if (destination == 0)
						{
							CUTSCENE::REQUEST_CUTSCENE_WITH_PLAYBACK_LIST((char*)"hs4_lsa_land_nimb", 1, 24);
							CUTSCENE::SET_CUTSCENE_PED_COMPONENT_VARIATION_FROM_PED("MP_1", playerPed, 0);
							CUTSCENE::REGISTER_ENTITY_FOR_CUTSCENE(playerPed, "MP_1", 0, 0, 64);
							CUTSCENE::SET_CUTSCENE_FADE_VALUES(true, true, true, true);
							CUTSCENE::START_CUTSCENE(0);
							while (CUTSCENE::IS_CUTSCENE_ACTIVE())
							{
								WAIT(0);
							}
							CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
							CUTSCENE::REMOVE_CUTSCENE();
						}
						if (NETWORK::NETWORK_IS_IN_SESSION())
						{
							NETWORK::NETWORK_SET_IN_MP_CUTSCENE(false, false);
						}
						ENTITY::FREEZE_ENTITY_POSITION(playerPed, false);
						CAM::DO_SCREEN_FADE_OUT(0);
						if (destination == 0)
							ENTITY::SET_ENTITY_HEADING(playerPed, 341.24f);
						else if (destination == 2)
							ENTITY::SET_ENTITY_HEADING(playerPed, 86.0f);
						PLAYER::SET_MAX_WANTED_LEVEL(maxWanted);
						worldtravel::SetPlayerLocationID(destination);
						CAM::DO_SCREEN_FADE_IN(800);
					}
				}
				else
				{
					worldtravel::HelpText::DisplayHelpText(Settings::GetLang("Lng35", "You cannot buy a plane ticket while wanted by the cops!"));
				}
			}
		}
	}

	// Airport Travel from North Yankton
	void AirportTravelNY()
	{
		if (!ENTITY::IS_ENTITY_DEAD(playerPed) &&
			(!ENTITY::IS_ENTITY_IN_AREA(playerPed, 2570.86f, -5099.23f, -10000.0f, 3486.60f, -4551.66f, 10000.0f, false, false, false) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, 3486.60f, -5099.23f, -10000.0f, 3747.66f, -4627.10f, 10000.0f, false, false, false) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, 3747.66f, -5399.22f, -10000.0f, 5789.13f, -4657.80f, 10000.0f, false, false, false)))
		{
			if (PLAYER::GET_PLAYER_WANTED_LEVEL(playerPed) == 0)
			{
				worldtravel::HelpText::DisplayHelpText(Settings::GetLang("Lng37", "Press ~INPUT_CONTEXT~ to fly to Los Santos or ~INPUT_CONTEXT_SECONDARY~ to Liberty City for $350."));
				if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 51) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 52))
				{
					int destination = 2;
					if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 51))
					{
						destination = 0;
					}
					else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 52))
					{
						destination = 1;
					}
					CAM::DO_SCREEN_FADE_OUT(800);
					WAIT(800);
					worldtravel::SetPlayerLocationID(-1);
					int maxWanted = PLAYER::GET_MAX_WANTED_LEVEL();
					PLAYER::SET_MAX_WANTED_LEVEL(0);
					ENTITY::FREEZE_ENTITY_POSITION(playerPed, true);
					if (PED::IS_PED_IN_ANY_VEHICLE(player, 0))
					{
						playerVehicle = PED::GET_VEHICLE_PED_IS_USING(player);
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(playerVehicle, true, true);
						VEHICLE::DELETE_VEHICLE(&playerVehicle);
					}
					if (destination == 0)
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToLCFromLSCoords[0] - 1500.00f, FlyToLCFromLSCoords[1] - 1500.00f, FlyToLCFromLSCoords[2] + 1500.00f, 0, 0, 1);
						loadSantos();
					}
					else if (destination == 1)
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToLSFromLCCoords[0] + 1500.00f, FlyToLSFromLCCoords[1] + 1500.00f, FlyToLSFromLCCoords[2] + 1500.00f, 0, 0, 1);
						loadLiberty();
					}
					if (destination == 0)
					{
						CAM::DO_SCREEN_FADE_OUT(0);
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, LosSantosIntAirportArrival[0], LosSantosIntAirportArrival[1], LosSantosIntAirportArrival[2], 0, 0, 1);
						ENTITY::SET_ENTITY_HEADING(playerPed, 341.24f);
					}
					else if (destination == 1)
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, FrancisIntAirportArrival[0], FrancisIntAirportArrival[1], FrancisIntAirportArrival[2], 0, 0, 1);
						ENTITY::SET_ENTITY_HEADING(playerPed, 97.4f);
					}
					CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
					CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);

					if (!NETWORK::NETWORK_IS_IN_SESSION())
					{
						if (destination == 0)
							TIME::ADD_TO_CLOCK_TIME(7, 0, 0);
						else if (destination == 1)
							TIME::ADD_TO_CLOCK_TIME(9, 0, 0);
					}
					else
					{
						if (destination == 0)
						{
							NetworkClockController(false);
							NETWORK::NETWORK_CLEAR_CLOCK_TIME_OVERRIDE();
						}
					}
					unloadYankton();
					if (NETWORK::NETWORK_IS_IN_SESSION())
					{
						NETWORK::NETWORK_SET_IN_MP_CUTSCENE(true, true);
					}
					if (destination == 0)
					{
						CUTSCENE::REQUEST_CUTSCENE_WITH_PLAYBACK_LIST((char*)"hs4_lsa_land_nimb", 1, 24);
						CUTSCENE::SET_CUTSCENE_PED_COMPONENT_VARIATION_FROM_PED("MP_1", playerPed, 0);
						CUTSCENE::REGISTER_ENTITY_FOR_CUTSCENE(playerPed, "MP_1", 0, 0, 64);
						CUTSCENE::SET_CUTSCENE_FADE_VALUES(true, true, true, true);
						CUTSCENE::START_CUTSCENE(0);
						while (CUTSCENE::IS_CUTSCENE_ACTIVE())
						{
							WAIT(0);
						}
						CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
						CUTSCENE::REMOVE_CUTSCENE();
						if (NETWORK::NETWORK_IS_IN_SESSION())
						{
							NETWORK::NETWORK_SET_IN_MP_CUTSCENE(false, false);
						}
					}
					if (destination == 1)
					{
						CUTSCENE::REQUEST_CUTSCENE_WITH_PLAYBACK_LIST((char*)"lc_lca_land_nimb", 1, 24);
						CUTSCENE::SET_CUTSCENE_PED_COMPONENT_VARIATION_FROM_PED("MP_1", playerPed, 0);
						CUTSCENE::REGISTER_ENTITY_FOR_CUTSCENE(playerPed, "MP_1", 0, 0, 64);
						CUTSCENE::SET_CUTSCENE_FADE_VALUES(true, true, true, true);
						CUTSCENE::START_CUTSCENE(0);
						CAM::DO_SCREEN_FADE_IN(250);
						while (CUTSCENE::IS_CUTSCENE_ACTIVE())
						{
							WAIT(0);
						}
						CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
						CUTSCENE::REMOVE_CUTSCENE();
					}
					if (NETWORK::NETWORK_IS_IN_SESSION())
					{
						NETWORK::NETWORK_SET_IN_MP_CUTSCENE(false, false);
					}
					CAM::DO_SCREEN_FADE_OUT(0);
					ENTITY::FREEZE_ENTITY_POSITION(playerPed, false);
					if (destination == 0)
						ENTITY::SET_ENTITY_HEADING(playerPed, 341.24f);
					else if (destination == 1)
						ENTITY::SET_ENTITY_HEADING(playerPed, 97.4f);
					PLAYER::SET_MAX_WANTED_LEVEL(maxWanted);
					worldtravel::SetPlayerLocationID(destination);
					CAM::DO_SCREEN_FADE_IN(800);
				}
			}
			else
			{
				worldtravel::HelpText::DisplayHelpText(Settings::GetLang("Lng35", "You cannot buy a plane ticket while wanted by the cops!"));
			}
		}
	}

	void AirportTravel()
	{
		if (worldtravel::IsLosSantos())
		{
			AirportTravelLS();
		}
		if (worldtravel::IsLibertyCity())
		{
			AirportTravelLC();
		}
		if (worldtravel::IsNorthYankton())
		{
			AirportTravelNY();
		}
	}

	// Handles the docks travel system from Los Santos to Liberty City
	void DocksTravel()
	{
		Entity player = playerPed;
		if (PED::IS_PED_IN_ANY_VEHICLE(player, 0))
		{
			player = PED::GET_VEHICLE_PED_IS_USING(player);
			Vector3 playerVelocity = ENTITY::GET_ENTITY_VELOCITY(player);
			float playerVelocityX = playerVelocity.x;
			float playerVelocityY = playerVelocity.y;
			float playerVelocityZ = playerVelocity.z;
			if (worldtravel::IsLosSantos())
			{
				if (!ENTITY::IS_ENTITY_DEAD(playerPed) && ENTITY::IS_ENTITY_AT_COORD(player, LosSantosDocks[0], LosSantosDocks[1], LosSantosDocks[2], 150.0f, 150.0f, 150.0f, 0, 1, 0))
				{
					GRAPHICS::DRAW_MARKER(1, 1170.40f, -2973.52f, 4.93f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 9.0f, 9.0f, 0.8f, 240, 200, 80, 150, false, false, 2, false, false, false, 0);
					if (ENTITY::IS_ENTITY_AT_COORD(player, LosSantosDocks[0], LosSantosDocks[1], LosSantosDocks[2], 10.0f, 10.0f, 1.0f, 0, 1, 0))
					{
						if (PLAYER::GET_PLAYER_WANTED_LEVEL(playerPed) == 0)
						{
							if (playerVelocityX < 0.1f && playerVelocityY < 0.1f && playerVelocityZ < 0.1f)
							{
								worldtravel::HelpText::DisplayHelpText(Settings::GetLang("Lng38", "Press ~INPUT_CONTEXT~ to ship this vehicle to Liberty City for $3500."));
								if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 51))
								{
									CAM::DO_SCREEN_FADE_OUT(800);
									WAIT(800);
									worldtravel::SetPlayerLocationID(-1);
									int maxWanted = PLAYER::GET_MAX_WANTED_LEVEL();
									PLAYER::SET_MAX_WANTED_LEVEL(0);
									ENTITY::FREEZE_ENTITY_POSITION(player, true);
									ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToCPFromLSCoordsMP[0], FlyToCPFromLSCoordsMP[1], FlyToCPFromLSCoordsMP[2], 0, 0, 1);
									loadLiberty();
									ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, LibertyCityDocksArrival[0], LibertyCityDocksArrival[1], LibertyCityDocksArrival[2], 0, 0, 1);
									ENTITY::SET_ENTITY_HEADING(player, 334.40f);
									CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
									CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);
									if (!NETWORK::NETWORK_IS_IN_SESSION())
									{
										TIME::ADD_TO_CLOCK_TIME(12, 0, 0);
									}
									else
									{
										NetworkClockController(true);
									}
									unloadSantos();
									if (NETWORK::NETWORK_IS_IN_SESSION())
									{
										NETWORK::NETWORK_SET_IN_MP_CUTSCENE(false, false);
									}
									ENTITY::SET_ENTITY_HEADING(playerPed, 334.40f);
									PLAYER::SET_MAX_WANTED_LEVEL(maxWanted);
									ENTITY::FREEZE_ENTITY_POSITION(player, false);
									worldtravel::SetPlayerLocationID(1);
									CAM::DO_SCREEN_FADE_IN(800);
								}
							}
						}
						else
						{
							worldtravel::HelpText::DisplayHelpText(Settings::GetLang("Lng39", "You cannot ship this vehicle while wanted by the cops!"));
						}
					}
				}
			}
			if (worldtravel::IsLibertyCity())
			{
				if (!ENTITY::IS_ENTITY_DEAD(playerPed) && ENTITY::IS_ENTITY_AT_COORD(player, LibertyCityDocks[0], LibertyCityDocks[1], LibertyCityDocks[2], 150.0f, 150.0f, 150.0f, 0, 1, 0))
				{
					GRAPHICS::DRAW_MARKER(1, 5935.99f, -3531.89f, 4.85f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 9.0f, 9.0f, 0.8f, 240, 200, 80, 150, false, false, 2, false, false, false, 0);
					if (ENTITY::IS_ENTITY_AT_COORD(player, LibertyCityDocks[0], LibertyCityDocks[1], LibertyCityDocks[2], 10.0f, 10.0f, 1.0f, 0, 1, 0))
					{
						if (PLAYER::GET_PLAYER_WANTED_LEVEL(playerPed) == 0)
						{
							if (playerVelocityX < 0.1f && playerVelocityY < 0.1f && playerVelocityZ < 0.1f)
							{
								worldtravel::HelpText::DisplayHelpText(Settings::GetLang("Lng40", "Press ~INPUT_CONTEXT~ to ship this vehicle to Los Santos for $3500."));
								if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 51))
								{
									CAM::DO_SCREEN_FADE_OUT(800);
									WAIT(800);
									worldtravel::SetPlayerLocationID(-1);
									int maxWanted = PLAYER::GET_MAX_WANTED_LEVEL();
									PLAYER::SET_MAX_WANTED_LEVEL(0);
									ENTITY::FREEZE_ENTITY_POSITION(player, true);
									ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToCPFromLCCoordsMP[0], FlyToCPFromLCCoordsMP[1], FlyToCPFromLCCoordsMP[2], 0, 0, 1);
									loadSantos();
									ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, LosSantosDocksArrival[0], LosSantosDocksArrival[1], LosSantosDocksArrival[2], 0, 0, 1);
									ENTITY::SET_ENTITY_HEADING(player, 90.0f);
									CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
									CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);

									if (!NETWORK::NETWORK_IS_IN_SESSION())
									{
										TIME::ADD_TO_CLOCK_TIME(12, 0, 0);
									}
									else
									{
										NetworkClockController(false);
										NETWORK::NETWORK_CLEAR_CLOCK_TIME_OVERRIDE();
									}
									unloadLiberty();
									ENTITY::SET_ENTITY_HEADING(playerPed, 90.0f);
									PLAYER::SET_MAX_WANTED_LEVEL(maxWanted);
									ENTITY::FREEZE_ENTITY_POSITION(player, false);
									worldtravel::SetPlayerLocationID(0);
									CAM::DO_SCREEN_FADE_IN(800);
								}
							}
						}
						else
						{
							worldtravel::HelpText::DisplayHelpText(Settings::GetLang("Lng39", "You cannot ship this vehicle while wanted by the cops!"));
						}
					}
				}
			}
		}
	}


	// Handles Fast Travelling
	void TeleportPlayer(Entity& player, float x, float y, float z, float heading)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(player, 0))
		{
			player = PED::GET_VEHICLE_PED_IS_USING(player);
		}
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, x, y, z, 0, 0, 1);
		ENTITY::SET_ENTITY_HEADING(player, heading);
		CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
		CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);
	}

	void PerformTeleport(Entity player, int currentLocation, int destination)
	{
		CAM::DO_SCREEN_FADE_OUT(800);
		WAIT(800);
		worldtravel::SetPlayerLocationID(-1);

		// TP player to temp location
		ENTITY::FREEZE_ENTITY_POSITION(player, true);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, FlyToLCFromLSCoords[0] + 1500, FlyToLCFromLSCoords[1] + 1500, FlyToLCFromLSCoords[2] + 1500, 0, 0, 1);

		// Unload the current map
		switch (currentLocation)
		{
		case 1:
			unloadLiberty();
			break;
		case 2:
			unloadYankton();
			break;
		case 3:
			unloadCayo();
			break;
		default:
			unloadSantos();
			break;
		}

		// Load the new map and teleport player
		switch (destination)
		{
		case 1: // Liberty City
			loadLiberty();
			TeleportPlayer(player, 4976.62f, -2914.04f, 16.48f, 0.0f);
			if (!NETWORK::NETWORK_IS_IN_SESSION()) TIME::ADD_TO_CLOCK_TIME(3, 0, 0);
			else NetworkClockController(true);
			break;
		case 2: // North Yankton
			loadYankton();
			TeleportPlayer(player, 3359.87f, -4849.52f, 111.67f, 0.0f);
			if (!NETWORK::NETWORK_IS_IN_SESSION()) TIME::ADD_TO_CLOCK_TIME(-1, 0, 0);
			break;
		case 3: // Cayo Perico
			loadCayo();
			TeleportPlayer(player, 4906.26f, -4912.76f, 3.36f, 0.0f);
			if (!NETWORK::NETWORK_IS_IN_SESSION()) TIME::ADD_TO_CLOCK_TIME(2, 0, 0);
			break;
		case 0: // Los Santos
			loadSantos();
			TeleportPlayer(player, -272.17f, -616.54f, 35.16f, 0.0f);
			if (!NETWORK::NETWORK_IS_IN_SESSION()) TIME::ADD_TO_CLOCK_TIME(-5, 0, 0);
			else
			{
				NetworkClockController(false);
				NETWORK::NETWORK_CLEAR_CLOCK_TIME_OVERRIDE();
			}
			break;
		}


		ENTITY::FREEZE_ENTITY_POSITION(player, false);
		worldtravel::SetPlayerLocationID(destination);
		CAM::DO_SCREEN_FADE_IN(800);
	}

	void TeleportBetweenMaps()
	{

		if (IsKeyJustUp(Settings::TeleportKey))
		{
			Settings::Load();
			if (Settings::FastTravel)
			{
				Entity player = playerPed;
				int currentLocation = worldtravel::GetPlayerLocationID();
				int destination = -1;

				// Determine destination based on current location
				switch (currentLocation)
				{
				case 0:
					destination = 1; // Liberty City
					break;
				case 1:
					if (Settings::EnableNorthYankton) // North Yankton (if enabled), or Cayo Perico (if enabled and not in session), or back to Los Santos
					{
						destination = 2;
						break;
					}
				case 2:
					destination = (Settings::EnableCayoPerico && !NETWORK::NETWORK_IS_IN_SESSION()) ? 3 : 0; // Cayo Perico (if enabled and not in session) or back to Los Santos
					break;
				case 3:
					destination = 0; // Los Santos
					break;
				default:
					destination = 0; // Fallback to Los Santos
					break;
				}

				// Teleport to the determined destination
				if (destination != -1)
				{
					PerformTeleport(player, currentLocation, destination);
				}
			}
		}
	}

	void SwitchMap(int currentLocation, int destination)
	{
		CAM::DO_SCREEN_FADE_OUT(800);
		WAIT(800);
		worldtravel::SetPlayerLocationID(-1);

		// Unload the current map
		switch (currentLocation)
		{
		case 1:
			unloadLiberty();
			break;
		case 2:
			unloadYankton();
			break;
		case 3:
			unloadCayo();
			break;
		default:
			unloadSantos();
			break;
		}

		// Load the new map
		switch (destination)
		{
		case 1: // Liberty City
			loadLiberty();
			if (!NETWORK::NETWORK_IS_IN_SESSION()) TIME::ADD_TO_CLOCK_TIME(3, 0, 0);
			else NetworkClockController(true);
			break;
		case 2: // North Yankton
			loadYankton();
			if (!NETWORK::NETWORK_IS_IN_SESSION()) TIME::ADD_TO_CLOCK_TIME(-1, 0, 0);
			break;
		case 3: // Cayo Perico
			loadCayo();
			if (!NETWORK::NETWORK_IS_IN_SESSION()) TIME::ADD_TO_CLOCK_TIME(2, 0, 0);
			break;
		case 0: // Los Santos
			loadSantos();
			if (!NETWORK::NETWORK_IS_IN_SESSION()) TIME::ADD_TO_CLOCK_TIME(-5, 0, 0);
			else
			{
				NetworkClockController(false);
				NETWORK::NETWORK_CLEAR_CLOCK_TIME_OVERRIDE();
			}
			break;
		}


		ENTITY::FREEZE_ENTITY_POSITION(player, false);
		worldtravel::SetPlayerLocationID(destination);
		CAM::DO_SCREEN_FADE_IN(800);
	}

	void CharacterSwitchLoadLS()
	{
		int currentLocation = worldtravel::GetPlayerLocationID();
		if (currentLocation > 0)
		{
			if (STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS())
			{
				if (STREAMING::GET_PLAYER_SWITCH_TYPE() == 1)
				{
					if (!NETWORK::NETWORK_IS_IN_SESSION())
					{
						if (STREAMING::GET_PLAYER_SWITCH_STATE() > 4)
						{
							SwitchMap(currentLocation, 0);
							while (STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS())
							{
								WAIT(0);
							}
						}
					}
					else
					{
						if (STREAMING::GET_PLAYER_SWITCH_STATE() > 2)
						{
							SwitchMap(currentLocation, 0);
							while (STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS())
							{
								WAIT(0);
							}
						}
					}
				}
			}
		}
	}

	void RunTick()
	{
		CayoPericoIslandHopperHelper();
		worldtravel::MpMap::CheckIfMPMapIsActive();
		AirportTravel();
		DocksTravel();
		TeleportBetweenMaps();
		FlightController();
		CharacterSwitchLoadLS();
		BlipVisibilityController();
		NpcSpawnBlocker();
		KeepLosSantosIplsDisabled();
		CreateBlips();
		static bool hasRunOnStartup = false; // Static variable to ensure this runs only once
		if (Settings::EnableLCOnStartup && !hasRunOnStartup)
		{
			hasRunOnStartup = true;
			if (worldtravel::IsLosSantos())
			{
				SwitchMap(0, 1);
				playerPed = PLAYER::PLAYER_PED_ID();
				bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
				ENTITY::SET_ENTITY_COORDS(playerPed, 5022.0f, -2644.89f, 15.55f, 1, 0, 0, 1);
			}
		}
	}


	void LevelSwitchMain()
	{
		initialize();
		Settings::Load();
		while (true)
		{
			playerPed = PLAYER::PLAYER_ID();
			playerPed = PLAYER::PLAYER_PED_ID();
			bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
			RunTick();
			WAIT(0);

		}
	}
}

void LevelSwitch()
{
	srand(GetTickCount());
	levelSwitch::LevelSwitchMain();
}
