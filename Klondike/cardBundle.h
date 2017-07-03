#pragma once

#include "stackContainer.h"
#include "card.h"

class cardBundle {
protected:
	stackContainer<card* >* _cards;
public:
	cardBundle() :_cards(NULL) {}

	virtual void init() = 0;
	virtual int keyHandling(SDL_Event e) = 0;
	virtual void render(SDL_Renderer* renderer, SDL_Texture* cardSprite) = 0;
	virtual void update() = 0;
	virtual void cleanup() = 0;

	virtual void alignCardPosition() = 0;

	void push(card* p) { _cards->push(p); }
	virtual card* pop() { return _cards->pop(); } // 오버라이딩을 위해
	card* get(int i) { return _cards->get(i); }
	int getLength() { return _cards->getLength(); }

	virtual bool checkTheCardCanGet(card* aCard) = 0;
};