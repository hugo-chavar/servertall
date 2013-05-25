#include <sstream>
#include <fstream>

#ifndef loggingLevel_t
enum loggingLevel_t {
	LOGGING_LEVEL_ERROR,
	LOGGING_LEVEL_WARNING,
	LOGGING_LEVEL_INFO,
	LOGGING_LEVEL_DEBUG
};
#endif//loggingLevel_t

#ifndef CURRENT_LOG_LEVEL
#define CURRENT_LOG_LEVEL LOGGING_LEVEL_DEBUG
#endif//CURRENT_LOG_LEVEL

#ifndef LOG_FILE_PATH
#define LOG_FILE_PATH "../serverLog.txt"
#endif//LOG_FILE_PATH

#ifndef LOG_ERROR
#define LOG_ERROR(message) CraPPyLog().log(LOGGING_LEVEL_ERROR,message,__FILE__,__LINE__,__FUNCTION__);
#endif//LOG_ERROR(message)

#ifndef LOG_WARNING
#define LOG_WARNING(message) if (CURRENT_LOG_LEVEL >= LOGGING_LEVEL_WARNING) CraPPyLog().log(LOGGING_LEVEL_WARNING,message,__FILE__,__LINE__,__FUNCTION__);
#endif//LOG_WARNING(message)

#ifndef LOG_INFO
#define LOG_INFO(message) if (CURRENT_LOG_LEVEL >= LOGGING_LEVEL_INFO) CraPPyLog().log(LOGGING_LEVEL_INFO,message,__FILE__,__LINE__,__FUNCTION__);
#endif//LOG_INFO(message)

#ifndef LOG_DEBUG
#define LOG_DEBUG(message) if (CURRENT_LOG_LEVEL >= LOGGING_LEVEL_DEBUG) CraPPyLog().log(LOGGING_LEVEL_DEBUG,message,__FILE__,__LINE__,__FUNCTION__);
#endif//LOG_DEBUG(message)

#ifndef LOGGING_LEVEL_ERROR_AS_STRING
#define LOGGING_LEVEL_ERROR_AS_STRING "ERROR"
#endif//LOGGING_LEVEL_ERROR_AS_STRING

#ifndef LOGGING_LEVEL_WARNING_AS_STRING
#define LOGGING_LEVEL_WARNING_AS_STRING "WARNING"
#endif//LOGGING_LEVEL_WARNING_AS_STRING

#ifndef LOGGING_LEVEL_INFO_AS_STRING
#define LOGGING_LEVEL_INFO_AS_STRING "INFO"
#endif//LOGGING_LEVEL_INFO_AS_STRING

#ifndef LOGGING_LEVEL_DEBUG_AS_STRING
#define LOGGING_LEVEL_DEBUG_AS_STRING "DEBUG"
#endif//LOGGING_LEVEL_DEBUG_AS_STRING

#ifndef DATE_SEPARATOR
#define DATE_SEPARATOR "-"
#endif//DATE_SEPARATOR

#ifndef TIME_SEPARATOR
#define TIME_SEPARATOR ":"
#endif//TIME_SEPARATOR

#ifndef LOG_TAG_SEPARATOR
#define LOG_TAG_SEPARATOR " | "
#endif//LOG_TAG_SEPARATOR

#ifndef LOG_TAG_FILE
#define LOG_TAG_FILE "File: "
#endif//LOG_TAG_FILE

#ifndef LOG_TAG_LINE
#define LOG_TAG_LINE "Line: "
#endif//LOG_TAG_LINE

#ifndef LOG_TAG_FUNCTION
#define LOG_TAG_FUNCTION "Function: "
#endif//LOG_TAG_FUNCTION

#ifndef LOG_TAG_MESSAGE
#define LOG_TAG_MESSAGE ":	"
#endif//LOG_TAG_MESSAGE

class CraPPyLog {
private:
	static FILE* logFile;

	std::ostringstream buffer;

	FILE* getLogFile();

	std::ostringstream& getBuffer();

	std::string getTimeStamp();

	std::string loggingLevelToString(loggingLevel_t loggingLevel);

public:
	CraPPyLog();

	void log(loggingLevel_t loggingLevel, std::string message, std::string filePath, long int lineNumber, std::string functionName);

	~CraPPyLog();
};