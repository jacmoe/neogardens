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

typedef struct Movement
{
    int forwards;    // 1 forwards, -1 backwards
    int turn;        // 1 clockwise, -1 anti-clockwise
    int rel_mouse_x; // > 0 clockwise, < 0 anti-clockwise
} Movement;


static int quit = 0;

// Returns a vector with the position of the mouse in buffer coordinates.
//Vector S_GetMousePos(Buffer *buf);

// Set to 1 to keep the mouse inside the window.
//void S_GrabMouse(int flag);

Movement S_GetMovement();

static void init(int width, int height);
static void shutdown();

void draw_maze(Buffer* buffer);

int main()
{
    int buffer_width = 320;
    int buffer_height = 240;

    init(buffer_width, buffer_height);
    
    //glfwSwapInterval(1);
    
    // Create main buffer
    Buffer* buffer = nasl_buffer_create(buffer_width, buffer_height);
    nasl_buffer_set_mainbuffer(buffer);

    Movement mov = { .forwards = 0, .turn = 0};

    // Main loop
    while(!quit)
    {
        mov = S_GetMovement();
        oldTime = time;
        time += .1;
        frameTime = time - oldTime; //frameTime is the time this frame has taken, in seconds

        //speed modifiers
        double moveSpeed = 0.008;//frameTime * 5.0; //the constant value is in squares/second
        double rotSpeed = 0.002;//frameTime * 2.0;   //the constant value is in radians/second

        if(mov.forwards > 0)
        {
            if (!worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)])
                posX += dirX * moveSpeed;
            if (!worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)])
                posY += dirY * moveSpeed;
        }
        if(mov.forwards < 0)
        {
            if (!worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)])
                posX -= dirX * moveSpeed;
            if (!worldMap[(int)(posX)][(int)(posY - dirY * moveSpeed)])
                posY -= dirY * moveSpeed;
        }
        if(mov.turn > 0)
        {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }
        if(mov.turn < 0)
        {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        }

        // Event polling
        //nasl_graphics_poll_events();

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

Movement S_GetMovement()
{
    static int fwd, turn;

    Movement mov = {0};
    SDL_GetRelativeMouseState(&mov.rel_mouse_x, NULL);
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type)  {
        case SDL_KEYDOWN:
            switch (ev.key.keysym.sym) {
            case SDLK_UP:
            case 'w':
                fwd = 1;
                break;

            case SDLK_DOWN:
            case 's':
                fwd = -1;
                break;

            case 'a':
                turn= 1;
                break;

            case 'd':
                turn = -1;
                break;

            case 'q':
                quit = 1;
            }
            break;

        case SDL_KEYUP:
            switch (ev.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_DOWN:
            case 'w':
            case 's':
                fwd = 0;
                break;

            case 'a':
            case 'd':
                turn = 0;
                break;
            }
            break;

        case SDL_QUIT:
            quit = 1;
            break;
        }
    }

    mov.forwards = fwd;
    mov.turn = turn;

    return mov;
}

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
        case 1:  color = RED;  break;
        case 2:  color = GREEN;  break;
        case 3:  color = BLUE;   break;
        case 4:  color = VIOLET;  break;
        default: color = YELLOW; break;
        }

        //give x and y sides different brightness
        if (side == 1) {color = color / 2;}

        //draw the pixels of the stripe as a vertical line
        nasl_draw_vertical_line(buffer, x, drawStart, drawEnd, color);
    }
}


static void init(int width, int height)
{
    nasl_graphics_init(width, height, "Neogardens Solid Color Raycast Maze Demo", 0, 3);
}

/*
static void handle_keypress(int key, int action)
{
    // Do we want to move forward?
    if (key == GLFW_KEY_W)
    {
        if(action == GLFW_PRESS)
            move_forwards = 1;
        else if(action == GLFW_RELEASE)
            move_forwards = 0;
    }
    // or do we want to go backward?
    else if (key == GLFW_KEY_S)
    {
        if(action == GLFW_PRESS)
            move_backwards = 1;
        else if(action == GLFW_RELEASE)
            move_backwards = 0;
    }
    // Do we want to turn left?
    if (key == GLFW_KEY_A)
    {
        if(action == GLFW_PRESS)
            turn_left = 1;
        else if(action == GLFW_RELEASE)
            turn_left = 0;
    }
    // or do we want to turn right?
    else if (key == GLFW_KEY_D)
    {
        if(action == GLFW_PRESS)
            turn_right = 1;
        else if(action == GLFW_RELEASE)
            turn_right = 0;
    }
}
//*/

static void shutdown()
{
    nasl_graphics_shutdown();
}
