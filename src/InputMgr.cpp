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

	UpdateCamera(dt);
	UpdateVelocityAndSelection(dt);
}

void InputMgr::UpdateCamera(float dt){
	/*float move = 400.0f;
	float rotate = 0.1f;

	Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;

	//WSAD movements
	if (mKeyboard->isKeyDown(OIS::KC_W))
	{
		if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			dirVec.z -= 2.5*move;
		else
			dirVec.z -= move;
	}

	if (mKeyboard->isKeyDown(OIS::KC_S))
	{
		if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			dirVec.z += 2.5*move;
		else
			dirVec.z += move;
	}

	if (mKeyboard->isKeyDown(OIS::KC_A))
	{
		if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			dirVec.x -= 2.5*move;
		else
			dirVec.x -= move;
	}

	if (mKeyboard->isKeyDown(OIS::KC_D))
	{
		if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			dirVec.x += 2.5*move;
		else
			dirVec.x += move;
	}


	//Y Axis movement
	if (mKeyboard->isKeyDown(OIS::KC_R))
	{
		if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			dirVec.y += 2*move;
		else
			dirVec.y += move;
	}

	if (mKeyboard->isKeyDown(OIS::KC_F))
	{
		if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			dirVec.y -= 2*move;
		else
			dirVec.y -= move;
	}

	//Y Axis rotation
	if (mKeyboard->isKeyDown(OIS::KC_Q))
	{
		if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			engine->gameMgr->cameraPitchNode->yaw(Ogre::Degree(10 * rotate));
		else
			engine->gameMgr->cameraNode->yaw(Ogre::Degree(5 * rotate));
	}

	if (mKeyboard->isKeyDown(OIS::KC_E))
	{
		if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			engine->gameMgr->cameraNode->yaw(Ogre::Degree(-10 * rotate));
		else
			engine->gameMgr->cameraNode->yaw(Ogre::Degree(-5 * rotate));
	}

	//X Axis rotation
	if (mKeyboard->isKeyDown(OIS::KC_Z))
	{
		if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			engine->gameMgr->cameraPitchNode->pitch(Ogre::Degree(10 * rotate));
		else
			engine->gameMgr->cameraPitchNode->pitch(Ogre::Degree(5 * rotate));
	}

	if (mKeyboard->isKeyDown(OIS::KC_X))
	{
		if (mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			engine->gameMgr->cameraPitchNode->pitch(Ogre::Degree(-10 * rotate));
		else
			engine->gameMgr->cameraPitchNode->pitch(Ogre::Degree(-5 * rotate));
	}

	//Stops camera from going underneath ground plane (0 at Y)
	//set to height of 3 or higher so it looks better as a y check of 0 or 1
	//makes it so you can see the black beneath the ground if you just sit there
	Ogre::Vector3 camYCheck = engine->gameMgr->cameraNode->getPosition();
	//Ogre::Vector3 camYCheck = engine->gameMgr->cameraNode->getPosition();


	if(camYCheck.y > 3){
		engine->gameMgr->cameraNode->translate(dirVec * dt, Ogre::Node::TS_LOCAL);
		//engine->gameMgr->cameraNode->translate(dirVec * dt, Ogre::Node::TS_LOCAL);
	}
	else if(camYCheck.y <= 3){
		engine->gameMgr->cameraPitchNode->setPosition(Ogre::Vector3(camYCheck.x, 4, camYCheck.z));
		//engine->gameMgr->cameraNode->setPosition(Ogre::Vector3(camYCheck.x, 4, camYCheck.z));
	}


	//NOTE: This is how we originally translated the camera
	//engine->gameMgr->cameraNode->translate(dirVec * dt, Ogre::Node::TS_LOCAL);*/
}

void InputMgr::UpdateVelocityAndSelection(float dt){
	keyboardTimer -= dt;

	if((mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_I))){
		keyboardTimer = keyTime;
		engine->entityMgr->player->desiredSpeed = engine->entityMgr->player->maxSpeed;
	}else if((mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_K))){
		keyboardTimer = keyTime;
		engine->entityMgr->player->desiredSpeed = -1 * engine->entityMgr->player->maxSpeed;
	}else {
		engine->entityMgr->player->desiredSpeed = 0;
	}

	if(( mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_J))){
		keyboardTimer = keyTime;
		engine->entityMgr->player->desiredHeading -= deltaHeading;
		//turn left is decreasing degrees, turn right is increasing degrees because increasing degrees gives us the +ive Z axis
	}
	if((mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_L))){
		keyboardTimer = keyTime;
		engine->entityMgr->player->desiredHeading += deltaHeading;
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
	OIS::MouseState ms = mMouse->getMouseState();

	Ogre::Viewport * viewport = engine->gfxMgr->mCamera->getViewport();

	ms.width = viewport->getActualWidth();
	ms.height = viewport->getActualHeight();

	float msX = ms.X.abs;
	float msY = ms.Y.abs;

	Ogre::Vector2 mousePos = Ogre::Vector2(msX/ms.width, msY/ms.height);
	//std::cout << "Mouse Position: (" << mousePos.x << " ," << mousePos.y << ")" << std::endl;

	Ogre::Ray mouseRay = engine->gfxMgr->mCamera->getCameraToViewportRay(mousePos.x, mousePos.y);

	std::pair<bool, Ogre::Real> result = mouseRay.intersects(engine->gameMgr->groundPlane);

	if(result.first) {
		Ogre::Vector3 point = mouseRay.getPoint(result.second);
		//std::cout << "Selected Point: (" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;

		int entityIndex = -1;
		float closestDistance = 25000;

		for(int i = 0; i < (int) engine->entityMgr->entities.size(); i++) {
			float currentDistance = point.squaredDistance(engine->entityMgr->entities[i]->position);

			/*std::cout << "Index: " << i << std::endl;
			std::cout << "ClosestDistance: " << closestDistance << std::endl;
			std::cout << "CurrentDistance: " << currentDistance << std::endl;*/

			if(currentDistance < closestDistance) {
				//std::cout << "New Closest" << std::endl;
				closestDistance = currentDistance;
				entityIndex = i;
			}
		}

		/*if(entityIndex != -1){
			if(mid == OIS::MB_Left) {
				engine->entityMgr->SelectEntity(entityIndex);
			}else if(mid == OIS::MB_Right){
				Intercept * c = new Intercept(engine->entityMgr->selectedEntity, engine->entityMgr->entities[entityIndex]);
				UnitAI * ai = (UnitAI*) engine->entityMgr->selectedEntity->aspects[2];

				if(mKeyboard->isKeyDown(OIS::KC_LSHIFT)) {
					ai->AddCommand(c);
				}else {
					ai->SetCommand(c);
				}
			}
		}else if(mid == OIS::MB_Right) {
			MoveTo * c = new MoveTo(engine->entityMgr->selectedEntity, point);
			UnitAI * ai = (UnitAI*) engine->entityMgr->selectedEntity->aspects[2];

			if(mKeyboard->isKeyDown(OIS::KC_LSHIFT)) {
				ai->AddCommand(c);
			}else {
				ai->SetCommand(c);
			}
		}*/
	}

	return true;
}

bool InputMgr::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID mid){
	return true;
}
