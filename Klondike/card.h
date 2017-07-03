#pragma once

#include <SDL.h>

static const int CARD_WIDTH = 79;
static const int CARD_HEIGHT = 123;

class card {
private:
	SDL_Rect _spritePosition; // 앞면일때 스프라이트 좌표
	SDL_Rect _backSideCard; // 뒷면일때 스프라이트 좌표
	SDL_Rect _position; // 화면 좌표

	// 0x?? 로 표시
	// 0x?0 : ?는 0~3으로 클로버, 다이아, 하트, 스페이드 순
	// 0x0? : ?는 0~C으로 1 ~ K 순
	// ex) 0x2C는 다이아K
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