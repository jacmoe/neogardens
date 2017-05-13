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


#define mapWidth 24
#define mapHeight 24

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

double posX = 22, posY = 12;  //x and y start position
double dirX = -1, dirY = 0; //initial direction vector
double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

double time = 0; //time of current frame
double oldTime = 0; //time of previous frame
double frameTime = 0;

static int init(int width, int height);
static int shutdown();
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void draw_maze(Buffer* buffer);

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
        oldTime = time;
        time = glfwGetTime();
        frameTime = time - oldTime; //frameTime is the time this frame has taken, in seconds
        //printf("%f\n", 1.0 / frameTime); //FPS counter

        // Event polling
        nasl_graphics_poll_events();

        // draw box and maze
        nasl_buffer_clear(buffer, GREY1);
    	draw_maze(buffer);
        //draw_box(buffer);

        // Render the main buffer
        nasl_graphics_render(buffer);
        // Swap buffers
        nasl_graphics_present();
    }

    // Destroy the main buffer
    nasl_buffer_destroy(buffer);

    shutdown();
}

//Fast vertical line from (x,y1) to (x,y2), with rgb color
void vert_line_buf(int x, int y1, int y2, uint32_t color, Buffer* buffer)
{
	if(y2 < y1)
	{ // swap y1 and y2
		y1 += y2;
		y2 = y1 - y2;
		y1 -= y2;
	}
	if(y2 < 0 || y1 >= buffer->height || x < 0 || x >= buffer->width)
	{ // no single point of the line is on the screen
		return;
	}
	if(y1 < 0) y1 = 0; // clip
	if(y2 >= buffer->width) y2 = buffer->height - 1; // clip
	for(int y = y1; y <= y2; y++)
	{
		nasl_buffer_set_pixel(buffer, x, y, color);
	}
}

// Draws a raycast image in the viewport of the maze represented in array
// map as seen from position xview, yview by a viewer looking at an angle
// viewer_angle, where angle 0 is due north. (Angles measured in radians)
void draw_maze(Buffer* buffer)
{
    for(int x = 0; x < buffer->width; x++)
    {
      //calculate ray position and direction
      double cameraX = 2 * x / (double)buffer->width - 1; //x-coordinate in camera space
      double rayPosX = posX;
      double rayPosY = posY;
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX;
      //which box of the map we're in
      int mapX = (int)rayPosX;
      int mapY = (int)rayPosY;

      //length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;

       //length of ray from one x or y-side to next x or y-side
      double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
      double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
      double perpWallDist;

      //what direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; //was there a wall hit?
      int side; //was a NS or a EW wall hit?
      //calculate step and initial sideDist
      if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (rayPosX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
      }
      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (rayPosY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
      }
      //perform DDA
      while (hit == 0)
      {
        //jump to next map square, OR in x-direction, OR in y-direction
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        //Check if ray has hit a wall
        if (worldMap[mapX][mapY] > 0) hit = 1;
      }
      //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
      if (side == 0) perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
      else           perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

      //Calculate height of line to draw on screen
      int lineHeight = (int)(buffer->height / perpWallDist);

      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + buffer->height / 2;
      if(drawStart < 0)drawStart = 0;
      int drawEnd = lineHeight / 2 + buffer->height / 2;
      if(drawEnd >= buffer->height)drawEnd = buffer->height - 1;

      //choose wall color
      uint32_t color;
      switch(worldMap[mapX][mapY])
      {
        case 1:  color = RED;  break; //red
        case 2:  color = GREEN;  break; //green
        case 3:  color = BLUE;   break; //blue
        case 4:  color = WHITE;  break; //white
        default: color = YELLOW; break; //yellow
      }

      //give x and y sides different brightness
      if (side == 1) {color = color / 2;}

      //draw the pixels of the stripe as a vertical line
      vert_line_buf(x, drawStart, drawEnd, color, buffer);
    }
}

void draw_box(Buffer* buffer)
{
	//left, top, right, bottom
	//nasl_draw_rect(buffer, VIEWPORT_LEFT, VIEWPORT_TOP, VIEWPORT_RIGHT, VIEWPORT_BOT, GREY3);
}


static int init(int width, int height)
{
    nasl_graphics_init(width, height, "Neogardens Wireframe Maze Demo", 0, 3);

    glfwSetKeyCallback(nasl_graphics_get_window(), key_callback);

    return 1;
}

static void handle_keypress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //speed modifiers
    double moveSpeed = frameTime * 10.0; //the constant value is in squares/second
    double rotSpeed = frameTime * 3.0; //the constant value is in radians/second
    ///*
    // Do we want to move forward?
    if(key == GLFW_KEY_W)
    {
        if(!worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)]) posX += dirX * moveSpeed;
        if(!worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)]) posY += dirY * moveSpeed;
    }
    // or do we want to go backward?
    else if(key == GLFW_KEY_S)
    {
        if(!worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)]) posX -= dirX * moveSpeed;
        if(!worldMap[(int)(posX)][(int)(posY - dirY * moveSpeed)]) posY -= dirY * moveSpeed;
    }
    // Do we want to turn left?
    if(key == GLFW_KEY_A)
    {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
    // or do we want to turn right?
    else if(key == GLFW_KEY_D)
    {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
      dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
      planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
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
