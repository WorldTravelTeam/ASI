#include "Shops.h"
#include <fstream>  
#include <windows.h>
#include <string>
#include <ctime>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

namespace shops
{
	int shopLocationID = 0;

	// Liberty City Blip Objects
	Blip ClukinBellAlgonquinBlip;
	Blip ClukinBellDukesBlip;

	Blip BurgerShotMemoryLanesBrokerBlip;
	Blip BurgerShotBrokerBlip;
	Blip BurgerShotBohanEastBlip;
	Blip BurgerShotBohanWestBlip;
	Blip BurgerShotAlgonquinNorthBlip;
	Blip BurgerShotAlgonquinStarJunctionBlip;
	Blip BurgerShotAlgonquinWestBlip;
	Blip BurgerShotAlderneyBlip;

	Blip DinerBrokerBlip;
	Blip SuperStarCafeAlgonquinStarJunctionBlip;
	Blip SuperStarCafeAlgonquinNorthBlip;
	Blip HomebrewCafeBrokerBlip;
	Blip MrFuksAlderneyBlip;
	Blip TWAtBrokerBlip;
	Blip TWAtAlgonquinBlip;
	Blip TWAtAlderneyBlip;
	Blip ComradesBarBrokerBlip;
	Blip SteinwayBeerGardenDukesBlip;
	Blip LuckyWinklesAlgonquinBlip;

	Blip UndergroundGunShopBrokerBlip;
	Blip UndergroundGunShopAlgonquinBlip;
	Blip UndergroundGunShopAlderneyBlip;

	Blip PayNSprayBrokerBlip;
	Blip PayNSprayAlgonquinWestBlip;
	Blip PayNSprayAlgonquinNorthBlip;
	Blip PayNSprayAlderneyNorthBlip;
	Blip PayNSprayAlderneySouthBlip;

	Blip ClothingStoreBrokerBlip;
	Blip ClothingStoreAlgonquinSouthBlip;
	Blip ClothingStoreAlgonquinEastBlip;
	Blip ClothingStoreAlgonquinNorthBlip;

	Blip TheMajesticCentralBlip;
	Blip BankAlgonquinSouthBlip;

	Blip CableCarBlip1;
	Blip CableCarBlip2;

	Blip ASCFBlip;
	Blip SexShopBlip;
	Blip SexShopJerseySouthBlip;
	Blip BahamaMamasBlip;
	Blip HonkersJerseyBlip;
	Blip TriangleClubBlip;

	//North Yankton Blip Objects
	Blip BankNorthYanktonBlip;


	struct BlipData {
		Blip& blip;
		std::vector<float> coords;
		std::string name;
		int sprite;
	};

	struct Location {
		Vector3 coordinates;
		float heading = 0.0f;
	};

	struct PayNSprayData {
		std::string doorName;
		std::string doorPropName;
		std::vector<float> doorLocation;
		std::string garage;
	};

	struct PayNSprayInfo {
		PayNSprayData PayNSprayLocation;
	};

	// check if an entity is within a given distance
	bool IsEntityAtLocation(Entity entity, float x, float y, float z, float radius) {
		return ENTITY::IS_ENTITY_AT_COORD(
			entity,
			x, y, z,
			radius, radius, radius, false, true, false
		);
	}

	void CreateBlipIfNotExists(Blip& blip, const std::vector<float>& coords, const std::string& name, int sprite) {
		if (!UI::DOES_BLIP_EXIST(blip)) {
			worldtravel::Blips::CreateBlip(
				blip, coords[0], coords[1], coords[2],
				1, sprite, name.c_str(), 0, false, 2, true, 1
			);
		}
	}

	void CreateShopBlipsLC() {
		if (!worldtravel::IsLibertyCity()) {
			return;
		}

		// Blip Configuration
		std::map<std::string, BlipData> blips = {
			{"ClukinBellAlgonquin",                {ClukinBellAlgonquinBlip,                {5066.1504f, -3185.6660f, 13.8030f}, "Clukin' Bell", 628}},
			{"ClukinBellDukes",                    {ClukinBellDukesBlip,                    {6373.4043f, -2895.9224f, 24.1033f}, "Clukin' Bell", 628}},
			{"BurgerShotMemoryLanesBroker",		   {BurgerShotMemoryLanesBrokerBlip,        {6389.4692f, -3911.3887f, 15.8449f}, "Memory Lanes", 628}},
			{"BurgerShotBroker",                   {BurgerShotBrokerBlip,                   {6826.2378f, -3030.5420f, 24.2120f}, "Burger Shot", 628}},
			{"BurgerShotBohanEast",                {BurgerShotBohanEastBlip,                {6299.3047f, -1671.3417f, 15.9075f}, "Burger Shot", 628}},
			{"BurgerShotBohanWest",                {BurgerShotBohanWestBlip,                {5638.3301f, -1752.8379f, 15.3157f}, "Burger Shot", 628}},
			{"BurgerShotAlgonquinNorth",           {BurgerShotAlgonquinNorthBlip,           {4761.3691f, -2062.2380f, 12.0470f}, "Burger Shot", 628}},
			{"BurgerShotAlgonquinStarJunction",    {BurgerShotAlgonquinStarJunctionBlip,    {5017.1978f, -2968.6631f, 12.8201f}, "Burger Shot", 628}},
			{"BurgerShotAlgonquinWest",            {BurgerShotAlgonquinWestBlip,            {4570.8115f, -3123.6797f, 3.81120f}, "Burger Shot", 628}},
			{"BurgerShotAlderney",                 {BurgerShotAlderneyBlip,                 {4181.9746f, -1628.2620f, 23.3140f}, "Burger Shot", 628}},
			{"DinerBroker",                        {DinerBrokerBlip,                        {6072.9814f, -3743.4500f, 14.8829f}, "69th Street Diner", 628}},
			{"SuperStarCafeAlgonquinStarJunction", {SuperStarCafeAlgonquinStarJunctionBlip, {4952.5571f, -3204.7126f, 14.7081f}, "SuperStar Cafe", 628}},
			{"SuperStarCafeAlgonquinNorth",        {SuperStarCafeAlgonquinNorthBlip,        {5203.9976f, -2275.1921f, 14.6489f}, "SuperStar Cafe", 628}},
			{"HomebrewCafeBroker",                 {HomebrewCafeBrokerBlip,                 {6655.1089f, -3198.0620f, 24.1856f}, "Homebrew Cafe", 59}},
			{"MrFuksAlderney",                     {MrFuksAlderneyBlip,                     {3947.2644f, -2168.1052f, 18.7802f}, "Mr. Fuk's Rice Box", 628}},
			{"TWAtBroker",                         {TWAtBrokerBlip,                         {6160.6509f, -3430.5513f, 23.1937f}, "TW@ Internet Cafe", 606}},
			{"TWAtAlgonquin",                      {TWAtAlgonquinBlip,                      {4852.9136f, -1860.1442f, 12.9132f}, "TW@ Internet Cafe", 606}},
			{"TWAtAlderney",                       {TWAtAlderneyBlip,                       {3611.6792f, -2790.4824f, 25.4438f}, "TW@ Internet Cafe", 606}},
			{"ComradesBarBroker",                  {ComradesBarBrokerBlip,                  {6124.5181f, -3747.0295f, 15.4847f}, "Comrades Bar", 93}},
			{"SteinwayBeerGardenDukes",            {SteinwayBeerGardenDukesBlip,            {6331.3066f, -2515.9221f, 35.5150f}, "Steinway Beer Garden", 93}},
			{"LuckyWinklesAlgonquin",              {LuckyWinklesAlgonquinBlip,              {4754.0688f, -2804.8669f, 10.3949f}, "Lucky Winkles", 93}},
			{"UndergroundGunShopBroker",           {UndergroundGunShopBrokerBlip,           {6243.5972f, -3169.5608f, 33.4374f}, "Underground Gun Shop", 110}},
			{"UndergroundGunShopAlgonquin",        {UndergroundGunShopAlgonquinBlip,        {5271.6997f, -3593.6382f, 10.1742f}, "Underground Gun Shop", 110}},
			{"UndergroundGunShopAlderney",         {UndergroundGunShopAlderneyBlip,         {3850.3928f, -2936.9114f, 13.6174f}, "Underground Gun Shop", 110}},
			{"PayNSprayBroker",                    {PayNSprayBrokerBlip,					{6246.4839f, -3543.1167f, 20.6996f}, "Pay 'n' Spray", 72}},
			{"PayNSprayAlgonquinWest",             {PayNSprayAlgonquinWestBlip,				{4674.4072f, -2880.7759f, 6.00740f}, "Pay 'n' Spray", 72}},
			{"PayNSprayAlgonquinNorth",            {PayNSprayAlgonquinNorthBlip,			{4880.3901f, -1707.3942f, 19.6576f}, "Pay 'n' Spray", 72}},
			{"PayNSprayAlderneyNorth",             {PayNSprayAlderneyNorthBlip,				{4040.0032f, -2079.8928f, 16.2785f}, "Pay 'n' Spray", 72}},
			{"PayNSprayAlderneySouth",             {PayNSprayAlderneySouthBlip,				{3888.0132f, -2982.1316f, 10.1923f}, "Pay 'n' Spray", 72}},
			{"ClothingStoreBroker",                {ClothingStoreBrokerBlip,				{6083.7803f, -3699.3035f, 15.8601f}, "Clothing Store", 73}},
			{"ClothingStoreAlgonquinSouth",        {ClothingStoreAlgonquinSouthBlip,		{5199.0508f, -3914.9797f, 14.7631f}, "Clothing Store", 73}},
			{"ClothingStoreAlgonquinEast",         {ClothingStoreAlgonquinEastBlip,			{5210.4600f, -2445.9326f, 14.7133f}, "Clothing Store", 73}},
			{"ClothingStoreAlgonquinNorth",        {ClothingStoreAlgonquinNorthBlip,		{4907.4253f, -1902.6703f, 25.6240f}, "Clothing Store", 73}},
			{"BankAlgonquinSouth",                 {BankAlgonquinSouthBlip,					{5176.0400f, -3723.6200f, 16.8800f}, "Bank", 272}},
			{"TheMajesticCentral",                 {TheMajesticCentralBlip,					{5022.1100f, -2659.0100f, 15.3900f}, "The Majestic", 475}},
			{"CableCar1",						   {CableCarBlip1,							{5338.0600f, -3029.4400f, 21.3600f}, "Cable Car", 36}},
			{"CableCar2",						   {CableCarBlip2,							{5625.5900f, -3029.1800f, 14.4900f}, "Cable Car", 36}},
			{"ASCF",					           {ASCFBlip,								{4051.2000f, -3629.7500f, 4.72000f}, "Alderney State Correctional Facility", 188}},
			{"SexShop",					           {SexShopBlip,						    {5024.4800f, -2885.6800f, 15.4900f}, "Sex Shop", 279}},
			{"SexShopJerseySouth",				   {SexShopJerseySouthBlip,					{5972.1600f, -3795.7300f, 9.97000f}, "Sex Shop", 279}},
			{"BahamaMamas",					       {BahamaMamasBlip,						{4788.6000f, -2856.3600f, 15.7500f}, "Bahama Mamas", 614}},
			{"HonkersJersey",					   {HonkersJerseyBlip,						{3615.3200f, -3247.4400f, 13.4100f}, "Honkers", 121}},
			{"TriangleClub",					   {TriangleClubBlip,						{6368.7500f, -1562.1600f, 18.9500f}, "The Triangle Club", 121}},
		};

		// create each blip
		for (const auto& [key, data] : blips) {
			CreateBlipIfNotExists(data.blip, data.coords, data.name, data.sprite);
		}
	}

	void CreateShopBlipsNY() {
		if (!worldtravel::IsNorthYankton()) {
			return;
		}

		// Blip Configuration
		std::map<std::string, BlipData> blips = {
			{"BankNorthYankton",                 {BankNorthYanktonBlip,					{5308.4995f, -5222.037f, 83.5129f}, "Bank", 272}}
		};

		// create each blip
		for (const auto& [key, data] : blips) {
			CreateBlipIfNotExists(data.blip, data.coords, data.name, data.sprite);
		}
	}

	void ActiveBlipController()
	{
		// Define all blips
		std::vector<Blip*> blipsLC = {
			&ClukinBellAlgonquinBlip,
			&ClukinBellDukesBlip,
			&BurgerShotMemoryLanesBrokerBlip,
			&BurgerShotBrokerBlip,
			&BurgerShotBohanEastBlip,
			&BurgerShotBohanWestBlip,
			&BurgerShotAlgonquinNorthBlip,
			&BurgerShotAlgonquinStarJunctionBlip,
			&BurgerShotAlgonquinWestBlip,
			&BurgerShotAlderneyBlip,
			&DinerBrokerBlip,
			&SuperStarCafeAlgonquinStarJunctionBlip,
			&SuperStarCafeAlgonquinNorthBlip,
			&HomebrewCafeBrokerBlip,
			&MrFuksAlderneyBlip,
			&TWAtBrokerBlip,
			&TWAtAlgonquinBlip,
			&TWAtAlderneyBlip,
			&ComradesBarBrokerBlip,
			&SteinwayBeerGardenDukesBlip, 
			&LuckyWinklesAlgonquinBlip,
			&UndergroundGunShopBrokerBlip,
			&UndergroundGunShopAlgonquinBlip,
			&UndergroundGunShopAlderneyBlip, 
			&PayNSprayBrokerBlip,
			&PayNSprayAlgonquinWestBlip,
			&PayNSprayAlgonquinNorthBlip,
			&PayNSprayAlderneyNorthBlip, 
			&PayNSprayAlderneySouthBlip,
			&ClothingStoreBrokerBlip, 
			&ClothingStoreAlgonquinSouthBlip,
			&ClothingStoreAlgonquinEastBlip, 
			&ClothingStoreAlgonquinNorthBlip,
			&BankAlgonquinSouthBlip, 
			&TheMajesticCentralBlip,
			&CableCarBlip1, 
			&CableCarBlip2,
			&ASCFBlip,
			&SexShopBlip,
			&BahamaMamasBlip,
			&HonkersJerseyBlip,
			&TriangleClubBlip,
			&SexShopJerseySouthBlip,

		};

		std::vector<Blip*> blipsNY = {
			&BankNorthYanktonBlip
		};

		bool updateShopLocationID = false;

		// Liberty City
		if (!worldtravel::IsLibertyCity() && shopLocationID == 1)
		{
			for (Blip* blip : blipsLC)
			{
				worldtravel::Blips::SetBlipDisplay(*blip, 0);
				updateShopLocationID = true;
			}
		}
		else if (worldtravel::IsLibertyCity() && shopLocationID != 1)
		{
			for (Blip* blip : blipsLC)
			{
				worldtravel::Blips::SetBlipDisplay(*blip, 2);
				updateShopLocationID = true;
			}
		}

		// North Yankton
		if (!worldtravel::IsNorthYankton() && shopLocationID == 2)
		{
			for (Blip* blip : blipsNY)
			{
				worldtravel::Blips::SetBlipDisplay(*blip, 0);
				updateShopLocationID = true;
			}
		}
		else if (worldtravel::IsNorthYankton() && shopLocationID != 2)
		{
			for (Blip* blip : blipsNY)
			{
				worldtravel::Blips::SetBlipDisplay(*blip, 2);
				updateShopLocationID = true;
			}
		}
		if (updateShopLocationID)
			shopLocationID = worldtravel::GetPlayerLocationID();
	}

	void ProcessPayNSpray(const PayNSprayInfo& info) {
		Player playerPed = PLAYER::PLAYER_PED_ID();
		Entity player = playerPed;

		if (PED::IS_PED_IN_ANY_VEHICLE(player, false)) {
			player = PED::GET_VEHICLE_PED_IS_USING(player);
		}

		if (!ENTITY::IS_ENTITY_DEAD(player) && IsEntityAtLocation(player, info.PayNSprayLocation.doorLocation[0], info.PayNSprayLocation.doorLocation[1], info.PayNSprayLocation.doorLocation[2], 15.0f))
		{
			if (!worldtravel::PayNSpray::IsPayNSprayActive(info.PayNSprayLocation.doorName))
				worldtravel::PayNSpray::ActivatePayNSpray(info.PayNSprayLocation.doorName, info.PayNSprayLocation.doorPropName, info.PayNSprayLocation.doorLocation[0], info.PayNSprayLocation.doorLocation[1], info.PayNSprayLocation.doorLocation[2]);
			worldtravel::PayNSpray::PayNSpray(info.PayNSprayLocation.doorName, info.PayNSprayLocation.garage);
		}
		else
		{
			if (worldtravel::PayNSpray::IsPayNSprayActive(info.PayNSprayLocation.doorName))
				worldtravel::PayNSpray::DeactivatePayNSpray(info.PayNSprayLocation.doorName);
		}
	}

	// spawn pay n sprays if near by
	void SpawnPayNSprays(const std::unordered_map<std::string, PayNSprayData>& payNSprays, const std::vector<PayNSprayInfo>& payNSprayInfo) {
		for (const auto& payNSprayLocation : payNSprayInfo) {
			ProcessPayNSpray(payNSprayLocation);
		}
	}

	void PayNSprays()
	{
		//Liberty City Pay N' Spray Garage Doors
		if (worldtravel::IsLibertyCity())
		{
			// Liberty City marker locations
			std::unordered_map<std::string, PayNSprayData> payNSpraysLC = {
				{"BrokerPayNSpray",				{"BrokerPayNSpray", "bs3respraydoor", {6246.409f, -3546.857f, 22.31999f}, "Broker PayNSpray"}},
				{"NorthAlgonquinPayNSpray",		{"NorthAlgonquinPayNSpray", "pspray_gdoor1_mh5", {4880.308f, -1711.296f,  21.25111f}, "North Algonquin PayNSpray"}},
				{"WestAlgonquinPayNSpray",		{"WestAlgonquinPayNSpray", "pspray_gdoor1_mh3", {4674.543f, -2883.918f,  7.151477f}, "West Algonquin PayNSpray"}},
				{"NorthAlderneyPayNSpray",		{"NorthAlderneyPayNSpray", "nj01axelsdoor", {4040.036f, -2075.698f,  18.15517f}, "North Alderney PayNSpray"}},
				{"SouthAlderneyPayNSpray",		{"SouthAlderneyPayNSpray", "nj03pnsgrgdoor01", {3887.951f, -2978.066f,  11.88733f}, "South Alderney PayNSpray"}},
			};

			// Liberty City marker configurations
			std::vector<PayNSprayInfo> payNSprayGaragesLC = {
				{payNSpraysLC["BrokerPayNSpray"]},
				{payNSpraysLC["NorthAlgonquinPayNSpray"]},
				{payNSpraysLC["WestAlgonquinPayNSpray"]},
				{payNSpraysLC["NorthAlderneyPayNSpray"]},
				{payNSpraysLC["SouthAlderneyPayNSpray"]},
			};
			SpawnPayNSprays(payNSpraysLC, payNSprayGaragesLC);
		}

	}

	void ShopsMain()
	{
		while (true)
		{
			CreateShopBlipsLC();
			CreateShopBlipsNY();
			ActiveBlipController();
			PayNSprays();
			WAIT(0);
		}
	}
}

void Shops()
{
	srand(GetTickCount());
	shops::ShopsMain();
}

