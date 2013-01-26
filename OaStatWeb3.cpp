#include "OaStatWeb3.hpp"
#include <cppcms/application.h>  
#include <cppcms/service.h>  
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
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
	
	
	dispatcher().assign("/", &OaStatWeb3::summary, this);
    mapper().assign("/");

    mapper().root("/oastatweb");
	
	CheckConnection();
	oaweapon = optconverter(new OaWeaponConverter());
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
	body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
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
	cppdb::result res = *sql<<"SELECT g.gamenumber,g.gametype, g.mapname, g.basegame,g.servername,g.time FROM oastat_games g "
			"WHERE 1 ORDER BY g.time DESC LIMIT ? OFFSET ?"<< limitCount << limitStart;
	optoutputter op(new OutputterCtemplate("gamelist.tpl") );
	op->addParameter("GAMENUMBER");
	op->addParameter("GAMETYPE");
	op->addParameter("MAPNAME");
	op->addParameter("BASEGAME");
	op->addParameter("SERVERNAME");
	op->addParameter("TIMESTAMP");
	string output;
	op->print(res,output);
	body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	stringstream gamelist_stream;
	gamelist_stream.str(std::string());
	gamelist_stream.clear();
	gamelist_stream << "Go to page: <br/>";
	for(int i=0;i<=numberOfGames/limitCount;++i) 
	{
		gamelist_stream << "<a href='" << url("/gamelist",i*limitCount)  << "'>" << i+1  <<"</a> ";
	}
	body_tpl.SetValueAndShowSection("BODY_ELEMENT",gamelist_stream.str(),"BODY_ELEMENT_LIST");
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
	body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	res = *sql<<"SELECT CASE k.MODTYPE WHEN 5 THEN 4 WHEN 7 THEN 6 WHEN 9 THEN 8 WHEN 13 THEN 12 ELSE k.MODTYPE END AS W,COUNT(0) AS C "
			"FROM oastat_kills k, oastat_players p WHERE k.target = p.playerid AND k.gamenumber = ? GROUP BY W ORDER BY C DESC"<<sgamenumber;
	output = "";
	OutputterCtemplate op2("list.tpl");
    op2.addParameter("WEAPON",oaweapon);
    op2.addParameter("COUNT");
    op2.print(res,output);
	body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	
	plot->gamescoregraph(sgamenumber);
	stringstream ss;
	ss << "<h2>Score graph</h2><img src=\"../media/scoretable" << sgamenumber << ".png\" alt=\"Scoregraph\"/>";
	body_tpl.SetValueAndShowSection("BODY_ELEMENT",ss.str(),"BODY_ELEMENT_LIST");
	
	string output2 = "";
    ctemplate::ExpandTemplate("body.tpl", ctemplate::DO_NOT_STRIP, &body_tpl, &output2);
    response().out() << output2 << endl;
};
void OaStatWeb3::playerpage(std::string playerid) {
	int sid = atoi(playerid.c_str());
	CheckConnection();
	ctemplate::TemplateDictionary body_tpl("body.tpl");
	body_tpl.SetValue("TITLE","Game summary");
	body_tpl.SetValue("SUBTITLE","Game - "+playerid);
	body_tpl.SetValue("ROOTPATH","..");
	
	cppdb::result res = *sql<<"SELECT g.gamenumber,g.gametype, g.mapname, g.basegame,g.servername,g.time FROM oastat_games g "
			"WHERE 'X' = (SELECT DISTINCT 'X' FROM oastat_userinfo ui1 WHERE ui1.gamenumber = g.gamenumber AND ui1.player = ?) "
			"ORDER BY g.time DESC LIMIT 10"<< sid;
	OutputterCtemplate op("gamelist.tpl");
	op.addParameter("GAMENUMBER");
	op.addParameter("GAMETYPE");
	op.addParameter("MAPNAME");
	op.addParameter("BASEGAME");
	op.addParameter("SERVERNAME");
	op.addParameter("TIMESTAMP");
	string output;
	op.print(res,output);
	body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	
	res = *sql<<"SELECT CASE k.MODTYPE WHEN 5 THEN 4 WHEN 7 THEN 6 WHEN 9 THEN 8 WHEN 13 THEN 12 ELSE k.MODTYPE END AS W,COUNT(0) AS C "
			"FROM oastat_kills k, oastat_players p WHERE k.attacker = ? GROUP BY W ORDER BY C DESC"<<sid;
	output = "";
	OutputterCtemplate op2("list.tpl");
    op2.addParameter("WEAPON",oaweapon);
    op2.addParameter("COUNT");
    op2.print(res,output);
	body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	
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
	//last 10 matches - start
	cppdb::result res = *sql<<"SELECT g.gamenumber,g.gametype, g.mapname, g.basegame,g.servername,g.time FROM oastat_games g "
			"WHERE MAPNAME = ? ORDER BY g.time DESC LIMIT 10"<<mapname;
	optoutputter op(new OutputterCtemplate("gamelist.tpl") );
	op->addParameter("GAMENUMBER");
	op->addParameter("GAMETYPE");
	op->addParameter("MAPNAME");
	op->addParameter("BASEGAME");
	op->addParameter("SERVERNAME");
	op->addParameter("TIMESTAMP");
	string output;
	op->print(res,output);
	body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	//last 10 matches - end
	//best weapons - start
	res = *sql<<"SELECT CASE k.MODTYPE WHEN 5 THEN 4 WHEN 7 THEN 6 WHEN 9 THEN 8 WHEN 13 THEN 12 ELSE k.MODTYPE END AS W,COUNT(0) AS C "
			"FROM oastat_kills k, oastat_games g WHERE k.gamenumber = g.gamenumber AND g.mapname = ? GROUP BY W ORDER BY C DESC"<<mapname;
    op = optoutputter(new OutputterCtemplate("list.tpl"));
    op->addParameter("WEAPON",oaweapon);
    op->addParameter("COUNT");
	output = "";
    op->print(res,output);
	body_tpl.SetValueAndShowSection("BODY_ELEMENT",output,"BODY_ELEMENT_LIST");
	//best weapons - end
	string output2 = "";
    ctemplate::ExpandTemplate("body.tpl", ctemplate::DO_NOT_STRIP, &body_tpl, &output2);
    response().out() << output2 << endl;
};

unsigned int OaStatWeb3::getNumberOfGames() {
	unsigned int result = 0;
	cppdb::result res = *sql<<"SELECT COUNT(0) FROM oastat_games";
	if(res.next()) {
		res >> result;
	}
	return result;
}