#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>

#include "./clock_widget.h"

#include "./seven_segment.h"

static volatile int running = 1;
void intHandler(int dummy)
{
	running = 0;
}

int main(int argc, char* argv[])
{
	signal(SIGINT, intHandler);
	int width = 700, height = 100;
	int pos_x = SDL_WINDOWPOS_CENTERED, pos_y =SDL_WINDOWPOS_CENTERED;
	char *path = NULL;
	SDL_Color c = {.r = 0, .g = 255, .b = 0};
	SDL_Color bc = {.r = 0, .g = 0, .b = 0};
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0)
		{
			if (sscanf(argv[++i], "%d", &width));
			else printf("Warning! invalid input for flag %s\n", argv[i]);
		} else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--height") == 0)
		{
			if (sscanf(argv[++i], "%d", &height));
			else printf("Warning! invalid input for flag %s\n", argv[i]);
		}  else if (strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--pos_x") == 0)
		{
			if (sscanf(argv[++i], "%d", &pos_x));
			else printf("Warning! invalid input for flag %s\n", argv[i]);
		} else if (strcmp(argv[i], "-y") == 0 || strcmp(argv[i], "--pos_y") == 0)
		{
			if (sscanf(argv[++i], "%d", &pos_y));
			else printf("Warning! invalid input for flag %s\n", argv[i]);
		}
		else if (strcmp(argv[i], "--help") == 0)
		{
			print_help();
		}
		else if (strcmp(argv[i], "--img-src") == 0)
		{
			size_t len = sizeof(argv[++i]);
			path = malloc(len+1);
			strcpy(path, argv[++i]);
		}
		else if (strcmp(argv[i], "-r") == 0)
		{
			int temp = 0;
			if ( ((sscanf(argv[++i], "%d", &temp)) <= 255) && (temp >= 0) )
				c.r = temp;
			else 
				printf("Error. Invalid colour value for red");
		}
		else if (strcmp(argv[i], "-g") == 0)
		{
			int temp = 0;
			if ( ((sscanf(argv[++i], "%d", &temp)) <= 255) && (temp >= 0) )
				c.g = temp;
			else 
				printf("Error. Invalid colour value for green");
		}
		else if (strcmp(argv[i], "-b") == 0)
		{
			int temp = 0;
			if ( ((sscanf(argv[++i], "%d", &temp)) <= 255) && (temp >= 0) )
				c.b = temp;
			else 
				printf("Error. Invalid colour value for blue");
		}		else if (strcmp(argv[i], "-br") == 0)
		{
			int temp = 0;
			if ( ((sscanf(argv[++i], "%d", &temp)) <= 255) && (temp >= 0) )
				bc.r = temp;
			else 
				printf("Error. Invalid colour value for red");
		}
		else if (strcmp(argv[i], "-bg") == 0)
		{
			int temp = 0;
			if ( ((sscanf(argv[++i], "%d", &temp)) <= 255) && (temp >= 0) )
				bc.g = temp;
			else 
				printf("Error. Invalid colour value for green");
		}
		else if (strcmp(argv[i], "-bb") == 0)
		{
			int temp = 0;
			if ( ((sscanf(argv[++i], "%d", &temp)) <= 255) && (temp >= 0) )
				bc.b = temp;
			else 
				printf("Error. Invalid colour value for blue");
		}
		else
		{
			printf("Unlnown flag: %s!\n", argv[i]);
			print_help();
		}
	}
	init_window(width, height, pos_x, pos_y);
	calculate_clock_scale(width, height);
	init_digit_places();
	if (path != NULL) {
		load_imgs(path);
		while (running)
		{
			get_time();
			draw_digits();
			SDL_Delay(10);
		}
	}
	else {
		while (running)
		{
			get_time();
			create_digits(c, bc);
			SDL_Delay(10);
		}
	}
	printf("\n");
	destroy_window();
	return 0;
}

void print_help()
{
	printf("-x --pos_x: Set window position coordinate X. Expects integer\n"
			"-y --pos_y: Set window position coordinate Y. Expects integer\n"
			"-w --width: Set window width. Expects integer\n"
			"-h --height: Set window height. Expects integer\n"
			"--img-src to select a folder from which to take digits (use with caution!)\n"
			"\tnote that the format has to have the final slash i.e. 'src/img/' \n"
			"\tnote also that the path has to contain files named 0.bmp to 9.bmp ans s.bmp for the separator\n"
			"-r -g -b parameters are used to select colour. All three flags expect an integer from 0 to 255"
			"-br -bg -bb parameters are used to select bg colour. All three flags expect an integer from 0 to 255"
			"--help Show help\n");
	exit(0);
}

void calculate_clock_scale(int w, int h)
{
	my_scale.margin_ver = h/100;
	my_scale.margin_hor = w/100;
	my_scale.size_y = h - 2*my_scale.margin_ver;
	my_scale.size_x = (w-my_scale.margin_hor*2)/7;
	my_scale.distance = my_scale.margin_hor/2;
}

void get_time(void)
{
	time_t raw_time;
	time(&raw_time);
	tell_the_time = localtime(&raw_time);
}

int init_window(int w, int h, int pos_x, int pos_y)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return 1;
	}
	window = SDL_CreateWindow(
		"Clock Widget",
		pos_x,
		pos_y,
		w,
		h,
		SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS
	);
	if (!window)
	{
		fprintf(stderr, "Error initializing window.\n");
		return 1;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error initializing renderer.\n");
		return 1;
	}
	return 0;
}

int destroy_window(void)
{
	for (int i = 0; i<11; i++)
	{
		SDL_DestroyTexture(digits[i]);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void load_imgs(char *img_src) {
	size_t len = strlen(img_src);
	char *src = malloc(len + 5 + 1);
	strcpy(src, img_src);
	src[len + 1] = '.';
	src[len + 2] = 'b';
	src[len + 3] = 'm';
	src[len + 4] = 'p';
	src[len + 5] = '\0';
	src[len] = 's';
	digits[10] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(src));
	for (int i = 0; i < 10; i++)
	{
		src[len] = '0'+i;
		digits[i] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(src));
		printf("%s\n", src);
	}
}

void init_digit_places(void)
{
	int margin = my_scale.margin_hor;
	for (int i = 0; i < 8; i++)
    {
		if (i == 2 || i == 5)
			digit_places[i].w = my_scale.size_x/3;
		else
			digit_places[i].w = my_scale.size_x;
		digit_places[i].h = my_scale.size_y;
		digit_places[i].x = margin;
		digit_places[i].y = my_scale.margin_ver;
		margin+=digit_places[i].w+my_scale.distance;
	}
}

void draw_digits(void)
{
	SDL_SetRenderDrawColor(renderer, 1, 1, 1, 1);
	SDL_RenderClear(renderer);
	int digit = tell_the_time->tm_hour/10;
	SDL_RenderCopy(renderer, digits[digit], NULL, &digit_places[0]);

	digit = tell_the_time->tm_hour%10;
	SDL_RenderCopy(renderer, digits[digit], NULL, &digit_places[1]);

	SDL_RenderCopy(renderer, digits[10], NULL, &digit_places[2]);

	digit = tell_the_time->tm_min/10;
	SDL_RenderCopy(renderer, digits[digit], NULL, &digit_places[3]);

	digit = tell_the_time->tm_min%10;
	SDL_RenderCopy(renderer, digits[digit], NULL, &digit_places[4]);

	SDL_RenderCopy(renderer, digits[10], NULL, &digit_places[5]);

	digit = tell_the_time->tm_sec/10;
	SDL_RenderCopy(renderer, digits[digit], NULL, &digit_places[6]);

	digit = tell_the_time->tm_sec%10;
	SDL_RenderCopy(renderer, digits[digit], NULL, &digit_places[7]);

	SDL_RenderPresent(renderer);
}

void create_digits(SDL_Color c, SDL_Color bc)
{

	SDL_SetRenderDrawColor(renderer, bc.r, bc.g, bc.b, 255);
	SDL_RenderClear(renderer);

	int digit = tell_the_time->tm_hour/10;
	calculate_scale(digit_places[0].h, digit_places[0].w);
	init_segments(digit_places[0].x, digit_places[0].y);
	draw_digit(renderer, digit, c);

	digit = tell_the_time->tm_hour%10;
	calculate_scale(digit_places[1].h, digit_places[1].w);
	init_segments(digit_places[1].x, digit_places[1].y);
	draw_digit(renderer, digit, c);

	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
	draw_separator(renderer, digit_places[2].x, digit_places[2].y);

	digit = tell_the_time->tm_min/10;
	calculate_scale(digit_places[3].h, digit_places[3].w);
	init_segments(digit_places[3].x, digit_places[3].y);
	draw_digit(renderer, digit, c);
	
	digit = tell_the_time->tm_min%10;
	calculate_scale(digit_places[4].h, digit_places[4].w);
	init_segments(digit_places[4].x, digit_places[4].y);
	draw_digit(renderer, digit, c);

	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
	draw_separator(renderer, digit_places[5].x, digit_places[5].y);

	digit = tell_the_time->tm_sec/10;
	calculate_scale(digit_places[6].h, digit_places[6].w);
	init_segments(digit_places[6].x, digit_places[6].y);
	draw_digit(renderer, digit, c);

	digit = tell_the_time->tm_sec%10;
	calculate_scale(digit_places[7].h, digit_places[7].w);
	init_segments(digit_places[7].x, digit_places[7].y);
	draw_digit(renderer, digit, c);
	SDL_RenderPresent(renderer);
}
