///////////////////////////////////////////////////////////////////////////
// DependencyAnalyzer.h : defines dependency analyzer structure          //
// ver 1.0                                                               //
//                                                                       // 
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Anirudh Raman, Syracuse University                    //
//                 asraman@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package provides a class DependencyAnalysis that is used to
*  analyze the dependencies of various files in the dependency table.
*  This module defines a Parser class.  Its instances collect
*  semi-expressions from one or more files for analysis.
*  Analysis consists of applying a set of rules to the semi-expression,
*  and for each rule that matches, invoking a set of one or more actions.
*  This is a header file used to declare the public and private variables.
*
*  Required Files:
*  ===============
*  FileSystem.h, FileSystem.cpp
*  display.h, display.cpp 
*
*  Maintainence History:
* =======================
*  ver 1.1 : 14 March 2019
*  - second release
*  ver 1.0 : 11 Feb 2019
*  - first release
*/

#pragma once

#include <vector>
#include <map>
#include "../CppParser/Parser/ConfigureParser.h"
#include "../CppParser/Parser/Parser.h"
#include "../CppParser/Parser/ActionsAndRules.h"
#include "../CppParser/AbstractSyntaxTree/AbstrSynTree.h"
#include "../CppParser/ScopeStack/ScopeStack.h"
#include "../CppCommWithFileXfer/Utilities/Utilities.h"
#define Util StringHelper

class DependencyAnalysis
{
public:
	DependencyAnalysis();
	~DependencyAnalysis();
	enum TypeInfo { none, classes, function, end, singleLineFn };
	std::map<std::string, std::vector<std::string>> summonParser(std::vector<std::string> files);
	void analyzeDepT(std::map < std::string, std::vector<std::string>> & depT);
	void DepTable(std::vector<std::string> files, CodeAnalysis::ASTNode* pGlobalScope, std::string fileSpec, std::map<std::string, std::vector<std::string>>& dependencyMap);
	void newInfoTable(CodeAnalysis::ASTNode * pGlobalScope, std::string file);
	TypeInfo findTypeInfo(CodeAnalysis::ASTNode* pGlobalScope);
	std::map<std::string, std::map<std::size_t, DependencyAnalysis::TypeInfo>> returnTypeInfoTable() { return typeInfoTable_; }

private:
	std::map<std::string, std::map<std::size_t, DependencyAnalysis::TypeInfo>> typeInfoTable_;
};

