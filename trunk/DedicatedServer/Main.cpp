#include <iostream>
#include <winsock2.h>
#include <windows.h>

#include "Main.h"
#include "ChatManager.h"
#include "SimulationManager.h"
#include "LoginManager.h"
#include "Game.h"
#include "Player.h"

#ifndef DEFAULT_PORT_TO_LISTEN
#define DEFAULT_PORT_TO_LISTEN 9443
#endif//DEFAULT_PORT_TO_LISTEN

#ifndef DEFAULT_MAX_PENDING_CONNECTIONS
#define DEFAULT_MAX_PENDING_CONNECTIONS 5
#endif//DEFAULT_MAX_PENDING_CONNECTIONS

int main(int argc, char *argv[]){
	std::string command;

	Game::instance().initialize();
	// AGREGAR addPlayer(player) EN EL LOGINMANAGER POR CADA LOGGEO.
	// Game::instance().addPlayer(player1);

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
	
	std::cin >> command;

	while (command != "exit"){

		std::cout << "COMMAND NOT RECOGNIZED: " << command << std::endl;

		std::cin >> command;
	}

	loginManager.stopLoginManager();

	std::cout << "finishing...." << std::endl;

	system("PAUSE");

	WSACleanup();

	return EXIT_SUCCESS;
}