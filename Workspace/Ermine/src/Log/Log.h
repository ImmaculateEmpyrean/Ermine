#pragma once

//Note - Modify this File always with respect to the project you are in for now this will have only one logger with it..

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include<memory>
#include<string>
#include<map>
#include<unordered_map>
#include<filesystem>
#include<fstream>
#include<optional>
#include<mutex>

enum class CreateLogFile :unsigned int
{
	DontCreateLogFile = 0,
	CreateLogFile = 1
};

class Log
{
private:
	Log(std::vector<std::pair<std::string, CreateLogFile>> Configuration); //Note-The enum is to decide wether to write to a file or Not.
																		  

public:
	static void Init(std::vector<std::pair<std::string, CreateLogFile>> Configuration); //Initialize The Log Library
	static void InitFromManifestFile(std::filesystem::path ManifestFilePath = "LogManifest.txt");

private:
	static void InitHelper(std::vector<std::pair<std::string, CreateLogFile>> Configuration);

private:
	static bool InitializationState;
	static std::once_flag InitializationFlag;
	static Log* PointerToSingletonLogClass;

	static std::shared_ptr<spdlog::logger> DefaultSTDOUTLogger;
	static std::shared_ptr<spdlog::logger> DefaultFileLogger;

	//static std::vector<std::shared_ptr<spdlog::logger>> LoggerContainer;
	//static std::vector<std::string> LoggerNameContainer;

	static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> StdOut_LoggingObjectsContainer;
	static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> File_LoggingObjectsContainer;

public:
	static std::shared_ptr<spdlog::logger>& GetDefaultSTDOUTLogger();
	static std::shared_ptr<spdlog::logger>& GetDefaultFileLogger();
	
	static std::optional<std::shared_ptr<spdlog::logger>> GetSTDOUTLoggerOfTheName(std::string Name);
	static std::optional<std::shared_ptr<spdlog::logger>> GetFileLoggerOfTheName(std::string Name);

public:
	static bool AmIInitialized(); //It is Better You Call This Function To Check Before Actually Using The Logger
};

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP) || defined(GAME_DEBUG_DEVELOP) || defined(GAME_DEBUG_SHIP)
#define LOG_WRITE
#endif 


#ifdef LOG_WRITE

	//Start These Macros Are Used To Specify If You Wanna Print To STDOUT Using Default Logger 
	#define STDOUTDefaultLog_Trace(...)    Log::GetDefaultSTDOUTLogger()->trace(__VA_ARGS__);
	#define STDOUTDefaultLog_Info(...)     Log::GetDefaultSTDOUTLogger()->info(__VA_ARGS__);
	#define STDOUTDefaultLog_Warn(...)     Log::GetDefaultSTDOUTLogger()->warn(__VA_ARGS__);
	#define STDOUTDefaultLog_Error(...)    Log::GetDefaultSTDOUTLogger()->error(__VA_ARGS__);
	#define STDOUTDefaultLog_Critical(...) Log::GetDefaultSTDOUTLogger()->critical(__VA_ARGS__);
	//Ended These Macros Are Used To Specify If You Wanna Print To STDOUT Using Default Logger 
	
	//Start These Macros Are Used To Specify If You Wanna Print To File Using Default Logger 
	#define FileDefaultLog_Trace(...)    Log::GetDefaultFileLogger()->trace(__VA_ARGS__);
	#define FileDefaultLog_Info(...)     Log::GetDefaultFileLogger()->info(__VA_ARGS__);
	#define FileDefaultLog_Warn(...)     Log::GetDefaultFileLogger()->warn(__VA_ARGS__);
	#define FileDefaultLog_Error(...)    Log::GetDefaultFileLogger()->error(__VA_ARGS__);
	#define FileDefaultLog_Critical(...) Log::GetDefaultFileLogger()->critical(__VA_ARGS__);
	//Ended These Macros Are Used To Specify If You Wanna Print To File Using Default Logger
	
	
	//Start These Macros Are Used To Specify If You Wanna Print To STDOUT Using CustomNamed Logger 
	#define STDOUTLog_Trace(name,...)    Log::GetSTDOUTLoggerOfTheName(name).value_or(Log::GetDefaultSTDOUTLogger())->trace(__VA_ARGS__);
	#define STDOUTLog_Info(name,...)     Log::GetSTDOUTLoggerOfTheName(name).value_or(Log::GetDefaultSTDOUTLogger())->info(__VA_ARGS__);
	#define STDOUTLog_Warn(name,...)     Log::GetSTDOUTLoggerOfTheName(name).value_or(Log::GetDefaultSTDOUTLogger())->warn(__VA_ARGS__);
	#define STDOUTLog_Error(name,...)    Log::GetSTDOUTLoggerOfTheName(name).value_or(Log::GetDefaultSTDOUTLogger())->error(__VA_ARGS__);
	#define STDOUTLog_Critical(name,...) Log::GetSTDOUTLoggerOfTheName(name).value_or(Log::GetDefaultSTDOUTLogger())->critical(__VA_ARGS__);
	//Ended These Macros Are Used To Specify If You Wanna Print To STDOUT Using CustomeNamed Logger 
	
	//Start These Macros Are Used To Specify If You Wanna Print To File Using CustomNamed Logger 
	#define FileLog_Trace(name,...)    Log::GetFileLoggerOfTheName(name).value_or(Log::GetDefaultFileLogger())->trace(__VA_ARGS__);
	#define FileLog_Info(name,...)     Log::GetFileLoggerOfTheName(name).value_or(Log::GetDefaultFileLogger())->info(__VA_ARGS__);
	#define FileLog_Warn(name,...)     Log::GetFileLoggerOfTheName(name).value_or(Log::GetDefaultFileLogger())->warn(__VA_ARGS__);
	#define FileLog_Error(name,...)    Log::GetFileLoggerOfTheName(name).value_or(Log::GetDefaultFileLogger())->error(__VA_ARGS__);
	#define FileLog_Critical(name,...) Log::GetFileLoggerOfTheName(name).value_or(Log::GetDefaultFileLogger())->critical(__VA_ARGS__);
	//Ended These Macros Are Used To Specify If You Wanna Print To File Using CustomNamed Logger 
	
	//Start These Macros Are To Be Used Most Of The Time And Just Work As Default With The Most Functionality
	#define Log_Trace(name,...) Log::GetSTDOUTLoggerOfTheName(name).value_or(Log::GetDefaultSTDOUTLogger())->trace(__VA_ARGS__);\
								Log::GetFileLoggerOfTheName(name).value_or(Log::GetDefaultFileLogger())->trace(__VA_ARGS__);
	
	#define Log_Info(name,...) Log::GetSTDOUTLoggerOfTheName(name).value_or(Log::GetDefaultSTDOUTLogger())->info(__VA_ARGS__);\
								Log::GetFileLoggerOfTheName(name).value_or(Log::GetDefaultFileLogger())->info(__VA_ARGS__);
	
	#define Log_Warn(name,...) Log::GetSTDOUTLoggerOfTheName(name).value_or(Log::GetDefaultSTDOUTLogger())->warn(__VA_ARGS__);\
								Log::GetFileLoggerOfTheName(name).value_or(Log::GetDefaultFileLogger())->warn(__VA_ARGS__);
	
	#define Log_Error(name,...) Log::GetSTDOUTLoggerOfTheName(name).value_or(Log::GetDefaultSTDOUTLogger())->error(__VA_ARGS__);\
								Log::GetFileLoggerOfTheName(name).value_or(Log::GetDefaultFileLogger())->error(__VA_ARGS__);
	
	#define Log_Critical(name,...) Log::GetSTDOUTLoggerOfTheName(name).value_or(Log::GetDefaultSTDOUTLogger())->critical(__VA_ARGS__);\
								Log::GetFileLoggerOfTheName(name).value_or(Log::GetDefaultFileLogger())->critical(__VA_ARGS__);
	//Ended These Macros Are To Be Used Most Of The Time And Just Work As Default With The Most Functionality

#endif

#ifndef LOG_WRITE
	//Start These Macros Are Stripped When Not In Debug Mode
	#define STDOUTDefaultLog_Trace(...)    
	#define STDOUTDefaultLog_Info(...)     
	#define STDOUTDefaultLog_Warn(...)     
	#define STDOUTDefaultLog_Error(...)    
	#define STDOUTDefaultLog_Critical(...) 
	//Ended These Macros Are Stripped When Not In Debug Mode
	
	//Start These Macros Are Stripped When Not In Debug Mode
	#define FileDefaultLog_Trace(...)    
	#define FileDefaultLog_Info(...)     
	#define FileDefaultLog_Warn(...)     
	#define FileDefaultLog_Error(...)    
	#define FileDefaultLog_Critical(...) 
	//Ended These Macros Are Stripped When Not In Debug Mode
	
	
	//Start These Macros Are Stripped When Not In Debug Mode
	#define STDOUTLog_Trace(name,...)    
	#define STDOUTLog_Info(name,...)     
	#define STDOUTLog_Warn(name,...)     
	#define STDOUTLog_Error(name,...)    
	#define STDOUTLog_Critical(name,...) 
	//Ended These Macros Are Used To Specify If You Wanna Print To STDOUT Using CustomeNamed Logger 
	
	//Start These Macros Are Stripped When Not In Debug Mode
	#define FileLog_Trace(name,...)    
	#define FileLog_Info(name,...)     
	#define FileLog_Warn(name,...)     
	#define FileLog_Error(name,...)    
	#define FileLog_Critical(name,...) 
	//Ended These Macros Are Stripped When Not In Debug Mode
	
	//Start These Macros Are Stripped When Not In Debug Mode
	#define Log_Trace(name,...) 
	#define Log_Info(name,...) 
	#define Log_Warn(name,...) 
	#define Log_Error(name,...) 
	#define Log_Critical(name,...) 
	//Ended These Macros Are Stripped When Not In Debug Mode

#endif // !DEBUG