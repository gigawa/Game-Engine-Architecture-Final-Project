/*
 * Engine.h
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */

#ifndef ENGINE_H_
#define ENGINE_H_

class EntityMgr;
class GameMgr;
class GfxMgr;
class InputMgr;
class UIMgr;
//class SoundMgr;
#include <SoundMgr.h>

class Engine {
public:
	Engine();
	virtual ~Engine();

	EntityMgr *entityMgr;
	GameMgr   *gameMgr;
	GfxMgr    *gfxMgr;
	InputMgr  *inputMgr;
	UIMgr	  *uiMgr;
	OgreSND::SoundMgr*		soundMgr;
	//SoundMgr *soundMgr;

	void Init();
	void Run();
	void TickAll(float dt);
	void Cleanup();

	bool keepRunning;
};

#endif /* ENGINE_H_ */
