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
========================= INCOMPLETE INSTRUCTIONS NEEDS TO BE EDITED =========================
These are instructions to install and run the robot
porgram for Project 3. These instructions are for WINDOWS machines

### POSSIBLE GEANY INSTALLATION GUIDE?


#### Installing SFML
1) Follow this link https://www.sfml-dev.org/download/sfml/2.5.1/ and at the bottom of the page you will see two MinGW versions; 32 bit and 64 bit. Make sure you download the version that corresponds with your MinGW version.

2) Once the SFML zip download is complete, right click the zip file and click 'Extract all'. A window should pop up asking you where you want to extract the files, make sure you choose your C: drive 

3) Once the extraction is complete, you will find in your C: drive a folder called 'SFML<...>' with the <...> being the version number. Rename the folder to just 'SFML'.

#### Installing the Project Files

1) Now head to the ENGR101 Project 3 page following this link https://ecs.wgtn.ac.nz/Courses/ENGR101_2020T1/Project3 and scroll all the way down to the bottom of the page where you will see a section of Attachments. Click 'AVC_Win10.zip' and this will initiate a download of the zip file.

2) Extract this zip file into any location you would like. Make sure you choose a reasonable and accessible location as these are the basis files of the project.

3) After this process has finished, you will see the folders 'AVC_robot' and 'AVC_server' in the chosen location of your extraction.

4) Now go ahead and open Geany. At the top left of the Geany window there should be an icon with a folder that says 'Open'. Click this and navigate to the 'AVC_robot' folder. Inside this folder you want to open 'makefile' AND 'robot.cpp'

#### Editing the AVC_robot makefile

1) Make sure you are viewing 'makefile'. Highlight all of the text with "Ctrl + A" and paste the following text:

INCLUDE = -I C:\\Users\Abigail\SFML\include
LIBS = -L C:\\Users\Abigail\SFML\lib
robot.exe: robot.o 
	g++ $(LIBS) -o robot robot.o -lsfml-window  -lsfml-graphics -lsfml-system -lsfml-network 
robot.o: robot.cpp 
	g++  -c $(INCLUDE) robot.cpp

2) Now we need to change the first and second lines of the makefile to YOUR specfic file path. In Libraries, head to your C: drive, go into the SFML folder and then click the include folder too. Copy the file path by clicking on the top bar and then "Ctrl + C".

3) Now open Geany, make sure you are viewing the makefile that you had opened and paste the file path so that the FIRST line reads: **INCLUDE = -I (insert file path here)**

 It should look like: **INCLUDE = -I C:\\SFML\include**
 
4) Time to do the same but with the second line! In Libraries, head to your C: drive, go into the SFML folder and then the lib folder. Copy the file path by clicking on the top bar and then "Ctrl + C".

5) Open Geany, in the makefile paste the file path in the SECOND line so that it reads: **LIBS = -L (insert file path here)**

 It should look like: **INCLUDE = -I C:\\SFML\lib**

#### Editing the AVC_robot makefile




