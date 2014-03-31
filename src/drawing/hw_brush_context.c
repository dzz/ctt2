#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>

#include "../system/surfaceCache.h"

#include "../document/animation.h"
#include "../document/layers.h"

#include "../hwgfx/blend_control.h"
#include "../hwgfx/shader.h"
#include "../hwgfx/texture.h"
#include "../hwgfx/framebuffer.h"
#include "../hwgfx/primitive.h"
#include "../hwgfx/misc.h"

#include "hw_brush_context.h"

typedef struct {
    gfx_shader screen_shader;
    gfx_shader dab_shader;
    gfx_shader brush_composite_shader;

    gfx_texture ui;

    gfx_coordinate_primitive screen_primitive;
    gfx_coordinate_primitive dab_primitive;

    /*active frame*/
    gfx_texture drawing_context_texture;
    gfx_framebuffer drawing_context_framebuffer;

    /*brush buffer*/
    gfx_framebuffer brushing_framebuffer;
    gfx_texture brushing_context_texture;
} brush_context;

brush_context _context;

void _build_ctxt_gfx_data( gfx_texture* tex, gfx_framebuffer* fb ) {
    texture_generate(tex, 1920,1080);
    framebuffer_create_framebuffer(fb);
    framebuffer_bind_texture(fb,tex);
}

void _destroy_ctxt_gfx_data( gfx_texture* tex, gfx_framebuffer* fb) {
    texture_drop(tex);
    framebuffer_drop(fb);
}

void createBrushContext(brush_context *ctxt) {
    shader_load( &ctxt->screen_shader, "shaders/hw_context.vert.glsl",
            "shaders/hw_context.frag.glsl" );
    shader_load( &ctxt->dab_shader, "shaders/dab.vert.glsl",
            "shaders/dab.frag.glsl" );
    shader_load( &ctxt->brush_composite_shader, "shaders/brush_composite.vert.glsl",
            "shaders/brush_composite.frag.glsl" );

    primitive_create_screen_primitive(&ctxt->screen_primitive);
    primitive_create_dab_primitive(&ctxt->dab_primitive);

    texture_generate( &ctxt->brushing_context_texture, 1920, 1080); 
    texture_generate( &ctxt->ui, 1920, 1080);

    _build_ctxt_gfx_data( &ctxt->brushing_context_texture, &ctxt->brushing_framebuffer );
    _build_ctxt_gfx_data( &ctxt->drawing_context_texture, &ctxt->drawing_context_framebuffer );
}

void destroyBrushContext(brush_context *ctxt) {
    shader_drop(&ctxt->dab_shader);
    shader_drop(&ctxt->screen_shader);

    texture_drop(&ctxt->ui);

    _destroy_ctxt_gfx_data(&ctxt->brushing_context_texture, &ctxt->brushing_framebuffer);
    _destroy_ctxt_gfx_data(&ctxt->drawing_context_texture, &ctxt->drawing_context_framebuffer);

    primitive_destroy_coordinate_primitive(&ctxt->screen_primitive);
    primitive_destroy_coordinate_primitive(&ctxt->dab_primitive);
}

void hw_brush_dab(float x,float y,float z, float r,float g, float b,float a, float jit) {
    framebuffer_render_start(&_context.brushing_framebuffer);
    {
        blend_enter( BLENDMODE_DAB_RENDERING ); 
        shader_bind( &_context.dab_shader );
        shader_bind_vec3( &_context.dab_shader, "dab_location",x,y,z );
        shader_bind_vec4( &_context.dab_shader, "base_color",r,g,b,a );
        shader_bind_vec2( &_context.dab_shader, "scr_size",1920.0, 1080.0);
        shader_bind_float(&_context.dab_shader, "jitter",jit); 
        primitive_render( &_context.dab_primitive);
        blend_exit();
    }
    framebuffer_render_end(&_context.brushing_framebuffer);
    hw_brush_commit_brush_stroke();
}

void initHwBrush(){
    createBrushContext(&_context);
}

void dropHwBrush(){ 
    destroyBrushContext(&_context);
}


void _render_drawing_context(brush_context* ctxt) {
    blend_enter( BLENDMODE_OVER ); 
    shader_bind( &ctxt->screen_shader);

    /* drawing context -> the layer we are drawing to */
    texture_bind( &ctxt->drawing_context_texture, TEX_UNIT_0 );
    primitive_render( &ctxt->screen_primitive );

    /*brushing context-> the context we are dabbing to */
    texture_bind( &ctxt->brushing_context_texture, TEX_UNIT_0 );
    primitive_render( &ctxt->screen_primitive );
    blend_exit();
}


void _composite_brush_to_drawing_context() {

    /*select the active frame buffer to splat the brush buffer onto*/
    framebuffer_render_start(&_context.drawing_context_framebuffer);

    shader_bind( &_context.brush_composite_shader);
    blend_enter(BLENDMODE_BRUSH_COMPOSITE); 
    texture_bind( &_context.brushing_context_texture, TEX_UNIT_0 );
    primitive_render( &_context.screen_primitive );
    blend_exit();
    framebuffer_render_end(&_context.drawing_context_framebuffer);

    /*empty the brush buffer*/
    framebuffer_render_start(&_context.brushing_framebuffer);
    gfx_clear();
    framebuffer_render_end(&_context.brushing_framebuffer);
}

void hw_render_layerstack(frame* fr) {
    unsigned int i;
    for(i = 0; i<numLayers;++i) {
        if( i == getActiveLayer() ) {
            _render_drawing_context(&_context);
        } else {
            renderLocalBuffer(getCompositeLayerFromFrame( fr,
                        i,
                        COMP_RESOLVE_VIRTUAL)->data);
        }
    }

}

void hw_brush_commit_brush_stroke() {
    _composite_brush_to_drawing_context();
}

void hw_brush_commit_context(SDL_Surface* frdata) {
    texture_download( &_context.drawing_context_texture, frdata);
}

void renderLocalBuffer( SDL_Surface* img) {
    blend_enter( BLENDMODE_OVER ); 
    shader_bind( &_context.screen_shader);
    texture_from_SDL_surface(&_context.ui,img);
    texture_bind(&_context.ui,TEX_UNIT_0);
    primitive_render( &_context.screen_primitive );
    blend_exit();
}

void hw_import_drawing_context( SDL_Surface* img) {
    texture_from_SDL_surface(&_context.drawing_context_texture,img);
}
