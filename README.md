/***********************************************************************************************************************************/


All thanks goes to Tamir Grosinger and Ben Gurion University for making this engine.
Link to the graphics engine: https://github.com/tamirgrBGU/CG3DBasicEngine.git



Projects by Matan Cohen: 
1. Functional executable Assignment1.cpp - Newton's Fractal in GLSL shaders.
    - Applies Newton's method on az^3 + bz^2 + cz + d where coeffs = (a,b,c,d)
    - press 1,2,3,4 to select coefficients a,b,c,d (respectively)
    - press key up and down to append and prepand (respectively) the selected coefficient by 0.01
    - Press key up and down to append and prepend (respectively) the number of newton's method iterations
    - Press space key to switch colors pallete
    - Double press D ( *two clicks in a row* ) to reset function, iteration number, color, zoom in and movement offset to default:
      function to z^3 - 1, iteration number to 0, color to color #1, zoom in to 1, movement to 0 xAxis and 0 yAxis
      zoom in to default (1) and
    - use scroll wheel up to zoom in (until visible ratio) and to print the width of each pixel
    - click and drag the fractal to move it around

/***********************************************************************************************************************************/


to run download the engine (git repo) from the link on top and change tutorial's folder content to be
the extracted tutorial.rar given here.

Engine For Animation Course
Graphic Engine based on Libigl

For compiling:
1. Clone or download the project
2. Download Cmake from the link in the site
3. Run Cmake gui. choose the project folder
and destination folder for the cpp project
files. choose after pressing configure
choose compiler (VS2019 for example).
After finish configuration successfully,
press configure again and after it finishes
press generate. 
4. If everything pass successfully got to the destination folder and launch the project. 
5. Copy configuration.txt from tutorial/sandBox to build/tutorial/sandBox sandBox as a startup project and compile the project (it could take few mineutes);   

