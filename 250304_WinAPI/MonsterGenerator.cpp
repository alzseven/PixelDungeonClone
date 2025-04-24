// MonsterGenerator.cpp
#include "MonsterGenerator.h"
#include "Monster.h"
#include "Level.h"
#include <random>
#include <algorithm>
#include <ctime>

MonsterGenerator::MonsterGenerator() {
    // 랜덤 시드 초기화
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // 지역별 몬스터 정의 초기화
    regionMonsters.resize(5); // 5개 지역 (하수도, 감옥, 동굴, 도시, 지옥)
    
    // 하수도 지역 몬스터
    regionMonsters[0].commonMonsters = {RAT, CRAB};
    regionMonsters[0].rareMonsters = {GNOLL};
    regionMonsters[0].bossMonsters = {GNOLL}; // 임시 보스
    
    // 감옥 지역 몬스터
    regionMonsters[1].commonMonsters = {SKELETON, THIEF};
    regionMonsters[1].rareMonsters = {GNOLL};
    regionMonsters[1].bossMonsters = {SKELETON}; // 임시 보스
    
    // 동굴 지역 몬스터
    regionMonsters[2].commonMonsters = {BAT, BRUTE};
    regionMonsters[2].rareMonsters = {SHAMAN};
    regionMonsters[2].bossMonsters = {BRUTE}; // 임시 보스
    
    // 도시 지역 몬스터
    regionMonsters[3].commonMonsters = {ELEMENTAL, MONK};
    regionMonsters[3].rareMonsters = {WARLOCK, GOLEM};
    regionMonsters[3].bossMonsters = {GOLEM}; // 임시 보스
    
    // 지옥 지역 몬스터
    regionMonsters[4].commonMonsters = {WARLOCK, ELEMENTAL};
    regionMonsters[4].rareMonsters = {DEMON};
    regionMonsters[4].bossMonsters = {DEMON}; // 임시 보스
}

MonsterGenerator::~MonsterGenerator() {
}

// 몬스터 생성 및 배치 메서드
std::vector<Monster*> MonsterGenerator::GenerateMonsters(Level* level, const std::vector<std::vector<int>>& map, int floorLevel) {
    std::vector<Monster*> monsters;
    
    // 맵 크기 계산
    int mapSize = map.size() * map[0].size();
    
    // 몬스터 수 결정
    int monsterCount = DetermineMonsterCount(floorLevel, mapSize);
    
    // 몬스터 유형 결정
    std::vector<MonsterType> monsterTypes = DetermineMonsterTypes(floorLevel, monsterCount);
    
    // 몬스터 배치 위치 결정
    std::vector<FPOINT> monsterPositions = DetermineMonsterPositions(level, map, monsterCount);
    
    // 몬스터 생성 및 배치
    for (int i = 0; i < monsterCount && i < monsterPositions.size(); i++) {
        Monster* monster = CreateMonster(monsterTypes[i], monsterPositions[i], 3.0f); // 기본 속도 3.0f
        if (monster) {
            monsters.push_back(monster);
        }
    }
    
    return monsters;
}

// 몬스터 생성 함수
Monster* MonsterGenerator::CreateMonster(MonsterType type, FPOINT position, float speed) {
    // 몬스터 생성
    Monster* monster = new Monster(position, 1000.f, 15, 4, 3);
    
    // 몬스터 유형에 따른 추가 설정 (향후 확장)
    // 현재는 기본 Monster 클래스만 사용
    
    return monster;
}

// 몬스터 배치 위치 결정 함수
std::vector<FPOINT> MonsterGenerator::DetermineMonsterPositions(Level* level, const std::vector<std::vector<int>>& map, int count) {
    std::vector<FPOINT> positions;
    
    // 바닥 타일 위치 수집
    std::vector<std::pair<int, int>> floorTiles;
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] == 1) { // TILE_FLOOR
                floorTiles.push_back({x, y});
            }
        }
    }
    
    // 입구/출구 위치 찾기
    std::pair<int, int> entrancePos = {-1, -1};
    std::pair<int, int> exitPos = {-1, -1};
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] == 3) { // TILE_ENTRANCE
                entrancePos = {x, y};
            } else if (map[y][x] == 4) { // TILE_EXIT
                exitPos = {x, y};
            }
        }
    }
    
    // 입구/출구 주변 타일 제외 (플레이어 시작 지점 보호)
    const int SAFE_RADIUS = 3; // 안전 반경
    std::vector<std::pair<int, int>> safeTiles = floorTiles;
    
    if (entrancePos.first != -1 && entrancePos.second != -1) {
        safeTiles.erase(
            std::remove_if(safeTiles.begin(), safeTiles.end(), 
                [entrancePos, SAFE_RADIUS](const std::pair<int, int>& pos) {
                    return std::abs(pos.first - entrancePos.first) + std::abs(pos.second - entrancePos.second) <= SAFE_RADIUS;
                }
            ),
            safeTiles.end()
        );
    }
    
    // 남은 타일이 너무 적으면 원래 타일 사용
    if (safeTiles.size() < count) {
        safeTiles = floorTiles;
    }
    
    // 랜덤하게 위치 선택
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(safeTiles.begin(), safeTiles.end(), gen);
    
    // 필요한 수만큼 위치 선택
    int posCount = (std::min)(count, static_cast<int>(safeTiles.size()));
    for (int i = 0; i < posCount; i++) {
        // 타일 좌표를 실제 위치로 변환
        FPOINT pos = level->GetPosByGridIndex(safeTiles[i].first, safeTiles[i].second);
        positions.push_back(pos);
    }
    
    return positions;
}

// 지역 및 난이도에 따른 몬스터 수 결정 함수
int MonsterGenerator::DetermineMonsterCount(int floorLevel, int mapSize) {
    // 기본 몬스터 수 + 층 레벨에 따른 추가 몬스터
    int baseCount = BASE_MONSTER_COUNT;
    int additionalCount = static_cast<int>(floorLevel * MONSTER_COUNT_SCALE);
    
    // 맵 크기에 따른 조정
    float mapSizeFactor = mapSize / 1000.0f; // 맵 크기 1000을 기준으로 조정
    int adjustedCount = static_cast<int>((baseCount + additionalCount) * mapSizeFactor);
    
    // 최소 몬스터 수 보장
    return (std::max)(2, adjustedCount);
}

// 지역 및 난이도에 따른 몬스터 유형 결정 함수
std::vector<MonsterGenerator::MonsterType> MonsterGenerator::DetermineMonsterTypes(int floorLevel, int count) {
    std::vector<MonsterType> types;
    
    // 지역 결정 (층 레벨에 따라)
    int region = (std::min)(4, floorLevel / 5); // 5층마다 새로운 지역
    
    for (int i = 0; i < count; i++) {
        float roll = static_cast<float>(rand()) / RAND_MAX;
        
        if (roll < BOSS_MONSTER_CHANCE && !regionMonsters[region].bossMonsters.empty()) {
            // 보스 몬스터 (낮은 확률)
            int bossIndex = GetRandomInt(0, regionMonsters[region].bossMonsters.size() - 1);
            types.push_back(regionMonsters[region].bossMonsters[bossIndex]);
        } else if (roll < BOSS_MONSTER_CHANCE + RARE_MONSTER_CHANCE && !regionMonsters[region].rareMonsters.empty()) {
            // 희귀 몬스터
            int rareIndex = GetRandomInt(0, regionMonsters[region].rareMonsters.size() - 1);
            types.push_back(regionMonsters[region].rareMonsters[rareIndex]);
        } else if (!regionMonsters[region].commonMonsters.empty()) {
            // 일반 몬스터
            int commonIndex = GetRandomInt(0, regionMonsters[region].commonMonsters.size() - 1);
            types.push_back(regionMonsters[region].commonMonsters[commonIndex]);
        } else {
            // 기본 몬스터 (RAT)
            types.push_back(RAT);
        }
    }
    
    return types;
}

// 랜덤 정수 생성 함수
int MonsterGenerator::GetRandomInt(int min, int max) {
    if (min > max) return min;
    return min + rand() % (max - min + 1);
}

// 랜덤 불리언 생성 함수
bool MonsterGenerator::GetRandomBool(float probability) {
    return (static_cast<float>(rand()) / RAND_MAX) < probability;
}
