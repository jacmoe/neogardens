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

#include "nasl_script.h"
#include "nasl_graphics.h"
#include "nasl_buffer.h"

static int init();
static int shutdown();

int main()
{

    init();

    // Create main buffer
    Buffer* buffer = nasl_buffer_create(800,600);
    // Clear main buffer to a blue color
    nasl_buffer_clear(buffer, BLUE);

    // Create a sub_buffer
    Buffer* sub_buffer = nasl_buffer_create(175,100);

    // Draw a palette by blitting 16 different sub_buffers into the main buffer
    int col = 0;
    int row = 50;
    for(int buf = 0; buf < 16; buf++)
    {
        nasl_buffer_clear(sub_buffer, c64_palette[buf]);
        nasl_buffer_blit(buffer, sub_buffer, 50 + (175 * col), row);
        col++;
        if(col % 4 == 0)
        {
            col = 0;
            row += 100;
        }
    }

    // Main loop
    while(nasl_graphics_running())
    {
        // Event polling
        nasl_graphics_poll_events();
        // Render the main buffer
        nasl_graphics_render(buffer);
        // Swap buffers
        nasl_graphics_present();
    }

    // Destroy the buffers we created
    nasl_buffer_destroy(sub_buffer);
    nasl_buffer_destroy(buffer);

    shutdown();
}

static int init()
{
    nasl_script_init();
    nasl_script_run("assets/scripts/init.bas");

    nasl_graphics_init(800, 600, "nasl test");

    return 1;
}

static int shutdown()
{
    nasl_graphics_shutdown();
    nasl_script_shutdown();

    return 1;
}
