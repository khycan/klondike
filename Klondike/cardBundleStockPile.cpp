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
	// ����
	shuffle(shuffleArray);
	
	// ���δ�� ī�� ���� �� ����
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

		// ī�� ����
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

	if (e.type == SDL_MOUSEBUTTONDOWN) { // ���콺 ����������
		SDL_GetMouseState(&tempx, &tempy);

		if (_xn <= tempx && _xn + CARD_WIDTH >= tempx && _yn <= tempy && _yn + CARD_HEIGHT >= tempy) { // ���µ� ī�嵦 Ŭ����
			_isClicked = true;
			_mouseX = tempx;
			_mouseY = tempy;

			std::cout << "card clicked!" << std::endl;
		}
		else if (_x <= tempx && _x + CARD_WIDTH >= tempx && _y <= tempy && _y + CARD_HEIGHT >= tempy) { // ����� ī�嵦 Ŭ����
			_openedIndex--;
			if (_openedIndex < 0) { // ����� ī�嵦�� ������� ��
				// ���� ��� ī�� �޸� ���·�
				for (int i = 0; i < _cards->getLength(); i++) {
					_cards->get(i)->disableDrawMode();
				}
				_openedIndex = _cards->getLength();
			}
			else { // ������� ���� ��
				_cards->get(_openedIndex)->enableDrawMode();
			}
		}

	}
	else if (e.type == SDL_MOUSEMOTION) { // ���콺�� �����̰� ������
		SDL_GetMouseState(&tempx, &tempy);
		if (_isClicked) { // Ŭ�������϶�
			int dx = tempx - _mouseX;
			int dy = tempy - _mouseY;

			// ���� �ű� �� �ִ� ī��� ���µ� ī���� �� ��ī���̹Ƿ�
			_cards->get(_openedIndex)->movePosition(dx, dy);

			_mouseX = tempx;
			_mouseY = tempy;
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP) { // ���콺�� ������
		if (_isClicked) {
			_isClicked = false;

			return _openedIndex;
		}
		_isClicked = false;
	}

	// ������ -1 ��ȯ
	return -1;
}

void cardBundleStockPile::render(SDL_Renderer * renderer, SDL_Texture * cardSprite)
{
	// �������� ī��
	for (int i = 0; i < _openedIndex; i++) {
		_cards->get(i)->render(renderer, cardSprite);
	}
	
	// ���µ� ī��
	for (int i = _cards->getLength() -1 ; i >= _openedIndex; i--) {
		_cards->get(i)->render(renderer, cardSprite);
	}
}

void cardBundleStockPile::update()
{
	// ī�� ����
	alignCardPosition();

	if (_cards->getLength() == 0) { // ī�尡 ������� ��

	}
}

void cardBundleStockPile::alignCardPosition()
{
	if (_openedIndex > getLength())
		_openedIndex = getLength();

	if (!_isClicked) {
		// �������� ī��
		for (int i = 0; i < _openedIndex; i++) {
			_cards->get(i)->setPosition(_x, _y);
		}

		// ���µ� ī��
		for (int i = _cards->getLength() - 1; i >= _openedIndex; i--) {
			_cards->get(i)->setPosition(_xn, _yn);
		}
	}
}

bool cardBundleStockPile::checkTheCardCanGet(card * aCard)
{
	return false;
}
