///////////////////////////////////////////////////////////////////////////
// Converter.h   : defines source code conversion to webpage functions   //
// ver 1.1                                                               //
//                                                                       // 
// Platform      : Visual Studio Enterprise 2017, Windows 10 Pro x64     //
// Author        : Anirudh Shankar Raman, Syracuse University            //
//                 CSE687 - Object Oriented Design, Spring 2019          //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodeConverter class which accepts DependencyTable
*  and uses its relationships to create linked webpages that point to each
*  other based on dependencies. It can also convert single files if needed.
*  The conversion process filters HTML special characters before printing
*  them into output files. The resulting output of this converter is a list
*  of files (vector<string>) of the created webpages. There is also an added
*  functionality where <div> and </div> are inserted at the start and end of
*  comments, classes and functions.
*
* Required Files:
 * ===============
 * FileSystem.h, FileSystem.cpp
 * display.h, display.cpp
 * dependencies.h, dependencies.cpp
*
*  Maintenance History:
* =======================
*  ver 1.1 - 14 Mar 2019
*  - second release
*  ver 1.0 : 6 Jan 2019
*  - first release
*/

#pragma once
#include<vector>
#include<iostream>
#include "../DirectoryNavigator/FileSystem/FileSystem.h"
#include "../display/display.h"
#include <fstream>
#include<string>
#include <algorithm>
#include <map>
#include"../DependencyAnalyzer/DependencyAnalyzer.h"

using TypeInfoT = std::map<std::string, std::map<std::size_t, DependencyAnalysis::TypeInfo>>;
namespace HTML
{
	class converter
	{
	public:

		converter();
		~converter();

		std::string cppToHtml(std::string filesToProcess_, std::map<std::string, std::vector<std::string>> depT, const TypeInfoT &typeTable);
		void dependenciesFetch(std::string &filesToProcess, const std::map<std::string, std::vector<std::string>> &depTable, std::ofstream &webpageOutput);
		void HandlingComments(std::vector<std::string> &filedata, const TypeInfoT &typeTable);
		void DivTagsAdded(std::string &file, std::vector<std::string> &filedata, const TypeInfoT &typeTable);
		void SwitchFn(size_t &i, std::vector<std::string> &filedata, std::map<std::size_t, DependencyAnalysis::TypeInfo>::const_iterator &iter2, size_t lineNo);
		void HandlingClasses(size_t &i, std::vector<std::string> &filedata, size_t &lineNo);
		void HandlingFunctions(size_t &i, std::vector<std::string> &filedata, size_t &lineNo);
		void HandlingSingleLnFOpen(size_t &i, std::vector<std::string> &filedata, size_t &lineNo);
		void HandlingSingleLnFClose(size_t &i, std::vector<std::string> &filedata, size_t &lineNo);
	};
};

