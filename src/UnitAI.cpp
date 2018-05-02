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
	std::cout << "Created" << std::endl;
}

UnitAI::~UnitAI() {

}


void UnitAI::Tick(float dt) {

	if(entity->health <= 0) {
		//std::cout << "No health" << std::endl;
		entity->engine->entityMgr->DestroyEnemy(entity->identity);
	}

	if(commands.size() > 0) {
		commands.front()->tick(dt);
		if(commands.front()->done()) {
			commands.pop_front();
		}
	}

	if(entity == entity->engine->entityMgr->player) {
		for (int i = 0; i < (int)entity->engine->entityMgr->items.size(); i++) {
			EntityItem * testEntity = entity->engine->entityMgr->items[i];

			if (testEntity->destroyed == false) {
				bool result = entity->sceneNode->_getWorldAABB().intersects(testEntity->sceneNode->_getWorldAABB());

				if(result) {
					//std::cout << "Hit Item" << std::endl;
					entity->engine->entityMgr->ApplyItem(i);
				}
			}
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
