#include "LoginManager.h"

#pragma warning(disable: 4355)

#include "Game.h"
#include "CraPPyLog.h"

// ----------------------------------- CONSTRUCTOR ---------------------------------------

LoginManager::LoginManager(int portToListen, int maxPendingConnections, ChatManager& chatManager, SimulationManager& simulationManager) : listener(INADDR_ANY,portToListen,maxPendingConnections,this->getPreLoggedClients(),this->getInstructionQueue()), chatManager(chatManager), simulationManager(simulationManager) {
	this->statusOk = true;
	this->error = "";
	this->maxFileUpdaters = MAX_FILE_UPDATERS;
	this->getClientUpdaters().reserve(this->maxFileUpdaters);
	for (unsigned int i = 0; i < this->maxFileUpdaters; i++) {
		this->createClientUpdater();
	}
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

ClientList& LoginManager::getPreLoggedClients() {
	return this->preLoggedClients;
}

ClientList& LoginManager::getLoggedClients() {
	return this->loggedClients;
}

InstructionQueue& LoginManager::getInstructionQueue() {
	return this->instructionQueue;
}

Listener& LoginManager::getListener() {
	return this->listener;
}

unsigned int LoginManager::getMaxFileUpdaters() {
	return this->maxFileUpdaters;
}

std::vector<ClientUpdater*>& LoginManager::getClientUpdaters() {
	return this->clientUpdaters;
}

ChatManager& LoginManager::getChatManager() {
	return this->chatManager;
}

SimulationManager& LoginManager::getSimulationManager() {
	return this->simulationManager;
}

void LoginManager::setStatusOk(bool statusOk) {
	this->statusOk = statusOk;
}

void LoginManager::setError(std::string error) {
	this->error = error;
}

void LoginManager::startListener() {
	this->getListener().startListening();
}

void LoginManager::stopListener() {
	this->getListener().stopListening();
}

//THIS METHOD SUCKS HARDER THAN "I HEART HUCKABEES". CLEAN UP
void LoginManager::processRequests() {
	Instruction instructionIn;
	Instruction instructionOut;
	Client* client = NULL;
	std::string argument = "";
	unsigned int index = 0;
	bool found = false;

	this->startListener();

	if (!this->getListener().isStatusOk()) {
		this->setStatusOk(false);
		this->setError(this->getListener().getError());
	}else{
		instructionIn = this->getInstructionQueue().getNextInstruction(true);
		while(!this->isStopping()) {
			instructionOut.clear();
			LOG_DEBUG("RECEIVED THE FOLLOWING ISTRUCTION: " + instructionIn.serialize());
			switch (instructionIn.getOpCode()) {
				case OPCODE_LOGIN_REQUEST:
					argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID);
					if ( (argument != "") && (this->getLoggedClients().isUserIDAvailable(argument)) ) {
						//std::string characterType = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_CHARACTER);
						//if (GameView::instance().isCharacterTypeValid(characterType)) {
							instructionOut.setOpCode(OPCODE_LOGIN_OK);
							instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_GREETING,"Welcome " + argument);
							instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_STAGE_NUMBER, stringUtilities::intToString(Game::instance().stageActual()));//Esto tmb puede estar trayendo problemas
							client = this->getPreLoggedClients().detachClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
							client->setUserID(argument);
							this->getLoggedClients().addClient(client);
							//GameView::instance().addPlayer(argument, characterType);
							//std::cout << "THE USER " << argument << " LOGGED IN" << std::endl;
							LOG_DEBUG("THE USER " + argument + " LOGGED IN");
							//GameView::instance().wakeUpPlayer(argument);
							//GameView::instance().startUpdatingPlayer(argument);
						//}
						//else {
						//	instructionOut.setOpCode(OPCODE_INVALID_CHARACTER);
						//	instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_ERROR,"Invalid main character number");
						//	client = this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
						//	std::cout << "INVALID CHARACTER NUMBER FOR USER ID " << argument << std::endl;
						//}
					} else if ((argument != "") && (!this->getLoggedClients().isUserIDAvailable(argument))) {
						//codigo que no se llama nunca
						instructionOut.setOpCode(OPCODE_LOGIN_OK);
						instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_GREETING,"Welcome back " + argument);
						instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_STAGE_NUMBER, stringUtilities::intToString(Game::instance().stageActual()));
						this->getPreLoggedClients().detachClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
						client = this->getLoggedClients().getClient(argument);
						client->addInstruction(instructionOut);
						instructionOut.clear();
						instructionOut.setOpCode(OPCODE_INIT_SYNCHRONIZE);
						//std::string characterInit = GameView::instance().managePlayerInitialSynch(argument);
						//instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_CHARACTER_INIT, characterInit);
						LOG_DEBUG("THE USER " + argument + " LOGGED IN");
						//GameView::instance().wakeUpPlayer(argument);
					}
					else {
						instructionOut.setOpCode(OPCODE_USERID_NOT_AVAILABLE);
						instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_ERROR,"Invalid user ID");
						client = this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
						LOG_DEBUG("THE USERID " + argument + " IS NOT AVAILABLE");
						//instructionOut.setOpCode(OPCODE_USERID_NOT_AVAILABLE);
						//instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_ERROR,"User ID already in use");
						//client = this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
						//std::cout << "THE USERID " << argument << " IS ALREADY IN USE" << std::endl;
					}
					client->addInstruction(instructionOut);
					break;
				case OPCODE_UPDATE_REQUEST:
					found=false;
					for (unsigned int i = 0; (i < this->getMaxFileUpdaters() && !found); i++) {
						if (this->getClientUpdaters()[i]->isAvailable()) {
							found = true;
							index = i;
						}
					}
					if (index < this->getMaxFileUpdaters()) {
						client = this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
						if (client != NULL) {// THIS CHECK SHOULD BE UNNECESARY.....
							LOG_DEBUG("THE USER " + instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID) + " IS UPDATING");
							common::Logger::instance().log("index:"+stringUtilities::intToString(index));							
							this->getClientUpdaters()[index]->setClient(client);
							this->getClientUpdaters()[index]->startClientUpdater();
						}
					} else {
						instructionOut.setOpCode(OPCODE_SERVER_BUSY);
						//TODO: Add some lame ass message
						this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID))->addInstruction(instructionOut);
					}
					break;
				case OPCODE_CONNECT_TO_CHAT:
					argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
					client = this->getPreLoggedClients().detachClient(argument);
					if ( (client != NULL) && (!this->getLoggedClients().isUserIDAvailable(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID))) ) {
						instructionOut.setOpCode(OPCODE_CHAT_CONNECTION_ESTABLISHED);
						instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_GREETING,"SOME CHAT GREEETING MESSAGE");
						client->getConnector().setInstructionQueue(&this->getChatManager().getInstructionQueue());
						client->setUserID(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID));
						this->getChatManager().getClients().addClient(client);
						client->addInstruction(instructionOut);
						LOG_DEBUG("THE USER " + instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID) + " CONNECTED TO CHAT");
					} else {
						LOG_ERROR("THE NON-EXISTANT USER " + argument + " TRIED TO CONNECT TO CHAT");
					}
					break;
				case OPCODE_CONNECT_TO_SIMULATION:
					argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
					client = this->getPreLoggedClients().detachClient(argument);
					if ( (client != NULL) && (!this->getLoggedClients().isUserIDAvailable(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID))) ) {
						instructionOut.setOpCode(OPCODE_SIMULATION_CONNECTION_ESTABLISHED);
						instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_GREETING,"SOME SIMULATION GREEETING MESSAGE");
						client->getConnector().setInstructionQueue(&this->getSimulationManager().getInstructionQueue());
						client->setUserID(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID));
						this->getSimulationManager().getClients().addClient(client);
						client->addInstruction(instructionOut);
						LOG_DEBUG("THE USER " + instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID) + " CONNECTED TO SIMULATION");
					} else {
						LOG_ERROR("THE NON-EXISTANT USER " + argument + " TRIED TO CONNECT TO SIMULATION");
					}
					break;
				case OPCODE_LOGOUT_REQUEST:
					argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
					client = this->getLoggedClients().detachClient(argument);
					if (client != NULL) {
						LOG_DEBUG("THE USER " + argument + " LOGGED OUT");
						client->stopClient();
						delete client;
					} else {
						LOG_ERROR("THE NON-EXISTANT USER " + argument + " TRIED TO LOG OUT");
					}
					break;
				case OPCODE_DISCONNECT:
					argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
					client = this->getPreLoggedClients().detachClient(argument);
					if (client != NULL) {
						LOG_DEBUG("THE USER " + argument + " DISCONNECTED");
						client->stopClient();
						delete client;
					} else {
						LOG_ERROR("THE NON-EXISTANT USER " + argument + " TRIED TO DISCONNECT");
					}
					break;
				case OPCODE_CONNECTION_ERROR:
					LOG_ERROR("THE USER " + instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID) + " DISCONECTED ABRUPTLY");
					client = this->getPreLoggedClients().detachClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
					if (client == NULL) {
						client = this->getLoggedClients().detachClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
					}
					if (client != NULL) {
					client->stopClient();
					delete client;
					} else {
						LOG_ERROR("CONNECTION ERROR RECEIVED FROM THE NON-EXISTANT USER " + instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
					}
					break;
				default:
					instructionOut.setOpCode(OPCODE_INVALID);
					this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID))->addInstruction(instructionOut);
				break;
			}
			//To avoid process an invalid instruction if server is shutting down.
			instructionIn = this->getInstructionQueue().getNextInstruction(true);
		}

		this->stopListener();

	}
}

void LoginManager::createClientUpdater() {
	ClientUpdater* clientUpdater = new ClientUpdater(this->getInstructionQueue(),&this->updaterMutex);
	this->getClientUpdaters().push_back(clientUpdater);
}

void* LoginManager::run() {
	this->processRequests();
	return NULL;
}

// ----------------------------------- PUBLIC METHODS ------------------------------------

bool LoginManager::isStatusOk() {
	return this->statusOk;
}

std::string LoginManager::getError() {
	return this->error;
}

void LoginManager::startLoginManager() {
	this->start();
	LOG_DEBUG("LOGIN MANAGER THREAD STARTED");
}

void LoginManager::stopLoginManager() {
	//TODO: GET EVERY CLIENT AND STOP IT.
	this->setStopping(true);
	this->getInstructionQueue().stopWaiting();
	this->join();
	LOG_DEBUG("LOGIN MANAGER THREAD STOPPED");
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

LoginManager::~LoginManager() {
	for (unsigned int i = 0; i < this->getMaxFileUpdaters(); i++) {
		if (!this->getClientUpdaters()[i]->isAvailable())
			this->getClientUpdaters()[i]->stopClientUpdater();
		delete this->getClientUpdaters()[i];
	}
}
