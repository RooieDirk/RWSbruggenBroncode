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
#include "readfiles.h"
#include <stdio.h>
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

MAPSTRINGSTRING bridge_map;
MAPSTRINGSTRING opening_map;
MAPSTRINGSTRING radiocallinpoint_map;
MAPSTRINGSTRING fairway_map;
MAPSTRINGSTRING isrs_map;
MAPSTRINGSTRING operatingtimes_map;


void InitMapfiles()
{
//     if (exists_file(std::string("bridge.json")))
//     {
//         int NoID = -1;
//         std::ifstream ifs("bridge.json");
//         json all = json::parse(ifs);
//         json result = all["Result"];
//         for (auto i : result) 
//         {
//             std::string s = i.dump();
//             long l;
//             if ( s.find("\"IsrsId\":") != std::string::npos ) //If there is no "ID" key we would get an exception!
//             {
//                 l = i.at("IsrsId");                    
//             }
//             else
//                { l = NoID; NoID--; }
//             
//             bridge_map.insert(MAPSTRINGSTRING::value_type( l, s ) );
//         } 
//         std::cout << "bridge_map.size():  " <<  bridge_map.size() << " " << NoID << std::endl;
//     }    
    if (exists_file(std::string("opening.json")))
    {
        int NoID = -1;
        std::ifstream ifs("opening.json");
        json all = json::parse(ifs);
        json result = all["Result"];
        for (auto i : result) 
        {
            std::string s = i.dump();
            long l;
            if ( s.find("\"ParentId\":") != std::string::npos ) //If there is no "ID" key we would get an exception!
            {
                l = i.at("ParentId");                    
            }
            else
               { l = NoID; NoID--; }
            
            opening_map.insert(MAPSTRINGSTRING::value_type( l, s ) );
        } 
        std::cout << "opening_map.size():  " <<  opening_map.size() << " " << NoID << std::endl;
    }
    if (exists_file(std::string("fairway.json")))
    {
        int NoID = -1;
        std::ifstream ifs("fairway.json");
        json all = json::parse(ifs);
        json result = all["Result"];
        for (auto i : result) 
        {
            std::string s = i.dump();
            long l;
            if ( s.find("\"Id\":") != std::string::npos ) //If there is now "ID" key we would get an exception!
            {
                l = i.at("Id");                    
            }
            else
               { l = NoID; NoID--; }
            
            fairway_map.insert(MAPSTRINGSTRING::value_type( l, s ) );
        } 
        std::cout << "fairway_map.size():  " <<  fairway_map.size() << " " << NoID << std::endl;
    }
    if (exists_file(std::string("radiocallinpoint.json")))
    {
        int NoID = -1;
        std::ifstream ifs("radiocallinpoint.json");
        json all = json::parse(ifs);
        json result = all["Result"];
        for (auto i : result) 
        {
            std::string s = i.dump();
            long l;
            if ( s.find("\"ParentId\":") != std::string::npos ) //If there is now "ID" key we would get an exception!
            {
                l = i.at("ParentId");                    
            }
            else
               { l = NoID; NoID--; }
            
            radiocallinpoint_map.insert(MAPSTRINGSTRING::value_type( l, s ) );
        } 
        std::cout << "radiocallinpoint_map.size():  " <<  radiocallinpoint_map.size() << " " << NoID << std::endl;
    }    
    if (exists_file(std::string("isrs.json")))
    {
        int NoID = -1;
        std::ifstream ifs("isrs.json");
        json all = json::parse(ifs);
        json result = all["Result"];
        for (auto i : result) 
        {
            std::string s = i.dump();
            long l;
            if ( s.find("\"Id\":") != std::string::npos ) //If there is now "ID" key we would get an exception!
            {
                l = i.at("Id");                    
            }
            else
               { l = NoID; NoID--; }
            
            isrs_map.insert(MAPSTRINGSTRING::value_type( l, s ) );
        } 
        std::cout << "isrs_map.size():  " <<  isrs_map.size() << " " << NoID << std::endl;
    }
    if (exists_file(std::string("operatingtimes.json")))
    {
        int NoID = -1;
        std::ifstream ifs("operatingtimes.json");
        json all = json::parse(ifs);
        json result = all["Result"];
        for (auto i : result) 
        {
            std::string s = i.dump();
            long l;
            if ( s.find("\"Id\":") != std::string::npos ) //If there is no "ID" key we would get an exception!
            {
                l = i.at("Id");                    
            }
            else
               { l = NoID; NoID--; }
            
            operatingtimes_map.insert(MAPSTRINGSTRING::value_type( l, s ) );
        } 
        std::cout << "operatingtimes_map.size():  " <<  operatingtimes_map.size() << " " << NoID << std::endl;
    }    
}

