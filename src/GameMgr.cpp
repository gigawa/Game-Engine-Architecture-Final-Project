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
#include <UIMgr.h>

#include <iostream>
#include <Types381.h>

#include <OgreOverlay.h>
#include <OgreSceneNode.h>
#include <OgreMeshManager.h>


GameMgr::GameMgr(Engine *engine): Mgr(engine) {
	cameraNode = 0;
	cameraPitchNode = 0;
	itemsLeft = 0;
	currentLevel = 0;
	levelLoaded = false;
	interim = false;
	createdPlayer = false;
	continueCheck = false;
}

GameMgr::~GameMgr() {
	// TODO Auto-generated destructor stub
}

void GameMgr::Init(){

}

void GameMgr::Tick(float dt) {
	if(engine->entityMgr->player->health <= 0) {
		engine->entityMgr->ResetEntities();
		//MakeEntities();
		//MakePlayer();
	}

	//Do checking for currentlevel for each
	int enemiesLeft = 0;

	for(int i = 0; i < engine->entityMgr->count; i++){
		if(!engine->entityMgr->entities[i]->destroyed){
			enemiesLeft++;
		}
	}

	//std::cout << "Enemiesleft: " << enemiesLeft << std::endl;

	//FRAMEWORK FOR LEVEL CHECK: check for curr level's value and enemies left
	// then set interim to true and display the next level label
	// After that, wait for N, once it is pressed, continueCheck will be set to true
	// and the next portion will execute:
	// set the continue checker to false so it only execute once, set interim back to false
	// since we're playing again, hide the label, and then make the next level
	if(currentLevel == 1 && levelLoaded == true && enemiesLeft == 1){
		std::cout << "You have completed level one!" << std::endl;
		if(interim == false){
			engine->uiMgr->createNextLevelLabel();
			interim = true;
		}
	}

	if(continueCheck == true && currentLevel == 1){
		continueCheck = false;
		interim = false;

		engine->uiMgr->hideNextLevelLabel();

		engine->entityMgr->ClearEntities();
		MakeLVL2();
	}

}

void GameMgr::LoadLevel(){

	engine->gfxMgr->mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	Ogre::Light* light = engine->gfxMgr->mSceneMgr->createLight("MainLight");
	light->setPosition(20.0, 80.0, 50.0);

	MakeGround();
	MakeSky();
	MakeLVL1();
}

void GameMgr::MakePlayer() {
	if(!createdPlayer) {
		createdPlayer = true;
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

		//engine->soundMgr->listSourceAndBuffer();

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

		//engine->soundMgr->listSourceAndBuffer();

		//On-hit noise for when player is hit - onhit.wav
		std::string filename3 = "data/watercraft/sounds/player_onhit.wav";
		soundID++;

		engine->soundMgr->loadAudio(filename3,soundID);
		std::cout << "|     Loaded player_onhit.wav audio" << std::endl;

		engine->soundMgr->reserveAudio(filename3,false,soundID);
		std::cout << "|     Reserved player_onhit.wav to sound ID " << soundID << std::endl;

		//engine->soundMgr->listSourceAndBuffer();

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

		//engine->soundMgr->listSourceAndBuffer();

		//NOTE: add a power-up collection noise, power-up execution and deletion

		std::cout << "--- DONE REGISTERING SOUND" << std::endl;
	}else {
		std::cout << "Player Exists" << std::endl;
		Entity381 * player = engine->entityMgr->player;
		//player->ogreEntity->setMaterialName("PlayerTank");
		player->health = 100;
		player->position = Ogre::Vector3::ZERO;
	}

}

void GameMgr::MakeRoom1(Ogre::Vector3 pos) {
	//std::cout << "Make Walls" << std::endl;
	float wallDistance = 1000;
	float scale = 10;
	Ogre::Vector3 scalex = Ogre::Vector3(scale, 2, 1);
	Ogre::Vector3 scalez = Ogre::Vector3(1, 2, scale);
	Ogre::Vector3 nextPos = Ogre::Vector3(pos.x, 1, pos.z + wallDistance);
	engine->entityMgr->CreateWall(nextPos, scalex);
	nextPos = Ogre::Vector3(pos.x, 1, pos.z - wallDistance);
	engine->entityMgr->CreateWall(nextPos, scalex);
	nextPos = Ogre::Vector3(pos.x + wallDistance, 1, pos.z);
	engine->entityMgr->CreateWall(nextPos, scalez);
	nextPos = Ogre::Vector3(pos.x - wallDistance, 1, pos.z);
	engine->entityMgr->CreateWall(nextPos, scalez);

	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(pos.x + 175, 0, pos.z)));
	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(pos.x - 175, 0, pos.z)));
	//engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(pos.x, 0, pos.z + 175)));

	//Making a damage boost and speed boost item here
	//engine->gameMgr->MakeDamageBoostItem(Ogre::Vector3(pos.x, 20, pos.z));
	engine->gameMgr->MakeSpeedBoostItem(Ogre::Vector3(pos.x-400, 20, pos.z-350));
	engine->gameMgr->MakeHealthPackItem(Ogre::Vector3(pos.x-100, 20, pos.z+150));
}

void GameMgr::MakeRoom2(Ogre::Vector3 pos) {
	//std::cout << "Make Walls" << std::endl;
	float wallDistance = 1000;
	float scale = 10;
	Ogre::Vector3 scalex = Ogre::Vector3(scale, 2, 1);
	Ogre::Vector3 scalez = Ogre::Vector3(1, 2, scale);
	Ogre::Vector3 nextPos = Ogre::Vector3(pos.x, 1, pos.z + wallDistance);
	engine->entityMgr->CreateWall(nextPos, scalex);
	nextPos = Ogre::Vector3(pos.x, 1, pos.z - wallDistance);
	engine->entityMgr->CreateWall(nextPos, scalex);
	nextPos = Ogre::Vector3(pos.x + wallDistance, 1, pos.z);
	engine->entityMgr->CreateWall(nextPos, scalez);
	nextPos = Ogre::Vector3(pos.x - wallDistance, 1, pos.z);
	engine->entityMgr->CreateWall(nextPos, scalez);

	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(pos.x + 475, 0, pos.z - 500)));
	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(pos.x + 775, 0, pos.z + 300)));
	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(pos.x - 400, 0, pos.z + 475)));
	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(pos.x - 575, 0, pos.z - 200)));

	//Making a damage boost and speed boost item here
	engine->gameMgr->MakeDamageBoostItem(Ogre::Vector3(pos.x + 100, 20, pos.z));
	engine->gameMgr->MakeSpeedBoostItem(Ogre::Vector3(pos.x-600, 20, pos.z-550));
	engine->gameMgr->MakeHealthPackItem(Ogre::Vector3(pos.x-300, 20, pos.z+350));
}

void GameMgr::MakeRoom3(Ogre::Vector3 pos) {
	//std::cout << "Make Walls" << std::endl;
	float wallDistance = 750;
	float scale = 10;
	Ogre::Vector3 scalex = Ogre::Vector3(scale, 2, 1);
	Ogre::Vector3 scalez = Ogre::Vector3(1, 2, scale);
	Ogre::Vector3 nextPos = Ogre::Vector3(pos.x, 1, pos.z + wallDistance);
	engine->entityMgr->CreateWall(nextPos, scalex);
	nextPos = Ogre::Vector3(pos.x, 1, pos.z - wallDistance);
	engine->entityMgr->CreateWall(nextPos, scalex);
	nextPos = Ogre::Vector3(pos.x + wallDistance, 1, pos.z);
	engine->entityMgr->CreateWall(nextPos, scalez);
	nextPos = Ogre::Vector3(pos.x - wallDistance, 1, pos.z);
	engine->entityMgr->CreateWall(nextPos, scalez);

	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(pos.x + 175, 0, pos.z)));
	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(pos.x - 175, 0, pos.z)));
	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(pos.x, 0, pos.z + 175)));

	//Making a damage boost and speed boost item here
	engine->gameMgr->MakeDamageBoostItem(Ogre::Vector3(pos.x, 20, pos.z));
	engine->gameMgr->MakeSpeedBoostItem(Ogre::Vector3(pos.x-400, 20, pos.z-350));
	engine->gameMgr->MakeHealthPackItem(Ogre::Vector3(pos.x-100, 20, pos.z+150));
}

void GameMgr::MakeLVL1(){
	MakeBoundary();
	MakePlayer();

	Ogre::Vector3 roomPos = Ogre::Vector3(1000, 0, 600);
	MakeRoom1(roomPos);

	currentLevel = 1; //set to first level out of 3, this reflects that
	levelLoaded = true; //now we start accepting checks for when all entities are killed
}

void GameMgr::MakeLVL2(){

	std::cout << "`` In GameMgr::MakeLVL2" << std::endl;

	MakeBoundary();
	MakePlayer();

	Ogre::Vector3 roomPos = Ogre::Vector3(1000, 0, 600);
	MakeRoom2(roomPos);

	currentLevel = 2; //set to first level out of 3, this reflects that
	levelLoaded = true; //now we start accepting checks for when all entities are killed

	std::cout << "`` finished GameMgr::MakeLVL2" << std::endl;

}


void GameMgr::MakeLVL3(){
	MakeBoundary();
	MakePlayer();

	Ogre::Vector3 roomPos = Ogre::Vector3(1000, 0, 600);
	MakeRoom3(roomPos);

	currentLevel = 3; //set to first level out of 3, this reflects that
	levelLoaded = true; //now we start accepting checks for when all entities are killed
}



void GameMgr::MakeBoundary(){
	//std::cout << "Make Walls" << std::endl;
	float scale = 75;
	float wallDistance = 3000;
	Ogre::Vector3 scalex = Ogre::Vector3(scale, 2, 1);
	Ogre::Vector3 scalez = Ogre::Vector3(1, 2, scale);
	Ogre::Vector3 pos = Ogre::Vector3(0, 1, wallDistance);
	engine->entityMgr->CreateWall(pos, scalex);
	pos = Ogre::Vector3(0, 1, -wallDistance);
	engine->entityMgr->CreateWall(pos, scalex);
	pos = Ogre::Vector3(wallDistance, 1, 0);
	engine->entityMgr->CreateWall(pos, scalez);
	pos = Ogre::Vector3(-wallDistance, 1, 0);
	engine->entityMgr->CreateWall(pos, scalez);
}

void GameMgr::MakeItem(Ogre::Vector3 pos) {
	engine->entityMgr->CreateEntityOfTypeAtPosition(ItemType, pos);
	itemsLeft++;
}

void GameMgr::MakeDamageBoostItem(Ogre::Vector3 pos){
	engine->entityMgr->CreateEntityOfTypeAtPosition(DamageBoostType, pos);
	itemsLeft++;
}


void GameMgr::MakeSpeedBoostItem(Ogre::Vector3 pos){
	engine->entityMgr->CreateEntityOfTypeAtPosition(SpeedBoostType, pos);
	itemsLeft++;
}

void GameMgr::MakeHealthPackItem(Ogre::Vector3 pos){
	engine->entityMgr->CreateEntityOfTypeAtPosition(HealthPackType, pos);
	itemsLeft++;
}

void GameMgr::MakeItems() {
	Ogre::Vector3 pos = Ogre::Vector3(500, 1, 0);
	int multiplier = 100;

	for(int i = 0; i < 5; i++) {
		pos.x += multiplier;
		engine->entityMgr->CreateEntityOfTypeAtPosition(ItemType, pos);
		itemsLeft++;
		std::cout << "Items Left: " << itemsLeft << std::endl;
	}
}

void GameMgr::MakeGround(){

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -10);
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
	//groundEntity->setMaterialName("Ocean2_Cg");
	//groundEntity->setMaterialName("Examples/Rockwall");
	groundEntity->setMaterialName("Tabletop");
}

void GameMgr::MakeSky(){
	engine->gfxMgr->mSceneMgr->setSkyBox(true, "Examples/MorningSkyBox");
}
