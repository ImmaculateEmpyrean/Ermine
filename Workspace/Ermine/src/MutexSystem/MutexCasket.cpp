#include "stdafx.h"
#include "MutexCasket.h"

Ermine::MutexCasket::MutexCasket(std::unique_lock<std::recursive_mutex> Mutex, Ermine::MutexLevel LevelNumber)
	:
	Mutex(std::move(Mutex)),
	LevelNumber(LevelNumber)
{
	ContainsMutex = true;
}

Ermine::MutexCasket::MutexCasket(MutexCasket&& rhs)
{
	MoveHelper(std::move(rhs));
}

Ermine::MutexCasket& Ermine::MutexCasket::operator=(MutexCasket&& rhs)
{
	MoveHelper(std::move(rhs));
	return *this;
}

std::optional<std::unique_lock<std::recursive_mutex>> Ermine::MutexCasket::GetMutex()
{
	if (ContainsMutex == true)
	{
		return std::move(Mutex);
		ContainsMutex = false;
	}
	else return {};
}

std::optional<Ermine::MutexLevel> Ermine::MutexCasket::GetLevelNumber()
{
	if (ContainsMutex == true)
		return LevelNumber;
	else return {};
}

Ermine::MutexLevel Ermine::MutexCasket::ForceGetLevelNumber()
{
	return LevelNumber;
}


void Ermine::MutexCasket::MoveHelper(MutexCasket&& rhs)
{
	if (rhs.ContainsMutex == true)
	{
		ContainsMutex = rhs.ContainsMutex;
		Mutex = std::move(rhs.Mutex);

		LevelNumber = rhs.LevelNumber;
	}
	else
	{
		ContainsMutex = false;
		LevelNumber = rhs.LevelNumber;
	}
}
