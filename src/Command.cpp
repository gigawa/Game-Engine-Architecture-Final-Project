/*
 * Command.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: gary
 */

#include "Command.h"
#include "Utils.h"
#include "UnitAI.h"

Command::Command(Entity381* ent, COMMAND_TYPE ct) {


}

Command::~Command() {

}

void Command::init(){



}

void Command::tick(float dt){



}

bool Command::done(){

	return true;

}


////////////////////////////////////////MOVETO

MoveTo::MoveTo(Entity381* ent, Ogre::Vector3 location) : Command(ent, MoveToCommand){
	targetLocation = location;
	MOVE_DISTANCE_THRESHOLD = 10;
	entity = ent;
	//std::cout << "Move To Created" << std::endl;
}

MoveTo::~MoveTo() {

}

void MoveTo::init(){

}

void MoveTo::tick(float dt){
	std::cout << "Move To Tick" << std::endl;
	//PrintVector(targetLocation);
	Ogre::Vector3 diff = targetLocation - entity->position;

	entity->desiredHeading = FixAngle(atan2(diff.z,diff.x) * 180/3.1415);

	if(diff.length() <= MOVE_DISTANCE_THRESHOLD) {
		entity->desiredSpeed = 0;
	}else {
		entity->desiredSpeed = entity->maxSpeed;
	}
}

bool MoveTo::done(){
	std::cout << "Move To Done" << std::endl;
	Ogre::Vector3 diff = targetLocation - entity->position;

	if(diff.length() < MOVE_DISTANCE_THRESHOLD) {
		entity->desiredSpeed = 0;
		return true;
	}else {
		return false;
	}

}

////////////////////////////////////////MOVETO

Follow::Follow(Entity381* ent, Entity381 * targetEnt) : Command(ent, FollowCommand){
	targetEntity = targetEnt;
	MOVE_DISTANCE_THRESHOLD = 10;
	entity = ent;
}

Follow::~Follow() {

}

void Follow::init(){

}

void Follow::tick(float dt){
	std::cout << "Follow Tick" << std::endl;
	Ogre::Vector3 diff = targetEntity->position - entity->position;

	entity->desiredHeading = FixAngle(atan2(diff.z,diff.x) * 180/3.1415);

	entity->desiredSpeed = entity->maxSpeed;
}

bool Follow::done(){
	Ogre::Vector3 diff = targetEntity->position - entity->position;
	EnemyTank * enemy = (EnemyTank*)entity;

	if(diff.length() < MOVE_DISTANCE_THRESHOLD || diff.squaredLength() > enemy->followDistance*enemy->followDistance || entity->position.squaredDistance(entity->startPosition) > enemy->range*enemy->range) {
		std::cout << "Done Following" << std::endl;
		enemy->following = false;
		entity->desiredSpeed = 0;
		MoveTo * move = new MoveTo(entity, entity->startPosition);
		entity->unitAI->AddCommand(move);
		return true;
	}else {
		return false;
	}

}

////////////////////////////////////////INTERCEPT

Intercept::Intercept(Entity381* ent, Entity381 * targetEnt) : Command(ent, InterceptCommand){
	targetEntity = targetEnt;
	entity = ent;
	MOVE_DISTANCE_THRESHOLD = 10;
}

Intercept::~Intercept() {

}

void Intercept::init(){



}

void Intercept::tick(float dt){
	entity->desiredSpeed = entity->maxSpeed;

	Ogre::Vector3 diff = targetEntity->position - entity->position;
	Ogre::Vector3 relVel = targetEntity->velocity - entity->velocity;

	float t = diff.length()/relVel.length();

	predictedPosition = targetEntity->position + (targetEntity->velocity * t);

	Ogre::Vector3 predictedDiff = predictedPosition - entity->position;

	entity->desiredHeading = FixAngle(atan2(predictedDiff.z,predictedDiff.x) * 180/3.1415);

}

bool Intercept::done(){

	Ogre::Vector3 diff = predictedPosition - entity->position;

	if(diff.length() < MOVE_DISTANCE_THRESHOLD) {
		std::cout << "Done" << std::endl;
		return true;
	}else {
		return false;
	}

}



