#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Constants for the grid size and game elements
#define GRID_SIZE 4
#define EMPTY 0
#define PIT 1
#define WUMPUS 2
#define GOLD 3
#define AGENT 4
#define UNKNOWN 5

// Game state variables
int grid[GRID_SIZE][GRID_SIZE];
bool discovered[GRID_SIZE][GRID_SIZE]; // To track what the player has discovered
int agentX = 0, agentY = 0;  // Agent's starting position
bool wumpusAlive = true;
bool gameOver = false;
bool goldFound = false;

// Function to randomly place Wumpus, pits, and gold in the grid
void initializeGrid() {
    srand(time(NULL));  // Seed random number generator
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = EMPTY;
            discovered[i][j] = false;  // Mark all cells as undiscovered initially
        }
    }

    // Randomly place exactly 2 pits
    int placedPits = 0;
    while (placedPits < 2) {
        int pitX = rand() % GRID_SIZE;
        int pitY = rand() % GRID_SIZE;
        if ((pitX == 0 && pitY == 0) || grid[pitX][pitY] != EMPTY) {
            continue; // Avoid placing pit where agent starts or on another object
        } else {
            grid[pitX][pitY] = PIT;
            placedPits++;
        }
    }

    // Randomly place Wumpus
    int wumpusX = rand() % GRID_SIZE;
    int wumpusY = rand() % GRID_SIZE;
    while (wumpusX == 0 && wumpusY == 0) {
        wumpusX = rand() % GRID_SIZE;
        wumpusY = rand() % GRID_SIZE;
    }
    grid[wumpusX][wumpusY] = WUMPUS;

    // Randomly place Gold
    int goldX = rand() % GRID_SIZE;
    int goldY = rand() % GRID_SIZE;
    while (goldX == 0 && goldY == 0) {
        goldX = rand() % GRID_SIZE;
        goldY = rand() % GRID_SIZE;
    }
    grid[goldX][goldY] = GOLD;
}

// Function to print the current grid state from the agent's perspective
void printGrid(bool revealAll) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (i == agentX && j == agentY) {
                printf("A ");  // Agent's position
            } else if (revealAll || discovered[i][j]) {
                switch (grid[i][j]) {
                    case EMPTY: printf(". "); break;
                    case PIT: printf("P "); break;
                    case WUMPUS: printf("W "); break;
                    case GOLD: printf("G "); break;
                }
            } else {
                printf("? ");  // Hide undiscovered areas
            }
        }
        printf("\n");
    }
}

// Function to check for percepts
void checkPercepts() {
    if (grid[agentX][agentY] == PIT) {
        printf("You fell into a pit! Game over.\n");
        gameOver = true;
    } else if (grid[agentX][agentY] == WUMPUS && wumpusAlive) {
        printf("You encountered the Wumpus! Game over.\n");
        gameOver = true;
    } else if (grid[agentX][agentY] == GOLD) {
        printf("You found the gold! You win!\n");
        goldFound = true;
        gameOver = true;
    } else {
        discovered[agentX][agentY] = true;  // Mark the current cell as discovered
        
        // Check for nearby pits (breeze)
        if (agentX > 0 && grid[agentX - 1][agentY] == PIT || 
            agentX < GRID_SIZE - 1 && grid[agentX + 1][agentY] == PIT ||
            agentY > 0 && grid[agentX][agentY - 1] == PIT ||
            agentY < GRID_SIZE - 1 && grid[agentX][agentY + 1] == PIT) {
            printf("You feel a breeze nearby.\n");
        }
        
        // Check for nearby Wumpus (stench)
        if (agentX > 0 && grid[agentX - 1][agentY] == WUMPUS || 
            agentX < GRID_SIZE - 1 && grid[agentX + 1][agentY] == WUMPUS ||
            agentY > 0 && grid[agentX][agentY - 1] == WUMPUS ||
            agentY < GRID_SIZE - 1 && grid[agentX][agentY + 1] == WUMPUS) {
            printf("You smell a stench nearby.\n");
        }

        // Check for nearby gold (glitter)
        if (agentX > 0 && grid[agentX - 1][agentY] == GOLD || 
            agentX < GRID_SIZE - 1 && grid[agentX + 1][agentY] == GOLD ||
            agentY > 0 && grid[agentX][agentY - 1] == GOLD ||
            agentY < GRID_SIZE - 1 && grid[agentX][agentY + 1] == GOLD) {
            printf("You see a glitter nearby.\n");
        }
    }
}

// Function to move the agent
void moveAgent(char move) {
    switch (move) {
        case 'w':
            if (agentX > 0) agentX--;
            break;
        case 's':
            if (agentX < GRID_SIZE - 1) agentX++;
            break;
        case 'a':
            if (agentY > 0) agentY--;
            break;
        case 'd':
            if (agentY < GRID_SIZE - 1) agentY++;
            break;
        default:
            printf("Invalid move!\n");
            break;
    }
}

// Main game loop
int main() {
    initializeGrid();
    
    while (!gameOver) {
        printf("\nCurrent Grid:\n");
        printGrid(false);  // Don't reveal all locations
        checkPercepts();
        
        if (gameOver) break;

        printf("Enter your move (w = up, s = down, a = left, d = right): ");
        char move;
        scanf(" %c", &move);

        moveAgent(move);
    }

    // Game is over, reveal all locations
    printf("\nGame Over! Here is the full grid:\n");
    printGrid(true);  // Reveal all hidden locations

    return 0;
}

