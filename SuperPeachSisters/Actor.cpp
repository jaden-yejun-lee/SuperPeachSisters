#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>

// ACTOR
Actor::Actor(int imageID, int startX, int startY, int dir, int depth, double size, bool isAlive, StudentWorld* world) : GraphObject(imageID, startX, startY, dir, depth, size) {
	m_alive = isAlive;
	m_studentWorld = world;
	
}

void Actor::kill() {
	m_alive = false;
}

bool Actor::isAlive() {
	if (m_alive) {
		return true;
	}

	return false;
}

StudentWorld* Actor::getWorld() {
	return m_studentWorld;
}

bool Actor::isDamageable() {
	return false;
}

void Actor::attemptToDamage() {

}

void Actor::bonk() {

}

bool Actor::blocksMovement() {
	return false;
}

Actor::~Actor() {

}

// PEACH
Peach::Peach(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Actor(imageID, startX, startY, 0, 0, 1, true, world) {
	m_hp = 1;
	m_temp_invincibility = 0;
	m_invincibility = 0;
	m_jump_distance = 0;
	m_shoot_power = false;
	m_jump_power = false;
	m_time_to_recharge = 0;
}

void Peach::doSomething() {
	double curX = getX();
	double curY = getY();
	int ch;
	if (isAlive()) {
		// check if currently invincible (STAR POWER)
		if (m_invincibility > 0) {
			m_invincibility--;
		}
		//check if currently temporarily invincible (FIRE POWER OR JUMP POWER)
		if (m_temp_invincibility > 0) {
			m_temp_invincibility--;
		}
		// check recharge mode with shoot power
		if (m_time_to_recharge > 0) {
			m_time_to_recharge--;
		}
		// if peach currently overlaps with any game object, she must bonk the object
		if (getWorld()->overlap(curX, curY, true)) {
			// *it gets bonked in overlap
		}
		// if peach previously initiated a jump
		if (m_jump_distance > 0) {
			double targetX = getX();
			double targetY = getY() + 4;
			// peach tries to move upward
			if (getWorld()->overlap(targetX, targetY, true)) {
				// bonk is called in overlap
				m_jump_distance = 0;
			}
			else {
				moveTo(targetX, targetY);
				m_jump_distance--;
			}
		}
		// peach is on ground or falling
		else if (m_jump_distance == 0) {
			if (getWorld()->fall(getX(), getX() + SPRITE_WIDTH - 1, getY() - 3, getY(), false) == false) {
				moveTo(getX(), getY() - 4);
			}
		}
	}
	if (getWorld()->getKey(ch)) {
		switch (ch) {
			// move Peach left
		case KEY_PRESS_LEFT:
			setDirection(180);
			if (getWorld()->overlap(getX() - 4, getY(), true)) {
			}
			else {
				moveTo(getX() - 4, getY());
			}
			break;
			// move Peach right
		case KEY_PRESS_RIGHT:
			setDirection(0);
			if (getWorld()->overlap(getX() + 4, getY(), true)) {
			}
			else {
				moveTo(getX() + 4, getY());
			}
			break;
			// make Peach jump
		case KEY_PRESS_UP:
			// if there is an object that would block one pixel below
			if (getWorld()->objectCanBlock()) {
				if (m_jump_power == false) {
					m_jump_distance = 8;
				}
				else if (m_jump_power == true) {
					m_jump_distance = 12;
				}
				getWorld()->playSound(SOUND_PLAYER_JUMP);
			}
			break;
			// make peach shoot
		case KEY_PRESS_SPACE:
			if (m_shoot_power == false) {
			}
			else if (m_time_to_recharge > 0) {
			}
			else {
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				m_time_to_recharge = 8;
				if (getDirection() == 0) {
					getWorld()->introduceNewObject(new PeachFireball(IID_PEACH_FIRE, getX() + 4, getY(), 0, 1, 1, getWorld()));
				}
				else if (getDirection() == 180) {
					getWorld()->introduceNewObject(new PeachFireball(IID_PEACH_FIRE, getX() - 4, getY(), 180, 1, 1, getWorld()));
				}
			}
		}
	}
}

bool Peach::isDamageable() {
	return true;
}

void Peach::attemptToDamage() {
	this->bonk();
}

void Peach::bonk() {
	if (m_temp_invincibility > 0 || m_invincibility > 0) {
		return;
	}
	else {
		m_hp--;
		if (m_hp >= 1) {
			getWorld()->playSound(SOUND_PLAYER_HURT);
		}
		else if (m_hp <= 0) {
			kill();

		}
		m_temp_invincibility = 10;
		changePower(false, 'f');
		changePower(false, 'm');
	}
}

void Peach::changePower(bool b, char goodie) {
	switch (goodie) {
	case 'f':
		m_shoot_power = b;
		break;
	case 'm':
		m_jump_power = b;
		break;
	case 's':
		break;
	}

}

bool Peach::checkPower(char goodie) {
	switch (goodie) {
	case 'f':
		if (m_shoot_power == true) {
			return true;
		}
		return false;
		break;
	case 'm':
		if (m_jump_power == true) {
			return true;
		}
		return false;
		break;
	case 's':
		if (m_invincibility > 0) {
			return true;
		}
		return false;
		break;
	}
}

void Peach::changeHP(int i) {
	m_hp = i;
}

void Peach::changeInvincibility(int i) {
	m_invincibility = i;
}

bool Peach::checkInvincibility() {
	if (m_invincibility > 0) {
		return true;
	}
	return false;
}

//OBSTACLE
Obstacle::Obstacle(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Actor(imageID, startX, startY, 0, 2, 1, true, world) {

}

void Obstacle::doSomething() {

}

bool Obstacle::blocksMovement() {
	return true;
}


// BLOCK
Block::Block(int imageID, int startX, int startY, int dir, int depth, double size, int goodie, StudentWorld* world) : Obstacle(imageID, startX, startY, 0, 2, 1, world) {
	m_goodie = goodie;
}

void Block::bonk() {

	if (m_goodie == 0) {
		getWorld()->playSound(SOUND_PLAYER_BONK);
	}
	else {
		getWorld()->playSound(SOUND_POWERUP_APPEARS);
		
		switch (m_goodie) {
		// flower goodie
		case 1: 
			// introduce a new flower object
			getWorld()->introduceNewObject(new Flower(IID_FLOWER, getX(), getY() + 8, 0, 1, 1, getWorld()));
			break;
		case 2:
			// introdue a new mushroom object
			getWorld()->introduceNewObject(new Mushroom(IID_MUSHROOM, getX(), getY() + 8, 0, 1, 1, getWorld()));
			break;
		case 3:
			// introduce a new star object
			getWorld()->introduceNewObject(new Star(IID_STAR, getX(), getY() + 8, 0, 1, 1, getWorld()));
			break;
		}

		m_goodie = 0;
	}

}

// PIPE
Pipe::Pipe(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Obstacle(imageID, startX, startY, 0, 2, 1, world) {

}

void Pipe::bonk() {
	getWorld()->playSound(SOUND_PLAYER_BONK);
}

// ENDER
Ender::Ender(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Actor(imageID, startX, startY, 0, 1, 1, true, world) {

}

void Ender::doSomething() {
	if (isAlive()) {
		if (enderOverlap() == true) {
			getWorld()->increaseScore(1000);
			kill();
		}
	}
}


// FLAG
Flag::Flag(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Ender(imageID, startX, startY, 0, 1, 1, world) {
	
}

bool Flag::enderOverlap() {
	if (getWorld()->overlapWithPeach(getX(), getY(), 'f')) {
		return true;
	}
	return false;
}

// MARIO
Mario::Mario(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Ender(imageID, startX, startY, 0, 1, 1, world) {

}

bool Mario::enderOverlap() {
	if (getWorld()->overlapWithPeach(getX(), getY(), 'm')) {
		return true;
	}
	return false;
}

// GOODIE
Goodie::Goodie(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Actor(imageID, startX, startY, 0, 1, 1, true, world) {

}

void Goodie::doSomething() {
	// if goodie overlaps with peach
	if (getWorld()->overlapWithPeach(getX(), getY(), ' ')) {
		changePowerScore();	// EACH DO ACTIVITY SHOULD INCREASE SCORE AND CHANGEPOWER
		getWorld()->changeHP(2);
		this->kill();
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
	}
	// else move
	else {
		// check two pixels below
		if (getWorld()->fall(getX(), getX() + SPRITE_WIDTH - 1, getY() - 2, getY(), false) == false) {
			moveTo(getX(), getY() - 2);
		}
		double targetX1 = getX() + 2;
		double targetX2 = getX() - 2;
		double targetY = getY();
		if (getDirection() == 0) {
			// check two pixels to the right
			if (getWorld()->overlap(getX() + 2, getY(), false)) {
				setDirection(180);
				return;
			}
			else {
				moveTo(targetX1, targetY);
			}
		}
		else if (getDirection() == 180) {
			//check two pixels to the left
			if (getWorld()->overlap(getX() - 2, getY(), false)) {
				setDirection(0);
				return;
			}
			else {
				moveTo(targetX2, targetY);
			}
		}
	}
}

// FLOWER
Flower::Flower(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Goodie(imageID, startX, startY, 0, 1, 1, world) {
	
}


void Flower::changePowerScore() {
	getWorld()->increaseScore(50);
	getWorld()->changePower(true, 'f');
}

// MUSHROOM
Mushroom::Mushroom(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Goodie(imageID, startX, startY, 0, 1, 1, world) {

}

void Mushroom::changePowerScore() {
	getWorld()->increaseScore(75);
	getWorld()->changePower(true, 'm');
}

// STAR
Star::Star(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Goodie(imageID, startX, startY, 0, 1, 1, world) {

}

void Star::changePowerScore() {
	getWorld()->increaseScore(100);
	getWorld()->changeInvincibility(150);
}

Projectile::Projectile(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Actor(imageID, startX, startY, dir, 1, 1, true, world) {

}

void Projectile::doSomething() {	

	if (projectileOverlap() == true) {
		kill();
		return;
	}
	else {
		// check two pixels below
		if (getWorld()->fall(getX(), getX() + SPRITE_WIDTH - 1, getY() - 2, getY(), false) == false) {
			moveTo(getX(), getY() - 2);
		}

		double targetX1 = getX() + 2;
		double targetX2 = getX() - 2;
		double targetY = getY();
		if (getDirection() == 0) {
			// check two pixels to the right
			if (getWorld()->overlap(getX() + 2, getY(), false)) {
				kill();
				return;
			}
			else {
				moveTo(targetX1, targetY);
			}
		}
		else if (getDirection() == 180) {
			//check two pixels to the left
			if (getWorld()->overlap(getX() - 2, getY(), false)) {
				kill();
				return;
			}
			else {
				moveTo(targetX2, targetY);
			}
		}


	}
}

// PEACH FIREBALL
PeachFireball::PeachFireball(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Projectile(imageID, startX, startY, dir, 1, 1, world) {

}

bool PeachFireball::projectileOverlap() {
	if (getWorld()->overlapWithPowerup(getX(), getY(), 'f')) {
		return true;
	}
	return false;
}

// SHELL
Shell::Shell(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Projectile(imageID, startX, startY, dir, 1, 1, world) {

}

bool Shell::projectileOverlap() {
	if (getWorld()->overlapWithPowerup(getX(), getY(), 't')) {
		return true;
	}
	return false;
}

// PEACHFIREBALL
PiranhaFireball::PiranhaFireball(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Projectile(imageID, startX, startY, dir, 1, 1, world) {

}

bool PiranhaFireball::projectileOverlap() {
	if (getWorld()->overlapWithPeach(getX(), getY(), 'p')) {
		return true;
	}
	return false;
}


// ENEMY
Enemy::Enemy(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Actor(imageID, startX, startY, dir, 0, 1, true, world) {

}

bool Enemy::isDamageable() {
	return true;
}

WalkingEnemy::WalkingEnemy(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Enemy(imageID, startX, startY, dir, 0, 1, world) {

}

void WalkingEnemy::doSomething() {
	// if walkingEnemy is alive
	if (isAlive()) {
		// if walkingEnemy overlaps with peach 	
		if (getWorld()->overlapWithPeach(getX(), getY(), ' ')) {
			getWorld()->peachBonk();
			return;
		}
		// if walkingEnemy can move 1 pixel in current direction
		if (getDirection() == 0) {
			if (!getWorld()->overlapGoomba(getX() + 2, getY(), false, 'r')) {

			}
			else {
				setDirection(180);
			}
		}
		else if (getDirection() == 180) {
			if (!getWorld()->overlapGoomba(getX() - 2, getY(), false, 'l')) {

			}
			else {
				setDirection(0);
			}
		}
		// determine if walkingEnemy can move 1 pixel in current direction without stepping off platform
		if (getDirection() == 0) {
			// return true if there is an object at desired position, return false if there is no object and there is a space in floor
			if (!getWorld()->overlapPlatform(getX() + 1, getY() - 1, 'r')) {
				setDirection(180);
			}
		}
		else if (getDirection() == 180) {
			if (!getWorld()->overlapPlatform(getX() - 1, getY() - 1, 'l')) {
				setDirection(0);
			}
		}
		if (getDirection() == 0) {
			if (!getWorld()->overlapGoomba(getX() + 2, getY(), false, 'r')) {
				moveTo(getX() + 1, getY());
			}
			else {
				return;
			}
		}
		else if (getDirection() == 180) {
			if (!getWorld()->overlapGoomba(getX() - 2, getY(), false, 'l')) {
				moveTo(getX() - 1, getY());

			}
			else {
				return;
			}
		}
	}
}

// GOOMBA
Goomba::Goomba(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : WalkingEnemy(imageID, startX, startY, dir, 0, 1, world) {
	
}

void Goomba::attemptToDamage() {
	getWorld()->increaseScore(100);
	kill();
}

void Goomba::bonk() {
	if (getWorld()->checkPower('s')) {
				getWorld()->playSound(SOUND_PLAYER_KICK);
				getWorld()->increaseScore(100);
				kill();
	}
}

// KOOPA
Koopa::Koopa(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : WalkingEnemy(imageID, startX, startY, dir, 0, 1, world) {

}

void Koopa::attemptToDamage() {
	int d = this->getDirection();
	getWorld()->increaseScore(100);
	kill();
	getWorld()->introduceNewObject(new Shell(IID_SHELL, getX(), getY() + 2, d, 1, 1, getWorld()));
}

void Koopa::bonk() {
	if (getWorld()->overlapWithPeach(getX() , getY(), ' ')) {
			// if peach has star power (invincibility)
			if (getWorld()->checkPower('s')) {
				getWorld()->playSound(SOUND_PLAYER_KICK);
				getWorld()->increaseScore(100);
				kill();
				getWorld()->introduceNewObject(new Shell(IID_SHELL, getX(), getY(), this->getDirection(), 1, 1, getWorld()));
			}
		}
}

StationaryEnemy::StationaryEnemy(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : Enemy(imageID, startX, startY, dir, 0, 1, world) {

}

// PIRANHA
Piranha::Piranha(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* world) : StationaryEnemy(imageID, startX, startY, dir, 0, 1, world)  {
	m_firing_delay = 0;
}

void Piranha::doSomething() {
	if (isAlive()) {
		increaseAnimationNumber();
		if (getWorld()->overlapWithPeach(getX(), getY(), ' ')) {
			getWorld()->peachBonk();
			return;
		}
		// if peach y is within 1.5 * SPRITE_HEIGHT
		if (getWorld()->overlapPiranha(getX(), getY())) {
			// set piranha direction
			// if false, peach is to the left of piranha
			if (!getWorld()->setPiranhaDirection(getX(), getY())) {
				setDirection(180);
			}
			else if (getWorld()->setPiranhaDirection(getX(), getY())) {
				setDirection(0);
			}			
			if (m_firing_delay > 0) {
				m_firing_delay--;
				return;
			}
			else if (m_firing_delay == 0) {
				// choose to fire by computing distance
				if (getWorld()->distanceBetweenPiranhaPeach(getX(), getY())) {
					// add a new piranha fireball
					getWorld()->introduceNewObject(new PiranhaFireball(IID_PIRANHA_FIRE, getX(), getY(), getDirection(), 1, 1, getWorld()));
					getWorld()->playSound(SOUND_PIRANHA_FIRE);
					m_firing_delay = 40;
				}
			}
		}
		// peach is not within y
		else {
			return;
		}

	}
}

void Piranha::attemptToDamage() {
	getWorld()->increaseScore(100);
	kill();
}

void Piranha::bonk() {
	if (getWorld()->overlapWithPeach(getX(), getY(), ' ')) {
		// if peach has star power (invincibility)
		if (getWorld()->checkPower('s')) {
			getWorld()->playSound(SOUND_PLAYER_KICK);
			getWorld()->increaseScore(100);
			kill();
		
		}
	}
}


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
