/*
 * EntityItem.cpp
 *
 *  Created on: May 1, 2018
 *      Author: grant
 */

#include <EntityItem.h>
#include <GfxMgr.h>
#include <GameMgr.h>

#include<Entity381.h>
#include<Physics2D.h>
#include<UnitAI.h>
#include<EntityMgr.h>

EntityItem::EntityItem(Engine *engine, std::string meshfname, Ogre::Vector3 pos, int ident){

	this->engine = engine;


	meshfilename = meshfname;
	position = pos;
	identity = ident;

	char tmp[10000];
	sprintf(tmp, "%i", ident);
	std::string identString = std::string(tmp);

	name = meshfname;

	ogreEntity = engine->gfxMgr->mSceneMgr->createEntity(meshfilename);
	sceneNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode(pos);
	sceneNode->attachObject(ogreEntity);

	sceneNode->scale(0.25, 0.25, 0.25);
	destroyed = false;
	//sceneNode->showBoundingBox(true);

}

EntityItem::~EntityItem() {
	// TODO Auto-generated destructor stub
}

void EntityItem::ApplyItem() {

	//Two cases: if its type is "damage" then apply double damage boost,
	//else if its type is "speed" then apply speed boost. Use meshfilename
	//to discern between the two.

	if(meshfilename == "cube.mesh"){ //Damage boost portion
		destroyed = true;
		sceneNode->setVisible(false);
		engine->gameMgr->itemsLeft--;

		//engine->entityMgr->player->damageBoostCount = engine->entityMgr->player->damageBoostCount + 10;
		engine->entityMgr->player->damageBoostCount = 10; //Sets to 10 instead of adding so players can't stack dmg boosts

		std::cout << "items items items - - - You applied an item!" << std::endl;
		std::cout << "Items Left: " << engine->gameMgr->itemsLeft << std::endl;
	}
	else if(meshfilename == "sphere.mesh"){ //Speed boost portion
		destroyed = true;
		sceneNode->setVisible(false);
		engine->gameMgr->itemsLeft--;
		std::cout << "items items items - - - You applied an item!" << std::endl;
		std::cout << "Items Left: " << engine->gameMgr->itemsLeft << std::endl;
	}
}

Wall::Wall(Engine *engine, std::string meshfname, Ogre::Vector3 pos, int ident, Ogre::Vector3 scale){

	this->engine = engine;


	meshfilename = meshfname;
	position = pos;
	identity = ident;

	char tmp[10000];
	sprintf(tmp, "%i", ident);
	std::string identString = std::string(tmp);

	name = meshfname;

	ogreEntity = engine->gfxMgr->mSceneMgr->createEntity(meshfilename);
	sceneNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode(pos);
	sceneNode->attachObject(ogreEntity);
	sceneNode->scale(scale);
	//sceneNode->showBoundingBox(true);

}

Wall::~Wall() {
	// TODO Auto-generated destructor stub
}

