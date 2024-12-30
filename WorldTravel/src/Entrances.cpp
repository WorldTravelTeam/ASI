#include "Entrances.h"
#include <fstream>  
#include <windows.h>
#include <string>
#include <ctime>
#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include "Settings.h"


namespace entrances
{
    struct Location {
        std::vector<float> coordinates;
        float heading = 0.0f;
    };

    struct MarkerInfo {
        std::string helpText;
        Location enterLocation;
        Location destLocation;
        float markerZOffset;
        float markerHorizontalScale;
        float markerVerticalScale;
        float markerTriggerDistance;
        bool clearWanted;
        bool allowVehicles;
    };


    ////////////////////////////////////////////////////
    ///          Marker Helper functions             ///
    ////////////////////////////////////////////////////

    // check if an entity is within a given distance
    bool IsEntityAtLocation(Entity entity, const Location& location, float radius) {
        return ENTITY::IS_ENTITY_AT_COORD(
            entity,
            location.coordinates[0], location.coordinates[1], location.coordinates[2],
            radius, radius, radius, false, true, false
        );
    }

    // draw a marker at a specific location
    void DrawMarker(const Location& location, float zOffset, float horizontalScale, float verticalScale) {
        GRAPHICS::DRAW_MARKER(
            1, location.coordinates[0], location.coordinates[1], location.coordinates[2] + zOffset,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            horizontalScale, horizontalScale, verticalScale,
            240, 200, 80, 150,
            false, false, 2, false, false, false, 0
        );
    }

	// handles player teleportation, and optionally clears wanted level, and sets player heading
    void TeleportPlayer(Entity player, const Location& dest, bool clearWanted) {
        if (PLAYER::GET_PLAYER_WANTED_LEVEL(player) > 0 && clearWanted) {
            PLAYER::SET_PLAYER_WANTED_LEVEL(player, 0, false);
            PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, false);
        }
        ENTITY::SET_ENTITY_COORDS_NO_OFFSET(
            player, dest.coordinates[0], dest.coordinates[1], dest.coordinates[2], false, false, true
        );
        ENTITY::SET_ENTITY_HEADING(player, dest.heading);
        CAM::SET_GAMEPLAY_CAM_RELATIVE_HEADING(0.0f);
        CAM::SET_GAMEPLAY_CAM_RELATIVE_PITCH(0.0f, 0.0f);
    }

    void Marker(const MarkerInfo& info) {
        Player playerPed = PLAYER::PLAYER_PED_ID();
        Entity player = playerPed;

        if (PED::IS_PED_IN_ANY_VEHICLE(player, false)) {
            if (info.allowVehicles) {
                player = PED::GET_VEHICLE_PED_IS_USING(player);
            }
            else {
                return;
            }
        }

        if (!ENTITY::IS_ENTITY_DEAD(player) && IsEntityAtLocation(player, info.enterLocation, 150.0f)) {
            DrawMarker(info.enterLocation, info.markerZOffset, info.markerHorizontalScale, info.markerVerticalScale);

            if (IsEntityAtLocation(player, info.enterLocation, info.markerTriggerDistance)) {
                worldtravel::HelpText::DisplayHelpText(info.helpText);

                if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 51)) { // INPUT_CONTEXT
                    CAM::DO_SCREEN_FADE_OUT(800);
                    WAIT(800);
                    TeleportPlayer(player, info.destLocation, info.clearWanted);
                    CAM::DO_SCREEN_FADE_IN(800);
                }
            }
        }
    }

    // spawn markers for a specific location set
    void SpawnMarkers(const std::unordered_map<std::string, Location>& locations, const std::vector<MarkerInfo>& markers) {
        for (const auto& marker : markers) {
            Marker(marker);
        }
    }
    ////////////////////////////////////////////////////
    ///          Marker Helper functions             ///
    ////////////////////////////////////////////////////



    void SpawnAllMarkers() {

        
        if (worldtravel::IsLosSantos())
        {
            // Los Santos marker locations
            std::unordered_map<std::string, Location> locationsLS = {

                {"HighEndGarageIn",     {{-104.17f,  824.31f, 235.37f},  49.0f}},
                {"HighEndGarageOut",    {{277.01f,  -102.32f, -99.26f},  80.0f}},
                {"FIBIn",               {{136.46f,  -769.90f,  45.75f}, 340.0f}},
                {"FIBOut",              {{132.20f,  -756.66f,  70.66f},  13.0f}},
            };

            // Los Santos marker configurations
            std::vector<MarkerInfo> markersLS = {
                {Settings::GetLang("Lng1", "Press ~INPUT_CONTEXT~ to enter the garage."),
                 locationsLS["HighEndGarageIn"], locationsLS["HighEndGarageOut"], -1.0f, 2.4f, 2.3f, 3.0f, false, true},

                {Settings::GetLang("Lng2", "Press ~INPUT_CONTEXT~ to exit the garage."),
                 locationsLS["HighEndGarageOut"], locationsLS["HighEndGarageIn"], -1.0f, 2.4f, 2.3f, 3.0f, false, true},

                {Settings::GetLang("Lng3", "Press ~INPUT_CONTEXT~ to take the lift to the lower FIB offices."),
                 locationsLS["FIBIn"], locationsLS["FIBOut"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng4", "Press ~INPUT_CONTEXT~ to take the lift to the lobby."),
                 locationsLS["FIBOut"], locationsLS["FIBIn"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},
            };

            SpawnMarkers(locationsLS, markersLS);
        }
        else if (worldtravel::IsLibertyCity())
        {
            // Liberty City marker locations
            std::unordered_map<std::string, Location> locationsLC = {

                {"RotterdamTowerSouthElevatorLobby",    {{4940.03f, -3360.68f,  14.61f},    0.0f}},
                {"RotterdamTowerSouthRoof",             {{4916.42f, -3368.93f, 335.44f},  270.0f}},
                {"RotterdamTowerNorthElevatorLobby",    {{4940.03f, -3353.13f,  14.61f},  180.0f}},
                {"RotterdamTowerNorthRoof",             {{4916.39f, -3344.59f, 335.44f},  270.0f}},
                {"RichardsMajesticLobby",               {{5004.09f, -2658.90f,  15.34f},  270.0f}},
                {"RichardsMajestic30thFloor",           {{5003.86f, -2658.95f, 114.44f},  270.0f}},
                {"MiddleParkEastPenthouseLobby",        {{5285.48f, -2415.46f,  15.79f},    0.0f}},
                {"MiddleParkEastPenthouseApartment",    {{5284.52f, -2404.51f,  45.05f},  270.0f}},
                {"NorthwoodPenthouseLobby",             {{4758.83f, -1781.88f,  20.15f},  270.0f}},
                {"NorthwoodPenthouseApartment",         {{4761.92f, -1788.95f,  38.96f},  180.0f}},
                {"TrumpTowerPenthouseGround",           {{5183.79f, -2646.17f,  14.79f},    0.0f}},
                {"TrumpTowerPenthouseApartment",        {{5174.56f, -2667.62f, 182.78f},  270.0f}},
                {"BruciePenthouseGround",               {{5965.39f, -3101.70f,   6.02f},   70.0f}},
                {"BruciePenthouseApartment",            {{5990.96f, -3107.44f,  29.01f},  239.0f}},
                {"GayTonyPenthouseGround",              {{5113.54f, -3476.07f,  14.74f},  180.0f}},
                {"GayTonyPenthouseApartment",           {{5117.27f, -3455.19f,  32.80f}, 100.34f}},
            };

            // Liberty City marker configurations
            std::vector<MarkerInfo> markersLC = {
                {Settings::GetLang("Lng5", "Press ~INPUT_CONTEXT~ to take the lift to the observation deck."),
                 locationsLC["RotterdamTowerSouthElevatorLobby"], locationsLC["RotterdamTowerSouthRoof"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng5", "Press ~INPUT_CONTEXT~ to take the lift to the observation deck."),
                 locationsLC["RotterdamTowerNorthElevatorLobby"], locationsLC["RotterdamTowerNorthRoof"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng6", "Press ~INPUT_CONTEXT~ to take the lift to the lobby."),
                 locationsLC["RotterdamTowerSouthRoof"], locationsLC["RotterdamTowerSouthElevatorLobby"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng6", "Press ~INPUT_CONTEXT~ to take the lift to the lobby."),
                 locationsLC["RotterdamTowerNorthRoof"], locationsLC["RotterdamTowerNorthElevatorLobby"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng7", "Press ~INPUT_CONTEXT~ to take the lift to the 30th floor."),
                 locationsLC["RichardsMajesticLobby"], locationsLC["RichardsMajestic30thFloor"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng6", "Press ~INPUT_CONTEXT~ to take the lift to the lobby."),
                 locationsLC["RichardsMajestic30thFloor"], locationsLC["RichardsMajesticLobby"], -1.0f, false, false},

                {Settings::GetLang("Lng8", "Press ~INPUT_CONTEXT~ to take the lift to the apartment."),
                 locationsLC["MiddleParkEastPenthouseLobby"], locationsLC["MiddleParkEastPenthouseApartment"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng6", "Press ~INPUT_CONTEXT~ to take the lift to the lobby."),
                 locationsLC["MiddleParkEastPenthouseApartment"], locationsLC["MiddleParkEastPenthouseLobby"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng8", "Press ~INPUT_CONTEXT~ to take the lift to the apartment."),
                 locationsLC["NorthwoodPenthouseLobby"], locationsLC["NorthwoodPenthouseApartment"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng6", "Press ~INPUT_CONTEXT~ to take the lift to the lobby."),
                 locationsLC["NorthwoodPenthouseApartment"], locationsLC["NorthwoodPenthouseLobby"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng9", "Press ~INPUT_CONTEXT~ to take the lift to the penthouse."),
                 locationsLC["TrumpTowerPenthouseGround"], locationsLC["TrumpTowerPenthouseApartment"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng10", "Press ~INPUT_CONTEXT~ to take the lift to the ground."),
                 locationsLC["TrumpTowerPenthouseApartment"], locationsLC["TrumpTowerPenthouseGround"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng8", "Press ~INPUT_CONTEXT~ to take the lift to the apartment."),
                 locationsLC["BruciePenthouseGround"], locationsLC["BruciePenthouseApartment"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng10", "Press ~INPUT_CONTEXT~ to take the lift to the ground."),
                 locationsLC["BruciePenthouseApartment"], locationsLC["BruciePenthouseGround"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng9", "Press ~INPUT_CONTEXT~ to take the lift to the penthouse."),
                 locationsLC["GayTonyPenthouseGround"], locationsLC["GayTonyPenthouseApartment"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng10", "Press ~INPUT_CONTEXT~ to take the lift to the ground."),
                 locationsLC["GayTonyPenthouseApartment"], locationsLC["GayTonyPenthouseGround"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},
            };
            SpawnMarkers(locationsLC, markersLC);
        }
        else if (worldtravel::IsNorthYankton())
        {
            // North Yankton marker locations
            std::unordered_map<std::string, Location> locationsNY = {

                {"NorthYanktonBankIn",     {{5308.50f, -5222.04f, 83.52f},  180.0f}},
                {"NorthYanktonBankOut",    {{5308.80f, -5213.31f, 83.52f},  0.0f}},
            };

            // North Yankton marker configurations
            std::vector<MarkerInfo> markersNY = {
                {Settings::GetLang("Lng11", "Press ~INPUT_CONTEXT~ to enter the bank."),
                 locationsNY["NorthYanktonBankIn"], locationsNY["NorthYanktonBankOut"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},

                {Settings::GetLang("Lng12", "Press ~INPUT_CONTEXT~ to exit the bank."),
                 locationsNY["NorthYanktonBankOut"], locationsNY["NorthYanktonBankIn"], -1.0f, 0.6f, 0.5f, 1.0f, false, false},
            };

            SpawnMarkers(locationsNY, markersNY);

        }
    }

	void EntrancesMain()
	{
		while (true)
		{
			SpawnAllMarkers();
			WAIT(0);

		}
	}
}

void Entrances()
{
	srand(GetTickCount());
	entrances::EntrancesMain();
}
