
#include <Arduino.h>
#include <Arduboy2.h>
#include "DungeonGenerator.h"
#include "Images.h"

extern Arduboy2 arduboy;


uint8_t Dungeon::getRoomLayoutFromSeed(int16_t xpos, int16_t ypos) {
    int32_t currentSeed = (static_cast<int32_t>(xpos) << 16) | static_cast<int32_t>(ypos);
    randomSeed(currentSeed);

    return random(5, 10);
}

uint8_t Dungeon::getRoomLayoutFromNeighbours(int16_t xpos, int16_t ypos) {
    bool leftRoomHasRightWall = this->wallRight(xpos-1, ypos);
    bool rightRoomHasLeftWall = this->wallLeft(xpos+1, ypos);
    bool aboveRoomHasDownWall = this->wallDown(xpos, ypos-1);
    bool belowRoomHasUpperWall = this->wallUp(xpos, ypos-1);

    uint8_t layout = 0;
    if (leftRoomHasRightWall) {
        layout = layout | static_cast<uint8_t>(RoomWallLayoutID::OneLeft);
    }
    if (rightRoomHasLeftWall) {
        layout = layout | static_cast<uint8_t>(RoomWallLayoutID::OneRight);
    }
    if (aboveRoomHasDownWall) {
        layout = layout | static_cast<uint8_t>(RoomWallLayoutID::OneUp);
    }
    if (belowRoomHasUpperWall) {
        layout = layout | static_cast<uint8_t>(RoomWallLayoutID::OneDown);
    }

    return layout;
}


bool Dungeon::wallLeft(int16_t xpos, int16_t ypos) {
    uint8_t leftRoom = Dungeon::getRoomLayoutFromSeed(xpos-1, ypos);
    return ((leftRoom & static_cast<uint8_t>(RoomWallLayoutID::OneRight)) != 0);
}
bool Dungeon::wallRight(int16_t xpos, int16_t ypos) {
    uint8_t rightRoom = Dungeon::getRoomLayoutFromSeed(xpos+1, ypos);
    return ((rightRoom & static_cast<uint8_t>(RoomWallLayoutID::OneLeft)) != 0);
}
bool Dungeon::wallUp(int16_t xpos, int16_t ypos) {
    uint8_t aboveRoom = Dungeon::getRoomLayoutFromSeed(xpos, ypos-1);
    return ((aboveRoom & static_cast<uint8_t>(RoomWallLayoutID::OneDown)) != 0);
}
bool Dungeon::wallDown(int16_t xpos, int16_t ypos) {
    uint8_t belowRoom = Dungeon::getRoomLayoutFromSeed(xpos, ypos+1);
    return ((belowRoom & static_cast<uint8_t>(RoomWallLayoutID::OneUp)) != 0);
}


void Dungeon::loadRooms(int16_t xpos, int16_t ypos) {
    for(int16_t i = xpos; i < xpos + 16; i++) {
        for(int16_t j = ypos; j < ypos+8; j++) {
            if (i%2 == j%2) {
                rooms[16 * j + i] = getRoomLayoutFromSeed(i, j);
            }
        }
    }
    
    for(int16_t i = xpos; i < xpos + 16; i++) {
        for(int16_t j = ypos; j < ypos+8; j++) {
            if (i%2 != j%2) {
                rooms[16 * j + i] = getRoomLayoutFromNeighbours(i, j);
            }
        }
    }
}

uint8_t getRoomImage(uint8_t roomLayout) {
    switch(roomLayout) {
        case static_cast<uint8_t>(RoomWallLayoutID::Zero):
            return &RoomWallLayoutData::Zero;
            break;
            
        case static_cast<uint8_t>(RoomWallLayoutID::OneLeft):
            return &RoomWallLayoutData::OneLeft;
            break;
        case static_cast<uint8_t>(RoomWallLayoutID::OneRight):
            return &RoomWallLayoutData::OneRight;
            break;
        case static_cast<uint8_t>(RoomWallLayoutID::OneUp):
            return &RoomWallLayoutData::OneUp;
            break;
        case static_cast<uint8_t>(RoomWallLayoutID::OneDown):
            return &RoomWallLayoutData::OneDown;
            break;
            
        case static_cast<uint8_t>(RoomWallLayoutID::TwoLeftRight):
            return &RoomWallLayoutData::TwoLeftRight;
            break;
        case static_cast<uint8_t>(RoomWallLayoutID::TwoLeftUp):
            return &RoomWallLayoutData::TwoLeftUp;
            break;
        case static_cast<uint8_t>(RoomWallLayoutID::TwoLeftDown):
            return &RoomWallLayoutData::TwoLeftDown;
            break;
        case static_cast<uint8_t>(RoomWallLayoutID::TwoUpDown):
            return &RoomWallLayoutData::TwoUpDown;
            break;
        case static_cast<uint8_t>(RoomWallLayoutID::TwoRightUp):
            return &RoomWallLayoutData::TwoRightUp;
            break;
        case static_cast<uint8_t>(RoomWallLayoutID::TwoRightDown):
            return &RoomWallLayoutData::TwoRightDown;
            break;

        case static_cast<uint8_t>(RoomWallLayoutID::ThreeLeftRightDown):
            return &RoomWallLayoutData::ThreeLeftRightDown;
            break;
        case static_cast<uint8_t>(RoomWallLayoutID::ThreeLeftRightUp):
            return &RoomWallLayoutData::ThreeLeftRightUp;
            break;
        case static_cast<uint8_t>(RoomWallLayoutID::ThreeLeftUpDown):
            return &RoomWallLayoutData::ThreeLeftUpDown;
            break;
        case static_cast<uint8_t>(RoomWallLayoutID::ThreeRightUpDown):
            return &RoomWallLayoutData::ThreeRightUpDown;
            break;
            
        case static_cast<uint8_t>(RoomWallLayoutID::Four):
            return &RoomWallLayoutData::Four;
            break;
    }
}

void Dungeon::draw() {
    for(int16_t i = 0; i < 16; i++) {
        for(int16_t j = 0; j < 8; j++) {
            uint8_t layout = this->rooms[16 * j + i];
            uint8_t layoutImage = this->getRoomImage(layout);
            Sprites::drawSelfMasked((i)*8, (j)*8, layoutImage, 0);
        }
    }
}














//End
