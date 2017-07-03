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
	// SDL 초기화
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// 확장 이미지 라이브러리 초기화
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		std::cout << "SDL_image could not initialize! SDL_Error: " << IMG_GetError() << std::endl;
		return false;
	}

	// window 생성
	_window = SDL_CreateWindow("Klondike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (_window == NULL)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// renderer 생성
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (_renderer == NULL)
	{
		std::cout << "Failed to create renderer : " << SDL_GetError() << std::endl;
		return false;
	}
	setupRenderer();

	// 폰트 매니저 생성
	_fm = new fontMngr(20, 700);

	// 이미지 로드
	if (!loadImages()) {
		return false;
	}

	// 끝내기 버튼 위치 설정
	_endButtonPosition.x = 1120;
	_endButtonPosition.y = 700;
	_endButtonPosition.w = 90;
	_endButtonPosition.h = 36;

	////////////////////////////////
	// 카드 덱 초기화
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
	for (int i = 0; i < 7; i++) { // lain 파일에 카드 추가
		for (int j = 0; j <= i; j++) {
			_cardBundles[i]->push(_cardBundles[11]->pop());
		}
	}
	///////////////////////////////

	return true;
}

void Framework::render() {
	// 설정된 색상으로 화면 클리어
	SDL_RenderClear(_renderer);

	///////////////////////////////
	// 렌더링 공간

	// 게임판 출력
	SDL_RenderCopy(_renderer, _gameTable, NULL, NULL);

	for (int i = 0; i < 12; i++) {  // 카드 출력
		_cardBundles[i]->render(_renderer, _cardSprite);
	}

	_fm->render(_renderer, _score); // 점수 출력

	SDL_RenderCopy(_renderer, _endButton, NULL, &_endButtonPosition); // 끝내기 버튼 출력
	// 렌더링 공간 끝
	//////////////////////////////

	// 이 함수가 호출되면 실제 화면 버퍼가 갱신됨
	SDL_RenderPresent(_renderer);
}

void Framework::update()
{
	for (int i = 0; i < 12; i++) {
		_cardBundles[i]->update();
	}

	// 점수 표시를 위한 점수 겟
	_score = _cardBundles[7]->getLength() + _cardBundles[8]->getLength() + _cardBundles[9]->getLength() + _cardBundles[10]->getLength();
	if (_score == 52) { // 52장의 모든 카드를 출력 파일에 넣었을 경우
		_run = false;
		_completeExit = false; // false이므로 새 게임이 시작된다.
	}
	_score *= 5;
}

void Framework::setupRenderer() {
	// renderer 크기를 윈도우 크기와 동일하게 설정
	SDL_RenderSetLogicalSize(_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	// renderer 색상을 흰색으로 설정
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
}

bool Framework::loadImages() {
	SDL_Surface* temp;

	// 게임판 로드
	temp = IMG_Load("resources/gameTable.png");
	if (temp == NULL) {
		std::cout << "Failed to load image : " << IMG_GetError() << std::endl;
		return false;
	}
	_gameTable = SDL_CreateTextureFromSurface(_renderer, temp);
	SDL_FreeSurface(temp);

	// 카드 스프라이트 로드
	temp = IMG_Load("resources/cardSprite.png");
	if (temp == NULL) {
		std::cout << "Failed to load image : " << IMG_GetError() << std::endl;
		return false;
	}
	_cardSprite = SDL_CreateTextureFromSurface(_renderer, temp);

	// 숫자 스프라이트 로드
	temp = IMG_Load("resources/numbers.png");
	if (temp == NULL) {
		std::cout << "Failed to load image : " << IMG_GetError() << std::endl;
		return false;
	}
	_fm->setTexture(SDL_CreateTextureFromSurface(_renderer, temp));

	// 끝내기버튼 로드
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

	// 윈도우 제거
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
		// quit 요청시
		if (e.type == SDL_QUIT) {
			_run = false;
		}
		// 키를 눌렀을때
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
			if (e.type == SDL_MOUSEBUTTONDOWN) {  // 끝내기 버튼을 눌렀는지 확인
				int tempx, tempy;

				SDL_GetMouseState(&tempx, &tempy);
				if (_endButtonPosition.x <= tempx && _endButtonPosition.x + 90 >= tempx && _endButtonPosition.y <= tempy && _endButtonPosition.y + 36 >= tempy) {
					std::cout << "button clicked!" << std::endl;
					_run = false;
					_completeExit = true; // 완전종료
					break;
				}
			}

			// 카드 덱으로 핸들링 넘김
			for (i = 0; i < 12; i++) {
				movedIndex = _cardBundles[i]->keyHandling(e);

				if (movedIndex != -1)
					break;
			}
			
			// 카드 이동 처리
			if (i < 12) {
				card* tempCard = _cardBundles[i]->get(movedIndex);
				for (j = 0; j < 12; j++) {
					if (_cardBundles[j]->checkTheCardCanGet(tempCard)) { // 카드 옮길 수 있을 경우
						if (i < 7) {
							if (j < 7) {   // lain -> lain 파일로 옮길 경우 ( 체인으로 옮길 수 있어야 하므로 )
								int cardCnt = _cardBundles[i]->getLength() - movedIndex;

								stackContainer<card* > tempDeck;
								for (int k = 0; k < cardCnt; k++) { // 체인된 카드 수 만큼 임시 파일에 옮김
									tempDeck.push(_cardBundles[i]->pop());
								}
								for (int k = 0; k < cardCnt; k++) { // 임시 파일의 카드들을 옮겨지는 파일로 옮김
									_cardBundles[j]->push(tempDeck.pop());
								}

								break;
							}
							else {   // lain -> output 파일로 옮길 경우 ( 체인이 안된다 )
								if (_cardBundles[i]->getLength() - 1 == movedIndex) { // 옮기려는 카드가 맨 아래 카드일 경우
									_cardBundles[j]->push(_cardBundles[i]->pop());
								}
							}
						}
						else if (i < 11) {
							_cardBundles[j]->push(_cardBundles[i]->pop());
						}
						else { // 스톡 파일 -> 다른 곳 일때 (openedIndex를 반환해야 한다)
							int cardCnt = _cardBundles[i]->getLength() - movedIndex;

							stackContainer<card* > tempDeck;
							for (int k = 0; k < cardCnt; k++) { // 체인된 카드 수 만큼 임시 파일에 옮김
								tempDeck.push(_cardBundles[i]->pop());
							}
							_cardBundles[j]->push(tempDeck.pop()); // openedIndex 한장만 옮김
							for (int k = 0; k < cardCnt-1; k++) { // 임시 파일의 카드들을 옮겨지는 파일로 옮김
								_cardBundles[i]->push(tempDeck.pop());
							}
						}
					}
				}
			}
		}
	}
}
