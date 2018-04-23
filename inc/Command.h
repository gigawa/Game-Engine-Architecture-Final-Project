/*
 * Command.h
 *
 *  Created on: Mar 28, 2018
 *      Author: sushil/gary
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <Entity381.h>
#include <OgreVector3.h>
//#include <Types381.h>

class Command {

public:
	Command(Entity381* ent, COMMAND_TYPE ct);
	virtual ~Command();

	virtual void init();
	virtual void tick(float dt);
	virtual bool done();

	Entity381* entity;
	COMMAND_TYPE commandType;

};

class MoveTo: public Command {

public:
	MoveTo(Entity381* ent, Ogre::Vector3 location);
	~MoveTo();

	void init();
	void tick(float dt);
	bool done();

	Ogre::Vector3 targetLocation;
	float MOVE_DISTANCE_THRESHOLD;

};

class Intercept: public Command {

public:
	Intercept(Entity381* ent, Entity381 * targetEnt);
	~Intercept();

	void init();
	void tick(float dt);
	bool done();

	Entity381 * targetEntity;
	Ogre::Vector3 predictedPosition;
	float MOVE_DISTANCE_THRESHOLD;

};

class Follow: public Command {

public:
	Follow(Entity381* ent, Entity381 * targetEnt);
	~Follow();

	void init();
	void tick(float dt);
	bool done();

	Entity381 * targetEntity;
	float MOVE_DISTANCE_THRESHOLD;

};



#endif /* COMMAND_H_ */
