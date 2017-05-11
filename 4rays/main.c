/*
*   This file is part of the
* ███╗   ██╗███████╗ ██████╗  ██████╗  █████╗ ██████╗ ██████╗ ███████╗███╗   ██╗███████╗
* ████╗  ██║██╔════╝██╔═══██╗██╔════╝ ██╔══██╗██╔══██╗██╔══██╗██╔════╝████╗  ██║██╔════╝
* ██╔██╗ ██║█████╗  ██║   ██║██║  ███╗███████║██████╔╝██║  ██║█████╗  ██╔██╗ ██║███████╗
* ██║╚██╗██║██╔══╝  ██║   ██║██║   ██║██╔══██║██╔══██╗██║  ██║██╔══╝  ██║╚██╗██║╚════██║
* ██║ ╚████║███████╗╚██████╔╝╚██████╔╝██║  ██║██║  ██║██████╔╝███████╗██║ ╚████║███████║
* ╚═╝  ╚═══╝╚══════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═══╝╚══════╝
*   project : https://github.com/jacmoe/neogardens
*
*   Copyright 2017 Jacob Moen
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "nasl_graphics.h"
#include "nasl_buffer.h"
#include "nasl_image.h"
#include "nasl_draw.h"


const int  WALL_HEIGHT = 64;
const int VIEWER_HEIGHT = 32;
const int VIEWER_DISTANCE = 128;
const int VIEWPORT_LEFT = 40;
const int VIEWPORT_RIGHT = 280;
const int VIEWPORT_TOP = 50;
const int VIEWPORT_BOT = 150;
const int VIEWPORT_HEIGHT = 100;
const int VIEWPORT_CENTER = 100;


typedef int map_type[16][16];

map_type map =
{
  { 5, 5, 5, 5,   5, 5, 5, 5,   5, 5, 5, 5,   5, 5, 5, 5},
  { 6, 5, 0, 5,   0, 0, 0, 2,   0, 0, 0, 0,   0, 0, 0, 5},
  { 6, 0, 0, 0,   0, 0, 0, 0,   3, 0, 0, 0,   0, 0, 0, 5},
  { 6, 0, 0, 0,   0, 0, 0, 0,   0, 6, 0, 0,   0, 0, 0, 5},

  { 7, 0, 0, 0,   0, 0, 0, 0,   0, 0, 5, 4,   0, 0, 0, 5},
  { 4, 0, 0, 0,   0, 0, 0, 0,   0, 0,10,10,   0, 0, 0, 5},
  {10, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0,10,  10, 0, 0, 5},
  {10, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,  10, 0, 0, 5},

  {10, 6, 6, 0,   0, 0, 0, 0,   0, 0, 0,10,  10, 0, 0, 5},
  { 4, 0, 0, 0,   0, 0, 0, 0,   0, 0,10,10,   0, 0, 0, 5},
  { 4, 6, 6, 0,   0, 0, 5, 4,   0, 3, 0, 0,   0, 0, 0, 5},
  { 6, 0, 0, 0,   0, 0, 0, 4,   0, 3, 0, 0,   0, 0, 0, 5},

  { 6, 0, 0, 0,   0, 0, 0, 3,   0, 0, 0, 0,   0, 0, 0, 2},
  { 6, 0, 0, 0,   0, 0, 0, 3,   0, 3, 0, 0,   0, 0, 0,10},
  { 6, 6, 6, 6,   6, 0, 0, 3,   0, 3, 0, 0,   0, 0, 0,10},
  { 6, 5, 5, 5,   5, 5, 5, 3,   4, 4, 5, 5,   5, 5, 5, 5}
};

float viewing_angle = 180;
int viewer_height = 32;
int xview = 6 * 64;
int yview = 10 * 64;

static int init(int width, int height);
static int shutdown();
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void draw_maze(map_type map, Buffer* buffer, int xview, int yview, float viewing_angle);
void draw_box(Buffer* buffer);

int main()
{
    int buffer_width = 320;
    int buffer_height = 240;

    init(buffer_width, buffer_height);

    // Create main buffer
    Buffer* buffer = nasl_buffer_create(buffer_width, buffer_height);
    nasl_buffer_set_mainbuffer(buffer);

    // Main loop
    while(nasl_graphics_running())
    {
        // Event polling
        nasl_graphics_poll_events();

        // draw box and maze
        nasl_buffer_clear(buffer, GREY1);
    	draw_maze(map, buffer, xview, yview, viewing_angle);
        draw_box(buffer);

        // Render the main buffer
        nasl_graphics_render(buffer);
        // Swap buffers
        nasl_graphics_present();
    }

    // Destroy the main buffer
    nasl_buffer_destroy(buffer);

    shutdown();
}

// Draws a raycast image in the viewport of the maze represented in array
// map as seen from position xview, yview by a viewer looking at an angle
// viewer_angle, where angle 0 is due north. (Angles measured in radians)
void draw_maze(map_type map, Buffer* buffer, int xview, int yview, float viewing_angle)
{

	int offset; 				// pixel y position and offset
	float xd, yd;				// distance to next wall in x and y
	int grid_x, grid_y;			// coordinates of x and y grid lines
	float xcross_x, xcross_y;	// ray intersection coordinates
	float ycross_x, ycross_y;

	int xdist, ydist;
	int xmaze, ymaze;
	int distance;

	// *** The raycasting begins:

	// loop through all columns of pixels in viewport:
	for(int column = VIEWPORT_LEFT; column < VIEWPORT_RIGHT; column++)
	{
		// calculate horizontal angle of ray relative to center ray:
		float column_angle = atan((float)(column - 160) / VIEWER_DISTANCE);

		// calculate angle of ray relative to maze coordinates:
		float radians = viewing_angle + column_angle;

		// rotate endpoint of ray to viewing angle:
		int x2 = 1024 * (cos(radians));
		int y2 = 1024 * (sin(radians));

		// translate relative to viewer's position:
		x2 += xview;
		y2 += yview;

		// initialize ray at viewer's position:
		float x = xview;
		float y = yview;

		// find difference in x,y coordates along the ray:
		int xdiff = x2 - xview;
		int ydiff = y2 - yview;

		// cheat to avoid divide-by-zero error:
		if(xdiff == 0) xdiff = 1;

		// get slope of ray:
		float slope = (float)ydiff / xdiff;

		// cheat (again) to avoid divide-by-zero error:
		if(slope == 0.0) slope = 0.0001;

		uint32_t color = RED;

		// cast ray from grid line to grid line:
		for(;;)
		{
			// if ray direction positive in x, get next x grid line:
			if(xdiff > 0) grid_x = ((int)x & 0xffc0) + 64;

			// if ray direction negative in x, get last x grid line:
			else grid_x = ((int)x & 0xffc0) - 1;

			// if ray direction positive in y, get next y grid line:
			if(ydiff > 0) grid_y = ((int)y & 0xffc0) + 64;

			// if ray direction negative in y, get last y grid line:
			else grid_y = ((int)y & 0xffc0) - 1;

			// get x,y coordinates where ray crosses x grid line:
			xcross_x = grid_x;
			xcross_y = y + slope * (grid_x - x);

			// get x,y coordinates where ray crosses y grid line:
			ycross_x = x + (grid_y - y) / slope;
			ycross_y = grid_y;

			// get distance to x grid line:
			xd = xcross_x - x;
			yd = xcross_y - y;
			xdist = sqrt(xd * xd + yd * yd);

			// get distance to y grid line:
			xd = ycross_x - x;
			yd = ycross_y - y;
			ydist = sqrt(xd * xd + yd * yd);

			// if x grid line is closer ...
			if(xdist < ydist)
			{
				// calculate maze grid coordinates of square:
				xmaze = xcross_x / 64;
				ymaze = xcross_y / 64;

				// set x and y to point of ray intersection:
				x = xcross_x;
				y = xcross_y;

				// is there a maze cube here? If so, stop looping:
				if(map[xmaze][ymaze])
				{
					color = c64_palette[map[xmaze][ymaze]];
					break;
				}
			}
			else	// if y grid line is closer:
			{
				// calculate maze grid coordinates of square:
				xmaze = ycross_x / 64;
				ymaze = ycross_y / 64;

				// set x and y to piont of ray intersection:
				x = ycross_x;
				y = ycross_y;

				// is there a maze cube here? If so, stop looping:
				if(map[xmaze][ymaze])
				{
					color = c64_palette[map[xmaze][ymaze]];
					break;
				}
			}

		} // raycast loop

		// *** Prepare to draw wall column:

		// get distance from viewer to intersection point:
		xd = x - xview;
		yd = y - yview;
		distance = (long)sqrt(xd * xd + yd * yd) * cos(column_angle);
		if(distance == 0) distance = 1;

		// calculate visible height of walls:
		int height = VIEWER_DISTANCE * WALL_HEIGHT / distance;

		// calculate bottom of wall on screen:
		int bot = VIEWER_DISTANCE * VIEWER_HEIGHT / distance + VIEWPORT_CENTER;

		// calculate top of wall on screen:
		int top = bot - height + 1;

		// clip wall to viewport
		if(top < VIEWPORT_TOP)
		{
			height -= (VIEWPORT_TOP - top);
			top = VIEWPORT_TOP;
		}
		if((top + height) > VIEWPORT_BOT)
		{
			height -= (bot - VIEWPORT_BOT);
		}

		// *** Draw the wall column:

		// find video offset of top pixel in wall column:
		offset = top * 320 + column;

		// loop through all pixels in wall column:
		for(int i = 0; i < height; i++)
		{
			// set wall pixels to white:
            buffer->pixels[offset] = color;

			// advance to next vertical pixel:
			offset += 320;
		}

	} // for each column in viewport

}

void draw_box(Buffer* buffer)
{
	//left, top, right, bottom
	nasl_draw_rect(buffer, VIEWPORT_LEFT, VIEWPORT_TOP, VIEWPORT_RIGHT, VIEWPORT_BOT, GREY3);
}


static int init(int width, int height)
{
    nasl_graphics_init(width, height, "Neogardens Wireframe Maze Demo", 0, 3);

    glfwSetKeyCallback(nasl_graphics_get_window(), key_callback);

    return 1;
}

static void handle_keypress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    int newx, newy = 0;
    ///*
    // Do we want to move forward?
    if(key == GLFW_KEY_W)
    {
        newx = xview;
        newy = yview;
        if(!map[newx][newy])
        {
            xview = newx;
            yview = newy;
        }
    }
    // or do we want to go backward?
    else if(key == GLFW_KEY_S)
    {
        newx = xview;
        newy = yview;
        if(!map[newx][newy])
        {
            xview = newx;
            yview = newy;
        }
    }
    // Do we want to turn left?
    if(key == GLFW_KEY_A)
    {
        viewing_angle -= 0.1;
    }
    // or do we want to turn right?
    else if(key == GLFW_KEY_D)
    {
        viewing_angle += 0.1;
    }
    //*/
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else {
        handle_keypress(window, key, scancode, action, mods);
    }
}

static int shutdown()
{
    nasl_graphics_shutdown();

    return 1;
}
