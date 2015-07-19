#if defined(__STDC__)
# define C89
# if defined(__STDC_VERSION__)
#  define C90
#  if (__STDC_VERSION__ >= 199409L)
#   define C94
#  endif
#  if (__STDC_VERSION__ >= 199901L)
#   define C99
#  endif
# endif
#endif

#include "audio.h"

#include <math.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>

int cur_channel = 0;
audio_track* tracks[ 32 ];

void initAudio() {
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio( AUDIO_SAMPLERATE, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNKSIZE );
    Mix_AllocateChannels( 32 );
}

void audio_create_clip(audio_clip* clip, char* clip_name) {
    clip->ChunkData = 0;
    clip->ChunkData=Mix_LoadWAV(clip_name);
    if(clip->ChunkData == 0) {

       printf(clip_name);
       printf(Mix_GetError());
       exit(1);
    }

    printf("%d -- clip->ChunkData\n",clip->ChunkData);
}

void audio_drop_clip(audio_clip* clip) {
    Mix_FreeChunk(clip->ChunkData);
    clip->ChunkData = 0;
}

void dropAudio() {
    Mix_CloseAudio();
    Mix_Quit();
}

void audio_create_track(audio_track* track) {
    if(cur_channel != 16 ) {
        track->track_num = cur_channel;
        tracks[cur_channel] = track;
        cur_channel++;
        printf("CURRENT CHANNEL: %i\n",cur_channel);
    } else {
        printf("too many channels.. \n");
        exit(1);
    }
}

void audio_play_clip_on_track(audio_clip* clip, audio_track* track, unsigned int loop) {
    if(loop==0)  {
        printf("%i\n",clip->ChunkData);

        if( Mix_PlayChannel(track->track_num, clip->ChunkData,0) == -1) {
           printf(Mix_GetError());
           exit(1);
        }
    }
    else {
        Mix_PlayChannel(track->track_num, clip->ChunkData,-1);
    }
}

#ifndef C99

float fmin(double a, double b) {
    if(a<b) return a; return a;
}

float fmax(double a, double b) {
    if(a>b) return a; return b;
}

#endif

void audio_reset_tracks() {
    int i;
    for(i=0; i<32;++i) {
        tracks[i] = 0;
    }
    cur_channel=0;
}

void audio_set_volume_on_track(audio_track* track, double volume) {
    printf("set vol for %d to %f\n",track->track_num,volume);
    Mix_Volume(track->track_num, (int)(volume*127.0));
}

void audio_set_track_panning(audio_track* track, double pan) {

    unsigned int pan_t = (unsigned int)floor(fmin(fmax(pan,1),-1)*127);
    Mix_SetPanning(track->track_num, 127+pan_t, 127-pan_t);
}
