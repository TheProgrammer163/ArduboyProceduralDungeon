#pragma once

#include <Arduino.h>
#include <Arduboy2.h>



enum class RoomWallLayoutID : uint8_t {
    Zero = 0,
    OneLeft = 0b1000,
    OneRight = 0b0100,
    OneUp = 0b0010,
    OneDown = 0b0001,
    
    TwoLeftRight = 0b1100,
    TwoLeftUp = 0b1010,
    TwoLeftDown = 0b1001,
    TwoUpDown = 0b0011,
    TwoRightUp = 0b0110,
    TwoRightDown = 0b0101,
    
    ThreeLeftRightDown = 0b1101,
    ThreeLeftRightUp = 0b1110,
    ThreeLeftUpDown = 0b1011,
    ThreeRightUpDown = 0b0111,

    Four = 0b1111
};

constexpr inline RoomWallLayoutID operator |(RoomWallLayoutID left, RoomWallLayoutID right)
{
	return static_cast<RoomWallLayoutID>(static_cast<uint8_t>(left) | static_cast<uint8_t>(right));
}

inline RoomWallLayoutID & operator |=(RoomWallLayoutID & left, RoomWallLayoutID right)
{
	left = (left | right);
	return left;
}

constexpr inline RoomWallLayoutID operator &(RoomWallLayoutID left, RoomWallLayoutID right)
{
	return static_cast<RoomWallLayoutID>(static_cast<uint8_t>(left) & static_cast<uint8_t>(right));
}

inline RoomWallLayoutID & operator &=(RoomWallLayoutID & left, RoomWallLayoutID right)
{
	left = (left & right);
	return left;
}

constexpr inline RoomWallLayoutID operator ^(RoomWallLayoutID left, RoomWallLayoutID right)
{
	return static_cast<RoomWallLayoutID>(static_cast<uint8_t>(left) ^ static_cast<uint8_t>(right));
}

inline RoomWallLayoutID & operator ^=(RoomWallLayoutID & left, RoomWallLayoutID right)
{
	left = (left ^ right);
	return left;
}

class Dungeon {
    public:
        static constexpr uint8_t tileWidth = 8;
        static constexpr uint8_t tileHeight = 8;

        static constexpr uint8_t width = 16;
        static constexpr uint8_t height = 8;
        static constexpr uint8_t roomCount = width * height;
    public:
        RoomWallLayoutID rooms[roomCount] = {};
        uint16_t x = 0;
        uint16_t y = 0;
    public:
        RoomWallLayoutID getRoomLayoutFromSeed(uint16_t xpos, uint16_t ypos);
        RoomWallLayoutID getRoomLayoutFromNeighbours(uint16_t xpos, uint16_t ypos);
        bool wallLeft(uint16_t xpos, uint16_t ypos);
        bool wallRight(uint16_t xpos, uint16_t ypos);
        bool wallUp(uint16_t xpos, uint16_t ypos);
        bool wallDown(uint16_t xpos, uint16_t ypos);
        void loadRooms(uint16_t xpos, uint16_t ypos);
        void draw();
        const uint8_t * getRoomImage(RoomWallLayoutID roomLayout);
};










//End
