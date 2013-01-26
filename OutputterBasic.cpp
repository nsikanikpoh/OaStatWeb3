/* 
 * File:   OutputterBasic.cpp
 * Author: poul
 * 
 * Created on 18. januar 2013, 18:21
 */

#include "OutputterBasic.hpp"

using namespace std;

void OutputterBasic::addParameter(std::string p) {
	parameters.push_back(p);
	optconverter c;
	converters.push_back(c);
};

void OutputterBasic::addParameter(std::string p, optconverter &c) {
	parameters.push_back(p);
	optconverter c2 = c;
	converters.push_back(c2);
};

void OutputterBasic::print(cppdb::result &res, std::string &outstr) {
	while(res.next()) {
		for(int i=0;i<parameters.size();i++) {
			string tmp;
			res >> tmp;
			cout << parameters.at(i) << ": " << ( converters.at(i) ? converters.at(i)->getValue(tmp) : tmp ) << ", ";
		}
	}
};
