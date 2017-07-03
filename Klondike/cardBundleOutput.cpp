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

	if (e.type == SDL_MOUSEBUTTONDOWN) { // ���콺 ����������
		SDL_GetMouseState(&tempx, &tempy);

		if (_x <= tempx && _x + CARD_WIDTH >= tempx && _y <= tempy && _y + CARD_HEIGHT >= tempy && _cards->getLength() > 0) { // ī�嵦 Ŭ����
			_isClicked = true;
			_mouseX = tempx;
			_mouseY = tempy;

			std::cout << "card clicked!" << std::endl;
		}
	}
	else if (e.type == SDL_MOUSEMOTION) { // ���콺�� �����̰� ������
		SDL_GetMouseState(&tempx, &tempy);
		if (_isClicked) { // Ŭ�������϶�
			int dx = tempx - _mouseX;
			int dy = tempy - _mouseY;

			// ���� �ű� �� �ִ� ī��� �� ��ī���̹Ƿ�
			_cards->get(_cards->getLength() - 1)->movePosition(dx, dy);

			_mouseX = tempx;
			_mouseY = tempy;
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP) { // ���콺�� ������
		if (_isClicked) {
			_isClicked = false;

			return _cards->getLength() - 1;
		}
	}

	// ������ -1 ��ȯ
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
	// ī�� ����
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

	if (_y - CARD_HEIGHT/3 <= p.y && _y + CARD_HEIGHT >= p.y && _x - CARD_WIDTH / 2 <= p.x && _x + CARD_WIDTH >= p.x) { // �浹 �˻�
		if (_cards->getLength() != 0) { // ������� ������
			card* temp = _cards->get(_cards->getLength() - 1);

			int lastInfo = temp->getCarInfo();
			int newInfo = aCard->getCarInfo();

			if ((lastInfo & 0xFFFFFF0F) == 0xC) { // ������ ī�尡 K�̸�
				return false;
			}
			else { // K������ ī���϶�
				if (((lastInfo & 0xFFFFFF0F) + 1) == (newInfo & 0xFFFFFF0F)) { // �� ī�尡 ������ī�庸�� ���ڰ� 1 Ŭ��
					if (((lastInfo & 0xFFFFFFF0) == (newInfo & 0xFFFFFFF0))) // ������ ī��� �� ī�尡 ���� �����϶�
						return true;
				}
			}
		}
		else { // ī�尡 ���������
			int newInfo = aCard->getCarInfo();

			if ((newInfo & 0xFFFFFF0F) == 0)  // �� ī�尡 A�϶�
				return true;
		}
	}

	return false;
}
