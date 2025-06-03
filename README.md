# ColonySim
A colony sim which uses ACO and other genetic algorithms alongside player input to create an immersive gameplay loop.

![Game View](https://github.com/amJohnnyma/ColonySim/blob/main/planning/gameview.png)

[Design Specs]()

## Installation - Linux only 
**SFML**
```
sudo apt update
```
```
sudo apt install libsfml-dev
```

**Install**
```
git clone https://github.com/amJohnnyma/ColonySim.git
```
**Build**
```
make clean
```
```
make
```
```
make run
```
* May work on windows, with some makefile tweaking
## Controls
* R: Reload world and sim
* Q: Zoom out
* E: Zoom in
* W: Move up
* A: Move left
* S: Move down
* D: Move right
* O: Increase pheromone influence
* P: Decrease pheromone influence
* K: Increase heuristic influence
* L: Decrease heuristic influence
* 1: Start sim
* 2: Stop sim

## UI
* Stats:
    - Base: Amount of food at the base (yellow square)
    - hf: heuristic influence
    - pf: pheromone influence
* Start/Stop button (S)
* Increase/decrease pheromone influence (+-p)
* Increase/decrease heuristic influence (+-h)
* FPS counter top right



### Next features
* NN for ants to build a base
* Ants die if they're away from a food source for too long
* Ant factions
* Player controls ant/s movement or lets ACO
* Find a cool theme
* Make a game
