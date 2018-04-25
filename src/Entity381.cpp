/*
 * Entity381.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: sushil
 */

#include <GfxMgr.h>

#include<Entity381.h>
#include<Physics2D.h>
#include<UnitAI.h>
#include<EntityMgr.h>

std::string IntToString(int x){
	char tmp[10000];
	sprintf(tmp, "%i", x);
	return std::string(tmp);
}

Entity381::Entity381(Engine *engine, std::string meshfname, Ogre::Vector3 pos, int ident){

	this->engine = engine;


	meshfilename = meshfname;
	position = pos;
	velocity = Ogre::Vector3(0, 0, 0);
	identity = ident;
	isSelected = false;

	name = meshfname + IntToString(identity);

	ogreEntity = engine->gfxMgr->mSceneMgr->createEntity(meshfilename);
	sceneNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode(pos);
	sceneNode->attachObject(ogreEntity);

	Physics2D* phx = new Physics2D(this);
	aspects.push_back((Aspect*) phx);
	Renderable * renderable = new Renderable(this);
	aspects.push_back((Aspect*)renderable);

	this->acceleration = 0;
	this->desiredHeading = this->heading = 0;
	this->turnRate = 0;
	this->desiredSpeed = this->speed = 0;
	this->minSpeed = this->maxSpeed = 0;

}

Entity381::~Entity381(){

}

void Entity381::Tick(float dt){
	for(unsigned int i = 0; i < aspects.size(); i++){
		aspects[i]->Tick(dt);
	}
}

//-------------------------------------------------------------------------------------------------------------------------------
Tank::Tank(Engine *engine, std::string meshfname, Ogre::Vector3 pos, int ident):
								Entity381(engine, meshfname, pos, ident){
	this->minSpeed = 0;
	this->maxSpeed = 100.0f;//meters per second...
	this->acceleration = 50.0f; // fast
	this->turnRate = 2.0f; //4 degrees per second
	this->health = 100;
	std::cout << "Created: " << this->name << std::endl;
}

Tank::~Tank(){

}

EnemyTank::EnemyTank(Engine *engine, std::string meshfname, Ogre::Vector3 pos, int ident):
								Entity381(engine, meshfname, pos, ident){
	this->minSpeed = 0;
	this->maxSpeed = 100.0f;//meters per second...
	this->acceleration = 50.0f; // fast
	this->turnRate = 2.0f; //4 degrees per second
	following = false;
	followDistance = 500;
	std::cout << "Created: " << this->name << std::endl;
	unitAI = new UnitAI(this);
	aspects.push_back((Aspect*)unitAI);
}

void EnemyTank::Tick(float dt) {
	for(unsigned int i = 0; i < aspects.size(); i++){
		aspects[i]->Tick(dt);
	}

	int followSqrDistance = followDistance*followDistance;
	Entity381 * player = engine->entityMgr->player;

	if(!following && position.squaredDistance(player->position) < followSqrDistance) {
		Follow * f = new Follow(this, player);
		following = true;

		unitAI->AddCommand(f);
	}
}

EnemyTank::~EnemyTank(){

}
