Instructions:
0. Open the A8.pro file in the root of the git repository. (IMPORTANT: do not open "A8_An-Educational-App.pro" in the "A8_An-Educational-App" folder as the project file -- this will not build correctly!)
1. Place a standard linux build of Box2D in the same directory at the same level as the folder containing this document (i.e. would be ../ from this document).
2. Compile using GCC. This shouldn't fail, but on occasion, the auto-generated user-specific kit includes flags that don't respect the necessary build order. If the build fails, please try the following:
    i. On the left side of the Qt Creator editor, expand the A8 project folder, and right click on "AlgoSolver" > select "Build AlgoSolver"
    ii. Perform the same procedure as (i), but for Spritesheet-Lib
    iii. Perform the same procedure as (i) and (ii), but for A8_An-Educational-App
