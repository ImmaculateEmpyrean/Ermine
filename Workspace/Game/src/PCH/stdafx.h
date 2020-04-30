#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<stack>
#include<queue>
#include<utility>

#include<thread>
#include<mutex>
#include<condition_variable>
#include<future>
#include<atomic>

#include<memory>
#include<optional>
#include<functional>

//Windows Only Includes..
#if defined(GAME_BUILDING_FOR_WINDOWS) && defined(GAME_BUILDING_ON_WINDOWS)
#include<Windows.h>
#endif

//User Defined Header Files
#include "Log.h"