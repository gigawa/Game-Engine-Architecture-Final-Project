/*
 * UIMgr.cpp
 *
 *  Created on: Apr 23, 2018
 *      Author: gary
 */

#include <UIMgr.h>
#include <Engine.h>
#include <GfxMgr.h>
#include <GameMgr.h>
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
	startedYet = false;

	speedBoostLabel = 0;
	damageBoostLabel = 0;

	nextLvlLabel = 0;

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

	//Instead of loading all the stuff immediately, load it all AFTER you press the start button.

	//startedYet = false;
	mTrayMgr->createButton(OgreBites::TL_CENTER,"StartButton","START");
	mTrayMgr->createButton(OgreBites::TL_CENTER,"CreditsButton","About");
	mTrayMgr->showBackdrop("ECSLENT/UI");

}

void UIMgr::Tick(float dt){


	mTrayMgr->refreshCursor();

	if(startedYet){ //health bar updating
		pbar->setProgress(engine->entityMgr->player->health/100);
	}

	if(startedYet){ //enemies left & boost labels updating

		int enemiesLeft = 0;

		for(int i = 0; i < engine->entityMgr->count; i++){

			if(!engine->entityMgr->entities[i]->destroyed){
				enemiesLeft++;
			}

		}

		Ogre::String stringEnemiesLeft = engine->entityMgr->player->IntToString(enemiesLeft - 1);
		enemiesLeftLabel->setCaption("Enemies Left: " + stringEnemiesLeft);

		Ogre::String stringDamageBoostLeft = engine->entityMgr->player->IntToString(engine->entityMgr->player->damageBoostCount);
		damageBoostLabel->setCaption("Double Damage Ammo: " + stringDamageBoostLeft);

		if(engine->entityMgr->player->speedBoostCount > 0){
		Ogre::String stringSpeedBoostLeft = engine->entityMgr->player->IntToString(engine->entityMgr->player->speedBoostCount);
		speedBoostLabel->setCaption("Speed boost - Time Left: " + stringSpeedBoostLeft);
		}
		else if(engine->entityMgr->player->speedBoostCount <= 0){
			speedBoostLabel->setCaption("Speed Boost Not On");
		} //end if

	}


}

void UIMgr::createNextLevelLabel(){

	nextLvlLabel = mTrayMgr->createLabel(OgreBites::TL_CENTER,"nextLevelLabel","Congratulations! Proceed by pressing N.",500);

}

void UIMgr::hideNextLevelLabel(){

	mTrayMgr->getWidget("nextLevelLabel")->hide();
	mTrayMgr->removeWidgetFromTray("nextLevelLabel");
}

void UIMgr::showNextLevelLabel(){

	mTrayMgr->getWidget("nextLevelLabel")->show();
	mTrayMgr->moveWidgetToTray("nextLevelLabel",OgreBites::TL_CENTER);

}

void UIMgr::editNextLevelLabel(){

	nextLvlLabel->setCaption("CONGRATULATIONS! You won! (Press ESC to quit)");

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

void UIMgr::startUI(){
	//Initialize all the usual UI elements that we need
	std::cout << "Calling startUI..." << std::endl;
	//mTrayMgr->removeWidgetFromTray("StartButton"); moved to after calling startUI in buttonPressed


	//Buttons for weapons/boosts
	damageBoostLabel = mTrayMgr->createLabel(OgreBites::TL_BOTTOMLEFT,"DamageBoostLabel", "Double Damage Ammo: ", 300);
	speedBoostLabel = mTrayMgr->createLabel(OgreBites::TL_BOTTOMLEFT, "SpeedBoostLabel", "Speed Boost Not On" , 300);

	//Menu
	Ogre::StringVector options;
	options.push_back("Menu");
	options.push_back("Create Enemy Tank");
	options.push_back("Create Damage Boost");
	options.push_back("Create Speed Boost");
	mTrayMgr->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "MyMenu", "Menu", 300, 4,options);

	//Splash screen hidden now
	mTrayMgr->hideBackdrop();

	//Label
	//mLabel = mTrayMgr->createLabel(OgreBites::TL_TOPRIGHT,"MyLabel","LABEL!!!",250);
	//mLabel->setCaption("MENU!!!");

	//Health bar (progress bar)
	pbar = mTrayMgr->createProgressBar(OgreBites::TL_TOPLEFT,"HealthBar", "Health", 300, 100);
	pbar->setProgress(engine->entityMgr->player->health/100);

	//Text box displaying number of enemies left
	//int textDisplay = engine->entityMgr->entities.size();
	//Ogre::String test = "lol";
	enemiesLeftLabel = mTrayMgr->createLabel(OgreBites::TL_TOP,"MyKillfeedLabel", "Left: ", 300);

	startedYet = true;
	std::cout << "startedYet is now set to: " << std::boolalpha << startedYet << std::endl;

	mTrayMgr->removeWidgetFromTray("StartButton");
	//mTrayMgr->destroyWidget("StartButton");

	std::cout << "Ending startUI." << std::endl;
}

void UIMgr::buttonHit(OgreBites::Button *b){

	if(!startedYet && b->getName()=="StartButton"){
		std::cout << "You started the game!" << std::endl;

		mTrayMgr->removeWidgetFromTray("CreditsButton");

		mTrayMgr->destroyWidget("CreditsButton");

		startUI();
		b->hide();
	}

	if(!startedYet && b->getName()=="CreditsButton"){
		std::cout << "Credits displayed!" << std::endl;

		mTrayMgr->getWidget("StartButton")->hide();
		mTrayMgr->getWidget("CreditsButton")->hide();


		mTrayMgr->createButton(OgreBites::TL_TOPLEFT,"CloseCreditsButton","Close", 100);
		mTrayMgr->createLabel(OgreBites::TL_TOP,"CreditsLabel", "Tabletop Tanks by Grant Hooks and Gary Situ. WASD to move. Space to shoot. Eliminate all enemies to win.", 1000);

		//b->hide();
	}

	if(!startedYet && b->getName()=="CloseCreditsButton"){
		std::cout << "Closed the credits!" << std::endl;

		mTrayMgr->removeWidgetFromTray("CloseCreditsButton");
		mTrayMgr->removeWidgetFromTray("CreditsLabel");

		mTrayMgr->getWidget("CloseCreditsButton")->hide();
		mTrayMgr->getWidget("CreditsLabel")->hide();

		mTrayMgr->destroyWidget("CloseCreditsButton");
		mTrayMgr->destroyWidget("CreditsLabel");

		mTrayMgr->getWidget("StartButton")->show();
		mTrayMgr->getWidget("CreditsButton")->show();

		//b->hide();
	}

    if(startedYet && b->getName()=="WeaponButton") {
        std::cout <<"Pressed weapon button!" << std::endl;
    }
    else if(startedYet && b->getName()=="BoostButton") {
        std::cout <<"Pressed boost button!" << std::endl;
    }

}

void UIMgr::itemSelected(OgreBites::SelectMenu *m){

	if(startedYet){
		switch(m->getSelectionIndex()){
		case 0:

			std::cout <<"Pressed menu!" << std::endl;
			break;
		case 1:
			{ //encapsulated to not initialize variables unless case is verified
			Ogre::Vector3 spawnPos = engine->entityMgr->player->position;
			spawnPos.x += 1000;
			std::cout <<"Pressed create enemy tank!" << std::endl;
			//mLabel->setCaption("MADE AN ENEMY!!!");
			engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,spawnPos);
			m->selectItem(0,true);
			break;
			}
		case 2:
			{
			Ogre::Vector3 spawnPos = engine->entityMgr->player->position;
			engine->gameMgr->MakeDamageBoostItem(Ogre::Vector3(spawnPos.x + 300, 20, spawnPos.z));
			m->selectItem(0,true);
			break;
		}
		case 3:
			{ //encapsulated to not initialize variables unless case is verified

			Ogre::Vector3 spawnPos = engine->entityMgr->player->position;
			engine->gameMgr->MakeSpeedBoostItem(Ogre::Vector3(spawnPos.x + 300, 20, spawnPos.z));
			m->selectItem(0,true);

				/* Old executed stuff
			std::cout <<"Loaded demo level!" << std::endl;
			//Spawn 5 enemy tanks & items at varying positions related to the player

			Ogre::Vector3 playerPos = engine->entityMgr->player->position;
			Ogre::Real xDemo = playerPos.x;
			Ogre::Real yDemo = playerPos.y; //unchanging Y coordinate
			Ogre::Real zDemo = playerPos.z;

			engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(xDemo+500,yDemo,zDemo-100)));
			engine->gameMgr->MakeItem(Ogre::Vector3(xDemo+500,yDemo,zDemo-100));
			engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(xDemo+800,yDemo,zDemo+200)));
			engine->gameMgr->MakeItem(Ogre::Vector3(xDemo+800,yDemo,zDemo+200));
			engine->entityMgr->CreateEntityOfTypeAtPosition(EnemyTankType,(Ogre::Vector3(xDemo+1000,yDemo,zDemo-150)));
			engine->gameMgr->MakeItem(Ogre::Vector3(xDemo+1000,yDemo,zDemo-150));
				*/

			m->selectItem(0,true);
			break;
			}
		default:
			break;
		}
	}


}


