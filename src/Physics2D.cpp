/*
 * Physics2D.cpp
 *
 *  Created on: Mar 1, 2018
 *      Author: sushil
 */


#include <Physics2D.h>
#include <Entity381.h>
#include <Utils.h>
#include <EntityMgr.h>

Physics2D::Physics2D(Entity381* ent):Aspect(ent){

}

Physics2D::~Physics2D(){

}


void Physics2D::Tick(float dt){

	bool hitEntity = false;
	Ogre::Vector3 hitDirection;

	for (int i = 0; i < (int)entity->engine->entityMgr->entities.size() && hitEntity == false; i++) {
		Entity381 * testEntity = entity->engine->entityMgr->entities[i];

		if (testEntity != entity && testEntity->destroyed == false) {
			bool result = entity->sceneNode->_getWorldAABB().intersects(testEntity->sceneNode->_getWorldAABB());

			if(result) {
				//std::cout << "Hit Entity" << std::endl;
				hitEntity = true;
				hitDirection = entity->position - testEntity->position;
			}
		}
	}

	for (int i = 0; i < (int)entity->engine->entityMgr->walls.size() && hitEntity == false; i++) {
		Wall * testEntity = entity->engine->entityMgr->walls[i];
		bool result = entity->sceneNode->_getWorldAABB().intersects(testEntity->sceneNode->_getWorldAABB());

		if(result) {
			//std::cout << "Hit Entity" << std::endl;
			hitEntity = true;
			hitDirection = entity->position - testEntity->position;
		}
	}

	if(entity->desiredHeading > entity->heading){
		if(entity->desiredHeading - entity->heading > 180) {
			entity->desiredHeading -= 360;
			entity->heading = Ogre::Math::lerp(entity->heading, entity->desiredHeading, dt * entity->turnRate);
		} else
			entity->heading = Ogre::Math::lerp(entity->heading, entity->desiredHeading, dt * entity->turnRate);
	} else if (entity->desiredHeading < entity->heading){
		if(entity->desiredHeading - entity->heading < -180) {
			entity->desiredHeading += 360;
			entity->heading = Ogre::Math::lerp(entity->heading, entity->desiredHeading, dt * entity->turnRate);
		} else
			entity->heading = Ogre::Math::lerp(entity->heading, entity->desiredHeading, dt * entity->turnRate);
	}

	//entity->heading = Ogre::Math::lerp(entity->heading, entity->desiredHeading, dt * entity->turnRate);
	if(!hitEntity) {
		entity->speed = Ogre::Math::Clamp(Ogre::Math::lerp(entity->speed, entity->desiredSpeed, dt * entity->acceleration), entity->maxSpeed*-1, entity->maxSpeed);
	}else {
		entity->speed = entity->desiredSpeed * -1;
	}
	entity->heading = FixAngle(entity->heading);

	//Now do the trig
	if(!hitEntity) {
		entity->velocity.y = 0.0f; // just to be safe, we do not want ships in the air.
		entity->velocity.x = Ogre::Math::Cos(Ogre::Degree(entity->heading)) * entity->speed; //adjacent/hyp
		entity->velocity.z = Ogre::Math::Sin(Ogre::Degree(entity->heading)) * entity->speed; //opposite/hyp
	}else {
		entity->velocity.y = 0.0f; // just to be safe, we do not want ships in the air.
		entity->velocity.x = Ogre::Math::Cos(Ogre::Degree(entity->heading)) * -0.1; //adjacent/hyp
		entity->velocity.z = Ogre::Math::Sin(Ogre::Degree(entity->heading)) * -0.1; //opposite/hyp
	}

	//This does not change!
	entity->position = entity->position + entity->velocity * dt;
}
