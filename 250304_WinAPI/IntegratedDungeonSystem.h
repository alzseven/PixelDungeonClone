#pragma once
#include "config.h"
#include "DungeonGenerator.h"
#include "MonsterGenerator.h"

class IDungeonGenerator;
class Level;
class IntegratedDungeonSystem {
private:
    IDungeonGenerator* dungeonGenerator;
    // DungeonGenerator* dungeonGenerator;
    MonsterGenerator monsterGenerator;

public:
    IntegratedDungeonSystem() = default;
    ~IntegratedDungeonSystem() = default;

    // Generate a complete dungeon with monsters
    void GenerateDungeon(bool isProcedural, Level* level, int width, int height, int roomCount, int minRoomSize, int maxRoomSize, WCHAR* filepath = nullptr, int floorLevel = 2);
    IDungeonGenerator* GetDungeonGenerator() const {return dungeonGenerator;}

    wstring GetFileNameFromWCHAR(const WCHAR* fullPath);
    string WStringToString(const std::wstring& wstr);
};
