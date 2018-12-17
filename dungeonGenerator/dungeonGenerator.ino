// Sam Sibbens
// 2018 DECEMBER 16
// Arduboy Dungeon Generator

#include <Arduino.h>
#include <Arduboy2.h>
#include "DungeonGenerator.h"
Arduboy2 arduboy;

Dungeon dungeon;

void setup() {
    arduboy.begin();
    arduboy.clear();
    arduboy.setFrameRate(60);
    dungeon.loadRooms(0, 0);
}

void loop() {
    //Prevent the Arduboy from running too fast
    if(!arduboy.nextFrame()) {return;}

    arduboy.clear();
    dungeon.draw();
    arduboy.display();
}
