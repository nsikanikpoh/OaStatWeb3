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

#include "plotgenerator.hpp"
#include <sys/stat.h>
#include <stdio.h>
#include <sstream>

using namespace boost;
using namespace std;

#define TOSTRING(INT) ((format("%i") % INT).str())

namespace {

const char* VALIDCHARS ="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*_-}{[]()#@.,/ +!:";

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

plotgenerator::plotgenerator(std::shared_ptr<cppdb::session>  parentsql)
{
	sql = parentsql;
}

int plotgenerator::gamescoregraph(int gamenumber) {
	boost::format filename("media/scoretable%1%.png");
	stringstream script_data;
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
		for(size_t i=0; i<playerids.size()+1;i++) {
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
				for(size_t i=0; i<playerids.size()+1;i++)
					table.at(i).pop_back();
			}
			currentscore[0] = second; //time
			for(size_t i=0; i<playerids.size();i++) {
				if(playerids.at(i) == playerid ) {
					currentscore.at(i+1) = score;
				}
			}
			//table.push_back(currentscore);
			for(size_t i=0; i<playerids.size()+1;i++) {
				table.at(i).push_back(currentscore.at(i));
			}
		}
		if(table.at(0).back() != second) {
			finalscore[0] = second;
			//table.push_back(finalscore);
			for(size_t i=0; i<playerids.size()+1;i++) {
				table.at(i).push_back(finalscore.at(i));
			}
		}
		
		for (size_t i=0; i<playerids.size(); ++i) {
			script_data << "\"" << names.at(i) << "\"\n";
			for (size_t j=0; j<table.at(i).size(); ++j) {
				script_data << table.at(0).at(j) << " " << table.at(i+1).at(j) << "\n";
			}
			script_data << "\n\n";
		}
		char name[] = "/tmp/gnuplotXXXXXX"; // tmp file in /tmp
		int fd = mkstemp(name);
		FILE* filepointer = fdopen(fd, "w+");
		if( filepointer == NULL) {
			unlink(name);
			return 3;
		}
		//cerr << "Data: " << script_data.str() << endl;
		int retval = fputs(script_data.str().c_str(),filepointer);
		if(retval >= 0) {
			retval= fflush(filepointer);
		}
		if(retval < 0) {
			unlink(name);
			fclose(filepointer);
			return 4;
		}
		boost::format system_gnuplot("./plot_script.gnuplot \"%1%\" \"%2%.tmp\" %3% 2> gnuplot.log");
		system_gnuplot % name % filename % (playerids.size()-1);
		//cerr << "Calling: " << system_gnuplot.str() << endl;
		int retcode = system(system_gnuplot.str().c_str());
		unlink(name);
		fclose(filepointer);
		if(retcode) {
			cerr << "gnuplot failed!" << endl;
			return 5;
		}
		
		
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
