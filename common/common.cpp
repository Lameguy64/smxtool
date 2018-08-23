/* 
 * File:   common.cpp
 * Author: Lameguy64
 * 
 * Created on October 3, 2017, 3:42 PM
 */

#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include "common.h"

int FileExists( const char* fileName )
{
	struct stat fd;
	
	if ( stat( fileName, &fd ) == 0 )
	{
		return -1;
	}
	
	return 0;
}

void EnsureFileExtension( std::string& path, const char* extension )
{
	bool foundExtension = false;
	int i;
	
	for ( i=path.size()-1; i >= 0; i-- )
	{
		if ( path[i] == '.' )
		{
			foundExtension = true;
			break;
		}
		else if ( ( path[i] == '\\' ) || ( path[i] == '/' ) )
		{
			break;
		}
	}
	
	if ( foundExtension )
	{
		if ( path.compare( i+1, strlen( extension ), extension ) == 0 )
		{
			return;
		}

		path.erase( i+1, (path.length()-i)-1 );
		path.append( extension );
	}
	else
	{
		path.append( "." );
		path.append( extension );
	}
}

void StripFileExtension( std::string& path )
{
	bool foundExtension = false;
	int i;
	
	for ( i=path.size()-1; i >= 0; i-- )
	{
		if ( path[i] == '.' )
		{
			foundExtension = true;
			break;
		}
		else if ( ( path[i] == '\\' ) || ( path[i] == '/' ) )
		{
			break;
		}
	}
	
	if ( foundExtension )
	{
		path.erase( i, path.length()-i );
	}
}

void MakeFilePathRelative( std::string& fileName, const char* basePath )
{
	std::string tempA = fileName;
	std::string tempB = tempA;
	
	StripPathname( tempA );
	StripFilename( tempB );
	
	if ( CalculateRelativePath( tempB.c_str(), fileName, basePath ) == 0 )
	{
		fileName = tempA + tempB;
		return;
	}
	
#ifdef _WIN32
	fileName += "\\" + tempA;
#else
	fileName += "/" + tempB;
#endif
}

int CalculateRelativePath( const char* path, std::string& output, 
		const char* basePath )
{
	int diffBegin = 0;
	int numDirs = 0;
	int inDir = false;

	char basePathed[MAX_PATH];
	std::string	pathfix;

	if ( basePath != NULL )
	{
		strcpy( basePathed, basePath );
	}
	else
	{
		getcwd( basePathed, MAX_PATH );
	}

	if ( ( path[0] == '.' ) && ( ( path[1] == '\\' ) || ( path[1] == '/' ) ) )
	{
		pathfix.append(basePathed);
		pathfix.append(path+1);
		path = pathfix.c_str();
	}

	output.clear();

    while ( ( path[diffBegin] != 0 ) && ( basePathed[diffBegin] != 0 ) )
	{
		// Check for difference
		if ( tolower( basePathed[diffBegin] ) != tolower( path[diffBegin] ) )
		{

			if ( diffBegin < 2 )
			{
				return 0;
			}

            // Snap diffBegin to parent's slash character
            while ( ( basePathed[diffBegin-1] != '\\' ) && 
				( basePathed[diffBegin-1] != '/' ) )
			{
				diffBegin--;
			}

            // Count number of directories from the destination path
            for ( int i=diffBegin; i<strlen( basePathed ); i++ )
			{
				if ( ( !inDir ) && ( ( basePathed[i] != '\\' ) && 
					( basePathed[i] != '\\' ) ) )
				{
					inDir = true;
				}
				else if ( ( inDir ) && ( ( basePathed[i] == '\\' ) || 
					( basePathed[i] == '/' ) ) )
				{
					inDir = false;
					numDirs++;
				}
            }

            if ( inDir )
			{
				numDirs++;
			}

            for ( int i=0; i<numDirs; i++ )
			{
#ifdef _WIN32
				output += "..\\";
#else
				output += "../";
#endif
			}

            output += &path[diffBegin];
			
			return 1;
		}

		diffBegin++;

    }

    // If specified path is shorter than profile
    if ( strlen( basePathed ) > strlen( path ) )
	{
		// Count number of directories from the destination path
		for ( int i=diffBegin; i<strlen( basePathed ); i++ )
		{
			if ( ( !inDir ) && ( ( basePathed[i] != '\\' ) && 
				( basePathed[i] != '/' ) ) )
			{
				inDir = true;
			}
			else if ( ( inDir ) && ( ( basePathed[i] == '\\' ) || 
				( basePathed[i] == '/' ) ) )
			{
				inDir = false;
				numDirs++;
			}
		}

		if ( inDir )
		{
			numDirs++;
		}
		
		for ( int i=0; i<numDirs; i++ )
		{
#ifdef _WIN32
				output += "..\\";
#else
				output += "../";
#endif
		}

    }
	else if ( strlen( path ) > strlen( basePathed ) )
	{
		if ( (path[diffBegin] == '\\') || (path[diffBegin] == '/') )
		{
			diffBegin++;
		}
        output += &path[diffBegin];
    }

    return 1;
}

void StripPathname( std::string &path )
{
    size_t pos = path.rfind( "/" );

    if ( pos == std::string::npos )
    {
		pos = path.rfind( "\\" );
		if ( pos == std::string::npos )
		{
			return;
		}
    }

    path.erase( 0, pos+1 );
}

void StripFilename( std::string& path )
{
	size_t pos = path.rfind( "/" );

    if ( pos == std::string::npos )
    {
		pos = path.rfind( "\\" );
		if ( pos == std::string::npos )
		{
			return;
		}
    }

    path.erase( pos+1 );
}

void Normalize(float *n, int count) {
	float length=0;

    for ( int i=0; i<count; i++ ) {
		if ( fabs(n[i]) > length ) {
			length = fabs( n[i] );
		}
    }

    for ( int i=0; i<count; i++ ) {
		n[i] /= length;
	}
}