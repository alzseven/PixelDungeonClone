#pragma once
#include "IDungeonGenerator.h"
#include <vector>
#include <random>

class ProceduralDungeonGenerator : public IDungeonGenerator {
public:
    ProceduralDungeonGenerator();
    ~ProceduralDungeonGenerator() override = default;

    // IDungeonGenerator 인터페이스 구현
    virtual std::vector<std::vector<int>> Generate(int width, int height) override;
    virtual std::vector<std::vector<int>> GetTileVariations(std::vector<std::vector<int>>& map) override;
    virtual void RenderWallTile(D2DImage* image, int x, int y, int tileX, int tileY, float scale,
                        const std::vector<std::vector<int>>& map) override;
private:
    // 방 구조체
    struct Room {
        int left, top, right, bottom;
        
        bool intersects(const Room& other, int margin) const;
        int area() const;
        int centerX() const;
        int centerY() const;
    };
    
    // 방 노드 구조체
    struct RoomNode {
        Room room;
        enum Type { STANDARD, ENTRANCE, EXIT, HIDDEN } type;
        std::vector<int> connections;
        bool isOnMainPath;
        
        RoomNode(const Room& room, Type type) 
            : room(room), type(type), isOnMainPath(false) {}
    };
    
    // 상수 정의
    static constexpr int MIN_ROOM_SIZE = 4;
    static constexpr int MAX_STANDARD_ROOM_SIZE = 8;
    static constexpr int MAX_LARGE_ROOM_SIZE = 12;
    static constexpr int MAX_GIANT_ROOM_SIZE = 16;
    static constexpr int ROOM_MARGIN = 2;
    
    // 던전 생성 관련 메서드들
    std::vector<Room> GenerateRooms(int width, int height, int minRooms, int maxRooms);
    void BuildLoopLayout(std::vector<RoomNode>& roomNodes);
    void ConnectRooms(std::vector<std::vector<int>>& map, const std::vector<RoomNode>& roomNodes);
    void CreateCorridor(std::vector<std::vector<int>>& map, int x1, int y1, int x2, int y2);
    void PlaceSpecialRooms(std::vector<std::vector<int>>& map, std::vector<RoomNode>& roomNodes);
    void CleanupWalls(std::vector<std::vector<int>>& map);
    void DrawRoom(std::vector<std::vector<int>>& map, const Room& room, int tileType = TILE_FLOOR);
    
    // 타일 변형 결정 메서드들
    int DetermineWallVariation(const std::vector<std::vector<int>>& map, int x, int y);
    int DetermineFloorVariation(const std::vector<std::vector<int>>& map, int x, int y);


    // 유틸리티 메서드들
    int GetRandomInt(int min, int max);
    bool GetRandomBool(float probability);


}; 