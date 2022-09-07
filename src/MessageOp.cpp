#include "MessageOp.hpp"

/*Message::Message(const std::string &str)
{
	std::string	strWithoutNL = std::string(str.begin(), str.end() - 1);
	std::queue<std::string>	que = split(strWithoutNL, ' ', false);
	if (que.size() > 0 && que.front()[0] == ':')
	{
		prefix = std::string(que.front().begin() + 1, que.front().end());
		que.pop();
	}
	if (que.size() > 0)
	{
		command = que.front();
		que.pop();
	}
	while (que.size() > 0)
	{
		if (que.front()[0] == ':')
		{
			std::string	s(que.front().begin() + 1, que.front().end());
			que.pop();
			while (que.size() > 0)
			{
				s.append(" ");
				s.append(que.front());
				que.pop();
			}
			parameters.push_back(s);
		}
		else
		{
			parameters.push_back(que.front());
			que.pop();
		}
	}
}*/

void getPrefix(string prefix){
    return prefix;
}

void getCmd(string cmd){
    return cmd;
}

void getParams(){
    return params;
}