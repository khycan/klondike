#include "cardBundleOutput.h"

cardBundleOutput::cardBundleOutput(int x, int y)
{
	_x = x;
	_y = y;
	_isClicked = false;
}

void cardBundleOutput::init()
{
	_cards = new stackContainer<card*>();
}

void cardBundleOutput::cleanup()
{
}

int cardBundleOutput::keyHandling(SDL_Event e)
{
	int tempx, tempy;

	if (e.type == SDL_MOUSEBUTTONDOWN) { // 마우스 눌려있을때
		SDL_GetMouseState(&tempx, &tempy);

		if (_x <= tempx && _x + CARD_WIDTH >= tempx && _y <= tempy && _y + CARD_HEIGHT >= tempy && _cards->getLength() > 0) { // 카드덱 클릭시
			_isClicked = true;
			_mouseX = tempx;
			_mouseY = tempy;

			std::cout << "card clicked!" << std::endl;
		}
	}
	else if (e.type == SDL_MOUSEMOTION) { // 마우스를 움직이고 있을때
		SDL_GetMouseState(&tempx, &tempy);
		if (_isClicked) { // 클릭상태일때
			int dx = tempx - _mouseX;
			int dy = tempy - _mouseY;

			// 내가 옮길 수 있는 카드는 맨 위카드이므로
			_cards->get(_cards->getLength() - 1)->movePosition(dx, dy);

			_mouseX = tempx;
			_mouseY = tempy;
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP) { // 마우스를 뗐을때
		if (_isClicked) {
			_isClicked = false;

			return _cards->getLength() - 1;
		}
	}

	// 없으면 -1 반환
	return -1;
}

void cardBundleOutput::render(SDL_Renderer * renderer, SDL_Texture * cardSprite)
{
	for (int i = 0; i < _cards->getLength(); i++) {
		_cards->get(i)->render(renderer, cardSprite);
	}
}

void cardBundleOutput::update()
{
	// 카드 정렬
	alignCardPosition();
}

void cardBundleOutput::alignCardPosition()
{
	if (!_isClicked) {
		for (int i = 0; i < _cards->getLength(); i++) {
			_cards->get(i)->setPosition(_x, _y);
		}
	}
}

bool cardBundleOutput::checkTheCardCanGet(card * aCard)
{
	SDL_Rect p = aCard->getPosition();

	if (_y - CARD_HEIGHT/3 <= p.y && _y + CARD_HEIGHT >= p.y && _x - CARD_WIDTH / 2 <= p.x && _x + CARD_WIDTH >= p.x) { // 충돌 검사
		if (_cards->getLength() != 0) { // 비어있지 않을때
			card* temp = _cards->get(_cards->getLength() - 1);

			int lastInfo = temp->getCarInfo();
			int newInfo = aCard->getCarInfo();

			if ((lastInfo & 0xFFFFFF0F) == 0xC) { // 마지막 카드가 K이면
				return false;
			}
			else { // K이하의 카드일때
				if (((lastInfo & 0xFFFFFF0F) + 1) == (newInfo & 0xFFFFFF0F)) { // 새 카드가 마지막카드보다 숫자가 1 클때
					if (((lastInfo & 0xFFFFFFF0) == (newInfo & 0xFFFFFFF0))) // 마지막 카드와 새 카드가 같은 문양일때
						return true;
				}
			}
		}
		else { // 카드가 비어있을때
			int newInfo = aCard->getCarInfo();

			if ((newInfo & 0xFFFFFF0F) == 0)  // 새 카드가 A일때
				return true;
		}
	}

	return false;
}
