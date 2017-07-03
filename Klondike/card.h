#pragma once

#include <SDL.h>

static const int CARD_WIDTH = 79;
static const int CARD_HEIGHT = 123;

class card {
private:
	SDL_Rect _spritePosition; // �ո��϶� ��������Ʈ ��ǥ
	SDL_Rect _backSideCard; // �޸��϶� ��������Ʈ ��ǥ
	SDL_Rect _position; // ȭ�� ��ǥ

	// 0x?? �� ǥ��
	// 0x?0 : ?�� 0~3���� Ŭ�ι�, ���̾�, ��Ʈ, �����̵� ��
	// 0x0? : ?�� 0~C���� 1 ~ K ��
	// ex) 0x2C�� ���̾�K
	char _cardInfo;
	bool _isDrawing;

public:
	card(char cardInfo);

	int getCarInfo();
	int getShape();
	int getNumber();
	SDL_Rect getPosition();

	void setPosition(int x, int y);
	void movePosition(int dx, int dy);

	void enableDrawMode();
	void disableDrawMode();
	bool isDrawingMode() { return _isDrawing; }
	
	void render(SDL_Renderer* renderer, SDL_Texture* cardSprite);
};