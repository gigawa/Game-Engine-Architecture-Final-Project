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

	if(commands.size() > 0) {
		commands.front()->tick(dt);
		if(commands.front()->done()) {
			commands.pop_front();
		}
	}
}

void UnitAI::SetCommand(Command *c) {
	std::cout << "Command Added" << std::endl;
	commands.clear();
	commands.push_back(c);
}

void UnitAI::AddCommand(Command *c) {
	commands.push_back(c);
	std::cout << "Command Added" << std::endl;
}
