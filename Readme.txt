================================================================================
Project 3: Boiler-Plate Install Instructions                                   
================================================================================
Note: Much of the original code for toying with meshes has been replaced 
however, I left "procedural_geometry.h & .cc" and it is recommended that you
move the contents of TGeom into that

1. Create a new "Win32 Console Application" and name it w/e you want, make
   sure to click "Next" and create an empty application.

2. Drag and drop the src and lib directories into your project folder

3. In the solution explorer right click on the header and source folders 
   and click "add existing" and add the contents of the src and lib folders
   into the correct directories in your solution explorer

4. Access the "Tools" tab at the top of the window and choose "NuGet package editor"
   and search for + install the following packages:
   dirent
   glew
   glfw
   glm
   libjpeg
   nupengl

5. In the solution explorer right click on the bolded project name and choose properties
   at the bottom.

6. While in the properties menu, under C/C++ click the down arrow next to "additional
   include directories" click "edit" and then when the menu pops up click the yellow
   folder to add a new include source. Include the src and lib folders here.

7. Click the arrow next to linker to expand the options. Choose "input" and under 
   "additional dependencies add opengl32.lib; right before user32.lib

This should install everything and all files should be configured to avoid the
rest of the compile/link errors. 