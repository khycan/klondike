#include "fontMngr.h"
#include <Windows.h>

fontMngr::fontMngr(int x, int y)
{
	_position.x = x;
	_position.y = y;
	_position.w = FONT_SIZE;
	_position.h = FONT_SIZE;
}

void fontMngr::init()
{
}

void fontMngr::render(SDL_Renderer * renderer, int score)
{
	char scoreStr[10];

	// 값 초기화
	memset(scoreStr, NULL, 10);

	// 점수 문자열로 변경
	itoa(score, scoreStr, 10);

	// 문자열 시작 위치 저장
	int originIndex = _position.x;

	// 그리기
	for (char* i = scoreStr; *i != NULL; i++) {
		SDL_Rect spritePosition;
		spritePosition.x = (*i - 48) * 36;
		spritePosition.y = 0;
		spritePosition.w = 36;
		spritePosition.h = 36;

		SDL_RenderCopy(renderer, _numSprite, &spritePosition, &_position);
		_position.x += 42;
	}
	_position.x = originIndex;
}
