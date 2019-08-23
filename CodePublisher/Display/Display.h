#pragma once
///////////////////////////////////////////////////////////////////////////
// Display.h     : defines webpage display using browser functions       //
// ver 1.0                                                               //
//                                                                       // 
// Platform      : Visual Studio Enterprise 2017, Windows 10 Pro x64     //
// Author        : Anirudh Raman, Syracuse University                    //
//                 asraman@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines Display class which accepts a list of files as a
*  vector<string> and uses the default internet browser to display them
*  one by one. Please note that the functionality has limiations:
*   1) Opera/MS Edge: will pop-up all tabs instantly.
*   2) Chrome/Firefox: will pop-up windows separately only if no
*      already existing Chrome/Firefox window is opened (all must be
*      closed before running this).
*
* Required Files:
* ===============
* Process.h, Process.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 7 Jan 2019
*  - first release
*/

#include<vector>

class display
{
public:
	display();
	~display();
	//a function to display valid HTMl files using process class in the web browser
	void showWebPage(std::vector<std::string> convertedFiles);
};

