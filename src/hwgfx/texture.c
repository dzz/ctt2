#include <SDL.h>
#include <GL/glew.h>
#include "texture.h"

unsigned char* _uc_data(int w,int h) {
        unsigned char* texture_data;
        int i;
        int addr=0;

        texture_data = malloc( w*h*4* sizeof(unsigned char));
        for(i=0; i<(w*h);++i) {
            texture_data[addr++]=0;
            texture_data[addr++]=0;
            texture_data[addr++]=0;
            texture_data[addr++]=0;
        }
        return texture_data;
}
unsigned char* _fp_data(int w,int h) {
        float* texture_data;
        int i;
        int addr=0;

        texture_data = malloc( w*h*4 * sizeof(float));
        for(i=0; i<(w*h);++i) {
            texture_data[addr++]=0.0f;
            texture_data[addr++]=0.0f;
            texture_data[addr++]=0.0f;
            texture_data[addr++]=0.0f;
        }
        return texture_data;
}

#define _LOD 0 
#define _NOBORDER 0 

void texture_generate(gfx_texture* texture,int w,int h) {
    unsigned char* texture_data = _uc_data(w,h);

    glGenTextures(1,&texture->texture_id);
    glBindTexture(GL_TEXTURE_2D,texture->texture_id);
    glTexImage2D(GL_TEXTURE_2D,_LOD,GL_RGBA,w,h ,_NOBORDER,
                GL_RGBA, GL_UNSIGNED_BYTE,texture_data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    free(texture_data);
}

void texture_generate_filtered(gfx_texture* texture,int w,int h) {
    unsigned char* texture_data = _uc_data(w,h);

    glGenTextures(1,&texture->texture_id);
    glBindTexture(GL_TEXTURE_2D,texture->texture_id);
    glTexImage2D(GL_TEXTURE_2D,_LOD,GL_RGBA,w,h ,_NOBORDER,
                GL_RGBA, GL_UNSIGNED_BYTE,texture_data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    free(texture_data);
}

void texture_generate_fp(gfx_texture* texture,int w,int h) {
    unsigned float* texture_data = _fp_data(w,h);
    glGenTextures(1,&texture->texture_id);
    glBindTexture(GL_TEXTURE_2D,texture->texture_id);
    glTexImage2D(GL_TEXTURE_2D,_LOD,GL_RGBA,w,h ,_NOBORDER,
                GL_RGBA, GL_FLOAT,texture_data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    free(texture_data);
}

void texture_from_SDL_surface(gfx_texture* texture, SDL_Surface* surf) {
    printf("\n{%d}\n",surf->w);
    SDL_LockSurface(surf);
    if(surf->w!=24) {
    glBindTexture(GL_TEXTURE_2D,texture->texture_id);
    glTexImage2D(GL_TEXTURE_2D,_LOD,GL_RGBA,surf->w,surf->h ,_NOBORDER,
                GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,(unsigned char*)surf->pixels);}
    SDL_UnlockSurface(surf);

}

void texture_from_SDL_surface_grayscale(gfx_texture* texture, SDL_Surface* surf) {
    SDL_LockSurface(surf);
    glBindTexture(GL_TEXTURE_2D,texture->texture_id);
    glTexImage2D(GL_TEXTURE_2D,_LOD,GL_RGBA,surf->w,surf->h ,_NOBORDER,
                GL_RED, GL_UNSIGNED_BYTE,(unsigned char*)surf->pixels);
    SDL_UnlockSurface(surf);

}

void texture_drop(gfx_texture* texture) {
    glDeleteTextures(1,&texture->texture_id);
}

void texture_bind(gfx_texture* texture, int texture_unit) {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture->texture_id );
}

void texture_download(gfx_texture* texture, SDL_Surface* target) {
    texture_bind(texture, TEX_UNIT_0);
    SDL_LockSurface(target);
    glGetTexImage(GL_TEXTURE_2D, _LOD, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, target->pixels );
    SDL_UnlockSurface(target);
}

