
#include <Arduino.h>
#include <Arduboy2.h>
#include "DungeonGenerator.h"
#include "Images.h"

extern Arduboy2 arduboy;


RoomWallLayoutID Dungeon::getRoomLayoutFromSeed(int16_t xpos, int16_t ypos) {
    int32_t currentSeed = (static_cast<int32_t>(xpos) << 16) | static_cast<int32_t>(ypos);
    randomSeed(currentSeed);

    return static_cast<RoomWallLayoutID>(random(5, 10));
}

RoomWallLayoutID Dungeon::getRoomLayoutFromNeighbours(int16_t xpos, int16_t ypos) {
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


bool Dungeon::wallLeft(int16_t xpos, int16_t ypos) {
    RoomWallLayoutID leftRoom = Dungeon::getRoomLayoutFromSeed(xpos-1, ypos);
    return ((leftRoom & RoomWallLayoutID::OneRight) != RoomWallLayoutID::Zero);
}
bool Dungeon::wallRight(int16_t xpos, int16_t ypos) {
    RoomWallLayoutID rightRoom = Dungeon::getRoomLayoutFromSeed(xpos+1, ypos);
    return ((rightRoom & RoomWallLayoutID::OneLeft) != RoomWallLayoutID::Zero);
}
bool Dungeon::wallUp(int16_t xpos, int16_t ypos) {
    RoomWallLayoutID aboveRoom = Dungeon::getRoomLayoutFromSeed(xpos, ypos-1);
    return ((aboveRoom & RoomWallLayoutID::OneDown) != RoomWallLayoutID::Zero);
}
bool Dungeon::wallDown(int16_t xpos, int16_t ypos) {
    RoomWallLayoutID belowRoom = Dungeon::getRoomLayoutFromSeed(xpos, ypos+1);
    return ((belowRoom & RoomWallLayoutID::OneUp) != RoomWallLayoutID::Zero);
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
    for(int16_t i = 0; i < 16; i++) {
        for(int16_t j = 0; j < 8; j++) {
            RoomWallLayoutID layout = this->rooms[16 * j + i];
            const uint8_t * layoutImage = this->getRoomImage(layout);
            Sprites::drawSelfMasked((i)*8, (j)*8, layoutImage, 0);
        }
    }
}














//End
