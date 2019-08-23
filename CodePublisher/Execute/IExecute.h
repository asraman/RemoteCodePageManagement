///////////////////////////////////////////////////////////////////////////
// IExecute.h  : defines an interface to a worker C++ class              //
// ver 1.1                                                               //
//                                                                       // 
// Application   : CSE-687 C++/CLI Shim demonstration                    //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Anirudh Shankar Raman, Syracuse University            //
//                 asraman@syr.edu                                       //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Description:
* ======================
*
*  The ExecutableFactory (shown below) can be used without the 'extern "C"'
*  marker but it is not recommended to do so. The marker will prevent
*  the compiler from doing any namemangeling on the function allowing
*  the use of PInvoke if needed. For this application it doesn't make
*  a difference so you can use either.
*
*  The created messages are meant to be shown in the WPF application.
*  the received messages are sent by the WPF application and displayed
*  on the console to demonstrate that native C++ code and C# code are
*  executing in the same program.
*
*  Public Interface:
* ===================
*  ExecutableFactory factory;
*
*  Build Command:
* ================
*  msbuild Execute.vcxproj
*
*  Maintainence History:
* =======================
*  ver 1.0 - March 23rd 2018
*    - first release
*
*/

#pragma once
#define IN_DLL
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
#include <vector>
// interface that allows Shim to use CppClass which contains
// CLR incompatible types (thread, condition variable and mutex)
// ONLY declare functions you WANT to use in the Shim and WPF app
class IExecutable {
public:
	virtual std::vector<std::string> get_Files(int argc, char ** argv) = 0;
	virtual ~IExecutable() {};
};


// The extern c is used to declare code in 'C'
extern "C" {
	// Instantiates Executable* as IExecutable*
	struct ExecutableFactory {
		// See CppLibrary.cpp for implementation
		DLL_DECL IExecutable* createClient();
	};
}