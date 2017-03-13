#define  APP_IMPLEMENTATION
#define  APP_NULL
#include "app.h"

#include <stdlib.h> // for rand and __argc/__argv
#include <string.h> // for memset

int app_proc( app_t* app, void* )
{
    APP_U32 canvas[ 320 * 200 ]; // a place for us to draw stuff
    memset( canvas, 0xC0, sizeof( canvas ) ); // clear to grey
    app_screenmode( app, APP_SCREENMODE_WINDOW );

    // keep running until the user close the window
    while( app_yield( app ) != APP_STATE_EXIT_REQUESTED )
    {
        // plot a random pixel on the canvas
        int x = rand() % 320;
        int y = rand() % 200;
        int color = rand() | ( rand() << 16 );
        canvas[ x + y * 320 ] = color;

        // display the canvas
        app_present( app, canvas, 320, 200, 0xffffff, 0x000000 );
    }
    return 0;
}

int main( int argc, char** argv )
{
    (void) argc, argv;
    return app_run( app_proc, NULL, NULL, NULL, NULL );
}
