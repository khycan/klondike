#pragma once

#include "cardBundle.h"

// ���������� ī�� ��
class cardBundleLain : public cardBundle {
private:
	static const int _GAP = 20; // �������� ī�� ���� ��
	int _x, _y;
	int _chainedIndex; // ü�ε� ī���� ���� �ε���
	bool _isClicked;
	bool _cardChained; // �̰� ������ �ش� ī�� ������ �� �����δ�

	int _mouseX, _mouseY;

public :
	cardBundleLain(int x, int y);

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