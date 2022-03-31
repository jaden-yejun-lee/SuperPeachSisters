# SuperPeachSisters
In Super Peach Sisters, Peach starts out a new game with three lives and continues to play
until all of her lives have been exhausted. There are multiple levels in Super Peach
Sisters, beginning with level 1 (NOT zero), and during each level except for the last
Peach must reach a flag to continue to the next level. On the last level, Peach must reach
(and thus save) Mario in order to win the entire game (there is no flag on the last level).

The Super Peach Sisters screen is exactly 256 pixels wide by 256 pixels high. The
bottom-leftmost pixel has coordinates x=0,y=0, while the upper-rightmost pixel has
coordinate x=255,y=255, where x increases to the right and y increases upward toward
the top of the screen. The GameConstants.h file we provide defines constants that
represent the game’s width and height (VIEW_WIDTH and VIEW_HEIGHT), which
you must use in your code instead of hard-coding the integers. Every object in the game
(e.g., Peach, koopas, stars, shells, etc.) will have an x coordinate in the range 0 to
VIEW_WIDTH−1 inclusive, and a y coordinate in the range 0 to VIEW_HEIGHT−1
inclusive. These constants also may be found in the same header file.

Each level has its layout defined in a data file, such as level01.txt or level02.txt. You can
find example level files in the Assets folder that we provide, and you may modify these
data files or add additional ones to create new and exciting levels. In every level except
for the last level, there must be at least one flag that Peach must reach in order to advance
to the next level. The last level must not contain a flag, but must contain Mario, who
Peach must reach to win the game. For more details on the format of the level data files,
please see the Level Data File section.

Peach may move left or right, and may also jump. Because of the strange physics of the
Mushroom Queendom, Peach can actually move left and right while jumping in the air as
well, which makes it easier for her to jump from platform to platform.

As Peach works through each level, she must avoid coming into contact with any of her
enemies. Coming into contact with an enemy (unless she currently has a superpower due
to picking up a goodie) will cause her to die, either resetting the level, or ending the game
if she has no lives left. Similarly, Peach must avoid all fireballs shot by Piranhas or she
will die. If Peach dies three times, the game is over.

At the beginning of each level and when Peach restarts the current level because she has
died, the level will be reset to an initial state. That is, all actors will be placed in their
initial positions and start with their initial states. All blocks will be reset, so they again
release any goodies when bonked by Peach, etc. Peach also starts out with no special
powers (i.e., no invincibility, jump power or shooting power).

Some of the blocks in each level may hold special goodies, and if Peach bonks them with
her head by jumping into them, she can cause them to release their goodie, which Peach
can pick up. These special blocks look just like all of the other blocks, so you will need to
learn where they are in each level by trying to bonk blocks here and there.

There are three different types of goodies: star goodies, mushroom goodies and flower
goodies. Star goodies give Peach invincibility for a limited amount of time, so she can’t
be injured by coming into contact with enemies like koopas, goombas, piranhas, or
piranha fireballs. Mushroom goodies give Peach the ability to jump extra high, allowing
her to reach platforms that would otherwise be out of reach. Finally, flower goodies
enable Peach to shoot fireballs to destroy her enemies.

The effect of a mushroom or a flower goodie lasts until either Peach comes into contact
with an enemy (in which case she loses the goodie’s power, but doesn’t die), or she
completes the current level. Similarly, invincibility from the star goodie lasts about 10
seconds or until Peach completes the current level, whichever comes first.

Assuming Peach has Fire Power (granted to her by picking up a flower goodie), the
player may hit the spacebar to have peach shoot a fireball. A fireball will destroy the first
enemy it contacts, and then disappears from the game. A fireball will fly forward until
either it hits an enemy, or it hits a block or a pipe in a horizontal direction (simply falling
down onto a block or pipe will not cause the fireball to dissipate). Beyond damaging
enemies, fireballs will otherwise pass over all other objects in the game. For example,
fireballs will pass over flags, Mario, mushrooms, flowers, stars and shells without
damaging them. Fireballs fall to the ground as they move forward, so for example, if one
flies past the edge of the current platform of blocks it will start falling down.

Each level may have up to three different types of enemies: koopas, goombas and
piranhas. Koopas and goombas simply move left and right on their platforms, stopping
before they fall off the edge of their platform or if they run into a block or pipe, and then
turning to move in the opposite direction. Piranhas don’t move, but will turn to face
Peach if she gets close enough. Piranhas will also shoot fireballs at Peach if she gets too
close. A fireball fired by a piranha will hurt Peach just as if it were an enemy coming into
contact with Peach. A fireball fired by a piranha will fly forward until it hits either a
block or a pipe, but beyond damaging Peach, it will otherwise pass over all other objects
in the game (goombas, koopas, other piranhas, flags, Mario, mushrooms, flowers, stars,
and shells) without damaging them. A fireballs fired by piranha falls to the ground as it
moves forward, so for example, if one flies past the edge of a platform of blocks, it will
start falling down to the level below.

If Peach destroys an enemy (either by contacting them while invincible, or by shooting
them with a fireball), the enemy will disappear from the level and Peach will get points.
When Peach destroys a koopa, it will produce a shell which will travel like a projectile
until it hits an block, pipe or an enemy, at which point it will dissipate (A shell behaves
identically to a fireball fired by Peach, but looks different). Shells float right past Peach,
but will destroy all enemies (koopas, goombas, or piranhas) they come into contact with.

As Peach plays, if she comes into contact with an enemy (e.g., goomba, koopa or
piranha) and she does not have any type of power (e.g., Star Power) she will lose a life,
and the level will reset (or the game will be over if she has no more lives). If Peach has
Star Power (invincibility) then coming into contact with an enemy will not hurt Peach,
and will immediately destroy the enemy. If Peach has Jump Power and/or Shoot Power
(but not Star Power) and comes into contact with an enemy, then she will immediately
lose those power(s) but she will not lose a life, and her enemy will not be hurt either. In
this case, Peach will have a short amount of temporary invincibility so she does not
immediately die on the next tick as the enemy continues to maintain contact with her (as
it moves). If, after losing a life, Peach has one or more remaining lives left, the level and
all of the actors (Peach, goombas, etc.) are reset to their initial state and Peach must again
play the level from scratch. If Peach dies and has no lives left, then the game is over.

If Peach reaches the flag on the level, then the level ends and Peach advances to the next
level. If Peach reaches Mario on the final level, then Peach wins the entire game and the
game is over.
