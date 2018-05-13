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

	void MakeLVL1();
	void MakeLVL2();
	void MakeLVL3();

	void MakePlayer();
	void MakeItems();
	void MakeBoundary();
	void MakeRoom1(Ogre::Vector3 pos);
	void MakeRoom2(Ogre::Vector3 pos);
	void MakeRoom3(Ogre::Vector3 pos);

	void MakeItem(Ogre::Vector3 pos);

	void MakeDamageBoostItem(Ogre::Vector3 pos);
	void MakeSpeedBoostItem(Ogre::Vector3 pos);
	void MakeHealthPackItem(Ogre::Vector3 pos);

	void Tick(float dt);

	Ogre::SceneNode *cameraNode;
	Ogre::SceneNode *cameraPitchNode;
	Ogre::Plane groundPlane;

	bool createdPlayer;

	int itemsLeft;

	int currentLevel; //stores current level, if at level 3 (last level) and no enemies left, player won
	bool levelLoaded; //set to true after making the first level, then is used in the tick to figure out
					  //when the player can move on/when to display the move on label
	bool interim; //in between levels, we don't want the checking for new level to always run
	bool continueCheck;

};

#endif /* GAMEMGR_H_ */
