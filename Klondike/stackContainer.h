#pragma once

#include <iostream>

static const int MAX_STACK_SIZE = 1024;

// tail방향으로 자라는 더블 링크드 리스트 스택
template <typename T>
class stackContainer {
	typedef struct _containerNode {
		struct _containerNode* previous;
		T value;
		struct _containerNode* next;
	}containerNode;
private:
	containerNode* head;
	containerNode* tail;
	int length;

public:
	stackContainer() : head(NULL), tail(NULL), length(0)
	{
	}

	~stackContainer() {
		containerNode* temp = head;

		// 모든 메모리 해제
		while (temp != NULL) {
			containerNode* oldtemp = temp;
			temp = temp->next;
			
			delete oldtemp->value;
			delete oldtemp;
		}
	}

	void push(T newValue) {
		containerNode* newNode = new containerNode();
		if (newNode == NULL) {
			std::cout << "allocating memory failed!" << std::endl;
			return;
		}
		newNode->value = newValue;
		newNode->previous = NULL;
		newNode->next = NULL;

		if (tail == NULL) { // 빈 스택
			tail = head = newNode;
		}
		else {
			tail->next = newNode;
			newNode->previous = tail;
			tail = newNode;
		}

		length++;
	}

	T pop() {
		T retValue;

		if (tail == NULL) { // 빈 스택
			std::cout << "stack is empty!" << std::endl;
			return NULL;
		}

		retValue = tail->value;

		if (head == tail) { // 하나 남음
			delete tail;
			tail = head = NULL;
		}
		else {
			tail = tail->previous;
			delete tail->next;
			tail->next = NULL;
		}

		length--;
		return retValue;
	}

	T get(int n) {
		containerNode* temp;
		if (n >= length / 2) {
			temp = tail;

			for (int i = 0; i < length - 1 - n; i++)
				temp = temp->previous;
		}
		else {
			temp = head;

			for (int i = 0; i < n; i++)
				temp = temp->next;
		}

		return temp->value;
	}

	containerNode* getStackHead() {
		return head;
	}

	const int getLength() {
		return length;
	}
};