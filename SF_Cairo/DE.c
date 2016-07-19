// Probably add in myvars.c somewhere
// OS X compilation
// gcc -Wall -g myvars.c DE.c -I/usr/local/include/cairo -L/usr/local/lib/ -lcairo `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -o DE
// Linux compilation
// gcc -Wall -g myvars.c DE.c -lm `pkg-config --cflags cairo` `pkg-config --libs cairo` `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`  -o DE
// Without gtk support:
// gcc -Wall -g DE.c -I/usr/local/include/cairo -L/usr/local/lib/ -lcairo -o DE

// To shared library: (comment out GTK related code for this to work) 
// gcc -I/usr/local/include/cairo -L/usr/local/lib/ -lcairo  -Wall -g -fPIC -c  myvars.c DE.c
// gcc -I/usr/local/include/cairo -L/usr/local/lib/ -lcairo -shared -o sf_frame_lib.so myvars.o DE.o


/* DISPLAY ELEMENTS	 6 Feb. 90 18:00
			definitions */
//#include <dos.h>
//#include <graphics.h>
#ifndef DE_H
#define DE_H
#include <math.h>
//#include <cairo.h>
#ifdef __APPLE__
	#include <cairo-quartz.h> // Is this available on linux? No!
#endif
//#include <gtk/gtk.h>
//#include <gdk/gdkkeysyms.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifndef GLOBALS
#define GLOBALS
#include "myconst.h"
#include "myext.h"
#endif

#include "DE.h"

//#include <boost/python/module.hpp>
//#include <boost/python/def.hpp>
//#include <boost/python/list.hpp>

/*									*/
/*	OPEN GRAPHICS: Initializes the graphics system and reports 	*/
/*	any errors which occured.					*/
/*									*/


void Open_Graphics(void)
{
//	int xasp,yasp;
//
//	// All old and should be replaced. (or probably is unneeded)
//	/* =	-	=	-	=	-	=	-	=	-	=	-	=	-	=	-	=	-	=	-	 */
//	/* Imported from graphics.h */
////	GraphDriver = DETECT;
//	/* Request auto-detection	*/
//	/*GraphMode=EGAHI;*/
////	initgraph( &GraphDriver, &GraphMode, "" );
////	ErrorCode = graphresult();		/* Read result of initialization*/
////	if( ErrorCode != grOk ){		/* Error occured during init	*/
////	exit( 1 );
////	}
//
//	//getpalette( &palette );		/* Read the palette from board	*/
//
//
//	// Lol, max color is probably in the millions now
////	MaxColors = getmaxcolor() + 1;	/* Read maximum number of colors*/
////
////	getaspectratio( &xasp, &yasp );	/* read the hardware aspect	*/
////	AspectRatio = (double)xasp / (double)yasp; /* Get correction factor	*/
////	GraphSqrFact=MaxX*AspectRatio/MaxY;		 /* for EGA cases */
////	setwritemode(XOR_PUT);
//	/* =	-	=	-	=	-	=	-	=	-	=	-	=	-	=	-	=	-	=	-	 */
}

// Maybe make an Initialize_Graphics just for SF? 
void Initialize_Graphics(cairo_t *cr)
{
	int Height,OldMaxX;
//	int t,t1; // t is unused
	int t1;
	int x,dx;

	MaxX = WINDOW_WIDTH;
	MaxY = WINDOW_HEIGHT;			/* Originally read the size of the screen	*/
//	SF_canvas = cr;

	// code works
//	int side_panel_size=(MaxX - MaxY)/2;
//	surface = cairo_quartz_surface_create(CAIRO_FORMAT_RGB24, MaxX, MaxY); // Notice that this isn't an image surface anymore
	// move the next two lines somewhere else

//	surface = cairo_image_surface_create(CAIRO_FORMAT_RGB16_565, MaxX, MaxY);
//	cr = cairo_create(surface);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);


//	cairo_set_line_width(cr, 10); // Line width equal to one pixel
	if(cairo_surface_get_type(cairo_get_target(cr) == CAIRO_SURFACE_TYPE_XLIB)
	{
		// Supply a value VAL between 100.0 and 240.0 (as a double)
		cairo_set_line_width(cr, (VAL * 1) / ((double) MaxY * 1));
	}
	else
	{
		cairo_set_line_width(cr, (90.1 * 1) / ((double) MaxY * 1)); // for image_surf use 239
	}
//	cairo_set_line_width(cr, (90.1 * 1) / ((double) MaxY * 1));

////	 Cairo uses a different coordinate system than graphics.h, so we reflect Cairo's through
////	 the x-asis to make it equal to that of graphics.h.
////	cairo_matrix_t x_reflection_matrix;
	// Reflecting it however means that text will also be reflected. We therefore also use a
	// reflection matrix for drawing fonts to reflect text back.
//	cairo_matrix_t font_reflection_matrix;
	// We need the options to turn off font anti-aliasing
	font_options = cairo_font_options_create();
//	cairo_matrix_init_identity(&x_reflection_matrix);
//	x_reflection_matrix.yy = -1.0;
//	cairo_set_matrix(cr, &x_reflection_matrix);

	// Mirror font back
	cairo_set_font_size(cr, 5.5); // 15.4 maximizes visibility while minimizing size
//	cairo_set_font_size(cr, 5.9);
//	cairo_get_font_matrix(cr, &font_reflection_matrix);
//	font_reflection_matrix.yy = font_reflection_matrix.yy * -1;
//	font_reflection_matrix.x0 += side_panel_size; // see (1)

//	cairo_set_font_matrix(cr, &font_reflection_matrix);
	// Translate negative height down because the reflection draws on top of the drawing surface
	// (i.e. out of frame, directly on top of the frame)

	// (1) Also translate the matrix over the x-axis to emulate the fact that DOS places the SF
	// square in the middle.
//	cairo_translate(cr, side_panel_size, -MaxY);
//	cairo_translate(cr, 0, -MaxY);

	// Turning off anti-alaising
	cairo_get_font_options(cr, font_options);
	cairo_font_options_set_antialias(font_options, CAIRO_ANTIALIAS_NONE);
	cairo_set_font_options(cr, font_options);
	cairo_select_font_face(cr,"Helvetica",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);

	cairo_path_t *empty_path = cairo_copy_path(cr);
	PrevShip = empty_path;
	// Sets all the values in the array to the empty path
	// Gives a vague warning, probably because this only works for types with the size of an int
	// (source: SO)
//	memset(PrevMissile, empty_path, MAX_NO_OF_MISSILES);  
//	PrevMissile = {empty_path};
	// Attemps above don't work, so we initialize manually
	for(int i = 0; i < MAX_NO_OF_MISSILES; i++)
	{
		PrevMissile[i] = empty_path;
	}
	
	PrevFort = empty_path;
	PrevMine = empty_path;

	// clears the screen, probably the dos screen, and sets the current graphics write
	// pointer to (0,0)
	//	cleardevice();

	//	The "textheight" function returns the height of a string in pixels.
//	Height=textheight("H"); /* Get basic text height */
	Height = TEXT_HEIGHT;

 	OldMaxX=MaxX;
  t1=4*Height;

  Panel_Y_End=MaxY;
  Panel_Y_Start=MaxY-t1+2;
  MaxY_Panel=Panel_Y_End-Panel_Y_Start;

  MaxY=MaxY-t1;
	MaxX=MaxY;

	// Any modern graphics library should handle this "if" statements themselves, if needed at
	// all because we don't really need to know anymore wether or not we are on a vga display.
	// This aspect ratio stuff has to do with the fact if your display has square pixels or not.
	// AspectRatio is defined in opengraphics. Pixels are always square nowadays
//	if(AspectRatio==1.0) /* VGA HI */ square pixel ratio
//	MaxX=MaxY;
//	else	/* for all others */
//	{
//		MaxX=MaxX*AspectRatio;	/********* MaxX and MaxY give a square */
//		MaxX=MaxX-t1/AspectRatio;	/******** less two panel lines */
//	}
	Xmargin=OldMaxX/2-MaxX/2;
	cairo_translate(cr, Xmargin, 0);
	// -- void setviewport(int left, int top, int right, int bottom, int clip);
	// setviewport function is used to restrict drawing to a particular portion on the screen. 	// For example "setviewport(100 , 100, 200, 200, 1)" will restrict our drawing activity
	// inside the rectangle(100,100, 200, 200).
	//
	// left, top, right, bottom are the coordinates of main diagonal of rectangle in which we wish to restrict our drawing. Also note that the point (left, top) becomes the new origin.
//	setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1);

	if(Game_Type==SPACE_FORTRESS)
	{			 /* set data value locations for space fortress */
		dx=MaxX/8;
		Points_X=x=2*TEXT_WIDTH;
		x=x+dx; Control_X=x;
		x=x+dx; Velocity_X=x;
		x=x+dx; Vulner_X=x;
		x=x+dx; IFF_X=x;
		x=x+dx; Interval_X=x;
		x=x+dx; Speed_X=x;
		x=x+dx; Shots_X=x;
	}
	else /* set data value locations for aiming test */
	{
		dx=MaxX/3;
		Mines_X=x=MaxX/6-2*TEXT_WIDTH;
		x=x+dx;	 Speed_X=x;
		x=x+dx-TEXT_WIDTH; Score_X=x;
	}
/* set graphics eraser is done in main */
}

void Close_Graphics(cairo_t *cr)
{
  cairo_destroy(cr);
	cairo_font_options_destroy(font_options);
  cairo_surface_destroy(surface);
}

void Close_Graphics_SF()
{
  Close_Graphics(SF_canvas);
}


float Fcos(int Headings_Degs) /* compute cos of 0 - 359 degrees */
{
		float arc;
		arc=Headings_Degs*ARC_CONV;
		return(cos(arc));
}

float Fsin(int Headings_Degs) /* compute sin of 0 - 359 degrees */
{
	float arc;
	arc=Headings_Degs*ARC_CONV; /* convert degrees to radians */
	return(sin(arc));
}

void snapCoords(cairo_t *canvas, int x, int y)
{
//		printf("Bef: %f %f\n", x, y);
//    cairo_user_to_device(canvas, &x, &y);
//		printf("After: %d %d\n", x, y);
//		&x = round(x) + 0.5;
//		&y = round(y) + 0.5;
//		cairo_device_to_user
//		printf("After [2]: %d %d\n", x, y);
}

void cairo_line(cairo_t *cr, int x_1, int y_1, int x_2, int y_2)
{
//	snapCoords(canvas, &x1, &y1 );
//
	double x1 = (double) x_1;
	double y1 = (double) y_1;
	double x2 = (double) x_2;
	double y2 = (double) y_2;

// This code generates straighter and sharper lines, but also drops parts of objects for 
// some reason //	cairo_user_to_device(cr, &x1, &y1);
//	x1 = round(x1) + 0.5;
//	y1 = round(y1) + 0.5;
//	cairo_device_to_user(cr, &x1, &y1);
// //	cairo_user_to_device(cr, &x2, &y2);
//	x2 = round(x2) + 0.5;
//	y2 = round(y2) + 0.5;
//	cairo_device_to_user(cr, &x2, &y2);
	cairo_move_to(cr, x1+0.5, y1+0.5);
	cairo_line_to(cr, x2+0.5, y2+0.5);
//	cairo_move_to(cr, x1, y1);
//	cairo_line_to(cr, x2, y2);

}



void cairo_text_at(cairo_t *cr, int x, int y, const char *string)
{
	cairo_move_to(cr, x, y);
	cairo_show_text(cr, string);
}


// Clip within the bounding box of the current_path()
void clip_path_rect(cairo_t *cr)
{
	double x1;
	double y1;
	double x2;
	double y2;
	cairo_path_extents(cr,&x1,&y1,&x2,&y2);
	cairo_path_t *ol_path = cairo_copy_path(cr);
	cairo_new_path(cr);
	// Create the bounding box
	cairo_rectangle(cr, x1-1, y1-1, (x2-x1)+1, (y2-y1)+1);
//	cairo_set_source_rgb(canvas, 1, 0, 0);
//	cairo_stroke_preserve(canvas);
	cairo_clip(cr);
	// Restore the old path

	cairo_append_path(cr,ol_path);
}

	// Clears the pixels on the previous path, i.e. sets them to black
void clear_prev_path(cairo_t *cr, cairo_path_t *prevPath)
{
	cairo_save(cr);
	cairo_set_source_rgb(cr, 0, 0, 0);
//	cairo_new_path(cr); assume the context is empty
	cairo_append_path(cr, prevPath);
	clip_path_rect(cr);
	cairo_stroke(cr);
	cairo_restore(cr);
//	cairo_reset_clip(cr);
}

void stroke_in_clip(cairo_t *cr)
{
	cairo_save(cr);
	clip_path_rect(cr);
	cairo_stroke(cr);
//	cairo_reset_clip(cr);
	cairo_restore(cr);
}


// Cleans all the previous paths from the context for the objects in need of an update
void clean(cairo_t *cr)
{
//	cairo_save(cr);
	if (Ship_Should_Clean)
	{
		clear_prev_path(cr, PrevShip);
		Ship_Should_Clean = 0;
	}
	if (Mine_Should_Clean)
	{
		clear_prev_path(cr, PrevMine);
		Mine_Should_Clean = 0;
	}
	if (Fort_Should_Clean)
	{
		clear_prev_path(cr, PrevFort);
		Fort_Should_Clean = 0;
	}
	for(int i=0;i<MAX_NO_OF_MISSILES;i++) 
	{
		if (Missile_Should_Clean[i])
		{
			clear_prev_path(cr, PrevMissile[i]);
			Missile_Should_Clean[i] = 0;
		}
	}
	if(Shell_Should_Clean)
	{
		clear_prev_path(cr, PrevShell);
		Shell_Should_Clean = 0;
	}
//	cairo_restore(cr);
	cairo_new_path(cr);

}

void update_drawing(cairo_t *cr)
{
	if (Ship_Should_Update) // Skip this (and other) ifs because they always should be visible?
	{
		Draw_Ship(cr, Ship_X_Pos,Ship_Y_Pos,Ship_Headings,SHIP_SIZE_FACTOR*MaxX);
		stroke_in_clip(cr);
		Ship_Should_Update = 0;
	}
	if (Fort_Should_Update)
	{
		Draw_Fort(cr, MaxX/2,MaxY/2,Fort_Headings,FORT_SIZE_FACTOR*MaxX);
		stroke_in_clip(cr);
		Fort_Should_Update = 0;
	}
	for(int i=0;i<MAX_NO_OF_MISSILES;i++)
	{
		if (Missile_Should_Update[i])
		{
			Draw_Missile(cr, Missile_X_Pos[i], Missile_Y_Pos[i], Missile_Headings[i], MISSILE_SIZE_FACTOR*MaxX, i);
			stroke_in_clip(cr);
			Missile_Should_Update[i] = 0;
		}
	}
	if (Mine_Should_Update)
	{
		Draw_Mine(cr, Mine_X_Pos,Mine_X_Pos,MINE_SIZE_FACTOR*MaxX);
		stroke_in_clip(cr);
		Mine_Should_Update = 0;
	}
	if(Shell_Should_Update)
	{
		Draw_Shell(cr, Shell_X_Pos,Shell_Y_Pos,Shell_Headings,SHELL_SIZE_FACTOR*MaxX);
		stroke_in_clip(cr);
		Shell_Should_Update = 0;
	}
}

void Draw_Frame(cairo_t *cr)
{
	int Height;
//	int t,t1,svcolor; // All unused

	int x,y,dx;


	// See initialize_graphics for description
//	Height=textheight("H");		/* Get basic text height */
		Height = TEXT_HEIGHT;

	// removes anything on the screen
//	cleardevice();
 	// FRAME_COLOR is the color of the green border
//	setcolor(FRAME_COLOR);
	cairo_rectangle(cr, -Xmargin, 0, WINDOW_WIDTH,WINDOW_HEIGHT); // Cheap fix
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_fill(cr);
	cairo_set_source_rgb(cr, SF_GREEN);
	/* handle panel */
	// See init graphics for description of this function
//	setviewport( Xmargin, Panel_Y_Start, Xmargin+MaxX, Panel_Y_End, 1);
	// Emulate this setviewport call with a matrix translation
	cairo_translate(cr, 0, Panel_Y_Start);

	/* data panel in screen global coordinates */

	// Bottom panel? yes bottom panel
//  void rectangle(int left, int top, int right, int bottom);
//	rectangle(0,0,MaxX,MaxY_Panel);
	cairo_rectangle(cr, 0, 0, MaxX, MaxY_Panel);
	// The line function is used to draw a line from a point(x1,y1) to point(x2,y2)
	// void line(int x1, int y1, int x2, int y2);

//	line(0,2*Height,MaxX,2*Height);
	// The line on top of the info panel
	cairo_line(cr, 0, 2*Height, MaxX, 2*Height);
	cairo_stroke(cr);

			/* write panel headers */
	if(Game_Type==SPACE_FORTRESS)
	{
			x=2;
			y=4;
			dx=MaxX/8; /* step between two headers */
			// Called somewhere else
			Data_Line=2*Height+4;
			// I guess gprintf(x_pixel, y_pixel, str);

//		gprintf ( &x, &y,"	PNTS");
//		x=x+dx; gprintf ( &x, &y," CNTRL");
//		x=x+dx; gprintf ( &x, &y," VLCTY");
//		x=x+dx; gprintf ( &x, &y," VLNER");
//		x=x+dx; gprintf ( &x, &y,"	IFF ");
//		x=x+dx; gprintf ( &x, &y,"INTRVL");
//		x=x+dx; gprintf ( &x, &y," SPEED");
//		x=x+dx; gprintf ( &x, &y," SHOTS");

			cairo_text_at(cr, x, y, "	PNTS");
			x=x+dx; cairo_text_at(cr, x, y, " CNTRL");
			x=x+dx; cairo_text_at(cr, x, y, " VLCTY");
			x=x+dx; cairo_text_at(cr, x, y, " VLNER");
			x=x+dx; cairo_text_at(cr, x, y, "	IFF ");
			x=x+dx; cairo_text_at(cr, x, y, "INTRVL");
			x=x+dx; cairo_text_at(cr, x, y, " SPEED");
			x=x+dx; cairo_text_at(cr, x, y, " SHOTS");

			/* draw vertical lines between columns */

	//		line(x,0,x,MaxY_Panel);
	//		x=x-dx; line(x,0,x,MaxY_Panel);
	//		x=x-dx; line(x,0,x,MaxY_Panel);
	//		x=x-dx; line(x,0,x,MaxY_Panel);
	//		x=x-dx; line(x,0,x,MaxY_Panel);
	//		x=x-dx; line(x,0,x,MaxY_Panel);
	//		x=x-dx; line(x,0,x,MaxY_Panel);
			cairo_line(cr,x,0,x,MaxY_Panel);
			x=x-dx; cairo_line(cr,x,0,x,MaxY_Panel);
			x=x-dx; cairo_line(cr,x,0,x,MaxY_Panel);
			x=x-dx; cairo_line(cr,x,0,x,MaxY_Panel);
			x=x-dx; cairo_line(cr,x,0,x,MaxY_Panel);
			x=x-dx; cairo_line(cr,x,0,x,MaxY_Panel);
			x=x-dx; cairo_line(cr,x,0,x,MaxY_Panel);
			cairo_stroke(cr);
	 }
	 else /* frame for aiming test */
	 {
		x=MaxX/6-32;
		y=4;
		dx=MaxX/3;		 /* step between two headers */
		Data_Line=2*Height+4;
	//		gprintf ( &x, &y,"	MINES");
	//	 x=x+dx; gprintf ( &x, &y," SPEED");
	//	 x=x+dx; gprintf ( &x, &y," SCORE");

			/* draw vertical lines between columns */
	//	 x=dx;	 line(x,0,x,MaxY_Panel);
	//	 x=x+dx; line(x,0,x,MaxY_Panel);

		cairo_text_at(cr, x, y, "	MINES");
		x=x+dx; cairo_text_at(cr, x, y, " SPEED");
		x=x+dx; cairo_text_at(cr, x, y, " SCORE");

			/* draw vertical lines between columns */
		x=dx; cairo_line(cr,x,0,x,MaxY_Panel);
		x=x+dx; cairo_line(cr,x,0,x,MaxY_Panel);
		cairo_stroke(cr);

	 }

	// setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1); /* in screen global coordinates */
	// translate back from the previous viewport call
	cairo_translate(cr, 0, -Panel_Y_Start);
	// full panel?
//	rectangle(0,0,MaxX,MaxY); 		/* main frame of the viewport */
	cairo_rectangle(cr,0,0,MaxX,MaxY); // Maybe drop this or something
 	cairo_stroke(cr);


	/* set graphics eraser is done in main */
	// Not needed basically
//	setcolor(svcolor); /* restore previous color */
}

// Draws the hexagon around the fortress, maybe drop this?
void Draw_Hexagone(cairo_t *cr,int X_Center,int Y_Center,int Hex_Outer_Radius)
{
	int Abs_Y;
//	int svcolor;

//	svcolor=getcolor(); /* save present color */
//	setcolor(HEX_COLOR);
	cairo_set_source_rgb(cr, SF_GREEN);

	Abs_Y=Hex_Outer_Radius*0.866;	/* sin(60)=0.866 */
	cairo_move_to(cr, X_Center+Hex_Outer_Radius,Y_Center); /* right-hand tip */
	cairo_line_to(cr, X_Center+Hex_Outer_Radius/2,Y_Center-Abs_Y);
	cairo_line_to(cr, X_Center-Hex_Outer_Radius/2,Y_Center-Abs_Y);
	cairo_line_to(cr, X_Center-Hex_Outer_Radius,Y_Center);
	cairo_line_to(cr, X_Center-Hex_Outer_Radius/2,Y_Center+Abs_Y);
	cairo_line_to(cr, X_Center+Hex_Outer_Radius/2,Y_Center+Abs_Y);
	cairo_line_to(cr, X_Center+Hex_Outer_Radius,Y_Center);

//	cairo_line(cr, X_Center+Hex_Outer_Radius,Y_Center, X_Center+Hex_Outer_Radius/2,Y_Center-Abs_Y);
//	cairo_line(cr, X_Center-Hex_Outer_Radius/2,Y_Center-Abs_Y, X_Center-Hex_Outer_Radius,Y_Center);
//	cairo_line(cr, X_Center-Hex_Outer_Radius/2,Y_Center+Abs_Y, X_Center+Hex_Outer_Radius/2,Y_Center+Abs_Y);
//	cairo_line_to(cr, X_Center+Hex_Outer_Radius,Y_Center);

//	setcolor(svcolor); /* restore previous color */
//	return(0);
}

void Draw_Ship(cairo_t *cr, int x, int y, int Headings, int size)
{
	/* size - is the entire length of the ship */
	int x1,y1;	/* ship's aft location */
	int x2,y2;	/* ship's nose location */
	int xl,yl;	 /* ship's left wing tip location */
	int xr,yr;	 /* ship's right wing tip location */
	int xc,yc;	/* fuselage and wings connecting point */
	int Right_Wing_Headings;
	int Left_Wing_Headings;
//	int svcolor;
//	float tmp;  // Unused

//	svcolor=getcolor(); /* save present color */
//	setcolor(SHIP_COLOR); // yellow
	cairo_set_source_rgb(cr, SF_YELLOW);

	xc=x;
	yc=y;
	x1=xc-0.5*size*Fsin(Headings);
	y1=yc+0.5*size*Fcos(Headings);
	x2=xc+0.5*size*Fsin(Headings);
	y2=yc-0.5*size*Fcos(Headings);
	cairo_line(cr,x1,y1,x2,y2);
	Right_Wing_Headings=Headings+135;
	if(Right_Wing_Headings>359) Right_Wing_Headings=Right_Wing_Headings-360;
	Left_Wing_Headings=Headings+225;
	if(Left_Wing_Headings>359) Left_Wing_Headings=Left_Wing_Headings-360;
	xr=xc+0.707*size*Fsin(Right_Wing_Headings);
	yr=yc-0.707*size*Fcos(Right_Wing_Headings);
	cairo_line(cr,xc,yc,xr,yr);
	xl=xc+0.707*size*Fsin(Left_Wing_Headings);
	yl=yc-0.707*size*Fcos(Left_Wing_Headings);
	cairo_line(cr,xc,yc,xl,yl);
	PrevShip = cairo_copy_path(cr);
//	setcolor(svcolor); /* restore previous color */
//	return(0);
}

void Draw_Fort(cairo_t *cr, int x, int y, int Headings, int size )
{
	int x1,y1;	 /* fort's aft location */
	int x2,y2;	 /* fort's nose location */
	int xl,yl;	 /* ship's left wing tip location */
	int xr,yr;	 /* ship's right wing tip location */
	int xc,yc;	 /* fuselage and wings connecting point */
	int xrt,yrt;	 /* tip of right wing */
	int xlt,ylt;	 /* tip of left wing */
	int Right_Wing_Headings;
	int Left_Wing_Headings;
	int Right_Wing_Tip_Headings;
	int Left_Wing_Tip_Headings;
//	int svcolor;

//	svcolor=getcolor(); /* save present color */
//	setcolor(FORT_COLOR); // blueee
	cairo_set_source_rgb(cr, SF_BLUE);
	x1=x;
	y1=y;
	x2=x1+size*Fsin(Headings);
	y2=y1-size*Fcos(Headings);
	cairo_line(cr,x1,y1,x2,y2);
	xc=x1+(x2-x1)*0.5;
	yc=y1+(y2-y1)*0.5;
	Right_Wing_Headings=Headings+90;
	if(Right_Wing_Headings>359) Right_Wing_Headings=Right_Wing_Headings-360;
	Left_Wing_Headings=Headings+270;
	if(Left_Wing_Headings>359) Left_Wing_Headings=Left_Wing_Headings-360;
	xr=xc+0.4*size*Fsin(Right_Wing_Headings)+0.5;
	yr=yc-0.4*size*Fcos(Right_Wing_Headings)+0.5;
	cairo_line(cr,xc,yc,xr,yr);
	xl=xc+0.4*size*Fsin(Left_Wing_Headings)+0.5;
	yl=yc-0.4*size*Fcos(Left_Wing_Headings)+0.5;
	cairo_line(cr,xc,yc,xl,yl);
	Right_Wing_Tip_Headings=Right_Wing_Headings+90;
	if(Right_Wing_Tip_Headings>359) Right_Wing_Tip_Headings=
						 Right_Wing_Tip_Headings-360;
	xrt=xr+0.5*size*Fsin(Right_Wing_Tip_Headings)+0.5;
	yrt=yr-0.5*size*Fcos(Right_Wing_Tip_Headings)+0.5;
	cairo_line(cr,xr,yr,xrt,yrt);
	Left_Wing_Tip_Headings=Right_Wing_Tip_Headings;
	xlt=xl+0.5*size*Fsin(Left_Wing_Tip_Headings)+0.5;
	ylt=yl-0.5*size*Fcos(Left_Wing_Tip_Headings)+0.5;
	cairo_line(cr,xl,yl,xlt,ylt);
	PrevFort = cairo_copy_path(cr);
//	setcolor(svcolor); /* restore previous color */
}

void Draw_Mine (cairo_t *cr, int x, int y, int size)	/* x,y is on screen center location
					size is half of horizontal diagonal */
{
//	int svcolor;

//	svcolor=getcolor(); /* save present color */
//	setcolor(MINE_COLOR); // maybe different than blue for easier recogniztion?
	cairo_set_source_rgb(cr, SF_BLUE);

	cairo_move_to(cr,x-size,y);
	cairo_line_to(cr,x,y-1.18*size);	 /* 1.3/1.1=1.18 */
	cairo_line_to(cr,x+size,y);
	cairo_line_to(cr,x,y+1.18*size);
	cairo_line_to(cr,x-size,y);
//	cairo_line(cr,x-size,y,x,y-1.18*size);
//	cairo_line(cr,x,y-1.18*size,x+size,y);
//	cairo_line(cr,x+size,y,x,y+1.18*size);
//	cairo_line(cr,x,y+1.18*size,x-size,y);
	PrevMine = cairo_copy_path(cr);
//	setcolor(svcolor); /* restore previous color */
}

void Draw_Missile (cairo_t *cr, int x, int y, int Headings, int size, int missile_idx)
{
	int x1,y1;	/* ship's aft location */
	int x2,y2;	/* ship's nose location */
	int xl,yl;	 /* ship's left wing tip location */
	int xr,yr;	 /* ship's right wing tip location */
	int xc,yc;	/* fuselage and wings connecting point */
	int Right_Wing_Headings;
	int Left_Wing_Headings;
//	int svcolor;

//	svcolor=getcolor(); /* save present color */
//	setcolor(MISSILE_COLOR);
	cairo_set_source_rgb(cr, 1, 0, 0);
	x1=x;
	y1=y;
	x2=x1+size*Fsin(Headings);
	y2=y1-size*Fcos(Headings);
	cairo_line(cr, x1,y1,x2,y2);
	xc=x2;
	yc=y2;
	Right_Wing_Headings=Headings+135;
	if(Right_Wing_Headings>359) Right_Wing_Headings=Right_Wing_Headings-360;
	Left_Wing_Headings=Headings+225;
	if(Left_Wing_Headings>359) Left_Wing_Headings=Left_Wing_Headings-360;
	xr=xc+0.25*size*Fsin(Right_Wing_Headings);
	yr=yc-0.25*size*Fcos(Right_Wing_Headings);
	cairo_line(cr,xc,yc,xr,yr);
	xl=xc+0.25*size*Fsin(Left_Wing_Headings);
	yl=yc-0.25*size*Fcos(Left_Wing_Headings);
	cairo_line(cr,xc,yc,xl,yl);
	PrevMissile[missile_idx] = cairo_copy_path(cr);
//	setcolor(svcolor); /* restore previous color */
}

void Draw_Shell(cairo_t *cr, int x, int y, int Headings, int size)
{
	int x1,y1;	/* shell's aft location */
	int x2,y2;	/* shell's nose location */
	int xl,yl;	 /* shell's left tip location */
	int xr,yr;	 /* shell's right tip location */
	int Right_Tip_Headings;
	int Left_Tip_Headings;
//	int svcolor;

//	svcolor=getcolor(); /* save present color */
//	setcolor(SHELL_COLOR);
	cairo_set_source_rgb(cr, 1, 0, 0);
	x1=x;
	y1=y;
	x2=x1+size*Fsin(Headings);
	y2=y1-size*Fcos(Headings);
	Right_Tip_Headings=Headings+30;
	if(Right_Tip_Headings>359) Right_Tip_Headings=Right_Tip_Headings-360;
	Left_Tip_Headings=Headings+330;
	if(Left_Tip_Headings>359) Left_Tip_Headings=Left_Tip_Headings-360;
	xr=x1+0.4*size*Fsin(Right_Tip_Headings);
	yr=y1-0.4*size*Fcos(Right_Tip_Headings);
	xl=x1+0.4*size*Fsin(Left_Tip_Headings);
	yl=y1-0.4*size*Fcos(Left_Tip_Headings);
	cairo_move_to(cr,x1,y1);
	cairo_line_to(cr,xl,yl);
	cairo_line_to(cr,x2,y2);
	cairo_line_to(cr,xr,yr);
	cairo_line_to(cr,x1,y1);
	PrevShell = cairo_copy_path(cr);
//	setcolor(svcolor); /* restore previous color */
}

int Find_Headings(int x1,int y1,int x2,int y2)
{
//	int quadrant;	// Unused
	double arcsinalfa;
	double b;
	double a;
	arcsinalfa=abs(x1-x2);
	a=pow(x1-x2,2)+pow(y1-y2,2);
	b=sqrt(a);
	arcsinalfa=asin(arcsinalfa/b);
	if (x1<x2)
		if (y1>y2) /* quadrant=1 */ return(arcsinalfa*57.3+0.5);
	else
		/* quadrant=2 */ return(180-arcsinalfa*57.3+0.5);
	 else
	 if (y1>y2) /* quadrant=4 */ return(360-arcsinalfa*57.3+0.5);
	else
		/* quadrant=3 */ return(180+arcsinalfa*57.3+0.5);
}

//void set_initial_vals(cairo_t *cr)
//{
//	memset(Missile_Should_Update, 0, MAX_NO_OF_MISSILES);
//	memset(Missile_Should_Clean, 0, MAX_NO_OF_MISSILES);
//}

void set_initial_vals(cairo_t *cr)
{
	memset(Missile_Should_Update, 0, MAX_NO_OF_MISSILES);
	memset(Missile_Should_Clean, 0, MAX_NO_OF_MISSILES);
	Reset_Screen(cr);
}

void Show_Score(cairo_t *cr, int val, int x, int y) /* anywhere within data panel */
{
//    int svcolor;
//    svcolor=getcolor();
		char val_str[15];


    cairo_set_source_rgb(cr, SF_YELLOW);
    cairo_translate(cr, 0, Panel_Y_Start);
    /* data panel in screen global coordinates */

//    putimage(x,y,buffer1,COPY_PUT); /* erase garbage */
		// MaxX/8 is equal to 'xdif', the width of each score rectangle
		cairo_rectangle(cr,0,x,Panel_Y_Start,x+MaxX/8); // Not sure if this call is okay (Y_Panel?)
		clip_path_rect(cr);
		cairo_fill(cr);

		sprintf(val_str, "%d", val);
    cairo_text_at(cr, x,y,val_str);
		cairo_reset_clip(cr);
//    setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1);   /* restore gaming area */
		cairo_translate(cr, 0 , -Panel_Y_Start);

//    setcolor(svcolor); /* restore previous color */
}

/* Every update_X function here had a "return(0)" zero statement on it's last line, without  
specifying a return type. I removed all of these return statements and modified the function 
return type to void to surpress warnings. */ 
// Magical 8's?
void Update_Points(cairo_t *cr)
{
    Show_Score(cr, Points,Points_X-8,Data_Line);
}

void Update_Control(cairo_t *cr)
{
    Show_Score(cr, Control,Control_X-8,Data_Line);
}

void Update_Velocity(cairo_t *cr)
{
    Show_Score(cr, Velocity,Velocity_X,Data_Line);
}

void Update_Vulner(cairo_t *cr)  /* for vulner only */
{
    Show_Score(cr, Vulner_Counter,Vulner_X,Data_Line);
}

/* IFF is missing here */

void Update_Interval(cairo_t *cr)
{
    Show_Score(cr, Double_Press_Interval,Interval_X,Data_Line);
}

void Update_Speed(cairo_t *cr)
{
    Show_Score(cr, Speed,Speed_X-8,Data_Line);
}

void Update_Shots(cairo_t *cr)
{
    Show_Score(cr, Missile_Stock,Shots_X,Data_Line);
}

void Reset_Screen(cairo_t *cr)
{
    int i;
        /*  reset variables */
    Ship_X_Pos=0.25*MaxX; /* on a 640 x 480 screen VGA-HI */
    Ship_Y_Pos=0.5*MaxY; /* same as above */
    Ship_X_Speed=0.0;
    Ship_Y_Speed=0.0;
    Ship_Headings=0;
    Mine_Flag=DEAD;
    for(i=0;i<MAX_NO_OF_MISSILES;i++) Missile_Flag[i]=DEAD;
    Missile_Type=VS_FRIEND;
    Missile_Vs_Mine_Only=OFF;
    Missiles_Counter=0;
    Shell_Flag=DEAD;
    Rotate_Input=0; /* joystick left/right */
    Accel_Input=0; /* joystick forward */
    End_Flag=OFF;
    Fort_Headings=270;
    Vulner_Counter=0;
    Timing_Flag=OFF; /* if screen reset between consecutive presses */
    Resource_Flag=OFF;
    Resource_Off_Counter=0;
    Bonus_Display_Flag=NOT_PRESENT;   /* in case bonus is pressed after */
    Bonus_Granted=OFF;
    Fort_Lock_Counter=0;

    /* reset screen */
//    Draw_Frame(cr);
//    if(AspectRatio==1.0)
//    {
//        Draw_Hexagone(cr, MaxX/2,MaxY/2,BIG_HEXAGONE_SIZE_FACTOR*MaxX);
//    Draw_Hexagone(cr, MaxX/2,MaxY/2,SMALL_HEXAGONE_SIZE_FACTOR*MaxX);
//    }
//    else
//    {
//        Draw_Hexagone(MaxX/2,MaxY/2,BIG_HEXAGONE_SIZE_FACTOR*MaxX/GraphSqrFact);
//        Draw_Hexagone(MaxX/2,MaxY/2,SMALL_HEXAGONE_SIZE_FACTOR*MaxX/GraphSqrFact);
//    }
//    Draw_Fort(cr, MaxX/2,MaxY/2,Fort_Headings,FORT_SIZE_FACTOR*MaxX);
		Fort_Should_Update = 1;
		Fort_Should_Clean = 1;
//    Draw_Ship(cr, Ship_X_Pos,Ship_Y_Pos,Ship_Headings,SHIP_SIZE_FACTOR*MaxX);
		Ship_Should_Update = 1;
		Ship_Should_Clean = 1;

            /* reset panel */
    Update_Points(cr);
    Update_Vulner(cr);
    Update_Interval(cr);
    Update_Shots(cr);
    Update_Control(cr);
    Update_Velocity(cr);
    Update_Speed(cr);

}  /* end reset screen */



void start_drawing()
{
	surface = cairo_image_surface_create(CAIRO_FORMAT_RGB16_565, WINDOW_WIDTH, WINDOW_HEIGHT);
//	surface = cairo_quartz_surface_create(CAIRO_FORMAT_RGB16_565, WINDOW_WIDTH, WINDOW_HEIGHT);
	SF_canvas = cairo_create(surface);
	Initialize_Graphics(SF_canvas);
	set_initial_vals(SF_canvas);
	// restore the line width
	cairo_set_line_width(SF_canvas, (224.1 * 1) / ((double) MaxY * 1));
	Draw_Frame(SF_canvas); // Draw the basis
}

void stop_drawing()
{
	// Specific SF_canvas function
	Close_Graphics(SF_canvas);
}

int move_update()
{
//	return (int) ((rand() % (3)) - 1);
	return 1;
}

// Maybe create a seperate SF version? also because returning does not make sense for GTK
//unsigned char* update_frame(cairo_t *cr)


void update_frame(cairo_t *cr)
{

//	if (rand() % 5)
//	{
//		Ship_Should_Clean = 0;	
//		Ship_Should_Update = 0;
//	}
//	else
//	{
//		Ship_X_Pos = (Ship_X_Pos + move_update()) % MaxX;
//		if (rand() % 2)
//		{
//			Ship_Y_Pos = (Ship_Y_Pos + rand() % 2) % MaxY;
//		}
//	}

//	if (rand() % 2)
//	{
//		Ship_Headings = (Ship_Headings + move_update()) % 360;
//	}
//	int m = 0;
//	if (rand() % 2)
//	{
//		
//	}
//	if (rand() % 2)
//	{
//		m = move_update();
//		Mine_X_Pos = (Mine_X_Pos + m) % MaxX;
//	}

	
//	Ship_X_Pos =  ((Ship_X_Pos + move_update()) % MaxX) + 1;
//	Ship_Y_Pos = (Ship_Y_Pos + move_update()) % MaxY;
	Fort_Headings = (Fort_Headings + 1) % 360;
	Fort_Headings = 45;	
	Mine_Y_Pos = (Mine_Y_Pos + move_update()) % MaxY;
	Mine_X_Pos = (Mine_X_Pos + move_update()) % MaxX;
	Missile_X = (Missile_X + move_update()) % MaxX;
	if (Missile_Y == 0)
	{
		Missile_Y = MaxY;
	}
	Missile_Y = (Missile_Y - 1) % MaxY;
	Missile_Y_Pos[0] = Missile_Y;
	Missile_X_Pos[0] = Missile_X;

	clean(cr);
	Draw_Hexagone(cr, MaxX/2,MaxY/2,SMALL_HEXAGONE_SIZE_FACTOR*MaxX);
	stroke_in_clip(cr);
	
//	cairo_reset_clip(cr);
	update_drawing(cr);
}

unsigned char* update_frame_SF()
{
	update_frame(SF_canvas);
	return cairo_image_surface_get_data(surface);
}

//int main(int argc, char *argv[])
//{
//	Initialized_Graphics = 0;
//
//// Basic GTK initialization
// 	GtkWidget *window;
//  GtkWidget *darea;
//  gtk_init(&argc, &argv);
//  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//  darea = gtk_drawing_area_new();
//  gtk_container_add(GTK_CONTAINER(window), darea);
//
//  g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
//  g_signal_connect(window, "destroy", G_CALLBACK(exit), NULL);
//  g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (on_key_press), NULL);
//
//  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
//  gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
//
//  gtk_window_set_title(GTK_WINDOW(window), "Space Fortress");
////	gtk_print_context_get_cairo_context();
//
//  gtk_widget_show_all(window);
//	animation_loop(darea);
//
//  gtk_main();
//
//	stop_drawing(); // GTK handles this I guess
//
//  return 0;
//}
//


//int main()
//{
//	start_drawing();
//	update_frame_SF();
//	cairo_surface_write_to_png(surface, "test.png");
//	stop_drawing();
//	
//}



#endif