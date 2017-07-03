#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "cardBundleLain.h"
#include "cardBundleOutput.h"
#include "cardBundleStockPile.h"
#include "fontMngr.h"

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 768;

class Framework {
private:
	SDL_Window* _window;
	SDL_Surface* _screenSurface;
	SDL_Renderer* _renderer;
	SDL_Texture* _cardSprite;
	SDL_Texture* _gameTable;
	SDL_Texture* _endButton;
	bool _run;
	bool _completeExit;
	int _score;

	SDL_Rect _endButtonPosition;

	// ī�� ���ϵ��� ������ �迭
	// 0 ~ 6 Lain, 7 ~ 10 Output, 11 StockPile
	cardBundle* _cardBundles[12];

	// ���� ��� ���� Ŭ����
	fontMngr* _fm;

	void setupRenderer();
	bool loadImages();

public:
	Framework();

	bool init();
	int run();   // ���� ȹ�� ����(��) ��ȯ
	void keyHandling();
	void render();
	void update();
	bool cleanup(); // ���� ���� ���� ���� ��ȯ
};