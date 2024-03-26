#ifndef SEVEN_SEGMENT_HEADER
#define SEVEN_SEGMENT_HEADER
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdint.h>

void draw_digit(SDL_Renderer *, int);
void init_segments(void);
void calculate_scale(int, int);

#endif /* SEVEN_SEGMENT_HEADER */