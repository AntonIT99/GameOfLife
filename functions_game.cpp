#include "functions_game.h"

MatrixXi create_field2d(unsigned int nx,unsigned int ny)
{
	return MatrixXi::Zero(nx,ny);
}

void init_field2d(MatrixXi &field, const double frac)
{
	unsigned int cells = frac*field.rows()*field.cols();
	unsigned int count = 0;

	while (count<cells)
	{
		int rand_x = randomInt(0, field.rows());
		int rand_y = randomInt(0, field.cols());
		if(field(rand_x,rand_y) != 1)
        {
			field(rand_x,rand_y) = 1;
		    count++;
		}
	}
}
void make_step(MatrixXi &field, MatrixXi  &field_tmp, vector<CELL> &modifiedCells)
{
    modifiedCells.clear();

	for(int i = 0; i < field.rows(); i++)
	{
		for(int j = 0; j < field.cols(); j++)
		{
			if (field(i,j) == 1)
			{
				if(amountNeighbors(field, i, j) <= 1){ //loneliness
                    field_tmp(i,j) = 0;
                    CELL c;
                    c.x = i;
                    c.y = j;
                    c.value = 0;
					modifiedCells.push_back(c);
				} else if (amountNeighbors(field, i, j) >= 4){ //overcrowding
					field_tmp(i,j) = 0;
                    CELL c;
                    c.x = i;
                    c.y = j;
                    c.value = 0;
					modifiedCells.push_back(c);
				}
			} else if (field(i,j) == 0 && amountNeighbors(field, i, j) == 3) {
				field_tmp(i,j) = 1; //birth
				CELL c;
                c.x = i;
                c.y = j;
                c.value = 1;
                modifiedCells.push_back(c);
			}
		}
	}

	field = field_tmp;
}

void plot_field(SDL_Renderer *renderer, MatrixXi const &field, int ResX, int ResY)
{
    const unsigned int nx = field.rows();
    const unsigned int ny = field.cols();

    for(unsigned int i = 0;i<nx;i++)
    {
        for(unsigned int j=0;j<ny;j++)
        {
            if(field(i,j)==1)
            {
                drawCell(renderer, i, j, ResX, ResY, nx, ny, 0, 255, 255);
            }
            /*else if (field(i,j)==0)
            {
                drawCell(renderer, i, j, ResX, ResY, nx, ny, 0, 0, 0);
            }*/
        }
    }
}

void refresh_field(SDL_Renderer *renderer, int ResX, int ResY, vector<CELL> modifiedCells, int nx, int ny)
{
    for(unsigned int i = 0; i<modifiedCells.size(); i++)
    {
        if(modifiedCells[i].value==1)
        {
            drawCell(renderer, modifiedCells[i].x, modifiedCells[i].y, ResX, ResY, nx, ny, 0, 255, 255);
        }
        else if (modifiedCells[i].value==0)
        {
            drawCell(renderer, modifiedCells[i].x, modifiedCells[i].y, ResX, ResY, nx, ny, 0, 0, 0);
        }
    }
}

void drawCell(SDL_Renderer *renderer, int Posx, int Posy, int ResW, int ResH, int Zeile, int Spalte, int R, int V, int B)
{
    double ratioHeight = (double)(ResH)/(double)(Zeile);
    double ratioWidth = (double)(ResW)/(double)(Spalte);

    int cellSize = 0;

    if (ratioHeight >= ratioWidth)
    {
        cellSize = (int)(ratioWidth);
    }
    else if (ratioHeight < ratioWidth)
    {
        cellSize = (int)(ratioHeight);
    }
    SDL_SetRenderDrawColor(renderer, R, V, B, 0);
    SDL_Rect rect = {Posy*cellSize, Posx*cellSize, cellSize, cellSize};
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}

int randomInt(int MIN, int MAX)
{
	return rand()%(MAX-MIN) + MIN;
}

int amountNeighbors(MatrixXi field, int x, int y)
{
	const unsigned int nx = field.rows();
	const unsigned int ny = field.cols();

	int cells = int(field((x-1+nx)%nx,y))+
            int(field((x+1)%nx,y))+
            int(field(x,(y-1+ny)%ny))+
            int(field(x,(y+1)%ny))+
            int(field((x-1+nx)%nx,(y-1+ny)%ny))+
            int(field((x-1+nx)%nx,(y+1)%ny))+
            int(field((x+1)%nx,(y-1+ny)%ny))+
            int(field((x+1)%nx,(y+1)%ny));
	return cells;
}
