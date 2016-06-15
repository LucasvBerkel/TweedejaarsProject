
/* ********** Compile with: ****************

g++ -o test x11_screen_grab.cpp -lGL -lGLU -lXrandr -lXext -lX11

******************************************** */


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <boost/python/list.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>

#include <X11/Xutil.h>

using namespace std;
namespace py = boost::python;

// #define WINDOW_DOXBOX "DOSBox"

boost::python::list getFrame(int DOSBoxWindowID)
{
  Display *display = XOpenDisplay(NULL);
  Window rootWindow = RootWindow(display, DefaultScreen(display)); 
  // cout << display << "\n";
  // XWindowAttributes DOSBoxWindowAttributes;

  // XGetWindowAttributes(display, DOSBoxWindowID, &DOSBoxWindowAttributes);

  // int width = DOSBoxWindowAttributes.width; // Use predefined values and dosbox game bounds
  // int height = DOSBoxWindowAttributes.height;
  int width = 640;
  int height = 480;

  XColor colors;
  XImage *image;

  image = XGetImage(
     display, DOSBoxWindowID, 0, 0, width, height, AllPlanes, ZPixmap);


  unsigned long red_mask = image->red_mask;
  unsigned long blue_mask = image->blue_mask;


  py::list grays;

  // TO DO: nicer scale functionality
  // int rows = 0;
  // int cols = 0;
  int border_size = 97; // 97
  int scorebar_size = 35; // 35
  for (int i = 1; i < height-scorebar_size; ++i) { // Note the different increments for i and j
    // rows++;
    // cols = 0;
     for (int j = border_size; j < width - border_size; ++++j) {
        // cols++;
         colors.pixel = XGetPixel(image, j, i);
         grays.append(0.0512*(colors.pixel & blue_mask)  + 0.0182*((colors.pixel & red_mask) >> 16)); 
     }
  }
  // printf( "Cols: %d\n", cols);
  // printf("Rows: %d\n", rows);  
  XDestroyImage(image);
  XCloseDisplay(display);
  return grays;
}

BOOST_PYTHON_MODULE(getframe)
{
  namespace python = boost::python;
  python::def("getFrame", &getFrame);
}
