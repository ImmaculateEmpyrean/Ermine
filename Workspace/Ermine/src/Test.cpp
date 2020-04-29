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
		
	RecieverWithBroadcastComponent Rec;

	SenderA* obj = new SenderA("Hanna");

	obj->Broadcast();
	
	int counter = 0;
	while (true)
	{
		_sleep(1000);
		obj->Broadcast();
		counter++;

		if (counter == 10)
			Rec.Flag = false;
	}
}