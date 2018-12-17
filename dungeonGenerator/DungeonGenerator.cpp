
#include <Arduino.h>
#include <Arduboy2.h>
#include "DungeonGenerator.h"
#include "Images.h"

extern Arduboy2 arduboy;


RoomWallLayoutID Dungeon::getRoomLayoutFromSeed(uint16_t xpos, uint16_t ypos) {
    int32_t currentSeed = ((static_cast<int32_t>(xpos) << 16) | (static_cast<int32_t>(ypos) << 0));
    randomSeed(currentSeed);

    return static_cast<RoomWallLayoutID>(random(5, 10));
}

RoomWallLayoutID Dungeon::getRoomLayoutFromNeighbours(uint16_t xpos, uint16_t ypos) {
    bool leftRoomHasRightWall = this->wallRight(xpos-1, ypos);
    bool rightRoomHasLeftWall = this->wallLeft(xpos+1, ypos);
    bool aboveRoomHasDownWall = this->wallDown(xpos, ypos-1);
    bool belowRoomHasUpperWall = this->wallUp(xpos, ypos-1);

    RoomWallLayoutID layout = RoomWallLayoutID::Zero;
	
    if (leftRoomHasRightWall) {
        layout |= RoomWallLayoutID::OneLeft;
    }
    if (rightRoomHasLeftWall) {
        layout |= RoomWallLayoutID::OneRight;
    }
    if (aboveRoomHasDownWall) {
        layout |= RoomWallLayoutID::OneUp;
    }
    if (belowRoomHasUpperWall) {
        layout |= RoomWallLayoutID::OneDown;
    }

    return layout;
}


bool Dungeon::wallLeft(uint16_t xpos, uint16_t ypos) {
    RoomWallLayoutID leftRoom = Dungeon::getRoomLayoutFromSeed(xpos-1, ypos);
    return ((leftRoom & RoomWallLayoutID::OneRight) != RoomWallLayoutID::Zero);
}
bool Dungeon::wallRight(uint16_t xpos, uint16_t ypos) {
    RoomWallLayoutID rightRoom = Dungeon::getRoomLayoutFromSeed(xpos+1, ypos);
    return ((rightRoom & RoomWallLayoutID::OneLeft) != RoomWallLayoutID::Zero);
}
bool Dungeon::wallUp(uint16_t xpos, uint16_t ypos) {
    RoomWallLayoutID aboveRoom = Dungeon::getRoomLayoutFromSeed(xpos, ypos-1);
    return ((aboveRoom & RoomWallLayoutID::OneDown) != RoomWallLayoutID::Zero);
}
bool Dungeon::wallDown(uint16_t xpos, uint16_t ypos) {
    RoomWallLayoutID belowRoom = Dungeon::getRoomLayoutFromSeed(xpos, ypos+1);
    return ((belowRoom & RoomWallLayoutID::OneUp) != RoomWallLayoutID::Zero);
}


void Dungeon::loadRooms(uint16_t xpos, uint16_t ypos) {
    for(uint16_t i = xpos; i < xpos + width; ++i) {
        for(uint16_t j = ypos; j < ypos + height; ++j) {
            if ((i % 2) == (j % 2)) {
                rooms[width * j + i] = getRoomLayoutFromSeed(i, j);
            }
        }
    }
    
    for(uint16_t i = xpos; i < xpos + width; ++i) {
        for(uint16_t j = ypos; j < ypos + height; ++j) {
            if ((i % 2) != (j % 2)) {
                rooms[width * j + i] = getRoomLayoutFromNeighbours(i, j);
            }
        }
    }
}

const uint8_t * const roomLookup[] PROGMEM =
{
	RoomWallLayoutData::Zero,
	RoomWallLayoutData::OneDown,
	RoomWallLayoutData::OneUp,
	RoomWallLayoutData::TwoUpDown,
	RoomWallLayoutData::OneRight,
	RoomWallLayoutData::TwoRightDown,
	RoomWallLayoutData::TwoRightUp,
	RoomWallLayoutData::ThreeRightUpDown,
	RoomWallLayoutData::OneLeft,
	RoomWallLayoutData::TwoLeftDown,
	RoomWallLayoutData::TwoLeftUp,
	RoomWallLayoutData::ThreeLeftUpDown,
	RoomWallLayoutData::TwoLeftRight,
	RoomWallLayoutData::ThreeLeftRightDown,
	RoomWallLayoutData::ThreeLeftRightUp,
	RoomWallLayoutData::Four,
};

const uint8_t * Dungeon::getRoomImage(RoomWallLayoutID roomLayout)
{
	const uint8_t index = static_cast<uint8_t>(roomLayout);
	
	if(index >= 0x10)
		return nullptr;
		
	return reinterpret_cast<const uint8_t *>(pgm_read_ptr(&roomLookup[index]));
}

void Dungeon::draw() {
    for(uint16_t i = 0; i < width; ++i) {
        for(uint16_t j = 0; j < height; ++j) {
            RoomWallLayoutID layout = this->rooms[width * j + i];
            const uint8_t * layoutImage = this->getRoomImage(layout);
            Sprites::drawSelfMasked(i * tileWidth, j * tileHeight, layoutImage, 0);
        }
    }
}














//End
