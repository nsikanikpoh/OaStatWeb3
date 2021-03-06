/*
===========================================================================
OaStatWeb3 - A web frontend for oastat
Copyright (C) 2013 Poul Sander

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/
 
Note that while this file can be redistrubuted under the GPL version 2 or later 
OaStatWeb3 as a whole is distributed under GPL version 3 or later 
with the following exception:
Additional permission under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or combining 
it with a compilable skin/template, the licensors of this Program grant you 
additional permission to convey the resulting work. Corresponding Source 
for a non-source form of such a combination shall include the source code 
for the parts of the skin/template used as well as that of the covered work. 
*/

#include "OpenArenaConverters.hpp"

std::string OaWeaponConverter::getValue(std::string key) {
	int weaponnumber = atoi(key.c_str());
	switch(weaponnumber) {
		case 0:
			key = "Unknown";
			break;
		case 1:
			key = "Shotgun";
			break;
		case 2:
			key = "Gauntlet";
			break;
		case 3:
			key = "Machinegun";
			break;
		case 4:
			key = "Grenade";
			break;
		case 5:
			key = "Grenade (splash)";
			break;
		case 6:
			key = "Rocket launcher";
			break;
		case 7:
			key = "Rocket (splash)";
			break;
		case 8:
			key = "Plasma";
			break;
		case 9:
			key = "Plasma (splash)";
			break;
		case 10:
			key = "Rail gun";
			break;
		case 11:
			key = "Lightning";
			break;
		case 12:
			key = "BFG";
			break;
		case 13:
			key = "BFG (splash)";
			break;
		case 14:
			key = "Water";
			break;
		case 15:
			key = "Slime";
			break;
		case 16:
			key = "Lava";
			break;
		case 17:
			key = "Crushed";
			break;
		case 18:
			key = "Telefrag";
			break;
		case 19:
			key = "Falling";
			break;
		case 20:
			key = "Suicide";
			break;
		case 21:
			key = "Laser";
			break;
		case 22:
			key = "Falling/Trigger";
			break;
		case 23:
			key = "Nail gun";
			break;
		case 24:
			key = "Chain gun";
			break;
		case 25:
			key = "Mines";
			break;
		case 26:
			key = "Kamikaze";
			break;
		case 27:
			key ="Juiced";
			break;
		case 28:
			key = "Grapple";
			break;
			/*
		p_weaponnames.push_back("Shotgun");
	p_weaponnames.push_back("Gauntlet");
	p_weaponnames.push_back("Machinegun");
	p_weaponnames.push_back("Grenade");
	p_weaponnames.push_back("Grenade (splash)");
	p_weaponnames.push_back("Rocket");
	p_weaponnames.push_back("Rocket (splash)");
	p_weaponnames.push_back("Plasma");
	p_weaponnames.push_back("Plasma (splash)");
	p_weaponnames.push_back("Rail gun");
	p_weaponnames.push_back("Ligntning gun");
	p_weaponnames.push_back("BFG");
	p_weaponnames.push_back("BFG Splash");
	p_weaponnames.push_back("Water");
	p_weaponnames.push_back("Slime");
	p_weaponnames.push_back("Lava");
	p_weaponnames.push_back("Crushed");
	p_weaponnames.push_back("Telefrag");
	p_weaponnames.push_back("Falling");
	p_weaponnames.push_back("Suicide");
	p_weaponnames.push_back("Laser");
	p_weaponnames.push_back("Trigger");
	p_weaponnames.push_back("Nail gun");
	p_weaponnames.push_back("Chain gun");
	p_weaponnames.push_back("Mines");
	p_weaponnames.push_back("Kamikaze");
	p_weaponnames.push_back("Juiced");
	p_weaponnames.push_back("Grapple");*/
	}

	return key;
}

std::string OaGametypeConverter::getValue(std::string key) {
	int gametype = atoi(key.c_str());
	switch(gametype) {
		case 0:
			key = "Deathmatch";
			break;
		case 1:
			key = "Tourney";
			break;
		case 3: 
			key = "Team deathmatch";
			break;
		case 4:
			key = "Capture the flag";
			break;
		case 5:
			key = "One flag capture";
			break;
		case 6:
			key = "Overload";
			break;
		case 7:
			key = "Harvester";
			break;
		case 8:
			key = "Elimination";
			break;
		case 9:
			key = "Ctf elimination";
			break;
		case 10:
			key = "Last man standing";
			break;
		case 11:
			key = "Double domination";
			break;
		case 12:
			key = "Domination";
			break;
			
		
	}
	return key;
}