#pragma once
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "color.h"

extern int const nx;
extern int const ny;

SDL_Window		*window;
SDL_Renderer	*renderer;

using namespace std;

inline void setColor(float r, float g, float b, float a = 1.0) {
	r = (r > 1.0f) ? 1.0f : ((r < 0.0f) ? 0.0f : r);
	g = (g > 1.0f) ? 1.0f : ((g < 0.0f) ? 0.0f : g);
	b = (b > 1.0f) ? 1.0f : ((b < 0.0f) ? 0.0f : b);
	a = (a > 1.0f) ? 1.0f : ((a < 0.0f) ? 0.0f : a);
	SDL_SetRenderDrawColor(renderer, Uint8(r * 255.0), Uint8(g * 255.0), Uint8(b * 255.0), Uint8(a * 255.0));
}

inline void setPixel(int x, int y) {
	SDL_RenderDrawPoint(renderer, x, ny - y - 1);
}

inline void setPixel(int x, int y, float r, float g, float b, float a = 1.0) {
	setColor(r, g, b, a);
	setPixel(x, y);
}

inline void drawLine(int x1, int y1, int x2, int y2) {
	SDL_RenderDrawLine(renderer, x1, ny - y1 - 1, x2, ny - y2 - 1);
}

bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow = window, SDL_Renderer* SDLRenderer = renderer) {
	SDL_Surface* saveSurface = NULL;
	SDL_Surface* infoSurface = NULL;
	infoSurface = SDL_GetWindowSurface(SDLWindow);
	if (infoSurface == NULL) {
		std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
	}
	else {
		unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
		if (pixels == 0) {
			std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
			return false;
		}
		else {
			if (SDL_RenderReadPixels(SDLRenderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
				std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
				pixels = NULL;
				return false;
			}
			else {
				saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
				if (saveSurface == NULL) {
					std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
					return false;
				}
				SDL_SaveBMP(saveSurface, filepath.c_str());
				SDL_FreeSurface(saveSurface);
				saveSurface = NULL;
			}
			delete[] pixels;
		}
		SDL_FreeSurface(infoSurface);
		infoSurface = NULL;
	}
	return true;
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16 *)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32 *)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

SDL_Surface* loadTexture(const char* image_path, int& imageWidth, int& imageHeight)
{
	SDL_Surface* image = IMG_Load(image_path);
	if (!image)
	{
		cout << "IMG_Load: " << IMG_GetError() << std::endl;
		return NULL;
	}

	imageWidth = image->w;
	imageHeight = image->h;

	if (image->format->BitsPerPixel != 24) {
		cout << "Not an 8-bit surface" << std::endl;
		return NULL;
	}

	if (image->format->BytesPerPixel != 3) {
		cout << "Not an 4-byte surface" << std::endl;
		return NULL;
	}

	color** data = new color*[image->h];
	if (image->h) {
		data[0] = new color[image->h * image->w];
		for (int i = 1; i < image->h; i++)
			data[i] = data[0] + i * image->w;
	}

	for (int x = 0; x < image->w; x++) {
		for (int y = 0; y < image->h; y++) {

			Uint32 value = getpixel(image, x, y);

			float red = float((value >> 16) & 0xff) / 255.0f;
			float green = float((value >> 8) & 0xff) / 255.0f;
			float blue = float(value & 0xff) / 255.0f;

			data[y][x] = color(red, green, blue);

		}
	}

	//SDL_FreeSurface(image);
	return image;
}