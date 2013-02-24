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

#ifndef OUTPUTTERCTEMPLATE_HPP
#define	OUTPUTTERCTEMPLATE_HPP

#include "OutputterBasic.hpp"
#include <ctemplate/template.h>

class OutputterCtemplate : public OutputterBasic {
public:
    OutputterCtemplate(std::string templateName) {
        template_name = templateName;
    }

    virtual void print(cppdb::result &res, std::string &outstr);
protected:
    std::string template_name;
};

#endif	/* OUTPUTTERCTEMPLATE_HPP */

