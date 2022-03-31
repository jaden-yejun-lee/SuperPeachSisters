#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;

// Actor Base Class
class Actor : public GraphObject {
  public:
	  Actor(int imageID, int startX, int startY, int dir, int depth, double size, bool isAlive, StudentWorld* world); 
	  void kill();
	  bool isAlive();
	  StudentWorld* getWorld();
	  virtual bool isDamageable();
	  virtual void attemptToDamage();
	  virtual void bonk();
	  virtual bool blocksMovement();
	  virtual void doSomething() = 0;
	  virtual ~Actor();

  private:
	  bool m_alive;
	  StudentWorld* m_studentWorld;
	  
};

// PEACH
class Peach : public Actor {
public:
	Peach(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void doSomething();
	virtual bool isDamageable();
	virtual void attemptToDamage();
	virtual void bonk();
	void changeInvincibility(int i);
	bool checkInvincibility();
	void changePower(bool b, char goodie);
	bool checkPower(char goodie);
	void changeHP(int i);

private:
	int m_hp;
	int m_invincibility;
	int m_temp_invincibility;
	int m_jump_distance;
	bool m_shoot_power;
	bool m_jump_power;
	int m_time_to_recharge;
};

// OBSTACLES
class Obstacle : public Actor {
public:
	Obstacle(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void doSomething();	// do nothing
	virtual bool blocksMovement(); // true
};

class Block : public Obstacle {
  public: 
	  Block(int imageID, int startX, int startY, int dir, int depth, double size, int goodie, StudentWorld* world);
	  virtual void bonk();

  private: 
	  int m_goodie;
	  // 0 equals no goodie
	  // 1 equals flower goodie
	  // 2 equals mushroom goodie
	  // 3 equals star goodie
};

class Pipe : public Obstacle {
  public:
  	Pipe(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void bonk();

};

// ENDERS
class Ender : public Actor {
public:
	Ender(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void doSomething();
	virtual bool enderOverlap() = 0;
};

class Flag : public Ender {
	public:
		Flag(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
		virtual bool enderOverlap();
};

class Mario : public Ender {
public:
	Mario(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual bool enderOverlap();
};

// GOODIES
class Goodie : public Actor {
public:
	Goodie(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void changePowerScore() = 0;
	virtual void doSomething();
};


class Flower : public Goodie {
public:
	Flower(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void changePowerScore();
};

class Mushroom : public Goodie {
public:
	Mushroom(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void changePowerScore();
};

class Star : public Goodie {
public:
	Star(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void changePowerScore();
};

// PROJECTILES
class Projectile : public Actor {
public:
	Projectile(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void doSomething();
	virtual bool projectileOverlap() = 0;
};

class PeachFireball : public Projectile {
public:
	PeachFireball(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual bool projectileOverlap();
};

class PiranhaFireball : public Projectile {
public:
	PiranhaFireball(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual bool projectileOverlap();
};

class Shell : public Projectile {
public:
	Shell(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual bool projectileOverlap();
};

// ENEMIES
class Enemy : public Actor {
public:
	Enemy(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual bool isDamageable();

};
	// WALKINGENEMIES
class WalkingEnemy : public Enemy {
public:
	WalkingEnemy(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void doSomething();
};

class Goomba : public WalkingEnemy {
public:
	Goomba(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void attemptToDamage();
	virtual void bonk();
};

class Koopa : public WalkingEnemy {
public:
	Koopa(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void attemptToDamage();
	virtual void bonk();
};
	// STATIONARYENEMIES
class StationaryEnemy : public Enemy {
public:
	StationaryEnemy(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
};

class Piranha : public StationaryEnemy {
public:
	Piranha(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world);
	virtual void doSomething();
	virtual void attemptToDamage();
	virtual void bonk();
private:
	int m_firing_delay;
};



#endif // ACTOR_H_
