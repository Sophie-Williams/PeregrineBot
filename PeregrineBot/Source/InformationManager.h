#pragma once
#include "BWAPI.h"
#include "UnitInfo.h"
#include "Utility.h"
#include <array>
#include <functional>
#include <memory>

// I couldn't put forward declarations of these above InformationManager
struct ScoutingOptionFor4
{
	std::array<BWAPI::TilePosition, 3> startToP1ToP2;
	BWAPI::TilePosition POther;
	std::array<double, 2> groundTimeFromStartToP1ToP2 = { 0, 0 };
	double airTimeFromStartToOther                    = 0;
	double maxTime                                    = 0;
	double meanTime                                   = 0;
	double stdDev                                     = 0;
};

struct sortByMeanTime
{
	bool operator()(const ScoutingOptionFor4& lhs, const ScoutingOptionFor4& rhs)
	{
		return (lhs.meanTime < rhs.meanTime);
	}
};

struct distAndTime
{
	double distance = 0;
	double time     = 0;
};

class InformationManager
{
private:
	InformationManager() = default;

public:
	static InformationManager& Instance()
	{
		static InformationManager instance;
		return instance;
	}

	void onUnitShow(BWAPI::Unit unit);
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitMorph(BWAPI::Unit unit);

	void setup();
	void update();
	void overlordScouting(BWAPI::Unit overlord);
	void spotting(BWAPI::Unit spotter);

	std::unique_ptr<ResourceUnitInfo> getClosestMineral(BWAPI::Unit u);

	bool m_isEnemyBaseDeduced = false;
	bool isEnemyBaseReached   = false;
	bool isEnemyBaseDestroyed = false;
	BWAPI::Race m_enemyRace   = BWAPI::Races::Unknown;
	bool isEnemyRaceRandom    = false;
	bool isIslandsOnMap       = false;

	std::set<BWAPI::TilePosition> allStarts;
	std::set<BWAPI::TilePosition> m_otherStarts; // would be a good idea to make this const
	std::set<BWAPI::Position> unscoutedPositions;
	std::set<BWAPI::Position> scoutedPositions;
	std::set<ScoutingOptionFor4, sortByMeanTime> scoutingOptions;
	std::map<std::set<BWAPI::TilePosition>, distAndTime> zerglingNetwork;
	std::map<std::set<BWAPI::TilePosition>, distAndTime> overlordNetwork;
	bool m_isEnemyBaseFromSpotting         = false;
	BWAPI::Position enemyBaseSpottingGuess = { 0, 0 };

	std::set<FriendlyUnitInfo> friendlyUnits;

	std::set<EnemyUnitInfo, std::less<>> m_enemyBuildings;
	std::set<EnemyUnitInfo, std::less<>> m_enemyArmy;
	EnemyUnitInfo m_enemyMain = { nullptr };

	std::set<ResourceUnitInfo> minerals;
	std::set<ResourceUnitInfo> geysers;

private:
	void setupScouting();
	void updateScouting();
	void overlordScoutingAtGameStart(BWAPI::Unit overlord);
	void overlordScoutingAfterBaseFound(BWAPI::Unit overlord);
	void overlordRetreatToHome(BWAPI::Unit overlord);
	void addToEnemyBuildings(BWAPI::Unit unit);
	void addToEnemyArmy(BWAPI::Unit unit);
	void validateEnemyUnits();
	void addToMinerals(BWAPI::Unit mineral);
	void addToGeysers(BWAPI::Unit geyser);
	void validateResources();
	void spotUnits(BWAPI::Unit spotter);
	void spotCreep(BWAPI::Unit spotter);

	struct
	{
		double ground = 0;
		double air    = 0;
	} maxBaseToBaseDistance;
	std::map<BWAPI::UnitType, double> m_spottingTimes;
	double m_spottingTime       = 0;
	bool m_isSpottingCreepTime  = true;
	bool m_isSpottingUnitsTime  = true;
	using unitAndPotentialBases = std::pair<BWAPI::Unit, std::set<BWAPI::Position>>;
	std::set<unitAndPotentialBases> spottedUnitsAndPotentialBases;
};
