///////////////////////////////////////////////////////////////////////////
// Execute.h : defines facade/executive for OOD Pr2 S19                  //
// ver 1.1                                                               //
//                                                                       // 
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Anirudh Shankar Raman, Syracuse University            //
//                 asraman@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodePublisher which uses different components of
*  CodeConverter, Display and DirectoryN to put together the entire
*  functionality of the CodePublisher. The mentioned packages are indep-
*  ndent of each other and their only interaction is through this facade
*
 * Required Files:
 * ===============
 * Process.h, Process.cpp
 * Converter.h, Converter.cpp
 * codeUtilities.h, codeUtilities.cpp
 * DirExplorerN.h, DirExplorerN.cpp
 * Display.h, Display.cpp
 * ConfigureParser.h, ConfigureParser.cpp
 * Parser.h, Parser.cpp
 * ActionsAndRules.h, ActionsAndRules.cpp
 * AbstrSynTree.h, AbstrSynTree.cpp
 * ScopeStack.h, ScopeStack.cpp
 * DependencyAnalyzer.h, DependencyAnalyzer.cpp
 * IExecute.h, IExecute.cpp
*
*  Maintenance History:
* =======================
*  ver 1.1 - 06 Apr 2019
*  - second release
*  ver 1.0 - 14 Feb 2019
*  - first release
*/

#pragma once
#include "../Converter/Converter.h"
#include "../display/display.h"
#include "../DirectoryNavigator/Utilities/CodeUtilities/CodeUtilities.h"
#include "../DirectoryNavigator/DirExplorer-Naive/DirExplorerN.h"
#include "../CppCommWithFileXfer/Utilities/Utilities.h"
#include "../CppParser/Parser/ConfigureParser.h"
#include "../CppParser/Parser/Parser.h"
#include "../CppParser/Parser/ActionsAndRules.h"
#include "../CppParser/AbstractSyntaxTree/AbstrSynTree.h"
#include "../CppParser/ScopeStack/ScopeStack.h"
#include "../DependencyAnalyzer/DependencyAnalyzer.h"
#include "IExecute.h"
#include<conio.h>
#include<vector>
#define Util StringHelper


class Execute : public IExecutable
{
public:
	Execute();
	~Execute();
	void Req3();
	std::vector<std::string> get_Files(int argc, char ** argv) override;
	void displayFiles(std::vector<std::string> Files);
};

IExecutable* ExecutableFactory::createClient()
{
	return new Execute;
}