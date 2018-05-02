/*
 * EntityMgr.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: sushil
 */

#include <EntityMgr.h>
#include <Engine.h>
#include <EntityItem.h>

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

void EntityMgr::CreateEnemy(Ogre::Vector3 pos) {
	EnemyTank *ent = new EnemyTank(this->engine, "EnemyTank.mesh", pos, count);
	count++;
	entities.push_back((Entity381 *) ent);
}

void EntityMgr::CreatePlayer(Ogre::Vector3 pos){
	Tank *ent = new Tank(this->engine, "PlayerTank.mesh", pos, count);
	player = (Entity381*)ent;
	count++;
	entities.push_back((Entity381 *) ent);
}

void EntityMgr::CreateItem(Ogre::Vector3 pos) {
	EntityItem *ent = new EntityItem(this->engine, "cube.mesh", pos, count);
	std::cout << "Created in MGR" << std::endl;
	items.push_back((EntityItem *) ent);
	std::cout << "Pushed Back" << std::endl;
}

void EntityMgr::DestroyEnemy(int index) {
	Entity381 * enemy = entities[index];
	//entities.erase(entities.begin() + index);
	//count--;
	enemy->DestroyEntity();

	//NOTE: Added destruction noise
	unsigned int playDestroyed13 = 13;
	engine->soundMgr->playAudio(playDestroyed13);
}

void EntityMgr::ApplyItem(int index) {
	EntityItem * item = items[index];
	item->ApplyItem();
}

void EntityMgr::CreateEntityOfTypeAtPosition(EntityTypes entType, Ogre::Vector3 pos){

	switch(entType){
	case PlayerType:
		CreatePlayer(pos);
		break;
	case EnemyTankType:
		CreateEnemy(pos);
		break;
	case ItemType:
		CreateItem(pos);
		break;
	default:
		CreateEntity("robot.mesh", pos);
		break;
	}
}

void EntityMgr::Tick(float dt){
	for(int i = 0; i < count; i++){
		if(entities[i]->destroyed == false) {
			entities[i]->Tick(dt);
		}
	}
}
