#include <ev3.h>
#include <ev3_output.h>
#include <ev3_lcd.h>
#include <ev3_button.h>
#include <ev3_command.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define Y 32
#define X 19
#define CELL_LENG 0.153
#define MAX_OBSTACLES 20


int counter = 0;


//Defining a big char array to start until path length is figured out
int currentArrSize = 100; // These are the number of possible entries in the path solution array

char *path;

//keep track of path length (used to resize char *path when needed)
int path_length = 0;

//0's are free cells.
//1's are obstacle cells.
int maze[Y][X] = {0};

//matrix to store the solution
int solution[Y][X] = {0};

int manhattanDistance;

int start_x = 0;
int start_y = 0;
int goal_x = 0;
int goal_y = 0;

void MoveFwdByOneTile()
{
	OnFwdSync(OUT_BC, 30);
	Wait(1100);
	Off(OUT_BC);
}

void RotateRightBy90()
{
	OnFwdReg(OUT_C, 19);
	OnRevReg(OUT_B, 19);
	Wait(1240);
	Off(OUT_BC);
}

void RotateLeftBy90()
{
	OnRevReg(OUT_C, 19);
	OnFwdReg(OUT_B, 19);
	Wait(1250);
	Off(OUT_BC);
}


int manhattan_distance(int x, int y)
{
  int temp = abs(x - goal_x) + abs(y - goal_y);
  return temp;
}

//function to print the solution matrix
void printsolution()
{
//    int i,j;
//    for(i=0;i<Y;i++)
//    {
//        for(j=0;j<X;j++)
//        {
//            printf("%d",solution[i][j]);
//        }
//        printf("\n");
//    }
//      printf("\n");
}

//function to print the maze matrix
void printmaze()
{
//    int i,j;
//    for(i=0;i<Y;i++)
//    {
//        for(j=0;j<X;j++)
//        {
//            printf("%d",maze[i][j]);
//        }
//        printf("\n");
//    }
//      printf("\n");
}

//function to solve the maze using backtracking
int backtracking(int row, int column)
{
    //destination index
    if((row == goal_x) && (column == goal_y))
    {
        solution[row][column] = 1;
        return 1;
    }

    //checking if we can visit in this cell or not
    //the indices of the cell must be in (0,SIZE-1)
    //and solution[r][c] == 0 is making sure that the cell is not already visited
    //maze[r][c] == 0 is making sure that the cell is not blocked
    if(row >=0 && column >=0 && row < Y && column < X && solution[row][column] == 0 && maze[row][column] == 0)
    {
        //if safe to visit then visit the cell
        solution[row][column] = 1;

        if(counter == 0)
        {
          counter += 1;

          if(manhattanDistance >= (manhattan_distance(row + 1, column) - 1 ))
          {
            manhattanDistance = manhattan_distance(row + 1, column);
            path_length += 1;
            if(backtracking(row + 1, column))
            {

              path[path_length] = 'U';
              path_length += 1;

              return 1;
            }
          }

          //Moving Right
          if(manhattanDistance >= (manhattan_distance(row, column + 1) - 1) )
          {
            manhattanDistance = manhattan_distance(row, column + 1);
            if(backtracking(row, column  + 1))
            {
              path[path_length] = 'L';
              path_length += 1;
              return 1;
            }
          }

          //Moving Up
          if(manhattanDistance >= (manhattan_distance(row - 1, column) - 1))
          {
            manhattanDistance = manhattan_distance(row - 1, column);
            if(backtracking(row - 1, column))
            {
              path[path_length] = 'D';
              path_length += 1;
              return 1;
            }
          }

          //Moving Left

          if(manhattanDistance >= (manhattan_distance(row, column - 1) - 1))
          {
            manhattanDistance = manhattan_distance(row, column - 1);
            if(backtracking(row, column - 1))
            {
              path[path_length] = 'R';
              path_length += 1;
              return 1;
            }
        }

        }

        else{

        if(manhattanDistance >= (manhattan_distance(row + 1, column)))
        {
          manhattanDistance = manhattan_distance(row + 1, column);
          if(backtracking(row + 1, column))
          {

            path[path_length] = 'U';
            path_length += 1;
            return 1;
          }
        }

        //Moving Right
        if(manhattanDistance >= (manhattan_distance(row, column + 1)))
        {
          manhattanDistance = manhattan_distance(row, column + 1);
          if(backtracking(row, column  + 1))
          {
            path[path_length] = 'L';
            path_length += 1;
            return 1;
          }
        }

        //Moving Up
        if(manhattanDistance >= (manhattan_distance(row - 1, column)))
        {
          manhattanDistance = manhattan_distance(row - 1, column);
          if(backtracking(row - 1, column))
          {
            path[path_length] = 'D';
            path_length += 1;
            return 1;
          }
        }

        //Moving Left
        if(manhattanDistance >= (manhattan_distance(row, column - 1)))
        {
          manhattanDistance = manhattan_distance(row, column - 1);
          if(backtracking(row, column - 1))
          {
            path[path_length] = 'R';
            path_length += 1;
            return 1;
          }
      }
    }

        //backtracking
        solution[row][column] = 0;
        // memset(path, '0', path_length);
        // path_length = 0;
        manhattanDistance = manhattan_distance(start_x,start_y);
        return 0;
    }
    return 0;

}


//padding for obstacles. 
void set_obstacle(int c, int r){
    maze[r-1][c-1] =1;
    maze[r-1][c] =1;
    maze[r-1][c+1] =1;
    maze[r-1][c+2] =1;
    maze[r][c-1] =1;
    maze[r][c] =1;
    maze[r][c+1] =1;
    maze[r][c+2] =1;
    maze[r+1][c-1] =1;
    maze[r+1][c] =1;
    maze[r+1][c+1] =1;
    maze[r+1][c+2] =1;
    maze[r+2][c-1] =1;
    maze[r+2][c] =1;
    maze[r+2][c+1] =1;
    maze[r+2][c+2] =1;
}


int main()
{
	InitEV3();
	ResetRotationCount(OUT_ALL);
	LcdClean(); // Erase everything on the display
	TermPrintln("JSR");
	Wait(500);

	path = malloc( currentArrSize * sizeof(char));
		//making all elements of the solution matrix 0

	double obstacle[MAX_OBSTACLES][2] =             /* obstacle locations          */
	{{0.305, 0.915},{0.61,0.915},{0.915, 0.915},{1.22,0.915},
	 {1.525,0.915},{1.83,0.915}, {1.525,1.83},{1.83, 1.83},
	 {2.135,1.83},{ 2.44, 1.83},{2.745,1.83},{ 0.61,1.83},
	 {0.61, 2.135}, {0.61, 2.44}, {0.61, 2.745},{0.305, 2.745},
	 {0.915, 2.745},{1.22,2.745},{ 1.525, 2.745},{1.83, 2.745},
	 };

	double start[2] = {0.61, 0.305};          /* start location              */

	double goal[2] = {1.525, 3.355};           /* goal location               */

	  int count = 0;


		int i,j;
		for(i=0; i<Y; i++)
		{
			for(j=0; j<X; j++)
			{
				solution[i][j] = 0;
			}
		}

		//Calculate the number of obstacle
		for(i=0; i < MAX_OBSTACLES; i++){
			if(obstacle[i][0] != -1) count++;
		//printf("%d\n", count);
		};

		int r,c;
		for(i=0; i < count; i++){
			c = (obstacle[i][0]/CELL_LENG);//c
			r = (obstacle[i][1]/CELL_LENG);//r
			set_obstacle(c,r);
		};

		start_y = (start[0]/CELL_LENG);
		start_x = (start[1]/CELL_LENG);

		//printf("First %d\n",start_x );

		goal_y = (goal[0]/CELL_LENG);
		goal_x = (goal[1]/CELL_LENG);
		//begin compute path
		manhattanDistance = manhattan_distance(start_x, start_y);

		//start index
		if (backtracking(start_x,start_y)) // compute path planning
		{
			printsolution();
			printmaze();
		}
		else
			LcdPrintf(1,"No solution\n");

		char instructions [path_length];

		j = path_length - 1;

		for(i = 0; i < path_length; i++)
		{

		  instructions[i] = path[j];
		  j -= 1;
		}

		char previous_command = instructions[0];

		for(i = 0; i < path_length; i++)
		{

		  if(instructions[i] != previous_command){


					if(instructions[i] == 'R' && previous_command == 'D')
					{
					  //printf("Command 1\n");
					  previous_command = instructions[i];
					  ///Turn 90 degrees
					  Wait(500);
		        RotateLeftBy90();
		        //Go Forward 0.135
		        Wait(500);
		        MoveFwdByOneTile();
					}

					else if(instructions[i] == 'D' && previous_command == 'R')
					{
					//  printf("Command 2\n");
					  previous_command = instructions[i];
					  //Turn -90 degrees
					  Wait(500);
					  RotateRightBy90();
					  //Go Forward 0.135
					  Wait(500);
					  MoveFwdByOneTile();
					}

					else if(instructions[i] == 'R' && previous_command == 'U')
					{
					//  printf("Command 3\n");
					  previous_command = instructions[i];
					  Wait(500);
					  RotateRightBy90();
					  //Go Forward 0.135
					  Wait(500);
					  MoveFwdByOneTile();
					}

					else if(instructions[i] == 'U' && previous_command == 'R')
					{
					  //printf("Command 4\n");
					  previous_command = instructions[i];
					  //Turn 90 degrees
					  Wait(500);
					  RotateLeftBy90();
					  //Go Forward 0.135
					  Wait(500);
					  MoveFwdByOneTile();
					}



					else if(instructions[i] == 'L' && previous_command == 'D')
					{
					  //printf("Command 5\n");
					  previous_command = instructions[i];
					  //Turn -90 degrees
					            RotateRightBy90();
					            //Go Forward 0.135
					            MoveFwdByOneTile();
					}

					else if(instructions[i] == 'D' && previous_command == 'L')
					{
					  //printf("Command 6\n");
					  previous_command = instructions[i];
					  ///Turn 90 degrees
					  Wait(500);
			          RotateLeftBy90();
			          //Go Forward 0.135
			          Wait(500);
			          MoveFwdByOneTile();
					}

					else if(instructions[i] == 'L' && previous_command == 'U')
					{
					  //printf("Command 7\n");
					  previous_command = instructions[i];
					  //Turn 90 degrees
					  RotateLeftBy90();
					  //Go Forward 0.135
					  Wait(500);
					  MoveFwdByOneTile();
					}

					else if(instructions[i] == 'U' && previous_command == 'L')
					{
					//  printf("Command 8\n");
					  previous_command = instructions[i];
					  //Turn -90 degrees
					  Wait(500);
					  RotateRightBy90();
					  //Go Forward 0.135
					  Wait(500);
					  MoveFwdByOneTile();
					}
		 }

		else
		{
		  previous_command = instructions[i];

	    MoveFwdByOneTile();

		}
	 }

	return 0;
}
