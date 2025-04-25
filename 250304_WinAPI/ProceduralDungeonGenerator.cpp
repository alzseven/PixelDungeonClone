#define NOMINMAX
#include "ProceduralDungeonGenerator.h"
#include <algorithm>
#include <ctime>

#include "D2DImage.h"

ProceduralDungeonGenerator::ProceduralDungeonGenerator() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

bool ProceduralDungeonGenerator::Room::intersects(const Room& other, int margin) const {
    return !(right + margin < other.left || 
             left - margin > other.right || 
             bottom + margin < other.top || 
             top - margin > other.bottom);
}

int ProceduralDungeonGenerator::Room::area() const {
    return (right - left + 1) * (bottom - top + 1);
}

int ProceduralDungeonGenerator::Room::centerX() const {
    return (left + right) / 2;
}

int ProceduralDungeonGenerator::Room::centerY() const {
    return (top + bottom) / 2;
}

std::vector<std::vector<int>> ProceduralDungeonGenerator::Generate(int width, int height) {
    // 1. 맵 초기화 (모두 벽으로)
    std::vector<std::vector<int>> map(height, std::vector<int>(width, TILE_WALL));
    
    // 2. 방 생성
    std::vector<Room> rooms = GenerateRooms(width, height, 8, 15);
    
    // 방이 너무 적으면 다시 시도
    if (rooms.size() < 8) {
        return Generate(width, height);
    }
    
    // 3. 방 노드 생성
    std::vector<RoomNode> roomNodes;
    roomNodes.reserve(rooms.size());
    for (const auto& room : rooms) {
        roomNodes.push_back(RoomNode(room, RoomNode::STANDARD));
    }
    
    // 4. 루프 레이아웃 구축 (메인 경로 + 가지)
    BuildLoopLayout(roomNodes);
    
    // 5. 방 적용 (맵에 그리기)
    for (const auto& node : roomNodes) {
        DrawRoom(map, node.room);
    }
    
    // 6. 방 연결
    ConnectRooms(map, roomNodes);
    
    // 7. 특수 방 배치
    PlaceSpecialRooms(map, roomNodes);
    
    // 8. 맵 정리
    CleanupWalls(map);
    
    return map;
}

std::vector<std::vector<int>> ProceduralDungeonGenerator::GetTileVariations(std::vector<std::vector<int>>& map) {
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

std::vector<ProceduralDungeonGenerator::Room> ProceduralDungeonGenerator::GenerateRooms(int width, int height, int minRooms, int maxRooms) {
    std::vector<Room> rooms;
    
    // 생성할 방 개수 결정
    int roomCount = GetRandomInt(minRooms, maxRooms);
    
    // 최대 시도 횟수 (무한 루프 방지)
    constexpr int MAX_ATTEMPTS = 300;
    int attempts = 0;
    
    while (rooms.size() < roomCount && attempts < MAX_ATTEMPTS) {
        // 방 크기 유형 결정 (표준, 대형, 거대)
        float roomTypeRoll = static_cast<float>(rand()) / RAND_MAX;
        int minSize = MIN_ROOM_SIZE;
        int maxSize;
        
        if (roomTypeRoll < 0.7f) {
            // 70% 확률로 표준 방
            maxSize = MAX_STANDARD_ROOM_SIZE;
        } else if (roomTypeRoll < 0.9f) {
            // 20% 확률로 대형 방
            maxSize = MAX_LARGE_ROOM_SIZE;
        } else {
            // 10% 확률로 거대 방
            maxSize = MAX_GIANT_ROOM_SIZE;
        }
        
        // 랜덤 방 크기 생성
        int roomWidth = GetRandomInt(minSize, maxSize);
        int roomHeight = GetRandomInt(minSize, maxSize);
        
        // 랜덤 방 위치 생성 (맵 경계 내)
        int left = GetRandomInt(1, width - roomWidth - 1);
        int top = GetRandomInt(1, height - roomHeight - 1);
        
        Room newRoom = {
            left,                // left
            top,                 // top
            left + roomWidth - 1,    // right
            top + roomHeight - 1     // bottom
        };
        
        // 다른 방과 겹치는지 확인
        bool overlaps = false;
        for (const auto& room : rooms) {
            if (newRoom.intersects(room, ROOM_MARGIN)) {
                overlaps = true;
                break;
            }
        }
        
        // 겹치지 않으면 방 추가
        if (!overlaps) {
            rooms.push_back(newRoom);
        }
        
        attempts++;
    }
    
    return rooms;
}

void ProceduralDungeonGenerator::BuildLoopLayout(std::vector<RoomNode>& roomNodes) {
    if (roomNodes.size() < 3) return;
    
    // 방 크기에 따라 정렬 (큰 방이 먼저 오도록)
    std::sort(roomNodes.begin(), roomNodes.end(), [](const RoomNode& a, const RoomNode& b) {
        return a.room.area() > b.room.area();
    });
    
    // 가장 큰 방을 중앙 방으로 설정
    int centerIdx = 0;
    roomNodes[centerIdx].isOnMainPath = true;
    
    // 메인 루프를 위한 방 선택 (전체 방의 약 60%)
    int mainPathCount = std::max(3, static_cast<int>(roomNodes.size() * 0.6f));
    
    // 중앙 방에서 가장 멀리 떨어진 방을 입구로 설정
    int entranceIdx = -1;
    int maxDistance = -1;
    
    for (size_t i = 1; i < roomNodes.size(); i++) {
        int distance = std::abs(roomNodes[i].room.centerX() - roomNodes[centerIdx].room.centerX()) +
                       std::abs(roomNodes[i].room.centerY() - roomNodes[centerIdx].room.centerY());
        
        if (distance > maxDistance) {
            maxDistance = distance;
            entranceIdx = i;
        }
    }
    
    roomNodes[entranceIdx].type = RoomNode::ENTRANCE;
    roomNodes[entranceIdx].isOnMainPath = true;
    
    // 입구에서 가장 멀리 떨어진 방을 출구로 설정
    int exitIdx = -1;
    maxDistance = -1;
    
    for (size_t i = 1; i < roomNodes.size(); i++) {
        if (i == entranceIdx) continue;
        
        int distance = std::abs(roomNodes[i].room.centerX() - roomNodes[entranceIdx].room.centerX()) +
                       std::abs(roomNodes[i].room.centerY() - roomNodes[entranceIdx].room.centerY());
        
        if (distance > maxDistance) {
            maxDistance = distance;
            exitIdx = i;
        }
    }
    
    roomNodes[exitIdx].type = RoomNode::EXIT;
    roomNodes[exitIdx].isOnMainPath = true;
    
    // 메인 루프 경로 생성 (입구 -> 중앙 -> 출구 경로 주변으로 원형 경로)
    std::vector<int> mainPathIndices;
    mainPathIndices.push_back(entranceIdx);
    
    // 입구와 중앙 사이의 방들 선택
    std::vector<std::pair<int, int>> candidatesEntryToCenter;
    for (size_t i = 0; i < roomNodes.size(); i++) {
        if (i == entranceIdx || i == centerIdx || i == exitIdx) continue;
        
        // 입구에서 중앙으로의 방향 벡터
        int dirX = roomNodes[centerIdx].room.centerX() - roomNodes[entranceIdx].room.centerX();
        int dirY = roomNodes[centerIdx].room.centerY() - roomNodes[entranceIdx].room.centerY();
        
        // 입구에서 현재 방으로의 방향 벡터
        int roomDirX = roomNodes[i].room.centerX() - roomNodes[entranceIdx].room.centerX();
        int roomDirY = roomNodes[i].room.centerY() - roomNodes[entranceIdx].room.centerY();
        
        // 내적이 양수면 같은 방향
        if (dirX * roomDirX + dirY * roomDirY > 0) {
            int distToEntry = std::abs(roomNodes[i].room.centerX() - roomNodes[entranceIdx].room.centerX()) +
                             std::abs(roomNodes[i].room.centerY() - roomNodes[entranceIdx].room.centerY());
            
            candidatesEntryToCenter.push_back({i, distToEntry});
        }
    }
    
    // 거리에 따라 정렬
    std::sort(candidatesEntryToCenter.begin(), candidatesEntryToCenter.end(), 
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  return a.second < b.second;
              });
    
    // 입구와 중앙 사이에 1-2개 방 선택
    int midPointCount = std::min(2, static_cast<int>(candidatesEntryToCenter.size()));
    for (int i = 0; i < midPointCount; i++) {
        mainPathIndices.push_back(candidatesEntryToCenter[i].first);
        roomNodes[candidatesEntryToCenter[i].first].isOnMainPath = true;
    }
    
    mainPathIndices.push_back(centerIdx);
    
    // 중앙과 출구 사이의 방들 선택
    std::vector<std::pair<int, int>> candidatesCenterToExit;
    for (size_t i = 0; i < roomNodes.size(); i++) {
        if (i == entranceIdx || i == centerIdx || i == exitIdx) continue;
        if (roomNodes[i].isOnMainPath) continue;
        
        // 중앙에서 출구로의 방향 벡터
        int dirX = roomNodes[exitIdx].room.centerX() - roomNodes[centerIdx].room.centerX();
        int dirY = roomNodes[exitIdx].room.centerY() - roomNodes[centerIdx].room.centerY();
        
        // 중앙에서 현재 방으로의 방향 벡터
        int roomDirX = roomNodes[i].room.centerX() - roomNodes[centerIdx].room.centerX();
        int roomDirY = roomNodes[i].room.centerY() - roomNodes[centerIdx].room.centerY();
        
        // 내적이 양수면 같은 방향
        if (dirX * roomDirX + dirY * roomDirY > 0) {
            int distToCenter = std::abs(roomNodes[i].room.centerX() - roomNodes[centerIdx].room.centerX()) +
                              std::abs(roomNodes[i].room.centerY() - roomNodes[centerIdx].room.centerY());
            
            candidatesCenterToExit.push_back({i, distToCenter});
        }
    }
    
    // 거리에 따라 정렬
    std::sort(candidatesCenterToExit.begin(), candidatesCenterToExit.end(), 
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  return a.second < b.second;
              });
    
    // 중앙과 출구 사이에 1-2개 방 선택
    midPointCount = std::min(2, static_cast<int>(candidatesCenterToExit.size()));
    for (int i = 0; i < midPointCount; i++) {
        mainPathIndices.push_back(candidatesCenterToExit[i].first);
        roomNodes[candidatesCenterToExit[i].first].isOnMainPath = true;
    }
    
    mainPathIndices.push_back(exitIdx);
    
    // 메인 경로 방들 연결
    for (size_t i = 0; i < mainPathIndices.size() - 1; i++) {
        int currentIdx = mainPathIndices[i];
        int nextIdx = mainPathIndices[i + 1];
        
        roomNodes[currentIdx].connections.push_back(nextIdx);
        roomNodes[nextIdx].connections.push_back(currentIdx);
    }
    
    // 루프 완성을 위해 출구에서 입구로 연결 (선택적)
    if (GetRandomBool(0.7f) && mainPathIndices.size() >= 4) {
        // 직접 연결하지 않고 다른 방을 통해 연결
        std::vector<int> remainingRooms;
        for (size_t i = 0; i < roomNodes.size(); i++) {
            if (!roomNodes[i].isOnMainPath) {
                remainingRooms.push_back(i);
            }
        }
        
        if (!remainingRooms.empty()) {
            // 출구와 입구 사이에 위치한 방 찾기
            int bridgeIdx = -1;
            int minDistance = INT_MAX;
            
            for (int idx : remainingRooms) {
                int distToExit = std::abs(roomNodes[idx].room.centerX() - roomNodes[exitIdx].room.centerX()) +
                                std::abs(roomNodes[idx].room.centerY() - roomNodes[exitIdx].room.centerY());
                
                int distToEntry = std::abs(roomNodes[idx].room.centerX() - roomNodes[entranceIdx].room.centerX()) +
                                 std::abs(roomNodes[idx].room.centerY() - roomNodes[entranceIdx].room.centerY());
                
                int totalDist = distToExit + distToEntry;
                
                if (totalDist < minDistance) {
                    minDistance = totalDist;
                    bridgeIdx = idx;
                }
            }
            
            if (bridgeIdx != -1) {
                roomNodes[bridgeIdx].isOnMainPath = true;
                roomNodes[exitIdx].connections.push_back(bridgeIdx);
                roomNodes[bridgeIdx].connections.push_back(exitIdx);
                roomNodes[bridgeIdx].connections.push_back(entranceIdx);
                roomNodes[entranceIdx].connections.push_back(bridgeIdx);
            }
        }
    }
    
    // 나머지 방들을 가지(branch)로 연결
    for (size_t i = 0; i < roomNodes.size(); i++) {
        if (roomNodes[i].isOnMainPath) continue;
        
        // 가장 가까운 메인 경로 방 찾기
        int nearestMainPathIdx = -1;
        int minDistance = INT_MAX;
        
        for (size_t j = 0; j < roomNodes.size(); j++) {
            if (!roomNodes[j].isOnMainPath) continue;
            
            int distance = std::abs(roomNodes[i].room.centerX() - roomNodes[j].room.centerX()) +
                          std::abs(roomNodes[i].room.centerY() - roomNodes[j].room.centerY());
            
            if (distance < minDistance) {
                minDistance = distance;
                nearestMainPathIdx = j;
            }
        }
        
        if (nearestMainPathIdx != -1) {
            roomNodes[i].connections.push_back(nearestMainPathIdx);
            roomNodes[nearestMainPathIdx].connections.push_back(i);
        }
    }
}

void ProceduralDungeonGenerator::ConnectRooms(std::vector<std::vector<int>>& map, const std::vector<RoomNode>& roomNodes) {
    // 각 방의 연결 정보를 기반으로 복도 생성
    for (size_t i = 0; i < roomNodes.size(); i++) {
        for (int connIdx : roomNodes[i].connections) {
            // 중복 연결 방지 (i < connIdx인 경우만 처리)
            if (i < connIdx) {
                // 두 방 사이에 복도 생성
                CreateCorridor(
                    map, 
                    roomNodes[i].room.centerX(), 
                    roomNodes[i].room.centerY(),
                    roomNodes[connIdx].room.centerX(),
                    roomNodes[connIdx].room.centerY()
                );
            }
        }
    }
}

void ProceduralDungeonGenerator::CreateCorridor(std::vector<std::vector<int>>& map, int x1, int y1, int x2, int y2) {
    // L자형 또는 Z자형 복도 생성
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> coinFlip(0, 2);
    
    int corridorType = coinFlip(gen);
    
    auto placeCorridor = [&](int x, int y) {
        if (map[y][x] == TILE_WALL) {
            map[y][x] = TILE_FLOOR;
            
            // 주변에 벽 배치 (복도 주변을 벽으로 감싸기)
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int newY = y + dy;
                    int newX = x + dx;
                    
                    // 맵 범위 체크
                    if (newY >= 0 && newY < map.size() && newX >= 0 && newX < map[0].size()) {
                        // 대각선 방향은 항상 벽으로
                        if (dx != 0 && dy != 0) {
                            if (map[newY][newX] == TILE_WALL) {
                                continue; // 이미 벽이면 스킵
                            }
                            if (map[newY][newX] == TILE_FLOOR) {
                                continue; // 이미 바닥이면 스킵 (다른 복도나 방과 연결)
                            }
                            map[newY][newX] = TILE_WALL;
                        }
                        // 상하좌우는 기존 타일이 벽일 경우에만 벽으로
                        else if (map[newY][newX] == TILE_WALL) {
                            continue; // 이미 벽이면 유지
                        }
                    }
                }
            }
        }
    };
    
    if (corridorType == 0) {
        // L자형 복도 (수평 먼저)
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            placeCorridor(x, y1);
        }

        for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
            placeCorridor(x2, y);
        }
    } else if (corridorType == 1) {
        // L자형 복도 (수직 먼저)
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
            placeCorridor(x1, y);
        }
        
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            placeCorridor(x, y2);
        }
    } else {
        // Z자형 복도 (중간 지점 경유)
        int midX = (x1 + x2) / 2;
        int midY = (y1 + y2) / 2;
        
        // 첫 번째 L자형 (시작점 -> 중간점)
        for (int x = std::min(x1, midX); x <= std::max(x1, midX); x++) {
            placeCorridor(x, y1);
        }
        
        for (int y = std::min(y1, midY); y <= std::max(y1, midY); y++) {
            placeCorridor(midX, y);
        }
        
        // 두 번째 L자형 (중간점 -> 끝점)
        for (int x = std::min(midX, x2); x <= std::max(midX, x2); x++) {
            placeCorridor(x, midY);
        }
        
        for (int y = std::min(midY, y2); y <= std::max(midY, y2); y++) {
            placeCorridor(x2, y);
        }
    }
}

void ProceduralDungeonGenerator::PlaceSpecialRooms(std::vector<std::vector<int>>& map, std::vector<RoomNode>& roomNodes) {
    if (roomNodes.empty()) return;
    
    // 입구 방 찾기
    int entranceIdx = -1;
    for (size_t i = 0; i < roomNodes.size(); i++) {
        if (roomNodes[i].type == RoomNode::ENTRANCE) {
            entranceIdx = i;
            break;
        }
    }
    
    if (entranceIdx == -1) return;
    
    // 출구 방 찾기
    int exitIdx = -1;
    for (size_t i = 0; i < roomNodes.size(); i++) {
        if (roomNodes[i].type == RoomNode::EXIT) {
            exitIdx = i;
            break;
        }
    }
    
    if (exitIdx == -1) return;
    
    // 입구 배치
    int entranceX = roomNodes[entranceIdx].room.centerX();
    int entranceY = roomNodes[entranceIdx].room.centerY();
    map[entranceY][entranceX] = TILE_ENTRANCE;
    ascInd = entranceY * TILE_X + entranceX;
    
    // 출구 배치
    int exitX = roomNodes[exitIdx].room.centerX();
    int exitY = roomNodes[exitIdx].room.centerY();
    map[exitY][exitX] = TILE_EXIT;
    descInd = exitY * TILE_X + exitX;
}

void ProceduralDungeonGenerator::CleanupWalls(std::vector<std::vector<int>>& map) {
    int height = map.size();
    int width = map[0].size();
    
    // 임시 맵에 변경사항을 저장
    std::vector<std::vector<int>> tempMap = map;
    
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (map[y][x] == TILE_WALL) {
                // 주변 8방향에 바닥이 하나라도 있는지 확인
                bool hasAdjacentFloor = false;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue;
                        if (map[y + dy][x + dx] == TILE_FLOOR || 
                            map[y + dy][x + dx] == TILE_DOOR || 
                            map[y + dy][x + dx] == TILE_ENTRANCE || 
                            map[y + dy][x + dx] == TILE_EXIT ||
                            map[y + dy][x + dx] == TILE_HIDDEN_DOOR) {
                            hasAdjacentFloor = true;
                            break;
                        }
                    }
                    if (hasAdjacentFloor) break;
                }
                
                // 주변에 바닥이 없으면 불필요한 벽이므로 제거
                if (!hasAdjacentFloor) {
                    tempMap[y][x] = TILE_NONE;
                }
            }
        }
    }
    
    // 최외각 벽 정리
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // 맵 테두리만 처리
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                if (map[y][x] == TILE_WALL) {
                    // 테두리 벽이 방의 일부인지 확인
                    bool isPartOfRoom = false;
                    
                    // 테두리 벽의 내부 방향 확인
                    int checkY = (y == 0) ? 1 : (y == height - 1) ? height - 2 : y;
                    int checkX = (x == 0) ? 1 : (x == width - 1) ? width - 2 : x;
                    
                    // 내부 방향에 바닥이 있는지 확인
                    if (map[checkY][checkX] == TILE_FLOOR || 
                        map[checkY][checkX] == TILE_DOOR || 
                        map[checkY][checkX] == TILE_ENTRANCE || 
                        map[checkY][checkX] == TILE_EXIT ||
                        map[checkY][checkX] == TILE_HIDDEN_DOOR) {
                        isPartOfRoom = true;
                    }
                    
                    // 방의 일부가 아니면 제거
                    if (!isPartOfRoom) {
                        tempMap[y][x] = TILE_NONE;
                    }
                }
            }
        }
    }
    
    // 변경사항을 원본 맵에 적용
    map = tempMap;
}

void ProceduralDungeonGenerator::DrawRoom(std::vector<std::vector<int>>& map, const Room& room, int tileType) {
    for (int y = room.top; y <= room.bottom; y++) {
        for (int x = room.left; x <= room.right; x++) {
            map[y][x] = tileType;
        }
    }
}

int ProceduralDungeonGenerator::GetRandomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

bool ProceduralDungeonGenerator::GetRandomBool(float probability) {
    return (static_cast<float>(rand()) / RAND_MAX) < probability;
}

// 개선된 DetermineWallVariation 함수 - 내부 코너 감지 추가
// int ProceduralDungeonGenerator::DetermineWallVariation(const std::vector<std::vector<int>>& map, int x, int y) {
//     int height = map.size();
//     int width = map[0].size();
//     
//     auto isWall = [&](int x, int y) -> bool {
//         if (x < 0 || x >= width || y < 0 || y >= height) return true;
//         return map[y][x] == TILE_WALL || map[y][x] == TILE_NONE;
//     };
//     
//     auto isFloor = [&](int x, int y) -> bool {
//         if (x < 0 || x >= width || y < 0 || y >= height) return false;
//         return map[y][x] == TILE_FLOOR || 
//                map[y][x] == TILE_DOOR || 
//                map[y][x] == TILE_ENTRANCE || 
//                map[y][x] == TILE_EXIT ||
//                map[y][x] == TILE_HIDDEN_DOOR;
//     };
//     
//     bool hasFloorAbove = isFloor(x, y-1);
//     bool hasFloorBelow = isFloor(x, y+1);
//     bool hasFloorLeft = isFloor(x-1, y);
//     bool hasFloorRight = isFloor(x+1, y);
//     
//     bool hasFloorTopLeft = isFloor(x-1, y-1);
//     bool hasFloorTopRight = isFloor(x+1, y-1);
//     bool hasFloorBottomLeft = isFloor(x-1, y+1);
//     bool hasFloorBottomRight = isFloor(x+1, y+1);
//     
//     // 내부 모서리 벽 결정 (세 방향이 바닥인 경우)
//     // 이 부분이 기존 코드에 없었던 내부 코너 감지 로직입니다
//     if (hasFloorAbove && hasFloorLeft && hasFloorTopLeft) {
//         return TILE_WALL_INNER_CORNER_TL; // 내부 좌상단 모서리 (새로운 타일 타입 필요)
//     }
//     if (hasFloorAbove && hasFloorRight && hasFloorTopRight) {
//         return TILE_WALL_INNER_CORNER_TR; // 내부 우상단 모서리 (새로운 타일 타입 필요)
//     }
//     if (hasFloorBelow && hasFloorLeft && hasFloorBottomLeft) {
//         return TILE_WALL_INNER_CORNER_BL; // 내부 좌하단 모서리 (새로운 타일 타입 필요)
//     }
//     if (hasFloorBelow && hasFloorRight && hasFloorBottomRight) {
//         return TILE_WALL_INNER_CORNER_BR; // 내부 우하단 모서리 (새로운 타일 타입 필요)
//     }
//     
//     // 외부 모서리 벽 결정 (두 방향이 바닥인 경우)
//     // 기존 코드는 여기서 추가 검사를 했지만, 실제로는 불필요합니다
//     if (hasFloorBelow && hasFloorRight && !hasFloorBottomRight) {
//         return TILE_WALL_CORNER_BR;
//     }
//     if (hasFloorBelow && hasFloorLeft && !hasFloorBottomLeft) {
//         return TILE_WALL_CORNER_BL;
//     }
//     if (hasFloorAbove && hasFloorRight && !hasFloorTopRight) {
//         return TILE_WALL_CORNER_TR;
//     }
//     if (hasFloorAbove && hasFloorLeft && !hasFloorTopLeft) {
//         return TILE_WALL_CORNER_TL;
//     }
//     
//     // 일반 벽 결정 (한 방향만 바닥인 경우)
//     if (hasFloorAbove && !hasFloorBelow && !hasFloorLeft && !hasFloorRight) {
//         return TILE_WALL_BOTTOM;
//     }
//     if (!hasFloorAbove && hasFloorBelow && !hasFloorLeft && !hasFloorRight) {
//         return TILE_WALL_TOP;
//     }
//     if (!hasFloorAbove && !hasFloorBelow && hasFloorLeft && !hasFloorRight) {
//         return TILE_WALL_RIGHT;
//     }
//     if (!hasFloorAbove && !hasFloorBelow && !hasFloorLeft && hasFloorRight) {
//         return TILE_WALL_LEFT;
//     }
//     
//     return TILE_WALL;
// }
int ProceduralDungeonGenerator::DetermineWallVariation(const std::vector<std::vector<int>>& map, int x, int y) {
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

int ProceduralDungeonGenerator::DetermineFloorVariation(const std::vector<std::vector<int>>& map, int x, int y) {
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

void ProceduralDungeonGenerator::RenderWallTile(D2DImage* image, int x, int y, int tileX, int tileY, float scale, const std::vector<std::vector<int>>& map) {
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
