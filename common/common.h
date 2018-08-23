/* 
 * File:   common.h
 * Author: Lameguy64
 *
 * Created on October 3, 2017, 3:42 PM
 */

#ifndef COMMON_H
#define COMMON_H

#include <string>

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#define PI 3.14159263

int FileExists( const char* fileName );

void EnsureFileExtension(std::string& path, const char* extension);
void StripFileExtension(std::string& path);

void MakeFilePathRelative(std::string& fileName, const char* basePath);
int CalculateRelativePath(const char* path, std::string& output, 
	const char* basePath);

void StripPathname(std::string& path);
void StripFilename(std::string& path);

void Normalize(float* n, int count);

#endif /* COMMON_H */
