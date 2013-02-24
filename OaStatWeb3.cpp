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

#include "OaStatWeb3.hpp"
#include <cppcms/application.h>
//#include <ctemplate/template_dictionary.h>  
#include <cppcms/service.h>  
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <ctemplate/template_dictionary.h>
#include "boost/lexical_cast.hpp"
#include <stdlib.h>
#include <boost/format.hpp>
#include <ctemplate/template.h>
//#include <c++/4.6/iosfwd>

#include "Bases.hpp"
#include "OutputterCtemplate.hpp"

using namespace std;
using namespace cppdb;


OaStatWeb3::OaStatWeb3(cppcms::service &srv) : cppcms::application(srv)
{
	dispatcher().assign("/gamelist/(\\d+)",&OaStatWeb3::gamelist, this ,1); 
	mapper().assign("gamelist","/gamelist/{1}"); 
	
	dispatcher().assign("/game/(\\d+)", &OaStatWeb3::onegame, this, 1);
    mapper().assign("game","/game/{1}");
	
	dispatcher().assign("/map/((\\w|-)+)", &OaStatWeb3::mappage, this, 1);
    mapper().assign("map","/map/{1}");
	
	dispatcher().assign("/player/(\\d+)", &OaStatWeb3::playerpage, this, 1);
    mapper().assign("player","/player/{1}");
	
	dispatcher().assign("/gametype/(\\d+)", &OaStatWeb3::gametypepage, this, 1);
	mapper().assign("gametype","/gametype/{1}");
	
	dispatcher().assign("/", &OaStatWeb3::summary, this);
    mapper().assign("/");

    mapper().root("/oastatweb");
	
	CheckConnection();
	oaweapon = optconverter(new OaWeaponConverter());
	oagametype = optconverter(new OaGametypeConverter());
	plot = boost::shared_ptr<plotgenerator>(new plotgenerator(sql));
}

OaStatWeb3::~OaStatWeb3()
{
}

void OaStatWeb3::CheckConnection() {
	if(!sql) {
		sql = boost::shared_ptr<cppdb::session>(new cppdb::session("mysql:database=oastat"));
	}
	try {
		//Execute some dummy sql:
		result r = *sql<<"SELECT 'X'";
		r.next();
	} catch (...) {
		//if the dummy sql fails then reconnect
		sql->open("mysql:database=oastat");
	}
}

void OaStatWeb3::summary() {
	CheckConnection();
	ctemplate::TemplateDictionary body_tpl("body.tpl");
	body_tpl.SetValue("TITLE","Summary page");
	body_tpl.SetValue("SUBTITLE","OAstat data");
	body_tpl.SetValue("ROOTPATH",".");
    cppdb::result res = *sql<<"SELECT CASE k.MODTYPE WHEN 5 THEN 4 WHEN 7 THEN 6 WHEN 9 THEN 8 WHEN 13 THEN 12 ELSE k.MODTYPE END AS W,COUNT(0) AS C "
			"FROM oastat_kills k GROUP BY W ORDER BY C DESC";
    optoutputter op(new OutputterCtemplate("list.tpl") );
    op->addParameter("WEAPON",oaweapon);
    op->addParameter("COUNT");
	string output;
    op->print(res,output);
	//body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	ctemplate::TemplateDictionary* weapon_kills = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
	weapon_kills->SetValue("ELEMENT_TITLE","Kill types");
	weapon_kills->SetValue("BODY_ELEMENT",output);
	string output2 = "";
    ctemplate::ExpandTemplate("body.tpl", ctemplate::DO_NOT_STRIP, &body_tpl, &output2);
    response().out() << output2 << endl;
};

void OaStatWeb3::gamelist(std::string startCount) {
	CheckConnection();
	unsigned int numberOfGames = getNumberOfGames();
	const int limitCount = 50;
	ctemplate::TemplateDictionary body_tpl("body.tpl");
	body_tpl.SetValue("TITLE","Gamelist");
	body_tpl.SetValue("SUBTITLE","Page");
	body_tpl.SetValue("ROOTPATH","..");
	int limitStart = atoi(startCount.c_str());
	cppdb::result res = *sql<<"SELECT g.gamenumber,g.gametype,g.gametype, g.mapname, g.basegame,g.servername,g.time FROM oastat_games g "
			"WHERE 1 ORDER BY g.time DESC LIMIT ? OFFSET ?"<< limitCount << limitStart;
	optoutputter op(new OutputterCtemplate("gamelist.tpl") );
	op->addParameter("GAMENUMBER");
	op->addParameter("GAMETYPE");
	op->addParameter("GAMETYPE_NAME",oagametype);
	op->addParameter("MAPNAME");
	op->addParameter("BASEGAME");
	op->addParameter("SERVERNAME");
	op->addParameter("TIMESTAMP");
	string output;
	op->print(res,output);
	//body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	ctemplate::TemplateDictionary* body_element = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
	body_element->SetValue("ELEMENT_TITLE","Game list");
	body_element->SetValue("BODY_ELEMENT",output);
	stringstream gamelist_stream;
	gamelist_stream.str(std::string());
	gamelist_stream.clear();
	gamelist_stream << "Go to page: <br/>";
	for(int i=0;i<=numberOfGames/limitCount;++i) 
	{
		gamelist_stream << "<a href='" << url("/gamelist",i*limitCount)  << "'>" << i+1  <<"</a> ";
	}
	//body_tpl.SetValueAndShowSection("BODY_ELEMENT",gamelist_stream.str(),"BODY_ELEMENT_LIST");
	body_element = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
	body_element->SetValue("ELEMENT_TITLE","Pages");
	body_element->SetValue("BODY_ELEMENT",gamelist_stream.str());
	string output2 = "";
    ctemplate::ExpandTemplate("body.tpl", ctemplate::DO_NOT_STRIP, &body_tpl, &output2);
    response().out() << output2 << endl;
};


void OaStatWeb3::onegame(std::string gamenumber) {
	int sgamenumber = atoi(gamenumber.c_str());
	CheckConnection();
	ctemplate::TemplateDictionary body_tpl("body.tpl");
	body_tpl.SetValue("TITLE","Game summary");
	body_tpl.SetValue("SUBTITLE","Game - "+gamenumber);
	body_tpl.SetValue("ROOTPATH","..");
	cppdb::result res = *sql<<"SELECT p.playerid,p.nickname,s.score FROM oastat_games g, oastat_players p, oastat_points s "
"WHERE  g.gamenumber = s.gamenumber AND p.playerid = s.player and g.gamenumber = ? "
"AND s.eventnumber = (select max(maxs.eventnumber) FROM oastat_points maxs where maxs.player = s.player AND maxs.gamenumber = g.gamenumber ) "
"ORDER BY s.score DESC"<<sgamenumber;
	OutputterCtemplate op("playertoplist.tpl");
	op.addParameter("PLAYERID");
	op.addParameter("NICKNAME");
	op.addParameter("SCORE");
	string output;
	op.print(res,output);
	ctemplate::TemplateDictionary* end_score = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
	end_score->SetValue("ELEMENT_TITLE","End score");
	end_score->SetValue("BODY_ELEMENT",output);
	res = *sql<<"SELECT CASE k.MODTYPE WHEN 5 THEN 4 WHEN 7 THEN 6 WHEN 9 THEN 8 WHEN 13 THEN 12 ELSE k.MODTYPE END AS W,COUNT(0) AS C "
			"FROM oastat_kills k, oastat_players p WHERE k.target = p.playerid AND k.gamenumber = ? GROUP BY W ORDER BY C DESC"<<sgamenumber;
	output = "";
	OutputterCtemplate op2("list.tpl");
    op2.addParameter("WEAPON",oaweapon);
    op2.addParameter("COUNT");
    op2.print(res,output);
	ctemplate::TemplateDictionary* most_kills = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
	most_kills->SetValue("BODY_ELEMENT",output);
	most_kills->SetValue("ELEMENT_TITLE","Most kills by weapon");
	try {
		plot->gamescoregraph(sgamenumber);
		stringstream ss;
		ss << "<img src=\"../media/scoretable" << sgamenumber << ".png\" alt=\"Scoregraph\"/>";
		ctemplate::TemplateDictionary* score_graph = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
		score_graph->SetValue("BODY_ELEMENT",ss.str());
		score_graph->SetValue("ELEMENT_TITLE","Score graph");
	} 
	catch (exception &e) {
		cerr << e.what() << endl;
	}
	
	string output2 = "";
    ctemplate::ExpandTemplate("body.tpl", ctemplate::DO_NOT_STRIP, &body_tpl, &output2);
    response().out() << output2 << endl;
};
void OaStatWeb3::playerpage(std::string playerid) {
	int sid = atoi(playerid.c_str());
	CheckConnection();
	string nickname;
	string output;
	ctemplate::TemplateDictionary body_tpl("body.tpl");
	body_tpl.SetValue("TITLE","Player page");
	//body_tpl.SetValue("SUBTITLE","Player - "+playerid);
	body_tpl.SetValue("ROOTPATH","..");
	cppdb::result res = *sql<<"SELECT lastseen,isbot,model,headmodel,nickname FROM oastat_players WHERE playerid = ?"<<sid;
	if(res.next()) {
		string lastseen,isbot,model,headmodel;
		res >> lastseen >> isbot >> model >> headmodel >> nickname;
		ctemplate::TemplateDictionary player_tpl("player.tpl");
		player_tpl.SetValue("lastseen",lastseen);
		player_tpl.SetValue("isbot",isbot);
		player_tpl.SetValue("model",model);
		player_tpl.SetValue("headmodel",headmodel);
		player_tpl.SetValue("nickname",nickname);
		ctemplate::ExpandTemplate("player.tpl", ctemplate::DO_NOT_STRIP, &player_tpl, &output);
		ctemplate::TemplateDictionary* player_info = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
		player_info->SetValue("BODY_ELEMENT",output);
		player_info->SetValue("ELEMENT_TITLE","Info");
	}	
	body_tpl.SetValue("SUBTITLE","Player - "+nickname);
	res = *sql<<"SELECT g.gamenumber,g.gametype,g.gametype, g.mapname, g.basegame,g.servername,g.time FROM oastat_games g "
			"WHERE 'X' = (SELECT DISTINCT 'X' FROM oastat_userinfo ui1 WHERE ui1.gamenumber = g.gamenumber AND ui1.player = ?) "
			"ORDER BY g.time DESC LIMIT 10"<< sid;
	OutputterCtemplate op("gamelist.tpl");
	op.addParameter("GAMENUMBER");
	op.addParameter("GAMETYPE");
	op.addParameter("GAMETYPE_NAME",oagametype);
	op.addParameter("MAPNAME");
	op.addParameter("BASEGAME");
	op.addParameter("SERVERNAME");
	op.addParameter("TIMESTAMP");
	output = "";
	op.print(res,output);
	ctemplate::TemplateDictionary* recent_games = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
	recent_games->SetValue("ELEMENT_TITLE","Recent games");
	recent_games->SetValue("BODY_ELEMENT",output);
	
	res = *sql<<"SELECT CASE k.MODTYPE WHEN 5 THEN 4 WHEN 7 THEN 6 WHEN 9 THEN 8 WHEN 13 THEN 12 ELSE k.MODTYPE END AS W,COUNT(0) AS C "
			"FROM oastat_kills k WHERE k.attacker = ? GROUP BY W ORDER BY C DESC"<<sid;
	output = "";
	OutputterCtemplate op2("list.tpl");
    op2.addParameter("WEAPON",oaweapon);
    op2.addParameter("COUNT");
    op2.print(res,output);
	ctemplate::TemplateDictionary* most_kills = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
	most_kills->SetValue("ELEMENT_TITLE","Most kills by weapon");
	most_kills->SetValue("BODY_ELEMENT",output);
	
	string output2 = "";
    ctemplate::ExpandTemplate("body.tpl", ctemplate::DO_NOT_STRIP, &body_tpl, &output2);
    response().out() << output2 << endl;
};

void OaStatWeb3::mappage(std::string mapname) {
	CheckConnection();
	ctemplate::TemplateDictionary body_tpl("body.tpl");
	body_tpl.SetValue("TITLE","Map - " + mapname);
	body_tpl.SetValue("SUBTITLE","Map summary");
	body_tpl.SetValue("ROOTPATH","..");
	string output2 = "";
	cout << "Mapname: " << mapname << endl;
	//Map info start
	{
		ctemplate::TemplateDictionary map_tpl("map.tpl");
		map_tpl.SetValue("MAPNAME",mapname);
		ctemplate::ExpandTemplate("map.tpl", ctemplate::DO_NOT_STRIP, &map_tpl, &output2);
		//body_tpl.SetValueAndShowSection("BODY_ELEMENT",output2,"BODY_ELEMENT_LIST");
		ctemplate::TemplateDictionary* body_element = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
		body_element->SetValue("ELEMENT_TITLE","Map information");
		body_element->SetValue("BODY_ELEMENT",output2);
    }
	//Map info end
	//last 10 matches - start
	cppdb::result res = *sql<<"SELECT g.gamenumber,g.gametype,g.gametype, g.mapname, g.basegame,g.servername,g.time FROM oastat_games g "
			"WHERE MAPNAME = ? ORDER BY g.time DESC LIMIT 10"<<mapname;
	optoutputter op(new OutputterCtemplate("gamelist.tpl") );
	op->addParameter("GAMENUMBER");
	op->addParameter("GAMETYPE");
	op->addParameter("GAMETYPE_NAME",oagametype);
	op->addParameter("MAPNAME");
	op->addParameter("BASEGAME");
	op->addParameter("SERVERNAME");
	op->addParameter("TIMESTAMP");
	string output;
	op->print(res,output);
	//body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	ctemplate::TemplateDictionary* body_element = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
	body_element->SetValue("ELEMENT_TITLE","Recent games on map");
	body_element->SetValue("BODY_ELEMENT",output);
	//last 10 matches - end
	//best weapons - start
	res = *sql<<"SELECT CASE k.MODTYPE WHEN 5 THEN 4 WHEN 7 THEN 6 WHEN 9 THEN 8 WHEN 13 THEN 12 ELSE k.MODTYPE END AS W,COUNT(0) AS C "
			"FROM oastat_kills k, oastat_games g WHERE k.gamenumber = g.gamenumber AND g.mapname = ? GROUP BY W ORDER BY C DESC"<<mapname;
    op = optoutputter(new OutputterCtemplate("list.tpl"));
    op->addParameter("WEAPON",oaweapon);
    op->addParameter("COUNT");
	output = "";
    op->print(res,output);
	//body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	body_element = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
	body_element->SetValue("ELEMENT_TITLE","Most kills by weapon");
	body_element->SetValue("BODY_ELEMENT",output);
	//best weapons - end
	output2 = "";
    ctemplate::ExpandTemplate("body.tpl", ctemplate::DO_NOT_STRIP, &body_tpl, &output2);
    response().out() << output2 << endl;
};

void OaStatWeb3::gametypepage(std::string gametypeNumber) {
	int gametype = atoi(gametypeNumber.c_str());
	CheckConnection();
	ctemplate::TemplateDictionary body_tpl("body.tpl");
	body_tpl.SetValue("TITLE",oagametype->getValue(gametypeNumber));
	body_tpl.SetValue("SUBTITLE","Gametype summary");
	body_tpl.SetValue("ROOTPATH","..");
	string output2 = "";
	//last 10 matches - start
	cppdb::result res = *sql<<"SELECT g.gamenumber,g.gametype,g.gametype, g.mapname, g.basegame,g.servername,g.time FROM oastat_games g "
			"WHERE g.gametype = ? ORDER BY g.time DESC LIMIT 10"<<gametype;
	optoutputter op(new OutputterCtemplate("gamelist.tpl") );
	op->addParameter("GAMENUMBER");
	op->addParameter("GAMETYPE");
	op->addParameter("GAMETYPE_NAME",oagametype);
	op->addParameter("MAPNAME");
	op->addParameter("BASEGAME");
	op->addParameter("SERVERNAME");
	op->addParameter("TIMESTAMP");
	string output;
	op->print(res,output);
	//body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	ctemplate::TemplateDictionary* body_element = body_tpl.AddSectionDictionary("BODY_ELEMENT_LIST");
	body_element->SetValue("ELEMENT_TITLE","Recent games");
	body_element->SetValue("BODY_ELEMENT",output);
	//last 10 matches - end
	output2 = "";
    ctemplate::ExpandTemplate("body.tpl", ctemplate::DO_NOT_STRIP, &body_tpl, &output2);
    response().out() << output2 << endl;
}

unsigned int OaStatWeb3::getNumberOfGames() {
	unsigned int result = 0;
	cppdb::result res = *sql<<"SELECT COUNT(0) FROM oastat_games";
	if(res.next()) {
		res >> result;
	}
	return result;
}
