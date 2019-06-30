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
#include "log.h"
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
bool NeedValidSslCert = false;
std::string PublicationDate;
std::string OutputFileName  = "RWS_bridges.gpx";
int TotalCount, WriteCounter = 0;
extern int ScaMin;
extern bool DoUseScaMin;
char buffer[256];

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void GetInternetData(std::string &ReadBuffer, std::string url)
{
    CURL *curl;
    CURLcode res;
    char errbuf[CURL_ERROR_SIZE] ;
    curl = curl_easy_init();
     
    if(curl) {
        if ( NeedValidSslCert ){
            res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
            if (res != CURLE_OK ){
                log("SSL certificate error");
            }
        }
            //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str() ); 
            // provide a buffer to store errors in */
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf); 
            // set the error buffer as empty before performing a request */
        errbuf[0] = 0;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ReadBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl); 
        if (res != CURLE_OK ){
            size_t len = strlen(errbuf);
            if(len)
                sprintf( buffer, "libcurl: (%d) %s", res, errbuf );
            else
                sprintf( buffer, "libcurl: (%d) ", res );
            
            log(buffer);
        }
    }
    else
        log("Curl did not initialize!");

}

void GetGeoGeneration()
{
    std::string readBuffer;
    std::string s = ServerAdress + "geogeneration" ;
    GetInternetData( readBuffer, s);

    log(readBuffer.c_str());
    
    json j = json::parse(readBuffer);
    GeoGeneration = j["GeoGeneration"];
    Active = j["Active"];
    PublicationDate = j["PublicationDate"];
}

void DownloadIntoMap(std::string rwsfilename, MAPSTRINGSTRING &the_map, std::string mapindex)
{
    json all;
    json result;
    int TotalCount = 1;
    std::string logtext("Downloading " + rwsfilename);
    log( logtext.c_str() );
    for (int i = 0; i < TotalCount; i=i+100)// get the bridges 1 by 1   TotalCount
    {
        std::string readBuffer;
        std::string s = ServerAdress + std::to_string(GeoGeneration) + "/" + rwsfilename + "?offset=" + std::to_string(i) + "&count=100";
        
        GetInternetData( readBuffer, s);

        if ( i == 0)
        {
            all  = json::parse(readBuffer);
            result = all["Result"];
            TotalCount = (int)all["TotalCount"];
            std::cout <<  "Total of " << TotalCount << " " << rwsfilename << " to download" ;
        }
        else
        {
            json j = json::parse(readBuffer);
            json r = j["Result"];
            for (int ii  = 0; ii < r.size(); ii++ )
            {
                result[result.size()] = r[ii];
                std::cout <<  "."  ;
            }
        }
    }
    sprintf( buffer, "Total of %i %s downloaded", TotalCount, rwsfilename.c_str() );
    log( buffer);

        int NoID = -1;

        for (auto i : result) 
        {
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
}


void WriteConfigFile( const char* filename)
{
   json j_object = {{"Url", "https://www.vaarweginformatie.nl/wfswms/dataservice/1.3/"},
                    {"OutputFileName", "RWS_bridges.gpx"},
                    {"DoUseScaMin", true},
                    {"ScaMin", 199999},
                    {"NeedValidSslCert", false}
       
};
    std::ofstream out(filename);
    //out << "RWSbridges configuration:" << '\n';
    out << j_object.dump(4) << "\n\n";
    out.close();
}

int ReadConfigFile( const char* filename)
{
    std::ifstream in(filename);
    if (in)
    {        
       json conf = json::parse(in);
        
        ServerAdress = conf["Url"];
        OutputFileName = conf["OutputFileName"];
        DoUseScaMin = conf["DoUseScaMin"];
        ScaMin = conf["ScaMin"];
        NeedValidSslCert = conf["NeedValidSslCert"];
    }
    else
    {
        WriteConfigFile(filename);
        if(WriteCounter)
        {
            log("Did try to write a new config file but it doesn't work");
            return 1;
        }
        WriteCounter++;
        
    }

    return 0;
}

int main ( int argc, char *argv[] )
{
    initlog();
    log("Start logging.");
    ReadConfigFile("rws_bruggen.conf");

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
    std::array<char, 64> buffer;
    buffer.fill(0);
    time_t rawtime;
    time(&rawtime);
    const auto timeinfo = localtime(&rawtime);
    strftime(buffer.data(), sizeof(buffer), "%d-%m-%Y %H:%M", timeinfo);
    std::string timeStr(buffer.data());
    
    gpxObj = new RWSbridges(OutputFileName);
        gpxObj->OpenGpxFile( std::string( timeStr ));
    makebridgetext();
    makelocktext();
    
    gpxObj->CloseGpxFile();
  
  return 0;
}
