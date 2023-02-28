
# PegSol

This is a AI solver for Peg Solitaire. 

The Peg Solitaire game
As explained in the [wikipedia entry](https://en.wikipedia.org/wiki/Peg_solitaire#Play). The player can move a peg jumping on top of another adjacent
peg, if there is a free adjacent cell to land. There are 4 valid jumps: Left, Right, Up and Down.

The objective is to clean the board until there is only 1 peg left. Some variants require that the last
remaining peg sits on the middle of the board. In this game we ignore that variant and win the game
if only 1 peg is left, no matter its final position. An AI agent or human player can chose the sequence
of jumps to win the game.



## Authors

- Base Code: Maurits van der Schee
- AI Solver: Md Reaj Uddin Rabbi


## Run

Clone the project

```bash
  git clone https://github.com/rrabbi8/pegsol.git
```

Go to the project directory

```bash
  cd pegsol
```

Run Makefile

```bash
  make
```

Run Pegsol AI solver

```bash
  
  
  ./pegsol <level> AI <budget> play_solution
    
```
  
  
  e.g: ./pegsol 5 AI 1200000 play_solution (play_solution is optional)
  
  Will run the 6th layout expanding maximum 1.2M nodes and will play the soltution found.
  


Play with Keyboard

```bash

  ./pegsol <level>

```
where, <level> is an integer 0-8


## Output

![Sample Output](https://github.com/rrabbi8/pegsol/issues/1#issue-1602812207)
