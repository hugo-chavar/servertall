#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "SDL.h"
#include "Stage.h"


class Engine {
	private:
		bool running;

		unsigned int desiredFPS;

	public:
		Engine();
	
		//TODO: Create getter/setter
		view::Stage worldView;

	
		bool isRunning();

		unsigned int getDesiredFPS();

		int execute();

		void initialize();

		//void onEvent(SDL_Event* sdlEvent);

		void update();

		//void render();

		//void cleanUp();

		~Engine();
};

#endif //_ENGINE_H_
