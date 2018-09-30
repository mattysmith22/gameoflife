# Game of life
This is a simulator of [Conway's game of life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) written in C and using ncurses for the interface.

# Example screen:

```
Iteration: 0
--------------------
--------------------
--------------------
--------------------
--------------------
----------##--------
---------##---------
---------#-#--------
------------#-------
--------------------
--------------------
--------------------
--------------------
--------------------
--------------------
--------------------
--------------------
--------------------
--------------------
--------------------
```

# How to use
Live cells are marked as a '#' whereas dead cells are marked as a '-'.

You can toggle a cell between these two states by clicking on it.

To run an iteration of the simulation press the 'n' key.

To quit press the 'q' key.

# To do
* Add a clear key to the simulation
* Make the grid dynamically resizable on startup
* Make the grid fit the terminal size
