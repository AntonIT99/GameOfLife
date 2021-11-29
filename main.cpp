#include <iostream>
#include<vector>
#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include <thread>
#include "functions_game.h"

using namespace Eigen;
using namespace std;

SDL_Renderer *init_renderer(SDL_Window *window, int xRes, int yRes)
{
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, xRes, yRes);
    return renderer;
}

void wait()
{
    bool resume = false;
    SDL_Event event;

    while (!resume)
    {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
        {
            resume = true;
        }
        if (event.type == SDL_KEYDOWN)
        {
            resume = true;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            resume = true;
        }
    }
}

void calculate_and_draw(MatrixXi &field2d, MatrixXi &field2d_tmp, vector<CELL> &modifiedCells, SDL_Renderer *renderer, int wRes, int hRes, bool &quit, bool &pause)
{
    while (!quit)
    {
        make_step(field2d, field2d_tmp, modifiedCells);
        refresh_field(renderer, wRes, hRes, modifiedCells, field2d.rows(), field2d.cols());
        while(pause)
        {
            this_thread::yield();
        }
    }
}

int main(int argc, char** argv)
{
    srand(time(NULL));

	unsigned int nZeile,nSpalte;
	//unsigned int nstep;
	double frac100, frac;
	int wRes, hRes;
	int cellSize;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
		cerr << "There was an error initializing SDL2: " << SDL_GetError() << endl;
		terminate();
	}

	do {
        cout << "Size of a cell (side of the square in pixels, must be between 1 and 100): " << endl;
        cin >> cellSize;
	} while (cellSize < 1 || cellSize > 100);

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);
    wRes = dm.w;
    hRes = dm.h;

    nZeile = hRes/cellSize;
    nSpalte = wRes/cellSize;

    /*if(nZeile<nSpalte)
    {
        nZeile++;
    } else if (nZeile>nSpalte)
    {
        nSpalte++;
    }*/

	/*cout<<"Einlesen der Systemgroesse n Zeile, n Spalte: " << endl;
	cout << "Zeile: ";
	cin >> nZeile;
	cout<< "Spalte: ";
	cin >> nSpalte;*/

	do {
		cout<< endl << "Part of the screen occupied by the cells (%): ";
		cin >> frac100;
		frac = frac100 / 100.00;
	} while (frac > 1 || frac < 0);

	/*cout << endl << "Wie viele Zeitschritte: ";
	cin >> nstep;*/

	/*cout << endl << "Resolution: ";
	cout<< endl << "xRes: ";
	cin >> wRes;
	cout<< "yRes:";
	cin >> hRes;*/

	// 2d Matrix erzeugen
	MatrixXi field2d, field2d_tmp;
	field2d = create_field2d(nZeile,nSpalte);
	field2d_tmp = create_field2d(nZeile,nSpalte);

	// 2d Feld initialisieren
	init_field2d(field2d,frac);
	field2d_tmp = field2d;

	SDL_Window *window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wRes, hRes, SDL_WINDOW_OPENGL);

    SDL_Renderer *renderer = init_renderer(window, wRes, hRes);
    SDL_RenderPresent(renderer);

    plot_field(renderer, field2d, wRes, hRes);

	// fuehre nstep Zeitschritte durch:

	vector<CELL> modifiedCells;

	bool quit = false;
	bool pause = false;
	SDL_Event event;

	//for(unsigned int count = 0; count < nstep && !quit; count++){
    //cout <<" nstep: "<< count <<" (max nstep: "<<nstep<<")"<<endl;

    thread t(calculate_and_draw, ref(field2d), ref(field2d_tmp), ref(modifiedCells), renderer, wRes, hRes, ref(quit), ref(pause));
    while (!quit)
    {
        SDL_WaitEvent(&event);
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        {
            quit = true;
        }
        else if (event.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !pause) //PAUSE with Space Bar
        {
            pause = true;
            wait();
            pause = false;
        }
    }
    t.join();
	//}

	SDL_RenderClear(renderer);
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

	return(0);
}





