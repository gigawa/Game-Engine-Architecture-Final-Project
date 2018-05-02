/*
 * Aspect.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: sushil
 */

#include<Aspect.h>
#include <Entity381.h>  //Aspect includes Entity381.h


Aspect::Aspect(Entity381 *ent){
	entity = ent;
}

Aspect::~Aspect(){

}

void Aspect::Tick(float dt){

}

//////////////////////////////////////////////


Renderable::Renderable(Entity381* ent):Aspect(ent){

}

Renderable::~Renderable(){

}

void Renderable::Tick(float dt){
//do something;
	if(entity->health > 0) {
		entity->sceneNode->setPosition(entity->position); //now ogre should render the sceneNode at the new position...
		entity->sceneNode->resetOrientation(); // yaw is relative to 0
		entity->sceneNode->yaw(Ogre::Degree(-entity->heading));
	}else {

	}

}

//////////////////////////////////////////////



