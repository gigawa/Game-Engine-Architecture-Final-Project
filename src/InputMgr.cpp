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

	if((mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W))){
		engine->entityMgr->player->desiredSpeed = engine->entityMgr->player->maxSpeed;
	}else if((mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S))){
		engine->entityMgr->player->desiredSpeed = -1 * engine->entityMgr->player->maxSpeed;
	}else {
		engine->entityMgr->player->desiredSpeed = 0;
	}

	if(( mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A))){
		engine->entityMgr->player->desiredHeading -= deltaHeading;
		//turn left is decreasing degrees, turn right is increasing degrees because increasing degrees gives us the +ive Z axis
	}
	if((mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D))){
		engine->entityMgr->player->desiredHeading += deltaHeading;
	}

	if((keyboardTimer < 0) && (mKeyboard->isKeyDown(OIS::KC_L))) {

		std::cout << "* - * You pressed L * - *" << std::endl;
		unsigned int playShoot3 = 7;
		bool playSoundCheck = engine->soundMgr->playAudio(playShoot3,false);
		std::cout << "playSoundCheck is: " << std::boolalpha << playSoundCheck << std::endl;
		std::cout << "* - * Done pressing L * - *" << std::endl;

	}



	if((keyboardTimer < 0) && (mKeyboard->isKeyDown(OIS::KC_SPACE))) {

		//If space is pressed, play shooting noise

		//std::cout << "...Playing shoot noise..." << std::endl;
		//engine->soundMgr->playSelectionSound(engine->entityMgr->player);
		//engine->soundMgr->playAudioSourceIndex(2);
		//std::string filename = "data/watercraft/sounds/shoot.wav";
		//engine->soundMgr->getBufferId(filename);
		//std::cout << "...Done playing shoot noise..." << std::endl;

		// end sound stuff for space button

		keyboardTimer = keyTime;

		Entity381 * player = engine->entityMgr->player;
		Ogre::Vector3 playerDirection = Ogre::Vector3(Ogre::Math::Cos(Ogre::Degree(player->heading)), 0, Ogre::Math::Sin(Ogre::Degree(player->heading)));

		bool hit = false;
		Ogre::Ray bulletRay = Ogre::Ray(player->position, playerDirection);

		for (int i = 0; i < (int)engine->entityMgr->entities.size() && !hit; i++) {
			Entity381 * testEntity = engine->entityMgr->entities[i];

			if (testEntity != engine->entityMgr->player) {
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
					testEntity->health -= 10;
					std::cout << "Hit Health: " << testEntity->health << std::endl;
					hit = true;
				} else {
					std::cout << "Not Hit" << std::endl;
				}
			}
		}
	}
}

void InputMgr::LoadLevel(){

}

bool InputMgr::keyPressed(const OIS::KeyEvent& ke){
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
