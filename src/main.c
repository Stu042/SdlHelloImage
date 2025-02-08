#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>


// Just using a number is generally bad practice, so here we define the return values the app will use.
#define ERROR_NO_ERROR (0)
#define ERROR_INIT_FAILED (1)
#define ERROR_WINDOW_FAILED (2)
#define ERROR_RENDERER_FAILED (3)
#define ERROR_LOAD_TEXTURE_FAILED (4)
#define ERROR_TEXTURE_QUERY_FAILED (5)


// Definition of a new window
typedef struct NEW_WINDOW {
	int width;
	int height;
	const char* title;
} NewWindow;


const NewWindow windowDef = {
	.width = 800,
	.height = 600,
	.title = "Hello World"
};

// Path to image, path relative to the executable to the copied assets folder (copied in CMakeLists.txt)
const char *imagePath = "assets/Croft.png";



// App start
int main(int argc, char *argv[]) {
	printf("Starting app\n");

	// Start SDL, we are using video and events, events will init automatically when we as for video.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error: Unable to initialise SDL. SDL Error: %s\n", SDL_GetError());
		return ERROR_INIT_FAILED;
	}

	// We want to be able to see whats happening so open a window.
	SDL_Window* window = SDL_CreateWindow(windowDef.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowDef.width, windowDef.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		printf("Error: Window could not be created! SDL Error: %s\n", SDL_GetError());
		SDL_Quit();
		return ERROR_WINDOW_FAILED;
	}

	// And in this window we want to render something
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("Error: Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return ERROR_RENDERER_FAILED;
	}

    // Load an image to display
	SDL_Texture *image = IMG_LoadTexture(renderer, imagePath);
	if (image == NULL) {
		printf("Error: Unable to load texture! SDL Error: %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return ERROR_LOAD_TEXTURE_FAILED;
	}

    // I dont want to stretch the image, so we need the image size to render it properly.
	int w;
	int h;
	if (SDL_QueryTexture(image, NULL, NULL, &w, &h) < 0) {
		printf("Error: Unable to query texture! SDL Error: %s\n", SDL_GetError());
		SDL_DestroyTexture(image);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return ERROR_TEXTURE_QUERY_FAILED;
	}
    int startX = (windowDef.width - w) / 2;
    int startY = (windowDef.height - h) / 2;

	// Setup variables for game loop, rect could be moved into the loop but very slightly better performance having it set outside.
	const SDL_Rect dstRect = {startX, startY, w, h};

	// Only rendering an image and clearing the display, we always want to clear to the same colour so setting here.
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	bool appIsRunning = true;

	// Main loop
	while (appIsRunning) {
		// Check for sdl events (only for quit so far, mostly close window button clicked)
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) { // When we click the close window button set the appIsRunning to exit the main loop.
				appIsRunning = false;     // We still render one frame but that is a no biggy.
			}
		}

		// Render
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, image, NULL, &dstRect);

		// Tell sdl to show the rendered buffer
		SDL_RenderPresent(renderer);
		SDL_Delay(0);
	}

	// Quit the app, tidy up first
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return ERROR_NO_ERROR;
}
