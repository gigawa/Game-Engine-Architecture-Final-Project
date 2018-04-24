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

UIMgr::UIMgr(Engine* eng): Mgr(eng){
	// Initialize the OverlaySystem (changed for Ogre 1.9)
	mOverlaySystem = new Ogre::OverlaySystem();

	mLabel = 0;
	mTrayMgr = 0;

	engine->gfxMgr->mSceneMgr->addRenderQueueListener(mOverlaySystem);

}

UIMgr::~UIMgr() {
}


void UIMgr::Init(){
	//init sdktrays

    mInputContext.mKeyboard = engine->inputMgr->mKeyboard;
    mInputContext.mMouse = engine->inputMgr->mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", engine->gfxMgr->mWindow, mInputContext, this);
    //mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    //mTrayMgr->hideCursor();
}

void UIMgr::stop(){

}

void UIMgr::LoadLevel(){
	mTrayMgr->createButton(OgreBites::TL_TOPLEFT, "MyButton", "Click this!");

	Ogre::StringVector options;
	options.push_back("Selection 1");
	options.push_back("Selection 2");
	options.push_back("Selection 3");
	options.push_back("Selection 4");
	mTrayMgr->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "MyMenu", "Menu", 300, 4,options);

	mTrayMgr->showBackdrop("ECSLENT/UI");

	mLabel = mTrayMgr->createLabel(OgreBites::TL_LEFT,"MyLabel","LABEL!!!",250);

	OgreBites::ProgressBar * pbar;
	pbar = mTrayMgr->createProgressBar(OgreBites::TL_TOP,"HealthBar", "Health", 300, 200);
	pbar->setProgress(100);
}

void UIMgr::Tick(float dt){
	mTrayMgr->refreshCursor();
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

    if(b->getName()=="MyButton") {
        std::cout <<"Pressed button!" << std::endl;
    }

}

void UIMgr::itemSelected(OgreBites::SelectMenu *m){


    Ogre::Vector3 pos;
    pos.x = 0;
    pos.y = 0;
    pos.z = 100;
    switch(m->getSelectionIndex()){
    case 0:
    	std::cout <<"Pressed selection 1!" << std::endl;
    	mLabel->setCaption("SELECTION 1!!!");
    	break;
    case 1:
    	//engine->entityMgr->CreateEntityOfTypeAtPosition(SpeedBoatType,pos);
    	std::cout <<"Pressed selection 2!" << std::endl;
    	mLabel->setCaption("SELECTION 2!!!");
    	break;
    case 2:
    	//engine->entityMgr->CreateEntityOfTypeAtPosition(DDG51Type,pos);
    	std::cout <<"Pressed selection 3!" << std::endl;
    	mLabel->setCaption("SELECTION 3!!!");
    	break;
    case 3:
    	//engine->entityMgr->CreateEntityOfTypeAtPosition(CarrierType,pos);
    	//mLabel->setCaption("Carrier has Arrived!");
    	std::cout <<"Pressed selection 4!" << std::endl;
    	mLabel->setCaption("SELECTION 4!!!");
    	break;
    default:
    	break;
    }


}


