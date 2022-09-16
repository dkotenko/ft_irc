#include "utils.hpp"

int xassert(int value, const std::string& err_message)
{
	if (!value) {
		std::cout << err_message << ". errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	return !value ? 1 : 0;
}

void sigh(int n) {
	(void)n;
    exit(0);
}

void printError(std::string s) {
    std::cout << s << ". errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
}

