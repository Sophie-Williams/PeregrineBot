#pragma once
#include "BWAPI.h"
#include "Squad.h"
#include "UnitInfo.h"
#include <deque>
#include <set>
#include <vector>

class ArmyManager {
private:
	ArmyManager() = default;

public:
	static ArmyManager& Instance()
	{
		static ArmyManager instance;
		return instance;
	}
	void update();

	using SquadList = std::vector<Squad>;
	const SquadList& getSquads() { return squads; };
	void zerglingAttack(BWAPI::Unit u);
	void zerglingAttackKnownBuildings(BWAPI::Unit u);
	void zerglingScoutingBeforeBaseFound(BWAPI::Unit u);
	void zerglingScoutSpreadOut(BWAPI::Unit u);
	void zerglingAttackKnownBuildings(const Squad& squad)
	{
		for (auto unit : squad) {
			zerglingAttackKnownBuildings(unit);
		}
	}
	void zerglingScoutingBeforeBaseFound(const Squad& squad)
	{
		for (auto unit : squad) {
			zerglingScoutingBeforeBaseFound(unit);
		}
	}
	void zerglingScoutSpreadOut(const Squad& squad)
	{
		for (auto unit : squad) {
			zerglingScoutSpreadOut(unit);
		}
	}

	const int SQUAD_RADIUS = 128;

private:
	std::set<EnemyUnitInfo> getZerglingAccessibleBuildings(BWAPI::Unit u);
	void putUnassignedInSquads();
	void attackWithSquad(Squad& squad);
	void handleIdleUnits();
	void incrementScoutLocationZerglingIndex();

	std::vector<Squad> squads;

	std::deque<BWAPI::Position> scoutLocationsZergling;
	std::deque<BWAPI::Position>::iterator scoutLocationIndex;
};
