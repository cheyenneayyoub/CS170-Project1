#include <iostream>
#include <queue>

using namespace std;

struct state{
  int puzzle[3][3];
  int heuristic;
  int cost = 0;
};

struct CustomCompare { //made a custom compare so it compares properly (from http://neutrofoton.github.io/blog/2016/12/29/c-plus-plus-priority-queue-with-comparator/)
  bool operator()(const state& lhs, const state& rhs) {
    return (lhs.cost + lhs.heuristic) > (rhs.cost + rhs.heuristic);
  }
};

void outputPuzzle(state problem);
state copyState(state toBeCopied);
int calcHeuristic(int algChoice, state problem);
void expand(priority_queue<state,vector<state>, CustomCompare >& node, state curr, int algChoice);
int misplacedTile(state problem);
state generalSearch(state problem, int algChoice);
int manhattan(state problem);


bool goalStateCheck(state checkPuzzle){ //function that returns true if checkPuzzle matches the goal state
  state goalState;
  goalState.puzzle[0][0] = 1;
  goalState.puzzle[0][1] = 2;
  goalState.puzzle[0][2] = 3;
  goalState.puzzle[1][0] = 4;
  goalState.puzzle[1][1] = 5;
  goalState.puzzle[1][2] = 6;
  goalState.puzzle[2][0] = 7;
  goalState.puzzle[2][1] = 8;
  goalState.puzzle[2][2] = 0;
  for(unsigned int i = 0; i < 3; ++i){
    for(unsigned int j = 0; j < 3; ++j){
      if(goalState.puzzle[i][j] != checkPuzzle.puzzle[i][j]){
        return false;
      }
    }
  }
  return true;
}

void outputPuzzle(state problem){ //a function that outputs the puzzle at the state it's in
  cout << "g(n) for this state: " << problem.cost << endl<< "h(n) for this state: " << problem.heuristic << endl;
  for(int i = 0; i < 3; ++i){
    for(int j = 0; j < 3; ++j){
      cout << problem.puzzle[i][j] << " ";
    }
    cout << endl;
  }
}

state copyState(state toBeCopied){ //a function that copies the puzzle at the state it's in so that we don't override the original state
  state copied;
  for(int i = 0; i < 3; ++i){
    for(int j = 0; j < 3; ++j){
      copied.puzzle[i][j] = toBeCopied.puzzle[i][j];
    }
  }
  copied.heuristic = toBeCopied.heuristic; //copies its h(n)
  copied.cost = toBeCopied.cost+1; //copies its cost and adds 1
  return copied;
}

int calcHeuristic(int algChoice, state problem) { //calculates the heuristic based on the user's input. This is used later in generalSearch() function
  if(algChoice == 1) { //if uniform cost search
    return 0;
  }
  if(algChoice == 2) { //if misplaced tile
    return misplacedTile(problem);
  }
  if(algChoice == 3) { //if manhattan dist
    return manhattan(problem);
  }
  return 0;
}


void expand(priority_queue<state,vector<state>, CustomCompare >& node, state curr, int algChoice){ //this function expands the parent state into all possible children states.
  int rowOfZero = 0;
  int columnOfZero = 0;

  for(int i = 0; i < 3; ++i){
    for(int j = 0; j < 3; ++j){
      if(curr.puzzle[i][j] == 0){ //looking for 0 in the puzzle
        rowOfZero = i;
        columnOfZero = j;
      }
    }
  }

  if(rowOfZero == 0){ //if it's in the first row, 0 can only go down
    state newState;
    newState = copyState(curr);
    swap(newState.puzzle[rowOfZero][columnOfZero], newState.puzzle[rowOfZero+1][columnOfZero]);
    newState.heuristic = calcHeuristic(algChoice, newState);
    node.push(newState);
    if(columnOfZero < 2) { //makes it possible for 0 to move right
      newState = copyState(curr);
      swap(newState.puzzle[rowOfZero][columnOfZero], newState.puzzle[rowOfZero][columnOfZero+1]);
      newState.heuristic = calcHeuristic(algChoice, newState);
      node.push(newState);
    }
    if(columnOfZero > 0) { //makes it possible for 0 to move left
      newState = copyState(curr);
      swap(newState.puzzle[rowOfZero][columnOfZero], newState.puzzle[rowOfZero][columnOfZero-1]);
      newState.heuristic = calcHeuristic(algChoice, newState);
      node.push(newState);
    }
  }
  else if(rowOfZero == 1){ //if 0 in middle row, this allows it to move up or down
    state newState;
    newState = copyState(curr);
    swap(newState.puzzle[rowOfZero][columnOfZero], newState.puzzle[rowOfZero+1][columnOfZero]); //moves down
    newState.heuristic = calcHeuristic(algChoice, newState);
    node.push(newState);
    newState = copyState(curr);
    swap(newState.puzzle[rowOfZero][columnOfZero], newState.puzzle[rowOfZero-1][columnOfZero]); //moves up
    newState.heuristic = calcHeuristic(algChoice, newState);
    node.push(newState);
    if(columnOfZero < 2) {//makes it possible for 0 to move right
      newState = copyState(curr);
      swap(newState.puzzle[rowOfZero][columnOfZero], newState.puzzle[rowOfZero][columnOfZero+1]);
      newState.heuristic = calcHeuristic(algChoice, newState);
      node.push(newState);
    }
    if(columnOfZero > 0) {//makes it possible for 0 to move left
      newState = copyState(curr);
      swap(newState.puzzle[rowOfZero][columnOfZero], newState.puzzle[rowOfZero][columnOfZero-1]);
      newState.heuristic = calcHeuristic(algChoice, newState);
      node.push(newState);
    }
  }
  else if(rowOfZero == 2){ //if 0 in third row, this allows it to move up
    state newState;
    newState = copyState(curr);
    swap(newState.puzzle[rowOfZero][columnOfZero], newState.puzzle[rowOfZero-1][columnOfZero]); //moves up
    newState.heuristic = calcHeuristic(algChoice, newState);
    node.push(newState);
    if(columnOfZero < 2) { //makes it possible for 0 to move right
      newState = copyState(curr);
      swap(newState.puzzle[rowOfZero][columnOfZero], newState.puzzle[rowOfZero][columnOfZero+1]);
      newState.heuristic = calcHeuristic(algChoice, newState);
      node.push(newState);
    }
    if(columnOfZero > 0) { //makes it possible for 0 to move right
      newState = copyState(curr);
      swap(newState.puzzle[rowOfZero][columnOfZero], newState.puzzle[rowOfZero][columnOfZero-1]);
      newState.heuristic = calcHeuristic(algChoice, newState);
      node.push(newState);
    }
  }
}

int misplacedTile(state problem){ //finds how many tiles are out of place and adds the number up and returns it to be used in calcHeuristic() function
  state goalState;
  goalState.puzzle[0][0] = 1;
  goalState.puzzle[0][1] = 2;
  goalState.puzzle[0][2] = 3;
  goalState.puzzle[1][0] = 4;
  goalState.puzzle[1][1] = 5;
  goalState.puzzle[1][2] = 6;
  goalState.puzzle[2][0] = 7;
  goalState.puzzle[2][1] = 8;
  goalState.puzzle[2][2] = 0;
  int count = 0;
  for(int i = 0; i < 3; ++i){
    for(int j = 0; j < 3; ++j){
      if(problem.puzzle[i][j] != goalState.puzzle[i][j]){ //finds a misplaced tile
        count++;
      }
    }
  }
  return count;
}

int manhattan(state problem){ // finds the manhattan distance
  state goalState;
  goalState.puzzle[0][0] = 1;
  goalState.puzzle[0][1] = 2;
  goalState.puzzle[0][2] = 3;
  goalState.puzzle[1][0] = 4;
  goalState.puzzle[1][1] = 5;
  goalState.puzzle[1][2] = 6;
  goalState.puzzle[2][0] = 7;
  goalState.puzzle[2][1] = 8;
  goalState.puzzle[2][2] = 0;
  int manhattanDist = 0;
  int probRow = 0;
  int probCol = 0;
  int goalRow = 0;
  int goalCol = 0;

  for(int puzzleRange = 1; puzzleRange < 10; ++puzzleRange){ //the numbers in the puzzle from 1 through 9 with 9 being placed as 0 so we don't count the manhattan distance for 0
    for(int i = 0; i < 3; ++i){
      for(int j = 0; j < 3; ++j){
        if(problem.puzzle[i][j] == puzzleRange){ //finds the row and column of when the problem puzzle equals the number in the puzzle
          probRow = i;
          probCol = j;
        }
        if(goalState.puzzle[i][j] == puzzleRange){ //finds the row and column of when the goal state equals the number in the puzzle
          goalRow = i;
          goalCol = j;
        }
      }
    }
    manhattanDist += abs(goalRow-probRow) + abs(goalCol-probCol); //calculates the total manhattan distance
  }      

    return manhattanDist;

  
}


state generalSearch(state problem, int algChoice){ //follows the gsa algorithm given
  problem.heuristic = calcHeuristic(algChoice, problem);

  priority_queue<state,vector<state>, CustomCompare > node;
  node.push(problem); //pushes the problem onto the priority queue
  state curr;
  int currMax = 0;
  int count = 0;

  while(node.size()!=0){ 
    if(node.size() > currMax) {
      currMax = node.size(); //updates the max for max queue size
    }
    curr = node.top();
    node.pop();
    if(goalStateCheck(curr)){  //if we reach the goal state
      outputPuzzle(curr);
      cout << "Success! Goal state reached" << endl;
      cout << "Solution depth: " << curr.cost << endl;
      cout << "Max queue size: " << currMax << endl;
      cout << "Number of nodes expanded: " << count << endl;
      return curr;
    }
    else{ //didn't reach goal state yet so we have to expand
      expand(node, curr, algChoice);
      count++;
      outputPuzzle(curr);
    }
  }
  exit(1);
}


int main() {
  cout << "Welcome to Cheyenne's CS170 8-Puzzle Solver. Type '1' to use a default puzzle or '2' to create your own." << endl;

  int defaultOrOwn;

  cin >> defaultOrOwn;

  while(defaultOrOwn < 1 || defaultOrOwn > 2){
    cout << "Please enter '1' for a default puzzle or '2' to create your own." << endl;
    cin >> defaultOrOwn;
  }
  if(defaultOrOwn == 1){ //choosing default puzzle
    int searchAlg;
    cout << "You have selected default." << endl;
    cout << "Pick '1' for Uniform Cost Search, '2' for A* with Misplace Tile, or '3' for A* with Manhattan Distance." << endl;
    cin >> searchAlg;
    while(searchAlg < 1 || searchAlg > 3){
      cout << "Pick '1' for Uniform Cost Search, '2' for A* with Misplace Tile, or '3' for A* with Manhattan Distance." << endl;
      cin >> searchAlg;
    }
    state defaultState;
    defaultState.puzzle[0][0] = 1;
    defaultState.puzzle[0][1] = 2;
    defaultState.puzzle[0][2] = 3;
    defaultState.puzzle[1][0] = 5;
    defaultState.puzzle[1][1] = 0;
    defaultState.puzzle[1][2] = 6;
    defaultState.puzzle[2][0] = 4;
    defaultState.puzzle[2][1] = 7;
    defaultState.puzzle[2][2] = 8;
    generalSearch(defaultState, searchAlg);
  }
  else if(defaultOrOwn == 2){ //choosing to make their own puzzle
    int ownPuzzleNums;
    state ownPuzzle;
    cout << "You have selected to make your own." << endl << "Please input your rows with spaces or tabs in between and use return to start a new row. Use 0 to indicate the blank." << endl;
    for(int i = 0; i < 3; ++i){
      for(int j = 0; j < 3; ++j){
        cin >> ownPuzzleNums;
        ownPuzzle.puzzle[i][j] = ownPuzzleNums;
      }
    }
    int searchAlg;

    cout << "Pick '1' for Uniform Cost Search, '2' for A* with Misplace Tile, or '3' for A* with Manhattan Distance." << endl;
    cin >> searchAlg;
    while(searchAlg < 1 || searchAlg > 3){
      cout << "Pick '1' for Uniform Cost Search, '2' for A* with Misplace Tile, or '3' for A* with Manhattan Distance." << endl;
      cin >> searchAlg;
    }
    generalSearch(ownPuzzle, searchAlg);
  }

return 0;
} 
