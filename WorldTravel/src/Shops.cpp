#include "Shops.h"
#include <fstream>  
#include <windows.h>
#include <string>
#include <ctime>
#include <iostream>
#include <vector>
#include <map>
#include "Settings.h"

namespace shops
{
	int shopLocationID = 0;

	// Blip Objects
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

	Blip BankNorthYanktonBlip;

	struct BlipData {
		Blip& blip;
		std::vector<float> coords;
		std::string name;
		int sprite;
	};

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
			{"ClukinBellAlgonquin",                {ClukinBellAlgonquinBlip,                {5066.1504f, -3185.6660f, 13.8030f}, Settings::GetLang("Lng13", "Clukin' Bell"), 628}},
			{"ClukinBellDukes",                    {ClukinBellDukesBlip,                    {6373.4043f, -2895.9224f, 24.1033f}, Settings::GetLang("Lng13", "Clukin' Bell"), 628}},
			{"BurgerShotMemoryLanesBroker",		   {BurgerShotMemoryLanesBrokerBlip,        {6389.4692f, -3911.3887f, 15.8449f}, Settings::GetLang("Lng14", "Memory Lanes"), 628}},
			{"BurgerShotBroker",                   {BurgerShotBrokerBlip,                   {6826.2378f, -3030.5420f, 24.2120f}, Settings::GetLang("Lng15", "Burger Shot"), 628}},
			{"BurgerShotBohanEast",                {BurgerShotBohanEastBlip,                {6299.3047f, -1671.3417f, 15.9075f}, Settings::GetLang("Lng15", "Burger Shot"), 628}},
			{"BurgerShotBohanWest",                {BurgerShotBohanWestBlip,                {5638.3301f, -1752.8379f, 15.3157f}, Settings::GetLang("Lng15", "Burger Shot"), 628}},
			{"BurgerShotAlgonquinNorth",           {BurgerShotAlgonquinNorthBlip,           {4761.3691f, -2062.2380f, 12.0470f}, Settings::GetLang("Lng15", "Burger Shot"), 628}},
			{"BurgerShotAlgonquinStarJunction",    {BurgerShotAlgonquinStarJunctionBlip,    {5017.1978f, -2968.6631f, 12.8201f}, Settings::GetLang("Lng15", "Burger Shot"), 628}},
			{"BurgerShotAlgonquinWest",            {BurgerShotAlgonquinWestBlip,            {4570.8115f, -3123.6797f, 3.81120f}, Settings::GetLang("Lng15", "Burger Shot"), 628}},
			{"BurgerShotAlderney",                 {BurgerShotAlderneyBlip,                 {4181.9746f, -1628.2620f, 23.3140f}, Settings::GetLang("Lng15", "Burger Shot"), 628}},
			{"DinerBroker",                        {DinerBrokerBlip,                        {6072.9814f, -3743.4500f, 14.8829f}, Settings::GetLang("Lng16", "69th Street Diner"), 628}},
			{"SuperStarCafeAlgonquinStarJunction", {SuperStarCafeAlgonquinStarJunctionBlip, {4952.5571f, -3204.7126f, 14.7081f}, Settings::GetLang("Lng17", "SuperStar Cafe"), 628}},
			{"SuperStarCafeAlgonquinNorth",        {SuperStarCafeAlgonquinNorthBlip,        {5203.9976f, -2275.1921f, 14.6489f}, Settings::GetLang("Lng17", "SuperStar Cafe"), 628}},
			{"HomebrewCafeBroker",                 {HomebrewCafeBrokerBlip,                 {6655.1089f, -3198.0620f, 24.1856f}, Settings::GetLang("Lng18", "Homebrew Cafe"), 59}},
			{"MrFuksAlderney",                     {MrFuksAlderneyBlip,                     {3947.2644f, -2168.1052f, 18.7802f}, Settings::GetLang("Lng19", "Mr. Fuk's Rice Box"), 628}},
			{"TWAtBroker",                         {TWAtBrokerBlip,                         {6160.6509f, -3430.5513f, 23.1937f}, Settings::GetLang("Lng20", "TW@ Internet Cafe"), 606}},
			{"TWAtAlgonquin",                      {TWAtAlgonquinBlip,                      {4852.9136f, -1860.1442f, 12.9132f}, Settings::GetLang("Lng20", "TW@ Internet Cafe"), 606}},
			{"TWAtAlderney",                       {TWAtAlderneyBlip,                       {3611.6792f, -2790.4824f, 25.4438f}, Settings::GetLang("Lng20", "TW@ Internet Cafe"), 606}},
			{"ComradesBarBroker",                  {ComradesBarBrokerBlip,                  {6124.5181f, -3747.0295f, 15.4847f}, Settings::GetLang("Lng21", "Comrades Bar"), 93}},
			{"SteinwayBeerGardenDukes",            {SteinwayBeerGardenDukesBlip,            {6331.3066f, -2515.9221f, 35.5150f}, Settings::GetLang("Lng22", "Steinway Beer Garden"), 93}},
			{"LuckyWinklesAlgonquin",              {LuckyWinklesAlgonquinBlip,              {4754.0688f, -2804.8669f, 10.3949f}, Settings::GetLang("Lng23", "Lucky Winkles"), 93}},
			{"UndergroundGunShopBroker",           {UndergroundGunShopBrokerBlip,           {6243.5972f, -3169.5608f, 33.4374f}, Settings::GetLang("Lng24", "Underground Gun Shop"), 110}},
			{"UndergroundGunShopAlgonquin",        {UndergroundGunShopAlgonquinBlip,        {5271.6997f, -3593.6382f, 10.1742f}, Settings::GetLang("Lng24", "Underground Gun Shop"), 110}},
			{"UndergroundGunShopAlderney",         {UndergroundGunShopAlderneyBlip,         {3850.3928f, -2936.9114f, 13.6174f}, Settings::GetLang("Lng24", "Underground Gun Shop"), 110}},
			{"PayNSprayBroker",                    {PayNSprayBrokerBlip,					{6246.4839f, -3543.1167f, 20.6996f}, Settings::GetLang("Lng25", "Pay 'n' Spray"), 72}},
			{"PayNSprayAlgonquinWest",             {PayNSprayAlgonquinWestBlip,				{4674.4072f, -2880.7759f, 6.00740f}, Settings::GetLang("Lng25", "Pay 'n' Spray"), 72}},
			{"PayNSprayAlgonquinNorth",            {PayNSprayAlgonquinNorthBlip,			{4880.3901f, -1707.3942f, 19.6576f}, Settings::GetLang("Lng25", "Pay 'n' Spray"), 72}},
			{"PayNSprayAlderneyNorth",             {PayNSprayAlderneyNorthBlip,				{4040.0032f, -2079.8928f, 16.2785f}, Settings::GetLang("Lng25", "Pay 'n' Spray"), 72}},
			{"PayNSprayAlderneySouth",             {PayNSprayAlderneySouthBlip,				{3888.0132f, -2982.1316f, 10.1923f}, Settings::GetLang("Lng25", "Pay 'n' Spray"), 72}},
			{"ClothingStoreBroker",                {ClothingStoreBrokerBlip,				{6083.7803f, -3699.3035f, 15.8601f}, Settings::GetLang("Lng26", "Clothing Store"), 73}},
			{"ClothingStoreAlgonquinSouth",        {ClothingStoreAlgonquinSouthBlip,		{5199.0508f, -3914.9797f, 14.7631f}, Settings::GetLang("Lng26", "Clothing Store"), 73}},
			{"ClothingStoreAlgonquinEast",         {ClothingStoreAlgonquinEastBlip,			{5210.4600f, -2445.9326f, 14.7133f}, Settings::GetLang("Lng26", "Clothing Store"), 73}},
			{"ClothingStoreAlgonquinNorth",        {ClothingStoreAlgonquinNorthBlip,		{4907.4253f, -1902.6703f, 25.6240f}, Settings::GetLang("Lng26", "Clothing Store"), 73}},
			{"BankAlgonquinSouth",                 {BankAlgonquinSouthBlip,					{5176.0400f, -3723.6200f, 16.8800f}, Settings::GetLang("Lng27", "Bank"), 272}},
			{"TheMajesticCentral",                 {TheMajesticCentralBlip,					{5022.1100f, -2659.0100f, 15.3900f}, Settings::GetLang("Lng28", "The Majestic"), 475}}
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
			{"BankNorthYankton",                 {BankNorthYanktonBlip,					{5308.4995f, -5222.037f, 83.5129f}, Settings::GetLang("Lng27", "Bank"), 272}}
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
			&ClukinBellAlgonquinBlip, &ClukinBellDukesBlip, &BurgerShotMemoryLanesBrokerBlip,
			&BurgerShotBrokerBlip, &BurgerShotBohanEastBlip, &BurgerShotBohanWestBlip,
			&BurgerShotAlgonquinNorthBlip, &BurgerShotAlgonquinStarJunctionBlip,
			&BurgerShotAlgonquinWestBlip, &BurgerShotAlderneyBlip, &DinerBrokerBlip,
			&SuperStarCafeAlgonquinStarJunctionBlip, &SuperStarCafeAlgonquinNorthBlip,
			&HomebrewCafeBrokerBlip, &MrFuksAlderneyBlip, &TWAtBrokerBlip,
			&TWAtAlgonquinBlip, &TWAtAlderneyBlip, &ComradesBarBrokerBlip,
			&SteinwayBeerGardenDukesBlip, &LuckyWinklesAlgonquinBlip,
			&UndergroundGunShopBrokerBlip, &UndergroundGunShopAlgonquinBlip,
			&UndergroundGunShopAlderneyBlip, &PayNSprayBrokerBlip,
			&PayNSprayAlgonquinWestBlip, &PayNSprayAlgonquinNorthBlip,
			&PayNSprayAlderneyNorthBlip, &PayNSprayAlderneySouthBlip,
			&ClothingStoreBrokerBlip, &ClothingStoreAlgonquinSouthBlip,
			&ClothingStoreAlgonquinEastBlip, &ClothingStoreAlgonquinNorthBlip,
			&BankAlgonquinSouthBlip, &TheMajesticCentralBlip
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

	void ShopsMain()
	{
		while (true)
		{
			CreateShopBlipsLC();
			CreateShopBlipsNY();
			ActiveBlipController();
			WAIT(0);
		}
	}
}

void Shops()
{
	srand(GetTickCount());
	shops::ShopsMain();
}
