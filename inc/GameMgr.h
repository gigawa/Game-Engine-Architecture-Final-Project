/*
 * GameMgr.h
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */

#ifndef GAMEMGR_H_
#define GAMEMGR_H_

#include <Mgr.h>

class GameMgr: public Mgr {
public:
	GameMgr(Engine *engine);
	virtual ~GameMgr();

	void Init();
	void LoadLevel();

	void MakeGround();
	void MakeSky();
	void MakeEntities();
	void MakePlayer();
	void MakeItems();
	void MakeBoundary();
	void MakeRoom(Ogre::Vector3 pos);

	void MakeItem(Ogre::Vector3 pos);

	void MakeDamageBoostItem(Ogre::Vector3 pos);

	Ogre::SceneNode *cameraNode;
	Ogre::SceneNode *cameraPitchNode;
	Ogre::Plane groundPlane;

	int itemsLeft;

};

#endif /* GAMEMGR_H_ */
