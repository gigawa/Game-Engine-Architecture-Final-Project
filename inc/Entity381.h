#ifndef __Entity381_h_
#define __Entity381_h_

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <Aspect.h>
#include <Types381.h>

#include <Engine.h>

class UnitAI;

class Entity381
{
public:
  Entity381(Engine *engine, std::string meshfilename, Ogre::Vector3 pos, int identity);
  virtual ~Entity381();
  std::string IntToString(int x);

  Engine *engine;

  //static data
  int identity;
  std::string name;
  std::string meshfilename;
  Ogre::SceneNode* sceneNode;
  Ogre::Entity*    ogreEntity;
  float acceleration, turnRate;
  float minSpeed, maxSpeed;

  //dynamic data
  Ogre::Vector3 position;
  Ogre::Vector3 velocity;
  bool isSelected;
  float desiredHeading, desiredSpeed;
  float heading, speed;


  int damageBoostCount; //Add 10 every time you pick up a damage boost, and -- it every fire
					    //If 0, no damage boost is applied, greater than 0, double damage

  float health;

  bool destroyed;

  std::vector<Aspect*> aspects;
  UnitAI * unitAI;


	//sound stuff from sound demo
	std::string soundFile;
	bool playSound;
	unsigned int auioId;
	bool didSelectSoundPlay; //part of selection process

  virtual void Tick(float dt);

  void DestroyEntity();

protected:


};

class Tank: Entity381 {
public:
	Tank(Engine *engine, std::string meshfilename, Ogre::Vector3 pos, int identity);
	virtual ~Tank();
};

class EnemyTank: Entity381 {
public:
	EnemyTank(Engine *engine, std::string meshfilename, Ogre::Vector3 pos, int identity);
	virtual ~EnemyTank();

	void Tick(float dt);
	void Shoot();

	float shootTime;
	float shotTimer;

	bool following;
	int followDistance;
	int range;
	Ogre::Vector3 startPosition;
};


#endif // #ifndef __Entity381_h_
