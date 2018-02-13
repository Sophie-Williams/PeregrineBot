#include "UnitInfo.h"

#include "Utility.h"

using namespace BWAPI;
using namespace Filter;

UnitInfo::UnitInfo(BWAPI::Unit unitToWrap)
{
	if (unitToWrap) {
		u = unitToWrap;
		update();
	} else {
		errorMessage("unit null 1");
	}
}

void UnitInfo::update() const
{
	if (u) {
		if (exists()) {
			lastFrameSeen = Broodwar->getFrameCount();
			pos           = u->getPosition();
			type          = u->getType();
		}
	} else {
		errorMessage("unit null 2");
	}
}

bool UnitInfo::exists() const
{
	if (u) {
		return u->exists();
	} else {
		errorMessage("unit null 3");
		return false;
	}
}

void EnemyUnitInfo::update() const
{
	UnitInfo::update();
	if (u) {
		if (exists()) {
			shields  = u->getShields();
			hp       = u->getHitPoints();
			energy   = u->getEnergy();
			velocity = std::make_pair(u->getVelocityX(), u->getVelocityY());
		}
	}
}

void ResourceUnitInfo::update() const
{
	UnitInfo::update();
	if (u) {
		if (exists()) {
			resources = u->getResources();
		}
	}
}

int ResourceUnitInfo::getResources() const
{
	return resources;
}
