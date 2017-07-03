#pragma once

#include "cardBundle.h"

class cardBundleStockPile : public cardBundle {
private:
	static const int _GAP = 10; // 뒤집어질 카드 덱과의 차이

	int _x, _y;
	int _xn, _yn; // 뒤집어진 카드덱 위치
	bool _isClicked;

	int _openedIndex;
	int _mouseX, _mouseY;

	void shuffle(int* cards);
public:
	cardBundleStockPile(int x, int y);

	void init();
	void cleanup();

	int keyHandling(SDL_Event e);
	void render(SDL_Renderer* renderer, SDL_Texture* cardSprite);
	void update();

	// 카드 재정렬 메소드
	void alignCardPosition();
	// 파라미터의 카드가 들어오기 올바른 카드인지 판단
	bool checkTheCardCanGet(card* aCard);
};