#ifndef __EntityMgr_h_
#define __EntityMgr_h_

#include <vector>
#include <Mgr.h>
#include <Entity381.h>
#include <Types381.h>

class EntityMgr: public Mgr {
public:
  EntityMgr(Engine *engine);
  virtual ~EntityMgr();

  //Engine *engine;

  std::vector<Entity381*> entities;

  Entity381 * player;
  int count;

  //Ogre::SceneManager *sceneMgr;

  void CreateEntityOfTypeAtPosition(EntityTypes type, Ogre::Vector3 pos);
  void DestroyEnemy(int index);

  void Tick(float dt);

protected:

private:
  void CreateEntity(std::string meshfilename, Ogre::Vector3 pos);
  void CreatePlayer(Ogre::Vector3 pos);
  void CreateEnemy(Ogre::Vector3 pos);
  void CreateItem(Ogre::Vector3 pos);

  //NOTE: Moved count to public so UIMgr can see it
  //int count;

};

#endif // #ifndef __EntityMgr_h_
