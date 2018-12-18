
#include <Arduino.h>
#include <Arduboy2.h>
#include "DungeonGenerator.h"
#include "Images.h"

extern Arduboy2 arduboy;


RoomWallLayoutID DungeonGenerator::getRoomLayoutFromSeed(uint16_t xpos, uint16_t ypos) {
    uint32_t xSeed = static_cast<uint32_t>(xpos);
    uint32_t ySeed = static_cast<uint32_t>(ypos);
    xSeed = (xSeed*100*100)/1337;
    ySeed = (ySeed*100*100)/501;
    uint32_t currentSeed = ((xSeed << 16) | (ySeed << 0));
    randomSeed(currentSeed);

    return static_cast<RoomWallLayoutID>(random(0, 15));
    
    //var _x = ((argument0*seed*seed)/1337);
    //var _y = ((argument1*seed*seed)/501);
    
}

RoomWallLayoutID DungeonGenerator::getRoomLayoutFromNeighbours(uint16_t xpos, uint16_t ypos) {
    bool leftRoomHasRightWall = (xpos == 0) || DungeonGenerator::hasWallRight(xpos - 1, ypos);
    bool rightRoomHasLeftWall = (xpos == (Dungeon::width - 1)) || DungeonGenerator::hasWallLeft(xpos + 1, ypos);
    bool aboveRoomHasDownWall = (ypos == 0) || DungeonGenerator::hasWallDown(xpos, ypos - 1);
    bool belowRoomHasUpperWall = (ypos == (Dungeon::height - 1)) || DungeonGenerator::hasWallUp(xpos, ypos + 1);

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


bool DungeonGenerator::hasWallLeft(uint16_t xpos, uint16_t ypos) {
    RoomWallLayoutID room = DungeonGenerator::getRoomLayoutFromSeed(xpos, ypos);
    return ((room & RoomWallLayoutID::OneLeft) != RoomWallLayoutID::Zero);
}
bool DungeonGenerator::hasWallRight(uint16_t xpos, uint16_t ypos) {
    RoomWallLayoutID room = DungeonGenerator::getRoomLayoutFromSeed(xpos, ypos);
    return ((room & RoomWallLayoutID::OneRight) != RoomWallLayoutID::Zero);
}
bool DungeonGenerator::hasWallUp(uint16_t xpos, uint16_t ypos) {
    RoomWallLayoutID room = DungeonGenerator::getRoomLayoutFromSeed(xpos, ypos);
    return ((room & RoomWallLayoutID::OneUp) != RoomWallLayoutID::Zero);
}
bool DungeonGenerator::hasWallDown(uint16_t xpos, uint16_t ypos) {
    RoomWallLayoutID room = DungeonGenerator::getRoomLayoutFromSeed(xpos, ypos);
    return ((room & RoomWallLayoutID::OneDown) != RoomWallLayoutID::Zero);
}


void Dungeon::loadRooms(uint16_t xpos, uint16_t ypos) {
    for(uint16_t i = xpos; i < xpos + width; ++i) {
        for(uint16_t j = ypos; j < ypos + height; ++j) {
            if ((i % 2) == (j % 2)) {
                this->getRoomLayoutAt(i, j) = DungeonGenerator::getRoomLayoutFromSeed(i, j);
            }
            else {
                this->getRoomLayoutAt(i, j) = DungeonGenerator::getRoomLayoutFromNeighbours(i, j);
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
    arduboy.fillScreen (WHITE);
    
    for(uint16_t i = 0; i < width; ++i) {
        for(uint16_t j = 0; j < height; ++j) {
            if (arduboy.pressed(A_BUTTON) && (!(i % 2) == (j % 2))) {
                continue;
            }
            
            if (arduboy.pressed(B_BUTTON) && ((i % 2) == (j % 2))) {
                continue;
            }
            RoomWallLayoutID layout = this->getRoomLayoutAt(i, j);
            const uint8_t * layoutImage = this->getRoomImage(layout);
            Sprites::drawOverwrite(i * tileWidth, j * tileHeight, layoutImage, 0);
        }
    }
    // Debugging
    /*
    uint8_t j = 1;
    uint8_t margin = tileWidth+4;
    for(uint8_t i = 0; i < 8; i++) {
        Sprites::drawOverwrite(i * margin + margin, j * margin, getRoomImage(static_cast<RoomWallLayoutID>(i)), 0);
    }
    j+=2;
    for(uint8_t i = 8; i < 16; i++) {
        Sprites::drawOverwrite(i * margin - 8*margin + margin, j * margin, getRoomImage(static_cast<RoomWallLayoutID>(i)), 0);
    }*/
}

RoomWallLayoutID & Dungeon::getRoomLayoutAt(uint16_t xpos, uint16_t ypos)
{
	return this->rooms[(ypos * width) + xpos];
}













//End
