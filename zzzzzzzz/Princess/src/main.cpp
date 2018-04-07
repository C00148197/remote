#define SDL_MAIN_HANDLED

#include <iostream>

#include "LTimer.h"
#include <chrono>
#include <time.h>
#include "Tile.h"
#include "AStar.h"

//Our worker thread function
int worker(void* data);
//Data access semaphore
SDL_sem* gDataLock = NULL;
//The "data buffer"
int gData = -1;
SDL_Rect r;
std::vector<Tile*> tileVector;


int increment = -1;

int GAME_SCALE = 3;

int main()
{
	SDL_Window* gameWindow = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_SHOWN);
	//SDL_Renderer* gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_Event *e = new SDL_Event();



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
	//renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_PRESENTVSYNC);


	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
	//SDL_RenderClear(renderer);


	// Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.

	r.x = 0;
	r.y = 0;
	r.w = 16;
	r.h = 16;



	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			bool wall = false;
			if (i == 15 && j > 5 && j < 27)
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
					r.x -= 8;
					break;
				case SDLK_RIGHT:
					r.x += 8;
					break;
				case SDLK_UP:
					r.y -= 8;
					break;
				case SDLK_DOWN:
					r.y += 8;
					break;
				default:
					break;
				}
			}
		}



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
	//    printf("%s starting...\n", data);

	//Pre thread random seeding
	srand(SDL_GetTicks());

	//Work 5 times
	while (9 == 9)
	{
		//Wait randomly
		//SDL_Delay(16 + rand() % 32);

		//Lock
		SDL_SemWait(gDataLock);

		increment++;


		if (increment > 1023)
		{
			increment = 0;
		}

		//for (int i = 0; i < tileVector.size(); i++)
		//{
		//    if (tileVector.at(i)->getSolid())
		//    {
		//        SDL_Rect temp = tileVector.at(i)->getRect();

		//        if (SDL_HasIntersection(&temp, &r))
		//        {
		//            cout << "coll" << endl;
		//        }
		//    }
		//}




		//Print pre work data
		//    printf("%s gets %d\n", data, gData);

		//"Work"
		gData = rand() % 256;

		//Print post work data
		//printf("%s sets %d\n\n", data, gData);

		//Unlock
		SDL_SemPost(gDataLock);

		//    cout << "inc" << increment << endl;
		if (tileVector.at(increment)->getSolid())
		{
			//    cout << "solid check" << endl;
			SDL_Rect temp = tileVector.at(increment)->getRect();

			if (SDL_HasIntersection(&temp, &r))
			{
				cout << "coll" << endl;
			}
		}


		//Wait randomly
		//    SDL_Delay(16 + rand() % 640);
	}

	//printf("%s finished!\n\n", data);

	return 0;
}