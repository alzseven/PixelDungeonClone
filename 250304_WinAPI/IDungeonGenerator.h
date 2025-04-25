#pragma once
#include <vector>

#include "WallTileRenderer.h"

class D2DImage;

class IDungeonGenerator {
protected:
    // 벽 타일 렌더러
    WallTileRenderer wallTileRenderer;
    int ascInd{ 0 };
    int descInd{ 0 };
public:
    virtual ~IDungeonGenerator() = default;

    // 던전 생성 메서드
    virtual std::vector<std::vector<int>> Generate(int width, int height) = 0;

    // 타일 변형 적용 메서드
    virtual std::vector<std::vector<int>> GetTileVariations(std::vector<std::vector<int>>& map) = 0;
    
    virtual void RenderWallTile(D2DImage* image, int x, int y, int tileX, int tileY, float scale, const std::vector<std::vector<int>>& map) = 0;
    
    virtual int GetAscInd() { return ascInd; }
    virtual int GetDescInd() { return descInd; }

    // 타일 상수
    static constexpr int TILE_NONE = 6;
    static constexpr int TILE_WALL = 0;
    static constexpr int TILE_FLOOR = 1;
    static constexpr int TILE_DOOR = 2;
    static constexpr int TILE_ENTRANCE = 3;
    static constexpr int TILE_EXIT = 4;
    static constexpr int TILE_HIDDEN_DOOR = 5;
    
    // 벽 변형 타일
    static constexpr int TILE_WALL_TOP = 10;
    static constexpr int TILE_WALL_BOTTOM = 11;
    static constexpr int TILE_WALL_LEFT = 12;
    static constexpr int TILE_WALL_RIGHT = 13;
    static constexpr int TILE_WALL_CORNER_TL = 14;  // Top-Left
    static constexpr int TILE_WALL_CORNER_TR = 15;  // Top-Right
    static constexpr int TILE_WALL_CORNER_BL = 16;  // Bottom-Left
    static constexpr int TILE_WALL_CORNER_BR = 17;  // Bottom-Right
    
    // 추가 벽 변형 타일 (Shattered Pixel Dungeon 스타일)
    static constexpr int TILE_WALL_INNER = 18;      // 내부 벽
    static constexpr int TILE_WALL_SOLO = 19;       // 독립된 벽
    static constexpr int TILE_WALL_TOP_LEFT = 20;   // 상단 좌측 모서리
    static constexpr int TILE_WALL_TOP_RIGHT = 21;  // 상단 우측 모서리
    static constexpr int TILE_WALL_BOTTOM_LEFT = 22;// 하단 좌측 모서리
    static constexpr int TILE_WALL_BOTTOM_RIGHT = 23;// 하단 우측 모서리
    static constexpr int TILE_WALL_SIDE_TOP = 24;   // 측면 상단
    static constexpr int TILE_WALL_SIDE_BOTTOM = 25;// 측면 하단
    static constexpr int TILE_WALL_SIDE_LEFT = 26;  // 측면 좌측
    static constexpr int TILE_WALL_SIDE_RIGHT = 27; // 측면 우측
    static constexpr int TILE_WALL_INNER_CORNER_TL = 28; // 내부 코너 상단 좌측
    static constexpr int TILE_WALL_INNER_CORNER_TR = 29; // 내부 코너 상단 우측
    static constexpr int TILE_WALL_INNER_CORNER_BL = 30; // 내부 코너 하단 좌측
    static constexpr int TILE_WALL_INNER_CORNER_BR = 31; // 내부 코너 하단 우측
    
    // 바닥 변형 타일
    static constexpr int TILE_FLOOR_NORMAL = 40;
    static constexpr int TILE_FLOOR_FANCY = 41;
    static constexpr int TILE_FLOOR_CRACKED = 42;
    static constexpr int TILE_FLOOR_MOSSY = 43;
}; 