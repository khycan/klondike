#include "card.h"

card::card(char cardInfo) {
	_cardInfo = cardInfo;
	_isDrawing = false;

	_spritePosition.x = getNumber() * CARD_WIDTH;
	_spritePosition.y = getShape() * CARD_HEIGHT;
	_spritePosition.w = CARD_WIDTH;
	_spritePosition.h = CARD_HEIGHT;

	_backSideCard.x = 0;
	_backSideCard.y = 4 * CARD_HEIGHT;
	_backSideCard.w = CARD_WIDTH;
	_backSideCard.h = CARD_HEIGHT;

	_position.w = CARD_WIDTH;
	_position.h = CARD_HEIGHT;
}

int card::getCarInfo() {
	return _cardInfo;
}

int card::getShape() {
	return (int)((_cardInfo & 0xF0) >> 4);
}

int card::getNumber() {
	return (int)(_cardInfo & 0x0F);
}

SDL_Rect card::getPosition() {
	return _position;
}

void card::setPosition(int x, int y) {
	_position.x = x;
	_position.y = y;
}

void card::movePosition(int dx, int dy) {
	_position.x += dx;
	_position.y += dy;
}

void card::enableDrawMode() {
	_isDrawing = true;
}

void card::disableDrawMode() {
	_isDrawing = false;
}

void card::render(SDL_Renderer* renderer, SDL_Texture* cardSprite) {
	if (_isDrawing)
		SDL_RenderCopy(renderer, cardSprite, &_spritePosition, &_position);
	else
		SDL_RenderCopy(renderer, cardSprite, &_backSideCard, &_position);
}