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

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "makedesctext.h"
#include "gpx.h"
#include "nlohmann/json.hpp"
#include "tinyxml/tinyxml.h"

using json = nlohmann::json;

extern MAPSTRINGSTRING bridge_map;
extern MAPSTRINGSTRING opening_map;
extern MAPSTRINGSTRING radiocallinpoint_map;
extern MAPSTRINGSTRING fairway_map;
extern MAPSTRINGSTRING isrs_map;
extern MAPSTRINGSTRING operatingtimes_map;
extern MAPSTRINGSTRING lock_map;
extern MAPSTRINGSTRING chamber_map;

extern std::string LockIcon;
extern std::string BridgeClosedIcon;
extern std::string BridgeOpenIcon;

RWSbridges* gpxObj;
std::string SymbolFile;

//MAPSTRINGSTRING DescText_map;

std::vector<std::string> months = {"**", " jan", " feb", " mrt", " apr", " mei", " jun", " jul", " aug", " sep", " okt", " nov", " dec"};

void OpenGpxObj()
{
    gpxObj = new RWSbridges("RWS_Bruggen.gpx");
        gpxObj->OpenGpxFile(std::string( "vandaag" ));
}

void makebridgetext()
{
    int count = 0;
        
    for (auto const& x : bridge_map)
    {
        std::string desc, t ;
        json j = json::parse(x.second);
        if ( x.second.find("\"IsrsId\":") != std::string::npos )
        {
            if ( std::string("NL") != FindCountryCode(j.at("IsrsId")) )
                continue;
        }
        
//First line of output ("City") and ("Name")        
        if ( x.second.find("\"City\":") != std::string::npos )
        {
            t = j.at("City");
            if ( t.length() > 0 ) //There is a City name available
                t.append( std::string(": ") );
        }
        if ( x.second.find("\"FairwayId\":") != std::string::npos ) 
            t.append( FindFairwayInfo( j.at("FairwayId") ).append("\n") );
        if ( x.second.find("\"Name\":") != std::string::npos )
        {
            t.append(j.at("Name"));
        }
        if ( t.length() > 0 )
        {
            desc.append(t.append("\n") );
            t="";
        }        
        
//Second line, fixed/open and width height etc
        t = FindOpeningInfo( j.at("Id") );
        if ( t.length() <= 1 )
        {
            if ( x.second.find("\"CanOpen\":") != std::string::npos )
                if ( j["CanOpen"] )
                { 
                    t = "Beweegbaar";
                    SymbolFile = BridgeOpenIcon;
                }
                else
                {
                    t = "Vast  ";
                    SymbolFile = BridgeClosedIcon;
                }
                    
            if ( x.second.find("\"Width\":") != std::string::npos )
            {
                t.append("    Breedte: ");
                float z = j.at("Width");
                t.append( FloatToStrNonzeros( j.at("Width") ) );
                t.append("m");
            }
        }
        if ( t.length() > 0 )
        {
            desc.append(t.append("\n") );
            t="";
        }
//3e line Remote controlled? VHF channel and tel.no.
        if ( x.second.find("\"IsRemoteControlled\":") != std::string::npos )
            if( j.at("IsRemoteControlled") == true )
                t.append(std::string("Op afstand bediend.").append("\t"));
        if ( x.second.find("\"PhoneNumber\":") != std::string::npos )
            t.append(std::string("tel: ").append(j.at("PhoneNumber") ).append("\t")); 
        t.append(FindVHFInfo( j.at("Id") ));    
        if ( t.length() > 0 )
        {
            desc.append(t.append("\n") );
            t="";
        }
///4th line Operating Times
        if ( x.second.find("\"OperatingTimesId\":") != std::string::npos )
        {
            t.append( FindOperatingTimes( j.at("OperatingTimesId") ));
            SymbolFile = BridgeOpenIcon;
        }
        else
            SymbolFile = BridgeClosedIcon;
        
        if ( t.length() > 0 )
        {
            desc.append(t.append("\n") );
            t="";
        }

        std::cout << desc << std::endl << std::endl;
        std::string lat, lon;
        
        if ( x.second.find("\"Geometry\":") != std::string::npos )
            if ( GetLatLonFromPOINT( j.at("Geometry"), lat, lon))
            {
                gpxObj->AddWaypoint( lat, lon, std::string(""), SymbolFile, desc);
            }
    }
}

void makelocktext()
{
    int count = 0;
        
    for (auto const& x : lock_map)
    {
        std::string desc, t ;
        SymbolFile = LockIcon;
        json j = json::parse(x.second);
        if ( x.second.find("\"IsrsId\":") != std::string::npos )
        { // if not in NL don't border
            if ( std::string("NL") != FindCountryCode(j.at("IsrsId")) )
                continue;
        }
        
//First line of output ("City") and ("Name")        
        if ( x.second.find("\"City\":") != std::string::npos )
        {
            t = j.at("City");
            if ( t.length() > 0 ) //There is a City name available
                t.append( std::string(": ") );
        }
        if ( x.second.find("\"FairwayId\":") != std::string::npos ) 
            t.append( FindFairwayInfo( j.at("FairwayId") ).append("\n") );
        if ( x.second.find("\"Name\":") != std::string::npos )
        {
            t.append(j.at("Name"));
        }
        if ( t.length() > 0 )
        {
            desc.append(t.append("\n") );
            t="";
        }        
        
//2e line Remote controlled? VHF channel and tel.no.
        if ( x.second.find("\"IsRemoteControlled\":") != std::string::npos )
            if( j.at("IsRemoteControlled") == true )
                t.append(std::string("Op afstand bediend.").append("\t"));
        if ( x.second.find("\"PhoneNumber\":") != std::string::npos )
            t.append(std::string("tel: ").append(j.at("PhoneNumber") ).append("\t")); 
        t.append(FindVHFInfo( j.at("Id") ));    
        if ( t.length() > 0 )
        {
            desc.append(t.append("\n") );
            t="";
        }
        
        if ( x.second.find("\"NumberOfChambers\":") != std::string::npos )
        {
            int count =  j.at("NumberOfChambers");
            long int id = j.at("Id");
            
            MAPSTRINGSTRING::const_iterator ii;
                for ( ii = chamber_map.begin(); ii != chamber_map.end(); ii++)
                {
                    json chamber = json::parse(ii->second);
                    if ( (long int)chamber.at("ParentId") == id )
                    {
                        t.append("kolk").append( std::to_string (count)).append("\n");
                        if (chamber.find("Length") != chamber.end() ) t.append("Lengte: ").append(FloatToStrNonzeros(chamber.at("Length"))).append(" \t");
                        if (chamber.find("Width") != chamber.end() ) t.append("Breedte: ").append(FloatToStrNonzeros(chamber.at("Width"))).append(" \t");
                        if (chamber.find("SillDepthBeBu") != chamber.end() ) t.append("Diepte-Bu: ").append(FloatToStrNonzeros(chamber.at("SillDepthBeBu"))).append(" \t");
                        if (chamber.find("SillDepthBoBi") != chamber.end() ) t.append("Diepte-Bi: ").append(FloatToStrNonzeros(chamber.at("SillDepthBoBi")));
                        if (chamber.find("Note") != chamber.end() ) t.append("\n").append("Note: ").append(chamber.at("Note"));
                        t.append("\n");
                        count--;
                
                    if ( count <= 0 ) break;
                }
            }            
        }
        if ( t.length() > 0 )
        {
            desc.append(t.append("\n") );
            t="";
        }
        
        
///3th line Operating Times
        if ( x.second.find("\"OperatingTimesId\":") != std::string::npos )
        {
            t.append( FindOperatingTimes( j.at("OperatingTimesId") ));
        }
        
        if ( t.length() > 0 )
        {
            desc.append(t.append("\n") );
            t="";
        }

        std::cout << desc << std::endl << std::endl;
        std::string lat, lon;
        
        if ( x.second.find("\"Geometry\":") != std::string::npos )
            if ( GetLatLonFromPOINT( j.at("Geometry"), lat, lon))
            {
                gpxObj->AddWaypoint( lat, lon, std::string(""), SymbolFile, desc);
            }
    }
}

std::string FloatToStrNonzeros(float x)
{
    std::string toFormat = std::to_string (x);
    
    while( toFormat.find(".") < toFormat.length()-2
    && (toFormat.substr( toFormat.length() - 1, 1) == "0" ))
    {
        toFormat.pop_back();
    }
            
    return toFormat;
}

std::string FindOpeningInfo( long id)
{
    std::string r;
    MAPSTRINGSTRING::const_iterator it;
    it = opening_map.find(id);
    if (it == opening_map.end())
        return r;
    json j = json::parse(it->second);
    
     if (j.find("Type") != j.end() )
    {
        std::string Type = j.at("Type");
            if ( Type == "VST") r = "Vaste brug"; else
            if ( Type == "HEF") r = "Hefbrug"; else
            if ( Type == "DR")  r = "Draaibrug"; else
            if ( Type == "PON") r = "Pontonbrug"; else   
            if ( Type == "BC")  r = "Basculebrug"; else
            if ( Type == "OPH") r = "Ophaalbrug"; else
            if ( Type == "OKW") r = "Keersluis / Stuw"; else
            if ( Type == "KLP") r = "Klapbrug / Stuw"; else
            if ( Type == "DOP") r = "Dubbele ophaalbrug"; else
            if ( Type == "DBC") r = "Dubbele basculebrug"; else    
            if ( Type == "DDR") r = "Draaibrug"; else r = Type;
            r.append(" \t");
            if (j.find("Width") != j.end() )
                r.append ( std::string("Breedte: ").append( FloatToStrNonzeros(j.at("Width")).append(" \t") ) );
            if (j.find("HeightClosed") != j.end() )
            {
                if (j.at("Type") == "VST" )
                    r.append ( std::string("Hoogte: ").append( FloatToStrNonzeros(j.at("HeightClosed")).append(" \t") ) );
                else
                    r.append ( std::string("Hoogte(gesloten): ").append( FloatToStrNonzeros(j.at("HeightClosed")).append(" \t") ) );
            }
            if (j.find("HeightOpened") != j.end() )
                r.append ( std::string("Hoogte(open): ").append( FloatToStrNonzeros(j.at("HeightOpened")).append(" \t") ) );
            if (j.find("SillDepth") != j.end() )
                r.append ( std::string("Drempel: ").append( FloatToStrNonzeros(j.at("SillDepth")).append(" \t") ) );
            
            if (j.find("Note") != j.end() )
                r.append ( std::string("\n").append(j.at("Note")) );
    }
    return r;
}

std::string FindFairwayInfo( long id)
{
    std::string r;
    MAPSTRINGSTRING::const_iterator it;
    it = fairway_map.find(id);
    if (it == fairway_map.end())
        return r;
    json j = json::parse(it->second);
    if (j.find("Name") != j.end() )
        r = j.at("Name");
    return r;
}

std::string FindVHFInfo( long id)
{
    std::string r;
    MAPSTRINGSTRING::const_iterator it;
    it = radiocallinpoint_map.find(id);
    if (it == radiocallinpoint_map.end())
        return r;
    json j = json::parse(it->second);
    if (j.find("VhfChannels") != j.end() )
        r = std::string("VHF: ").append(j.at("VhfChannels").dump());
    return r;
}

bool GetLatLonFromPOINT(std::string point, std::string &lat, std::string &lon)
{
    bool returnval = true;
    std::string s = point;
    std::string::size_type p  = s.find('(');
    s = s.substr(p + 1);
    p  = s.find(' ');
    lon = s.substr(0, p);
    s = s.substr(p + 1);
    p  = s.find(')');
    lat = s.substr(0, p);
    if( lat.find('.') > lat.length()-3) //atleast 2 decimals in latitude
        returnval = false;
    else
        lat = lat.substr(0, p+4); // reduce to for digits behind decimal
    if( lon.find('.') > lon.length()-3) //atleast 2 decimals in longitude
        returnval = false;
     else
        lon = lon.substr(0, p+4); // reduce to for digits behind decimal
    if ( 7.25 < ::atof(lon.c_str()) )
        returnval = false;
    if ( 50.8 > ::atof(lat.c_str()) )
        returnval = false;
    return returnval;
}

std::string FindCountryCode( long id)
{
    std::string r;
    MAPSTRINGSTRING::const_iterator it;
    it = isrs_map.find(id);
    if (it == isrs_map.end())
        return r;
    json j = json::parse(it->second);
    if (j.find("CountryCode") != j.end() )
        r = j.at("CountryCode");
    return r;
}

std::string FindOperatingTimes( long id)
{
    std::string r;
    MAPSTRINGSTRING::const_iterator it;
    it = operatingtimes_map.find(id);
    if (it == operatingtimes_map.end())
    {
        return r; //not found
    }
    json j = json::parse(it->second);

    if (j.find("Note") != j.end() )
        r.append( j.at("Note") ).append("\n");

    if (j.find("OperatingPeriods") != j.end() )
    {
        json Operiods =  j["OperatingPeriods"]; 
        for (auto i : Operiods) 
        {
            std::string s = i.at("Start");
            std::string e = i.at("End");
            r.append( s.substr(2)).append( months[std::stoi(s.substr(0,2))]).append(" - ").append( e.substr(2)).append( months[std::stoi(e.substr(0,2))]).append("\n");
            json Orules =  i["OperatingRules"]; //{"
            for (auto ii : Orules)  
            {
                std::string day, fromto, note;
                json ii_json = ii; //copy to a local json to avoid iterator compare problems

                if( ii_json.find("IsMonday") != ii_json.end() ) if ( (bool) ii_json.at("IsMonday")) day.append("ma ");
                if( ii_json.find("IsTuesday") != ii_json.end() ) if ( (bool) ii_json.at("IsTuesday")) day.append("di ");
                if( ii_json.find("IsWednesday") != ii_json.end() ) if ( (bool) ii_json.at("IsWednesday")) day.append("wo ");
                if( ii_json.find("IsThursday") != ii_json.end() ) if ( (bool) ii_json.at("IsThursday")) day.append("do ");
                if( ii_json.find("IsFriday") != ii_json.end() ) if ( (bool) ii_json.at("IsFriday")) day.append("vr ");
                if( ii_json.find("IsSaturday") != ii_json.end() ) if ( (bool) ii_json.at("IsSaturday")) day.append("za ");
                if( ii_json.find("IsSunday") != ii_json.end() ) if ( (bool) ii_json.at("IsSunday")) day.append("zo ");
                if( ii_json.find("IsHoliday") != ii_json.end() ) if ( (bool) ii_json.at("IsHoliday")) day.append("fstd ");
                if( day == "ma di wo do vr ") day = "ma..vr";
                if( day == "za zo fstd ") day = "za/zo/fstd";
                if( day == "za zo ") day = "za/zo";
                if( day == "zo fstd ") day = "zo/fstd";
                if( day == "ma di wo do vr za zo ") day = "ma..zo";
                if( day == "ma di wo do vr za ") day = "ma..za";
                if( day == "ma di wo do vr za zo fstd ") day = "ma..zo & fstd";
                //If operatingtimes are available add them otherwise "----"
                if ( (ii_json.find("From") != ii_json.end())  && (ii_json.find("To") != ii_json.end() ) ) 
                    fromto = CalcTimes( ii_json.at(("From")), ii_json.at(("To"))  );
                else
                    fromto = " --- ";
                
                if( ii_json.find("Note") != ii_json.end() ) note.append("  ").append( ii_json.at("Note") );
                    
                r.append(day).append("\t").append(fromto).append( note ) .append("\n");
            }
        }
    }
    return r;
}

std::string CalcTimes( long from, long to)
{ // From & To are given in ms since epoch in UTC
    std::stringstream ss;
    int uur, min;
    from = ( (from + 3600000) % 86400000) / 60000; //add one hour, remove whole days and make minutes
    to = ( (to + 3600000) % 86400000) / 60000;
    min= from % 60;
    uur = (int)(from - min)/60;
    ss << std::setw(2) << std::setfill('0') << uur << ":" << std::setw(2) << std::setfill('0') << min << " - " ;

    min= to % 60;
    uur = (int)(to - min)/60;
    ss << std::setw(2) << std::setfill('0') << uur << ":" << std::setw(2) << std::setfill('0') << min;
    std::string s= ss.str();

    return s;
}
