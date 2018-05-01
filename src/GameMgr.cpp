/*
 * GameMgr.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */

#include <Engine.h>

#include <EntityMgr.h>
#include <GameMgr.h>
#include <GfxMgr.h>
#include <SoundMgr.h>

#include <iostream>
#include <Types381.h>

#include <OgreOverlay.h>
#include <OgreSceneNode.h>
#include <OgreMeshManager.h>


GameMgr::GameMgr(Engine *engine): Mgr(engine) {
	cameraNode = 0;
	cameraPitchNode = 0;
}

GameMgr::~GameMgr() {
	// TODO Auto-generated destructor stub
}

void GameMgr::Init(){

}

void GameMgr::LoadLevel(){

	  engine->gfxMgr->mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	  Ogre::Light* light = engine->gfxMgr->mSceneMgr->createLight("MainLight");
	  light->setPosition(20.0, 80.0, 50.0);

	  MakePlayer();

	  MakeGround();
	  MakeSky();
	  MakeEntities();
}

void GameMgr::MakePlayer() {
	Ogre::Vector3 pos = Ogre::Vector3(0, 0, 0);
	engine->entityMgr->CreateEntityOfTypeAtPosition(PlayerType, pos);

	//A node to manipulate pitch for camera separate from yaw
	//cameraPitchNode = cameraNode->createChildSceneNode();
	cameraNode = engine->entityMgr->player->sceneNode->createChildSceneNode(Ogre::Vector3(-200, 80, -80));

	// A node to attach the camera to so we can move the camera node instead of the camera.
	//cameraNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	cameraPitchNode = cameraNode->createChildSceneNode();
	cameraPitchNode->attachObject(engine->gfxMgr->mCamera);
	engine->gfxMgr->mCamera->lookAt(Ogre::Vector3(0, 50, 0));



	//Registering sounds for the player
	std::cout << "--- BEGIN REGISTERING shoot.wav SOUND" << std::endl;

		//Shooting noise - shoot.wav
	std::string filename = "data/watercraft/sounds/shoot.wav";
	unsigned int soundID = 3;

	engine->soundMgr->loadAudio(filename,soundID);
	std::cout << "|     Loaded shoot.wav audio" << std::endl;

	engine->entityMgr->player->auioId = 3;

	engine->soundMgr->reserveAudio(filename,false,soundID);
	std::cout << "|     Reserved shoot.wav to sound ID " << soundID << std::endl;

	engine->soundMgr->listSourceAndBuffer();

		//Moving noise - moving.wav

	//TESTING RELEASE BUFFER
	//std::cout << "[[[[[ RELEASING SOURCE..." << std::endl;
	//engine->soundMgr->releaseSource(3);
	//std::cout << "[[[[[ SOURCE RELEASED." << std::endl;

	std::string filename2 = "data/watercraft/sounds/moving.wav";
	soundID++;

	engine->soundMgr->loadAudio(filename2,soundID);
	std::cout << "|     Loaded moving.wav audio" << std::endl;

	engine->soundMgr->reserveAudio(filename2,false,soundID);
	std::cout << "|     Reserved moving.wav to sound ID " << soundID << std::endl;

	engine->soundMgr->listSourceAndBuffer();

		//On-hit noise for when player is hit - onhit.wav
	std::string filename3 = "data/watercraft/sounds/player_onhit.wav";
	soundID++;

	engine->soundMgr->loadAudio(filename3,soundID);
	std::cout << "|     Loaded player_onhit.wav audio" << std::endl;

	engine->soundMgr->reserveAudio(filename3,false,soundID);
	std::cout << "|     Reserved player_onhit.wav to sound ID " << soundID << std::endl;

	engine->soundMgr->listSourceAndBuffer();

	//std::cout << "[[[[[ DELETING ALL BUFFERS..." << std::endl;
	//engine->soundMgr->deleteAllBuffers();
	//std::cout << "[[[[[ FINISHED DELETING ALL BUFFERS..." << std::endl;
	//engine->soundMgr->listSourceAndBuffer();


		//Destruction noise - destroyed.wav
	std::string filename4 = "data/watercraft/sounds/destroyed.wav";
	soundID++;

	engine->soundMgr->loadAudio(filename4,soundID);
	std::cout << "|     Loaded destroyed.wav audio" << std::endl;

	engine->soundMgr->reserveAudio(filename4,false,soundID);
	std::cout << "|     Reserved destroyed.wav to sound ID " << soundID << std::endl;

	engine->soundMgr->listSourceAndBuffer();

		//NOTE: add a power-up collection noise, power-up execution and deletion

	std::cout << "--- DONE REGISTERING SOUND" << std::endl;

}

void GameMgr::MakeEntities(){
	Ogre::Vector3 pos = Ogre::Vector3(0, 0, 1000);
	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType, pos);
}

void GameMgr::MakeGround(){

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	groundPlane = plane;

	  Ogre::MeshManager::getSingleton().createPlane(
	    "ground",
	    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	    plane,
	    15000, 15000, 20, 20,
	    true,
	    1, 5, 5,
	    Ogre::Vector3::UNIT_Z);

	  Ogre::Entity* groundEntity = engine->gfxMgr->mSceneMgr->createEntity("ground");
	  engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	  groundEntity->setCastShadows(false);
	  //groundEntity->setMaterialName("Ocean2_HLSL_GLSL");
	  //groundEntity->setMaterialName("OceanHLSL_GLSL");
	  groundEntity->setMaterialName("Ocean2_Cg");
	  //groundEntity->setMaterialName("NavyCg");
}

void GameMgr::MakeSky(){
	engine->gfxMgr->mSceneMgr->setSkyBox(true, "Examples/MorningSkyBox");
}
