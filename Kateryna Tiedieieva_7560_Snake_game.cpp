#include"raylib.h" //drawing
#include"raymath.h" //math with vector2d
#include<deque> //struct which the snake is made out of

using namespace std;

int cellSize = 30;
int cellCount = 30;
bool gameOver = false;
int Score = 0;


class Snake {
private:
	Vector2 up = { 0, -1 }; //vector change to go up
	Vector2 down = { 0, 1 }; //go down
	Vector2 left = { -1, 0 }; //go left
	Vector2 right = { 1, 0 }; // go right

	deque<Vector2> body = { Vector2{6,7}, Vector2{6,8}, Vector2{6,9} }; //snake body
	Vector2 direction = { 1, 0 }; //current direction of snake

public:
	Vector2 getSnakeHead() {
		return body[0];
	} //returns vector of the snake head

	Vector2 getDirection() {
		return direction;
	}

	void draw() {

		//loops through the body and draws a rectangle at each location
		for (unsigned int i = 0; i < body.size(); i++) {

			float x = body[i].x;
			float y = body[i].y;

			DrawRectangle((int)x * cellSize, (int)y * cellSize, cellSize, cellSize, GREEN);
		}
	}
	void addSegment() {
		body.push_back(Vector2{ 100, 100 });
	} //adds segment to the snake body

	void hitSelf() {
		for (int i = 1; i <= body.size() - 1; i++) {
			if (Vector2Equals(body[0], body[i])) { //if head is in the same place as other body part
				gameOver = true;
			}
		}
	} //checks if the snake hit itself and if yes stops the game

	void move() {
		if (IsKeyPressed(KEY_W) && !Vector2Equals(direction, down)) {
			direction = up;
		}
		if (IsKeyPressed(KEY_A) && !Vector2Equals(direction, right)) {
			direction = left;
		}
		if (IsKeyPressed(KEY_D) && !Vector2Equals(direction, left)) {
			direction = right;
		}
		if (IsKeyPressed(KEY_S) && !Vector2Equals(direction, up)) {
			direction = down;
		}

		//removes the tail part of the snake
		body.pop_back();
		//adds a new part to the front of the snake. This removal and addition visually mimics the snakes movement acroos the screen.
		body.push_front(Vector2Add(body[0], direction));
	}

	//check if the snake has hit the edge of the screen and if yes it moves it to the opposite side.
	void edgeCollision() {

		if (body[0].x >= cellCount) { //hit right side
			body[0].x = 0;
		}
		if (body[0].x <= -1) { //hit left side
			body[0].x = (float)cellCount - 1;
		}
		if (body[0].y >= cellCount) { //hit bottom
			body[0].y = 0;
		}
		if (body[0].y <= -1) { //hit top
			body[0].y = (float)cellCount - 1;
		}

	}
};

class Food {
private:
	Vector2 coords;

public:
	Food() {
		setRandCoords(); //add food to random spot
	}

	Vector2 getCoords() {
		return coords;
	}

	void setRandCoords() {
		int num1 = rand() % cellCount;
		int num2 = rand() % cellCount;
		coords = { (float)num1, (float)num2 };
	} //change coordinates of food 

	void draw() {
		DrawRectangle((int)coords.x * cellSize, (int)coords.y * cellSize, cellSize, cellSize, RED);
	}
};


int main() {

	SetTargetFPS(10); //speed of game
	InitWindow(cellSize * cellCount, cellSize * cellCount, "My Snake Game");

	Snake snake = Snake(); //create snake
	Food food = Food(); //create food

	//The main game loop that repeats at a rate of targetFPS and runs until the window is exited by the esc key or clixking the close window buttton in the top right
	while (!WindowShouldClose()) {
		BeginDrawing(); //start drawing
		ClearBackground(DARKGREEN);
		DrawText(TextFormat("Score: %i", Score), 0, 0, 50, WHITE);

		snake.draw();
		food.draw();
		snake.move();

		// check if the snake head has hit the food and the game is not over
		if (Vector2Equals(snake.getSnakeHead(), food.getCoords()) && gameOver == false) {
			food.setRandCoords();
			snake.addSegment();
			Score++;
		}

		//checks if the game is over
		if (gameOver) {
			DrawText("Game Over", (cellSize * cellCount) / 5, (cellSize * cellCount) / 3, 100, DARKPURPLE);
		}

		snake.hitSelf(); //did snake hit itself
		snake.edgeCollision(); //did snake hit edge

		EndDrawing();
	}

	CloseWindow();
	return 0;
}