#include <ctime>
#include "cardBundleStockPile.h"

cardBundleStockPile::cardBundleStockPile(int x, int y)
{
	_x = x;
	_y = y;
	_xn = _x + CARD_WIDTH + _GAP;
	_yn = _y;
	_openedIndex = 0;
	_isClicked = false;
}

void cardBundleStockPile::init()
{
	_cards = new stackContainer<card*>();

	int shuffleArray[52], n=0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			shuffleArray[n++] = i * 16 + j;
		}
	}
	// 셔플
	shuffle(shuffleArray);
	
	// 섞인대로 카드 생성 후 넣음
	for (int i = 0; i < 52; i++) {
		_cards->push(new card(shuffleArray[i]));
	}

	_openedIndex = _cards->getLength();
}

void cardBundleStockPile::shuffle(int * cards)
{
	srand((unsigned int)time(NULL));
	int randNumber = rand() % 3000, temp, temp2;
	for (int i = 0; i < randNumber; i++) {
		temp2 = rand() % 52;

		// 카드 셔플
		temp = cards[temp2];
		cards[temp2] = cards[1];
		cards[1] = temp;

		temp = cards[0];
		cards[0] = cards[8];
		cards[8] = temp;

		temp = cards[51];
		cards[51] = cards[30];
		cards[30] = temp;

		temp = cards[17];
		cards[17] = cards[41];
		cards[41] = temp;
	}
}

void cardBundleStockPile::cleanup()
{
	delete _cards;
}

int cardBundleStockPile::keyHandling(SDL_Event e)
{
	int tempx, tempy;

	if (e.type == SDL_MOUSEBUTTONDOWN) { // 마우스 눌려있을때
		SDL_GetMouseState(&tempx, &tempy);

		if (_xn <= tempx && _xn + CARD_WIDTH >= tempx && _yn <= tempy && _yn + CARD_HEIGHT >= tempy) { // 오픈된 카드덱 클릭시
			_isClicked = true;
			_mouseX = tempx;
			_mouseY = tempy;

			std::cout << "card clicked!" << std::endl;
		}
		else if (_x <= tempx && _x + CARD_WIDTH >= tempx && _y <= tempy && _y + CARD_HEIGHT >= tempy) { // 비오픈 카드덱 클릭시
			_openedIndex--;
			if (_openedIndex < 0) { // 비오픈 카드덱이 비어있을 시
				// 덱의 모든 카드 뒷면 상태로
				for (int i = 0; i < _cards->getLength(); i++) {
					_cards->get(i)->disableDrawMode();
				}
				_openedIndex = _cards->getLength();
			}
			else { // 비어있지 않을 때
				_cards->get(_openedIndex)->enableDrawMode();
			}
		}

	}
	else if (e.type == SDL_MOUSEMOTION) { // 마우스를 움직이고 있을때
		SDL_GetMouseState(&tempx, &tempy);
		if (_isClicked) { // 클릭상태일때
			int dx = tempx - _mouseX;
			int dy = tempy - _mouseY;

			// 내가 옮길 수 있는 카드는 오픈된 카드의 맨 위카드이므로
			_cards->get(_openedIndex)->movePosition(dx, dy);

			_mouseX = tempx;
			_mouseY = tempy;
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP) { // 마우스를 뗐을때
		if (_isClicked) {
			_isClicked = false;

			return _openedIndex;
		}
		_isClicked = false;
	}

	// 없으면 -1 반환
	return -1;
}

void cardBundleStockPile::render(SDL_Renderer * renderer, SDL_Texture * cardSprite)
{
	// 뒤집어진 카드
	for (int i = 0; i < _openedIndex; i++) {
		_cards->get(i)->render(renderer, cardSprite);
	}
	
	// 오픈된 카드
	for (int i = _cards->getLength() -1 ; i >= _openedIndex; i--) {
		_cards->get(i)->render(renderer, cardSprite);
	}
}

void cardBundleStockPile::update()
{
	// 카드 정렬
	alignCardPosition();

	if (_cards->getLength() == 0) { // 카드가 비어있을 시

	}
}

void cardBundleStockPile::alignCardPosition()
{
	if (_openedIndex > getLength())
		_openedIndex = getLength();

	if (!_isClicked) {
		// 뒤집어진 카드
		for (int i = 0; i < _openedIndex; i++) {
			_cards->get(i)->setPosition(_x, _y);
		}

		// 오픈된 카드
		for (int i = _cards->getLength() - 1; i >= _openedIndex; i--) {
			_cards->get(i)->setPosition(_xn, _yn);
		}
	}
}

bool cardBundleStockPile::checkTheCardCanGet(card * aCard)
{
	return false;
}
