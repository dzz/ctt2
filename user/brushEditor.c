#include "brushEditor.h"
#include "../drawing/drawingSurfaces.h"
#include "../drawing/brush.h"
#include "h_slider.h"

#include <SDL.h>

static SDL_Surface* bg;
static SDL_Surface* slider;
static SDL_Surface* handle;
static SDL_Surface* preview_bg;

double sVals[32];
unsigned int toggles[32];

SDL_Surface* slider_images[64];
SDL_Surface* button_images[64];

double get_brusheditor_value(int idx) {
	return sVals[idx];
}

void brusheditor_redraw_stroke() {
	redraw_stroke_sample();
}

void redraw_stroke_sample() {
	//285 to 385, and same height as sliders
	SDL_Surface *brushSample = createDrawingSurface(100,244);
	SDL_BlitSurface(preview_bg,NULL,brushSample,NULL);
	brush_setValuesFromUI();
	{
		int v;
		for( v = 0; v< 200; ++v ) {
				float p = (float)v/200;
				int x = (int)((cosf((p*3.14*4)))*10) + 57;
				brush_drawStrokeSegment(
								x,v,x,v+1,p,p,brushSample);
		}
	}

	SDL_Rect rect;
	rect.x = 295;
	rect.y = 6;
	rect.w = 100;
	rect.h = 244;

	SDL_BlitSurface(brushSample,NULL,bg,&rect);
	SDL_FreeSurface(brushSample);
}

int button_origin_x;
int button_origin_y;

SDL_Rect get_buttons_rect() {
	SDL_Rect r;
	r.w = 45;
	r.h = 20;

	r.x = 230;
	r.y = 7;
	return r;
}
#define NUM_BUTTONS 5
void draw_buttons_to_bg() {
	int i;
	SDL_Rect r= get_buttons_rect();

	for(i=0; i< NUM_BUTTONS; ++i) {
			SDL_BlitSurface(button_images[(i*2)+toggles[i]],NULL,bg,&r);
			r.y+=r.h;
	}
}

int get_brusheditor_toggle(int idx) {
	return toggles[idx];
}

int get_brusheditor_radio() {
	return toggles[2]+(toggles[3]*2)+(toggles[4]*3);
}

void initBrushEditor() {
	bg = SDL_LoadBMP("ui_gen/be_bg.bmp");
	slider = SDL_LoadBMP("ui_gen/be_slider.bmp");
	handle = SDL_LoadBMP("be_handle.bmp");
	preview_bg = SDL_LoadBMP("be_preview.bmp");

	toggles[2] = 1;
	toggles[0] = 1;
	sVals[0] = 0.5; //size
	sVals[1] = 0.5; //power
	sVals[2] = 0.5; //alpha
	sVals[3] = 1;  //pressure dynamics
	sVals[4] = 0.0; //jitter
	sVals[5] = 0.0; //noise

	slider_images[0] = SDL_LoadBMP("ui_gen/slider_0.bmp");
	slider_images[1] = SDL_LoadBMP("ui_gen/slider_1.bmp");
	slider_images[2] = SDL_LoadBMP("ui_gen/slider_2.bmp");
	slider_images[3] = SDL_LoadBMP("ui_gen/slider_3.bmp");
	slider_images[4] = SDL_LoadBMP("ui_gen/slider_4.bmp");
	slider_images[5] = SDL_LoadBMP("ui_gen/slider_5.bmp");

	button_images[0] = SDL_LoadBMP("ui_gen/button_0_up.bmp");
	button_images[1] = SDL_LoadBMP("ui_gen/button_0_down.bmp");
	button_images[2] = SDL_LoadBMP("ui_gen/button_1_up.bmp");
	button_images[3] = SDL_LoadBMP("ui_gen/button_1_down.bmp");
	button_images[4] = SDL_LoadBMP("ui_gen/button_2_up.bmp");
	button_images[5] = SDL_LoadBMP("ui_gen/button_2_down.bmp");
	button_images[6] = SDL_LoadBMP("ui_gen/button_3_up.bmp");
	button_images[7] = SDL_LoadBMP("ui_gen/button_3_down.bmp");
	button_images[8] = SDL_LoadBMP("ui_gen/button_4_up.bmp");
	button_images[9] = SDL_LoadBMP("ui_gen/button_4_down.bmp");

	redraw_stroke_sample();
	draw_buttons_to_bg();
}

void destroyBrushEditor() {
	int i;
	for(i=0;i<6;++i) {
		SDL_FreeSurface(slider_images[i]);
	}
	for(i=0;i<8;++i) {
		SDL_FreeSurface(button_images[i]);
	}

	SDL_FreeSurface(bg);
	SDL_FreeSurface(slider);
	SDL_FreeSurface(handle);
	SDL_FreeSurface(preview_bg);
}

void renderBrushEditor(SDL_Surface *target, UI_AREA *area) {
		int i;
		SDL_Rect spos;

		SDL_BlitSurface(bg,NULL,target,area);

		//our slider images are 30x244 and we have a
		// 400x256 area to fill with sliders

		for(i=0; i<6; ++i) {
			spos.x = (area->x + (i*35))+10; //5px margin
			spos.y = area->y + 6; //12px margin
			spos.w = 30;
			spos.h = 244;
			SDL_BlitSurface(slider_images[i], NULL,target,&spos);

			spos.y = (int)(((1-sVals[i])*(spos.h-12))+spos.y-8)+6;
			spos.h = 16;
			SDL_BlitSurface(handle, NULL,target,&spos);
		}

}

void handle_mousedown_for_buttons(int x, int y, UI_AREA *area) {
	SDL_Rect r = get_buttons_rect();
	if((x>r.x) && (x< (r.x+r.w))){
		int button_id = (y-r.y) / r.h;
		if(button_id>=0) {
			toggles[button_id] = !toggles[button_id];
			//manual radio buttoning
			if(button_id>1) {
					switch(button_id) {
							case 2:
									toggles[3] = 0;
									toggles[4] = 0;
									break;
							case 3:
									toggles[2] = 0;
									toggles[4] = 0;
									break;
							case 4:
									toggles[2] = 0;
									toggles[3] = 0;
									break;
					}
			}
			redraw_stroke_sample();
			draw_buttons_to_bg();

		}
	}
}

void brusheditor_mousedown(int x,int y, UI_AREA *area) {
		int idx = (x-10) / 35;
		int vert = (y-6);

		//we're in the margins
		if(idx<0) {
				return;
		}
		//we have six sliders
		if(idx>6) {
			handle_mousedown_for_buttons(x,y,area);
		}
		else {
			sVals[idx] = (double)vert / 244.0;
			if(sVals[idx]>1) sVals[idx]=1;
			if(sVals[idx]<0) sVals[idx]=0;
			sVals[idx] = 1-sVals[idx];

			srand(0);
			redraw_stroke_sample();
		}
}
