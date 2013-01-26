/* 
 * File:   OpenArenaConverters.hpp
 * Author: poul
 *
 * Created on 19. januar 2013, 12:37
 */

#ifndef OPENARENACONVERTERS_HPP
#define	OPENARENACONVERTERS_HPP

#include "Bases.hpp"

class OaWeaponConverter : public BaseConverter {
public:
    virtual std::string getValue(std::string key);
};

#endif	/* OPENARENACONVERTERS_HPP */

