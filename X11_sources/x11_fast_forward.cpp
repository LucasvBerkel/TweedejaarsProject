#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <boost/python/def.hpp>
#include <boost/python/module.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

using namespace std;

void fastForward(int DOSBoxWindowID) // Add an fast forwarding amount
{
    Display *display = XOpenDisplay(NULL);
    Window rootWindow = RootWindow(display, DefaultScreen(display));    
    XKeyEvent event;
    event.type = KeyPress;      
    event.display = display;
    event.send_event = False;
    event.window = DOSBoxWindowID;
    event.root = rootWindow;
    event.time = CurrentTime;
    event.same_screen = True;
    // Use XStringToKeysym to convert string representations of keycodes from python to keycodes 
    // eg int key=XStringToKeysym("XK_Up");
    // printf("%ld and %ld\n", XStringToKeysym(code.c_str()), XK_space);
    event.keycode = XKeysymToKeycode(display, XK_2);
    // event.keycode = XK_asciitilde; 
    XSendEvent(display, DOSBoxWindowID, True, KeyPressMask, (XEvent *)&event);
    event.type = KeyRelease;
    XSendEvent(display, DOSBoxWindowID, True, KeyReleaseMask, (XEvent *)&event);
    // event.type = KeyPress;      
    // XSendEvent(display, DOSBoxWindowID, True, KeyReleaseMask, (XEvent *)&event);
    // int ms = 20; // Was originally defined at 100, see if shorter works out
    // usleep(ms * 1000); // Sleep in miliseconds
    // // XFlush(display);
    // event.type = KeyPress;      
    // XSendEvent(display, DOSBoxWindowID, True, KeyReleaseMask, (XEvent *)&event);
    // ms = 40; // Was originally defined at 100, see if shorter works out
    // usleep(ms * 1000); // Sleep in miliseconds
    // event.type = KeyRelease;
    // XSendEvent(display, DOSBoxWindowID, True, KeyReleaseMask, (XEvent *)&event);

    // event.type = KeyPress;      
    // XSendEvent(display, DOSBoxWindowID, True, KeyReleaseMask, (XEvent *)&event);
    // event.type = KeyPress;
    XFlush(display);


    XCloseDisplay(display);

}


BOOST_PYTHON_MODULE(fastforward)
{
  namespace python = boost::python;
  python::def("fastForward", &fastForward);
}


// int main(int argc, char *argv[])
// {

//     sendPress(39845891 ,  32); // 32 is space
// }
