#pragma once
#include <SDL.h>
#include <SDL_image.h>

class fontMngr {
private:
	static const int FONT_SIZE = 48;

	SDL_Texture* _numSprite;
	SDL_Rect _position;

public:
	fontMngr(int x, int y);
	void setTexture(SDL_Texture* texture) { _numSprite = texture; }

	void init();
	void render(SDL_Renderer* renderer, int score);
};