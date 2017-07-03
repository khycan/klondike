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

	// ī�� ������ �޼ҵ�
	void alignCardPosition();
	// �Ķ������ ī�尡 ������ �ùٸ� ī������ �Ǵ�
	bool checkTheCardCanGet(card* aCard);
};