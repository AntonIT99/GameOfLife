#ifndef FUNCTIONS_GAME_H_
#define FUNCTIONS_GAME_H_

#include <cstdlib>
#include<vector>
#include <Eigen/Dense>
#include <SDL2/SDL.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <time.h>

using namespace Eigen;
using namespace std;


typedef struct CELL CELL;
struct CELL
{
    int x;
    int y;
    int value;
};

MatrixXi create_field2d(unsigned int nx,unsigned int ny);
void init_field2d(MatrixXi &field, const double frac);
void make_step(MatrixXi &field, MatrixXi &field_tmp, vector<CELL> &modifiedCells);
void plot_field(SDL_Renderer *renderer, MatrixXi const &field, int ResX, int ResY);
void refresh_field(SDL_Renderer *renderer, int ResX, int ResY, vector<CELL> modifiedCells, int nx, int ny);
void refresh_field2(SDL_Renderer *renderer, int ResX, int ResY, vector<CELL> modifiedCells, int nx, int ny);
void drawCell(SDL_Renderer *renderer, int Posx, int Posy, int ResW, int ResH, int Zeile, int Spalte, int R, int V, int B);
int randomInt(int MIN, int MAX);
int amountNeighbors(MatrixXi field, int x, int y);

#endif
