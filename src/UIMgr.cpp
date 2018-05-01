/*
 * UIMgr.cpp
 *
 *  Created on: Apr 23, 2018
 *      Author: gary
 */

#include <UIMgr.h>
#include <Engine.h>
#include <GfxMgr.h>
#include <InputMgr.h>
#include <EntityMgr.h>
#include <Types381.h>
#include <SoundMgr.h>

UIMgr::UIMgr(Engine* eng): Mgr(eng){
	// Initialize the OverlaySystem (changed for Ogre 1.9)
	mOverlaySystem = new Ogre::OverlaySystem();

	mLabel = 0;
	mTrayMgr = 0;
	pbar = 0;

	engine->gfxMgr->mSceneMgr->addRenderQueueListener(mOverlaySystem);

}

UIMgr::~UIMgr() {
}


void UIMgr::Init(){
	//init sdktrays

    mInputContext.mKeyboard = engine->inputMgr->mKeyboard;
    mInputContext.mMouse = engine->inputMgr->mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", engine->gfxMgr->mWindow, mInputContext, this);
    //mTrayMgr->showFrameStats(OgreBites::TL_TOPRIGHT);
    //mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    //mTrayMgr->hideCursor();
}

void UIMgr::stop(){

}

void UIMgr::LoadLevel(){
	mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "WeaponButton", "Weapon");
	mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "BoostButton", "Boost");

	Ogre::StringVector options;
	options.push_back("Menu");
	options.push_back("Create Enemy Tank");
	options.push_back("START!");
	options.push_back("Selection 3");
	mTrayMgr->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "MyMenu", "Menu", 300, 4,options);

	mTrayMgr->showBackdrop("ECSLENT/UI");

	mLabel = mTrayMgr->createLabel(OgreBites::TL_TOPRIGHT,"MyLabel","LABEL!!!",250);
	mLabel->setCaption("MENU!!!");

	pbar = mTrayMgr->createProgressBar(OgreBites::TL_TOPLEFT,"HealthBar", "Health", 300, 100);
	pbar->setProgress(engine->entityMgr->player->health/100);
}

void UIMgr::Tick(float dt){
	mTrayMgr->refreshCursor();
	pbar->setProgress(engine->entityMgr->player->health/100);
}

void UIMgr::windowResized(Ogre::RenderWindow* rw){
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = engine->inputMgr->mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void UIMgr::windowClosed(Ogre::RenderWindow* rw){

}

bool UIMgr::keyPressed(const OIS::KeyEvent &arg) {
	std::cout << "Key Pressed: " << arg.key << std::endl;
	return true;
}
bool UIMgr::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
bool UIMgr::mouseMoved(const OIS::MouseEvent &arg){
    if (mTrayMgr->injectMouseMove(arg)) return true;
	return false;
}
bool UIMgr::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID mid) {
	std::cout << "Mouse clicked" << std::endl;
	if (mTrayMgr->injectMouseDown(me, mid)) return true;
	return false;
}
bool UIMgr::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    /* normal mouse processing here... */
	return false;
}

void UIMgr::buttonHit(OgreBites::Button *b){

    if(b->getName()=="WeaponButton") {
        std::cout <<"Pressed weapon button!" << std::endl;
    }
    else if(b->getName()=="BoostButton") {
        std::cout <<"Pressed boost button!" << std::endl;
    }

}

void UIMgr::itemSelected(OgreBites::SelectMenu *m){


    Ogre::Vector3 spawnPos = engine->entityMgr->player->position;
    spawnPos.x += 500;

    switch(m->getSelectionIndex()){
    case 0:
    	std::cout <<"Pressed menu!" << std::endl;
    	//mLabel->setCaption("MENU!!!");
    	break;
    case 1:
    	//engine->entityMgr->CreateEntityOfTypeAtPosition(SpeedBoatType,pos);
    	std::cout <<"Pressed create enemy tank!" << std::endl;
    	mLabel->setCaption("MADE AN ENEMY!!!");
    	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,spawnPos);
    	m->selectItem(0,true);
    	break;
    case 2:
    	//engine->entityMgr->CreateEntityOfTypeAtPosition(DDG51Type,pos);
    	std::cout <<"STARTED!" << std::endl;
    	mTrayMgr->hideBackdrop();
    	mLabel->setCaption("STARTED!!!");
    	m->selectItem(0,true);
    	break;
    case 3:
    	//engine->entityMgr->CreateEntityOfTypeAtPosition(CarrierType,pos);
    	//mLabel->setCaption("Carrier has Arrived!");
    	std::cout <<"Pressed selection 3!" << std::endl;
    	mLabel->setCaption("SELECTION 3!!!");
    	m->selectItem(0,true);
    	break;
    default:
    	break;
    }


}


