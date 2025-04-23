#include "WallTileRenderer.h"
#include <algorithm>
#include <set>

WallTileRenderer::WallTileRenderer() {
    // 부분 타일 위치 초기화
    InitializeSubTilePositions();
}


/*좌상단 (TL): 1 (2^0)
상단 (T): 2 (2^1)
우상단 (TR): 4 (2^2)
좌측 (L): 8 (2^3)
우측 (R): 16 (2^4)
좌하단 (BL): 32 (2^5)
하단 (B): 64 (2^6)
우하단 (BR): 128 (2^7)*/
namespace WallPatternCodes {
    // 4비트 패턴 코드 (C# 코드에서 가져옴)
    std::set<int> wallTop = { 0b1111, 0b0110, 0b0011, 0b0010, 0b1010, 0b1100, 0b1110, 0b1011, 0b0111 };
    std::set<int> wallSideLeft = { 0b0100 };
    std::set<int> wallSideRight = { 0b0001 };
    std::set<int> wallBottm = { 0b1000 };
    std::set<int> wallFull = { 0b1101, 0b0101, 0b1101, 0b1001 }; // 중복 코드 0b1101 있음

    // 8비트 패턴 코드 (C# 코드에서 가져옴)
    std::set<int> wallInnerCornerDownLeft = { /* ... C# 코드의 0b11110001 등의 값들을 여기에 나열 ... */ };
    std::set<int> wallInnerCornerDownRight = { /* ... C# 코드의 0b11000111 등의 값들을 여기에 나열 ... */ };
    std::set<int> wallDiagonalCornerDownLeft = { 0b01000000 };
    std::set<int> wallDiagonalCornerDownRight = { 0b00000001 };
    std::set<int> wallDiagonalCornerUpLeft = { 0b00010000, 0b01010000 };
    std::set<int> wallDiagonalCornerUpRight = { 0b00000100, 0b00000101 };
    std::set<int> wallFullEightDirections = { /* ... C# 코드의 0b00010100 등의 값들을 여기에 나열 ... */ };
    std::set<int> wallBottmEightDirections = { 0b01000001 };

    // 필요하다면 다른 HashSet들도 추가
}
void WallTileRenderer::InitializeSubTilePositions() {
    
    std::array<TilePosition, 4> standardWallTiles = {{ {0, 8}, {1, 8}, {0, 9}, {1, 9} }}; // 일반 벽
    std::array<TilePosition, 4> topEdgeTiles = {{ {0, 12}, {1, 12}, {0, 13}, {1, 13} }}; // 상단 벽 (아래 바닥)
    std::array<TilePosition, 4> bottomEdgeTiles = {{ {0, 25}, {1, 25}, {0, 20}, {1, 20} }}; // 하단 벽 (위 바닥)
    std::array<TilePosition, 4> leftEdgeTiles = {{ {0, 20}, {5, 20}, {0, 20}, {5, 21} }}; // 좌측 벽 (우측 바닥)
    std::array<TilePosition, 4> rightEdgeTiles = {{ {24, 18}, {0, 20}, {24, 19}, {0, 20} }}; // 우측 벽 (좌측 바닥)

    std::array<TilePosition, 4> externalCornerTLTiles = {{ {20, 19}, {0, 20}, {0, 20}, {0, 20} }}; // 외부 좌상단
    std::array<TilePosition, 4> externalCornerTRTiles = {{ {0, 20}, {19, 19}, {0, 20}, {0, 20} }}; // 외부 우상단
    std::array<TilePosition, 4> externalCornerBLTiles = {{ {24, 18}, {0, 20}, {24, 19}, {0, 20} }}; // 외부 좌하단
    std::array<TilePosition, 4> externalCornerBRTiles = {{ {0, 20}, {5, 20}, {0, 20}, {5, 21} }}; // 외부 우하단

    std::array<TilePosition, 4> innerCornerTLTiles = {{ {4, 25}, {1, 25}, {24, 19}, {0, 20} }}; // 내부 좌상단
    std::array<TilePosition, 4> innerCornerTRTiles = {{ {1, 25}, {3, 25}, {0, 20}, {5, 21} }}; // 내부 우상단
    std::array<TilePosition, 4> innerCornerBLTiles = {{ {0, 20}, {0, 20}, {24, 19}, {5, 21} }}; // 내부 좌하단 (좌표 수정 필요)
    std::array<TilePosition, 4> innerCornerBRTiles = {{ {0, 20}, {0, 20}, {24, 19}, {5, 21} }}; // 내부 우하단 (좌표 수정 필요)

    // TODO: wallFull, wallFullEightDirections 등 C# 코드에 정의된 다른 모든 HashSet 이름에 해당하는
    //       시각적 타입에 대한 부분 타일 좌표 배열(예: wallFullTiles, wallFullEightDirectionsTiles 등)을
    //       여러분의 타일셋을 보고 정의해야 합니다.


    // C# 코드의 HashSet들을 순회하며 patternSubTileMap 채우기

    // wallTop에 해당하는 패턴 코드들
    for (int pattern : WallPatternCodes::wallTop) {
        patternSubTileMap[pattern] = topEdgeTiles; // 또는 wallTopTiles 등 정의된 배열 사용
    }

    // wallSideLeft에 해당하는 패턴 코드들
    for (int pattern : WallPatternCodes::wallSideLeft) {
        patternSubTileMap[pattern] = leftEdgeTiles; // 또는 wallSideLeftTiles 등
    }

    // wallSideRight에 해당하는 패턴 코드들
     for (int pattern : WallPatternCodes::wallSideRight) {
        patternSubTileMap[pattern] = rightEdgeTiles; // 또는 wallSideRightTiles 등
    }

    // wallBottm에 해당하는 패턴 코드들
     for (int pattern : WallPatternCodes::wallBottm) {
        patternSubTileMap[pattern] = bottomEdgeTiles; // 또는 wallBottomTiles 등
    }

    // wallInnerCornerDownLeft에 해당하는 패턴 코드들
    for (int pattern : WallPatternCodes::wallInnerCornerDownLeft) {
        patternSubTileMap[pattern] = innerCornerBLTiles; // 이름에 맞게 내부 좌하단 타일 사용
    }

    // wallInnerCornerDownRight에 해당하는 패턴 코드들
     for (int pattern : WallPatternCodes::wallInnerCornerDownRight) {
        patternSubTileMap[pattern] = innerCornerBRTiles; // 이름에 맞게 내부 우하단 타일 사용
    }

    // wallDiagonalCornerDownLeft (C# 코드 이름과 역할 매칭 확인 필요)
    // 이름상 "대각선 아래 왼쪽 코너"는 바닥이 좌하단 대각선에 있을 때의 외부 코너일 수 있습니다.
    // C++ 예시에서는 WALL_CORNER_BL (바닥이 우상단 대각선)과 WALL_CORNER_BR (바닥이 좌상단 대각선)이 외부 코너였습니다.
    // C# 코드의 이름이 헷갈릴 수 있으므로, 패턴 코드와 결과 타일을 보며 직접 확인하는 것이 좋습니다.
    // 일단 이름 그대로 "DownLeft" 외부 코너 스프라이트가 있다고 가정합니다.
    for (int pattern : WallPatternCodes::wallDiagonalCornerDownLeft) {
        patternSubTileMap[pattern] = externalCornerTLTiles; // 패턴 0b01000000(TL 바닥) -> External TL
    }

    // wallDiagonalCornerDownRight (C# 코드 이름과 역할 매칭 확인 필요)
     for (int pattern : WallPatternCodes::wallDiagonalCornerDownRight) {
        patternSubTileMap[pattern] = externalCornerBRTiles; // 패턴 0b00000001(BR 바닥) -> External BR
    }

    // wallDiagonalCornerUpLeft (C# 코드 이름과 역할 매칭 확인 필요)
     for (int pattern : WallPatternCodes::wallDiagonalCornerUpLeft) {
        patternSubTileMap[pattern] = externalCornerBLTiles; // 패턴 0b00010000(BL 바닥) -> External BL
    }

    // wallDiagonalCornerUpRight (C# 코드 이름과 역할 매칭 확인 필요)
     for (int pattern : WallPatternCodes::wallDiagonalCornerUpRight) {
        patternSubTileMap[pattern] = externalCornerTRTiles; // 패턴 0b00000100(TR 바닥) -> External TR
    }

}

bool WallTileRenderer::IsWall(const std::vector<std::vector<int>>& map, int x, int y) {
    if (x < 0 || x >= map[0].size() || y < 0 || y >= map.size()) {
        return true; // 맵 경계 밖은 벽으로 처리
    }
    return map[y][x] == 0 || map[y][x] == 6; // TILE_WALL
}

bool WallTileRenderer::IsFloor(const std::vector<std::vector<int>>& map, int x, int y) {
    if (x < 0 || x >= map[0].size() || y < 0 || y >= map.size()) {
        return false; // 맵 경계 밖은 바닥이 아님
    }
    return map[y][x] == 1 || // TILE_FLOOR
           map[y][x] == 2 || // TILE_DOOR
           map[y][x] == 3 || // TILE_ENTRANCE
           map[y][x] == 4 || // TILE_EXIT
           map[y][x] == 5;   // TILE_HIDDEN_DOOR
}

WallTileRenderer::WallType WallTileRenderer::DetermineWallType(const std::vector<std::vector<int>>& map, int x, int y) {
// 주변 8칸 바닥 상태 확인
    bool tl = IsFloor(map, x - 1, y - 1);
    bool above = IsFloor(map, x, y - 1);
    bool tr = IsFloor(map, x + 1, y - 1);
    bool left = IsFloor(map, x - 1, y);
    bool right = IsFloor(map, x + 1, y);
    bool bl = IsFloor(map, x - 1, y + 1);
    bool below = IsFloor(map, x, y + 1);
    bool br = IsFloor(map, x + 1, y + 1);

    int patternCode = 0;
    if (IsFloor(map, x, y + 1))    patternCode |= (1 << 0); // 비트 0: UP (T)
    if (IsFloor(map, x + 1, y + 1)) patternCode |= (1 << 1); // 비트 1: UP-RIGHT (TR)
    if (IsFloor(map, x + 1, y))    patternCode |= (1 << 2); // 비트 2: RIGHT (R)
    if (IsFloor(map, x + 1, y - 1)) patternCode |= (1 << 3); // 비트 3: RIGHT-DOWN (BR)
    if (IsFloor(map, x, y - 1))    patternCode |= (1 << 4); // 비트 4: DOWN (B)
    if (IsFloor(map, x - 1, y - 1)) patternCode |= (1 << 5); // 비트 5: DOWN-LEFT (BL)
    if (IsFloor(map, x - 1, y))    patternCode |= (1 << 6); // 비트 6: LEFT (L)
    if (IsFloor(map, x - 1, y + 1)) patternCode |= (1 << 7); // 비트 7: LEFT-UP (TL)


    // **이 함수는 이제 패턴 코드만 반환하거나 (WallType enum 사용 시),**
    // **또는 patternCode를 저장해두고 GetWallRenderInfo에서 직접 사용합니다.**
    // WallType enum을 계속 사용한다면, 패턴 코드 -> WallType 매핑 룩업 테이블이 별도로 필요합니다.
    // 또는 간단하게 GetWallRenderInfo에서 패턴 코드를 계산하고 patternSubTileMap에서 바로 찾는 것이 효율적입니다.

     // WallType enum이 여전히 다른 게임 로직에 필요하다면:
     // std::map<int, WallType> patternToWallType; // 이 맵도 초기화 필요
     // return patternToWallType[patternCode]; // 맵에서 찾아서 반환

     // WallType enum이 렌더링 목적으로만 사용된다면, patternSubTileMap을 바로 사용
     // 이 경우 이 함수는 사실상 필요 없어지고, GetWallRenderInfo에서 패턴 코드를 계산합니다.

     // 여기서는 WallType enum을 유지하면서 패턴 코드를 결정하는 함수로 둡니다.
     // 패턴 코드 -> WallType 매핑은 GetSubTiles 내부에서 처리하거나,
     // GetWallRenderInfo에서 패턴 코드를 계산하고 직접 patternSubTileMap을 사용하도록 변경합니다.
     // GetWallRenderInfo에서 직접 사용하는 방식이 더 간단합니다.

    // 여기서는 패턴 코드 자체를 반환하거나, WallType::WALL_STANDARD 등을 기본값으로 반환하는 것은 의미가 없습니다.
    // 따라서 DetermineWallType 함수의 역할 변경을 고려해야 합니다.
    // 가장 깔끔한 방법은 GetWallRenderInfo에서 패턴 코드를 계산하고 바로 map에서 찾는 것입니다.
    // 기존 DetermineWallType 함수는 삭제하거나, 다른 용도로 변경합니다.
    // 여기서는 설명을 위해 DetermineWallType에서 패턴 코드를 계산하도록 하되, 반환 타입은 int로 변경하거나
    // GetWallRenderInfo 내부에 통합하는 것을 추천합니다.

    // 임시로 패턴 코드를 반환하는 것으로 가정합니다.
    // 실제로는 GetWallRenderInfo 내부에서 계산하는 것이 좋습니다.
    return static_cast<WallType>(patternCode);

    //
    // // 주변 타일 상태 확인 (바닥인지 아닌지)
    // bool above = IsFloor(map, x, y - 1);
    // bool below = IsFloor(map, x, y + 1);
    // bool left = IsFloor(map, x - 1, y);
    // bool right = IsFloor(map, x + 1, y);
    //
    // bool tl = IsFloor(map, x - 1, y - 1);
    // bool tr = IsFloor(map, x + 1, y - 1);
    // bool bl = IsFloor(map, x - 1, y + 1);
    // bool br = IsFloor(map, x + 1, y + 1);

    // **벽 타입 결정 로직 개선:**
    // 가장 특수한 경우부터 체크하여 우선순위를 높입니다.

    int floor_cardinal_count = (int)above + (int)below + (int)left + (int)right;

    // 1. 고립된 벽 (주변 4방향 모두 바닥) - 필요하다면 추가
    if (floor_cardinal_count == 4) return WALL_ISOLATED;

    // 2. 직선 벽 조각 (양옆 또는 위아래가 바닥) - 필요하다면 추가
    if (left && right && !above && !below) return WALL_HORIZONTAL;
    if (above && below && !left && !right) return WALL_VERTICAL;


    // 3. 내부 모서리 벽 (벽 덩어리 안에 바닥이 파고든 형태 - 3방향이 바닥인 경우)
    // 바닥이 위, 왼쪽, 좌상단 대각선에 있는 경우 (WALL_INNER_CORNER_TL)
    if (above && left && tl) return WALL_INNER_CORNER_TL;
    // 바닥이 위, 오른쪽, 우상단 대각선에 있는 경우 (WALL_INNER_CORNER_TR)
    if (above && right && tr) return WALL_INNER_CORNER_TR;
    // 바닥이 아래, 왼쪽, 좌하단 대각선에 있는 경우 (WALL_INNER_CORNER_BL)
    if (below && left && bl) return WALL_INNER_CORNER_BL;
    // 바닥이 아래, 오른쪽, 우하단 대각선에 있는 경우 (WALL_INNER_CORNER_BR)
    if (below && right && br) return WALL_INNER_CORNER_BR;

    // 4. 외부 모서리 벽 (벽 덩어리의 모서리 형태 - 2방향은 벽/경계, 1방향 대각선은 바닥)
    // 바닥이 우하단 대각선에 있고, 아래/오른쪽은 벽/경계인 경우 (WALL_CORNER_BR)
    if (!below && !right && br) return WALL_CORNER_BR;
    // 바닥이 좌하단 대각선에 있고, 아래/왼쪽은 벽/경계인 경우 (WALL_CORNER_BL)
    if (!below && !left && bl) return WALL_CORNER_BL;
    // 바닥이 우상단 대각선에 있고, 위/오른쪽은 벽/경계인 경우 (WALL_CORNER_TR)
    if (!above && !right && tr) return WALL_CORNER_TR;
    // 바닥이 좌상단 대각선에 있고, 위/왼쪽은 벽/경계인 경우 (WALL_CORNER_TL)
    if (!above && !left && tl) return WALL_CORNER_TL;

     // 5. 직선 벽 (한 방향만 바닥)
    // 아래에만 바닥이 있는 경우 (WALL_TOP)
    if (below && floor_cardinal_count == 1) return WALL_TOP;
    // 위에만 바닥이 있는 경우 (WALL_BOTTOM)
    if (above && floor_cardinal_count == 1) return WALL_BOTTOM;
    // 오른쪽에만 바닥이 있는 경우 (WALL_LEFT)
    if (right && floor_cardinal_count == 1) return WALL_LEFT;
    // 왼쪽에만 바닥이 있는 경우 (WALL_RIGHT)
    if (left && floor_cardinal_count == 1) return WALL_RIGHT;


    // 6. 기본 벽 (주변이 모두 벽 또는 경계)
    return WALL_STANDARD;

    // 참고: Shattered Pixel Dungeon은 이보다 더 다양한 조합(예: T-junction 형태 등)을
    //       처리하기 위해 주변 8방향의 상태를 비트마스크 등으로 표현하여
    //       타일 타입을 결정하는 lookup table 방식을 사용하기도 합니다.
}

std::array<WallTileRenderer::SubTile, 4> WallTileRenderer::GetSubTiles(WallType wallType) {
    std::array<SubTile, 4> result;
    std::array<TilePosition, 4>* sourceArray = nullptr;
    
    // 벽 타일 유형에 따라 적절한 부분 타일 배열 선택
    switch (wallType) {
        case WALL_STANDARD:
            sourceArray = &standardWallSubTiles;
            break;
        case WALL_TOP:
            sourceArray = &topWallSubTiles;
            break;
        case WALL_BOTTOM:
            sourceArray = &bottomWallSubTiles;
            break;
        case WALL_LEFT:
            sourceArray = &leftWallSubTiles;
            break;
        case WALL_RIGHT:
            sourceArray = &rightWallSubTiles;
            break;
        case WALL_CORNER_TL:
            sourceArray = &cornerTLSubTiles;
            break;
        case WALL_CORNER_TR:
            sourceArray = &cornerTRSubTiles;
            break;
        case WALL_CORNER_BL:
            sourceArray = &cornerBLSubTiles;
            break;
        case WALL_CORNER_BR:
            sourceArray = &cornerBRSubTiles;
            break;
        case WALL_INNER_CORNER_TL:
            sourceArray = &innerCornerTLSubTiles;
            break;
        case WALL_INNER_CORNER_TR:
            sourceArray = &innerCornerTRSubTiles;
            break;
        case WALL_INNER_CORNER_BL:
            sourceArray = &innerCornerBLSubTiles;
            break;
        case WALL_INNER_CORNER_BR:
            sourceArray = &innerCornerBRSubTiles;
            break;
        case WALL_HORIZONTAL:
            sourceArray = &horizontalSubTiles;
            break;
        case WALL_VERTICAL:
            sourceArray = &verticalSubTiles;
            break;
        case WALL_ISOLATED:
            sourceArray = &isolatedSubTiles;
            break;
    }
    
    // 부분 타일 정보 생성
    for (int i = 0; i < 4; ++i) {
        result[i].source = (*sourceArray)[i];
        
        // 8x8 그리드 내에서의 오프셋 계산
        if (i == TOP_LEFT) {
            result[i].offsetX = 0;
            result[i].offsetY = 0;
        } else if (i == TOP_RIGHT) {
            result[i].offsetX = 8;
            result[i].offsetY = 0;
        } else if (i == BOTTOM_LEFT) {
            result[i].offsetX = 0;
            result[i].offsetY = 8;
        } else { // BOTTOM_RIGHT
            result[i].offsetX = 8;
            result[i].offsetY = 8;
        }
    }
    
    return result;
}

WallTileRenderer::WallRenderInfo WallTileRenderer::GetWallRenderInfo(const std::vector<std::vector<int>>& map, int x, int y)
{
    WallRenderInfo result;

    if (!IsWall(map, x, y)) {
        result.tileType = -1;
        return result;
    }

    // 주변 8칸 바닥 상태 확인 및 패턴 코드 계산
    // **Direction2D.eightDirectionsList의 확정된 순서에 기반합니다!**
    // 순서: UP, UPRIGHT, RIGHT, RIGHTDOWN, DOWN, DOWNLEFT, LEFT, LEFTUP
    // 비트:  0,     1,       2,       3,       4,      5,     6,      7

    int patternCode = 0;
    if (IsFloor(map, x, y + 1))    patternCode |= (1 << 0); // 비트 0: UP (T)
    if (IsFloor(map, x + 1, y + 1)) patternCode |= (1 << 1); // 비트 1: UP-RIGHT (TR)
    if (IsFloor(map, x + 1, y))    patternCode |= (1 << 2); // 비트 2: RIGHT (R)
    if (IsFloor(map, x + 1, y - 1)) patternCode |= (1 << 3); // 비트 3: RIGHT-DOWN (BR)
    if (IsFloor(map, x, y - 1))    patternCode |= (1 << 4); // 비트 4: DOWN (B)
    if (IsFloor(map, x - 1, y - 1)) patternCode |= (1 << 5); // 비트 5: DOWN-LEFT (BL)
    if (IsFloor(map, x - 1, y))    patternCode |= (1 << 6); // 비트 6: LEFT (L)
    if (IsFloor(map, x - 1, y + 1)) patternCode |= (1 << 7); // 비트 7: LEFT-UP (TL)


    // C# 코드에 4비트 패턴이 있다면, 그 패턴 코드가 어떻게 계산되는지 확인해야 합니다.
    // 예를 들어, 8비트 코드를 계산한 후 하위 4비트만 사용하는지, 아니면 별도의 4방향 리스트를 사용하는지 등
    // C# 코드의 패턴 계산 로직을 정확히 따라야 합니다.
    // 만약 4비트 패턴이 8비트 패턴셋에 포함된 값들이라면 (예: 0b00001111 == 15),
    // 위 8비트 패턴 계산으로 충분합니다.
    // 하지만 wallTop, wallSideLeft 등의 HashSet에 있는 4비트 패턴 값이
    // 8비트 패턴 코드의 하위 4비트와 일치하지 않는다면, 4비트 패턴을 계산하는 별도의 로직이 필요합니다.


    // 패턴 코드에 해당하는 부분 타일 정보 룩업 (이후 로직은 동일)
    auto it = patternSubTileMap.find(patternCode);

    if (it != patternSubTileMap.end()) {
        const auto& sourceSubTiles = it->second;
        result.tileType = patternCode; // 패턴 코드를 타입으로 사용
        // 부분 타일 정보 생성 (offsetX, offsetY 계산) ...
        for (int i = 0; i < 4; ++i) {
            result.subTiles[i].source = sourceSubTiles[i];
            if (i == TOP_LEFT) { result.subTiles[i].offsetX = 0; result.subTiles[i].offsetY = 0; }
            else if (i == TOP_RIGHT) { result.subTiles[i].offsetX = 8; result.subTiles[i].offsetY = 0; }
            else if (i == BOTTOM_LEFT) { result.subTiles[i].offsetX = 0; result.subTiles[i].offsetY = 8; }
            else { result.subTiles[i].offsetX = 8; result.subTiles[i].offsetY = 8; }
        }

    } else {
        // 누락된 패턴 처리 ...
        // (이전 답변과 동일한 누락 패턴 처리 로직 사용)
        //Debug.LogError($"Missing pattern code in lookup table: {patternCode}");
        auto defaultIt = patternSubTileMap.find(0); // 기본 패턴 0 조회
        if (defaultIt != patternSubTileMap.end()) {
            const auto& sourceSubTiles = defaultIt->second;
            result.tileType = 0; // 기본 패턴 코드
            for (int i = 0; i < 4; ++i) {
                result.subTiles[i].source = sourceSubTiles[i];
                if (i == TOP_LEFT) { result.subTiles[i].offsetX = 0; result.subTiles[i].offsetY = 0; }
                else if (i == TOP_RIGHT) { result.subTiles[i].offsetX = 8; result.subTiles[i].offsetY = 0; }
                else if (i == BOTTOM_LEFT) { result.subTiles[i].offsetX = 0; result.subTiles[i].offsetY = 8; }
                else { result.subTiles[i].offsetX = 8; result.subTiles[i].offsetY = 8; }
            }
        } else {
            result.tileType = -1;
        }
    }

    return result;
}
// 기존 GetSubTiles 함수는 더 이상 사용하지 않거나, 간단한 WallType -> SubTile 배열 매핑 용도로 변경 가능
// 예를 들어, WALL_STANDARD, WALL_FLOOR 등 몇 가지 기본 타입에 대해서만 사용하고
// 복잡한 벽 패턴은 GetWallRenderInfo에서 직접 처리
// WallTileRenderer::WallRenderInfo WallTileRenderer::GetWallRenderInfo(const std::vector<std::vector<int>>& map, int x, int y) {
//     WallRenderInfo result;
//     
//     // 벽 타일 유형 결정
//     WallType wallType = DetermineWallType(map, x, y);
//     
//     // 타일 유형 설정 (enum 값이 이미 기존 시스템의 타일 타입 상수와 일치하도록 설정됨)
//     result.tileType = static_cast<int>(wallType);
//     
//     // 부분 타일 배열 생성
//     result.subTiles = GetSubTiles(wallType);
//     
//     return result;
// }
