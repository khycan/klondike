#pragma once

#include "cardBundle.h"

class cardBundleStockPile : public cardBundle {
private:
	static const int _GAP = 10; // �������� ī�� ������ ����

	int _x, _y;
	int _xn, _yn; // �������� ī�嵦 ��ġ
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

	// ī�� ������ �޼ҵ�
	void alignCardPosition();
	// �Ķ������ ī�尡 ������ �ùٸ� ī������ �Ǵ�
	bool checkTheCardCanGet(card* aCard);
};