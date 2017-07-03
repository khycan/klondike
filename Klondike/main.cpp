#include "Framework.h"
#include <Windows.h>
#include <string>

int main(int argc, char* argv[]) {
	int money = -52; // 52�޷��� ���� �����ϹǷ�

	while (true) {
		Framework* klondike = new Framework();

		klondike->init();

		money += klondike->run();

		if (klondike->cleanup()) { // ���� �����϶�
			break;
		}
	}

	std::string ret_ = std::to_string(money);
	std::wstring ret = std::wstring(ret_.begin(), ret_.end());
	ret += L"�޷��� ȹ���ϼ̽��ϴ�.";

	MessageBox(NULL, ret.c_str(), L"���� ���", MB_OK);

	return 0;
}