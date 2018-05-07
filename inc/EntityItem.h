/*
 * EntityItem.h
 *
 *  Created on: May 1, 2018
 *      Author: grant
 */

#ifndef INC_ENTITYITEM_H_
#define INC_ENTITYITEM_H_

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <Aspect.h>
#include <Types381.h>

#include <Engine.h>

class EntityItem {
public:
	EntityItem(Engine *engine, std::string meshfilename, Ogre::Vector3 pos, int identity);
	virtual ~EntityItem();

	virtual void ApplyItem();

	Engine *engine;

	int identity;
	std::string name;
	std::string meshfilename;
	Ogre::SceneNode* sceneNode;
	Ogre::Entity*    ogreEntity;

	//dynamic data
	Ogre::Vector3 position;

	bool destroyed;

	void DestroyEntity();
};

class Wall {
public:
	Wall(Engine *engine, std::string meshfilename, Ogre::Vector3 pos, int identity, Ogre::Vector3 scale);
	virtual ~Wall();

	Engine *engine;

	int identity;
	std::string name;
	std::string meshfilename;
	Ogre::SceneNode* sceneNode;
	Ogre::Entity*    ogreEntity;

	//dynamic data
	Ogre::Vector3 position;

	bool destroyed;

	void DestroyEntity();
};

#endif /* INC_ENTITYITEM_H_ */
