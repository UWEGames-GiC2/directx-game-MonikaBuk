[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/4B2l0wK5)
# NAAA-No-Afterlife-At-All-
UWE Game Engine Programming Scarle 2021


# Save the bird. 

 Find the bird and defeat all enemies.
 
## Controls:
 T - Switching camera view, __
 W A S D - Movement,__
 Space - Jump__
 Mouse - Rotation,__
 Left - Mouse Button - Shooting,__
 Enter- Progress with dialogues or in the menu,__
 Escape - Leave the game at any point;__
 
## Screenshots 

<div style="display: flex; justify-content: center;"> 

  <img src="https://github.com/UWEGames-GiC2/directx-game-MonikaBuk/assets/115149820/c6037f37-e493-4a1d-9927-e2afbf89265e" width="400" alt="Image 1"> 

  <img src="https://github.com/UWEGames-GiC2/directx-game-MonikaBuk/assets/115149820/f6d094a6-e445-4069-b105-e0ca9a0b9b6f" width="400" alt="Image 2"> 

</div> 


## Demo 

[![YouTube Video](https://img.youtube.com/vi/nce04Y1PjnQ/0.jpg)](https://www.youtube.com/watch?v=nce04Y1PjnQ) 

 
## About the Project 
 Map And Enemies: Reads in a text file of IDs and creates a map based on numbers. In my case 2 was the enemy, 4 the hostage and 1 are the walls. 0 represents empty tiles which are walkable areas for pathfinding. This map can be edited and scaled. The number of enemies 
 can be increased by editing the map and adjusting the size of rows and columns when initialising. I also have a Breadth-first search algorithm. All Enemies will find their way to the player if the player is at a certain distance; if the player gets too close the enemy will move towards the player and shoot. __

 The weapon has a 2D animation. 3Danimation was attempted even when loading it will load the bones in and store them in an array it will not update the position right.
 ![image](https://github.com/UWEGames-GiC2/directx-game-MonikaBuk/assets/115149820/656f1108-73f8-489d-8e6f-529cf736533c)

 There is also a dialogue system with a bird which will pause the game time. If not all of the enemies are defeated it will have a different response and let the player defeat all of them before entering the win state.

 The player can loose by running out of time or from enemies damage.

## References
free3d.com. (n.d.). LEGO Han Solo Free 3D Model - .obj .mb .fbx .mtl - Free3D. [online] Available at: https://free3d.com/3d-model/lego-han-solo-37069.html [Accessed 1 Apr 2024].

‌
