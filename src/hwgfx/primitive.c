#include <GLXW/glxw.h>

#include "primitive.h"
#include "OGL_OBJ.h"

#define COORDINATE_ATTRIBUTE_INDEX  0
#define UV_ATTRIBUTE_INDEX 1
#define FLOATS_PER_VERT 2

#define NOT_NORMALIZED 0
#define NO_STRIDE 0
#define NO_POINTER_OFFSET 0

/** BASIC TYPE **/
void primitive_create_coordinate_primitive(gfx_coordinate_primitive* 
        primitive, gfx_float* coordinates, int verts ){

    primitive->_num_verts = verts;
    primitive->mode = GL_TRIANGLE_FAN;

    glGenVertexArrays(1, &primitive->vert_array);
    glBindVertexArray(primitive->vert_array);


    //install our coordinates
    glGenBuffers(1, &primitive->vert_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, primitive->vert_buffer);
    glBufferData(GL_ARRAY_BUFFER, (verts*FLOATS_PER_VERT) * sizeof(GLfloat), 
            coordinates, 
            GL_STATIC_DRAW);

    glVertexAttribPointer(COORDINATE_ATTRIBUTE_INDEX, 
            FLOATS_PER_VERT, 
            GL_FLOAT, 
            NOT_NORMALIZED, 
            NO_STRIDE, 
            NO_POINTER_OFFSET ); 

    glEnableVertexAttribArray(COORDINATE_ATTRIBUTE_INDEX);
    
    OGL_OBJ("varray",   primitive->vert_array,  OGL_RECV);
    OGL_OBJ("vbuffer",  primitive->vert_buffer, OGL_RECV);
}


void primitive_destroy_coordinate_primitive(gfx_coordinate_primitive*
        primitive){

    glDeleteVertexArrays(1,&primitive->vert_array);
    glDeleteBuffers(1,&primitive->vert_buffer);

    OGL_OBJ("varray",   primitive->vert_array,  OGL_DROP);
    OGL_OBJ("vbuffer",  primitive->vert_buffer, OGL_DROP);

}

void primitive_render(gfx_coordinate_primitive*
        primitive) {
    glBindVertexArray(primitive->vert_array);
    glDrawArrays(primitive->mode, 0, primitive->_num_verts);
}


/** UV COORD TYPE **/
void primitive_create_coordinate_uv_primitive(gfx_coordinate_uv_primitive* 
        uv_primitive, gfx_float* coordinates, gfx_float* uvs,
        int verts ){

    //initialize basic object
    primitive_create_coordinate_primitive( 
            (gfx_coordinate_primitive*)uv_primitive, 
            coordinates, verts);

    //install our UVs
    glGenBuffers(1, &uv_primitive->uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, uv_primitive->uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, (verts*FLOATS_PER_VERT) * sizeof(GLfloat), 
            uvs, GL_STATIC_DRAW);

    glVertexAttribPointer(UV_ATTRIBUTE_INDEX, 
            FLOATS_PER_VERT, 
            GL_FLOAT, 
            NOT_NORMALIZED, 
            NO_STRIDE, 
            NO_POINTER_OFFSET ); 

    glEnableVertexAttribArray(UV_ATTRIBUTE_INDEX);

    OGL_OBJ("uvbuffer",uv_primitive->uv_buffer,OGL_RECV);
}

void primitive_destroy_coordinate_uv_primitive(gfx_coordinate_uv_primitive*
        uv_primitive) {

    primitive_destroy_coordinate_primitive((gfx_coordinate_primitive*)
            uv_primitive);

    glDeleteBuffers(1,&uv_primitive->uv_buffer);

    OGL_OBJ("uvbuffer",uv_primitive->uv_buffer,OGL_DROP);
}

/** PRIMITIVE TEMPLATES **/

void primitive_create_scrquad_primitive(gfx_coordinate_primitive* primitive) {
    primitive_create_screen_primitive(primitive);
}

void primitive_create_dab_primitive(gfx_coordinate_primitive* primitive) {
    const gfx_float dab_verts[4][2] = {
        {  -1.0, -1.0 }, 
        {  1.0, -1.0 }, 
        {  1.0, 1.0 }, 
        {  -1.0, 1.0 } }; 
    primitive_create_coordinate_primitive(primitive, (gfx_float*)dab_verts, 4);
}

void primitive_create_screen_primitive(gfx_coordinate_primitive* primitive) {
    const gfx_float context_verts[4][2] = {
        {  0.0, 0.0 }, 
        {  1.0, 0.0 }, 
        {  1.0, 1.0 }, 
        {  0.0, 1.0 } }; 
    primitive_create_coordinate_primitive(primitive, (gfx_float*)context_verts, 4); 
}
