/* 
 * File:   plotgenerator.hpp
 * Author: poul
 *
 * Created on 5. marts 2012, 20:28
 */

#ifndef PLOTGENERATOR_HPP
#define	PLOTGENERATOR_HPP

//#include "include/gnuplot-iostream.h"
#include <iostream> 
#include <vector>
#include "boost/format.hpp"
#include <cppdb/frontend.h>

class plotgenerator
{
public:
	plotgenerator(boost::shared_ptr<cppdb::session> parentsql);
	
	int gamescoregraph(int gamenumber);
private:
	boost::shared_ptr<cppdb::session> sql;
};

#endif	/* PLOTGENERATOR_HPP */

