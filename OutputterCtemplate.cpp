/* 
 * File:   OutputterCtemplate.cpp
 * Author: poul
 * 
 * Created on 18. januar 2013, 18:28
 */

#include "OutputterCtemplate.hpp"

using namespace std;

void OutputterCtemplate::print(cppdb::result &res, std::string &outstr) {
	ctemplate::TemplateDictionary dict(template_name);
	while(res.next()) {
		ctemplate::TemplateDictionary* single_entry = dict.AddSectionDictionary("ELEMENT");
		for(int i=0;i<OutputterBasic::parameters.size();i++) {
			std::string tmp;
			res >> tmp;
			single_entry->SetValue(parameters.at(i),converters.at(i) ? converters.at(i)->getValue(tmp) : tmp);
		}
	}
	ctemplate::ExpandTemplate(template_name, ctemplate::DO_NOT_STRIP, &dict, &outstr);

}