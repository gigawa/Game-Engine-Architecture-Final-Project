/*
 * UIMgr.h
 *
 *  Created on: Apr 23, 2018
 *      Author: gary
 */

#ifndef SRC_UIMGR_H_
#define SRC_UIMGR_H_

#include <OgreWindowEventUtilities.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <Mgr.h>

#include <SdkTrays.h>


class UIMgr : public Mgr, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener, public Ogre::WindowEventListener, public OgreBites::SdkTrayListener
{

protected:
	virtual void windowResized(Ogre::RenderWindow *rw);
	virtual void windowClosed(Ogre::RenderWindow *rw);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    void buttonHit(OgreBites::Button *b);
    void itemSelected(OgreBites::SelectMenu *m);

    void startUI();

public:
	UIMgr(Engine *engine);
	~UIMgr();

	virtual void Init();
	virtual void Tick(float dt);
	virtual void LoadLevel();
	virtual void stop();

	OgreBites::Label *nextLvlLabel;
	void createNextLevelLabel();
	void hideNextLevelLabel();
	void showNextLevelLabel();
	void editNextLevelLabel(); //Changes the caption to a congratulations message for game won

	OgreBites::InputContext mInputContext;
	OgreBites::SdkTrayManager* mTrayMgr;
	Ogre::OverlaySystem* mOverlaySystem;
	OgreBites::Label *mLabel;
	OgreBites::Label *enemiesLeftLabel;
	OgreBites::Label *damageBoostLabel; //displays # of ammo left with double damage boost
	OgreBites::Label *speedBoostLabel; //displays time left for speed boost
	OgreBites::ProgressBar * pbar;
	bool startedYet;



};

#endif /* SRC_UIMGR_H_ */












