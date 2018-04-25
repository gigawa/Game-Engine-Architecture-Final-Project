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

  float health;

  std::vector<Aspect*> aspects;

  virtual void Tick(float dt);

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

	UnitAI * unitAI;
	bool following;
	float followDistance;
};


#endif // #ifndef __Entity381_h_
