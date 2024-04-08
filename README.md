# Shell Game Engine + Dungeon Game
This is a project undertaken for a level 6 University module called "Software Architecture for Games" in 2023. As an assessment submission, it has been marked 82% (first class in UK University marking).

## Software Architecture
The main object of the module and this project was developing a game engine to support the creation of a simple game. We were provided with the bare essentials in the shape of a Shell Engine, made by the module tutor, Chris Rook - see branch `base_shell` for what I started with. Over the course of the module, I then expanded the engine with many features, focusing on solid, decoupled, cohesive architecture. You can see the final version of the engine on branch `shell`. This includes, but does not limit to:
* Game object (w/ collidable, moving)  inheritance tree
* Object factory and object manager
* SFX manager
* Service system with a Service manager
* Event system

A report describing the architecture including class diagrams, as produced for the assignment, can be accessed here: [KF6017-Report-19002175.pdf](https://github.com/mkocvara/KF6017/files/14908390/KF6017-Report-19002175.pdf)


## Dungeon
Using this engine, I built a small game involving a fast-paced crawl through a series of small dungeon levels, facing increasing numbers of enemies, fighting them and picking up power-ups (boons) after each level. The game is very light on content, but has been built in a way that would easily allow adding more weapons, more enemies, more boons.

*A video demo of the game:*

https://github.com/mkocvara/KF6017/assets/28301057/7630ca3c-a403-4b27-8816-4ad4d1dcb78a

## Level Creation
The biggest feature the game uses is dynamically loading and constructing levels from text files, allowing easy, code-less level design. The file simply contains two grids of characters, each character representing an element of the level. A level file's content begins with a line as such:
`MAP`

Before it, comments can be written preceded by a `#`.

A grid of symbols follows. Must be rectangular. Does not have to be square. Example:
```
.####.
##__##
#__#_#
#_####
#__#_#
#_#__#
#____#
######
```

Wherein:
- `.` is nothing (filler),
- `#` is a wall,
- `_` is a floor

For best results:
* walls should be fully inclosed. 
* if a wall is adjacent to a `.` it should have 2 adjacent walls (makes them connected)
* no `.` should be within the walled area
* walls surrounded by floors are fine

![image](https://github.com/mkocvara/KF6017/assets/28301057/59b7da6e-25f1-40d4-b46e-c2608cf7f101)

This must be followed by the following line:
`ENTITIES`

After that is a second grid, which should be matching in size (though shorter lines should be fine). This grid determines spawn points for Orcs and the Knight.

`K` is knight
`O` is an orc

Other symbols are ignored, but are used to place the above symbols. They must not be placed where walls are and placing them outside of the walls may cause the level to be impossible to complete. The following examples are equivalent

Example 1:
```
ENTITIES
...#####...
...#_O_#...
...#___#...
####___####
#_________#
#O___K___O#
#_________#
####___####
...#___#...
...#_O_#...
...#####...
```

Example 2:
```
ENTITIES
...........
.....O.....
...........
...........
...........
.O...K...O.
...........
...........
...........
.....O.....
...........
```

The level:

![image](https://github.com/mkocvara/KF6017/assets/28301057/61009e3f-4615-4c89-bce2-c9896ff7f5b8)
