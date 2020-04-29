#include "stdafx.h"

#include "EventSystem/EventBroadcastStation.h"

#include "TestFolder/RecieverWithBroadcastComponent.h"
#include "TestFolder/SenderWithSendComponent.h"

#include "LogSystem/Log.h"

int main()
{
	std::vector<std::pair<std::string, CreateLogFile>> Configuration;
	Configuration.emplace_back(std::make_pair<std::string, CreateLogFile>( "Augustus",CreateLogFile::CreateLogFile ));
	Log::Init(Configuration);
	//Log::InitFromManifestFile();

	if (Log::AmIInitialized() == true)
	{
		//Log::GetFileLoggerOfTheName("HannaBhi").value_or(Log::GetDefaultFileLogger())->critical("The Empire Lives ON !");
		Log_Trace("Rommulus", "This Is The Founder Of Rome!!!!");
		Log_Info("Rommulus", "I Have Founded The City Of Rome");
		Log_Warn("Rommulus", "The State Is Surrounded By Enemies");
		Log_Error("Rommulus", "We Have Been Attacked By The Latins");
		Log_Critical("Rommulus", "An Aristocratic Coup D'etat My King This Heralds The Beginning Of The Age Of SPQR....");

		Log_Trace("Augustus", "This Is The Founder Of The Roman Empire!!!!");
		Log_Info("Augustus", "I Have Founded The Principate Of Rome");
		Log_Warn("Augustus", "The State Is In An Uproar Due To Civil Unrest And Civil Wars");
		Log_Error("Augustus", "We Have Been Attacked By The Germans");
		Log_Critical("Augustus", "The Julio-Claudians Are Done My Imperator With His Death There Will Be None In The House Of Ceaser Left");

		Log_Critical("Rommulus", "Rome Has Not Yet Fallen It Still Lives On In The Minds Of Ardent Conservatives and Traditionalists");
		Log_Critical("Veer", "Guess This Is It");
		Log_Trace("Augustus", "I am a member of the Populares!! Da");

		STDOUTDefaultLog_Trace("Just Wanna Test This Macro");
		/*CoreLog_Trace("this is just a standrad fare trace message..");
		CoreLog_Info("this is just a standrad fare info message..");
		CoreLog_Warn("this is just a standrad fare warn message..");
		CoreLog_Error("This is just a standrad fare error message..");
		CoreLog_Critical("This is just a standrad fare critical message..");
		//CoreLog_Fatal("This is fatal");
		//Log::GetDefaultLogger()->warn("The Empire!");
		//Log::GetDefaultLogger()->critical("Tet");*/
	}
	/*auto station = Ermine::EventBroadcastStation::GetStation(); //This also tends to initialize the class

	RecieverWithBroadcastComponent RecieverObject2;

	SenderA obj("All Hail The Imperator");
	
	obj.Broadcast();
	
	RecieverWithBroadcastComponent RecieverObject;

	while (true)
	{
		_sleep(1000);
		obj.Broadcast();
	}*/
}