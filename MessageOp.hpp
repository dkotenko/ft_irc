#include <string>
#include <vector>
#include "utils.hpp"

class MessageInput
{
	private:
		std::string						prefix;
		std::string						command;
		std::vector<std::string>		parameters;

        MessageInput();
        MessageInput(const Message& copy);
        MessageInput &operator=(const Message& other);
	public:
        MessageInput(const std::string &str);
		virtual ~MessageInput();
		const std::string				&getPrefix() const;
		const std::string				&getCmd() const;
		const std::vector<std::string>	&getParams() const;
}