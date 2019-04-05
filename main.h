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

#ifndef MAIN_H
#define MAIN_H

//#include "tinyxml/tinyxml.h"
//#include "gpx.h"
#include "nlohmann/json.hpp"
#include "readfiles.h"

typedef std::map <long, std::string> MAPSTRINGSTRING;

void DownloadIntoMap(std::string rwsfilename, MAPSTRINGSTRING &bridge_map, std::string mapindex);
void GetGeoGeneration();
void GetInternetData(std::string &ReadBuffer, std::string url);
#endif // MAIN_H
