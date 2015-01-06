WPILib Documentation Can Be Found HERE
======================================
http://first.wpi.edu/FRC/roborio/release/docs/cpp/



robotRIO2015-1389
=================
All code running on the robotRIO for the 2015 season



SETTING UP NI TOOLS, WPILib, and Configuring Eclipse for the 2015 season
=========================================================================
Note: make sure all installations are done on the same drive

1. Download 2015 FRC Update: http://www.ni.com/download/first-robotics-software-2015/5112/en/

2. Install update

3. Download Eclipse Luna for C/C++: https://eclipse.org/downloads/packages/eclipse-ide-cc-developers/lunasr1

4. Extract Eclipse, and move the extracted directory to %ProgramFiles% (Generally C:\Program Files)

5. Download and install FRC WPIlib toolchains on your root directory: http://first.wpi.edu/FRC/roborio/toolchains/

6. Open up Eclipse. Go to Help -> Install New Software -> Add -> Set name to "FRC plugins" and location to "http://first.wpi.edu/FRC/roborio/release/eclipse/"
 -> Select all items that come up -> OK and install
 
7. Open up and example project by going to New -> Project -> WPIlib Robot C++ Development -> Example Robot C++ Project.

8. Try building the project. If it works, you're done. If you get an error along the lines of "g++ not found in PATH," proceed to steep 9




9. Download and install MinGW on your root Directory: http://sourceforge.net/projects/mingw/files/latest/download?source=files

10. Select mingw32-gcc-c++ (Mark it for installation)

11. Then go to Installation -> Apply Changes

12. Now, go to Start -> Control Panel -> System and Security -> System -> Advanced System settings -> Environment Variables. Scroll down to the System variables.
Locate your PATH variable. Select Edit. Select the very end of the variable(move your cursor right after the last semicolon) and add "C:\MinGW\bin;" to the end.
Hit OK, restart eclipse, and try building again. By this point, your project should be building properly.

Setting up and cloning the GitHub repo for the robotRIO code
============================================================
1. Download any GUI git (i.e. SourceTree, Windows Git, etc.)
3. Create a new WPIlib C++ project in eclipse. Clear the "src" folder of any files.
2. Select your service to github upon first install, sign in, and clone the directory "team1389/robotRIO2015-1389." Set your destination path as the "src" folder of
your eclipse project.
3. Select OK, clone, and restart eclipse.  
