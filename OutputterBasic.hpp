/* 
 * File:   OutputterBasic.hpp
 * Author: poul
 *
 * Created on 18. januar 2013, 18:21
 */

#ifndef OUTPUTTERBASIC_HPP
#define	OUTPUTTERBASIC_HPP

#include "Bases.hpp"
#include <vector>

class OutputterBasic : public BaseOutputter {
public:
    virtual void addParameter(std::string p);

    virtual void addParameter(std::string p, optconverter &c);

    virtual void print(cppdb::result &res, std::string &outstr);

protected:
    std::vector<std::string> parameters;
    std::vector<optconverter> converters;
};

#endif	/* OUTPUTTERBASIC_HPP */

