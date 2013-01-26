/* 
 * File:   Bases.hpp
 * Author: poul
 *
 * Created on 18. januar 2013, 18:17
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
    virtual void addParameter(std::string p) = 0;
    virtual void addParameter(std::string p, optconverter &conv) = 0;
    virtual void print(cppdb::result &res, std::string &outstr) = 0;
};

typedef boost::shared_ptr<BaseOutputter> optoutputter;


#endif	/* BASES_HPP */

