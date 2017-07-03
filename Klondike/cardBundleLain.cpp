#include "cardBundleLain.h"

#include <iostream>

cardBundleLain::cardBundleLain(int x, int y)
{
	_x = x;
	_y = y;
	_isClicked = false;
	_cardChained = false;
}

void cardBundleLain::init()
{
	_cards = new stackContainer<card*>();
}

int cardBundleLain::keyHandling(SDL_Event e)
{
	int tempx, tempy;
	
	if (e.type == SDL_MOUSEBUTTONDOWN) { // 마우스 눌려있을때
		SDL_GetMouseState(&tempx, &tempy);
		if (_x <= tempx && _x + CARD_WIDTH >= tempx && _y <= tempy) { // 현재 카드덱의 y좌표보다 크고 카드너비 사이에 있을때
			_isClicked = true;
			_mouseX = tempx;
			_mouseY = tempy;

			std::cout << "card clicked!" << std::endl;
		}

		for (int i = 0; i < _cards->getLength(); i++) { // 이벤트가 발생하는 카드 찾기
			SDL_Rect p = _cards->get(i)->getPosition();

			if (i == _cards->getLength() - 1 && p.x <= tempx && p.x + CARD_WIDTH >= tempx && p.y <= tempy && p.y + CARD_HEIGHT >= tempy) {  // 맨 밑 카드일때
				_cardChained = true;
				_chainedIndex = i;

				std::cout << "mouse 좌표 : " << "(" << tempx << ", " << tempy << ")" << std::endl;
				std::cout << "카드 좌표 : " << "(" << p.x << ", " << p.y << ")" << std::endl;
				break;
			}
			else if (p.x <= tempx && p.x + CARD_WIDTH >= tempx && p.y <= tempy && p.y + _GAP >= tempy && _cards->get(i)->isDrawingMode()) {  // 중간 겹쳐진 카드일때
				_cardChained = true;
				_chainedIndex = i;

				std::cout << "mouse 좌표 : " << "(" << tempx << ", " << tempy << ")" << std::endl;
				std::cout << "카드 좌표 : " << "(" << p.x << ", " << p.y << ")" << std::endl;
				break;
			}
		}
		
	}
	else if (e.type == SDL_MOUSEMOTION) { // 마우스를 움직이고 있을때
		SDL_GetMouseState(&tempx, &tempy);
		if (_isClicked) { // 클릭상태일때
			if (_cardChained) {
				int dx = tempx - _mouseX;
				int dy = tempy - _mouseY;

				for (int i = _chainedIndex; i < _cards->getLength(); i++) { // 카드 옮기기
					_cards->get(i)->movePosition(dx, dy);
				}

				_mouseX = tempx;
				_mouseY = tempy;
			}
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP) { // 마우스를 뗐을때
		if (_isClicked && _cardChained) {
			_isClicked = false;
			_cardChained = false;

			// 다른 곳으로 옮길 카드 인덱스 반환
			return _chainedIndex;
		}

		_isClicked = false;
		_cardChained = false;
	}

	// 없으면 -1 반환
	return -1;
}

void cardBundleLain::render(SDL_Renderer * renderer, SDL_Texture * cardSprite)
{
	for (int i = 0; i < _cards->getLength(); i++) {
		_cards->get(i)->render(renderer, cardSprite);
	}
}

void cardBundleLain::update()
{
	// 카드 정렬
	alignCardPosition();
}

void cardBundleLain::alignCardPosition()
{
	int x = _x, y = _y;

	if (!_isClicked) {
		for (int i = 0; i < _cards->getLength(); i++) {
			_cards->get(i)->setPosition(x, y);

			// 카드 간격만큼 더해준다
			y += _GAP;

			if (i == _cards->getLength() - 1) { // 마지막 카드는 앞면으로
				_cards->get(i)->enableDrawMode();
			}
		}
	}
}

bool cardBundleLain::checkTheCardCanGet(card * aCard)
{
	SDL_Rect p = aCard->getPosition();

	if (_y <= p.y && _x - CARD_WIDTH / 2 <= p.x && _x + CARD_WIDTH >= p.x) { // 충돌 검사
		if (_cards->getLength() != 0) { // 비어있지 않을때
			card* temp = _cards->get(_cards->getLength() - 1);

			int lastInfo = temp->getCarInfo();
			int newInfo = aCard->getCarInfo();

			if ((lastInfo & 0xFFFFFF0F) == 0) { // 마지막 카드가 A이면
				return false;
			}
			else { // A이상의 카드일때
				if (((lastInfo & 0xFFFFFF0F) - 1) == (newInfo & 0xFFFFFF0F)) { // 새 카드가 마지막카드보다 숫자가 1 작을때
					if (((lastInfo & 0xFFFFFFF0) == 0x00 || (lastInfo & 0xFFFFFFF0) == 0x30) && ((newInfo & 0xFFFFFFF0) == 0x10 || (newInfo & 0xFFFFFFF0) == 0x20)) // 마지막 카드가 검정색이고 새 카드가 빨간색일때
						return true;
					else if (((lastInfo & 0xFFFFFFF0) == 0x10 || (lastInfo & 0xFFFFFFF0) == 0x20) && ((newInfo & 0xFFFFFFF0) == 0x00 || (newInfo & 0xFFFFFFF0) == 0x30)) // 마지막 카드가 빨간색이고 새 카드가 검정색일때
						return true;
				}
			}
		}
		else { // 카드가 비어있을때
			int newInfo = aCard->getCarInfo();

			if ((newInfo & 0xFFFFFF0F) == 0xC)  // 새 카드가 K일때
				return true;
		}
	}

	return false;
}

void cardBundleLain::cleanup()
{
	delete _cards;
}

