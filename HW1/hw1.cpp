/*******************************************************
 * Homework 1: Rasterization                           *
 *-----------------------------------------------------*
 * Here you will implement the circle rasterization    *
 * method decribed in the handout.                     *
 * To compile this in linux:                           *
 *        g++ hw1.cpp                                  *
 * Then, run the program as follows:                   *
 *        ./a.out 200                                  *
 * to generate a 200x200 image containing a circular   *
 * arc.  Note that the coordinate system we're using   *
 * places pixel centers at integer coordinates and     *
 * has the origin at the lower left.                   *
 * Your code will generate a .ppm file containing the  *
 * final image. These images can be viewed using       *
 * "display" in Linux or Irfanview in Mac/Windows.     *
 *******************************************************/

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
using namespace std;

// We'll store image info as globals; not great programming practice
// but ok for this short program.
int radius;
bool **image;

//this will determine which semi-circle the function will render.
bool type;

void renderPixel(int x, int y, bool type) {
    assert(x >= 0 && y >= 0 && x <= radius && y <= radius);
    
    // TODO:  light up the pixel's symmetric counterpart
    
    //the centers of the ppm file will act as the origin for the circle.
    int cx = radius/2;
    int cy = radius/2;
    //right top
    image[cx+y][cy+x] = 1;
    //right top-mid
    image[cx+x][cy+y] = 1;

    if(type==1)
    {
        //right bottom-mid
        image[cx-x][cy+y] = 1;
        //right bottom
        image[cx-y][cy+x] = 1;
    }

    if(type==0)
    {
        //left top-mid
        image[cx+x][cy-y] = 1;
        //left top
        image[cx+y][cy-x] = 1;
    }
}

void rasterizeArc(int r, bool type) 
{
    // TODO:  rasterize the arc using renderPixel to light up pixels
    
    // x y and r are based on the formula of a circle x^2+y^2=r^2
    int x = 0;
    int y = r;
    //decision varaible
    int d = 1 - r;

     // renders intial pixel of the arc
    renderPixel(x, y, type);
    //loops until the 45 degree limit is reached
    while (y >= x) 
    {
        //increments x to determine next pixel's placement
        x++;
        //if the d is greater than one the next pixel will be (x+1, y), otherwise it will be d is less than one it will be (x+1, y-1)
        if (d < 0)
            d += 2 * x + 3;
        else {
            d += 2 * (x - y) +5;
            y--;
        }
        //once chosen, pixel is rendered
        renderPixel(x, y, type);
    }
}

// You shouldn't need to change anything below this point.

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " circleSize\n";
        return 0;
    }

#ifdef _WIN32
    sscanf_s(argv[1], "%d", &radius);
#else
    sscanf(argv[1], "%d", &radius);
#endif
    if (radius <= 0) {
        cout << "Image must be of positive size.\n";
        return 0;
    }

    // reserve image as 2d array
    image = new bool*[radius+1];
    for (int i = 0; i <= radius; i++) image[i] = new bool[radius+1];

    // if the size of the resolution doesnt fit the semi-circle, the radiuses of the circles are shrunken proportionally to the res of the ppm file
    if(radius>=300)
    {   
        rasterizeArc(100, 1);
        rasterizeArc(150, 0);
    }
    
    else
    {
        rasterizeArc(radius/3, 1);
        rasterizeArc(radius/2, 0);
    }

    char filename[50];
#ifdef _WIN32
    sprintf_s(filename, 50, "circle%d.ppm", radius);
#else
    sprintf(filename, "circle%d.ppm", radius);
#endif

    ofstream outfile(filename);
    outfile << "P3\n# " << filename << "\n";
    outfile << radius+1 << ' ' << radius+1 << ' ' << 1 << endl;

    for (int i = 0; i <= radius; i++)
    for (int j = 0; j <= radius; j++)
        outfile << image[radius-i][j] << " 0 0\n";

    // delete image data
    for (int i = 0; i <= radius; i++) delete [] image[i];
    delete [] image;

    return 0;
}
