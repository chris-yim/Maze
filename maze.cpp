#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>

#include "maze.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

//OWN CODE--------------------------------------------------------------------------

bool find_marker(char ch, char** maze, int height, int width, int& row, int& column)
{
  row = -1;
  column = -1;

  for(int h = 0; h < height; h++){
    for(int w = 0; w < width; w++)
    {
      if(maze[h][w] == ch)
      {
        row = h;
        column = w;
        return true;
      }
    }
  }

  return false;
}

bool valid_solution(char* path, char**maze, int height, int width)
{
  int pathLength = strlen(path);
  //cout << "Length of path instruc: " << pathLength << endl;

  int startRow = 0;
  int startCol = 0;
  if(!find_marker('>', maze, height, width, startRow, startCol))
  {
    cerr << "Cannot find start marker" << endl;
  }
  int endRow = 0;
  int endCol = 0;
  if(!find_marker('X', maze, height, width, endRow, endCol))
  {
    cerr << "Cannot find end marker" << endl;
  }

  int currentRow = startRow;
  int currentCol = startCol;
  int moveCount = 0;
  for(int i = 0; i < pathLength; i++)
  {
    switch(*(path+i))
    {
      case 'N':
      {
        currentRow--;
        break;
      }
      case 'E':
      {
        currentCol++;
        break;
      }
      case 'S':
      {
        currentRow++;
        break;
      }
      case 'W':
      {
        currentCol--;
        break;
      } 
      default:
      {
        cerr << "Reached default case" << endl;
        break;
      }
    }
    if(maze[currentRow][currentCol] == 'X')
    {
      cout << "Reached the end" << endl;
      return true;
    }
    else if(maze[currentRow][currentCol] != ' ')
    {
      cout << "\nReached non-empty space, it contains: " << maze[currentRow][currentCol] << ", row: " << currentRow << ", col: " << currentCol << endl;
      return false;
    }
  }

  // if(moveCount == pathLength)
  // {
  //   return true;
  // }
  return false;
}

char *find_path(char **maze, int height, int width, char start, char end)
{
  char* pathSolution = new char[1000];
  for(int i = 0; i < 999; i++)
  {
    *(pathSolution + i) = ' ';
  }
  //Find location of start and of finish
  int startRow = 0;
  int startCol = 0;
  if(!find_marker(start, maze, height, width, startRow, startCol))
  {
    cerr << "Cannot find start marker" << endl;
  }
  int endRow = 0;
  int endCol = 0;
  if(!find_marker(end, maze, height, width, endRow, endCol))
  {
    cerr << "Cannot find end marker" << endl;
  }

  int currentRow = startRow;
  int currentCol = startCol;
  for(int i = 0; i < 4; i++) //0 to 3 represents N, E, S, W
  {
    if(i == 0)
    { //North
      if(maze[currentRow-1][currentCol] == ' ' && !(currentRow >= height || currentCol >= width || currentRow < 0 || currentCol < 0))
      {
        *pathSolution = 'N';
        maze[currentRow-1][currentCol] = '#';
        if(find_path_helper(maze, height, width, start, end, pathSolution, 2, currentRow-1, currentCol))
        {
          return pathSolution;
        }
      }
    }
    else if (i == 1)
    { //East
      if(maze[currentRow][currentCol+1] == ' ' && !(currentRow >= height || currentCol >= width || currentRow < 0 || currentCol < 0))
      {
        *pathSolution = 'E';
        maze[currentRow][currentCol+1] = '#';
        if(find_path_helper(maze, height, width, start, end, pathSolution, 2, currentRow, currentCol+1))
        {
          //cout << "East" << endl;
          return pathSolution;
        }
      }
    }
    else if (i == 2)
    { //South
      if(maze[currentRow+1][currentCol] == ' ' && !(currentRow >= height || currentCol >= width || currentRow < 0 || currentCol < 0))
      {
        *pathSolution = 'S';
        maze[currentRow+1][currentCol] = '#';
        if(find_path_helper(maze, height, width, start, end, pathSolution, 2, currentRow+1, currentCol))
        {
          //cout << "South" << endl;
          return pathSolution;
        }
      }
    } 
    else if (i == 3)
    { //West
      if(maze[currentRow][currentCol-1] == ' ' && !(currentRow >= height || currentCol >= width || currentRow < 0 || currentCol < 0))
      {
        *pathSolution = 'W';
        maze[currentRow][currentCol-1] = '#';
        if(find_path_helper(maze, height, width, start, end, pathSolution, 2, currentRow, currentCol-1))
        {
          return pathSolution;
        }
      }
    }
  }
  strcpy(pathSolution, "no solution");
  return pathSolution;
}

bool find_path_helper(char **maze, int height, int width, char start, char end, char* path, int step, int currentRow, int currentCol)
{
  // cout << "Current path values: "; 
  // for(int i = 0; i < strlen(path); i++)
  // {
  //   cout << *(path + i);
  // }
  // cout << endl;

  int endRow = 0;
  int endCol = 0;
  if(!find_marker(end, maze, height, width, endRow, endCol))
  {
    cerr << "Cannot find end marker" << endl;
  }
  if(currentRow == endRow && currentCol == endCol)
  {
    cout << "Reached the end" << endl;
    if(valid_solution(path, maze, height, width))
    {
      return true;
    }
  }

  for(int i = 0; i < 4; i++) //0 to 3 represents N, E, S, W
  {
    if(i == 0)
    { //North
      if(maze[currentRow-1][currentCol] == end)
      {
        *(path+step-1) = 'N';
        maze[currentRow-1][currentCol] = '#';
        return true;
      }
      else if(maze[currentRow-1][currentCol] == ' ' && !(currentRow >= height || currentCol >= width || currentRow < 0 || currentCol < 0))
      {
        *(path+step-1) = 'N';
        maze[currentRow-1][currentCol] = '#';
        if(find_path_helper(maze, height, width, start, end, path, step+1, currentRow-1, currentCol))
        {
          return true;
        }
        else
        {
          maze[currentRow-1][currentCol] = ' ';
        }
      }
    }
    else if (i == 1)
    { //East
      if(maze[currentRow][currentCol+1] == end)
      {
         *(path+step-1) = 'E';
        maze[currentRow][currentCol+1] = '#';
        return true;
      }
      else if(maze[currentRow][currentCol+1] == ' ' && !(currentRow >= height || currentCol >= width || currentRow < 0 || currentCol < 0))
      {
        *(path+step-1) = 'E';
        maze[currentRow][currentCol+1] = '#';
        if(find_path_helper(maze, height, width, start, end, path, step+1, currentRow, currentCol+1))
        {
          return true;
        }
        else
        {
          maze[currentRow][currentCol+1] = ' ';
        }
      }
    }
    else if (i == 2)
    { //South
      if(maze[currentRow+1][currentCol] == end)
      {
         *(path+step-1) = 'S';
        maze[currentRow+1][currentCol] = '#';
        return true;
      }
      else if(maze[currentRow+1][currentCol] == ' ' && !(currentRow >= height || currentCol >= width || currentRow < 0 || currentCol < 0))
      {
        *(path+step-1) = 'S';
        maze[currentRow+1][currentCol] = '#';
        if(find_path_helper(maze, height, width, start, end, path, step+1, currentRow+1, currentCol))
        {
          return true;
        }
        else
        {
          maze[currentRow+1][currentCol] = ' ';
        }
      }
    } 
    else if (i == 3)
    { //West
      if(maze[currentRow][currentCol-1] == end)
      {
         *(path+step-1) = 'W';
        maze[currentRow][currentCol-1] = '#';
        return true;
      }
      else if(maze[currentRow][currentCol-1] == ' ' && !(currentRow >= height || currentCol >= width || currentRow < 0 || currentCol < 0))
      {
        *(path+step-1) = 'W';
        maze[currentRow][currentCol-1] = '#';
        if(find_path_helper(maze, height, width, start, end, path, step+1, currentRow, currentCol-1))
        {
          return true;
        }
        else
        {
          maze[currentRow][currentCol-1] = ' ';
        }
      }
    }
  }
  //print_maze(maze, height, width);
  *(path+step-1) = ' ';
  return false;
}