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
#include "main.h"
#include "readfiles.h"
#include "makedesctext.h"
#include "gpx.h"
#include <stdio.h>
#include <fstream>
#include <curl/curl.h>



// for convenience
using json = nlohmann::json;

/////////////////////////////////////////////////////////////////////////////////////////////
//Filenames  Change if needed
std::string LockIcon = std::string("Sluis"); //Do not add the ".png" extension
std::string BridgeClosedIcon = std::string("Brug_dicht"); //Do not add the ".png" extension
std::string BridgeOpenIcon = std::string("Brug_open"); //Do not add the ".png" extension
/////////////////////////////////////////////////////////////////////////////////////////////

extern RWSbridges* gpxObj;

extern MAPSTRINGSTRING bridge_map;
extern MAPSTRINGSTRING opening_map;
extern MAPSTRINGSTRING radiocallinpoint_map;
extern MAPSTRINGSTRING fairway_map;
extern MAPSTRINGSTRING isrs_map;
extern MAPSTRINGSTRING operatingtimes_map;
extern MAPSTRINGSTRING lock_map;
extern MAPSTRINGSTRING chamber_map;

std::string ServerAdress = "https://www.vaarweginformatie.nl/wfswms/dataservice/1.3/";
int GeoGeneration;
bool Active;
std::string PublicationDate;
std::string fbridges  = "bridges.gpx";
int TotalCount;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void GetGeoGeneration()
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    std::string s = ServerAdress + "geogeneration" ;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, s.c_str() ); 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);       
        json j = json::parse(readBuffer);
        GeoGeneration = j["GeoGeneration"];
        Active = j["Active"];
        PublicationDate = j["PublicationDate"];
    }        
}

void DownloadIntoMap(std::string rwsfilename, MAPSTRINGSTRING &the_map, std::string mapindex)
{
    json all;
    json result;
    int TotalCount = 1;
    
    for (int i = 0; i < TotalCount; i=i+100)// get the bridges 1 by 1   TotalCount
    {
        CURL *curl;
        CURLcode res;
        std::string readBuffer;
        std::string s = ServerAdress + std::to_string(GeoGeneration) + "/" + rwsfilename + "?offset=" + std::to_string(i) + "&count=100";
        
        std::cout <<  s  << "\r" << std::endl ;

        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, s.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            if ( i == 0)
            {
                all  = json::parse(readBuffer);
                result = all["Result"];
                TotalCount = (int)all["TotalCount"];
            }
            else
            {
                json j = json::parse(readBuffer);
                json r = j["Result"];
                for (int ii  = 0; ii < r.size(); ii++ )
                {
                    result[result.size()] = r[ii];
                }
            }
        }   
    }
//    all["Result"] = result;
    
//     if (exists_file(std::string("bridge.json")))
//     {
        int NoID = -1;
//         std::ifstream ifs("bridge.json");
 //        json all;// = json::parse(ifs);
 //       result = all["Result"];
        for (auto i : result) 
        {
 //           std::string s = i.dump();
            long l;
            json ii = i;
            if ( ii.find( mapindex.c_str() ) != ii.end() ) //If there is no "ID" key we would get an exception!
            {
                l = i.at(mapindex.c_str());                
            }
            else
               { l = NoID; NoID--; }
            
            the_map.insert(MAPSTRINGSTRING::value_type( l, ii.dump() ) );
        } 
        std::cout << "the_map.size():  " <<  the_map.size() << " " << NoID << std::endl;
//    }    
//     o << std::setw(4) << all << std::endl;
//     o.close();
}

void DoDownload(std::string SerName, std::string fname)
{
    std::ofstream o(fname.c_str() );
    json all;
    json result;
    int TotalCount = 1;
    
    for (int i = 0; i < TotalCount; i=i+100)// get the bridges 1 by 1   TotalCount
    {
        CURL *curl;
        CURLcode res;
        std::string readBuffer;
        std::string s = ServerAdress + std::to_string(GeoGeneration) + "/" + SerName + "?offset=" + std::to_string(i) + "&count=100";
        
        std::cout <<  s  << "\r" << std::endl ;

        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, s.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            if ( i == 0)
            {
                all  = json::parse(readBuffer);
                result = all["Result"];
                TotalCount = (int)all["TotalCount"];
            }
            else
            {
                json j = json::parse(readBuffer);
                json r = j["Result"];
                for (int ii  = 0; ii < r.size(); ii++ )
                {
                    result[result.size()] = r[ii];
                }
            }
        }   
    }
    std::cout << std::endl;
    all["Result"] = result;   
    o << std::setw(4) << all << std::endl;
    o.close();
}

int main(int argc, char **argv)
{
    GetGeoGeneration();
    DownloadIntoMap( std::string("lock"), lock_map, std::string("Id"));
    DownloadIntoMap( std::string("chamber"), chamber_map, std::string("Id"));
    DownloadIntoMap(std::string("bridge"), bridge_map, std::string("IsrsId"));
    DownloadIntoMap(std::string("fairway"), fairway_map, std::string("Id"));
    DownloadIntoMap(std::string("operatingtimes"), operatingtimes_map, std::string("Id"));
    DownloadIntoMap(std::string("isrs"), isrs_map, std::string("Id"));
    DownloadIntoMap(std::string("opening"), opening_map, std::string("Id"));
    DownloadIntoMap(std::string("radiocallinpoint"), radiocallinpoint_map, std::string("ParentId"));
//    InitMapfiles(); // Only if read from files instead of reading from www
    
    gpxObj = new RWSbridges("RWS_Bruggen.gpx");
        gpxObj->OpenGpxFile(std::string( "vandaag" ));
    
    makebridgetext();
    makelocktext();
    
    gpxObj->CloseGpxFile();
  
  return 0;
}
