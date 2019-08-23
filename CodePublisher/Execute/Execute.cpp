///////////////////////////////////////////////////////////////////////////
// Execute.h : defines facade/executive for OOD Pr2 S19                  //
// ver 1.1                                                               //
//                                                                       // 
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Anirudh Shankar Raman, Syracuse University            //
//                 asraman@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "Execute.h"
#include "../CppParser/Parser/ConfigureParser.h"
#include "../CppParser/Parser/Parser.h"
#include "../CppParser/Parser/ActionsAndRules.h"
#include "../CppParser/AbstractSyntaxTree/AbstrSynTree.h"
#include "../CppParser/ScopeStack/ScopeStack.h"
#include<regex>
#include "IExecute.h"

using namespace CodeUtilities;
using namespace Utilities;
using namespace FileSystem;
using namespace HTML;
using namespace CodeAnalysis;

//default constructor
Execute::Execute()
{
}

//default destructor
Execute::~Execute()
{
}

//function to display list of files
void Execute::displayFiles(std::vector<std::string> Files) {
	for (auto file : Files)
		std::cout << "\n" << file; 
}

//function to display error message incase user enter invalid comand line argument
std::string customUsage()
{
	std::string usage;
	usage += "\n  Command Line: path [/option]* [/pattern]*";
	usage += "\n    path is relative or absolute path where processing begins";
	usage += "\n    [/option]* are one or more options of the form:";
	usage += "\n      /s - walk directory recursively";
	usage += "\n    [pattern]* are one or more pattern strings of the form:";
	usage += "\n      *.h *.cpp *.cs *.txt or *.*";
	usage += "\n";
	return usage;

}

//function to Processing comand line arguments and retrive files using regex
 inline std::vector<std::string> Execute::get_Files(int argc, char** argv) {
	ProcessCmdLine pcl(argc, argv);
	pcl.usage(customUsage());

	preface("Command Line: ");
	pcl.showCmdLine();
	pcl.showRegexes();

	if (pcl.parseError())
	{
		pcl.usage();
		_getche();
		return std::vector<std::string>();
	}

	DirExplorerN de(pcl.path());
	for (auto patt : pcl.patterns())
	{
		de.addPattern(patt);
	}

	if (pcl.hasOption('s'))
	{
		de.recurse();
	}

	de.search();

	std::vector<std::string> filesRetreived = de.fileToProcess();
	std::vector<std::string> CppToHtmlFiles;

	std::cout << "\n\t==> Before Regex is applied : ";
	//Execute obj;
	displayFiles(filesRetreived);
	if (pcl.regexes().size() > 0) {
		for (auto regeX : pcl.regexes()) {
			std::regex s(regeX);
			for (auto file : filesRetreived) {
				auto fileName = Path::getName(file);
				if (regex_match(fileName, s))
					CppToHtmlFiles.push_back(file);
			}
		}
	}
	else {
		CppToHtmlFiles = filesRetreived;
	}

	std::cout << "\n\t==> After Regex is applied : ";
	displayFiles(CppToHtmlFiles);

	HTML::converter conv;  //converter object
	std::vector<std::string> htmlFiles;  //new vector

	DependencyAnalysis object;
	std::map<std::string, std::vector<std::string>> depT;
	depT = object.summonParser(CppToHtmlFiles);
	std::map<std::string, std::map<std::size_t, DependencyAnalysis::TypeInfo>> typeInfo = object.returnTypeInfoTable();

	for (auto file : CppToHtmlFiles) {
		htmlFiles.push_back(conv.cppToHtml(file, depT, typeInfo));
	}

	return htmlFiles;
}
