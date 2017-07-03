#pragma once

#include "cardBundle.h"

// 포개어지는 카드 덱
class cardBundleLain : public cardBundle {
private:
	static const int _GAP = 20; // 포개어진 카드 사이 갭
	int _x, _y;
	int _chainedIndex; // 체인된 카드의 시작 인덱스
	bool _isClicked;
	bool _cardChained; // 이게 켜지면 해당 카드 밑으로 다 움직인다

	int _mouseX, _mouseY;

public :
	cardBundleLain(int x, int y);

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