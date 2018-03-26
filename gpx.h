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

#ifndef GPX_H
#define GPX_H

#include "tinyxml/tinyxml.h"

// bool isDocLoaded;
// TiXmlDocument XmlDoc;
// char* filename;

// class RWSobjects
// {
// public:
//     RWSobjects( const char* fname);
//     bool isDocLoaded;
//     TiXmlDocument XmlDoc;
//     const char* filename;
//     
// }; //lat, lon, std::string(""), std::string( "Info-Info"), desc

class RWSbridges// : public RWSobjects
{
public:
    RWSbridges( const char* filename);
    void OpenGpxFile(std::string PubDate);
    void CloseGpxFile(void);
    void AddWaypoint(std::string lat, std::string lon, std::string name, std::string symbol, std::string description);
    bool isDocLoaded;
    TiXmlDocument XmlDoc;
    TiXmlElement * gpx;
    const char* filename;
};


#endif // GPX_H
