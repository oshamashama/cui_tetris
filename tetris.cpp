#include <bits/stdc++.h>
#include <unistd.h>
#include <random>
#include <iostream>
#include <future>
#include <thread>
#include <utility>
using namespace std;


struct tetris
{
    int H, W;
    int score;
    string BLOCK, EMPTY;
    vector<vector<bool> > m;
    vector<vector<bool> > fall_piece;
    vector<vector<bool> > fall_piece_reset;
    vector<vector<vector<bool> > > piece_list;
    bool falling_piece;
    tetris(int H_, int W_, string BLOCK_, string EMPTY_) : H(H_/2*2), W(W_/2*2), BLOCK(BLOCK_), EMPTY(EMPTY_) {
        vector<bool> r_m(W, false);
        m.resize(H, r_m);
        fall_piece.resize(H, r_m);
        fall_piece_reset.resize(H, r_m);
        vector<vector<bool> > pt(2,vector<bool>(4,false));
        piece_list.resize(7, pt);
        set_piece_data();
        falling_piece = true;
        score = 0;
    }

    void set_piece_data() {
                piece_list.at(0).at(1).at(0) = true;
                piece_list.at(0).at(1).at(1) = true;
                piece_list.at(0).at(1).at(2) = true;
                piece_list.at(0).at(1).at(3) = true;
                piece_list.at(1).at(1).at(0) = true;
                piece_list.at(1).at(1).at(1) = true;
                piece_list.at(1).at(0).at(1) = true;
                piece_list.at(1).at(0).at(2) = true;
                piece_list.at(2).at(0).at(0) = true;
                piece_list.at(2).at(0).at(1) = true;
                piece_list.at(2).at(1).at(1) = true;
                piece_list.at(2).at(1).at(2) = true;
                piece_list.at(3).at(1).at(0) = true;
                piece_list.at(3).at(1).at(1) = true;
                piece_list.at(3).at(0).at(0) = true;
                piece_list.at(3).at(0).at(1) = true;
                piece_list.at(4).at(1).at(0) = true;
                piece_list.at(4).at(1).at(1) = true;
                piece_list.at(4).at(1).at(2) = true;
                piece_list.at(4).at(0).at(2) = true;
                piece_list.at(5).at(0).at(0) = true;
                piece_list.at(5).at(0).at(1) = true;
                piece_list.at(5).at(0).at(2) = true;
                piece_list.at(5).at(1).at(2) = true;
                piece_list.at(6).at(1).at(0) = true;
                piece_list.at(6).at(1).at(1) = true;
                piece_list.at(6).at(1).at(2) = true;
                piece_list.at(6).at(0).at(1) = true;
    }

    bool update() {
        string S;
        cin >> S;
        //a right
        //d left
        //s down
        //r rotate 
        for(int II = 0; II < S.size(); II++) {
            if(S.at(II) == 'a' && check_left()) {
                for(int i = 0; i < H; i++) {
                    for(int j = 0; j < W - 1; j++) {
                        fall_piece.at(i).at(j) = fall_piece.at(i).at(j + 1);
                    }
                    fall_piece.at(i).at(W - 1) = 0;
                }
            }
            if(S.at(II) == 's' && check_fall()) {
                for(int i = H - 1; i > 0; i--) {
                    for(int j = 0; j < W; j++) {
                        fall_piece.at(i).at(j) = fall_piece.at(i - 1).at(j);
                    }
                }
            }
            if(S.at(II) == 'S' && check_fall()) {
                for(int k = 0; k < 2*W; k++) {
                    if(check_fall()) {
                        for(int i = H - 1; i > 0; i--) {
                            for(int j = 0; j < W; j++) {
                                fall_piece.at(i).at(j) = fall_piece.at(i - 1).at(j);
                            }
                        }
                    }
                }
            }
            if(S.at(II) == 'd' && check_right()) {
                for(int i = 0; i < H; i++) {
                    for(int j = W - 1; j > 0; j--) {
                        fall_piece.at(i).at(j) = fall_piece.at(i).at(j - 1);
                    }
                    fall_piece.at(i).at(0) = 0;
                }
            }
            if(S.at(II) == 'r' && check_rotate()) {
                rotate();
            }
        }
        if(check_fall()) {
            for(int i = H - 2; i >= 0; i--) {
                for(int j = 0; j < W; j++) {
                    fall_piece.at(i + 1).at(j) = fall_piece.at(i).at(j);
                }
            }
        } else {
            for(int i = 0; i < H; i++) {
                for(int j = 0; j < W; j++) {
                    m.at(i).at(j) = fall_piece.at(i).at(j) || m.at(i).at(j);
                }
            }
            new_piece();
            if(check_fall()) {
                for(int i = H - 2; i >= 0; i--) {
                    for(int j = 0; j < W; j++) {
                        fall_piece.at(i + 1).at(j) = fall_piece.at(i).at(j);
                    }
                }
            } else {
                for(int i = 0; i < 20; i++) cout << "             やーい！w GAME  OVER やーい！w             " << endl;
                return true;
            }
        }
        int t_score = 0;
        for(int i = 0; i < H; i++) {
            bool c = 1;
            for(int j = 0; j < W; j++) {
                c = m.at(i).at(j) && c;
            }
            if(c) {
                for(int h = i; h > 0; h--) {
                    m.at(h) = m.at(h - 1);
                }
                for(int w = 0; w < W; w++) {
                    m.at(0).at(w) = 0;
                }
                t_score++;
                i = 0;
            }
        }
        score += 432432*t_score;
        return false;
    }

    bool check_fall() {
        for(int i = 0; i < H; i++) {
            for(int j = 0; j < W; j++) {
                if(fall_piece.at(i).at(j) && (i == H - 1 || m.at(i + 1).at(j))) return false;
            }
        }
        return true;
    }
    bool check_rotate() {
        for(int i_m = 0; i_m < H; i_m++) {
            for(int j_m = 0; j_m < W; j_m++) {
                if(fall_piece.at(i_m).at(j_m)) {
                    i_m -= 2;
                    j_m -= 2;
                    vector<vector<bool> > memo(8,vector<bool>(8,0));
                    vector<vector<bool> > r_memo(8,vector<bool>(8,0));
                    for(int i = 0; i < 8; i++) {
                        for(int j = 0; j < 8; j++) {
                            if(0 <= i_m + i && i_m + i < H && 0 <= j_m + j && j_m + j < W) memo.at(i).at(j) = fall_piece.at(i_m + i).at(j_m + j);
                        }
                    }
                    for(int i = 0; i < 8; i++) {
                        for(int j = 0; j < 8; j++) {
                            r_memo.at(8 - 1 - i).at(j) = memo.at(j).at(i);
                        }
                    }
                    vector<vector<bool> > tmp_map(H,vector<bool>(W,0));
                    for(int i = 0; i < 8; i++) {
                        for(int j = 0; j < 8; j++) {
                            if(0 <= i_m + i && i_m + i < H && 0 <= j_m + j && j_m + j < W) tmp_map.at(i_m + i).at(j_m + j) = r_memo.at(i).at(j);
                            else return false;
                        }
                    }
                    for(int i = 0; i < H; i++) {
                        for(int j = 0; j < W; j++) {
                            if(tmp_map.at(i).at(j) && m.at(i).at(j)) return false;
                        }
                    }
                    return true;
                }
            }
        }
        return true;
    }
    void rotate() {
        for(int i_m = 0; i_m < H; i_m++) {
            for(int j_m = 0; j_m < W; j_m++) {
                if(fall_piece.at(i_m).at(j_m)) {
                    i_m -= 2;
                    j_m -= 2;
                    vector<vector<bool> > memo(8,vector<bool>(8,0));
                    vector<vector<bool> > r_memo(8,vector<bool>(8,0));
                    for(int i = 0; i < 8; i++) {
                        for(int j = 0; j < 8; j++) {
                            if(0 <= i_m + i && i_m + i < H && 0 <= j_m + j && j_m + j < W) memo.at(i).at(j) = fall_piece.at(i_m + i).at(j_m + j);
                        }
                    }
                    for(int i = 0; i < 8; i++) {
                        for(int j = 0; j < 8; j++) {
                            r_memo.at(8 - 1 - i).at(j) = memo.at(j).at(i);
                        }
                    }
                    fall_piece = fall_piece_reset;
                    for(int i = 0; i < 8; i++) {
                        for(int j = 0; j < 8; j++) {
                            fall_piece.at(i_m + i).at(j_m + j) = r_memo.at(i).at(j);
                        }
                    }
                    return;
                }
            }
        }
        return;
    }
    
    bool check_right() {
        for(int i = 0; i < H; i++) {
            for(int j = 0; j < W; j++) {
                if(fall_piece.at(i).at(j) && (j == W - 1 || m.at(i).at(j + 1))) return false;
            }
        }
        return true;
    }

    bool check_left() {
        for(int i = 0; i < H; i++) {
            for(int j = 0; j < W; j++) {
                if(fall_piece.at(i).at(j) && (j == 0 || m.at(i).at(j - 1))) return false;
            }
        }
        return true;
    }

    void new_piece() {
        random_device rnd;
        int piece_number = rnd()%7;
        fall_piece = fall_piece_reset;
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 4; j++) {
                fall_piece.at(i + 1).at(W/2 - 2 + j) = piece_list.at(piece_number).at(i).at(j);
            }
        }
    }

    void print() {
        system("clear");
        for(int i = 0; i < H; i++) {
            cout << "#######################";
            for(int j = 0; j < W; j++) {
                if(m.at(i).at(j) || fall_piece.at(i).at(j)) cout << BLOCK;
                else cout << EMPTY;
            }

            cout << "#######################";
            cout << endl;
        }
    }
};


int main() {
    int H = 20;
    int W = 10;
    string B = "■";
    string E = " ";
    tetris tet(H, W, B, E);
    int now = 0;
    tet.new_piece();
    while(true) {
        double update_cycle = 0.05;
        double fall_speed = 1.0;
        double finish_time = 100.0;
        tet.print();
        cout << "                          " << now << endl;
        cout << "                  score : " <<tet.score << endl;
        if(tet.update()) return 0;
        tet.print();
        
//        usleep(update_cycle*1000000);
        if(now*update_cycle > finish_time) break;
        else now++;
    }
}