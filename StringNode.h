#pragma once
#include<string>

class StringNode
{
public:

	std::string data;
	StringNode *next;

	StringNode() {
		data = "";
		next = NULL;
	}

	StringNode(std::string data, StringNode *next) {
		this->data = data;
		this->next = next;
	}

	StringNode(std::string data) {
		this->data = data;
		this->next = NULL;
	}

	StringNode* getNext() {
		return next;
	}
	std::string getData() {
		return data;
	}
	std::string setData(std::string newData) {
		data = newData;
	}
	std::string setNext(StringNode *str) {
		next = str;
	}

};

