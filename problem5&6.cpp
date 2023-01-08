
//
// Created by Yousef Abdalla  on 12/22/2022.
//
#include <bits/stdc++.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
using namespace std;


class  board {
    int sz;
public:
    // save last move for backtrack
    vector<pair<int, int>> moves;
    vector<vector<char>> G;
    // create board
    board(int n) {
        sz = n;
        G.resize(n, vector<char>(n, '.'));
        for (int i = 1; i < n - 1; i++)G[0][i] = 'D';
        for (int i = 1; i < n - 1; i++)G[i][0] = 'L';
    }

    // print grid
    void show() {

        for (int i = 0; i < sz; i++) {
            cout << "| ";
            for (int j = 0; j < sz; j++)cout << i << " " << j << " (" << G[i][j] << ")   ";
            cout << "| \n";
        }
        cout << '\n' << '\n';
    }
    // check turn
    bool if_his_turn(int i, int j, int p,vector<vector<char>>&g) {
        if (g[i][j] == 'D' && p == 0)return true;
        if (g[i][j] == 'L' && p)return true;
        return false;
    }
    //return 1,2 for d and 3,4 for L
    int can_move(int i, int j, int p,vector<vector<char>>&g) {

        if (!if_his_turn(i, j, p, g))return 0;
        // next jump
        if (g[i][j] == 'D' && p == 0) {
            if (i + 1 < sz)
                if (g[i + 1][j] == '.')return 1;
            // double jump
            if (i + 2 < sz) {
                if (g[i + 1][j] == 'L' && g[i + 2][j] == '.')return 2;
            }
        }

        if (g[i][j] == 'L' && p == 1) {
            // next jump
            if (j + 1 < sz)
                if (g[i][j + 1] == '.')return 3;
            // double jump
            if (j + 2 < sz)
                if (g[i][j + 1] == 'D' && g[i][j + 2] == '.')return 4;
        }
        //can't move
        return 0;
    }


    int  move(int i, int j, int p,vector<vector<char>>&g) {
        // move and save last move
        int now = can_move(i, j, p,g);
        if (now == 1) {
            g[i][j] = '.';
            g[i + 1][j] = 'D';
            moves.push_back({i + 1, j});
            return 1;
        } else if (now == 2) {
            g[i][j] = '.';
            g[i + 2][j] = 'D';
            moves.push_back({i + 2, j});
            return 1;
        } else if (now == 3) {
            G[i][j] = '.';
            G[i][j + 1] = 'L';
            moves.push_back({i, j + 1});
            return 2;
        } else if (now == 4) {
            g[i][j] = '.';
            g[i][j + 2] = 'L';
            moves.push_back({i, j + 2});
            return 2;
        } else {
            //push non exist number
            moves.push_back({10000,10000});
            return 0;
        }
    }

    // if last row or last col ==rockets there is winner here
    int check_win(vector<vector<char>>g) {
        int p1 = 0, p2 = 0;
        for (int i = 1; i < sz; i++)p1 += (g[sz - 1][i] == 'D');
        for (int i = 1; i < sz; i++)p2 += (g[i][sz - 1] == 'L');
        if (p1 == sz - 2) {
            return 2;
        }
        if (p2 == sz - 2) {
            return 1;
        }
        return 0;
    }
    // AI backtracking algorithm
        int minimax(vector<vector<char>>&g,int depth, bool ismax,bool firsttime=true) {
            int result = check_win(g);
            if (result != 0 || depth == 0) { return result; }
            if (ismax) {
                    // get possible moves for player
                int final_score = -100000;
                for (int i = 0; i < sz; i++) {
                    for (int j = 0; j < sz; j++) {
                        if (g[i][j] == 'D') {
                            if (can_move(i, j, 0,g)) {
                                move(i, j, 0,g);
                                int score = minimax(g,depth - 1, false, false);
                                g[i][j] = 'D';
                                g[moves.back().first][moves.back().second]='.';
                                moves.pop_back();
                                final_score = max(final_score, score);
                            }
                        }
                    }
                    return final_score;
                }
            } else {
                // // get possible moves for player
                int finali=0, finalj=0;
                int final_score = 100000;
                for (int i = 0; i < sz; i++){
                    for (int j = 0; j < sz; j++) {
                        if (g[i][j] == 'L') {
                            if (can_move(i, j, 1,g)) {
                                move(i, j, 1,g);
                                int score = minimax(g,depth - 1, true, false);
                                g[i][j] = 'L';
                                g[moves.back().first][moves.back().second]='.';
                                moves.pop_back();
                                if (score < final_score) {
                                    final_score = score;
                                    finali = i;
                                    finalj = j;
                                }
                            }
                        }
                    }
                }
                    if (firsttime){
                        move(finali, finalj, 1,G);
                }
                else
                    return final_score;

            }
        }
};


class player {
    int score = 0;
public:
    player() {
        score = 0;
    }

    void increase() {
        score++;
    }

    int points() {
        return score;
    }
};


int main() {
    int way;
    //player p1,p2;
    player p1, p2;
    while (true) {
        system("CLS");
        cout << "1-play with friend \n2-play with AI\n";
        cin >> way;
        if (way != 1 && way != 2)break;
        cout << "player 1 score: " << p1.points() << "   " << "player 2 score: " << p2.points() << endl;
        cout << "Rockets D for player 1 and Rockets L for player 2\n";
        int boardsize;
        cout << "Choose board size(must be more than 2), if you want to end game set size 0\n";
        cin >> boardsize;
        if (boardsize == 0) { break; }
        else if (boardsize <= 2) {
            cout << "choose bigger size\n";
            sleep(2);
            continue;
        }
        int x, y;
        int turn = 0;
        board b(boardsize);
        if (way == 1) {
            while (true) {
                int end = b.check_win(b.G);
                b.show();
                if (!end) {
                    if (!turn) {
                        cout << "player 1 turn choose one of D rockets to move  x  y : ";
                        cin >> x >> y;
                        if (!b.if_his_turn(x, y, turn, b.G)) {
                            cout << "Wrong choice !!!! Your turn is done, choose a valid rocket next time\n";
                            turn = 1 - turn;
                            sleep(3);
                            system("CLS");
                            continue;
                        }
                        b.move(x, y, turn, b.G);
                        turn = 1 - turn;
                        system("CLS");

                    } else {
                        cout << "player 2 turn choose one of L rockets to move  x  y : ";
                        cin >> x >> y;
                        if (!b.if_his_turn(x, y, turn, b.G)) {
                            cout << "Wrong choice !!!! Your turn is done, choose a valid rocket next time\n";
                            turn = 1 - turn;
                            sleep(3);
                            system("CLS");
                            continue;
                        }
                        b.move(x, y, turn, b.G);
                        turn = 1 - turn;
                        system("CLS");
                    }
                } else if (end == 2) {
                    cout << "player 1 won \n";
                    p1.increase();
                    cout << endl;
                    sleep(3);
                    break;
                } else {
                    cout << "player 2 won \n";
                    p2.increase();
                    cout << endl;
                    sleep(3);
                    break;
                }
            }

        } else {
            while (b.check_win(b.G) == 0) {
                if (!turn) {
                    b.show();
                    cout << "player 1 turn choose one of D rockets to move  x  y : ";
                    cin >> x >> y;
                    b.move(x, y, 0, b.G);
                    b.moves.pop_back();
                    turn = 1;
                } else {
                    cout<<"AI turn\n";
                    b.minimax(b.G, 100, 0, true);
                    b.show();
                    turn = 0;
                }
                system("CLS");

            }
            b.show();
            if (turn == 1) {
                cout << "player 1 won \n";
                p1.increase();
                cout << endl;
                sleep(3);
            } else {
                cout << "AI won \n";
                p2.increase();
                cout << endl;
                sleep(3);
            }
        }
        cout << "Great game, Congratulations to the winner\n";
    }
}
