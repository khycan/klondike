#include "Framework.h"
#include <Windows.h>
#include <string>

int main(int argc, char* argv[]) {
	int money = -52; // 52달러를 내고 시작하므로

	while (true) {
		Framework* klondike = new Framework();

		klondike->init();

		money += klondike->run();

		if (klondike->cleanup()) { // 완전 종료일때
			break;
		}
	}

	std::string ret_ = std::to_string(money);
	std::wstring ret = std::wstring(ret_.begin(), ret_.end());
	ret += L"달러를 획득하셨습니다.";

	MessageBox(NULL, ret.c_str(), L"게임 결과", MB_OK);

	return 0;
}