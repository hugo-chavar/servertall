#include "Main.h"

#include <iostream>
#include <winsock2.h>
#include <windows.h>

#include "ChatManager.h"
#include "SimulationManager.h"
#include "LoginManager.h"
#include "GameView.h"
#include "Player.h"

int main(int argc, char *argv[]){
	std::string command;

	GameView::instance().initialize();

	WSAData ws;
	WSAStartup(MAKEWORD(2,2),&ws);

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

	std::cout << "SERVER STARTUP COMPLETE." << command << std::endl;

	std::cin >> command;

	while (command != "exit"){

		std::cout << "COMMAND NOT RECOGNIZED: " << command << std::endl;

		std::cin >> command;
	}

	loginManager.stopLoginManager();

	std::cout << "Finishing...." << std::endl;

	system("PAUSE");

	WSACleanup();

	return EXIT_SUCCESS;
}