//#define CTT2_SCREENMODE_DEBUG 

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/wglext.h>
#endif

#ifdef __linux__
/*
 *
 *
 */
#endif

/*debug sux*/
#undef _DEBUG

#include <Python.h>

#include <SDL.h>
#include <SDL_syswm.h>

#include "system/ctt2.h"
#include "system/ctt2_host.h"

#include "drawing/node_resource_ids.h"
#include "system/extended_video.h"
#include "system/wm_handler.h"
#include "system/log.h"
#include "system/surfaceCache.h"
#include "hardware/tablet.h"

#include "document/animation.h"

#include "drawing/shader_brush.h"
#include "drawing/brush.h"
#include "drawing/drawingSurfaces.h"

#include "compositor/compositor.h"

#include "document/layers.h"

#include "user/stylus.h"
#include "user/panels.h"
#include "user/dispatch.h"
#include "user/yank_put.h"

#include "user/editors/colorPicker.h"

static SDL_Window* opengl_window;
static SDL_Surface* ui_surface = NULL;
static int drawingContextInvalid = 1;

static SDL_GLContext gl_context;

//#define CTT2_SCREENMODE_DEBUG

#ifndef CTT2_SCREENMODE_DEBUG
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;
#else
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
#endif

/**************************************/

static unsigned int ctt2_keyframe_mode = 0;

void toggleKeyframingMode() {
    ctt2_keyframe_mode = !ctt2_keyframe_mode;
}

unsigned int getKeyframingMode() {
    return ctt2_keyframe_mode;
}

void ctt2_insertkeyframe() {
    animation_insert_keyframe_at_cursor();
    animation_cursor_move( 0, DO_NOT_COMMIT_DRAWING_CONTEXT );

}

/**************************************/

void updateViewingSurface() {
    SDL_GL_SwapWindow( opengl_window );
    glClear(GL_COLOR_BUFFER_BIT);
}

SDL_Surface* getViewingSurface(){
    return ui_surface;
}

/**************************************/

void disable_vsync()
{	
#ifdef _WIN32
    typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)( int );
    PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
    wglSwapIntervalEXT = 
        (PFNWGLSWAPINTERVALPROC)SDL_GL_GetProcAddress( "wglSwapIntervalEXT" );
    wglSwapIntervalEXT(0);
#endif
#ifdef __linux__
    printf("linux unimplemented disable_vsync()");
#endif
}


void initWindowingSystemMessages() {
    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
//SDL_ShowCursor(0);
}

/**************************************/

void initDisplay() {
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "%s\n", SDL_GetError() );
        exit(1);
    } 

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    opengl_window = SDL_CreateWindow( "ctt2_hw", 0, 0, 
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

    if( opengl_window == NULL ) {
        printf( "%s\n", SDL_GetError() );
        exit(1);
    }
}

void dropDisplay() {
    SDL_DestroyWindow( opengl_window);
}

void DIRTY_DISPLAY_ABORT() {
    dropDisplay();
}

/**************************************/

void initOpenGL() {
    gl_context = SDL_GL_CreateContext(opengl_window);	
    disable_vsync();
    initExtendedVideo();
}

void dropOpengl() {
    printf("Dropping ogl\n");
    SDL_GL_DeleteContext(gl_context);
}

/**************************************/

void dropPython(){
    if(PyErr_Occurred()) {
        PyErr_Print();
        printf("press a key...\n");
        _getch();
    }
    api_drop();
    printf("CH:Py_Finalize()\n");
    Py_Finalize();
}

void initPython() {
    Py_SetProgramName("ctt2_py");
    if( api_init() == API_FAILURE ) {
        dropPython();
        exit(1);
    }
}



/*****************************************************************************/

int main(int argc, char **argv){ 
    const int CYCLES_BETWEEN_SCREENBUFFER_UPDATES   = 240;
    int screenbuffer_cycles                         = 20;
    int finished                                    = 0;

    initLog();
    initDisplay();
    initWindowingSystemMessages();
    initOpenGL();
    initCompositor();
    initLayers();
    initAnimation();
    initHwBrush();
    initBrush();
    initPython();
    animation_cursor_move(0,DO_NOT_COMMIT_DRAWING_CONTEXT);

    initYankPut();
    initTablet(opengl_window);

    ui_surface = createDrawingSurface(1366,768);

    initPanels(ui_surface);

    /** MAIN DISPATCH LOOP **/
    {
        SDL_Event event;

        while(finished == 0) {
            if(SDL_PollEvent(&event)) {
                
                switch (event.type) {
                    case SDL_QUIT:
                        finished = 1;
                        break;
                    case SDL_SYSWMEVENT:
                        handle_wm_event(event);
                        break;
                    case SDL_KEYDOWN:
                        finished = dispatch_key(event.key.keysym.sym,1);
                        if( api_dispatch_key(event.key.keysym.sym,1) 
                                == API_FAILURE ) finished = 1;
                        break;
                    case SDL_KEYUP:
                        dispatch_key(event.key.keysym.sym,0);
                        if( api_dispatch_key(event.key.keysym.sym,1) 
                                == API_FAILURE ) finished = 1;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        dispatch_mousedown(event.button.button,
                                event.button.x,
                                event.button.y );
                        if(api_dispatch_mousedown(
                                    event.button.button, 
                                    event.button.x, 
                                    event.button.y) == API_FAILURE ) 
                                        finished = 1;
                        break;
                    case SDL_MOUSEBUTTONUP:
                        dispatch_mouseup(event.button.button,
                                event.button.x,
                                event.button.y );
                        if(api_dispatch_mouseup(
                                    event.button.button, 
                                    event.button.x, 
                                    event.button.y) == API_FAILURE ) 
                                        finished = 1;
                        break;
                    case SDL_MOUSEMOTION:
                        dispatch_mousemotion(event.motion.x, 
                                event.motion.y );

                        if(api_dispatch_mousemotion(
                                    event.motion.x, 
                                    event.motion.y) == API_FAILURE ) 
                                        finished = 1;
                        break;
                }
            }


            if(screenbuffer_cycles++ > CYCLES_BETWEEN_SCREENBUFFER_UPDATES ) {
                frame* fr           = getActiveFrame();
                screenbuffer_cycles = 0;

                hw_render_layerstack(fr);
                if( getPanelsEnabled() == PANELS_ENABLED ){
                    SDL_FillRect(ui_surface, NULL, 
                            SDL_MapRGBA( ui_surface->format, 0,0,0,0));
                    renderPanels        (ui_surface);
                    gfx_surface_render  (ui_surface);
                }
                if(api_tick() == API_FAILURE) { finished = 1; }
                updateViewingSurface();
            }
        }
    }
    /** FINISHED **/
    dropBrush();
    dropHwBrush();
    dropTablet();
    dropAnimation();
    dropPanels();
    dropDrawingSurfaces();
    dropYankPut();
    dropPython();
    dropExtendedVideo();
    dropOpengl();
    dropDisplay();
    dropLog();
    SDL_Quit();
    return 0;
}
