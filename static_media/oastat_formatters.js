function gametypeFormatter (cellvalue, options, rowObject)
{
   key = cellvalue;
   switch(Number(cellvalue))
   {
	case 0:
		key = "Deathmatch";
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
		key = "CTF Elimination";
		break;
	case 10:
		key = "Last man standing";
		break;
	case 11:
		key = "Double Domination";
		break;
	case 12:
		key = "Domination";
		break;
   }
   return key;
}


function gamenumberFormatter (key, options, rowObject)
{
   return '<a href="../game/'+key+'">'+key+'</a>';
}

function mapFormatter (key, options, rowObject)
{
   return '<a href="../map/'+key+'">'+key+'</a>';
}

function timeFormatter (key, options, rowObject)
{
   return Math.floor(key/60)+':'+key%60;
}


function playerFormatter (cellvalue, options, rowObject)
{
	return '<a href="../player/'+cellvalue+'">'+rowObject[1]+'</a>';
}

function weaponFormatter (cellvalue, options, rowObject)
{
   key = cellvalue;
   switch(Number(cellvalue))
   {
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
   }
   return key;
}
