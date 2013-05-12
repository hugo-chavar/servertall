#include "ChatManager.h"

#include <iostream>


// ----------------------------------- CONSTRUCTOR ---------------------------------------

ChatManager::ChatManager() {
	this->statusOk = true;
	this->error = "";
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

void ChatManager::setStatusOk(bool statusOk) {
	this->statusOk = statusOk;
}

void ChatManager::setError(std::string error) {
	this->error = error;
}

//THIS METHOD SUCKS HARDER THAN "I HEART HUCKABEES". CLEAN UP
void ChatManager::processRequests() {
	Instruction instructionIn;
	Instruction instructionOut;
	Client* client = NULL;
	std::string argument = "";

	instructionIn = this->getInstructionQueue().getNextInstruction(true);
	while(!this->isStopping()){
		instructionOut.clear();
		std::cout << instructionIn.serialize() << std::endl;
		switch (instructionIn.getOpCode()) {
			case OPCODE_DISCONNECT_FROM_CHAT:
				argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
				client = this->getClients().detachClient(argument);
				client->stopClient();
				delete client;
				instructionOut.setOpCode(OPCODE_CHAT_MESSAGE_IN);
				//instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_USERD_ID,argument);
				instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_FROM,argument);
				instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_MESSAGE,"---logged out---");
				this->getClients().addBroadcast(instructionOut,argument);
				std::cout << "THE USER " << argument << " DISCONNECTED FROM CHAT" << std::endl;
				break;
			case OPCODE_CHAT_MESSAGE_OUT:
				instructionOut.setOpCode(OPCODE_CHAT_MESSAGE_IN);
				//instructionOut.insertArgument("userID",instructionIn.getArgument("userID"));
				instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_FROM,instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
				instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_MESSAGE,instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_MESSAGE));
				if (instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_TO) != "") {
					client = this->getClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_TO));
					if (client != NULL) {
						client->addInstruction(instructionOut);
						std::cout << "SENDING SINGLE CHAT INSTRUCTION: " << instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_TO) << std::endl;
					} else {
						std::cout << "IT SEEMS CLIENT " << instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_TO) << " HAS LOGGED OUT" << std::endl;
					}
				} else {
					std::cout << "BROADCASTING CHAT INSTRUCTION: " << instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_MESSAGE) << std::endl;
					this->getClients().addBroadcast(instructionOut,instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
				}
				break;
			case OPCODE_CONNECTION_ERROR: {
				std::cout << "THE USER " << instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID) << " DISCONECTED ABRUPTLY FROM CHAT" << std::endl;
				client = this->getClients().detachClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
				if (client != NULL) {
					client->stopClient();
					delete client;
				}
				break;
			}
			default:
				instructionOut.setOpCode(OPCODE_INVALID);
				instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_MESSAGE,"invalid opcode");
			this->getClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID))->addInstruction(instructionOut);
		}
		//To avoid process an invalid instruction if server is shutting down.
		instructionIn = this->getInstructionQueue().getNextInstruction(true);
	}
}

void* ChatManager::run() {
	this->processRequests();
	return NULL;
}

// ----------------------------------- PUBLIC METHODS ------------------------------------

ClientList& ChatManager::getClients() {
	return this->clients;
}

InstructionQueue& ChatManager::getInstructionQueue() {
	return this->instructionQueue;
}

bool ChatManager::isStatusOk() {
	return this->statusOk;
}

std::string ChatManager::getError() {
	return this->error;
}

void ChatManager::startChatManager() {
	this->start();
}

void ChatManager::stopChatManager() {
	//TODO: GET EVERY CLIENT AND STOP IT.
	this->setStopping(true);
	this->getInstructionQueue().stopWaiting();
	this->join();
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

ChatManager::~ChatManager() {
}
