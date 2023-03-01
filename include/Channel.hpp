#ifndef CHANEL_H
#define CHANEL_H

#include <map>
#include <string>
#include <vector>

struct endMessage {
	std::string from_m;
	std::vector<std::string> to_m;
	std::string datamessage;
};

class Channel {
public:
	Channel(std::string& chname);
	void addUser(std::string& username);
	void deleteUser(std::string& username);
	void addMessage(std::string& from_username,
					std::vector<std::string> to_usernames, std::string message);
	std::vector<std::string> const& getUsers() const;
	endMessage getMessage();
	void kickUser(std::string username);
	void setTopic(std::string topic);
	std::string getTopic();
	void editMode();
	void inviteUser(std::string nickname);
	bool containsUser(std::string username);

	std::string getOperatorUsername();

private:
	std::vector<std::string> users;
	std::vector<endMessage> messages;
	std::string m_name;
	std::string operator_username;
	std::string topic;
	std::vector<std::string> parameters;
};

#endif // CHANEL_H