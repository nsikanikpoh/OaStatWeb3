/* 
 * File:   OutputterCtemplate.hpp
 * Author: poul
 *
 * Created on 18. januar 2013, 18:28
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

