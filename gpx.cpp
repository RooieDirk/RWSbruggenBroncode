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

#include "gpx.h"

extern RWSbridges* gpxObj;
int ScaMin = 199999;
bool DoUseScaMin = true;

RWSbridges::RWSbridges( std::string fname)
{
    filename = fname.c_str();
}
void RWSbridges::OpenGpxFile(std::string PubDate)
{
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    XmlDoc.LinkEndChild( decl );
    gpx = new TiXmlElement( "gpx" );  
	XmlDoc.LinkEndChild( gpx );
        gpx->SetAttribute("OriginalSource", "RWS dataservices");
        gpx->SetAttribute("TimeStamp", PubDate.c_str());
    isDocLoaded = true;    
}
void RWSbridges::CloseGpxFile()
{
    XmlDoc.SaveFile(filename);
    isDocLoaded = false;
}

void RWSbridges::AddWaypoint(std::string lat, std::string lon, std::string name, std::string symbol, std::string description)
{
    TiXmlElement * wpt = new TiXmlElement( "wpt" );
    gpx->LinkEndChild( wpt );
        wpt->SetAttribute("lat", lat);
        wpt->SetAttribute("lon", lon);
        
    if ( DoUseScaMin){
        TiXmlElement * Ext = new TiXmlElement( "extensions" );
        wpt->LinkEndChild( Ext );        
            TiXmlElement * sca_min = new TiXmlElement( "opencpn:scale_min_max" );
            Ext->LinkEndChild( sca_min );
            std::string str = std::to_string(ScaMin);
            sca_min->SetAttribute( "UseScale", "true" );
            sca_min->SetAttribute( "ScaleMin", str );
    }
 
        
    TiXmlElement * aname = new TiXmlElement( "name" );
    wpt->LinkEndChild( aname );
    aname->LinkEndChild( new TiXmlText( name ));
    
    TiXmlElement * sym = new TiXmlElement( "sym" );
    wpt->LinkEndChild( sym );
    sym->LinkEndChild( new TiXmlText( symbol ));
    
    TiXmlElement * desc = new TiXmlElement( "desc" );
    wpt->LinkEndChild( desc );
    desc->LinkEndChild( new TiXmlText( description ));
}
