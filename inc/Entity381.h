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
  Ogre::Vector3 startPosition;
  Ogre::Vector3 position;
  Ogre::Vector3 velocity;
  bool isSelected;
  float desiredHeading, desiredSpeed;
  float heading, speed;


  int damageBoostCount; //Add 10 every time you pick up a damage boost, and -- it every fire
					    //If 0, no damage boost is applied, greater than 0, double damage

  float speedBoostCount; //Set it to some arbritrary number, - dt from it in tick, and modify speedMultiplier based on this
  int speedMultiplier; //Set to 1, is set to 2 instead if speedBoostCount is greater than 0

  float health;

  bool destroyed;

  float flashTimer;

  Ogre::SceneNode * muzzleSceneNode;

  std::vector<Aspect*> aspects;
  UnitAI * unitAI;


	//sound stuff from sound demo
	std::string soundFile;
	bool playSound;
	unsigned int auioId;
	bool didSelectSoundPlay; //part of selection process

  virtual void Tick(float dt);

  void DestroyEntity();
  void ResetEntity();

protected:


};

class Tank: Entity381 {
public:
	Tank(Engine *engine, std::string meshfilename, Ogre::Vector3 pos, int identity);
	virtual ~Tank();

	Ogre::ParticleSystem * particleSystem;
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
};


#endif // #ifndef __Entity381_h_
