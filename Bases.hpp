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

