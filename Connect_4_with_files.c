#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define ROWS 6
#define COLS 7

enum bool{ False, True };
enum winner { O = -1, Tie = 0, X = 1, PLAY = 2 };

int maximum(int a, int b)
{
    return a > b ? a : b;
}

int minimum(int a, int b)
{
    return a < b ? a : b;
}


void write_board(FILE* fp, char board[ROWS][COLS], char *str)
{
    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLS; j++) 
        {
            sprintf(str, "| %c ", board[i][j]);
        }
        strcat(str, "|");
        fprintf(fp, "%s\n", str);
        memset(str,0,strlen(str));
    }
}

void print_board(char board[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLS; j++) 
        {
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
    if (check_win(board))
    {
        if (player == 0)
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
    enum winner gs = game_state(board, player);
    if (gs == O)
        return 10;
    else if (gs == X)
        return -10;
    else if (gs == Tie)
        return 0;
    if (depth == 0)
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
                    val = maximum(evaluate_move(board, depth - 1, 0), val);
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
                    val = minimum(evaluate_move(board, depth - 1, 1), val);
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



int main(int argc, char** argv)
{
    /*
    do
    {
        puts("Enter number of rows:");
        scanf("%d", &rows);
        puts("Enter number of coloums:");
        scanf("%d", &cols);
    } while (rows < 3 || rows > 20 || cols < 3 || cols > 40);
    */

    FILE* fptr;
    fptr = open(argv[1], "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    char* str = (char*)calloc(255, sizeof(char*));
    int mode;

    printf("Enter mode (1 - two players, 0 - singleplayer):");
    scanf("%d", &mode);
    
    sprintf(str, "%d %d %d", ROWS, COLS, mode);
    fprintf(fptr, str);
    memset(str, 0, strlen(str));

    char board[ROWS][COLS];
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = ' ';
        }
    }

    int turns = 0, flag = 1;
    if (mode)
    {
        while (ROWS * COLS > turns && !(check_win(board)))
        {
            if (flag)
            {
                print_board(board);
                write_board(fptr, board, str);
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
                fprintf(fptr, "%d\n", position);
                turns++;
                flag = 0;
            }
            else
            {
                print_board(board);
                write_board(fptr, board, str);
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
                fprintf(fptr, "%d\n", position);
                turns++;
                flag = 1;
            }
        }

        print_board(board);
        write_board(fptr, board, str);

        if (turns == ROWS * COLS && !(check_win(board)))
        {
            puts("Tie!");
            fprintf(fptr, "Tie!\n");
        }
        else
        {

            if (flag)
            {
                printf("Player O wins in %d turns!", turns);
                fprintf(fptr, "Player O wins in %d turns!\n", turns);
            }
            else
            {
                printf("Player X wins in %d turns!", turns);
                fprintf(fptr, "Player X wins in %d turns!\n", turns);
            }
        }
    }
    else
    {
        while (turns < ROWS * COLS && !(check_win(board)))
        {
            if (flag)
            {
                print_board(board);
                write_board(fptr, board, str);
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
                fprintf(fptr, "%d\n", position);
                turns++;
                flag = 0;
            }
            else
            {
                print_board(board);
                write_board(fptr, board, str);
                int position = 0;
                if (turns < 2)
                {
                    srand(time(NULL));
                    position = rand() % COLS;
                }
                else
                {
                    position = find_best_move(board,9);
                }

                for (int row = ROWS - 1; row >= 0; row--)
                {
                    if (board[row][position] == ' ')
                    {
                        board[row][position] = 'O';
                        break;
                    }
                }
                fprintf(fptr, "%d\n", position);
                turns++;
                flag = 1;
            }
        }

        print_board(board);
        write_board(fptr, board, str);

        if (turns == ROWS * COLS && !(check_win(board)))
        {
            puts("Tie!");
            fprintf(fptr, "Tie!\n");
        }
        else
        {

            if (flag)
            {
                printf("Computer wins in %d turns!", turns);
                fprintf(fptr, "Computer wins in %d turns!\n", turns);
            }
            else
            {
                printf("Player X wins in %d turns!", turns);
                fprintf(fptr, "Player X wins in %d turns!\n", turns);
            }
        }
    }

    fclose(fptr);

    return 0;
}
