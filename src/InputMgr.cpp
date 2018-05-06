/*
 * InputMgr.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */

#include <Engine.h>
#include <GfxMgr.h>
#include <InputMgr.h>
#include <EntityMgr.h>
#include <GameMgr.h>
#include <Command.h>
#include <UnitAI.h>
#include <UIMgr.h>

#include <Utils.h>

InputMgr::InputMgr(Engine *engine) : Mgr(engine) {

	this->mInputMgr = 0;
	this->mKeyboard = 0;
	this->mMouse = 0;
	this->keyboardTimer = keyTime;
	deltaSpeed = 10.0f;
	deltaHeading = 0.25f;
}

InputMgr::~InputMgr() {

}

void InputMgr::Init(){

	OIS::ParamList pl;
	size_t windowHandle = 0;
	std::ostringstream windowHandleStr;

	engine->gfxMgr->mWindow->getCustomAttribute("WINDOW", &windowHandle);
	windowHandleStr << windowHandle;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));

#if defined OIS_WIN32_PLATFORM
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif
	mInputMgr = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(
			mInputMgr->createInputObject(OIS::OISKeyboard, false));
	mMouse = static_cast<OIS::Mouse*>(
			mInputMgr->createInputObject(OIS::OISMouse, true));

	int left, top;
	unsigned int width, height, depth;

	engine->gfxMgr->mWindow->getMetrics(width, height, depth, left, top);
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

}



void InputMgr::Stop(){
	if(mInputMgr){
		mInputMgr->destroyInputObject(mMouse);
		mInputMgr->destroyInputObject(mKeyboard);

		OIS::InputManager::destroyInputSystem(mInputMgr);
		mInputMgr = 0;
	}
}

void InputMgr::Tick(float dt){
	//Must capture both every tick for buffered input to work
	mMouse->capture();
	mKeyboard->capture();
	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE)){
		engine->keepRunning = false;
	}

	//	mTrayMgr->frameRenderingQueued(fe);

	UpdateVelocityAndSelection(dt);
}

void InputMgr::UpdateVelocityAndSelection(float dt){
	keyboardTimer -= dt;


	//Added speed boost functionality/checking here
	if(engine->entityMgr->player->speedBoostCount > 0){
		engine->entityMgr->player->speedMultiplier = 2;
		engine->entityMgr->player->maxSpeed = 350.0f;
		engine->entityMgr->player->turnRate = 8.0f;
		engine->entityMgr->player->speedBoostCount -= dt;
	}
	else if(engine->entityMgr->player->speedBoostCount <= 0){
		engine->entityMgr->player->speedMultiplier = 1;
		engine->entityMgr->player->maxSpeed = 100.0f;
		engine->entityMgr->player->turnRate = 2.0f;
	}



	if((mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W))){

		//NOTE: If W/UP, S/DOWN, A/LEFT, or D/RIGHT is pressed, play moving noise

		unsigned int playMoving9 = 9;
		engine->soundMgr->playAudio(playMoving9,false);

		// end sound stuff for moving


		engine->entityMgr->player->desiredSpeed = engine->entityMgr->player->maxSpeed;
	}
	else if((mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S))){
		unsigned int playMoving9 = 9;
		engine->soundMgr->playAudio(playMoving9,false);

		engine->entityMgr->player->desiredSpeed = -1 * engine->entityMgr->player->maxSpeed;
	}
	else {
		engine->entityMgr->player->desiredSpeed = 0;
	}//end if



	if(( mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A))){
		unsigned int playMoving9 = 9;
		engine->soundMgr->playAudio(playMoving9,false);

		engine->entityMgr->player->desiredHeading -= deltaHeading;
		//turn left is decreasing degrees, turn right is increasing degrees because increasing degrees gives us the +ive Z axis
	}//end if


	if((mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D))){
		unsigned int playMoving9 = 9;
		engine->soundMgr->playAudio(playMoving9,false);

		engine->entityMgr->player->desiredHeading += deltaHeading;
	}//end if



	if((keyboardTimer < 0) && (mKeyboard->isKeyDown(OIS::KC_SPACE))) {

		//NOTE: If space is pressed, play shooting noise

		unsigned int playShoot7 = 7;
		engine->soundMgr->playAudio(playShoot7,true);

		// end sound stuff for shooting

		//NOTE: Also decrease # of boosted bullets if you have more than 0

		std::cout << "Damage boost ammo left: " << engine->entityMgr->player->damageBoostCount << std::endl;

		if(engine->entityMgr->player->damageBoostCount > 0){
			engine->entityMgr->player->damageBoostCount--;
		}

		keyboardTimer = keyTime;

		Entity381 * player = engine->entityMgr->player;
		Ogre::Vector3 playerDirection = Ogre::Vector3(Ogre::Math::Cos(Ogre::Degree(player->heading)), 0, Ogre::Math::Sin(Ogre::Degree(player->heading)));

		bool hit = false;
		Ogre::Ray bulletRay = Ogre::Ray(player->position, playerDirection);

		for (int i = 0; i < (int)engine->entityMgr->entities.size() && !hit; i++) {
			Entity381 * testEntity = engine->entityMgr->entities[i];

			if (testEntity != engine->entityMgr->player && testEntity->destroyed == false) {
				std::pair<bool, Ogre::Real> result = bulletRay.intersects(testEntity->sceneNode->_getWorldAABB());

				/*std::cout << "Player Direction: ";
				PrintVector(playerDirection);
				std::cout << "Ray Origin: ";
				PrintVector(bulletRay.getOrigin());
				std::cout << "Ray Direction: ";
				PrintVector(bulletRay.getDirection());
				std::cout << "Entity Scene Node Position: ";
				PrintVector(engine->entityMgr->entities[i]->sceneNode->_getWorldAABB().getCenter());*/

				if (result.first) {
					//std::cout << "Hit Position: ";
					//PrintVector(bulletRay.getPoint(result.second));

					// If damage boost ammo is on, then deal double damage, else deal normal damage
					if(engine->entityMgr->player->damageBoostCount > 0){
						testEntity->health -= 20;
						std::cout << "(With Damage Boost) Hit Health: " << testEntity->health << std::endl;
						//engine->entityMgr->player->damageBoostCount--;
						hit = true;
					}
					else if (engine->entityMgr->player->damageBoostCount <= 0){
						testEntity->health -= 10;
						std::cout << "(Without Damage Boost) Hit Health: " << testEntity->health << std::endl;
						hit = true;
					}
				}
				else {
					std::cout << "Not Hit" << std::endl;
				}//end if
			}// end if
		}//end for
	}//end if

	//If player is no longer moving, and moving.wav is still playing, stop moving.wav
	//std::cout << "Desired speed is: " << engine->entityMgr->player->desiredSpeed << std::endl;
	unsigned int playMoving9 = 9;
	if(engine->entityMgr->player->desiredSpeed == 0 && engine->soundMgr->isAudioPlaying(playMoving9)){
		engine->soundMgr->stopAudio(playMoving9);
	}
}

void InputMgr::LoadLevel(){

}

bool InputMgr::keyPressed(const OIS::KeyEvent& ke){

	switch (ke.key){

		case OIS::KC_O:
			std::cout << "You pressed O" << std::endl;
			break;

		default:
			break;
	}

	return true;
}

bool InputMgr::keyReleased(const OIS::KeyEvent& ke){
	return true;
}


bool InputMgr::mouseMoved(const OIS::MouseEvent& me){

    if (engine->uiMgr->mTrayMgr->injectMouseMove(me)) return true;
	return true;
}

bool InputMgr::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID mid){

	std::cout << "Mouse pressed" << std::endl;
    if (engine->uiMgr->mTrayMgr->injectMouseDown(me, mid)) return true;

	return true;
}

bool InputMgr::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID mid){
	if (engine->uiMgr->mTrayMgr->injectMouseUp(me, mid)) return true;
	return true;
}
