# AVC Project Plan

## Team Name: Team 2 (how creative!)

## Team Members & contact info:
- abi6ail : Abigail Espejo abbie.espejo@gmail.com
- Zismac : Rhys Macdonald rhys.macdonald@protonmail.com
- MetallicHead : Thomas Yang yangthom@myvuw.ac.nz
- mcintogare : Gareth McIntosh mcintogare@myvuw.ac.nz

## Communication tool:
Discord

## Roles:
Abigail - Project Manager (organising team meetings, reporting regularly on progress)

Thomas Yang - Software Architect (writing core code and extending functionality)

Gareth and Rhys - Software writing, testing and documentation (debugging software and committing to
git, writing test cases and documenting performance against milestones)

## Milestone table

Date | Objectives | Tasks
--- | --- | ---
 03/06 | Finish plan | Update the README with plan
 04/06 | Get sfml installed and working | Make sure it works for everyone
 05/06 | Brainstorm requirements & tests for core | Discord call and brainstorm
 05/06 | Finish the core code | Thomas finish writing code for core 
 06/06 | Test the core code | Gareth & Rhys test the core code
 07/06 | Brainstorm requirements & tests for completion | Discord call and brainstorm
 09/06 | Finish the completion code | Thomas finish writing the completion code
 11/06 | Test the completion code | Gareth & Rhys test the completion code
 11/06 | Project progress report 1 | Abigail write up the progress report
 12/06 | Brainstorm requirements & tests for challenge | Discord call and brainstorm
 13/06 | Finish the challenge code | Thomas finish writing the challenge code
 18/06 | Project progress report 2 | Abigail write up the progress report
 18/06 | Test the challenge | Gareth & Rhys test the challenge code
 19/06 | Code due date | Submit the code
 24/06 | Report due date | Submit the report
 
 ## Installation guide
These are instructions to install and run the robot program for Project 3. These instructions are for Windows machines.

#### Installing MinGW C++ compiler
1) Download the MinGW C++ compiler [here](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/7.3.0/threads-posix/dwarf/i686-7.3.0-release-posix-dwarf-rt_v5-rev0.7z/download). The download should start automatically.

2) When the download is finished, at the bottom left you will see that **mingw-get-setup.exe** has been downloaded. Open it.

3) A window will pop up asking if you want to install, click **Install**.

4) Next it will ask you where you want to install MinGW, with your `C:` drive being the given option, this is fine so click **Continue**.

5) After the installation has finished (it will say 100%) then click **Continue** to be prompted to the next window.

6) A pop-up window will appear. Make sure that you selected all the check-boxes under the **Package** tab. e.g. mingw32-base, mingw32-gcc=g++, msys-base and so on.

7) At the top left corner of the window click **Installation > Apply Changes**. Wait for the process to complete. Once you see the successful installation message, close the window.

8) Head to the MinGW file that was installed in your `C:` drive. You want to find the `bin` folder and copy the file path to that folder.

9) Now open your **Control Panel** and click **System**, then **Advanced system settings**.

10) Click **Environment Variables**. In the section System Variables, find the `PATH` environment variable and select it, click **Edit**. If the `PATH` environment variable does not exist, click **New**.

11) In the **Edit System Variable** (or **New System Variable**) window, specify the value of the `PATH` environment variable. Paste the path value. Click OK. Close all remaining windows by clicking OK.

#### Installing SFML
1) Follow this link https://www.sfml-dev.org/download/sfml/2.5.1/ and at the bottom of the page you will see two MinGW versions; 32 bit and 64 bit. Make sure you download the version that corresponds with your MinGW version.

2) Once the SFML zip download is complete, right click the zip file and click 'Extract all'. A window should pop up asking you where you want to extract the files, make sure you choose your C: drive 

3) Once the extraction is complete, you will find in your C: drive a folder called 'SFML<...>' with the <...> being the version number. Rename the folder to just 'SFML'.

#### Installing the Project Files

1) Now head to the ENGR101 Project 3 page following this link https://ecs.wgtn.ac.nz/Courses/ENGR101_2020T1/Project3 and scroll all the way down to the bottom of the page where you will see a section of Attachments. Click 'AVC_Win10.zip' and this will initiate a download of the zip file.

2) Extract this zip file into any location you would like. Make sure you choose a reasonable and accessible location as these are the basis files of the project. After this process has finished, you will see the folders 'AVC_robot' and 'AVC_server' in the chosen location of your extraction.

#### Editing the AVC_robot makefile

1) Open Geany. At the top left of the Geany window there should be an icon with a folder that says 'Open'. Click this and navigate to the **AVC_robot** folder. Inside this folder you want to open **makefile**

2) In the makefile, highlight all of the text with **Ctrl + A** and paste the following text:

```
INCLUDE = -I C:\\Users\Abigail\SFML\include
LIBS = -L C:\\Users\Abigail\SFML\lib
robot.exe: robot.o 
	g++ $(LIBS) -o robot robot.o -lsfml-window  -lsfml-graphics -lsfml-system -lsfml-network 
robot.o: robot.cpp 
	g++  -c $(INCLUDE) robot.cpp
```

2) Now we need to change the first and second lines of the makefile to YOUR specfic file path for certain folders. In Libraries, head to your C: drive, go into the SFML folder, and then go into the include folder too. 

3) Copy the file path by clicking on the top bar that shows the file directory (it should automatically highlight when clicked) and then press **Ctrl + C**.

3) Go back to Geany where you had opened the makefile and paste the file path so that the **FIRST** line reads something similar to:

``` INCLUDE = -I C:\\SFML\include ```
 
4) For the second line it is a similar process. Again, head to your C: drive, go into the SFML folder and then the lib folder. Copy the file path by clicking the file directory on the top bar and pressing **Ctrl + C**.

5) Open Geany, in the makefile paste the file path in the **SECOND** line reads something similar to:

```INCLUDE = -I C:\\SFML\lib ```

6) Lastly, save the makefile by pressing **Ctrl + S** but DON'T close it.

#### Editing the AVC_server makefile

1) Great! We're going to do pretty much the same thing but with the **server** makefile. At the top left of the Geany window click 'Open', head to to the **AVC_server** folder and click **makefile**. 

2) In the **AVC_server** makefile, highlight all of the text with **Ctrl + A** and paste the following text:

```
INCLUDE = -I C:\\Users\Abigail\SFML\include
LIBS = -L C:\\Users\Abigail\SFML\lib
server3.exe: server3.o 
	g++ $(LIBS) -o server3 server3.o -lsfml-window  -lsfml-graphics -lsfml-system -lsfml-network 
server3.o: server3.cpp 
	g++  -c $(INCLUDE) server3.cpp
```

3) Now we just need to change those file paths on the first and second line. So for the first line, head to your AVC_robot makefile and copy the first line, go back to the in the AVC_server makefile and highlight the first line, then paste the copied text.

4) Go back to the AVC_robot makefile, highlight all of the second line, go back to the AVC_server makefile and highlight the second line, then paste the copied text. 

#### Changing the build command

1) Open Geany and you will see an icon with the label **Build**, click the drop down arrow next to it.

2) Click **Set build commands**

3) Under **Independent Commands**, your **Make** command should be: ```mingw32-make``` and **Make Custom Target...** should be: ```make```

4) After making the necessary changes hit OK.
