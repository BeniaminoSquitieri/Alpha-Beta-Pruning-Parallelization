/*
/*
 * Course: High Performance Computing 2022/2023
 *
 * Lecturer: Francesco Moscato fmoscato@unisa.it
 *
 * Group:
 * Milone Marco	0622701992	m.milone15@studenti.unisa.it
 * Sarno Fabrizio	0622702052  f.sarno14@studenti.unisa.it
 * Squitieri Beniamino	0622702021  b.squitieri@studenti.unisa.it
 *
 * Provide a parallel version of the Alpha-beta pruning algorithm to improve MinMax algorithm’s performances in Tic-Tac-Toe game.
 * The implementation MUST use shared memory paradigm and has to be implemented by using OpenMP. Students MUST provide parallel
 * processes on different game tree’s branches, and each process has to evaluate the best move to make in order to win the match against
 * the other opponent.
 *
 * Copyright (C) 2023 - All Rights Reserved
 *
 * This file is part of CommonAssignmentOpenMP
 *
 * CommonAssignmentOpenMP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CommonAssignmentOpenMP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CommonAssignmentOpenMP . If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <omp.h>

// Constants for the different game results (WIN, DRAW, LOSS)
#define WIN 1000
#define DRAW 0
#define LOSS -1000

// Markers for AI and player ('O' and 'X' respectively),
// and an empty space on the board ('-')
#define AI_MARKER 'O'
#define PLAYER_MARKER 'X'
#define EMPTY_SPACE '-'

#define WINNING_STATES_SIZE 8
#define START_DEPTH 0
#define BOARD_SIZE 3
#define MAX_MOVES 9

/** @brief Function that finds the maximum of two integers
 * @param a is the first integer
 * @param b is the second integer
 * @return returns the maximum between a and b */
int max(int a, int b)
{
    return (a > b) ? a : b;
}

/** @brief Function that finds the minimum of two integers
 * @param a is the first integer
 * @param b is the second integer
 * @return returns the minimum between a and b */
int min(int a, int b)
{
    return (a < b) ? a : b;
}

/** @brief Function that, given the status, determines whether the game ended with a victory or a defeat for the player or with a draw
 * @param state is an integer used to evaluate the outcome of the game */
void print_game_state(int state)
{
    if (state == WIN)
    {
        printf("WIN\n");
    }
    else if (state == DRAW)
    {
        printf("DRAW\n");
    }
    else if (state == LOSS)
    {
        printf("LOSS\n");
    }
}

/** @brief Structure that models a pair of board coordinates
 * @param x is the value for the row
 * @param y is the value for the column*/
struct Pair
{
    int x, y;
};

/** @brief Structure that models a move of the board
 * @param score is an integer indicating the quality of the move
 * @param p is a pair of coordinates of type Pair*/
struct Move
{
    int score;
    struct Pair p;
};

/** @brief Matrix of type Pair which contains all the set of three pairs
 * representing the winning combinations of tris*/
struct Pair winning_states[WINNING_STATES_SIZE][BOARD_SIZE] = {
    {{0, 0}, {0, 1}, {0, 2}},
    {{1, 0}, {1, 1}, {1, 2}},
    {{2, 0}, {2, 1}, {2, 2}},
    {{0, 0}, {1, 0}, {2, 0}},
    {{0, 1}, {1, 1}, {2, 1}},
    {{0, 2}, {1, 2}, {2, 2}},
    {{0, 0}, {1, 1}, {2, 2}},
    {{2, 0}, {1, 1}, {0, 2}}};

/** @brief Function that prints the tris board
 * @param board is matrix of char representing the tris board */
void print_board(char board[BOARD_SIZE][BOARD_SIZE])
{
    printf("\n");
    printf("%c | %c | %c\n", board[0][0], board[0][1], board[0][2]);
    printf("--+---+--\n");
    printf("%c | %c | %c\n", board[1][0], board[1][1], board[1][2]);
    printf("--+---+--\n");
    printf("%c | %c | %c\n\n", board[2][0], board[2][1], board[2][2]);
}

/** @brief Function that calculates all available legal moves on the board (spaces that are not occupied by either player)
 * @param board is matrix of char representing the tris board
 * @param legal_moves is a pointer of type Pair used to save all legal moves
 * @return number of legal moves*/
int get_legal_moves(char board[BOARD_SIZE][BOARD_SIZE], struct Pair *legal_moves)
{
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] != AI_MARKER && board[i][j] != PLAYER_MARKER)
            {
                legal_moves[count].x = i; // Inserisce nuova coppia che rappresenta la mossa possibile
                legal_moves[count].y = j;
                count++;
            }
        }
    }
    return count;
}

/** @brief Function that calculate all board positions occupied by a specific marker.(spaces that are not occupied by either player)
 * @param board is matrix of char representing the tris board
 * @param occupied_positions is a pointer of type Pair used to save all occupied positions
 * @param marker is a char representing the player marker
 * @return number of occupied positions*/
int get_occupied_positions(char board[BOARD_SIZE][BOARD_SIZE], char marker, struct Pair *occupied_positions)
{
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (marker == board[i][j])
            {
                occupied_positions[count].x = i;
                occupied_positions[count].y = j;
                count++;
            }
        }
    }
    return count;
}

/** @brief Check if the board is full (no more available moves).
 * @param board is matrix of char representing the tris board
 * @return true if the board is full false otherwise*/
bool board_is_full(char board[BOARD_SIZE][BOARD_SIZE])
{
    struct Pair legal_moves[MAX_MOVES];
    int count = get_legal_moves(board, legal_moves);

    if (count == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/** @brief Check if the game is won
 * @param occupied_positions is a pointer of type Pair which contains all pairs of occupied positions
 * @param occupied_positions_count is an integer indicating the number of occupied cells
 * @return true if the game is won false otherwise*/
bool game_is_won(struct Pair *occupied_positions, int occupied_positions_count)
{
    bool game_won;

    for (int i = 0; i < WINNING_STATES_SIZE; i++)
    {
        game_won = true;
        struct Pair *curr_win_state = winning_states[i];
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            bool found = false;
            for (int k = 0; k < occupied_positions_count; k++)
            {
                if (occupied_positions[k].x == curr_win_state[j].x &&
                    occupied_positions[k].y == curr_win_state[j].y)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                game_won = false;
                break;
            }
        }

        if (game_won)
        {
            break;
        }
    }
    return game_won;
}

/** @brief Given a marker returns the other player's marker
 * @param marker marker is a char representing the player marker
 * @return other player's marker*/
char get_opponent_marker(char marker)
{
    char opponent_marker;
    if (marker == PLAYER_MARKER)
    {
        opponent_marker = AI_MARKER;
    }
    else
    {
        opponent_marker = PLAYER_MARKER;
    }

    return opponent_marker;
}

/** @brief This function determines the state of a tic-tac-toe board for a given player marker.
* @param board is matrix of char representing the tris board .
* @param marker is a char representing the player marker.
* @return an integer representing the state of the board for the player marker: WIN if the player has won,
    LOSS if the opponent has won, DRAW if the board is full but no one has won.*/
int get_board_state(char board[BOARD_SIZE][BOARD_SIZE], char marker)
{
    char opponent_marker = get_opponent_marker(marker);

    struct Pair occupied_positions[MAX_MOVES];
    int occupied_positions_count = get_occupied_positions(board, marker, occupied_positions);

    bool is_won = game_is_won(occupied_positions, occupied_positions_count);

    if (is_won)
    {
        return WIN;
    }

    occupied_positions_count = get_occupied_positions(board, opponent_marker, occupied_positions);
    bool is_lost = game_is_won(occupied_positions, occupied_positions_count);

    if (is_lost)
    {
        return LOSS;
    }

    bool is_full = board_is_full(board);
    if (is_full)
    {
        return DRAW;
    }
    return DRAW;
}

/** @brief Function that use the alpha-beta pruning algorithm to calculate the best move for the AI player.
 * @param board is matrix of char representing the tris board.
 * @param marker is a char representing the player marker.
 * @param depth The current depth of the search in the game tree.
 * @param alpha The best score that the maximizing player (AI player) can guarantee at this point.
 * @param beta The best score that the minimizing player (Player) can guarantee at this point.
 * @return A struct Move representing the best move and its corresponding score.
 */
struct Move alpha_beta(char board[BOARD_SIZE][BOARD_SIZE], char marker, int depth, int alpha, int beta)
{
    // Initialize best move
    struct Pair best_move = {-1, -1};
    int best_score = (marker == AI_MARKER) ? LOSS : WIN;
    int i, score;
    struct Pair curr_move;
    struct Pair legal_moves[MAX_MOVES];
    int size = get_legal_moves(board, legal_moves);

    // If hit a terminal state(leaf node), return the best score and move
    if (board_is_full(board) || DRAW != get_board_state(board, AI_MARKER))
    {
        best_score = get_board_state(board, AI_MARKER);
        struct Move m = {best_score, best_move};
        return m;
    }

    for (i = 0; i < size; i++)
    {
        curr_move = legal_moves[i];
        board[curr_move.x][curr_move.y] = marker;
        // Maximizing player 's turn
        if (marker == AI_MARKER)
        {
            score = alpha_beta(board, PLAYER_MARKER, depth + 1, alpha, beta).score;
            // Get the best scoring move
            if (best_score < score)
            {
                best_score = score - depth * 10;
                best_move = curr_move;
                // Check if this branch' s best move is worse than the best
                // option of a previously search branch.If it is, skip it
                alpha = max(alpha, best_score);
                board[curr_move.x][curr_move.y] = EMPTY_SPACE;
                if (beta <= alpha)
                {
                    break;
                }
            }
        }
        // Minimizing opponent 's turn
        else
        {
            score = alpha_beta(board, AI_MARKER, depth + 1, alpha, beta).score;
            if (best_score > score)
            {
                best_score = score + depth * 10;
                best_move = curr_move;
                // Check if this branch' s best move is worse than the best
                // option of a previously search branch. If it is, skip it
                beta = min(beta, best_score);
                board[curr_move.x][curr_move.y] = EMPTY_SPACE;
                if (beta <= alpha)
                {
                    break;
                }
            }
        }
        board[curr_move.x][curr_move.y] = EMPTY_SPACE;
    }
    // Return the best move and score
    struct Move m = {best_score, best_move};
    return m;
}

/** @brief Function that use the alpha-beta pruning algorithm to calculate the best move for the AI
 * player by exploiting a branch parallelization using open MP.
 * @param board is matrix of char representing the tris board .
 * @param marker is a char representing the player marker.
 * @param depth The current depth of the search in the game tree.
 * @param alpha The best score that the maximizing player (AI player) can guarantee at this point.
 * @param beta The best score that the minimizing player (Player) can guarantee at this point.
 * @return a Pair representing the best move to make.
 */
struct Pair parallel_minimax_optimization(char (*board)[BOARD_SIZE], char marker, int depth, int alpha, int beta)
{
    // Initialize best move
    struct Pair best_move = {-1, -1};
    int best_score = (marker == AI_MARKER) ? LOSS : WIN;
    struct Move m;
    int i, score;
    struct Pair curr_move;
    struct Pair legal_moves[9];
    int size = get_legal_moves(board, legal_moves);

    // If hit a terminal state (leaf node), return the best score and move
    if (board_is_full(board) || DRAW != get_board_state(board, AI_MARKER))
    {
        best_score = get_board_state(board, AI_MARKER);
        struct Move m = {best_score, best_move};
        return m.p;
    }

    #pragma omp parallel for private(curr_move, m, score) shared(best_score, best_move)
    for (i = 0; i < size; i++)
    {
        char board_copy[BOARD_SIZE][BOARD_SIZE];
        memcpy(board_copy, board, sizeof(board)); // Create a copy of the board for each thread
        curr_move = legal_moves[i];
        board_copy[curr_move.x][curr_move.y] = marker;

        if (marker == AI_MARKER)
        {
            score = alpha_beta(board_copy, PLAYER_MARKER, depth, alpha, beta).score;
            // Management of access to shared variables
            #pragma omp critical
            {
                if (best_score < score)
                {
                    best_move = curr_move;
                    best_score = score;
                }
            }
        }
        else
        {
            score = alpha_beta(board_copy, AI_MARKER, depth, alpha, beta).score;
            // Management of access to shared variables
            #pragma omp critical
            {
                if (best_score > score)
                {
                    best_move = curr_move;
                    best_score = score;
                }
            }
        }
    }
    // Return the best move
    return best_move;
}

/** @brief Function that use the alpha-beta pruning algorithm to calculate the best move for the AI.
 * @param board is matrix of char representing the tris board .
 * @param marker is a char representing the player marker.
 * @param depth The current depth of the search in the game tree.
 * @param alpha The best score that the maximizing player (AI player) can guarantee at this point.
 * @param beta The best score that the minimizing player (Player) can guarantee at this point.
 * @return a Pair representing the best move to make.
 */
struct Pair minimax_optimization(char (*board)[BOARD_SIZE], char marker, int depth, int alpha, int beta)
{
    // Initialize best move
    struct Pair best_move = {-1, -1};
    int best_score = (marker == AI_MARKER) ? LOSS : WIN;
    struct Move m;
    int i, score;
    struct Pair curr_move;
    struct Pair legal_moves[9];
    int size = get_legal_moves(board, legal_moves);

    // If we hit a terminal state (leaf node), return the best score and move
    if (board_is_full(board) || DRAW != get_board_state(board, AI_MARKER))
    {
        best_score = get_board_state(board, AI_MARKER);
        struct Move m = {best_score, best_move};
        return m.p;
    }

    for (i = 0; i < size; i++)
    {

        curr_move = legal_moves[i];
        board[curr_move.x][curr_move.y] = marker;

        if (marker == AI_MARKER)
        {
            score = alpha_beta(board, PLAYER_MARKER, depth, alpha, beta).score;
            if (best_score < score)
            {
                best_move = curr_move;
                best_score = score;
            }
        }
        else
        {
            score = alpha_beta(board, AI_MARKER, depth, alpha, beta).score;
            if (best_score > score)
            {
                best_move = curr_move;
                best_score = score;
            }
        }
        board[curr_move.x][curr_move.y] = EMPTY_SPACE;
    }
    // Return the best move and score
    return best_move;
}

/** @brief Function that checks if the game is over.
 * @param board is matrix of char representing the tris board .
 * @return true if the game is over, false otherwise.*/
bool game_is_done(char board[BOARD_SIZE][BOARD_SIZE])
{
    if (board_is_full(board))
    {
        return true;
    }
    if (DRAW != get_board_state(board, AI_MARKER))
    {
        return true;
    }

    return false;
}

int main(int argc, char**argv)
{
    clock_t start_time, end_time;
    double total_time;
    char board[BOARD_SIZE][BOARD_SIZE] = {{EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE}, // Initialization of the empty board
                                          {EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE},
                                          {EMPTY_SPACE, EMPTY_SPACE, EMPTY_SPACE}};
    /*Set of moves made by the player. These moves were obtained by playing
    two AI that used the same alpha beta pruning algorithm. In fact,
    they always lead to a draw as the decision criterion is the same.*/
    struct Pair playerMoves[5] = {{0, 0}, {0, 1}, {2, 0}, {1, 2}, {2, 2}};
    int i = 0;

    // Calculate the start time
    start_time = clock();

    // Main loop of the game
    while (!game_is_done(board))
    {
        struct Pair p = playerMoves[i];
        board[p.x][p.y] = PLAYER_MARKER;
        i += 1;
    // print_board(board); Check board update after Player move
    #ifdef _OPENMP
        struct Pair a = parallel_minimax_optimization(board, AI_MARKER, START_DEPTH, INT_MIN, INT_MAX); // Running parallel version
    #else
        struct Pair a = minimax_optimization(board, AI_MARKER, START_DEPTH, INT_MIN, INT_MAX); // Running sequential version
    #endif
        board[a.x][a.y] = AI_MARKER;
        //print_board(board); //Check board update after AI move
    }
    // int state = get_board_state(board, PLAYER_MARKER);
    // print_game_state(state); Printing of the game result

    // Calculate finish time and total time
    end_time = clock();
    total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    /*File management section*/
    FILE *fp;
    char path[200];
    sprintf(path, "Informations/opt%d/ResultsThreads.csv", atoi(argv[1]));
    char *filename = path;
    fp = fopen(filename, "a+");
    if (fp == NULL)
    {
        perror("Errore durante l'apertura del file");
    }
    #ifdef _OPENMP
    fprintf(fp, "OpenMP; %d; %.6f;\n", atoi(argv[2]), total_time);
    #else
    fprintf(fp, "Sequential; 0; %.6f;\n", total_time);
    #endif
    fclose(fp); 

    return 0;
}