/*
 * EntityMgr.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: sushil
 */

#include <EntityMgr.h>
#include <Engine.h>

EntityMgr::EntityMgr(Engine *eng): Mgr(eng){
	count = 0;
	player = 0;
}

EntityMgr::~EntityMgr(){

}

void EntityMgr::CreateEntity(std::string meshfilename, Ogre::Vector3 pos){
	Entity381 *ent = new Entity381(this->engine, meshfilename, pos, count);
	count++;
	entities.push_back(ent);
}

void EntityMgr::CreatePlayer(Ogre::Vector3 pos){
	Tank *ent = new Tank(this->engine, "cube.mesh", pos, count);
	player = (Entity381*)ent;
	count++;
	entities.push_back((Entity381 *) ent);
}


void EntityMgr::CreateEntityOfTypeAtPosition(EntityTypes entType, Ogre::Vector3 pos){

	switch(entType){
	case PlayerType:
		CreatePlayer(pos);
		break;
	case EnemyTankType:
		CreateEntity("cube.mesh", pos);
		break;
	default:
		CreateEntity("robot.mesh", pos);
		break;
	}
}

void EntityMgr::Tick(float dt){
	for(int i = 0; i < count; i++){
		entities[i]->Tick(dt);
	}
}
