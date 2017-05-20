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

#include "nasl_graphics.h"
#include "nasl_buffer.h"
#include "nasl_image.h"
#include "nasl_draw.h"


static void init(int width, int height);
static void shutdown();

char maze[16][16] = {
	{1, 1, 1, 1,    1, 1, 1, 1,    1, 1, 1, 1,    1, 1, 1, 1},
	{1, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 1},
	{1, 0, 1, 0,    1, 1, 1, 0,    0, 0, 0, 0,    1, 1, 0, 1},
	{1, 0, 1, 0,    1, 0, 1, 0,    1, 1, 1, 0,    1, 1, 0, 1},

	{1, 0, 1, 0,    1, 0, 1, 0,    0, 0, 1, 0,    0, 1, 0, 1},
	{1, 0, 1, 1,    1, 0, 1, 0,    1, 0, 1, 1,    1, 1, 0, 1},
	{1, 0, 0, 0,    0, 0, 0, 0,    1, 0, 0, 0,    0, 0, 0, 1},
	{1, 0, 1, 1,    1, 0, 1, 0,    1, 1, 1, 0,    1, 1, 0, 1},

	{1, 0, 1, 0,    0, 0, 1, 0,    0, 0, 1, 0,    1, 1, 0, 1},
	{1, 0, 1, 0,    1, 0, 1, 1,    1, 1, 1, 0,    1, 1, 0, 1},
	{1, 0, 1, 0,    1, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 1},
	{1, 0, 1, 1,    1, 0, 1, 0,    1, 0, 1, 1,    1, 1, 0, 1},

	{1, 0, 0, 0,    1, 0, 1, 0,    1, 0, 0, 0,    0, 1, 0, 1},
	{1, 0, 1, 1,    1, 0, 1, 1,    1, 1, 1, 1,    1, 1, 0, 1},
	{1, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 1},
	{1, 1, 1, 1,    1, 1, 1, 1,    1, 1, 1, 1,    1, 1, 1, 1}
};

typedef struct xy {
	int x, y;
};

struct xy increment[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
struct xy left[4] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
struct xy right[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
struct xy pos = {1,3};

int direction = 3;
int visibility = 4;

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
    // Clear main buffer to a dark grey color
    //nasl_buffer_clear(buffer, GREY1);

    Buffer* background = nasl_image_load("assets/textures/background.png");
    nasl_buffer_blit(buffer, background, 0, 0);

    draw_box(buffer);
    draw_maze(buffer);

    SDL_Event event;
    int quit = 0;

    // Main loop
    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {

            switch(event.type)
            {
                /* SDL_QUIT event (window close) */
                case SDL_QUIT:
                    quit = 1;
                    break;

                default:
                    break;
            }

            // Render the main buffer
            nasl_graphics_render(buffer);
            // Swap buffers
            nasl_graphics_present();
        }
    }

    // Destroy the main buffer
    nasl_buffer_destroy(buffer);

    shutdown();
}

// Draw the maze stored in array maze[] into video
// memory at address screen.
void draw_maze(Buffer* buffer)
{
	struct xy block, lblock, rblock;
	int oldleft, oldright;

	// Draw the maze at each distance allowed by visibility
	for(int dist = 0; dist < visibility; dist++)
	{
		// Find current square of maze
		block.x = pos.x + dist * increment[direction].x;
		block.y = pos.y + dist * increment[direction].y;

		// Find square to the left of current square
		lblock.x = block.x + left[direction].x;
		lblock.y = block.y + left[direction].y;

		// Find square to the right of current square
		rblock.x = block.x + right[direction].x;
		rblock.y = block.y + right[direction].y;

		// Draw image of squares according to distance
		switch(dist)
		{
			case 0:		// Draw current square

				// Is wall open to the left?
				// If not, draw wall
				if (maze[block.x][block.y])
				{
					nasl_draw_line(buffer, 82, 19, 135, 44, GREY3);
					nasl_draw_line(buffer, 135, 44, 135, 93, GREY3);
					nasl_draw_line(buffer, 135, 93, 82, 118, GREY3);
				}
				else // Else draw opening
				{
					nasl_draw_line(buffer, 82, 44, 135, 44, GREY3);
					nasl_draw_line(buffer, 135, 44, 135, 93, GREY3);
					nasl_draw_line(buffer, 135, 93, 82, 93, GREY3);
				}

				// Is wall open to the right?
				// If not, draw wall
				if (maze[rblock.x][rblock.y])
				{
					nasl_draw_line(buffer, 294, 19, 242, 44, GREY3);
					nasl_draw_line(buffer, 242, 44, 242, 93, GREY3);
					nasl_draw_line(buffer, 294, 118, 242, 93, GREY3);
				}
				else	// Else draw opening
				{
					nasl_draw_line(buffer, 294, 44, 242, 44, GREY3);
					nasl_draw_line(buffer, 242, 44, 242, 93, GREY3);
					nasl_draw_line(buffer, 242, 93, 294, 93, GREY3);
				}
				break;

			case 1:		// Repeat for next square

				// Can we see the next square?
				// If not, draw wall
				if(maze[block.x][block.y])
				{
					nasl_draw_line(buffer, 135, 44, 135, 93, GREY3);
					nasl_draw_line(buffer, 242, 44, 242, 93, GREY3);
					nasl_draw_line(buffer, 135, 44, 242, 44, GREY3);
					nasl_draw_line(buffer, 135, 93, 242, 93, GREY3);
				}
				else	// Else draw sides of the next square
				{
					if (maze[lblock.x][lblock.y])
					{
						nasl_draw_line(buffer, 135, 44, 162, 57, GREY3);
						nasl_draw_line(buffer, 162, 57, 162, 80, GREY3);
						nasl_draw_line(buffer, 162, 80, 135, 93, GREY3);
					}
					else
					{
						nasl_draw_line(buffer, 135, 57, 162, 57, GREY3);
						nasl_draw_line(buffer, 162, 57, 162, 80, GREY3);
						nasl_draw_line(buffer, 162, 80, 135, 80, GREY3);
					}
					if (maze[rblock.x][rblock.y])
					{
						nasl_draw_line(buffer, 242, 44, 215, 57, GREY3);
						nasl_draw_line(buffer, 215, 57, 215, 80, GREY3);
						nasl_draw_line(buffer, 215, 80, 242, 93, GREY3);
					}
					else
					{
						nasl_draw_line(buffer, 242, 57, 215, 57, GREY3);
						nasl_draw_line(buffer, 215, 57, 215, 80, GREY3);
						nasl_draw_line(buffer, 215, 80, 242, 80, GREY3);
					}
				}
				break;

			case 2:		// Do it again
				if (maze[block.x][block.y])
				{
					nasl_draw_line(buffer, 162, 57, 162, 80, GREY3);
					nasl_draw_line(buffer, 215, 57, 215, 80, GREY3);
					nasl_draw_line(buffer, 162, 57, 215, 57, GREY3);
					nasl_draw_line(buffer, 162, 80, 215, 80, GREY3);
				}
				else
				{
					if (maze[lblock.x][lblock.y])
					{
						nasl_draw_line(buffer, 162, 57, 175, 63, GREY3);
						nasl_draw_line(buffer, 175, 63, 175, 74, GREY3);
						nasl_draw_line(buffer, 175, 74, 162, 80, GREY3);
					}
					else
					{
						nasl_draw_line(buffer, 162, 63, 175, 63, GREY3);
						nasl_draw_line(buffer, 175, 63, 175, 74, GREY3);
						nasl_draw_line(buffer, 175, 74, 162, 74, GREY3);
					}
					if (maze[rblock.x][rblock.y])
					{
						nasl_draw_line(buffer, 215, 57, 202, 63, GREY3);
						nasl_draw_line(buffer, 202, 63, 202, 74, GREY3);
						nasl_draw_line(buffer, 202, 74, 215, 80, GREY3);
					}
					else
					{
						nasl_draw_line(buffer, 215, 63, 202, 63, GREY3);
						nasl_draw_line(buffer, 202, 63, 202, 74, GREY3);
						nasl_draw_line(buffer, 202, 74, 215, 74, GREY3);
					}
				}
				break;

			case 3:		// And again
				if (maze[block.x][block.y])
				{
					nasl_draw_line(buffer, 175, 63, 175, 74, GREY3);
					nasl_draw_line(buffer, 202, 63, 202, 74, GREY3);
					nasl_draw_line(buffer, 175, 63, 202, 63, GREY3);
					nasl_draw_line(buffer, 175, 74, 202, 74, GREY3);
				}
				else
				{
					if (maze[lblock.x][lblock.y])
					{
						nasl_draw_line(buffer, 175, 63, 182, 66, GREY3);
						nasl_draw_line(buffer, 182, 66, 182, 70, GREY3);
						nasl_draw_line(buffer, 182, 70, 175, 74, GREY3);
					}
					else
					{
						nasl_draw_line(buffer, 175, 66, 182, 66, GREY3);
						nasl_draw_line(buffer, 182, 66, 182, 70, GREY3);
						nasl_draw_line(buffer, 182, 70, 175, 70, GREY3);
					}
					if (maze[rblock.x][rblock.y])
					{
						nasl_draw_line(buffer, 202, 63, 195, 66, GREY3);
						nasl_draw_line(buffer, 195, 66, 195, 70, GREY3);
						nasl_draw_line(buffer, 195, 70, 202, 74, GREY3);
					}
					else
					{
						nasl_draw_line(buffer, 202, 66, 195, 66, GREY3);
						nasl_draw_line(buffer, 195, 66, 195, 70, GREY3);
						nasl_draw_line(buffer, 195, 70, 202, 70, GREY3);
					}
				}
				break;
		}

		// If view is obscured by wall, stop drawing
		if(maze[block.x][block.y]) break;
	}
}

void draw_box(Buffer* buffer)
{
	//left, top, right, bottom
	nasl_draw_rect(buffer, 82, 19, 294, 119, GREY3);
}

static void init(int width, int height)
{
    nasl_graphics_init(width, height, "Neogardens Wireframe Maze Demo", 0, 3);
}

static void shutdown()
{
    nasl_graphics_shutdown();

    return 1;
}
