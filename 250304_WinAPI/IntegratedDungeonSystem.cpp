// IntegratedDungeonSystem.cpp
#include "IntegratedDungeonSystem.h"
#include "Level.h"
#include <random>
#include <ctime>

#include "FileLoadDungeonGenerator.h"
#include "ProceduralDungeonGenerator.h"
//
// IntegratedDungeonSystem::IntegratedDungeonSystem() {
//     // Initialize random seed
//     std::srand(static_cast<unsigned int>(std::time(nullptr)));
// }
//
// IntegratedDungeonSystem::~IntegratedDungeonSystem() {
// }
//
// // Generate dungeon and initialize
// void IntegratedDungeonSystem::GenerateDungeon(Level* level, int width, int height, int floorLevel) {
//     if (!level) return;
//     
//     // 1. Generate dungeon map
//     currentMapData = dungeonGenerator.Generate(width, height);
//     
//     // 2. Determine tile variations
//     currentFrameMapData = tileVariationManager.ProcessTileMap(currentMapData);
//     
//     // 3. Set map data in level
//     level->SetMapData(currentMapData);
//     level->SetFrameMapData(currentFrameMapData);
//     
//     // 4. Generate monsters
//     std::vector<Monster*> monsters = GenerateMonsters(level, floorLevel);
//     
//     // 5. Add monsters to level
//     level->AddMonsters(monsters);
//     
//     // 6. Initialize environmental effects - REMOVED
//     // environmentalEffectsManager.Initialize();
// }
//
// // Generate monsters
// std::vector<Monster*> IntegratedDungeonSystem::GenerateMonsters(Level* level, int floorLevel) {
//     return monsterGenerator.GenerateMonsters(level, currentMapData, floorLevel);
// }
void IntegratedDungeonSystem::GenerateDungeon(bool isProcedural, Level* level, int width, int height, int roomCount, int minRoomSize,
    int maxRoomSize, WCHAR* filepath, int floorLevel)
{
    if (isProcedural == true) {
        dungeonGenerator = new ProceduralDungeonGenerator();
    }
    else {
        wstring wFileName = GetFileNameFromWCHAR(filepath);
        string fileName = WStringToString(wFileName);
        dungeonGenerator = new FileLoadDungeonGenerator(fileName);
    }
   
    
    // dungeonGenerator = new DungeonGenerator();
    
    // Generate the dungeon layout
    auto map = dungeonGenerator->Generate(width, height);
    auto renderMap = dungeonGenerator->GetTileVariations(map);
    
    // Get the generated map data
    // std::vector<std::vector<int>> mapData = dungeonGenerator.GetMapData();
    //
    // Set the map data in the level
    level->SetMapData(map);
    level->SetRenderMap(renderMap);
    
    // Generate monsters based on the dungeon layout
    std::vector<Monster*> monsters = monsterGenerator.GenerateMonsters(level, map, floorLevel); // Generate 10 monsters
        
    // Add the monsters to the level
    level->AddMonsters(monsters);
}

wstring IntegratedDungeonSystem::GetFileNameFromWCHAR(const WCHAR* fullPath)
{
    std::wstring path(fullPath);
    size_t pos = path.find_last_of(L"\\/");
    if (pos == std::wstring::npos) return path;
    return path.substr(pos + 1);
}

string IntegratedDungeonSystem::WStringToString(const std::wstring& wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], -1, &result[0], size_needed, NULL, NULL);
    return result;
}
