#include "classes.cpp"
#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

Grid g(0);
int day = 0;
int simtime;
int population, city_size;
float infected_percentage;
int days = 0, entered_days = 0, increase_in_cases = 0;
bool graphic = false, quick = false;

void print_result(int uninf, int inf, int imm, int qua, int dead){
    cout << "\nSimulation started with:\n";
    cout << (population - (population/100*infected_percentage)) << " uninfected.\n";
    cout << (population/100*infected_percentage) << " infected.\n";
    cout << "\nAnd ended with:\n";
    cout << uninf << " uninfected poeple.\n";
    cout << inf << " infected.\n";
    cout << imm << " recovered.\n";
    cout << qua << " currenly in quarantine.\n";
    cout << dead << " dead.\n";

}

/* Draws grid */
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int row, col;
        for(int count= 0; count < g.width*g.width; count++){
                row = floor(count / g.width);
                col = count % g.width;
                if(g.grid[count].state == 1){
                    glColor3f(0.0f, 0.101f, 0.8f);
                } else if (g.grid[count].state == 2){
                    glColor3f(0.7f, 0.301f, 0.3f);
                } else if (g.grid[count].state == 3){
                    glColor3f(0.0f, 0.9f, 0.0f);
                } else if (g.grid[count].state == 4){
                    glColor3f(0.9f, 0.9f, 0.0f);
                } else if (g.grid[count].state == 5){
                    glColor3f(1.0f, 1.0f, 1.0f);
                } else {
                    glColor3f(0.0f, 0.0f, 0.0f);
                }

                glBegin(GL_QUADS);
                glVertex2f(0.1f*col, 0.1f*row);
                glVertex2f(0.1f*(col+1), 0.1f*row);
                glVertex2f(0.1f*(col+1), 0.1f*(row+1));
                glVertex2f(0.1f*col, 0.1f*(row+1));
                glEnd();
        }

        glutSwapBuffers();
        usleep(1);
        glutPostRedisplay();

        /* Every N-th day do testing */
        if (days != -1){
            if(days == 0){
                for (unsigned int pos = 0; pos < g.grid.size(); pos++){
                    if((rand() % 100) < 80){
                        if(g.grid[pos].state == 2){
                            if ((rand() % 100) < 50){
                                g.grid[pos].state = 4;
                                g.grid[pos].duration = 14;
                            }
                        }
                    }
                }
                days = entered_days;
            }
            days--;
        }
        
        /* Counts appearance of each state in grid */
        int uninf = 0, inf = 0, imm = 0, qua = 0, dead = 0;
            for (unsigned int cnt = 0; cnt < g.grid.size(); cnt++){
                if(g.grid[cnt].state == 1){
                    uninf++;
                }
                if(g.grid[cnt].state == 2){
                    inf++;
                }
                if(g.grid[cnt].state == 3){
                    imm++;
                }
                if(g.grid[cnt].state == 4){
                    qua++;
                }
                if(g.grid[cnt].state == 5){
                    dead++;
                }
            }
            
            /* Commented csv format output for graphs */

            /*if(day == 0){
                cout << uninf << ";" << inf << ";" << 0 << ";" << imm << ";" << qua << ";" << dead << endl; 
            } else {
                if ( inf-increase_in_cases < 0){
                    cout << uninf << ";" << inf << ";" << 0 << ";" << imm << ";" << qua << ";" << dead << endl;
                } else {
                    cout << uninf << ";" << inf << ";" << inf-increase_in_cases << ";" << imm << ";" << qua << ";" << dead << endl;
                }
            }
            day++;
            */
        
           /* Overview of each step in simulation */
           cout << "\nDAY: ";
            cout << day;
            cout << "\n Uninfected: ";
            cout << uninf;
            cout << "\n Infected: ";
            cout << inf;
            if(day == 0){
               cout << "\n New cases: ";
                cout << 0; 
            } else {
                if ((inf-increase_in_cases) < 0){
                    cout << "\n New cases: ";
                    cout << 0;
                } else {
                    cout << "\n New cases: ";
                    cout << inf-increase_in_cases;
                } 
            }
            cout << "\n Recovered: ";
            cout << imm;
            cout << "\n Quaratined: ";
            cout << qua;
            cout << "\n Dead: ";
            cout << dead;
            cout << "\n Total alive: ";
            cout << imm+uninf+inf+qua;
            cout << "\n ";
            day++;
            increase_in_cases = inf;

        if (quick != true){
            usleep(100000);
        }

        g.updateGrid();

        if (simtime == 0){
            print_result(uninf,inf,imm,qua,dead);
            exit(0);
        } else {
            simtime--;
        }
}
 

void setup_view(int s){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, s/10, s/10, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, s, s);
 }

/* Printing help */
void printhelp(){
    cout << "This is program for simulating COVID development in a town\n";
    cout << "You need to enter following arguments:\n";
    cout << "-p <int_value> - population (number of people living in the city, for example 40000).\n";
    cout << "-i <float_value> - percentage of infected people in the city.\n";
    cout << "-c <int_value> - city size (<600> will result in 600*600 = 360000). Note: city size/population = population density\n";
    cout << "-d <int_value> - day period in which there will be periodical testing in the city, for example 30 = testing will occur every 30 days, -1 = testing won't happen at all. \n";
    cout << "-g --graphic - enables graphic mode ( for Merlin use 'ssh -X xloginXX@merlin.fit.vutbr.cz' to enable remote display ) \n";
    cout << "-q --quick - enables quicker simulation \n";
    cout << "-t <int_value> - number of days that simulation runs \n";

    exit(0);
}


int main(int argc, char *argv[]){

    if(argc < 2){
        printhelp();
    }

    /* Struct for longargs */
    static struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 'p'},
        {NULL, 0, NULL, 'i'},
        {NULL, 0, NULL, 'c'},
        {NULL, 0, NULL, 'd'},
        {NULL, 0, NULL, 't'},
        {"quick", 0, no_argument, 'q'},
        {"graphic", no_argument, NULL, 'g'}

    };

    char ch;
    /* Loading and setting arguments */
    while ((ch = getopt_long(argc, argv, "hgqp:i:c:d:t:", long_options, NULL)) != -1){
        switch (ch){
            case 'p':
                population = atoi(optarg);
                break;
            case 'h':
                printhelp();
                break;
            case 'i':
                infected_percentage = atoi(optarg);
                break;
            case 'd':
                entered_days = atoi(optarg);
                break;
            case 'c':
                city_size = atoi(optarg);
                break;  
            case 'g':
                graphic = true;
                break;  
            case 'q':
                quick = true;
                break; 
            case 't':
                simtime = atoi(optarg);
                break;
            case '?':
                cout << "UKNOWN ARGUMENT!";
                exit(1);
            default: 
                cout << "UKNOWN ARGUMENT!";
                exit(1);
        }
    }

    days = entered_days;

    if (population > (city_size*city_size)){
        cout << "Population cannot be greater than size of the city.\n";
        exit(0);
    }

    g = Grid(city_size);
    g.fill_grid(population, infected_percentage);
    g.empty_fill_grid();

    cout << "\nSimulation started with:\nPopulation: " << population << "\nPercentage of infected: " << infected_percentage << "\nCity size: " << city_size << "\n";
    if (entered_days < 0){
        cout << "Testing won't occur\n";
    } else {
        cout << "Testing will occur every: " << entered_days << " days\n";
    }
    cout << "Simulation will run for: " << simtime << " days\n";
    usleep(10000);

    if (graphic == true){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800,800);
    glutCreateWindow("Covid Simulation");
    glClearColor(0, 0, 0, 0.0f);
    glutDisplayFunc(display);
    setup_view(city_size);
    glutMainLoop();
    } else {
        while (true){
            if (quick == true){
                usleep(300000);
            } else {
                usleep(1000000);
            }
            
            /* Every N-th day do testing */
             if (days != -1){
                if(days == 0){
                    for (unsigned int pos = 0; pos < g.grid.size(); pos++){
                        if((rand() % 100) < 80){
                            if(g.grid[pos].state == 2){
                                if ((rand() % 100) < 50){
                                    g.grid[pos].state = 4;
                                    g.grid[pos].duration = 14;
                                }
                            }
                        }
                    }
                    days = entered_days;
                }

                days--;
                }
        
        /* Commented csv format output for graphs */
        int uninf = 0, inf = 0, imm = 0, qua = 0, dead = 0;
            for (unsigned int cnt = 0; cnt < g.grid.size(); cnt++){
                if(g.grid[cnt].state == 1){
                    uninf++;
                }
                if(g.grid[cnt].state == 2){
                    inf++;
                }
                if(g.grid[cnt].state == 3){
                    imm++;
                }
                if(g.grid[cnt].state == 4){
                    qua++;
                }
                if(g.grid[cnt].state == 5){
                    dead++;
                }
            } 



            /*if(day == 0){
                cout << uninf << ";" << inf << ";" << 0 << ";" << imm << ";" << qua << ";" << dead << endl; 
            } else {
                if ( inf-increase_in_cases < 0){
                    cout << uninf << ";" << inf << ";" << 0 << ";" << imm << ";" << qua << ";" << dead << endl;
                } else {
                    cout << uninf << ";" << inf << ";" << inf-increase_in_cases << ";" << imm << ";" << qua << ";" << dead << endl;
                }
            }
            day++;
            */


            /* Overview of each step in simulation */
            cout << "\nDAY: ";
            cout << day;
            cout << "\n Uninfected: ";
            cout << uninf;
            cout << "\n Infected: ";
            cout << inf;
            if(day == 0){
               cout << "\n New cases: ";
                cout << 0; 
            } else {
                if ((inf-increase_in_cases) < 0){
                    cout << "\n New cases: ";
                    cout << 0;
                } else {
                    cout << "\n New cases: ";
                    cout << inf-increase_in_cases;
                } 
            }
            cout << "\n Recovered: ";
            cout << imm;
            cout << "\n Quaratined: ";
            cout << qua;
            cout << "\n Dead: ";
            cout << dead;
            cout << "\n Total alive: ";
            cout << imm+uninf+inf+qua;
            cout << "\n ";
            day++;
            increase_in_cases = inf;
            g.updateGrid();

            if (simtime == 0){
                print_result(uninf,inf,imm,qua,dead);
                exit(0);
            } else {
                simtime--;
            }
        }
    }


    return EXIT_SUCCESS;
}
