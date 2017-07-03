#pragma once

#include "cardBundle.h"

class cardBundleOutput : public cardBundle {
private:
	int _x, _y;
	bool _isClicked;

	int _mouseX, _mouseY;

public:
	cardBundleOutput(int x, int y);

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