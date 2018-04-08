#define SDL_MAIN_HANDLED

#include <iostream>

#include "LTimer.h"
#include <chrono>
#include <time.h>
#include "Tile.h"
#include "AStar.h"
#include "NodeLayout.h"

//Our worker thread function
int worker(void* data);
//Data access semaphore
SDL_sem* gDataLock = NULL;
//The "data buffer"
int gData = -1;
SDL_Rect r;

std::vector<SDL_Rect*> enemyVector;

std::vector<Tile*> tileVector;
SDL_Point velo;

int increment = -1;
int dir = 0;
int GAME_SCALE = 3;

std::vector<SDL_Point> nodePositions;
NodeLayout m_nodeLayout;
int *layout;


int starDist;

AStar* astar;
std::vector<Node*> m_seekPath;
bool m_targetChosen = false;

int distToTar;
int distToSeekNode;

SDL_Point m_vel;

float calculateMagnitude(SDL_Point vec1, SDL_Point vec2)
{
	return sqrt(((vec2.x - vec1.x) * (vec2.x - vec1.x)) + ((vec2.y - vec1.y) * (vec2.y - vec1.y)));
}

float calculateMagnitude(SDL_Point vec) {
	return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

void normalise(SDL_Point &v) {
	float magnitude = calculateMagnitude(v);

	if (magnitude > 0)
	{
		v.x = v.x / magnitude;
		v.y = v.y / magnitude;
	}
}

void seek(float deltaTime, SDL_Point v, float dist, bool seekingWorker) {
	/********************************************//**
												  *  ...  // seeks to a given position
												  ***********************************************/
	float targetSpeed;

	if (seekingWorker == false) {

		if (dist < 70) {
			targetSpeed = 0;
		}
		else if (dist > 200) {
			targetSpeed = 8;
		}
		else {
			targetSpeed = 8 * (dist / 200);
		}
	}
	//else {
	//	if (dist < 30) {
	//		// abducts player and returns to patrol
	//		m_workers.at(m_targetIndex)->setAbducted(true);
	//		m_indexesOfAbducted.push_back(m_targetIndex);
	//		m_behaviour = RETURN;
	//	}
	//}

	normalise(v);
	//v *= targetSpeed;

	//float timeToTarget = 4;

	//m_accel = v - (m_vel / timeToTarget);

	//if (calculateMagnitude(m_accel) > m_maxAccel) {
	//	normalise(m_accel);
	//}

	//	m_vel += 1 * deltaTime;
}


void seekPath(float deltaTime) {

	SDL_Point v1{ r.x,r.y };
	SDL_Point v2{ enemyVector.at(0)->x,  enemyVector.at(0)->y };

	SDL_Point vecToWorker{ v1.x - v2.x, v1.y - v2.y};

	distToTar = calculateMagnitude(vecToWorker);

	//// if there are nodes to seek to
	if (!m_seekPath.empty()) {
		// directional vector to next node
		SDL_Point vecToNextPoint = SDL_Point{ m_seekPath.at(0)->getPos().x - r.x, -m_seekPath.at(0)->getPos().y - r.y };

		// distance to next node
		distToSeekNode = calculateMagnitude(vecToNextPoint);

		// if the next node is closer than the worker
		if (distToSeekNode < distToTar) {
			seek(deltaTime, vecToNextPoint, distToSeekNode, false);

			if (distToTar < 70) {
				m_seekPath.erase(m_seekPath.begin());
			}
		}
		// if the worker is closer than the next node
		else {
			seek(deltaTime, vecToWorker, distToTar, true);
		}
	}
	// if there aren't nodes to seek to
	else {
		seek(deltaTime, vecToWorker, distToTar, true);
	}
}





void chooseTarget() 
{
	float closestDistTarget = 99999;


	float dist = calculateMagnitude(SDL_Point{ r.x, r.y }, SDL_Point{ enemyVector.at(0)->x, enemyVector.at(0)->y });

	if (dist < closestDistTarget) 
	{
			starDist = dist;
			m_targetChosen = true;
	}
	
}

void setupSeekPath() {
	/********************************************//**
												  *  ...  // setup path to seek to worker
												  ***********************************************/
	int indexClosestToWorker;
	int indexClosestToSweeper;

	float closestDistWorker = 99999;
	float closestDistSweeper = 99999;

	for (int i = 0; i < m_nodeLayout.getNoOfNodes() - 1; i++) {
		float distWorker = calculateMagnitude(m_nodeLayout.getNodes()[i]->getPos(), SDL_Point{ r.x,r.y });

		if (distWorker < closestDistWorker) {
			closestDistWorker = distWorker;
			indexClosestToWorker = i;
		}

		float distSweeper = calculateMagnitude(m_nodeLayout.getNodes()[i]->getPos(), SDL_Point{ enemyVector.at(0)->x, enemyVector.at(0)->y });

		if (distSweeper < closestDistSweeper) {
			closestDistSweeper = distSweeper;
			indexClosestToSweeper = i;
		}
	}

	if (!m_seekPath.empty()) {
		// if the node that is closest (the destination) to the player has changed
		if (m_nodeLayout.getNodes()[indexClosestToWorker] != m_seekPath.at(m_seekPath.size() - 1)) {
			m_seekPath.clear();
			astar->calculatePath(m_nodeLayout.getNodes()[indexClosestToWorker], m_nodeLayout.getNodes()[indexClosestToSweeper], m_seekPath);
		}
	}
	else {
		// create initial path
		astar->calculatePath(m_nodeLayout.getNodes()[indexClosestToWorker], m_nodeLayout.getNodes()[indexClosestToSweeper], m_seekPath);
	}
}



int main()
{
	SDL_Window* gameWindow = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_SHOWN);
	//SDL_Renderer* gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_Event *e = new SDL_Event();

	//init semaphore
	gDataLock = SDL_CreateSemaphore(4);

	unsigned int lastTime = 0;
	float deltaTime = 0;
	unsigned int currentTime = 0;
	srand(time(NULL));

	bool debug = false;

	srand(time(NULL));

	const int SCREEN_FPS = 60;

	const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;


	//The frames per second timer
	LTimer fpsTimer;

	//The frames per second cap timer
	LTimer capTimer;

	//Start counting frames per second
	int countedFrames = 0;

	fpsTimer.start();


	// Setup renderer
	// sets renderer for window
	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);

	r.x = 0;
	r.y = 0;
	r.w = 16;
	r.h = 16;



	nodePositions.push_back(SDL_Point{ 4*8, 8 }); //top

	nodePositions.push_back(SDL_Point{ 4*8, 29*8 }); //bottom wall 1


	nodePositions.push_back(SDL_Point{ 14 * 8, 8 }); //bottom

	nodePositions.push_back(SDL_Point{ 14 * 8, 29 * 8 }); //top wall 2



	nodePositions.push_back(SDL_Point{ 24 * 8, 8 }); //top

	nodePositions.push_back(SDL_Point{ 24 * 8, 0 * 8 }); //bottom wall 3

	m_nodeLayout = nodePositions;
	astar = new AStar(m_nodeLayout);

	enemyVector.push_back(new SDL_Rect({ 16, 86, 16, 16 }));


	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			bool wall = false;


			if (i == 5 && j >= 0 && j < 27)
			{
				wall = true;
			}


			if (i == 15 && j > 5 && j < 27)
			{
				wall = true;
			}

			if (i == 25 && j > 5 && j < 32)
			{
				wall = true;
			}
			
			tileVector.push_back(new Tile(i * 16, j * 16, 16, 16, wall));
		}
	}

	/////$$$$$$$$$$$$$$$$
	//Run the threads
	srand(SDL_GetTicks());
	SDL_Thread* threadA = SDL_CreateThread(worker, "Thread A", (void*)"Thread A");
	SDL_Delay(16 + rand() % 32);
	SDL_Thread* threadB = SDL_CreateThread(worker, "Thread B", (void*)"Thread B");
	///$$$$$$$$$$$



	while (7 == 7)
	{
		dir = 0;
		velo = SDL_Point{ 0,0 };

		while (SDL_PollEvent(e))
		{
			switch (e->type)
			{
				/* Look for a keypress */
			case SDL_KEYDOWN:
				/* Check the SDLKey values and move change the coords */
				switch (e->key.keysym.sym)
				{
				case SDLK_LEFT:
				
					dir = 4;
					break;
				case SDLK_RIGHT:
					
					dir = 6;
					break;
				case SDLK_UP:
				
					dir = 8;
					break;
				case SDLK_DOWN:
					
					dir = 2;
					break;
				default:
					break;
				}
			}
		}

		if (dir == 2)
		{
			velo.y = 8;
		}
		else if (dir == 8)
		{
			velo.y = -8;
		}
		else if (dir == 4)
		{
			velo.x = -8;
		}
		else if (dir == 6)
		{
			velo.x = 8;
		}


		r.x += velo.x;
		r.y += velo.y;


		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200); //default white

		SDL_RenderClear(renderer);

		for (int i = 0; i < tileVector.size(); i++)
		{
			tileVector.at(i)->render(renderer, i);
		}

		//Calculate and correct fps
		int avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 2000000)
		{
			avgFPS = 0;
		}

		////$$$$$$$$
		if (m_targetChosen == false)
		{
			chooseTarget();
		}
		else
		{
			setupSeekPath();
		
			SDL_Point bab{ enemyVector.at(0)->x, enemyVector.at(0)->y };

			seekPath(deltaTime);

			astar;

			int q = 5;
		}
		//$$$$$$$

		//Set text to be rendered
		if (avgFPS > 1)
		{
		}
		++countedFrames;

		//If frame finished early
		int frameTicks = capTimer.getTicks();

		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);

			currentTime = SDL_GetTicks();
			if (currentTime > lastTime)
			{
				deltaTime = ((float)(currentTime - lastTime)) / 1000;

				lastTime = currentTime;
			}

			

			SDL_SetRenderDrawColor(renderer, 255, 200, 0, 200); //yellow

			// Render rect
			SDL_RenderFillRect(renderer, &r);

			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 200); //yellow

		

			SDL_RenderFillRect(renderer, enemyVector.at(0));

			SDL_RenderPresent(renderer);

		//	cout << "test tick" << endl;
		}
	}


	//Wait for threads to finish
	SDL_WaitThread(threadA, NULL);
	SDL_WaitThread(threadB, NULL);

	SDL_RenderPresent(renderer);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gameWindow);

	IMG_Quit();
	SDL_Quit();

	return 0;
}



void close()
{
	//Free semaphore
	SDL_DestroySemaphore(gDataLock);
	gDataLock = NULL;
}

int worker(void* data)
{
//	srand(SDL_GetTicks());

	while (9 == 9)
	{
		SDL_SemWait(gDataLock);

		increment++;


		if (increment > 1023)
		{
			increment = 0;
		}

		gData = rand() % 256;

		int tempIndex = increment;

		if (tempIndex > 1023)
		{
			tempIndex = 0;
		}

		//unlock data
		SDL_SemPost(gDataLock);

		if (tileVector.at(tempIndex)->getSolid())
		{
			SDL_Rect temp = tileVector.at(tempIndex)->getRect();

			if (SDL_HasIntersection(&temp, &r))
			{
				cout << "coll" << endl;
				if (dir == 2)
				{
					r.y -= 8;
				}
				if (dir == 8)
				{
					r.y += 8;
				}
				if (dir == 4)
				{
					r.x += 8;
				}
				if (dir == 6)
				{
					r.x -= 8;
				}
			}
		}
	}

	return 0;
}