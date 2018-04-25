/*
 * UnitAI.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: gary
 */

#include "UnitAI.h"
#include "EntityMgr.h"
#include "Command.h"

UnitAI::UnitAI(Entity381* ent):Aspect(ent){

}

UnitAI::~UnitAI() {

}


void UnitAI::Tick(float dt) {
	int followSqrDistance = 10000;
	Entity381 * player = entity->engine->entityMgr->player;
	if(entity->position.squaredDistance(player->position) < followSqrDistance) {
		Follow * f = new Follow(entity, player);
		AddCommand(f);
	}

	if(commands.size() > 0) {
		commands.front()->tick(dt);
		if(commands.front()->done()) {
			commands.pop_front();
		}
	}
}

void UnitAI::SetCommand(Command *c) {
	commands.clear();
	commands.push_back(c);
}

void UnitAI::AddCommand(Command *c) {
	commands.push_back(c);
}
