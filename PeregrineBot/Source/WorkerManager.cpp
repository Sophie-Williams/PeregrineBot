#include "WorkerManager.h"

#include "BuildOrderManager.h"
#include "BuildingManager.h"
#include "OrderManager.h"
#include "Utility.h"

using namespace BWAPI;
using namespace Filter;

WorkerManager::WorkerManager()
{
}

WorkerManager& WorkerManager::Instance()
{
	static WorkerManager instance;
	return instance;
}

void WorkerManager::DoAllWorkerTasks(BWAPI::Unit u)
{
	// if our worker is idle
	if (u->isIdle()) {
		// Order workers carrying a resource to return them to the center,
		// otherwise find a mineral patch to harvest.
		if (u->isCarryingGas() || u->isCarryingMinerals()) {
			u->returnCargo();
		}
		// The worker cannot harvest anything if it
		// is carrying a powerup such as a flag
		else if (!u->getPowerUp()) {
			// Harvest from the nearest mineral patch or gas refinery
			if (!u->gather(u->getClosestUnit(IsMineralField || IsRefinery))) {
				// If the call fails, then print the last error message
				DebugMessenger::Instance() << Broodwar->getLastError() << std::endl;
				DebugMessenger::Instance() << "Worker couldn't gather mineral or from refinery" << std::endl;
			} else {
			}

		} // closure: has no powerup
		else {
			DebugMessenger::Instance() << "is idle and has power up?" << std::endl;
		}
	}

	BuildingManager::Instance().isAnythingToBuild(u);
}

//s7jones: @PurpleWaveJadien In plain english, is mineral locking : -spam worker on mineral until it has gathered - return with minerals once it has gathered - each worker assigned to one mineral always
//jaj22 : spam every other frame at most
//s7jones : @PurpleWaveJadien - also have no more than 2 / 2.5 workers to a mineral patch ? -choose minerals based on closest then least amount of workers ?
//Moderator Subscriber Twitch Prime PurpleWaveJadien : spam until it starts gathering, then stop spamming
//bftjoe : you check which mineral its trying to target and only issue command if its wrong
//Moderator Subscriber Twitch Prime PurpleWaveJadien : 2 workers basically saturates a mineral with mineral locking.you get extremely marginal return past that; you'd likely be better off long distance mining
//Moderator Subscriber Twitch Prime PurpleWaveJadien : ^ right, what bftjoe said.spam whenever getTarget is a different mineralwant
