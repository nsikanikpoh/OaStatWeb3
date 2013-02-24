/*
===========================================================================
OaStatWeb2 - A web frontend for oastat
Copyright (C) 2012 Poul Sander

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
OaStatWeb2 as a whole is distributed under GPL version 3 or later 
with the following exception:
Additional permission under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or combining 
it with a compilable skin/template, the licensors of this Program grant you 
additional permission to convey the resulting work. Corresponding Source 
for a non-source form of such a combination shall include the source code 
for the parts of the skin/template used as well as that of the covered work. 
*/

#ifndef OASTATWEB_HPP
#define	OASTATWEB_HPP

#include <cppcms/application.h>
#include <boost/format.hpp>
#include <cppdb/frontend.h>
#include <boost/shared_ptr.hpp>
#include "OpenArenaConverters.hpp"
#include "plotgenerator.hpp"


class OaStatWeb3 : public cppcms::application
{
public:
	OaStatWeb3(cppcms::service &srv);
	virtual ~OaStatWeb3();
	void summary();
	void gamelist(std::string startCount);
	void onegame(std::string gamenumber);
	void playerpage(std::string playerid);
	void mappage(std::string mapname);
	void gametypepage(std::string gametypeNumber);
private:
	unsigned int getNumberOfGames();
	boost::shared_ptr<cppdb::session> sql;
	boost::shared_ptr<plotgenerator> plot;
	/**
	 * Checks to see if the connection is still alive. If it is not then reopen the connection.
	 * This function should be called by all pages in the beginning to ensure that the connection is alive.
	 * This is easy to overlook during testing. 
     */
	void CheckConnection();
	optconverter oaweapon;
	optconverter oagametype;
};

#endif	/* OASTATWEB_HPP */

