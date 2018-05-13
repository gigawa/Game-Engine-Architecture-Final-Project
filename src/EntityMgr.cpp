/*
 * EntityMgr.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: sushil
 */

#include <EntityMgr.h>
#include <Engine.h>
#include <EntityItem.h>
#include <UnitAI.h>

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
	//ent->ogreEntity->setMaterialName("EnemyTank");
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
	//std::cout << "Created in MGR" << std::endl;
	items.push_back((EntityItem *) ent);
	//std::cout << "Pushed Back" << std::endl;
}

void EntityMgr::CreateDamageBoostItem(Ogre::Vector3 pos) {

	EntityItem *ent = new EntityItem(this->engine, "Bullet.mesh", pos, count);
	//std::cout << "Created * *DAMAGE BOOST* * in MGR" << std::endl;
	items.push_back((EntityItem *) ent);
	//std::cout << "Pushed Back" << std::endl;

}


void EntityMgr::CreateSpeedBoostItem(Ogre::Vector3 pos) {

	EntityItem *ent = new EntityItem(this->engine, "Speed.mesh", pos, count);
	//std::cout << "Created ~ ~SPEED BOOST~ ~ in MGR" << std::endl;
	ent->ogreEntity->setMaterialName("speed");
	items.push_back((EntityItem *) ent);
	//std::cout << "Pushed Back" << std::endl;

}

void EntityMgr::CreateHealthPackItem(Ogre::Vector3 pos) {

	EntityItem *ent = new EntityItem(this->engine, "Health.mesh", pos, count);
	//std::cout << "Created + +HEALTH PACK+ + in MGR" << std::endl;
	ent->ogreEntity->setMaterialName("health");
	items.push_back((EntityItem *) ent);
	//std::cout << "Pushed Back" << std::endl;

}

void EntityMgr::CreateWall(Ogre::Vector3 pos, Ogre::Vector3 scale) {
	Wall *ent = new Wall(this->engine, "cube.mesh", pos, count, scale);
	//std::cout << "Created in MGR" << std::endl;
	walls.push_back((Wall *) ent);
	//std::cout << "Pushed Back" << std::endl;
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
	case DamageBoostType:
		CreateDamageBoostItem(pos);
		break;
	case SpeedBoostType:
		CreateSpeedBoostItem(pos);
		break;
	case HealthPackType:
		CreateHealthPackItem(pos);
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

void EntityMgr::ResetEntities() {
	for(int i = 0; i < (int)entities.size(); i++) {
		if(entities[i] != player) {
			entities[i]->health = 30;
			entities[i]->destroyed = false;
			entities[i]->sceneNode->setVisible(true);
			entities[i]->position = entities[i]->startPosition;
			entities[i]->unitAI->commands.clear();
		}
	}

	for(int i = 0; i < (int)items.size(); i++) {
		items[i]->destroyed = false;
		items[i]->sceneNode->setVisible(true);
	}

	for(int i = 0; i < (int)walls.size(); i++) {
		walls[i]->destroyed = false;
		walls[i]->sceneNode->setVisible(true);
	}

	player->health = 100;
	player->position = player->startPosition;
}

void EntityMgr::ClearEntities() {
	for(int i = 0; i < (int)entities.size(); i++) {
		if(entities[i] != player) {
			entities[i]->DestroyEntity();
		}
	}

	for(int i = 0; i < (int)items.size(); i++) {
		items[i]->DestroyEntity();
	}

	for(int i = 0; i < (int)walls.size(); i++) {
		walls[i]->DestroyEntity();
	}

	//entities.clear();
	//items.clear();
	//walls.clear();

	//entities.push_back(player);

	player->health = 100;
	player->position = player->startPosition;
}
