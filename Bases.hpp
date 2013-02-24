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

#ifndef BASES_HPP
#define	BASES_HPP

#include <iostream>
#include <cppdb/frontend.h>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

class BaseConverter {
public:
    virtual std::string getValue(std::string key) = 0;
};

typedef boost::shared_ptr<BaseConverter> optconverter;


class BaseOutputter {
public:
	/**
	 * Add a parameter you can extract from the sql query given
	 * Do not feed the object more parameters than the SQL have output
	 * The parameter will be passed to print function unmodifeid
     * @param p A parameter in all capital
     */
    virtual void addParameter(std::string p) = 0;
	/**
	 * Add a parameter you can extract from the sql query given
	 * Do not feed the object more parameters than the SQL have outpout
	 * The parameter will be processed by the converter before being passed
	 * to the print function
     * @param p A parameter in all capital
     * @param conv A converter object used for processing the parameter
     */
    virtual void addParameter(std::string p, optconverter &conv) = 0;
	/**
	 * This takes an SQL result set and prints it to a string
	 * The Outputter must already know all the parameters of the result set.
     * @param res A result set with a SQL SELECt statement inistialized
     * @param outstr The result of the print
     */
    virtual void print(cppdb::result &res, std::string &outstr) = 0;
};

typedef boost::shared_ptr<BaseOutputter> optoutputter;


#endif	/* BASES_HPP */

