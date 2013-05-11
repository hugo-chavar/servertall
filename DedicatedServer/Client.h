#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "Connector.h"

class Client {
private:
	bool active;

	Connector connector;

public:
	Client(Socket* socket, InstructionQueue* instructionQueue, std::string userID = "", bool inyectUserIDonReceive =  false);

	bool isActive();

	void setActive(bool active);

	Connector& getConnector();

	std::string getUserID();

	void setUserID(std::string userID);

	Socket* getSocket();

	void addInstruction(Instruction instruction);

	void startClient();

	void stopClient();

	~Client();
};

#endif // _CLIENT_H_