#include "FileLoadDungeonGenerator.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "config.h"
#include "D2DImage.h"

FileLoadDungeonGenerator::FileLoadDungeonGenerator(const std::string& filePath)
    : filePath(filePath) {
    LoadDungeonData();
}

std::vector<std::vector<int>> FileLoadDungeonGenerator::Generate(int width, int height) {
    if (!LoadDungeonData() || !ValidateDungeonData(width, height)) {
        return std::vector<std::vector<int>>(height, std::vector<int>(width, TILE_WALL));
    }
    
    // 던전 데이터를 요청된 크기로 조정
    std::vector<std::vector<int>> map(height, std::vector<int>(width, TILE_WALL));
    
    // 원본 데이터를 중앙에 복사
    int startY = (height - dungeonData.size()) / 2;
    int startX = (width - dungeonData[0].size()) / 2;
    
    for (size_t y = 0; y < dungeonData.size(); y++) {
        for (size_t x = 0; x < dungeonData[0].size(); x++) {
            if (startY + y < height && startX + x < width) {
                map[startY + y][startX + x] = dungeonData[y][x];
            }
        }
    }
    
    // 특수 방 배치
    // PlaceSpecialRooms(map);
    
    // 문 배치
    // PlaceDoorsAtRoomBorders(map);
    
    return map;
}

std::vector<std::vector<int>> FileLoadDungeonGenerator::GetTileVariations(std::vector<std::vector<int>>& map) {
    int height = map.size();
    int width = map[0].size();
    
    // 임시 맵에 변형을 저장
    std::vector<std::vector<int>> tempMap = map;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y][x] == TILE_WALL) {
                tempMap[y][x] = DetermineWallVariation(map, x, y);
            }
            else if (map[y][x] == TILE_FLOOR) {
                tempMap[y][x] = DetermineFloorVariation(map, x, y);
            }
        }
    }
    
    return tempMap;
}

bool FileLoadDungeonGenerator::LoadDungeonData() {
    HANDLE hFile = CreateFile(
        std::wstring(filePath.begin(), filePath.end()).c_str(),
        GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    // 파일 크기 계산
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return false;
    }
    
    // 파일 데이터 읽기
    std::vector<Map> tileData(fileSize / sizeof(Map));
    DWORD bytesRead;
    if (!ReadFile(hFile, tileData.data(), fileSize, &bytesRead, NULL)) {
        CloseHandle(hFile);
        return false;
    }
    
    CloseHandle(hFile);
    
    // Map 배열을 2차원 벡터로 변환
    dungeonData.clear();
    dungeonData.resize(TILE_Y, std::vector<int>(TILE_X, TILE_WALL));

    for (int i=0; i<TILE_X * TILE_Y; ++i)
    {
        if (i < tileData.size())
        {
            int y = i / TILE_Y;
            int x = i % TILE_X;
            dungeonData[y][x] = tileData[i].type;
        }
        
    }
    
    // for (int y = 0; y < TILE_Y; y++) {
    //     for (int x = 0; x < TILE_X; x++) {
    //         int index = y * TILE_X + x;
    //         if (index < tileData.size()) {
    //             dungeonData[y][x] = tileData[index].type;
    //         }
    //     }
    // }
    
    return !dungeonData.empty();
}

bool FileLoadDungeonGenerator::ValidateDungeonData(int expectedWidth, int expectedHeight) {
    if (dungeonData.empty()) {
        return false;
    }
    
    // 모든 행이 같은 너비를 가지는지 확인
    int width = dungeonData[0].size();
    for (const auto& row : dungeonData) {
        if (row.size() != width) {
            return false;
        }
    }
    
    // 요청된 크기와 비교
    return width <= expectedWidth && dungeonData.size() <= expectedHeight;
}

int FileLoadDungeonGenerator::DetermineWallVariation(const std::vector<std::vector<int>>& map, int x, int y) {
    int height = map.size();
    int width = map[0].size();
    
    auto isFloor = [&](int x, int y) -> bool {
        if (x < 0 || x >= width || y < 0 || y >= height) return false;
        return map[y][x] == TILE_FLOOR || 
               map[y][x] == TILE_DOOR || 
               map[y][x] == TILE_ENTRANCE || 
               map[y][x] == TILE_EXIT ||
               map[y][x] == TILE_HIDDEN_DOOR;
    };
    
    // 비트마스크 생성: 8개 방향의 타일 상태를 하나의 값으로 압축
    // 0b00000000: 시계 방향으로 8방향 표현 (상, 우상, 우, 우하, 하, 좌하, 좌, 좌상)
    int mask = 0;
    if (isFloor(x, y-1)) mask |= 0b00000001; // 상
    if (isFloor(x+1, y-1)) mask |= 0b00000010; // 우상
    if (isFloor(x+1, y)) mask |= 0b00000100; // 우
    if (isFloor(x+1, y+1)) mask |= 0b00001000; // 우하
    if (isFloor(x, y+1)) mask |= 0b00010000; // 하
    if (isFloor(x-1, y+1)) mask |= 0b00100000; // 좌하
    if (isFloor(x-1, y)) mask |= 0b01000000; // 좌
    if (isFloor(x-1, y-1)) mask |= 0b10000000; // 좌상
    
    // 비트마스크 값에 따라 타일 타입 결정
    switch (mask) {
        // 내부 모서리
        case 0b10000011: return TILE_WALL_INNER_CORNER_TL; // 상+좌+좌상
        case 0b00000111: return TILE_WALL_INNER_CORNER_TR; // 상+우+우상
        case 0b01110000: return TILE_WALL_INNER_CORNER_BL; // 하+좌+좌하
        case 0b00011100: return TILE_WALL_INNER_CORNER_BR; // 하+우+우하
        
        // 외부 모서리
        case 0b01000001: return TILE_WALL_CORNER_TL; // 상+좌 (좌상X)
        case 0b00000101: return TILE_WALL_CORNER_TR; // 상+우 (우상X)
        case 0b01010000: return TILE_WALL_CORNER_BL; // 하+좌 (좌하X)
        case 0b00010100: return TILE_WALL_CORNER_BR; // 하+우 (우하X)
        
        // 일반 벽
        case 0b00000001: return TILE_WALL_BOTTOM; // 상
        case 0b00010000: return TILE_WALL_TOP; // 하
        case 0b01000000: return TILE_WALL_RIGHT; // 좌
        case 0b00000100: return TILE_WALL_LEFT; // 우
        
        // 필요에 따라 더 많은 케이스 추가 가능
        
        default: return TILE_WALL; // 기본 벽
    }
}

int FileLoadDungeonGenerator::DetermineFloorVariation(const std::vector<std::vector<int>>& map, int x, int y) {
    // 벽과의 거리에 따라 바닥 타일 결정
    bool isNearWall = false;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int newY = y + dy;
            int newX = x + dx;
            if (newY >= 0 && newY < map.size() && newX >= 0 && newX < map[0].size()) {
                if (map[newY][newX] == TILE_WALL || map[newY][newX] == TILE_NONE) {
                    isNearWall = true;
                    break;
                }
            }
        }
        if (isNearWall) break;
    }
    
    // 랜덤으로 바닥 타일 결정
    float random = static_cast<float>(rand()) / RAND_MAX;
    
    if (!isNearWall) {
        if (random < 0.15f) return TILE_FLOOR_FANCY;
        if (random < 0.25f) return TILE_FLOOR_CRACKED;
        if (random < 0.35f) return TILE_FLOOR_MOSSY;
    } else {
        if (random < 0.1f) return TILE_FLOOR_CRACKED;
        if (random < 0.2f) return TILE_FLOOR_MOSSY;
    }
    
    return TILE_FLOOR_NORMAL;
}

void FileLoadDungeonGenerator::PlaceSpecialRooms(std::vector<std::vector<int>>& map) {
    int height = map.size();
    int width = map[0].size();
    
    // 입구와 출구를 배치할 수 있는 위치 찾기
    std::vector<std::pair<int, int>> possibleEntrancePositions;
    std::vector<std::pair<int, int>> possibleExitPositions;
    
    // 맵의 가장자리를 따라 가능한 위치 찾기
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (map[y][x] == TILE_FLOOR) {
                // 주변에 벽이 있는지 확인
                bool hasAdjacentWall = false;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue;
                        if (map[y + dy][x + dx] == TILE_WALL) {
                            hasAdjacentWall = true;
                            break;
                        }
                    }
                    if (hasAdjacentWall) break;
                }
                
                if (hasAdjacentWall) {
                    // 맵의 상단이나 좌측에 가까운 위치는 입구로
                    if (y < height / 2 || x < width / 2) {
                        possibleEntrancePositions.push_back({x, y});
                    }
                    // 맵의 하단이나 우측에 가까운 위치는 출구로
                    else {
                        possibleExitPositions.push_back({x, y});
                    }
                }
            }
        }
    }
    
    // 입구 배치
    if (!possibleEntrancePositions.empty()) {
        int entranceIdx = rand() % possibleEntrancePositions.size();
        auto pos = possibleEntrancePositions[entranceIdx];
        map[pos.second][pos.first] = TILE_ENTRANCE;
        ascInd = pos.second * TILE_X + pos.first;
    }
    
    // 출구 배치
    if (!possibleExitPositions.empty()) {
        int exitIdx = rand() % possibleExitPositions.size();
        auto pos = possibleExitPositions[exitIdx];
        map[pos.second][pos.first] = TILE_EXIT;
        descInd = pos.second * TILE_X + pos.first;
    }
}

void FileLoadDungeonGenerator::PlaceDoorsAtRoomBorders(std::vector<std::vector<int>>& map) {
    int height = map.size();
    int width = map[0].size();
    
    // 임시 맵에 변경사항을 저장
    std::vector<std::vector<int>> tempMap = map;
    
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (IsValidDoorLocation(map, x, y)) {
                // 70% 확률로 문 배치
                if (GetRandomBool(0.7f)) {
                    tempMap[y][x] = TILE_DOOR;
                }
            }
        }
    }
    
    // 변경사항을 원본 맵에 적용
    map = tempMap;
}

bool FileLoadDungeonGenerator::IsValidDoorLocation(const std::vector<std::vector<int>>& map, int x, int y) const {
    // 현재 위치가 벽이어야 함
    if (map[y][x] != TILE_WALL) return false;
    
    // 수직 복도 체크
    bool isVerticalCorridor = (map[y-1][x] == TILE_FLOOR && map[y+1][x] == TILE_FLOOR &&
                             map[y][x-1] == TILE_WALL && map[y][x+1] == TILE_WALL);
                             
    // 수평 복도 체크
    bool isHorizontalCorridor = (map[y][x-1] == TILE_FLOOR && map[y][x+1] == TILE_FLOOR &&
                               map[y-1][x] == TILE_WALL && map[y+1][x] == TILE_WALL);
    
    return isVerticalCorridor || isHorizontalCorridor;
}

bool FileLoadDungeonGenerator::GetRandomBool(float probability) const {
    return (static_cast<float>(rand()) / RAND_MAX) < probability;
}

void FileLoadDungeonGenerator::RenderWallTile(D2DImage* image, int x, int y, int tileX, int tileY, float scale, const std::vector<std::vector<int>>& map) {
    // 벽 타일 렌더링 정보 가져오기
    WallTileRenderer::WallRenderInfo renderInfo = wallTileRenderer.GetWallRenderInfo(map, x, y);
    
    // 각 부분 타일 렌더링
    for (int i = 0; i < 16; ++i)
    {
        if (i < renderInfo.subTiles.size()) {
            const auto& subTile = renderInfo.subTiles[i];
            // Rest of the code
           
       
        
            // 부분 타일의 소스 위치
            int srcX = subTile.source.x;
            int srcY = subTile.source.y;
        
            // 부분 타일의 대상 위치
            int destX = tileX + subTile.offsetX * scale;
            int destY = tileY + subTile.offsetY * scale;
        
            // 부분 타일 렌더링
            image->RenderFrameScale(
                destX, destY,
                scale, scale,
                srcX, srcY
            );
        }
    }
}
