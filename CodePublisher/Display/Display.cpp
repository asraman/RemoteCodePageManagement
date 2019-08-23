///////////////////////////////////////////////////////////////////////////
// Display.h     : implements webpage display using browser functions    //
// ver 1.0                                                               //
//                                                                       // 
// Platform      : Visual Studio Enterprise 2017, Windows 10 Pro x64     //
// Author        : Anirudh Raman, Syracuse University                    //
//                 asraman@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////

#include "display.h"
#include "../Process/Process.h"

// Default Constructor
display::display()
{
}

// Default Destructor
display::~display()
{
}

// A function which takes an argument of vector of string and display a valid HTML file using process class
void display::showWebPage(std::vector<std::string> convertedFiles)
{
	std::cout << "\n  Demonstrating code pop-up in notepad";
	std::cout << "\n ======================================";

	Process p;
	p.title("test application");
	std::string appPath = "C:/Program Files/Mozilla Firefox/firefox.exe";  // path to application to start
	p.application(appPath);

	
	for (auto f:convertedFiles)
	{
		std::string cmdLine = "/A " + f; 
		p.commandLine(cmdLine);

		std::cout << "\n  starting process: \"" << appPath << "\"";
		std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";
		p.create();
		CBP callback = []() { std::cout << "\n  --- child process exited with this message ---"; };
		p.setCallBackProcessing(callback);
		p.registerCallback();

		WaitForSingleObject(p.getProcessHandle(), INFINITE);  // wait for created process to terminate
	}	
}

// Test stub to run Display
#ifdef TEST_display

int main()
{
	std::cout << "\n testing Display Package";
	std::vector<std::string> htmlFiles;
	display d;
	htmlFiles.push_back("./display.cpp.html");
	d.showWebPage(htmlFiles);
	std::cout << "Printing converted file names" << "\n";

	return 0;

}

#endif