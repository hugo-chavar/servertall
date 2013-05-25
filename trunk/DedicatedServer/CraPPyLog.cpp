#include "CraPPyLog.h"

#include <time.h>

#include "StringUtilities.h"

// ---------------------------- STATIC MEMBERS INITIALIZATION ----------------------------

FILE* CraPPyLog::logFile = fopen(LOG_FILE_PATH,"w");

// ----------------------------------- CONSTRUCTOR ---------------------------------------

CraPPyLog::CraPPyLog() {
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

FILE* CraPPyLog::getLogFile() {
	return this->logFile;
}

std::ostringstream& CraPPyLog::getBuffer() {
	return this->buffer;
}

std::string CraPPyLog::getTimeStamp() {
	std::string timeStamp = "";
	__time64_t rawTime;
	struct tm timeInfo;
	_time64(&rawTime); 
	_localtime64_s(&timeInfo,&rawTime); 

	timeStamp = timeStamp + 
				stringUtilities::intToString(timeInfo.tm_mday) + DATE_SEPARATOR +
				stringUtilities::intToString(timeInfo.tm_mon + 1) + DATE_SEPARATOR +
				stringUtilities::intToString(timeInfo.tm_year + 1900) + " " +
				stringUtilities::intToString(timeInfo.tm_hour) + TIME_SEPARATOR +
				stringUtilities::intToString(timeInfo.tm_min) + TIME_SEPARATOR +
				stringUtilities::intToString(timeInfo.tm_sec);

	return  timeStamp;
}

std::string CraPPyLog::loggingLevelToString(loggingLevel_t loggingLevel) {
	std::string loggingLevelAsString;

	switch (loggingLevel) {
		case LOGGING_LEVEL_ERROR:
			loggingLevelAsString = LOGGING_LEVEL_ERROR_AS_STRING;
			break;
		case LOGGING_LEVEL_WARNING:
			loggingLevelAsString = LOGGING_LEVEL_WARNING_AS_STRING;
			break;
		case LOGGING_LEVEL_INFO:
			loggingLevelAsString = LOGGING_LEVEL_INFO_AS_STRING;
			break;
		case LOGGING_LEVEL_DEBUG:
			loggingLevelAsString = LOGGING_LEVEL_DEBUG_AS_STRING;
			break;
	}

	return loggingLevelAsString;
}

// ----------------------------------- PUBLIC METHODS ------------------------------------

void CraPPyLog::log(loggingLevel_t loggingLevel, std::string message, std::string filePath, long int lineNumber, std::string functionName) {
	this->getBuffer() << this->getTimeStamp() << LOG_TAG_SEPARATOR;
	this->getBuffer() << LOG_TAG_FILE << filePath << LOG_TAG_SEPARATOR;
	this->getBuffer() << LOG_TAG_LINE << stringUtilities::longToString(lineNumber) << LOG_TAG_SEPARATOR;
	this->getBuffer() << LOG_TAG_FUNCTION << functionName << LOG_TAG_SEPARATOR;
	this->getBuffer() << this->loggingLevelToString(loggingLevel) << LOG_TAG_MESSAGE << message;
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

CraPPyLog::~CraPPyLog() {
	this->getBuffer() << std::endl;
	//The use of this C functions is for thread safety.
	fprintf(this->getLogFile(),"%s",this->getBuffer().str().c_str());
	fflush(this->getLogFile());
}