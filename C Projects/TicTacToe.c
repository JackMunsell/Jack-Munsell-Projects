#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>


void display_board(char board[3][3]){
    for(int i =0;i<3;i++){
        for(int j=0;j<3;j++){
            printf("%c ",board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void AI_turn(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        //check rows
        if (board[i][0] == 'X' && board[i][1] == 'X' && board[i][2] == 'E') {
            board[i][2] = 'O';  // AI wins
            return;
        } else if (board[i][0] == 'X' && board[i][2] == 'X' && board[i][1] == 'E') {
            board[i][1] = 'O';
            return;
        } else if (board[i][1] == 'X' && board[i][2] == 'X' && board[i][0] == 'E') {
            board[i][0] = 'O';
            return;
        }

        //check columns
        if (board[0][i] == 'X' && board[1][i] == 'X' && board[2][i] == 'E') {
            board[2][i] = 'O';
            return;
        } else if (board[0][i] == 'X' && board[2][i] == 'X' && board[1][i] == 'E') {
            board[1][i] = 'O';
            return;
        } else if (board[1][i] == 'X' && board[2][i] == 'X' && board[0][i] == 'E') {
            board[0][i] = 'O';
            return;
        }
    }

    //check diagonals
    if (board[0][0] == 'X' && board[1][1] == 'X' && board[2][2] == 'E') {
        board[2][2] = 'O';
        return;
    } else if (board[0][0] == 'X' && board[2][2] == 'X' && board[1][1] == 'E') {
        board[1][1] = 'O';
        return;
    } else if (board[1][1] == 'X' && board[2][2] == 'X' && board[0][0] == 'E') {
        board[0][0] = 'O';
        return;
    }

    if (board[0][2] == 'X' && board[1][1] == 'X' && board[2][0] == 'E') {
        board[2][0] = 'O';
        return;
    } else if (board[0][2] == 'X' && board[2][0] == 'X' && board[1][1] == 'E') {
        board[1][1] = 'O';
        return;
    } else if (board[1][1] == 'X' && board[2][0] == 'X' && board[0][2] == 'E') {
        board[0][2] = 'O';
        return;
    }

    int i, j;
    do {
        i = rand() % 3;
        j = rand() % 3;
    } while (board[i][j] != 'E');
    board[i][j] = 'O'; 

    printf("AI takes spot %d, %d \n", i + 1, j + 1);
}

bool check_win(char board[3][3]){
    for(int i = 0;i<3;i++){
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != 'E') {
            return true;
        }
    }

    for (int j = 0; j < 3; j++) {
        if (board[0][j] == board[1][j] && board[1][j] == board[2][j] && board[0][j] != 'E') {
            return true;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != 'E') {
        return true;
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != 'E') {
        return true;
    }
    return false;
}

void move(char board[3][3],int turn){
    int i, j;
    char mark = (turn % 2 == 0) ? 'X' : 'O';

    printf("Enter the spot you want, row, column Ex. 1 3\n");
    scanf("%d %d",&i, &j);

    j--;
    i--;

    if(board[i][j]=='E'){
        board[i][j] = mark;
    }else{
        printf("Spot is already used try again\n");
        display_board(board);
        move(board, turn);
    }
}

int main(){
    int turn = 0;
    char board[3][3] = {
        {'E','E','E'},
        {'E','E','E'},
        {'E','E','E'}
    };
    
    while(turn<9){
        display_board(board);
        if(turn % 2 == 0){
            printf("Player X turn\n");
            move(board,turn);
        }else{
            printf("AI (Player O) turn\n");
            AI_turn(board);
        }
        
        
        

        if(check_win(board)==true){
            if(turn%2==0){
                printf("The winner is Player X\n");
            }else{
                printf("The winner is Player O (AI)\n");
            }
            break;
        }
        turn++;
    }
    display_board(board);
    if(turn == 9 && !check_win(board)){
        printf("The game is a Draw! \n");
    }
}
