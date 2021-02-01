#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;


/*#define population 45000
#define infected_percentage 1*/

// state of person
#define EMPTY 0
#define UNINFECTED 1
#define INFECTED 2
#define RECOVERED 3
#define QUARANTINED 4
#define DEAD 5

// direction
#define UP 10
#define RIGHT 11
#define LEFT 12
#define DOWN 13

class Cell {
    public:

    // 0 - EMPTY, 1 -Uninfected, 2 - Infected, 3 - RECOVERED, 4 - Quarantine
    int state;
    int duration;

    Cell(int state, int duration){
        this->duration = duration;
        this->state = state;
    }
};

class Grid {
    public:
    vector<Cell> grid;
    vector<Cell> new_grid;

    int width;

    Grid(int width){
        this->width = width;
        grid.reserve(width*width);
        new_grid.reserve(width*width);
    }

    void empty_fill_grid(){
        for (int x=0; x < this->width*this->width; x++){
            this->new_grid.push_back(Cell(0,0));
        }
    }

    /* Generating grid of people */
    void fill_grid(int population, float infected_percentage){

        /* Percentage of infected */
        int infected = population/100*infected_percentage;
        int healthy = population-infected;

        for (int x=0; x < infected; x++){
            this->grid.push_back(Cell(2,rand() % 15));
        }
        for (int x=0; x < healthy; x++){
            this->grid.push_back(Cell(1,0));
        }
        for (int x=0; x < ((this->width*this->width)-population); x++){
            this->grid.push_back(Cell(0,0));
        }
        
        /* Shuffling people around */
        random_shuffle((this->grid).begin(), (this->grid).end());
    }

    /* Converts 2D coordinates to 1D array */
    int _2Dto1D(int row, int col){
        return (this->width * row) + col;
    }


    /* Resolves states based on current state and state of cell person is about to move to */
    void resolve_states(int current_state, int next_state, int pos, int next_pos){
        switch (current_state)
        {
        case UNINFECTED:
            
            switch (next_state)
            {   
                case EMPTY:
                    this->new_grid[pos].state = EMPTY; 
                    this->new_grid[next_pos].state = UNINFECTED; 
                    break;
                case INFECTED:


                    if ((rand() % 10) > 4){
                        this->new_grid[pos].state = INFECTED; 
                        this->new_grid[pos].duration = 14; 
                    }
                    else {
                            this->new_grid[pos].state = UNINFECTED;
                        }
                    break;

                case UNINFECTED:
                case RECOVERED:
                case QUARANTINED:
                case DEAD:
                    this->new_grid[pos].state = this->grid[pos].state;
                    break;
            }  
            break;
        
        case INFECTED:

            if ((rand() % 10000) > 9997){
                this->new_grid[pos].state = DEAD;
                break;
            }

            if(this->grid[pos].duration == 0){
                this->new_grid[pos].state = RECOVERED;
                break;
            } else{

                this->grid[pos].duration--;

                switch (next_state)
                {   
                    case EMPTY:

                        this->new_grid[pos].state = EMPTY; 

                        this->new_grid[next_pos].state = INFECTED; 
                        this->new_grid[next_pos].duration = this->grid[pos].duration;
                        break;
                    
                    case UNINFECTED:
                        if ((rand() % 10) > 4){
                        this->new_grid[next_pos].state = INFECTED; 
                        this->new_grid[next_pos].duration = 14; 
                        }

                        this->new_grid[pos].state = this->grid[pos].state;
                        this->new_grid[pos].duration = this->grid[pos].duration;
                        break;

                    case RECOVERED:
                    case INFECTED:
                    case QUARANTINED:
                    case DEAD:
                        this->new_grid[pos].state = this->grid[pos].state;
                        this->new_grid[pos].duration = this->grid[pos].duration;
                        break;
                }
            }
            break;

        case RECOVERED:
            switch (next_state)
                {
                    case EMPTY:

                        this->new_grid[pos].state = EMPTY; 
                        this->new_grid[next_pos].state = RECOVERED; 
                        break;
                    case INFECTED:
                    case UNINFECTED:
                    case RECOVERED:
                    case QUARANTINED:
                    case DEAD:
                        this->new_grid[pos].state = this->grid[pos].state;
                        break;
                }
            break;
        
        case QUARANTINED:
            if (this->grid[pos].duration == 0){
                this->new_grid[pos].state = RECOVERED; 
            } else {
                this->grid[pos].duration--;
                this->new_grid[pos].state = QUARANTINED;
                this->new_grid[pos].duration = this->grid[pos].duration;
            }
            break;
        case DEAD:
            this->new_grid[pos].state = DEAD;
            break;
        }  
    }

    /* Gets random position to which person is about to move and further goes to "resolve_state" to decide what happens based on that movement */
    void m(Cell c, int pos){
        
        // 2D -> 1D (width * row) + col
        /* 1D ->2D 
        *   row = i/width
        *   col = i%width
        */
       
        int row, col, current_state, next_state, next_pos;
        row = pos / floor(this->width);
        col = pos % this->width;

        int move = (rand() % 4)+10;
        
        switch (move)
        {
            case UP:
                // Cell not at the top border
                if (row != 0){
                    next_pos = _2Dto1D(row-1,col);
                    current_state = c.state;
                    next_state = this->new_grid[next_pos].state;
                    resolve_states(current_state, next_state, pos, next_pos);
                } else {             
                    next_pos = pos;
                    current_state = c.state;
                    next_state = this->grid[next_pos].state;
                    resolve_states(current_state, next_state, pos, next_pos);
                }
                break;
            
            case RIGHT:
            
                // Cell not at the right border
                if(col != this->width-1){
                    next_pos = _2Dto1D(row, col+1);
                    current_state = c.state;
                    if (this->new_grid[next_pos].state != EMPTY){
                        next_state = this->new_grid[next_pos].state;
                    } else{
                        next_state = this->grid[next_pos].state;
                    }
                    resolve_states(current_state, next_state, pos, next_pos);
                } else {
                    next_pos = pos;
                    current_state = c.state;
                    next_state = this->grid[next_pos].state;
                    resolve_states(current_state, next_state, pos, next_pos);
                }
                break;
                
            case LEFT:
                
                // Cell not at the left border 
                if(col != 0){
                    next_pos = _2Dto1D(row, col-1);
                    current_state = c.state;
                    next_state = this->new_grid[next_pos].state;
                    resolve_states(current_state, next_state, pos, next_pos);
                } else {
                    next_pos = pos;
                    current_state = c.state;
                    next_state = this->grid[next_pos].state;
                    resolve_states(current_state, next_state, pos, next_pos);
                }
                break;
                
            case DOWN:
                
                // Cell not at the bottom border
                if(row != this->width-1){
                    next_pos = _2Dto1D(row+1, col);
                    current_state = c.state;
                    next_state = this->grid[next_pos].state;
                    resolve_states(current_state, next_state, pos, next_pos);
                } else {
                    next_pos = pos;
                    current_state = c.state;
                    next_state = this->grid[next_pos].state;
                    resolve_states(current_state, next_state, pos, next_pos);
                }
                break;
        }
    }

    /* Updates grid */
    void updateGrid(){
        int count = 0;
        for (int x=0; x < this->width; x++){
            for (int y=0; y < this->width; y++){
                
                if(this->grid[count].state != EMPTY){
                    m(this->grid[count], count);                
                }
                count++;
            }
        }
        this->grid.clear();
        this->grid = this->new_grid;
        this->new_grid.clear();
        this->empty_fill_grid();
    }


};