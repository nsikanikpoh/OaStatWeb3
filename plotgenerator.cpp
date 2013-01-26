/* 
 * File:   plotgenerator.cpp
 * Author: poul
 * 
 * Created on 5. marts 2012, 20:28
 */

#include "plotgenerator.hpp"
#include "include/gnuplot_i.hpp"
#include <sys/stat.h>

using namespace boost;
using namespace std;

#define TOSTRING(INT) ((format("%i") % INT).str())

namespace {

const char* VALIDCHARS ="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789^*_-}{[]()#@.,/ +!:";

void cleanString(string &str) {
	size_t found;

	found=str.find_first_not_of(VALIDCHARS);
	while (found!=string::npos)
	{
			str[found]='*';
			found=str.find_first_not_of(VALIDCHARS,found+1);
	}
}

}

plotgenerator::plotgenerator(boost::shared_ptr<cppdb::session>  parentsql)
{
	sql = parentsql;
}

int plotgenerator::gamescoregraph(int gamenumber) {
	boost::format filename("media/scoretable%1%.png");
	filename % gamenumber;
	struct stat buffer;
	if ( stat( filename.str().c_str(), &buffer ) == 0 ) return 1 ;
	cppdb::result res;
	string name;
	int score,playerid,second = 0;
	vector<string> names;
	vector<int> finalscore; //scores of players at the end of match, the first element is the second
	vector<int> currentscore; //scores while building gnuplot data file, the first element is the second
	vector<int> playerids;
	string playerids_str;
	//Push default times
	finalscore.push_back(1000); //Will be reset later
	currentscore.push_back(0);
	res = *sql<<"SELECT p.playerid,p.nickname,s.score FROM oastat_games g, oastat_players p, oastat_points s " 
"WHERE  g.gamenumber = s.gamenumber AND p.playerid = s.player and g.gamenumber = ? "
"AND s.eventnumber = (select max(maxs.eventnumber) FROM oastat_points maxs where maxs.player = s.player AND maxs.gamenumber = g.gamenumber ) "
"ORDER BY s.score DESC LIMIT 0,8"<<gamenumber;
	while(res.next()) {
		res >> playerid >> name >> score; cleanString(name);
		playerids.push_back(playerid);
		names.push_back(name);
		finalscore.push_back(score);
		currentscore.push_back(0);
		if(playerids_str.length()) {
			playerids_str += ",";
		}
		playerids_str += TOSTRING(playerid);
	}
	
	if(playerids_str.length()) {
		vector< vector<int> > table;
		//table.push_back(currentscore);
		for(int i=0; i<playerids.size()+1;i++) {
			vector<int> acoloumn;
			acoloumn.push_back(0);
			table.push_back(acoloumn);
		}
		boost::format f("SELECT s.player, s.second, s.score FROM oastat_points s  WHERE s.gamenumber = ? AND s.player IN (%1%) ORDER BY SECOND");
		f % playerids_str;
		res = *sql<<f.str()<<gamenumber;
		while(res.next()) {
			res >> playerid >> second >> score;
			if(currentscore[0] == second)
				//table.pop_back(); //Only have one entry per second, remove the oldest one
			{
				for(int i=0; i<playerids.size()+1;i++)
					table.at(i).pop_back();
			}
			currentscore[0] = second; //time
			for(int i=0; i<playerids.size();i++) {
				if(playerids.at(i) == playerid ) {
					currentscore.at(i+1) = score;
				}
			}
			//table.push_back(currentscore);
			for(int i=0; i<playerids.size()+1;i++) {
				table.at(i).push_back(currentscore.at(i));
			}
		}
		if(table.at(0).back() != second) {
			finalscore[0] = second;
			//table.push_back(finalscore);
			for(int i=0; i<playerids.size()+1;i++) {
				table.at(i).push_back(finalscore.at(i));
			}
		}
		
		/*for(int i=0;i<playerids.size();i++) {
			cout << playerids.at(i) << " " << names.at(i) << endl;
		}
		for(int i=0;i<table.size();i++) {
			for(int j=0;j<table.at(i).size();j++) {
				cout << table[i][j] << " ";
			}
			cout << endl;
		}*/
		//Gnuplot::set_terminal_std("svg");
		Gnuplot g1("lines");
		g1.savetops("/dev/null");
		g1.set_title("Score over time");
		g1.set_xlabel("Time (seconds)");
		g1.set_ylabel("Score");
		g1.operator << ("set key left top" );
		for(int i=0;i<playerids.size();i++) {
			/*if(playerids.size()==i-1)
				g1.savetops("test_output");*/
			g1.plot_xy(table.at(0),table.at(i+1),names.at(i));
		}
		g1.cmd("set terminal png");
		boost::format set_output_filename("set output \"%1%.tmp\"");
		set_output_filename % filename;
		g1.cmd(set_output_filename.str());
		g1.replot();
		g1.remove_tmpfiles();
		//g1.showonscreen();
		
	}
	boost::format system_mv("mv \"%1%.tmp\" \"%1%\"");
	system_mv % filename;
	int retcode = system(system_mv.str().c_str());
	if(retcode) {
		cerr << "plotgeneretor error: mv failed with exitcode: " << retcode << " (is the \"media\" folder missing?) " << endl;
		return 2;
	}
	return 0;
}
