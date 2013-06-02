#include "Main.h"

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include "ChatManager.h"
#include "SimulationManager.h"
#include "LoginManager.h"
#include "GameView.h"
#include "CraPPyLog.h"

int main(int argc, char *argv[]){
	LOG_DEBUG("PROGRAM STARTED");
	std::string command;

	WSAData ws;
	WSAStartup(MAKEWORD(2,2),&ws);
	srand(time(NULL));

	GameView::instance().initialize();


	ChatManager chatManager;
	chatManager.startChatManager();

	SimulationManager simulationManager;
	simulationManager.startSimulationManager();

	//Port to listen and max pending connections must be retrieved from YAML
	LoginManager loginManager(DEFAULT_PORT_TO_LISTEN,DEFAULT_MAX_PENDING_CONNECTIONS,chatManager,simulationManager);
	loginManager.startLoginManager();

	if (!loginManager.isStatusOk()){
		std::cerr << "server startup failed." << std::endl;
		std::cerr << "Error: " << loginManager.getError() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "SERVER STARTUP COMPLETE." << std::endl;

	std::cin >> command;

	while (command != "exit"){

		std::cout << "COMMAND NOT RECOGNIZED: " << command << std::endl;

		std::cin >> command;
	} 

	loginManager.stopLoginManager(); 
	simulationManager.stopSimulationManager();
	chatManager.stopChatManager();

	std::cout << "Finishing...." << std::endl;

	WSACleanup();

	LOG_DEBUG("PROGRAM FINISHED");

	return EXIT_SUCCESS;
}