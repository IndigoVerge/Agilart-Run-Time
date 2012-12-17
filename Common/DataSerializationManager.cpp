/*******************************************************************************
 * Agilart Run-time runs on microcontrollers or other compatible hardware and
 * allows you to create, execute and monitor programs remotely via simple web API.
 * To make programs, Agilart Visual Designer can be used at www.agilart.com.
 *
 * Copyright (C) 2012  CompletIT (www.agilart.com), Ivan Dragoev (idragoev@completit.com),
 * Bobi Rakova (brakova@completit.com)
 *
 * This file is part of Agilart Run-time.
 *
 * Agilart Run-time is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Agilart Run-time is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************/

//#include <boost/archive/tmpdir.hpp>
#include <iostream>
#include <sys/stat.h>
#include "DataSerializationManager.h"
#include "Logging/EventLogger.h"

#include <fstream>
#include <string>

using namespace std;

DataSerializationManager::DataSerializationManager() {
	// TODO Auto-generated constructor stub

}

DataSerializationManager::~DataSerializationManager() {
	// TODO Auto-generated destructor stub
}

//void DataSerializationManager::SaveProgram( const ProgramDescriptor& descriptor, const char* file_name )
//{
//	ofstream ofs( file_name );
//	boost::archive::xml_oarchive xml( ofs );
//	xml << BOOST_SERIALIZATION_NVP( descriptor );
//
//	ofs.close();
//}
//
//void DataSerializationManager::LoadProgram( ProgramDescriptor& descriptor, const char* file_name )
//{
//	if( DataSerializationManager::FileExists( file_name ) == false )
//	{
//		EventLogger::Instance()->WriteVerbose( "Load program source file not found: %s", file_name );
//		return;
//	}
//
//	ifstream ifs( file_name );
//	boost::archive::xml_iarchive xml( ifs );
//	xml >> BOOST_SERIALIZATION_NVP( descriptor );
//
//	ifs.close();
//}
//
//void DataSerializationManager::SaveMapping( const ProgramMappingDescriptor& descriptor, const char* file_name )
//{
//	ofstream ofs( file_name );
//	boost::archive::xml_oarchive xml( ofs );
//	xml << BOOST_SERIALIZATION_NVP( descriptor );
//
//	ofs.close();
//}
//
//void DataSerializationManager::LoadMapping( ProgramMappingDescriptor& descriptor, const char* file_name )
//{
//	if( DataSerializationManager::FileExists( file_name ) == false )
//	{
//		EventLogger::Instance()->WriteVerbose( "Load program mappings source file not found: %s", file_name );
//		return;
//	}
//
//	ifstream ifs( file_name );
//	boost::archive::xml_iarchive xml( ifs );
//	xml >> BOOST_SERIALIZATION_NVP( descriptor );
//
//	ifs.close();
//}

bool DataSerializationManager::FileExists( const char* fileName )
{
	struct stat buf;

	if (stat(fileName, &buf) != -1)
		return true;

	return false;
}

void DataSerializationManager::Save( string content, string filename )
{
	ofstream ofs( filename.c_str() );
	ofs << content;
	ofs.close();
}

void DataSerializationManager::Delete( string filename )
{
	remove(filename.c_str());
}

string DataSerializationManager::Load( string filename )
{
//	if( DataSerializationManager::FileExists( filename.c_str() ) == false )
//	{
//		EventLogger::Instance()->WriteVerbose( "Cannot find file with name: %s", filename.c_str() );
//		return "";
//	}

	ifstream ifs( filename.c_str() );
	string content = "";
	string line;
	//ifs >> content;

	while ( ifs.good() )
	{
	      getline( ifs, line );
	      content += line;
	}

	ifs.close();

	return content;
}
