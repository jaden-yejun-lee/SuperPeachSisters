#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
#include <cstdlib>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    std::list<Actor*> myActors;
    m_levelOver = false;
    m_gameOver = false;
    m_peach = nullptr;
}

int StudentWorld::init()
{
   
    Level lev(assetPath());

    ostringstream leveloss;
    leveloss << "level";
    leveloss.fill('0');
    leveloss << setw(2) << getLevel();
    leveloss << ".txt";

    string level_file = leveloss.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success)
    {
        Level::GridEntry ge;
        for (int x = 0; x < 32; x++) {
            for (int y = 0; y < 32; y++) {
                ge = lev.getContentsOf(x, y);
                int randDir = (randInt(0, 1)) * 180;
                switch (ge)
                {
                case Level::empty:
                    break;

                case Level::peach:
                    m_peach = new Peach(IID_PEACH, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, 0, 1, this);
                    break;

                case Level::block:
                    myActors.push_back(new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, 2, 1, 0, this));
                    break;

                case Level::flower_goodie_block:
                    myActors.push_back(new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, 2, 1, 1, this));
                    break;

                case Level::pipe:
                    myActors.push_back(new Pipe(IID_PIPE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, 2, 1, this));
                    break;

                case Level::goomba:
                    myActors.push_back(new Goomba(IID_GOOMBA, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, randDir, 0, 1, this));
                    break;

                case Level::flag:
                    myActors.push_back(new Flag(IID_FLAG, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, 1, 1, this));
                    break;

                case Level::mario:
                    myActors.push_back(new Mario(IID_MARIO, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, 1, 1, this));
                    break;

                case Level::mushroom_goodie_block:
                    myActors.push_back(new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, 2, 1, 2, this));
                    break;
                case Level::star_goodie_block:
                    myActors.push_back(new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, 2, 1, 3, this));
                    break;
                case Level::koopa:
                    myActors.push_back(new Koopa(IID_KOOPA, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, randDir, 0, 1, this));
                    break;
                case Level::piranha:
                    myActors.push_back(new Piranha(IID_PIRANHA, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, randDir, 0, 1, this));
                    break;
                }
            }
        }

    }

    return GWSTATUS_CONTINUE_GAME;
}


StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
    //return GWSTATUS_PLAYER_DIED;

    // iterate through list and have each actor doSomething()
    list<Actor*> ::iterator it;
    it = myActors.begin();

    m_peach->doSomething();
    while (it != myActors.end()) {

        if ((*it)->isAlive()) {
            (*it)->doSomething();


            it++;
        }
        else {
            Actor* temp;
            temp = (*it);
            it = myActors.erase(it);
            delete temp;
            temp = nullptr;
        }
       
    }

    // if peach died during this tick
    if (!m_peach->isAlive()) {
        playSound(SOUND_PLAYER_DIE);
        decLives();
        return GWSTATUS_PLAYER_DIED;

    }

    // if peach has reached a flag          
    if (m_levelOver == true) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }

    // if peach reached a mario
    if (m_gameOver == true) {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }



    // Update status text on top of the screen
    ostringstream oss;
    oss << "Lives: " << getLives() << "  ";

    oss << "Level: ";
    oss.fill('0');
    oss << setw(2) << getLevel() << "  ";
    oss << "Points: ";
    oss << setw(6) << getScore() << " ";

    if (m_peach->checkInvincibility()) {
        oss << "StarPower! ";
    }

    if (checkPower('f')) {
        oss << "ShootPower! ";
    }

    if (checkPower('m')) {
        oss << "JumpPower! ";
    }

    string s = oss.str();
    setGameStatText(s);

    // continue playing the current level
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_peach;
    m_peach = nullptr;
    m_levelOver = false;
    list<Actor*> ::iterator it;
    it = myActors.begin();

    while (it != myActors.end()) {
        Actor* temp;
        temp = (*it);
        it = myActors.erase(it);
        delete temp;
    }
}

// if there is an object that would block one pixel below
bool StudentWorld::objectCanBlock() {
    
    double x = m_peach->getX();
    double y = m_peach->getY();

    list<Actor*>::iterator it;
    it = myActors.begin();
    while (it != myActors.end()) {
        // should encompass a range of x values and a range of y values

        if ((x >= (*it)->getX() && x <= (*it)->getX() + SPRITE_WIDTH - 1) || (x + SPRITE_WIDTH - 1 >= (*it)->getX() && x + SPRITE_WIDTH - 1 <= (*it)->getX() + SPRITE_WIDTH - 1)) {
            if (y <= (*it)->getY() + SPRITE_HEIGHT && y >= (*it)->getY()) {
                if ((*it)->blocksMovement()) {
                    return true;
                }
            }
        }

        it++;
    }

    return false;
}


void StudentWorld::introduceNewObject(Actor* a) {
  myActors.push_back(a);
}

void StudentWorld::changePower(bool b, char goodie) {

    switch (goodie) {
    case 'f':
        m_peach->changePower(b, 'f');
        break;
    case 'm':
        m_peach->changePower(b, 'm');
        break;
    case 's':
        m_peach->changePower(b, 's');
        break;
    }

   
}

bool StudentWorld::checkPower(char goodie) {

    switch (goodie) {
    case 'f':
        if (m_peach->checkPower('f') == true) {
            return true;
        }
        return false;
        break;
    case 'm':
        if (m_peach->checkPower('m') == true) {
            return true;
        }
        return false;
        break;
    case 's':
        if (m_peach->checkInvincibility() == true) {
            return true;
        }
        return false;
        break;
    }

}

void StudentWorld::changeInvincibility(int i) {
    m_peach->changeInvincibility(i);
}

void StudentWorld::changeHP(int i) {
    m_peach->changeHP(i);
}

void StudentWorld::peachBonk() {
    m_peach->bonk();
}


bool StudentWorld::overlap(double x, double y, bool shouldBonk) {

    list<Actor*>::iterator it;
    it = myActors.begin();
    while (it != myActors.end()) {
      

        if (x + SPRITE_WIDTH - 1 > (*it)->getX() && x < (*it)->getX() + SPRITE_WIDTH - 1 
            && (*it)->getY() < y + SPRITE_HEIGHT - 1 && y < (*it)->getY() + SPRITE_HEIGHT - 1) {

 
            if ((*it)->blocksMovement()) {

                if (shouldBonk == true) {
                    (*it)->bonk();
                }

                return true;
            }
                

            else {
                if (shouldBonk == true) {
                    (*it)->bonk();
                }
                return false;
            }


        }

        it++;
    }

    return false;


}

bool StudentWorld::overlapGoomba(double x, double y, bool shouldBonk, char dir) {
    list<Actor*>::iterator it;
    it = myActors.begin();
    while (it != myActors.end()) {

        switch (dir) {
        case 'r':

            if (x + SPRITE_WIDTH - 1 > (*it)->getX() &&  x  < (*it)->getX() 
                && (*it)->getY() < y + SPRITE_HEIGHT - 1 && y < (*it)->getY() + SPRITE_HEIGHT - 1 ) {


                if ((*it)->blocksMovement()) {

                    if (shouldBonk == true) {
                        (*it)->bonk();
                    }

                    return true;
                }

                else {
                    if (shouldBonk == true) {
                        (*it)->bonk();
                    }
                    return false;
                }

                return true;

            }
            break;
            
        case 'l':
            if (x > (*it)->getX() && x < (*it)->getX() + SPRITE_WIDTH - 1
                && (*it)->getY() < y + SPRITE_HEIGHT - 1 && y < (*it)->getY() + SPRITE_HEIGHT - 1) {


                if ((*it)->blocksMovement()) {

                    if (shouldBonk == true) {
                        (*it)->bonk();
                    }

                    return true;
                }

                else {
                    if (shouldBonk == true) {
                        (*it)->bonk();
                    }
                    return false;
                }

            }
            break;


        }


        it++;
    }

    return false;

}


bool StudentWorld::overlapWithPeach(double x, double y, char object) {
    if (x + SPRITE_WIDTH - 1 > m_peach->getX() && x < m_peach->getX() + SPRITE_WIDTH - 1 
        && m_peach->getY() < y + SPRITE_HEIGHT - 1 && y < m_peach->getY() + SPRITE_HEIGHT - 1) {

        switch (object) {
        
        // if peach overlaps with flag
        case 'f':
            m_levelOver = true;
            return true;
            break;

        case 'm':
            m_gameOver = true;
            return true;
            break;

        case 'p':
            m_peach->attemptToDamage();
            return true;
            break;

        default: 
            return true;

        }


       }

    return false;
}

bool StudentWorld::overlapWithPowerup(double x, double y, char goodie) {

    list<Actor*>::iterator it;
    it = myActors.begin();
    while (it != myActors.end()) {

        if ((*it)->isAlive()) {
            switch (goodie) {
            case 'f':

                if (x + SPRITE_WIDTH - 1 > (*it)->getX() && x < (*it)->getX() + SPRITE_WIDTH - 1 && (*it)->getY() < y + SPRITE_HEIGHT - 1 && y < (*it)->getY() + SPRITE_HEIGHT - 1) {

                    if ((*it)->isDamageable()) {
                        (*it)->attemptToDamage();
                        return true;
                    }
                }
                break;

            case 'm':
            case 's':

                if (x + SPRITE_WIDTH - 1 > (*it)->getX() && x < (*it)->getX() + SPRITE_WIDTH - 1 && (*it)->getY() < y + SPRITE_HEIGHT - 1 && y < (*it)->getY() + SPRITE_HEIGHT - 1) {

                    if ((*it)->isDamageable()) {
                        (*it)->attemptToDamage();
                        return true;
                    }
                }
                break;

            case 't':

                if (x == (*it)->getX() && y == (*it)->getY()) {

                }

                else if (x + SPRITE_WIDTH - 1 >= (*it)->getX() && x <= (*it)->getX() + SPRITE_WIDTH - 1 && (*it)->getY() <= y + SPRITE_HEIGHT - 1 && y <= (*it)->getY() + SPRITE_HEIGHT - 1) {

                    if ((*it)->isDamageable()) {
                        (*it)->attemptToDamage();
                        return true;
                    }

                }
                break;

            }
        }

        it++;
    }

    return false;

}


bool StudentWorld::overlapPlatform(double x, double y, char dir) {
    list<Actor*>::iterator it;
    it = myActors.begin();
    while (it != myActors.end()) {

        switch (dir) {
        case 'r':

            // if it does overlap return true
            if (x + SPRITE_WIDTH - 1 >= (*it)->getX() && x + SPRITE_WIDTH - 1 <= (*it)->getX() + SPRITE_WIDTH - 1
                && y >= (*it)->getY() && y <= (*it)->getY() + SPRITE_HEIGHT - 1) {
                return true;
            }


            break;

        case 'l':

            // if it does overlap return true
            if (x >= (*it)->getX() && x <= (*it)->getX() + SPRITE_WIDTH - 1 && y >= (*it)->getY() && y <= (*it)->getY() + SPRITE_HEIGHT - 1) {
                return true;
            }

            break;
        }

        it++;

    }
    return false;
}

bool StudentWorld::fall(double sx, double ex, double sy, double ey, bool shouldBonk)
{

    list<Actor*>::iterator it;
    it = myActors.begin();
    while (it != myActors.end()) {

      
        if (sx <= (*it)->getX() + SPRITE_WIDTH - 1 && ex >= (*it)->getX() && sy <= (*it)->getY() + SPRITE_HEIGHT - 1 && ey >= (*it)->getY())
        {
            if (shouldBonk == true) {
               (*it)->bonk();
            }

            if ((*it)->blocksMovement() == true) {
                return true;
            }
           
        }

        it++;
    }

    return false;
}

bool StudentWorld::overlapPiranha(double x, double y) {

    if (m_peach->getY() <= y + (SPRITE_HEIGHT * 1.5) && m_peach->getY() >= y - (SPRITE_HEIGHT * 1.5)) {
        return true;
    }
    return false;
}

bool StudentWorld::setPiranhaDirection(double x, double y) {
    if (m_peach->getX() < x) {
        return false;
    }
    else {
        return true;
    }
  
}


bool StudentWorld::distanceBetweenPiranhaPeach(double x, double y) {
// if peach's x coordinate is less than 8 * spritewidth pixels away from piranhas x coordinate
    if (x - 8 * SPRITE_WIDTH < m_peach->getX() && m_peach->getX() < x + 8 * SPRITE_WIDTH) {
        return true;
    }

    return false;

}