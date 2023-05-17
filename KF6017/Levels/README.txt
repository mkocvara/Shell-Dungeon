A level file's content begins with a line as such:
MAP

A grid of symbols follows. Must be rectangular. Does not have to be square. Example:
.####.
##__##
#__#_#
#_####
#__#_#
#_#__#
#____#
######

Wherein:
. is nothing (filler)
# is a wall
_ is a floor

For best results:
- walls should be fully inclosed. 
- if a wall is adjacent to a . it should have 2 adjacent walls (makes them connected)
- no . should be within the walled area
- walls surrounded by floors are fine

This must be followed by the following line:
ENTITIES

After that is a second grid, which should be matching in size (though shorter lines should be fine). This grid determines spawn points for Orcs and the Knight.

K is knight
O is an orc

Other symbols are ignored, but are used to place the above symbols.

E.g.1:
.####.
##K_##
#__#_#
#_####
#O_#_#
#_#_O#
#_O__#
######

E.g.2:
......
..K...
......
......
.O....
....O.
..O...
......