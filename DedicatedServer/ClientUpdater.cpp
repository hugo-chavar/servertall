#include "ClientUpdater.h"

#include <iostream>
#include "DirList.h"
#include "StringUtilities.h"
#include <fstream>

#include "Instruction.h"

// ----------------------------------- CONSTRUCTOR ---------------------------------------

ClientUpdater::ClientUpdater(InstructionQueue& serverInstructionQueue) : serverInstructionQueue(serverInstructionQueue) {
	this->shuttingDown = false;
	this->statusOk = true;
	this->error = "";
	this->available = true;
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

bool ClientUpdater::isShuttingDown() {
	return this->shuttingDown;
}

void ClientUpdater::setShuttingDown(bool shuttingDown) {
	this->shuttingDown = shuttingDown;
}

void ClientUpdater::setAvailable(bool available) {
	this->available = available;
}

InstructionQueue& ClientUpdater::getInstructionQueue() {
	return this->instructionQueue;
}

InstructionQueue& ClientUpdater::getServerInstructionQueue() {
	return this->serverInstructionQueue;
}

Client* ClientUpdater::getClient() {
	return this->client;
}

void ClientUpdater::setStatusOk(bool statusOk) {
	this->statusOk = statusOk;
}

void ClientUpdater::setError(std::string error) {
	this->error = error;
}

void ClientUpdater::updateClient() {
	Instruction instructionIn;
	Instruction instructionOut;
	std::string argument = "";
	bool finished = false;
	this->getClient()->getConnector().setInstructionQueue(&this->getInstructionQueue());

	sendDirectory("../Images");
	sendDirectory("../Configuration");

	this->getClient()->getConnector().setInstructionQueue(&this->getServerInstructionQueue());
	instructionOut.clear();
	instructionOut.setOpCode(OPCODE_UPDATE_COMPLETE);
	this->getClient()->addInstruction(instructionOut);

	this->stopClientUpdater();
}

void ClientUpdater::sendFile(std::string path)
{
	Instruction instructionOut;
	Instruction instructionIn;
	instructionOut.clear();

	std::ifstream archivo;
	//chequeo que el archivo abra correctamente
	archivo.open(path,std::ios::binary);
	if (!archivo)
		return;

	instructionOut.setOpCode(OPCODE_UPDATE_FILE_START);
	instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_SERIALIZED_PATH,path);
	this->getClient()->addInstruction(instructionOut);
	instructionOut.clear();
	this->receiveConfirmation();

	archivo.seekg(0, std::ios::end);
	int tamanio=(int)archivo.tellg();
	archivo.seekg(0, std::ios::beg);
	int enviados=0;
	char buffer[TAMBUFFER];
	
	//envio el archivo por partes
	while(enviados<tamanio)
	{
		archivo.seekg(enviados);
		archivo.read(buffer,sizeof(buffer));
		int leidos=(int)archivo.gcount();
		enviados+=leidos;
		std::string str_buff=stringUtilities::replaceCharForString('\0',buffer,leidos,"<BARRACERO>");
		instructionOut.setOpCode(OPCODE_UPDATE_FILE);
		instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_SERIALIZED_FILE,str_buff);
		this->getClient()->addInstruction(instructionOut);
		instructionOut.clear();
		this->receiveConfirmation();
	}
	archivo.close();
	instructionOut.setOpCode(OPCODE_UPDATE_FILE_COMPLETE);
	this->getClient()->addInstruction(instructionOut);
	this->receiveConfirmation();
}

void ClientUpdater::sendDirectory(std::string path)
{
	Instruction instructionOut;
	Instruction instructionIn;
	instructionOut.clear();
	//Envio arbol de directorios
	DirList dirList=DirList();
	std::string dir_string="";
	dirList.serializarDirectorio(path,dir_string);
	instructionOut.setOpCode(OPCODE_UPDATE_DIRECTORY);
	instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_SERIALIZED_PATH,path);
	instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_SERIALIZED_DIR,dir_string);
	this->getClient()->addInstruction(instructionOut);
	do
	{
	instructionIn = this->getInstructionQueue().getNextInstruction(true);
	}
	while(instructionIn.getOpCode()!=OPCODE_UPDATE_RECV);

	//Envio archivos
	std::vector<std::string> directorios_v;
	stringUtilities::splitString(dir_string,directorios_v,'~');
	std::string directorioCorriente=path;

	for(int i=0;i<directorios_v.size();i++)
	{
	std::string str=directorios_v[i];
		if(str!="")
		{
			if(str.back()=='>')
			{
				str.erase(str.end()-1);
				directorioCorriente=directorioCorriente+"/"+str;
			}
			else if(str=="..")
			{
				//Borro desde la ultima / al final en el directorio corriente
				int inicial=directorioCorriente.find_last_of('/',directorioCorriente.size()-1);
				directorioCorriente.erase(inicial,directorioCorriente.size()-1);
			}
			else 
			{
				sendFile(directorioCorriente+"/"+str);
			}
		}
	}
}

void ClientUpdater::receiveConfirmation()
{
	Instruction instructionIn;
			do
			{
				instructionIn = this->getInstructionQueue().getNextInstruction(true);
			}
			while(instructionIn.getOpCode()!=OPCODE_UPDATE_RECV);
}

void* ClientUpdater::run() {
	this->updateClient();
	return NULL;
}

// ----------------------------------- PUBLIC METHODS ------------------------------------

bool ClientUpdater::isAvailable() {
	return this->available;
}

bool ClientUpdater::isStatusOk() {
	return this->statusOk;
}

std::string ClientUpdater::getError() {
	return this->error;
}

void ClientUpdater::setClient(Client* client) {
	this->client = client;
}

void ClientUpdater::startClientUpdater() {
	this->start();
}

void ClientUpdater::stopClientUpdater() {
	this->setShuttingDown(true);
	this->getInstructionQueue().stopWaiting();
	this->join();
	this->setClient(NULL);
	this->setAvailable(true);
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

ClientUpdater::~ClientUpdater() {
}