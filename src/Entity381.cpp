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

	Ogre::Entity * muzzleEntity = engine->gfxMgr->mSceneMgr->createEntity("cube.mesh");
	muzzleEntity->setMaterialName("MuzzleFlash");
	muzzleSceneNode = sceneNode->createChildSceneNode(Ogre::Vector3(50, 30, 0));
	muzzleSceneNode->scale(Ogre::Vector3(0.001, 0.5, 0.5));
	muzzleSceneNode->attachObject(muzzleEntity);
	muzzleSceneNode->setVisible(false);
	//sceneNode->showBoundingBox(true);

	this->startPosition = pos;

	Physics2D* phx = new Physics2D(this);
	aspects.push_back((Aspect*) phx);
	Renderable * renderable = new Renderable(this);
	aspects.push_back((Aspect*)renderable);
	unitAI = new UnitAI(this);
	aspects.push_back((Aspect*)unitAI);

	this->acceleration = 0;
	this->desiredHeading = this->heading = 0;
	this->turnRate = 0;
	this->desiredSpeed = this->speed = 0;
	this->minSpeed = this->maxSpeed = 0;
	this->health = 0;
	this->flashTimer = 0;

	//Sound initializations
	this->soundFile = "Boat-Sound.wav";
	this->playSound = false;
	this->auioId = 0;
	this->didSelectSoundPlay = false;
	destroyed = false;


}

void Entity381::DestroyEntity() {
	destroyed = true;
	sceneNode->setVisible(false);
	muzzleSceneNode->setVisible(false);
	/*sceneNode->detachAllObjects();
	engine->gfxMgr->mSceneMgr->destroySceneNode(sceneNode);
	engine->gfxMgr->mSceneMgr->destroyEntity(ogreEntity);*/
}

Entity381::~Entity381(){

}

void Entity381::Tick(float dt){
	for(unsigned int i = 0; i < aspects.size(); i++){
		aspects[i]->Tick(dt);
	}

	if(flashTimer > 0) {
		flashTimer -= dt;
		muzzleSceneNode->setVisible(true);
	}else {
		flashTimer = 0;
		muzzleSceneNode->setVisible(false);
	}
}

std::string Entity381::IntToString(int x){
	char tmp[10000];
	sprintf(tmp, "%i", x);
	return std::string(tmp);
}

//-------------------------------------------------------------------------------------------------------------------------------
Tank::Tank(Engine *engine, std::string meshfname, Ogre::Vector3 pos, int ident):
								Entity381(engine, meshfname, pos, ident){
	this->minSpeed = 0;
	this->maxSpeed = 100.0f;//meters per second...
	this->acceleration = 50.0f; // fast
	this->turnRate = 2.0f; //4 degrees per second
	this->health = 100;

	this->damageBoostCount = 0;
	//std::cout << "Created: " << this->name << std::endl;

	this->speedBoostCount = 0;
	this->speedMultiplier = 1;

	this->ogreEntity->setMaterialName("PlayerTank");

	particleSystem = 0;

	/*// create a particle system named explosions using the explosionTemplate
	particleSystem = engine->gfxMgr->mSceneMgr->createParticleSystem("explosions", "explosionTemplate");

	Ogre::SceneNode * particleNode = sceneNode->createChildSceneNode("Explosion Node");

	// attach the particle system to a scene node
	particleNode->attachObject((Ogre::MovableObject*)particleSystem);*/
}

Tank::~Tank(){

}

EnemyTank::EnemyTank(Engine *engine, std::string meshfname, Ogre::Vector3 pos, int ident):
								Entity381(engine, meshfname, pos, ident){
	this->minSpeed = 0;
	this->maxSpeed = 100.0f;//meters per second...
	this->acceleration = 50.0f; // fast
	this->turnRate = 2.0f; //4 degrees per second
	this->health = 30;
	this->ogreEntity->setMaterialName("EnemyTank");
	following = false;
	followDistance = 750;
	range = 750;
	shootTime = 1;
	shotTimer = 1;
	std::cout << "Created: " << this->name << std::endl;
}

void EnemyTank::Tick(float dt) {
	for(unsigned int i = 0; i < aspects.size(); i++){
		aspects[i]->Tick(dt);
	}

	int followSqrDistance = followDistance*followDistance;
	Entity381 * player = engine->entityMgr->player;

	if(position.squaredDistance(player->position) < followSqrDistance && position.squaredDistance(startPosition) < range*(range*0.9)) {
		shotTimer -= dt;

		if(shotTimer <= 0) {
			Shoot();
			shotTimer = shootTime;
		}

		if(!following) {

			Follow * f = new Follow(this, player);
			following = true;

			unitAI->SetCommand(f);
		}
	}

	if(flashTimer > 0) {
		flashTimer -= dt;
		muzzleSceneNode->setVisible(true);
	}else {
		flashTimer = 0;
		muzzleSceneNode->setVisible(false);
	}
}

void EnemyTank::Shoot() {
	flashTimer = 0.05;
	Ogre::Vector3 tankDirection = Ogre::Vector3(Ogre::Math::Cos(Ogre::Degree(heading)), 0, Ogre::Math::Sin(Ogre::Degree(heading)));

	Ogre::Ray bulletRay = Ogre::Ray(position, tankDirection);

	Entity381 * player = engine->entityMgr->player;

	std::pair<bool, Ogre::Real> result = bulletRay.intersects(player->sceneNode->_getWorldAABB());

	if (result.first) {
		//std::cout << "Hit Position: ";
		//PrintVector(bulletRay.getPoint(result.second));
		player->health -= 10;
		std::cout << "Hit Health: " << player->health << std::endl;

		//NOTE: Added sound of on-hit
		unsigned int playPlayerOnHit11 = 11;
		engine->soundMgr->playAudio(playPlayerOnHit11);

	} else {
		std::cout << "Not Hit" << std::endl;
	}
}

EnemyTank::~EnemyTank(){

}
