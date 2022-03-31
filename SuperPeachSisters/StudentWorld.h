#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <list>



// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Peach;
class Actor;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  virtual ~StudentWorld();
  bool overlap(double x, double y, bool shouldBonk);
  bool overlapGoomba(double x, double y, bool shouldBonk, char dir);
  bool overlapWithPeach(double x, double y, char object);
  bool overlapWithPowerup(double x, double y, char goodie);
  bool objectCanBlock();
  bool overlapPlatform(double x, double y, char dir);
  void introduceNewObject(Actor* a);
  bool checkPower(char goodie);
  void changePower(bool b, char goodie);
  void changeInvincibility(int i);
  void changeHP(int i);
  void peachBonk();
  bool fall(double x, double y, double ex, double ey, bool shouldBonk);
  bool overlapPiranha(double x, double y);
  bool setPiranhaDirection(double x, double y);
  bool distanceBetweenPiranhaPeach(double x, double y);

private:
	std::list<Actor*> myActors;
	Peach* m_peach;
	StudentWorld* studentWorld;
	bool m_levelOver;
	bool m_gameOver;
};

#endif // STUDENTWORLD_H_
