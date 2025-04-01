# Car-Game-for-DSA

DATA STRUCTURES AND ALGORITHMS
CPE 264 - H3 

CAR GAME FINAL PROJECT
INCREMENT 1 DOCUMENTATION


Current header files utilized:







These are the game constants and global variables:











← [ These global variables have values that dynamically change throughout the game. ]











This is our struct ObstacleNode definition for our linked list implementation in inserting, updating, and deleting obstacles. 










These are some of our supplementary functions that helped set the visual graphics of our game:










void setColor(): function for setting colors in-game.
void gotoxy(): repositions the cursor position or the insertion point for specific positioning of outputs like the car, obstacles, lanes, and UI.
void hideCursor(): hides the blinking cursor bar.

These functions are responsible for displaying the main tracks and simulating actual road lanes for our car game.





The getLaneX() function below is the function that accepts the current lane an object (car or obstacle) is in and returns an int value for setting the x location of an obstacle or the car on that lane.






Below are the functions responsible for drawing, clearing, and moving the car forward and backward. 





























void drawCar(): is simply the function that sets the visual appearance for our car.
void clearCar(): clears the previous position of the car when the car is moved from one position to another.
void moveCarForward():  moves the car forward by updating the value of carY, decrementing it every time the function is called.
void moveCarBackward(): moves the car backward by incrementing the value of carY while it is still above the original position or while carY < HEIGHT.

Below are the functions responsible for all obstacle-related functionalities:


void addObstacle(): is the function responsible for inserting or creating nodes for new obstacles in our ObstacleNode linked list. These nodes are being inputted with random data in-game, depending on the randomness of the inputted data that the program will assign to them, specifically in the int lane and the bool boss variable parameters. However, this randomness is not completely random and is still controlled by other values like game levels, spawn rate, and obstacle limits.

void clearObstacles(): simply clears the previous position of an obstacle while it is moving down or moving past the car.














void drawObstacles(): is a function that sets the visual appearance of the obstacles as it traverses the linked list. Depending on the type of obstacle, the design will differ.







void allLessThanSix(): is a function that accepts an array of obstacle counts per lane, with a size of LANE_COUNT, and returns false if and only if any of the lanes have an obstacle count of greater than or equal to OBSTACLE_LIMIT, which is 6.













void explosionEffect(): is a function that is called when an overlap (collision) between the car position and the obstacle position is detected, showing a simple visual animation.

void updateObstacle(): This function is responsible for updating the positions of the obstacles. It checks for collision (overlap) with the car, removes the obstacles that go off-screen, and adjusts the game’s difficulty based on the player’s score.
Void spawnObstacle(): This function is responsible for randomly spawning the obstacles. It calls allLessThanSix() function to ensure that there are less than 6 obstacles at once in a specific lane.




Void drawUI(): This handles the UI design that the player can see displaying information such as scores, level, as well as the guide for how to move the car.

void levelUpEffect(): This function simply displays the current achieved level. This function is called at every interval of 20 points.









void saveHighScore(): This function creates a file called “highscore.txt” that is saved in the same directory as the program. It saves the high score of the player. If the file does not exist initially, it will create the file and then save the high score otherwise, it updates the high score once the score of the player is greater than the high score stored in the file.
void loadHighScore(): This function is called to load in the high score from the file in order for the drawUI() function to display the highscore correctly.
void activatePowerUp(): This function randomly activates one of the two power-ups. Shield is for protecting the player from one collision and slow motion that temporarily slows down obstacle movement for a set duration (Bound for changes).
void gameOverScreen(): This function is similar to drawUI but is displayed once the game over criterion is met (lives are at 0). This displays the Score of the player as well as the high score recorded.
void gameOverScreen(): This function is similar to drawUI but is displayed once the game over criterion is met (lives are at 0). This displays the Score of the player as well as the high score recorded.

void restartGame(): This function restarts the state of the game back to default, clears the obstacles, sets the score to 0, and resets the speed,  lives, and power-ups to their default values.

void gameLoop(): This function serves as the main game loop, continuously running until the game ends or until the game-over criterion is met. It handles user input, updates the game state, manages obstacles, and renders the UI.
int main(): This function initializes the console game. It also seeds the number generator, clears the screen to prepare for the UI, and starts the game loop.
