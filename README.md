# AI Car
A simple car game using a neural network to control the cars. 
Every time all the cars die, the best one is turned into the predecessor for which the new population is made.
The best cars are given a fitness-score based on distance travelled, if they finished and their average time. The cars are then sorted based on their fitness score. Then the top 3 are used as a basis for a new population:
* 50% of the new cars are based on the best car
* 25% of the new cars are based on the second best car
* 25% of the new cars are based on the third best car
* One car is a copy of the best one from last generation as to make sure the mutations in the neural network doesn't make the cars worse.

See videos for a simple showcase of the game and how it performs.

## Old version:
This version does not use a fitness score to determine the best cars, and only has one track.

[![Video](https://img.youtube.com/vi/fFn9S7x9gFM/0.jpg)](https://www.youtube.com/watch?v=fFn9S7x9gFM)

## New version:
This version is with the fitness score system, and the tracks are picked at random every time the cars restarts.
To make the tracks, a separate program is used to just draw the lines. The output .txt-file is then loaded into this program and is added to the list of random tracks that are picked.

[![Video](https://img.youtube.com/vi/E0kOrQoAwm0/0.jpg)](https://youtu.be/E0kOrQoAwm0)
