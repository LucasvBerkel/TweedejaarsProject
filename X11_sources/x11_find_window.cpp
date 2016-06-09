
/* ********** Compile with: ****************

g++ -o test x11_screen_grab.cpp -lGL -lGLU -lXrandr -lXext -lX11

******************************************** */


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <boost/python.hpp>

using namespace std;

#define ACTIVE_WINDOWS "_NET_CLIENT_LIST"

#define WINDOW_DOXBOX "DOSBox"

// Find the DOSBox Window so we can do cool stuff with it!
boost::python::object findDOSBoxWindow()
{
    Window window;
    Display *display = XOpenDisplay(NULL);
    Window rootWindow = RootWindow(display, DefaultScreen(display));    

    bool found = false;
    Atom atom = XInternAtom(display, ACTIVE_WINDOWS, true);
    Atom actualType;
    int format;
    unsigned long numItems;
    unsigned long bytesAfter;
    
    unsigned char *data = '\0';
    Window *list;    
    char *windowName;

    int status = XGetWindowProperty(display, rootWindow, atom, 0L, (~0L), false,
    AnyPropertyType, &actualType, &format, &numItems, &bytesAfter, &data);
    list = (Window *)data;

    
    if (status >= Success && numItems) {
        for (int i = 0; i < numItems; ++i) {
            status = XFetchName(display, list[i], &windowName);
            if (!windowName)
            {
                continue;
            }
            if (status >= Success) {
                string windowNameStr(windowName);
                if (windowNameStr.find(WINDOW_DOXBOX) == 0) {
                    window = list[i];
                    found = true;
                    break;
                }
            }

        }
    }

    XFree(windowName);
    XFree(data);
    XCloseDisplay(display);
    using boost::python::object;

    return object((int) window);
}


BOOST_PYTHON_MODULE(getwindow)
{
  namespace python = boost::python;
  python::def("findDOSBoxWindow", findDOSBoxWindow);
}


// int main(int argc, char *argv[])
// {
//     printf("%d\n", findDOSBoxWindow());
// }
