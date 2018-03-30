/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018  Dirk Smits <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAKEDESCTEXT_H
#define MAKEDESCTEXT_H

#include <iostream>
#include "readfiles.h"
#include <stdio.h>
#include <fstream>
#include "nlohmann/json.hpp"

void makebridgetext();
void makelocktext();
std::string FloatToStrNonzeros(float x);
std::string FindOpeningInfo( long id);
std::string FindFairwayInfo( long id);
std::string FindVHFInfo( long id);
bool GetLatLonFromPOINT(std::string point, std::string &lat, std::string &lon);
std::string FindCountryCode( long id);
std::string FindOperatingTimes( long id);
std::string CalcTimes( long from, long to);
void OpenGpxObj();

#endif // MAKEDESCTEXT_H
