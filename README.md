# Remote Code Page Management
> This is an academic project for the course CSE687. It is focused on building tools for software development. There are many state-of-the-art tools like Visual Studio, Gitlab, Github, which does the same but for many big companies, they use the tools of their own. The project provides you the opportunity to practice OOD principles with building big software, it also illustrates some common practice in the industry.

This project was built in 4 stages and the detailed information is as follows:
>I have built packages with the name Converter, Display, DependencyAnalyzer, Execute and GUI whereas the rest of the packags were provided by the instructor as helper codes for the implementation. With the help of utilization of the helper codes the software was developed in a span of 4 months. 


## Project Work Flow from Project-1 to Project-4 

![projectFlow](https://user-images.githubusercontent.com/49284567/63611648-5b657a00-c5aa-11e9-91c4-4985cc3bf3d5.jpg)

## Project #1 - Source Code Publisher 

### Finds and Loads a file for processing:
Files will be loaded when their names match a regular expression. That might be as simple as *.h & *.cpp, but will often be somewhat more complex. You will spend some time getting familiar with the C++11 regex classes. Files that match can be opened with the std::iostream functionality.
### Converts file into a webpage fragment:
This involves creating a new file with the same name, but with an html extension. Contents of the source code file are copied directly to the html file.
Then all html markup characters are replaced with their escape sequence equivalents, e.g., < is replaced by &lt, > is replaced by &gt, etc.
That text is prepended with "< pre >" and postpended with "< /pre >". That ensures that the rendered text preserves all of the white space included in the original file.
### Convert html fragment into a valid html document:
This is done by adding template text (from a template file) for a head section and the beginning of the body. Finally a small piece of template code, read from a template file is added to the end of the html fragment text.
At this point the file is viewable on a browser.

## Project #2 - Source Code Publisher for Projects

### Finds and loads files for Processing from a directory tree:
Starting at a path specified on the command line, recursively searches each folder in the directory tree rooted at the path, for files with names that match a regular expression. Each matching file is loaded and processed, using the functionalities developed in Project #1.
### Records file dependencies for each processed file: 
Dependencies will be based on #include statements found in each file. Dependencies are recorded with web links in the constructed web pages. Only #include statements that cite files in the directory tree are recorded with web links.
### Provides functionalities for optionally hiding comments and/or function bodies:
The intent is to allow users some flexibility in what is visible. We want to allow users to:

-> show or hide comments

-> show or hide function bodies

-> show just class names by hiding the entire class scope

This is done by wrapping text that may be hidden with a styled "< div >" tag. The "< div >" tags must be placed appropriately & in oder to do that you will need to do some relatively simple parsing using a parser.


## Project #3 - Code Publisher Client
![client-publisher](https://user-images.githubusercontent.com/49284567/63611763-9d8ebb80-c5aa-11e9-88a9-262d831295f4.jpg)

### Implement a tabbed display
The Client display will need at least two views, one for navigating to the directory to be analyzed, and one for displaying a published file.
### Navigation view:
The Client's navigation view supports navigating to the directory to be analyzed and supplying a file pattern, used for selecting files to show in the Display view.
### Display view:
The Display view shows a list of converted files for the project and allows the user to display a selected file either as a browser view, or in a pop up window showing the web page source.
This view also allows users to define what is visible. We want to allow users to:

-> show or hide comments

-> show or hide function bodies

-> show just class names by hiding the entire class scope

That will be indicated by checkboxes for each of the above.
### Provide Code Publisher interface
This step replaces the Execute package, from Project #2, with an interface and a class that implements the interface to use Code Publisher's functionalities.
### Connect to Code Publisher:
Build a C++\CLI translater to connect the Client GUI to the Code Publisher, built as a dynamic link library. The translater simply delegates function invocations in the GUI to calls into methods in the Code Publisher, using the Publisher's interface.


## Project #4 - Remote Code Page Management

![client-channel-server](https://user-images.githubusercontent.com/49284567/63611939-024a1600-c5ab-11e9-8cc4-491b76ba1d41.jpg)
### Implement a communication channel:
The communication channel was provided as the part of the helper code since it required more time to develop as the code consisted of buffer level interaction. 
### Design messages:
Designed messages for remote operations that mimic what you provided in Project #3. A Message class is provided as part of the communication channel implementation. Simply configure messages with that to trigger remote operations and to retrieve results.
### Develop a server:
A server is needed to accept request messages, and use the Project #2 Code Publisher functionality, as modified by Project #3 to carry out publishing operations.
The server has a message dispatcher that routes messages to the appropriate publication facilities. These diagrams will help you understand what is needed.
### Connect Client to Publisher Server:
Use the C++\CLI translater built in Project #3 to connect the Client GUI to the Code Publisher Server. That works in a manner very similar to the local connection you made there between Client and your local Code Publisher.

Software details - Developed on Windows 10:

```sh
Microsoft Visual Studio 2017 Enterprise
```

## Developer info

Anirudh Raman, 

asraman@syr.edu

## Instructor
Dr. James W Fawcett, 

Syracuse University, NY
