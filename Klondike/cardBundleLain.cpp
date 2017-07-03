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
	
	if (e.type == SDL_MOUSEBUTTONDOWN) { // ���콺 ����������
		SDL_GetMouseState(&tempx, &tempy);
		if (_x <= tempx && _x + CARD_WIDTH >= tempx && _y <= tempy) { // ���� ī�嵦�� y��ǥ���� ũ�� ī��ʺ� ���̿� ������
			_isClicked = true;
			_mouseX = tempx;
			_mouseY = tempy;

			std::cout << "card clicked!" << std::endl;
		}

		for (int i = 0; i < _cards->getLength(); i++) { // �̺�Ʈ�� �߻��ϴ� ī�� ã��
			SDL_Rect p = _cards->get(i)->getPosition();

			if (i == _cards->getLength() - 1 && p.x <= tempx && p.x + CARD_WIDTH >= tempx && p.y <= tempy && p.y + CARD_HEIGHT >= tempy) {  // �� �� ī���϶�
				_cardChained = true;
				_chainedIndex = i;

				std::cout << "mouse ��ǥ : " << "(" << tempx << ", " << tempy << ")" << std::endl;
				std::cout << "ī�� ��ǥ : " << "(" << p.x << ", " << p.y << ")" << std::endl;
				break;
			}
			else if (p.x <= tempx && p.x + CARD_WIDTH >= tempx && p.y <= tempy && p.y + _GAP >= tempy && _cards->get(i)->isDrawingMode()) {  // �߰� ������ ī���϶�
				_cardChained = true;
				_chainedIndex = i;

				std::cout << "mouse ��ǥ : " << "(" << tempx << ", " << tempy << ")" << std::endl;
				std::cout << "ī�� ��ǥ : " << "(" << p.x << ", " << p.y << ")" << std::endl;
				break;
			}
		}
		
	}
	else if (e.type == SDL_MOUSEMOTION) { // ���콺�� �����̰� ������
		SDL_GetMouseState(&tempx, &tempy);
		if (_isClicked) { // Ŭ�������϶�
			if (_cardChained) {
				int dx = tempx - _mouseX;
				int dy = tempy - _mouseY;

				for (int i = _chainedIndex; i < _cards->getLength(); i++) { // ī�� �ű��
					_cards->get(i)->movePosition(dx, dy);
				}

				_mouseX = tempx;
				_mouseY = tempy;
			}
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP) { // ���콺�� ������
		if (_isClicked && _cardChained) {
			_isClicked = false;
			_cardChained = false;

			// �ٸ� ������ �ű� ī�� �ε��� ��ȯ
			return _chainedIndex;
		}

		_isClicked = false;
		_cardChained = false;
	}

	// ������ -1 ��ȯ
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
	// ī�� ����
	alignCardPosition();
}

void cardBundleLain::alignCardPosition()
{
	int x = _x, y = _y;

	if (!_isClicked) {
		for (int i = 0; i < _cards->getLength(); i++) {
			_cards->get(i)->setPosition(x, y);

			// ī�� ���ݸ�ŭ �����ش�
			y += _GAP;

			if (i == _cards->getLength() - 1) { // ������ ī��� �ո�����
				_cards->get(i)->enableDrawMode();
			}
		}
	}
}

bool cardBundleLain::checkTheCardCanGet(card * aCard)
{
	SDL_Rect p = aCard->getPosition();

	if (_y <= p.y && _x - CARD_WIDTH / 2 <= p.x && _x + CARD_WIDTH >= p.x) { // �浹 �˻�
		if (_cards->getLength() != 0) { // ������� ������
			card* temp = _cards->get(_cards->getLength() - 1);

			int lastInfo = temp->getCarInfo();
			int newInfo = aCard->getCarInfo();

			if ((lastInfo & 0xFFFFFF0F) == 0) { // ������ ī�尡 A�̸�
				return false;
			}
			else { // A�̻��� ī���϶�
				if (((lastInfo & 0xFFFFFF0F) - 1) == (newInfo & 0xFFFFFF0F)) { // �� ī�尡 ������ī�庸�� ���ڰ� 1 ������
					if (((lastInfo & 0xFFFFFFF0) == 0x00 || (lastInfo & 0xFFFFFFF0) == 0x30) && ((newInfo & 0xFFFFFFF0) == 0x10 || (newInfo & 0xFFFFFFF0) == 0x20)) // ������ ī�尡 �������̰� �� ī�尡 �������϶�
						return true;
					else if (((lastInfo & 0xFFFFFFF0) == 0x10 || (lastInfo & 0xFFFFFFF0) == 0x20) && ((newInfo & 0xFFFFFFF0) == 0x00 || (newInfo & 0xFFFFFFF0) == 0x30)) // ������ ī�尡 �������̰� �� ī�尡 �������϶�
						return true;
				}
			}
		}
		else { // ī�尡 ���������
			int newInfo = aCard->getCarInfo();

			if ((newInfo & 0xFFFFFF0F) == 0xC)  // �� ī�尡 K�϶�
				return true;
		}
	}

	return false;
}

void cardBundleLain::cleanup()
{
	delete _cards;
}

