#include "stdafx.h"

#include "Log.h"
#include<filesystem>

//Start Static Variable Declarations//
std::shared_ptr<spdlog::logger> Log::DefaultSTDOUTLogger;
std::shared_ptr<spdlog::logger> Log::DefaultFileLogger;

std::once_flag Log::InitializationFlag;
bool Log::InitializationState = false;
Log* Log::PointerToSingletonLogClass;

std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Log::StdOut_LoggingObjectsContainer;
std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> Log::File_LoggingObjectsContainer;

//Ended Static Variable Declarations//

static std::vector<std::string> SeperateStringUsingSeperators(std::string StringBase, std::string Seperator)
{
	std::vector<std::string> FinalString;
	FinalString.emplace_back(std::string()); //Emplace A Default One

	for (int i = 0; i < StringBase.length(); i++)
	{
		int j = 0;
		bool Flag = false;

		while (Seperator[j] == StringBase[i])
		{
			if (j == Seperator.length() - 1)
			{
				FinalString.emplace_back(std::string());
				Flag = true;
				break;
			}
			else
			{
				i++;
				j++;
			}
			if (!(i < StringBase.length()))
				break;
		}

		if (Flag == false)
			i = i - j;
		else continue;

		FinalString[FinalString.size() - 1].push_back(StringBase[i]);
	}

	if (FinalString[FinalString.size() - 1] == "")
		FinalString.erase(FinalString.end() - 1);

	return FinalString;
}

static bool CheckIfFolderExists(std::filesystem::path TotalPath, std::string FolderName)
{
	std::string FolderNameCopy1 = "\\" + FolderName + "\\";
	std::string FolderNameCopy2 = "\\" + FolderName;

	if (TotalPath.u8string().find(FolderNameCopy1) != std::string::npos)
		return true;
	if (TotalPath.u8string().find(FolderNameCopy2) != std::string::npos)
		return true;
	return false; //if it did reach here then we are not looking for this directory..
}

static bool CheckIfPathPointsToDirectory(std::filesystem::path FolderPath, std::string FolderName)
{
	std::string PathCopy = FolderPath.u8string();
	std::string FolderNameExtracted;

	for (auto i = PathCopy.rbegin(); i != PathCopy.rend(); i++)
	{
		if (*i == '\\' || *i == '/')
			break;
		FolderNameExtracted = *i + FolderNameExtracted;
	}

	if (FolderNameExtracted == FolderName)
		return true;
	else
		return false;
}

static std::filesystem::path GetLogFolderPath()
{
	bool LogFolderFound = false;
	std::string LogDirectoryName = "LogSystemLogFolder";

	static std::filesystem::path LogFilePath = ""; //This Is Actually A Static Variable So Its Kindaa Initialized Only Once
	
	std::filesystem::path CurrentFilePath = std::filesystem::current_path();
	
	if (LogFilePath.u8string() == "")
	{
		std::filesystem::recursive_directory_iterator Iterator(CurrentFilePath);

		for (auto i : Iterator)
		{
			if (CheckIfPathPointsToDirectory(i, LogDirectoryName))
			{
				LogFilePath = i;
				LogFolderFound = true;
				break; //We Know For a Fact There is only One Log Folder and We Are Inside It
			}
		}

		//Start Essentially Code Here Means Please Create Your Own Log Folder//
		if (LogFolderFound == false)
		{
			std::filesystem::path NewLogFolderPath = CurrentFilePath.u8string() + "\\" + LogDirectoryName;
			std::filesystem::create_directories(NewLogFolderPath);
			LogFilePath = NewLogFolderPath;
		}
		//Ended Essentially Code Here Means Please Create Your Own Log Folder//
		
		return LogFilePath;
	}
	else return LogFilePath;
}



Log::Log(std::vector<std::pair<std::string, CreateLogFile>> Configuration)
{
	for (auto& i : Configuration)
	{
		std::shared_ptr<spdlog::logger> StdOut_Logger = spdlog::stdout_color_mt(i.first);
		StdOut_Logger->set_level(spdlog::level::trace);

		std::shared_ptr<spdlog::logger> File_Logger;
		StdOut_LoggingObjectsContainer.insert({ i.first,StdOut_Logger });

		if (i.second == CreateLogFile::CreateLogFile)
		{
			std::string FileLoggerFilePath = GetLogFolderPath().u8string() + "\\" + i.first + ".txt";
			File_Logger = spdlog::basic_logger_mt(i.first+std::string("FileLogger"), FileLoggerFilePath);
			File_Logger->set_level(spdlog::level::trace);

			File_LoggingObjectsContainer.insert({ i.first,File_Logger });
		}
	}
}

void Log::InitHelper(std::vector<std::pair<std::string, CreateLogFile>> Configuration)
{
	std::call_once(InitializationFlag, [&]() {
		spdlog::set_pattern("%^[%T] %n : %v%$");
		PointerToSingletonLogClass = new Log(Configuration);

		DefaultSTDOUTLogger = spdlog::stdout_color_mt("Default");
		DefaultSTDOUTLogger->set_level(spdlog::level::trace);

		std::string DefaultFileLoggerPath = GetLogFolderPath().u8string() + "\\" + "DefaultFileLogger.txt";
		DefaultFileLogger = spdlog::basic_logger_mt("DefaultFileLogger", DefaultFileLoggerPath);
		DefaultFileLogger->set_level(spdlog::level::trace);

		InitializationState = true;
	});
}

void Log::Init(std::vector<std::pair<std::string, CreateLogFile>> Configuration)
{
	InitHelper(Configuration);
}

void Log::InitFromManifestFile(std::filesystem::path ManifestFilePath)
{
	//Complete This Function....
	std::vector<std::pair<std::string, CreateLogFile>> Configuration;
	std::ifstream File(ManifestFilePath,std::ios::binary);

	if (!File) //The Debugger Will Break If The Manifest File Could Not Be Opened
	{
		std::filesystem::remove("LoggerError.txt"); //Remove The File If It Already Exists So that a new File Can Be Written

		std::ofstream OutputFile("LoggerError.txt",std::ios::app);
		OutputFile << "Please See That a LogManifestFile.txt is Created and populated" << std::endl;
		OutputFile << "Population Structure ->" << std::endl;
		OutputFile << "{Name Of The File Logger}' '{CreateLogFile}" << std::endl;
		OutputFile << "{Name Of The File Logger is Compulsory and must be given oh and ignore'{}'}" << std::endl;
		OutputFile << "' 'This Acts as a seperator ignore the '' and just give a space in the writing" << std::endl;
		OutputFile << "Note - CreateLogFile is not compulsory if you only want stdout output... However see that there is no\n\
					  Spelling Mistake, Anything Not CreateLogFile Translates To Donot Create Log File" << std::endl;
		OutputFile << "An Example Lastly - " << std::endl;
		OutputFile << "OutputFileLine May Look Something Like - " << std::endl;
		OutputFile << "Rommulus CreateLogFile" << std::endl;
		OutputFile << "This Says Create Logger Rommulus Which can also automatically log to file" << std::endl;
		OutputFile << "Tiberius" << std::endl;
		OutputFile << "A Line Like This Will Only Generate a stdout logger with the name tiberius" << std::endl;
	}
	
	std::string Line;
	while (std::getline(File, Line)) //Gets A Line From The ManifestFile..)
	{
		std::vector<std::string> BrokenApartLine = SeperateStringUsingSeperators(Line, " ");
		BrokenApartLine.emplace_back(std::string("")); //This is just a placeholder so that create LogFile Does Not Fail...

		std::pair<std::string, CreateLogFile> TemperoryCasket;

		if (BrokenApartLine[0] == "")
			continue;

		TemperoryCasket.first = BrokenApartLine[0];

		if (BrokenApartLine[1] == std::string("CreateLogFile"))
		{
			TemperoryCasket.second = CreateLogFile::CreateLogFile;
		}
		else
		{
			TemperoryCasket.second = CreateLogFile::DontCreateLogFile;
		}
		Configuration.emplace_back(TemperoryCasket);
	}

	//InitHelper(Configuration); //This Function Does The Actual Initialization Stuff
	InitHelper(Configuration);
}

std::shared_ptr<spdlog::logger>& Log::GetDefaultSTDOUTLogger()
{
	return DefaultSTDOUTLogger;
}

std::shared_ptr<spdlog::logger>& Log::GetDefaultFileLogger()
{
	return DefaultFileLogger;
}

template<typename Key,typename Value>
static std::optional<Value>CheckMapForValueAndGetIt(std::unordered_map<Key, Value>& MapInQuestion, Key keytocheck)
{
	auto Casket = MapInQuestion.find(keytocheck);//StdOut_LoggingObjectsContainer.find(Name);
	if (Casket == MapInQuestion.end())//StdOut_LoggingObjectsContainer.end())
		return {};
	else
	{
		return Casket->second;
	}
}

std::optional<std::shared_ptr<spdlog::logger>> Log::GetSTDOUTLoggerOfTheName(std::string Name)
{
	auto Value = CheckMapForValueAndGetIt(StdOut_LoggingObjectsContainer, Name);
	return Value;
}

std::optional<std::shared_ptr<spdlog::logger>> Log::GetFileLoggerOfTheName(std::string Name)
{
	auto Value = CheckMapForValueAndGetIt(File_LoggingObjectsContainer, Name);
	return Value;
}

bool Log::AmIInitialized()
{
	return InitializationState;
}
