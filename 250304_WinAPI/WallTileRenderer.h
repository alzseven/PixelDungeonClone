#pragma once
#include <vector>
#include <array>
#include <map>

// 개선된 벽 타일 렌더러 클래스 - 내부/외부 코너 구분 명확화
class WallTileRenderer {
public:
    // 부분 타일 인덱스 (2x2 그리드)
    enum SubTileIndex {
        TOP_LEFT = 0,     
        TOP_RIGHT = 1,     
        BOTTOM_LEFT = 2,     
        BOTTOM_RIGHT = 3
    };

    // 타일 유형 (타일맵에서의 위치)
    struct TilePosition {
        int x, y;  // 타일맵에서의 x, y 좌표
    };

    // 부분 타일 정보
    struct SubTile {
        TilePosition source;  // 타일맵에서의 소스 위치
        int offsetX, offsetY; // 렌더링 오프셋 (2x2 그리드 내에서의 위치)
    };

    // 벽 타일 유형 - 기존 코드와의 호환성을 위해 상수 값 지정
    enum WallType {
        WALL_STANDARD = 1,     // 일반 벽 (TILE_WALL)
        WALL_TOP = 7,          // 상단 벽 (TILE_WALL_TOP)
        WALL_BOTTOM = 8,       // 하단 벽 (TILE_WALL_BOTTOM)
        WALL_LEFT = 9,         // 좌측 벽 (TILE_WALL_LEFT)
        WALL_RIGHT = 10,       // 우측 벽 (TILE_WALL_RIGHT)
        WALL_CORNER_TL = 11,   // 좌상단 외부 모서리 (TILE_WALL_CORNER_TL)
        WALL_CORNER_TR = 12,   // 우상단 외부 모서리 (TILE_WALL_CORNER_TR)
        WALL_CORNER_BL = 13,   // 좌하단 외부 모서리 (TILE_WALL_CORNER_BL)
        WALL_CORNER_BR = 14,   // 우하단 외부 모서리 (TILE_WALL_CORNER_BR)
        WALL_INNER_CORNER_TL = 19, // 내부 좌상단 모서리 (새로운 타일 타입)
        WALL_INNER_CORNER_TR = 20, // 내부 우상단 모서리 (새로운 타일 타입)
        WALL_INNER_CORNER_BL = 21, // 내부 좌하단 모서리 (새로운 타일 타입)
        WALL_INNER_CORNER_BR = 22,  // 내부 우하단 모서리 (새로운 타일 타입)
        WALL_HORIZONTAL = 23,
        WALL_VERTICAL = 24,
        WALL_ISOLATED = 25,
    };

    WallTileRenderer();
    ~WallTileRenderer() = default;

    // 주변 타일 상태에 따라 적절한 벽 타일 유형 결정
    WallType DetermineWallType(const std::vector<std::vector<int>>& map, int x, int y);

    // 벽 타일 유형에 따라 부분 타일 배열 생성
    std::array<SubTile, 4> GetSubTiles(WallType wallType);

    // 벽 타일 렌더링 정보 생성
    struct WallRenderInfo {
        int tileType;                  // 최종 타일 유형 (기존 시스템과의 호환성)
        std::array<SubTile, 4> subTiles; // 부분 타일 배열
    };

    // 주변 타일 상태에 따라 벽 타일 렌더링 정보 생성
    WallRenderInfo GetWallRenderInfo(const std::vector<std::vector<int>>& map, int x, int y);

private:
    // 타일맵에서 부분 타일 위치 초기화
    void InitializeSubTilePositions();

    // 각 벽 타일 유형별 부분 타일 매핑
    std::array<TilePosition, 4> standardWallSubTiles;
    std::array<TilePosition, 4> topWallSubTiles;
    std::array<TilePosition, 4> bottomWallSubTiles;
    std::array<TilePosition, 4> leftWallSubTiles;
    std::array<TilePosition, 4> rightWallSubTiles;
    std::array<TilePosition, 4> cornerTLSubTiles;
    std::array<TilePosition, 4> cornerTRSubTiles;
    std::array<TilePosition, 4> cornerBLSubTiles;
    std::array<TilePosition, 4> cornerBRSubTiles;
    std::array<TilePosition, 4> innerCornerTLSubTiles;
    std::array<TilePosition, 4> innerCornerTRSubTiles;
    std::array<TilePosition, 4> innerCornerBLSubTiles;
    std::array<TilePosition, 4> innerCornerBRSubTiles;
    std::array<TilePosition, 4> horizontalSubTiles;
    std::array<TilePosition, 4> verticalSubTiles;
    std::array<TilePosition, 4> isolatedSubTiles;

    std::map<int, std::array<TilePosition, 4>> patternSubTileMap;
    
    // 타일 유형 확인 헬퍼 함수
    bool IsWall(const std::vector<std::vector<int>>& map, int x, int y);
    bool IsFloor(const std::vector<std::vector<int>>& map, int x, int y);
};
