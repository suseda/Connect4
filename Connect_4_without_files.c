#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define ROWS 4
#define COLS 4

enum bool{ False, True };

enum winner{ O = -1, Tie = 0, X = 1, PLAY = 2};

int max(int a, int b)
{
    return a>b ? a : b;
}

int min(int a, int b)
{
    return a<b ? a : b;
}

void print_board(char board[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("| %c ", board[i][j]);
        }
        puts("|");
    }
}

enum bool is_valid_move(int position, char board[ROWS][COLS])
{
    for (int row = ROWS - 1; row >= 0; row--)
    {
        if (board[row][position] == ' ')
        {
            return True;
        }
    }
    return False;
}


enum bool check_vertical(char board[ROWS][COLS])
{
    for (int row = 0; row < ROWS - 3; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (board[row][col] == board[row + 1][col] && board[row][col] == board[row + 2][col] && board[row][col] == board[row + 3][col] && board[row][col] != ' ')
                return True;
        }
    }
    return False;
}

enum bool check_horizontal(char board[ROWS][COLS])
{
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS - 3; col++)
        {
            if (board[row][col] == board[row][col + 1] && board[row][col] == board[row][col + 2] && board[row][col] == board[row][col + 3] && board[row][col] != ' ')
                return True;
        }
    }
    return False;
}

enum bool check_left_diagonal(char board[ROWS][COLS])
{
    int index = ROWS - 1;
    for (int row = ROWS; 0 <= row - 3; row--)
    {
        for (int col = COLS; 0 <= col - 3; col--)
        {
            if (board[index][col] == board[index - 1][col - 1] && board[index][col] == board[index - 2][col - 2] && board[index][col] == board[index - 3][col - 3] && board[index][col] != ' ')
                return True;
        }
        index--;
    }
    return False;
}

enum bool check_right_diagonal(char board[ROWS][COLS])
{
    int index = ROWS - 1;
    for (int row = ROWS - 1; 0 <= row - 3; row--)
    {
        for (int col = 0; col < COLS - 3; col++)
        {
            if (board[index][col] == board[index - 1][col + 1] && board[index][col] == board[index - 2][col + 2] && board[index][col] == board[index - 3][col + 3] && board[index][col] != ' ')
                return True;
        }
        index--;
    }
    return False;
}


enum bool check_win(char board[ROWS][COLS])
{
    return (check_horizontal(board) || check_vertical(board) || check_right_diagonal(board) || check_left_diagonal(board));
}

enum winner game_state(char board[ROWS][COLS], int player)
{
    if(check_win(board))
    {
        if(player == 0)
            return O;
        else
            return X;
    }
    else
    {
        for (int j = 0; j < COLS; j++)
        {
            for (int i = 0; i < ROWS; i++)
            {
                if (board[i][j] == ' ')
                {
                    return PLAY;
                }
            }
        }
        return Tie;
    }
    
}

int evaluate_move(char board[ROWS][COLS], int depth, int player)
{
    enum winner gs = game_state(board,player);
    if (gs == O)
        return 10;
    else if (gs == X)
        return -10;
    else if (gs == Tie)
        return 0;
    if(depth == 0)
        return 0;
    
    
    if (player) 
    {
        int val = -100;
        
        for (int j = 0; j < COLS; j++)
        {
            for (int i = ROWS - 1; i >= 0; i--)
            {
                if (board[i][j] == ' ')
                {
                    board[i][j] = 'O';
                    val = max(evaluate_move(board, depth - 1, 0), val);
                    board[i][j] = ' ';
                    break;
                }
            }
        }
        return val;
    }
    else 
    {
        int val = 100;
        for (int j = 0; j < COLS; j++)
        {
            for (int i = ROWS - 1; i >= 0; i--)
            {
                if (board[i][j] == ' ')
                {
                    board[i][j] = 'X';
                    val = min(evaluate_move(board, depth - 1, 1),val);
                    board[i][j] = ' ';
                    break;
                }
            }
        }
        return val;
    }
}

int find_best_move(char board[ROWS][COLS], int depth)
{
    int best_move = -1, best_score = -100;
    for (int j = 0; j < COLS; j++)
    {
        for (int i = ROWS - 1; i >= 0; i--)
        {
            if (board[i][j] == ' ')
            {
                board[i][j] = 'O';
                int score = evaluate_move(board, depth, 0);
                board[i][j] = ' ';
                if (best_score < score)
                {
                    best_score = score;
                    best_move = j;
                }  
                break;
            }
        }
    }

    return best_move;
}


int main()
{
    char board[ROWS][COLS];
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = ' ';
        }
    }

    int mode = 0, turns = 0, flag = 1;
    if (mode)
    {
        while (ROWS * COLS > turns && !(check_win(board)))
        {
            if (flag)
            {
                print_board(board);
                int position = 0;
                do
                {
                    puts("Enter number of coloum you want to play:");
                    scanf("%d", &position);

                    if (!(position > 0 && position < COLS + 1) || !(is_valid_move(position - 1, board)))
                    {
                        puts("Move is not valid!");
                    }
                } while (!(position > 0 && position < COLS + 1) || !(is_valid_move(position - 1, board)));

                position = position - 1;

                for (int row = ROWS - 1; row >= 0; row--)
                {
                    if (board[row][position] == ' ')
                    {
                        board[row][position] = 'X';
                        break;
                    }
                }
                turns++;
                flag = 0;
            }
            else
            {
                print_board(board);
                int position = 0;
                do
                {
                    puts("Enter number of coloum you want to play:");
                    scanf("%d", &position);

                    if (!(position > 0 && position < COLS + 1) || !(is_valid_move(position - 1, board)))
                    {
                        puts("Move is not valid!");
                    }
                } while (!(position > 0 && position < COLS + 1) || !(is_valid_move(position - 1, board)));

                position = position - 1;

                for (int row = ROWS - 1; row >= 0; row--)
                {
                    if (board[row][position] == ' ')
                    {
                        board[row][position] = 'O';
                        break;
                    }
                }
                turns++;
                flag = 1;
            }
        }

        print_board(board);

        if (turns == ROWS * COLS && !(check_win(board)))
        {
            puts("Tie!");
        }
        else
        {

            if (flag)
                printf("Player O wins in %d turns!", turns);
            else
                printf("Player X wins in %d turns!", turns);
        }
    }
    else
    {
        while (turns < ROWS * COLS && !(check_win(board)))
        {
            if (flag)
            {
                print_board(board);
                int position = 0;
                do
                {
                    puts("Enter number of coloum you want to play:");
                    scanf("%d", &position);

                    if (!(position > 0 && position < COLS + 1) || !(is_valid_move(position - 1, board)))
                    {
                        puts("Move is not valid!");
                    }
                } while (!(position > 0 && position < COLS + 1) || !(is_valid_move(position - 1, board)));
                position = position - 1;

                for (int row = ROWS - 1; row >= 0; row--)
                {
                    if (board[row][position] == ' ')
                    {
                        board[row][position] = 'X';
                        break;
                    }
                }
                turns++;
                flag = 0;
            }
            else
            {
                int position = 0;
                if (turns < 2)
                {
                    srand(time(NULL));
                    position = rand() % COLS;
                }
                else
                {
                    position = find_best_move(board, 3);
                }

                for (int row = ROWS - 1; row >= 0; row--)
                {
                    if (board[row][position] == ' ')
                    {
                        board[row][position] = 'O';
                        break;
                    }
                }
                turns++;
                flag = 1;
            }
        }
        print_board(board);

        if (turns == ROWS * COLS && !(check_win(board)))
        {
            puts("Tie!");
        }
        else
        {

            if (flag)
                printf("Computer wins in %d turns!", turns);
            else
                printf("Player wins in %d turns!", turns);
        }
    }

    return 0;
}
