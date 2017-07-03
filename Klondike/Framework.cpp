#include "Framework.h"
#include <iostream>

Framework::Framework() {
	_window = NULL;
	_screenSurface = NULL;
	_renderer = NULL;
	_run = true;
	_fm = NULL;
	_score = 0;
	_completeExit = false;
}

bool Framework::init() {
	// SDL �ʱ�ȭ
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Ȯ�� �̹��� ���̺귯�� �ʱ�ȭ
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		std::cout << "SDL_image could not initialize! SDL_Error: " << IMG_GetError() << std::endl;
		return false;
	}

	// window ����
	_window = SDL_CreateWindow("Klondike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (_window == NULL)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// renderer ����
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (_renderer == NULL)
	{
		std::cout << "Failed to create renderer : " << SDL_GetError() << std::endl;
		return false;
	}
	setupRenderer();

	// ��Ʈ �Ŵ��� ����
	_fm = new fontMngr(20, 700);

	// �̹��� �ε�
	if (!loadImages()) {
		return false;
	}

	// ������ ��ư ��ġ ����
	_endButtonPosition.x = 1120;
	_endButtonPosition.y = 700;
	_endButtonPosition.w = 90;
	_endButtonPosition.h = 36;

	////////////////////////////////
	// ī�� �� �ʱ�ȭ
	int lain_x = 90, output_x = 640;
	for (int i = 0; i < 12; i++) {
		if (i < 7) {
			_cardBundles[i] = new cardBundleLain(lain_x, 180);
			lain_x += 90+CARD_WIDTH;
		}
		else if (i < 11) {
			_cardBundles[i] = new cardBundleOutput(output_x, 20);
			output_x += 40+ CARD_WIDTH;
		}
		else
			_cardBundles[i] = new cardBundleStockPile(20, 20);

		_cardBundles[i]->init();
	}
	for (int i = 0; i < 7; i++) { // lain ���Ͽ� ī�� �߰�
		for (int j = 0; j <= i; j++) {
			_cardBundles[i]->push(_cardBundles[11]->pop());
		}
	}
	///////////////////////////////

	return true;
}

void Framework::render() {
	// ������ �������� ȭ�� Ŭ����
	SDL_RenderClear(_renderer);

	///////////////////////////////
	// ������ ����

	// ������ ���
	SDL_RenderCopy(_renderer, _gameTable, NULL, NULL);

	for (int i = 0; i < 12; i++) {  // ī�� ���
		_cardBundles[i]->render(_renderer, _cardSprite);
	}

	_fm->render(_renderer, _score); // ���� ���

	SDL_RenderCopy(_renderer, _endButton, NULL, &_endButtonPosition); // ������ ��ư ���
	// ������ ���� ��
	//////////////////////////////

	// �� �Լ��� ȣ��Ǹ� ���� ȭ�� ���۰� ���ŵ�
	SDL_RenderPresent(_renderer);
}

void Framework::update()
{
	for (int i = 0; i < 12; i++) {
		_cardBundles[i]->update();
	}

	// ���� ǥ�ø� ���� ���� ��
	_score = _cardBundles[7]->getLength() + _cardBundles[8]->getLength() + _cardBundles[9]->getLength() + _cardBundles[10]->getLength();
	if (_score == 52) { // 52���� ��� ī�带 ��� ���Ͽ� �־��� ���
		_run = false;
		_completeExit = false; // false�̹Ƿ� �� ������ ���۵ȴ�.
	}
	_score *= 5;
}

void Framework::setupRenderer() {
	// renderer ũ�⸦ ������ ũ��� �����ϰ� ����
	SDL_RenderSetLogicalSize(_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	// renderer ������ ������� ����
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
}

bool Framework::loadImages() {
	SDL_Surface* temp;

	// ������ �ε�
	temp = IMG_Load("resources/gameTable.png");
	if (temp == NULL) {
		std::cout << "Failed to load image : " << IMG_GetError() << std::endl;
		return false;
	}
	_gameTable = SDL_CreateTextureFromSurface(_renderer, temp);
	SDL_FreeSurface(temp);

	// ī�� ��������Ʈ �ε�
	temp = IMG_Load("resources/cardSprite.png");
	if (temp == NULL) {
		std::cout << "Failed to load image : " << IMG_GetError() << std::endl;
		return false;
	}
	_cardSprite = SDL_CreateTextureFromSurface(_renderer, temp);

	// ���� ��������Ʈ �ε�
	temp = IMG_Load("resources/numbers.png");
	if (temp == NULL) {
		std::cout << "Failed to load image : " << IMG_GetError() << std::endl;
		return false;
	}
	_fm->setTexture(SDL_CreateTextureFromSurface(_renderer, temp));

	// �������ư �ε�
	temp = IMG_Load("resources/endButton.png");
	if (temp == NULL) {
		std::cout << "Failed to load image : " << IMG_GetError() << std::endl;
		return false;
	}
	_endButton = SDL_CreateTextureFromSurface(_renderer, temp);


	SDL_FreeSurface(temp);
	return true;
}

bool Framework::cleanup() {
	for (int i = 0; i < 12; i++) {
		_cardBundles[i]->cleanup();
		delete _cardBundles[i];
		_cardBundles[i] = NULL;
	}

	// ������ ����
	SDL_DestroyWindow(_window);

	//IMG_Quit();
	SDL_Quit();

	return _completeExit;
}

int Framework::run() {
	while (_run) {
		keyHandling();
		update();
		render();
	}

	return _score;
}

void Framework::keyHandling()
{
	SDL_Event e;
	int movedIndex, i, j;

	while (SDL_PollEvent(&e) != 0) {
		// quit ��û��
		if (e.type == SDL_QUIT) {
			_run = false;
		}
		// Ű�� ��������
		else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
				_run = false;
				break;
			default:
				break;
			}

		}
		else {
			if (e.type == SDL_MOUSEBUTTONDOWN) {  // ������ ��ư�� �������� Ȯ��
				int tempx, tempy;

				SDL_GetMouseState(&tempx, &tempy);
				if (_endButtonPosition.x <= tempx && _endButtonPosition.x + 90 >= tempx && _endButtonPosition.y <= tempy && _endButtonPosition.y + 36 >= tempy) {
					std::cout << "button clicked!" << std::endl;
					_run = false;
					_completeExit = true; // ��������
					break;
				}
			}

			// ī�� ������ �ڵ鸵 �ѱ�
			for (i = 0; i < 12; i++) {
				movedIndex = _cardBundles[i]->keyHandling(e);

				if (movedIndex != -1)
					break;
			}
			
			// ī�� �̵� ó��
			if (i < 12) {
				card* tempCard = _cardBundles[i]->get(movedIndex);
				for (j = 0; j < 12; j++) {
					if (_cardBundles[j]->checkTheCardCanGet(tempCard)) { // ī�� �ű� �� ���� ���
						if (i < 7) {
							if (j < 7) {   // lain -> lain ���Ϸ� �ű� ��� ( ü������ �ű� �� �־�� �ϹǷ� )
								int cardCnt = _cardBundles[i]->getLength() - movedIndex;

								stackContainer<card* > tempDeck;
								for (int k = 0; k < cardCnt; k++) { // ü�ε� ī�� �� ��ŭ �ӽ� ���Ͽ� �ű�
									tempDeck.push(_cardBundles[i]->pop());
								}
								for (int k = 0; k < cardCnt; k++) { // �ӽ� ������ ī����� �Ű����� ���Ϸ� �ű�
									_cardBundles[j]->push(tempDeck.pop());
								}

								break;
							}
							else {   // lain -> output ���Ϸ� �ű� ��� ( ü���� �ȵȴ� )
								if (_cardBundles[i]->getLength() - 1 == movedIndex) { // �ű���� ī�尡 �� �Ʒ� ī���� ���
									_cardBundles[j]->push(_cardBundles[i]->pop());
								}
							}
						}
						else if (i < 11) {
							_cardBundles[j]->push(_cardBundles[i]->pop());
						}
						else { // ���� ���� -> �ٸ� �� �϶� (openedIndex�� ��ȯ�ؾ� �Ѵ�)
							int cardCnt = _cardBundles[i]->getLength() - movedIndex;

							stackContainer<card* > tempDeck;
							for (int k = 0; k < cardCnt; k++) { // ü�ε� ī�� �� ��ŭ �ӽ� ���Ͽ� �ű�
								tempDeck.push(_cardBundles[i]->pop());
							}
							_cardBundles[j]->push(tempDeck.pop()); // openedIndex ���常 �ű�
							for (int k = 0; k < cardCnt-1; k++) { // �ӽ� ������ ī����� �Ű����� ���Ϸ� �ű�
								_cardBundles[i]->push(tempDeck.pop());
							}
						}
					}
				}
			}
		}
	}
}
