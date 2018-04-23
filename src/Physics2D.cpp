/*
 * Physics2D.cpp
 *
 *  Created on: Mar 1, 2018
 *      Author: sushil
 */


#include <Physics2D.h>
#include <Entity381.h>
#include <Utils.h>

Physics2D::Physics2D(Entity381* ent):Aspect(ent){

}

Physics2D::~Physics2D(){

}


void Physics2D::Tick(float dt){

	//Now do the trig
	entity->velocity.y = 0.0f; // just to be safe, we do not want ships in the air.
	entity->velocity.x = Ogre::Math::Cos(Ogre::Degree(entity->heading)) * entity->speed; //adjacent/hyp
	entity->velocity.z = Ogre::Math::Sin(Ogre::Degree(entity->heading)) * entity->speed; //opposite/hyp

	//This does not change!
	entity->position = entity->position + entity->velocity * dt;
}
