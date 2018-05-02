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
#include <Entity381.h>
#include <Types381.h>
#include <SoundMgr.h>

UIMgr::UIMgr(Engine* eng): Mgr(eng){
	// Initialize the OverlaySystem (changed for Ogre 1.9)
	mOverlaySystem = new Ogre::OverlaySystem();

	mLabel = 0;
	enemiesLeftLabel = 0;
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
	//Buttons for weapons/boosts
	mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "WeaponButton", "Weapon");
	mTrayMgr->createButton(OgreBites::TL_BOTTOMLEFT, "BoostButton", "Boost");

	//Menu
	Ogre::StringVector options;
	options.push_back("Menu");
	options.push_back("Create Enemy Tank");
	options.push_back("START!");
	options.push_back("Load Demo Level");
	mTrayMgr->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "MyMenu", "Menu", 300, 4,options);

	//Splash screen
	mTrayMgr->showBackdrop("ECSLENT/UI");

	//Label
	mLabel = mTrayMgr->createLabel(OgreBites::TL_TOPRIGHT,"MyLabel","LABEL!!!",250);
	mLabel->setCaption("MENU!!!");

	//Health bar (progress bar)
	pbar = mTrayMgr->createProgressBar(OgreBites::TL_TOPLEFT,"HealthBar", "Health", 300, 100);
	pbar->setProgress(engine->entityMgr->player->health/100);

	//Text box displaying number of enemies left
	//int textDisplay = engine->entityMgr->entities.size();
	//Ogre::String test = "lol";
	enemiesLeftLabel = mTrayMgr->createLabel(OgreBites::TL_TOP,"MyKillfeedLabel", "Left: ", 100);

}

void UIMgr::Tick(float dt){
	mTrayMgr->refreshCursor();
	pbar->setProgress(engine->entityMgr->player->health/100);

	//Update enemies left label
	Ogre::String enemiesLeft = engine->entityMgr->player->IntToString(engine->entityMgr->entities.size() - 1);
	enemiesLeftLabel->setCaption("Left: " + enemiesLeft);


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

    switch(m->getSelectionIndex()){
    case 0:

    	std::cout <<"Pressed menu!" << std::endl;
    	break;
    case 1:
    	{ //encapsulated to not initialize variables unless case is verified
        Ogre::Vector3 spawnPos = engine->entityMgr->player->position;
        spawnPos.x += 1000;
    	std::cout <<"Pressed create enemy tank!" << std::endl;
    	mLabel->setCaption("MADE AN ENEMY!!!");
    	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,spawnPos);
    	m->selectItem(0,true);
    	break;
    	}
    case 2:

    	std::cout <<"STARTED!" << std::endl;
    	mTrayMgr->hideBackdrop();
    	mLabel->setCaption("STARTED!!!");
    	m->selectItem(0,true);
    	break;
    case 3:
    	{ //encapsulated to not initialize variables unless case is verified
    	std::cout <<"Loaded demo level!" << std::endl;
    	mLabel->setCaption("LETS GOOO!!!");

    	//Spawn 5 enemy tanks at varying positions related to the player

    	Ogre::Vector3 playerPos = engine->entityMgr->player->position;
    	Ogre::Real xDemo = playerPos.x;
    	Ogre::Real yDemo = playerPos.y; //unchanging Y coordinate
    	Ogre::Real zDemo = playerPos.z;

    	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(xDemo+500,yDemo,zDemo-100)));
    	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(xDemo+800,yDemo,zDemo+200)));
    	engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(xDemo+1000,yDemo,zDemo-150)));
    	//engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(xDemo+1300,yDemo,zDemo-300)));
    	//engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(xDemo+1400,yDemo,zDemo+100)));

    	m->selectItem(0,true);
    	break;
    	}
    default:
    	break;
    }


}


