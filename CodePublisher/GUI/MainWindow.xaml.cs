///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for Project3HelpWPF                      //
// ver 1.2                                                           //
// Author : Anirudh Raman, asraman@syr.edu                           //
// Source : Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018//
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for Project3HelpWPF demo.  It's 
 * responsibilities are to:
 * - Provide a display of directory contents of a remote ServerPrototype.
 * - It provides a subdirectory list and a filelist for the selected directory.
 * - You can navigate into subdirectories by double-clicking on subdirectory
 *   or the parent directory, indicated by the name "..".
 *   
 * Required Files:
 * ---------------
 * Mainwindow.xaml, MainWindow.xaml.cs
 * Translater.dll
 * 
 * Maintenance History:
 * --------------------
 *  * ver 1.2 : 30 Apr 2019
 * - third release
 * ver 1.1 : 07 Aug 2018
 * - fixed bug in DirList_MouseDoubleClick by returning when selectedDir is null
 * ver 1.0 : 30 Mar 2018
 * - first release
 * - Several early prototypes were discussed in class. Those are all superceded
 *   by this package.
 */

// Translater has to be statically linked with CommLibWrapper
// - loader can't find Translater.dll dependent CommLibWrapper.dll

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;
using MsgPassingCommunication;
using System.IO;
using Path = System.IO.Path;
using System.Threading;
using System.Diagnostics;


namespace WpfApp1
{
  public partial class MainWindow : Window
  {
        private string working_directory { get; set; }

        private string Patterns { get; set; }

        private string Regex { get; set; }

        private List<String> files;

        public MainWindow()
        {
          InitializeComponent();
        }

    private Stack<string> pathStack_ = new Stack<string>();
    private Translater translater;
    private CsEndPoint endPoint_;
    private Thread rcvThrd = null;
    private Dictionary<string, Action<CsMessage>> dispatcher_ 
      = new Dictionary<string, Action<CsMessage>>();

        private int selfport;
        private int serverport;

    // ----< process incoming messages on child thread >----------------
    private void processMessages()
    {
      ThreadStart thrdProc = () => {
        while (true)
        {
          CsMessage msg = translater.getMessage();
          string msgId = msg.value("command");
          if (dispatcher_.ContainsKey(msgId))
            dispatcher_[msgId].Invoke(msg);
        }
      };
      rcvThrd = new Thread(thrdProc);
      rcvThrd.IsBackground = true;
      rcvThrd.Start();
    }

    //----< function dispatched by child thread to main thread >-------
    private void clearDirs()
    {
      lstFiles.Items.Clear();
    }

    //----< function dispatched by child thread to main thread >-------
    private void addDir(string dir)
    {
      lstFiles.Items.Add(dir);
    }

    //----< function dispatched by child thread to main thread >-------
    private void insertParent()
    {
      lstFiles.Items.Insert(0, "[..]");
    }

    //----< function dispatched by child thread to main thread >-------
    private void clearFiles()
    {
      lstFiles.Items.Clear();
    }

    //----< function dispatched by child thread to main thread >-------
    private void addFile(string file)
    {
      lstFiles.Items.Add(file);
    }

    //----< add client processing for message with key >---------------
    private void addClientProc(string key, Action<CsMessage> clientProc)
    {
      dispatcher_[key] = clientProc;
    }

    //----< load getDirs processing into dispatcher dictionary >-------
    private void DispatcherLoadGetDirs()
    {
      Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
      {
        Action clrDirs = () =>
        {
          clearDirs();
        };
        Dispatcher.Invoke(clrDirs, new Object[] { });
        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          if (key.Contains("dir"))
          {
            Action<string> doDir = (string dir) =>
            {
              addDir(dir);
            };
            Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
          }
        }
        Action insertUp = () =>
        {
          insertParent();
        };
        Dispatcher.Invoke(insertUp, new Object[] { });
      };
      addClientProc("getDirs", getDirs);
    }

    //----< load getFiles processing into dispatcher dictionary >------
    private void DispatcherLoadGetFiles()
    {
      Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
      {
        Action clrFiles = () =>
        {
        
        };
        Dispatcher.Invoke(clrFiles, new Object[] { });
        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          if (key.Contains("file"))
          {
            Action<string> doFile = (string file) =>
            {
              addFile(file);
            };
            Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
          }
        }
      };
      addClientProc("getFiles", getFiles);
    }

        //----< load the files that need to be converted into dispatcher dictionary >------
        private void DispatcherLoadConvertedFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    lstConvertedFiles.Items.Clear();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            lstConvertedFiles.Items.Add(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                        
                    }
                }
            };
            addClientProc("convertFiles", getFiles);
        }

        //----< load the the downloaded files after conversion into dispatcher dictionary >------
        private void DispatcherLoadDownloadedFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("name"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            var process = new Process();
                            process.StartInfo.FileName = "firefox";
                            process.StartInfo.Arguments = System.IO.Path.GetFullPath("../../../"+file);
                            process.Start();
                            process.WaitForExit();
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("downloadFiles", getFiles);
        }

        //----< load all dispatcher processing >---------------------------
        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherLoadConvertedFiles();
            DispatcherLoadDownloadedFiles();
        }

        //----< mouse double click event handler - opens HTML converted files >---------------------------
        private void LstConvertedFiles_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            endPoint_.machineAddress = "localhost";
            string selectedItem = lstConvertedFiles.SelectedItem.ToString();

            selectedItem = System.IO.Path.Combine("..\\..\\..\\..\\ConvertedPages", selectedItem);
            selectedItem = Path.GetFullPath(selectedItem);
            serverEndPoint.port = serverport;
            endPoint_.port = selfport;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.remove("command");
            msg.add("command", "downloadFiles");
            msg.add("file", selectedItem);
            msg.add("fileName", System.IO.Path.GetFileName(selectedItem));
            translater.postMessage(msg);
        }

        //----< start Comm, fill window display with dirs and files >------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string[] args = System.Environment.GetCommandLineArgs();
            selfport = int.Parse(args[1]);
            serverport = int.Parse(args[2]);

          // start Comm
          endPoint_ = new CsEndPoint();
          endPoint_.machineAddress = "localhost";
          endPoint_.port = selfport;
          translater = new Translater();
          translater.listen(endPoint_);

          // start processing messages
          processMessages();

          // load dispatcher
          loadDispatcher();

          CsEndPoint serverEndPoint = new CsEndPoint();
          serverEndPoint.machineAddress = "localhost";
          serverEndPoint.port = serverport;
            working_directory = Directory.GetCurrentDirectory() + "/../../../../";
            working_directory = Path.GetFullPath(working_directory);
            Patterns = "*.h *.cpp";
            Regex = "[A-C](.*)";
            txtPatterns.Text = Patterns;
            txtRegex.Text = Regex;

            txtPath.Text = ".";
          pathStack_.Push("../");
          CsMessage msg = new CsMessage();
          msg.add("to", CsEndPoint.toString(serverEndPoint));
          msg.add("from", CsEndPoint.toString(endPoint_));
          msg.add("command", "getDirs");
          msg.add("path", pathStack_.Peek());
          translater.postMessage(msg);
          msg.remove("command");
          msg.add("command", "getFiles");
          translater.postMessage(msg);

            Automated_Publish();
            demoReq1(); // demonstrating requirement 1
            demoReq2(); // demonstrating requirement 2
            demoReq3(); // demonstrating requirement 3
            demoReq4(); // demonstrating requirement 4
            demoReq5(); // demonstrating requirement 5
            demoReq6(); // demonstrating requirement 6
            demoReq7(); // demonstrating requirement 7
            demoReq8(); // demonstrating requirement 8

        }

    //----< strip off name of first part of path >---------------------
    private string removeFirstDir(string path)
    {
      string modifiedPath = path;
      int pos = path.IndexOf("/");
      modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
      return modifiedPath;
    }

    //----< respond to mouse double-click on dir name >----------------
    private void DirList_MouseDoubleClick(object sender, MouseButtonEventArgs e)
    {
      // build path for selected dir
      string selectedDir = (string)lstFiles.SelectedItem;
      if (selectedDir == null)
        return;
      string path;
      if(selectedDir == "[..]")
      {
        if (pathStack_.Count > 1)  // don't pop off "Storage"
          pathStack_.Pop();
        else
          return;
      }
      else
      {
        path = pathStack_.Peek() + "/" + selectedDir;
        pathStack_.Push(path);
      }

      // display path in Dir TextBlcok
      txtPath.Text = removeFirstDir(pathStack_.Peek());
      
      // build message to get dirs and post it
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = serverport;
      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(endPoint_));
      msg.add("command", "getDirs");
      msg.add("path", pathStack_.Peek());
      translater.postMessage(msg);
      
      // build message to get files and post it
      msg.remove("command");
      msg.add("command", "getFiles");
      translater.postMessage(msg);
    }

        // -----< Event for publish button >----------------------------------------- 
        private void BtnPublish_Click(object sender, RoutedEventArgs e)
        {
          Publish();
        }

        // ----------< Publish the files >--------------------------------------------
        private void Publish()
        {
            Console.WriteLine("\t==> Publish Files on double click");
            List<String> newfiles = new List<string>();
            newfiles.Add(System.IO.Path.GetFullPath("../../../../" + txtPath.Text.ToString()));
            newfiles.Add("/s");
            Patterns = txtPatterns.Text.ToString();
            newfiles.AddRange(Patterns.Split(' '));
            newfiles.Add(txtRegex.Text.ToString());
            string strArgs = "";
            foreach (var a in newfiles)
            {
                strArgs += a.ToString() + "##";
            }
            List<String> resultFiles = new List<String>();
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = serverport;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "convertFiles");
            msg.add("strArgs", strArgs);
            translater.postMessage(msg);
        }

        // --------------< A function to publish files automatically >-----------------
        private void Automated_Publish()
        {
            List<String> newfiles = new List<string>() ;
            newfiles.Add(System.IO.Path.GetFullPath(txtPath.Text.ToString()));
            newfiles.Add("/s");
            Patterns = txtPatterns.Text.ToString();
            newfiles.AddRange(Patterns.Split(' '));
            newfiles.Add(txtRegex.Text.ToString());
            string strArgs = "";
            foreach (var a in newfiles)
            {
                strArgs += a.ToString() + "##";
            }
            List<String> resultFiles = new List<String>();
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = serverport;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "convertFiles");
            msg.add("strArgs", strArgs);
            translater.postMessage(msg);
        }

        //-------< Demonstrating Requirement 1 >------------------------------------
        void demoReq1()
        {
            Console.WriteLine("\n--------------------------------------------------\n");
            Console.WriteLine("Demonstrating Requirement 1: \n");
            Console.WriteLine("Used Visual Studio 2017 and its C++ Windows Console Projects \n");
            Console.WriteLine("--------------------------------------------------\n");
        }

        //-------< Demonstrating Requirement 2 >------------------------------------
        void demoReq2()
        {
            Console.WriteLine("Demonstrating Requirement 2: \n");
            Console.WriteLine("Used Windows Presentation Foundation (WPF) and C# to implement the Client GUI.s \n");
            Console.WriteLine("--------------------------------------------------\n");
        }

        //-------< Demonstrating Requirement 3 >------------------------------------
        void demoReq3()
        {
            Console.WriteLine("Demonstrating Requirement 3: \n");
            Console.WriteLine("Assembled working parts from Projects #1, #2, and #3 into a Client-Server configuration. " +
                              "Unlike conventional Client-Server architectures, the Client need not wait for a reply from the " +
                              "Publisher server for a request before sending additional requests. \n");
            Console.WriteLine("Clients sending asynchronous messages to server\n");
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.port = serverport;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "echo");
            msg.add("demo_msg", "demo msg 1");
            translater.postMessage(msg);
            msg.add("command", "echo");
            msg.add("demo_msg", "demo msg 2");
            translater.postMessage(msg);
            msg.add("command", "echo");
            msg.add("demo_msg", "demo msg 3");
            translater.postMessage(msg);
            Console.WriteLine("\n-------------------------------------------------\n");
        }

        //-------< Demonstrating Requirement 4 >------------------------------------
        void demoReq4()
        {
            Console.WriteLine("Demonstrating Requirement 4: \n");
            Console.WriteLine("Provided a Graphical User Interface (GUI) for the client that supports navigating remote directories " +
                              "to find a project for conversion, " + "and supports displaying the conversion results " +
                              "in a way that meets Project #3 requirements. \n");
            Console.WriteLine("-------------------------------------------------\n");
        }

        //-------< Demonstrating Requirement 5 >------------------------------------
        void demoReq5()
        {
            Console.WriteLine("Demonstrating Requirement 5: \n");
            Console.WriteLine("Provided message designs appropriate for this application. " +
                              "All messages are instances of the same Message class, but have a specified set of attributes " +
                              "and body contents suited for the intended task. \n");
            Console.WriteLine("Structure of messages is as below\n");
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.port = serverport;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "echo");
            msg.add("demo_msg", "demo msg 1");
            msg.show();
            Console.WriteLine("\n-------------------------------------------------\n");
        }

        //-------< Demonstrating Requirement 6 >------------------------------------
        void demoReq6()
        {
            Console.WriteLine("Demonstrating Requirement 6: \n");
            Console.WriteLine("Supported converting source code in the server and, with a separate request, " +
                              "transferring one or more converted files back to the local client, " +
                              "using the communication channel. \n");
            Console.WriteLine("------------------------------------------------\n");
        }

        //-------< Demonstrating Requirement 7 >------------------------------------
        void demoReq7()
        {
            Console.WriteLine("Demonstrating Requirement 7: \n");
            Console.WriteLine("Demonstrated correct operations for two or more concurrent clients. \n");
            Console.WriteLine("------------------------------------------------\n");
        }

        //-------< Demonstrating Requirement 8 >------------------------------------
        void demoReq8()
        {
            Console.WriteLine("Demonstrating Requirement 8: \n");
            Console.WriteLine("Included an automated test that accepts the server url and remote path to " +
                              "my project directory on its command line, invokes the Code Publisher, through its interface, " +
                              "to convert all the project files matching a pattern that accepts *.h and *.cpp files, " +
                              "and then opens the Client GUI's Display view. \n");
            Console.WriteLine("------------------------------------------------\n");
        }
    }
}


