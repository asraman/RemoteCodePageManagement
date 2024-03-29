///////////////////////////////////////////////////////////////////////
// ServerPrototype.h - Console App that processes incoming messages  //
// ver 1.1                                                           //
// Author: Anirudh Raman, asraman@syr.edu                            //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019 //
///////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../CppParser/FileSystem/FileSystem.h"
#include <chrono>
#include "../Execute/IExecute.h"

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

Files Server::getFiles(const Repository::SearchPath& path)
{
  return Directory::getFiles(path);
}

Dirs Server::getDirs(const Repository::SearchPath& path)
{
  return Directory::getDirectories(path);
}

template<typename T>
void show(const T& t, const std::string& msg)
{
  std::cout << "\n  " << msg.c_str();
  for (auto item : t)
  {
    std::cout << "\n    " << item.c_str();
  }
}

std::function<Msg(Msg)> echo = [](Msg msg) {
  Msg reply = msg;
  reply.to(msg.from());
  reply.from(msg.to());
  return reply;
};

std::function<Msg(Msg)> downloadFiles = [](Msg msg) {

	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("downloadFiles");
	reply.attribute("name","../DownloadedPages/" + msg.value("fileName"));
	return reply;
};

std::function<Msg(Msg)> convertFiles = [](Msg msg) {
	
	std::vector<std::string> args;
	std::vector<std::string> cmd;
	std::string delimiter = "##";

	size_t pos = 0;
	std::string token;
	std::string s = msg.value("strArgs");
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		cmd.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	char array[20][100] = { {0} };
	strcpy_s(array[0], "");
	std::cout << "\n-------- Command line Parameters in CPP have been recived -------- \n";
	for (size_t i = 1; i <= cmd.size(); i++) {
		strcpy_s(array[i], cmd[i - 1].c_str());
	}
	// converting into char** so that can be passed as argv
	char *ptr_array[20];
	for (int i = 0; i < 20; i++)
		ptr_array[i] = array[i];
	char **tmp_array = ptr_array;

	ExecutableFactory factory;
	IExecutable *obj = factory.createClient();
	std::vector<std::string> files;
	files = obj->get_Files(cmd.size()+1, tmp_array);
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("convertFiles");
	size_t count = 0;
	for (auto item : files)
	{
		std::string countStr = Utilities::Converter<size_t>::toString(++count);
		reply.attribute("file" + countStr, item);
	}
	return reply;
};

std::function<Msg(Msg)> getFiles = [](Msg msg) {
	
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getFiles");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files files = Server::getFiles(searchPath);
    size_t count = 0;
    for (auto item : files)
    {
      std::string countStr = Utilities::Converter<size_t>::toString(++count);
      reply.attribute("file" + countStr, item);
    }
  }
  else
  {
    std::cout << "\n  getFiles message did not define a path attribute";
  }
  return reply;
};

std::function<Msg(Msg)> getDirs = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getDirs");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files dirs = Server::getDirs(searchPath);
    size_t count = 0;
    for (auto item : dirs)
    {
      if (item != ".." && item != ".")
      {
        std::string countStr = Utilities::Converter<size_t>::toString(++count);
        reply.attribute("dir" + countStr, item);
      }
    }
  }
  else
  {
    std::cout << "\n  getDirs message did not define a path attribute";
  }
  return reply;
};

int main(int argc, char** argv)
{
  std::cout << "\n      Server Console";
  std::cout << "\n ==========================";

  int port = int(argv[1]);
  if (argc >= 2)
  {
	  std::istringstream iss(argv[1]);

	  if (iss >> port)
	  {
		  std::cout << "\n Port assigned to server as per user requirement!";
	  }
  }

  MsgPassingCommunication::EndPoint serverEndPoint("localhost", port);  // listening endpoint

  Server server(serverEndPoint, "ServerPrototype");
  server.start();
  server.addMsgProc("echo", echo);
  server.addMsgProc("getFiles", getFiles);
  server.addMsgProc("getDirs", getDirs);
  server.addMsgProc("serverQuit", echo);
  server.addMsgProc("convertFiles",convertFiles);
  server.addMsgProc("downloadFiles", downloadFiles);
  server.processMessages();
  
  server.stop();
  return 0;
}

