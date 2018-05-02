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
	destroyed = true;
	sceneNode->setVisible(false);
	engine->gameMgr->itemsLeft--;
	std::cout << "Items Left: " << engine->gameMgr->itemsLeft << std::endl;
}

