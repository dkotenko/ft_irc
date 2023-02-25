//
// Created by Redwyn Poetess on 03.09.2022.
//

#include "InputMessage.hpp"

std::vector<std::string> &InputMessage::getParams() {
	return params;
}

int InputMessage::getParamsSize() {
	return params.size();
}