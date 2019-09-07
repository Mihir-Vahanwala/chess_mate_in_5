#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <map>
#include <istream>
using namespace std;

enum occupied {empty, white, black};
enum resident {none, pawn, knight, bishop, rook, queen, king};
enum direction {N, NW, W, SW, S, SE, E, NE};



int signum(int n)
{
    if (n == 0) return 0;
    if (n>0) return 1;
    else return -1;
}


class Square
{
public:
    occupied colour;
    resident piece;
    
    Square()
    {
        colour = empty; piece = none;
    }
    
    void operator = (Square &square)
    {
        colour = square.colour; piece = square.piece;
    }
    
    Square move (Square &a, Square &b)
    {
        Square temp ;
        temp = b;
        b = a;
        a.colour = empty; a.piece = none;
        return temp;
    }
};


class Location
{
public:
    int file; int rank;
    Location() {};
    Location (int a, int b) {file = a; rank = b;}
    void operator = (Location &location)
    {
        file = location.file; rank = location.rank;
    }
    
    void operator = (Location location)
    {
        file = location.file; rank = location.rank;
    }
    
    bool onBoard (Location &location)
    {
        return (location.file<8 && location.rank<8 && location.file > -1 && location.rank>-1);
    }
    
    bool forward(Location &location, direction arrow)
    {
        switch (arrow)
        {
            case N: {location.rank++; break;}
            case NW: {location.rank++; location.file--; break;}
            case W: {location.file--; break;}
            case SW: {location.rank--; location.file--; break;}
            case S: {location.rank--; break;}
            case SE: {location.rank--; location.file++; break;}
            case E: {location.file++; break;}
            case NE: {location.rank++; location.file++; break;}
                
        }
        return onBoard(location);
    }
    
    bool neigh(Location &location, int i)
    {
        if (forward(location, direction(i))) {if (forward(location, direction((i+1)%8))) return true; else return false;}
        else return false;
    }
    
    void print(Location location)
    {
        char alphafile;
        alphafile = 'a'+location.file;
        int disprank;
        disprank = location.rank + 1;
        cout<<alphafile<<disprank<<" ";
    }
    
    bool operator == (Location location)
    {
        return (file == location.file && rank == location.rank);
    }
    
    bool operator != (Location location)
    {
        return (file != location.file || rank != location.rank);
    }
};

bool find(Location king, vector <Location> range)
{
    bool answer = false;
    for (int i=0; i<range.size()&& !answer; i++)
    {
        answer = (king == range[i]);
    }
    return answer;
}

int nfind(Location king, vector <Location> range)
{
    int answer = 0;
    for (int i=0; i<range.size(); i++)
    {
        if (king == range[i]) answer++;
    }
    return answer;
}

Location convert(string s)
{
    Location answer(s[0] - 'a', s[1] - '1');
    return answer;
}

string convert(Location l)
{
    string s; s.resize(2);
    s[0] = 'a' + l.file; s[1] = '1' + l.rank;
    return s;
}

struct Move
{

    string s;
    int n;
    
    Move() {s = "No mate in 1"; n = 5;}
    
    Move(string s1, int n1)
    {
        s = s1; n = n1;
    }
    
    Move (const Move &m)
    {
        s =m.s;
        n= m.n;
    }
};



class Chessboard
{
public:
    Square chessboard[8][8]; vector <Location> whiteRange; vector <Location> blackRange;
    Location whiteK, blackK; occupied turn;
    bool wkc, bkc, wqc, bqc; int enpassant;
    int fiftyMove;
    
    Chessboard()
    {
        fiftyMove = 0;
        turn = white;
        wkc = true; wqc = true; bkc = true; bqc = true;
        enpassant = -2;
        for (int i=0; i<8; i++)
        {
            chessboard[i][1].colour = white; chessboard[i][1].piece = pawn;
            chessboard[i][6].colour = black; chessboard[i][6].piece = pawn;
            chessboard[i][0].colour = white; chessboard[i][7].colour = black;
        }
        
        for (int i=0; i<8; i++)
        {
            for (int j=2; j<6; j++)
            {
                chessboard[i][j].colour = empty; chessboard[i][j].piece = none;
            }
        }
        
        for (int i= 0; i<8; i=i+7)
        {
            chessboard[0][i].piece=rook; chessboard[7][i].piece =rook;
            chessboard[1][i].piece = knight; chessboard[6][i].piece = knight;
            chessboard[2][i].piece = bishop; chessboard[5][i].piece = bishop;
            chessboard[3][i].piece = queen; chessboard[4][i].piece = king;
        }
        whiteK.file = 4; whiteK.rank = 0; blackK.file = 4; blackK.rank = 7;
    }
    
    Chessboard (const Chessboard &original)
    {
        fiftyMove = original.fiftyMove;
        for (int i=0; i<8; i++)
        {
            for (int j=0; j<8; j++)
            {
                chessboard[i][j].colour = original.chessboard[i][j].colour;
                chessboard[i][j].piece = original.chessboard[i][j].piece;
            }
        }
        wkc = original.wkc; bkc = original.bkc; wqc = original.wqc; bqc = original.bqc;
        enpassant = original.enpassant;
        whiteK.file = original.whiteK.file; blackK.file = original.blackK.file;
        whiteK.rank = original.whiteK.rank; blackK.rank = original.blackK.rank;
        turn = original.turn;
        whiteRange.resize(original.whiteRange.size());
        blackRange.resize(original.blackRange.size());
        for (int a=0; a<whiteRange.size(); a++)
        {
            whiteRange[a].file = original.whiteRange[a].file;
            whiteRange[a].rank = original.whiteRange[a].rank;
        }
        
        for (int a=0; a<blackRange.size(); a++)
        {
            blackRange[a].file = original.blackRange[a].file;
            blackRange[a].rank = original.blackRange[a].rank;
        }
    }
    
    Chessboard(int chessmen)
    {
    label1: for (int i = 0; i<8; i++)
        {
            for (int j=0; j<8; j++)
            {
                chessboard[i][j].colour = empty;
                chessboard[i][j].piece = none;
            }
        }
        cout<<"White King? ";
        string wk;
        cin>>wk;
        Location drop(convert(wk));
        chessboard[drop.file][drop.rank].colour = white;
        chessboard[drop.file][drop.rank].piece = king;
        whiteK.file = drop.file; whiteK.rank = drop.rank;
        
        print();
        
        cout<<"Black King? ";
        string bk;
        cin>>bk;
        Location place(convert(bk));
        while (drop == place)
        {
            cout<<"Place on unoccupied square... ";
            cin>>bk;
            place = convert(bk);
        }
        chessboard[place.file][place.rank].colour = black;
        chessboard[place.file][place.rank].piece = king;
        blackK.file = place.file; blackK.rank = place.rank;
        
        print();
        
        for (int a=2; a<chessmen; a++)
        {
            int piececode, colour;
            cout<<"Enter colour...";
            cin>>colour;
            cout<<"Enter piece code... ";
            cin>>piececode;
            while (piececode == 6)
            {
                cout<<"We can only have two kings... ";
                cin>>piececode;
            }
            string s;
            cout<<"Enter location... ";
            cin>>s;
            Location plant(convert(s));
            while (chessboard[plant.file][plant.rank].piece != none || (resident(piececode) == pawn && (plant.rank == 0 || plant.rank == 7)) )
            {
                cout<<"Place on unoccupied and valid square... ";
                cin>>s;
                plant = convert(s);
            }
            chessboard[plant.file][plant.rank].piece = resident(piececode);
            chessboard[plant.file][plant.rank].colour = occupied(colour);
            print();
        }
        wkc = true; bkc = true; wqc = true; bqc = true; enpassant = -2;
        getRanges();
        if ((find(whiteK, blackRange) && find(blackK, whiteRange)) || nfind(whiteK, blackRange) > 2 || nfind(blackK, whiteRange) > 2)
        {
            cout<<"Invalid position. Construct again..."<<endl;
            goto label1;
        }
        if (find(whiteK, blackRange)) turn = white;
        else if (find(blackK, whiteRange)) turn = black;
        else
        {
            
            cout<<"Whose turn? ";
            int reply;
            cin>>reply;
            turn = occupied(reply);
            
        }
        cout<<endl;
        print();
        cout<<turn<<endl;
        fiftyMove = 0;
    }
    
    
    bool clear(Location a, Location b)
    {
        if (!(a.file == b.file || a.rank == b.rank || abs(a.file - b.file) == abs(a.rank - b.rank))) return false;
        Location point(a.file, a.rank);
        int fileadd, rankadd;
        fileadd = signum(b.file - a.file); rankadd = signum(b.rank - a.rank);
        bool answer = true;
        point.file = point.file+fileadd;
        point.rank = point.rank+rankadd;
        while (point != b && answer)
        {
            answer = (chessboard[point.file][point.rank].piece == none);
            point.file = point.file + fileadd;
            point.rank = point.rank + rankadd;
        }
        return answer;
    }
    
    Square square(Location l)
    {
        return chessboard[l.file][l.rank];
    }
    
    void promote(Location end)
    {
        if (end.rank == 7 || end.rank == 0)
        {
            cout<<"Enter piece code...";
            int newpiece;
            cin>>newpiece;
            if (newpiece >5 || newpiece < 2) cout<<"ERROR";
            chessboard[end.file][end.rank].piece = resident(newpiece);
            getRanges();
        }
        else {}
    }
    
    void promoteA(Location end, int piececode)
    {
        if (end.rank == 7 || end.rank == 0)
        {
            chessboard[end.file][end.rank].piece = resident(piececode);
            getRanges();
        }
        else{}
    }
    
    void switchTurn()
    {
        if (turn == white) turn = black;
        else turn = white;
    }
    
    void getRanges()
    {
        whiteRange.resize(0); blackRange.resize(0);
        for (int i=0; i<8; i++)
        {
            for (int j=0; j<8; j++)
            {
                if (chessboard[i][j].piece == none) {}
                
                
                else if (chessboard[i][j].piece == pawn && chessboard[i][j].colour == white)
                {
                    Location location(i, j);
                    if (location.forward(location, NW))
                    {
                        if (chessboard[location.file][location.rank].colour == white) {}
                        else
                        {
                        whiteRange.push_back(location);
                        }
                    }
                    location.file = i; location.rank = j;
                    if (location.forward(location, NE))
                    {if (chessboard[location.file][location.rank].colour == white) {}
                    else
                    {
                        whiteRange.push_back(location);
                    }}
                    
                }
                
                else if (chessboard[i][j].piece == pawn && chessboard[i][j].colour == black)
                {
                    Location location(i, j);
                    if (location.forward(location, SW))
                    {if (chessboard[location.file][location.rank].colour == black) {}
                    else
                    {
                        blackRange.push_back(location);
                    }}
                    location.file = i; location.rank = j;
                    if (location.forward(location, SE))
                    {if (chessboard[location.file][location.rank].colour == black) {}
                    else
                    {
                        blackRange.push_back(location);
                    }}
                }
                
                else if (chessboard[i][j].piece == king && chessboard[i][j].colour == white)
                {
                    int k;
                    for (k = 0 ; k <= 7; k++)
                    {
                        Location location(i, j);
                        if (location.forward(location, direction(k)))
                        {
                            if (chessboard[location.file][location.rank].colour == white) {}
                            else whiteRange.push_back(location);
                        }
                    }
                }
                
                else if (chessboard[i][j].piece == king && chessboard[i][j].colour == black)
                {
                    int k;
                    for (k = 0 ; k <= 7; k++)
                    {
                        Location location(i, j);
                        if (location.forward(location, direction(k)))
                        {
                            if (chessboard[location.file][location.rank].colour == black) {}
                            else blackRange.push_back(location);
                        }
                    }
                }
                
                else if (chessboard[i][j].piece == queen && chessboard[i][j].colour == white)
                {
                    int k;
                    for (k=0; k<=7; k++)
                    {
                        Location location(i, j);
                        while (location.forward(location, direction(k)))
                        {
                            if (chessboard[location.file][location.rank].colour == white) break;
                            else if (chessboard[location.file][location.rank].colour == empty) {whiteRange.push_back(location);}
                            else
                            {
                                whiteRange.push_back(location); break;
                            }
                        }
                    }
                }
                
                else if (chessboard[i][j].piece == queen && chessboard[i][j].colour == black)
                {
                    int k;
                    for (k=0; k<=7; k++)
                    {
                        Location location(i, j);
                        while (location.forward(location, direction(k)))
                        {
                            if (chessboard[location.file][location.rank].colour == black) break;
                            else if (chessboard[location.file][location.rank].colour == empty) {blackRange.push_back(location);}
                            else
                            {
                                blackRange.push_back(location); break;
                            }
                        }
                    }
                }
                
                else if (chessboard[i][j].piece == rook && chessboard[i][j].colour == black)
                {
                    int k;
                    for (k=0; k<=7; k=k+2)
                    {
                        Location location(i, j);
                        while (location.forward(location, direction(k)))
                        {
                            if (chessboard[location.file][location.rank].colour == black) break;
                            else if (chessboard[location.file][location.rank].colour == empty) {blackRange.push_back(location);}
                            else
                            {
                                blackRange.push_back(location); break;
                            }
                        }
                    }
                }
                
                else if (chessboard[i][j].piece == rook && chessboard[i][j].colour == white)
                {
                    int k;
                    for (k=0; k<=7; k=k+2)
                    {
                        Location location(i, j);
                        while (location.forward(location, direction(k)))
                        {
                            if (chessboard[location.file][location.rank].colour == white) break;
                            else if (chessboard[location.file][location.rank].colour == empty) {whiteRange.push_back(location);}
                            else
                            {
                                whiteRange.push_back(location); break;
                            }
                        }
                    }
                }
                
                else if (chessboard[i][j].piece == bishop && chessboard[i][j].colour == white)
                {
                    int k;
                    for (k=1; k<=7; k=k+2)
                    {
                        Location location(i, j);
                        while (location.forward(location, direction(k)))
                        {
                            if (chessboard[location.file][location.rank].colour == white) break;
                            else if (chessboard[location.file][location.rank].colour == empty) {whiteRange.push_back(location);}
                            else
                            {
                                whiteRange.push_back(location); break;
                            }
                        }
                    }
                }
                
                else if (chessboard[i][j].piece == bishop && chessboard[i][j].colour == black)
                {
                    int k;
                    for (k=1; k<=7; k=k+2)
                    {
                        Location location(i, j);
                        while (location.forward(location, direction(k)))
                        {
                            if (chessboard[location.file][location.rank].colour == black) break;
                            else if (chessboard[location.file][location.rank].colour == empty) {blackRange.push_back(location);}
                            else
                            {
                                blackRange.push_back(location); break;
                            }
                        }
                    }
                }
                
                else if (chessboard[i][j].piece == knight && chessboard[i][j].colour == white)
                {
                    int k;
                    for (k=0; k<=7; k++)
                    {
                        Location location(i, j);
                        if (location.neigh(location, k))
                        {
                            if (chessboard[location.file][location.rank].colour == white) {}
                            else {whiteRange.push_back(location);}
                        }
                    }
                }
                
                else if (chessboard[i][j].piece == knight && chessboard[i][j].colour == black)
                {
                    int k;
                    for (k=0; k<=7; k++)
                    {
                        Location location(i, j);
                        if (location.neigh(location, k))
                        {
                            if (chessboard[location.file][location.rank].colour == black) {}
                            else {blackRange.push_back(location);}
                        }
                    }
                }
            }
        }
    }
    
    bool whistle()
    {
        getRanges();
        if (turn == white) {if (find(whiteK, blackRange)) { return true;} else return false;}
        else {if (find(blackK, whiteRange)) { return true;} else return false;}
    }
    void print()
    {
        for (int rank = 7; rank >=0; rank--)
        {
            for (int file = 0; file <= 7; file ++)
            {
                cout<<chessboard[file][rank].colour<<chessboard[file][rank].piece<<" ";
            }
            cout<<endl;
        }
    }
    
    void newPrint()
    {
        for (int j=7; j>=0; j--)
        {
            for (int i=0; i<8; i++)
            {
                if (chessboard[i][j].colour == empty && chessboard[i][j].piece == none) cout<<"  ";
                else if (chessboard[i][j].colour == white && chessboard[i][j].piece == king) wcout<<L'\u2654';
                else if (chessboard[i][j].colour == white && chessboard[i][j].piece == queen) wcout<<L'\u2655';
                else if (chessboard[i][j].colour == white && chessboard[i][j].piece == rook) wcout<<L'\u2656';
                else if (chessboard[i][j].colour == white && chessboard[i][j].piece == bishop) wcout<<L'\u2657';
                else if (chessboard[i][j].colour == white && chessboard[i][j].piece == knight) wcout<<L'\u2658';
                else if (chessboard[i][j].colour == white && chessboard[i][j].piece == pawn) wcout<<L'\u2659';
                else if (chessboard[i][j].colour == black && chessboard[i][j].piece == king) wcout<<L'\u265A';
                else if (chessboard[i][j].colour == black && chessboard[i][j].piece == queen) wcout<<L'\u265B';
                else if (chessboard[i][j].colour == black && chessboard[i][j].piece == rook) wcout<<L'\u265C';
                else if (chessboard[i][j].colour == black && chessboard[i][j].piece == bishop) wcout<<L'\u265D';
                else if (chessboard[i][j].colour == black && chessboard[i][j].piece == knight) wcout<<L'\u265E';
                else if (chessboard[i][j].colour == black && chessboard[i][j].piece == pawn) wcout<<L'\u265F';
            }
            cout<<endl;
        }
    }
    
    bool validMove(string s)
    {
        Square sq;
        
        if (s == "0-0" && turn == white)
        {
            if (!wkc) return false;
            if (chessboard[4][0].colour != white || chessboard[4][0].piece != king) {wkc = false; return false;}
            if (chessboard[7][0].colour != white || chessboard[7][0].piece != rook) {wkc = false; return false;}
            Location a(4, 0), b(7, 0), c(5, 0), d(6, 0);
            if (!clear(a, b)) return false;
            if (find(a, blackRange) || find(c, blackRange) || find(d, blackRange)) return false;
            sq.move(chessboard[4][0], chessboard[6][0]);
            sq.move(chessboard[7][0], chessboard[5][0]);
            whiteK.file = 6; whiteK.rank = 0;
            getRanges();
            if (find(whiteK, blackRange)) return false;
            turn = black;
            wkc = false; wqc = false;
            enpassant = -2;
            fiftyMove ++ ;
            return true;
        }
        
        if (s == "0-0" && turn == black)
        {
            if (!bkc) return false;
            if (chessboard[4][7].colour != black || chessboard[4][7].piece != king) {bkc = false; return false;}
            if (chessboard[7][7].colour != black || chessboard[7][7].piece != rook) {bkc = false; return false;}
            
            Location a(4, 7), b(7, 7), c(5, 7), d(6, 7);
            if (!clear(a, b)) return false;
            if (find(a, whiteRange) || find(c, whiteRange) || find(d, whiteRange) ) return false;
            sq.move(chessboard[4][7], chessboard[6][7]);
            sq.move(chessboard[7][7], chessboard[5][7]);
            blackK.file = 6; blackK.rank = 7;
            getRanges();
            if (find(blackK, whiteRange)) return false;
            turn = white;
            bkc = false; bqc = false;
            enpassant = -2;
            fiftyMove ++;
            return true;
        }
        
        if (s == "0-0-0" && turn == white)
        {
            if (!wqc) return false;
            if (chessboard[4][0].colour != white || chessboard[4][0].piece != king) {wqc = false; return false;}
            if (chessboard[0][0].colour != white || chessboard[0][0].piece != rook) {wqc = false; return false;}
            
            Location a(4, 0), b(3, 0), c(2, 0), d(0, 0);
            if (!clear(a, d)) return false;
            if (find(a, blackRange) || find(b, blackRange) || find(c, blackRange)) return false;
            sq.move(chessboard[4][0], chessboard[2][0]);
            sq.move(chessboard[0][0], chessboard[3][0]);
            whiteK.file = 2; whiteK.rank = 0;
            getRanges();
            if (find(whiteK, blackRange)) return false;
            turn = black;
            wkc = false; wqc = false;
            enpassant = -2;
            fiftyMove++;
            return true;
        }
        
        if (s == "0-0-0" && turn == black)
        {
            if (!bqc) return false;
            if (chessboard[4][7].colour != black || chessboard[4][7].piece != king) {bqc= false; return false;}
            if (chessboard[0][7].colour != black || chessboard[0][7].piece != rook) {bqc= false; return false;}
            
            Location a(4, 7), b(3, 7), c(2, 7), d(0, 7);
            if (!clear(a, d)) return false;
            if (find(a, whiteRange) || find(b, whiteRange) || find(c, whiteRange)) return false;
            sq.move(chessboard[4][7], chessboard[2][7]);
            sq.move(chessboard[0][7], chessboard[3][7]);
            blackK.file = 2; blackK.rank = 7;
            getRanges();
            if (find(blackK, whiteRange)) return false;
            turn = white;
            bkc = false; bqc = false;
            enpassant = -2;
            fiftyMove++;
            return true;
        }
        
        if (s.size() != 5) return false;
        
        Location start(convert(s.substr(0, 2)));
        Location end(convert(s.substr(3, 2)));
        
        if (chessboard[start.file][start.rank].colour != turn) return false;
        if (chessboard[end.file][end.rank].colour == turn) return false;
        if (chessboard[start.file][start.rank].piece == none) return false;
        
        if (chessboard[start.file][start.rank].piece == queen)
        {
            if (!clear(start, end)) return false;
            bool add = (chessboard[end.file][end.rank].colour == empty);
            Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
            if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
            switchTurn();
            enpassant = -2;
            if (add) fiftyMove ++;
            else fiftyMove = 0;
            return true;
        }
        
        if (chessboard[start.file][start.rank].piece == king)
        {
            if (abs(start.file - end.file) + abs(start.rank - end.rank) > 2) return false;
            if (abs(start.file - end.file) != 1 && abs(start.rank - end.rank) != 1) return false;
            
            bool add = (chessboard[end.file][end.rank].colour == empty);
            
            Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
            
            if (turn == white) {whiteK.file = end.file; whiteK.rank = end.rank; }
            else {blackK.file = end.file; blackK.rank = end.rank; }
            
            if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; if (turn == white) {whiteK.file = start.file; whiteK.rank = start.rank;} else {blackK.file = start.file; blackK.rank = start.rank;} return false;}
            
            if (turn == white) {wkc = false; wqc = false;}
            if (turn == black) {bkc = false; bqc = false;}
            
            switchTurn();
            enpassant = -2;
            if (add) fiftyMove++;
            else fiftyMove = 0;
            return true;
        }
        
        if (chessboard[start.file][start.rank].piece == knight)
        {
            if (abs(start.file - end.file) + abs(start.rank - end.rank) != 3) return false;
            if (start.file == end.file || start.rank == end.rank) return false;
            
            bool add = (chessboard[end.file][end.rank].colour == empty);
            
            Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
            if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
            switchTurn();
            enpassant = -2;
            if (add) fiftyMove++;
            else fiftyMove = 0;
            return true;
        }
        
        if (chessboard[start.file][start.rank].piece == bishop)
        {
            if (abs(start.file - end.file) != abs(start.rank - end.rank)) return false;
            if (!clear(start, end)) return false;
            
            bool add = (chessboard[end.file][end.rank].colour == empty);
            
            Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
            if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
            switchTurn();
            enpassant = -2;
            if (add) fiftyMove++;
            else fiftyMove = 0;
            return true;
        }
        
        if (chessboard[start.file][start.rank].piece == rook)
        {
            if (start.file != end.file && start.rank != end.rank) return false;
            if (!clear(start, end)) return false;
            
            bool add = (chessboard[end.file][end.rank].colour == empty);
            
            Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
            if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
            switchTurn();
            enpassant = -2;
            if (start.file == 0 && start.rank == 0) wqc = false;
            if (start.file == 7 && start.rank == 0) wkc = false;
            if (start.file == 0 && start.rank == 7) bqc = false;
            if (start.file == 7 && start.rank == 7) bkc = false;
            
            if (end.file == 0 && end.rank == 0) wqc = false;
            if (end.file == 7 && end.rank == 0) wkc = false;
            if (end.file == 0 && end.rank == 7) bqc = false;
            if (end.file == 7 && end.rank == 7) bkc = false;
            
            if (add) fiftyMove++;
            else fiftyMove = 0;
            
            return true;
        }
        
        if (chessboard[start.file][start.rank].piece == pawn)
        {
            if (abs(end.rank - start.rank) > 2 || abs(end.file - start.file) > 1 || start.rank == end.rank) return false;
            
            if (turn == white)
            {
                if (end.rank < start.rank) return false;
                if (end.rank - start.rank == 2)
                {
                    if (start.rank != 1) return false;
                    if (start.file != end.file ) return false;
                    if (!clear(start, end)) return false;
                    if (chessboard[end.file][end.rank].colour != empty) return false;
                    sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                    switchTurn();
                    enpassant = start.file;
                    fiftyMove = 0;
                    return true;
                }
                
                if (start.file != end.file) //start.rank and end.rank guaranteed to be right
                {
                    if (chessboard[end.file][end.rank].colour != black)
                    {
                        if (start.rank != 4) return false;
                        else
                        {
                            if (enpassant == end.file)
                            {
                                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                                Square temp = chessboard[end.file][start.rank];
                                chessboard[end.file][start.rank].piece = none;
                                chessboard[end.file][start.rank].colour = empty;
                                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][start.rank] = temp; return false;}
                                switchTurn();
                                enpassant = -2;
                                fiftyMove = 0;
                                return true;
                            }
                            else return false;
                        }
                    }
                    Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
                    promote(end);
                    switchTurn();
                    enpassant = -2;
                    fiftyMove = 0;
                    return true;
                    
                }
                
                if (chessboard[end.file][end.rank].colour != empty) return false;
                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                promote(end);
                switchTurn();
                enpassant = -2;
                fiftyMove = 0;
                return true;
             }
            
            
            
            
            if (turn == black)
            {
                if (end.rank > start.rank) return false;
                if (end.rank - start.rank == -2)
                {
                    if (start.rank != 6) return false;
                    if (start.file != end.file ) return false;
                    if (!clear(start, end)) return false;
                    if (chessboard[end.file][end.rank].colour != empty) return false;
                    sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                    switchTurn();
                    enpassant = start.file;
                    fiftyMove = 0;
                    return true;
                }
                
                if (start.file != end.file) //start.rank and end.rank guaranteed to be right
                {
                    if (chessboard[end.file][end.rank].colour != white)
                    {
                        if (start.rank != 3) return false;
                        else
                        {
                            if (enpassant == end.file)
                            {
                                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                                Square temp = chessboard[end.file][start.rank];
                                chessboard[end.file][start.rank].piece = none;
                                chessboard[end.file][start.rank].colour = empty;
                                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][start.rank] = temp; return false;}
                                switchTurn();
                                enpassant = -2;
                                fiftyMove = 0;
                                return true;
                            }
                            else return false;
                        }
                    }
                    Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
                    promote(end);
                    switchTurn();
                    enpassant = -2;
                    fiftyMove = 0;
                    return true;
                    
                }
                
                if (chessboard[end.file][end.rank].colour != empty) return false;
                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                promote(end);
                switchTurn();
                enpassant = -2;
                fiftyMove = 0;
                return true;
            }
        }
        
        return false;
    }
    
    bool validMoveA(string s, int piececode) //fiftyMove feature NOT added; this function is called only to find mate
    {
        Square sq;
        
        if (s == "0-0" && turn == white)
        {
            if (!wkc) { return false;}
            if (chessboard[4][0].colour != white || chessboard[4][0].piece != king) {wkc = false; return false;}
            if (chessboard[7][0].colour != white || chessboard[7][0].piece != rook) {wkc = false; return false;}
            Location a(4, 0), b(7, 0), c(5, 0), d(6, 0);
            if (!clear(a, b)) { return false;}
            if (find(a, blackRange) || find(c, blackRange) || find(d, blackRange)) { return false;}
            sq.move(chessboard[4][0], chessboard[6][0]);
            sq.move(chessboard[7][0], chessboard[5][0]);
            whiteK.file = 6; whiteK.rank = 0;
            getRanges();
            if (find(whiteK, blackRange)) { return false;}
            turn = black;
            wkc = false; wqc = false;
            enpassant = -2;
            return true;
        }
        
        if (s == "0-0" && turn == black)
        {
            if (!bkc) return false;
            if (chessboard[4][7].colour != black || chessboard[4][7].piece != king) {bkc = false; return false;}
            if (chessboard[7][7].colour != black || chessboard[7][7].piece != rook) {bkc = false; return false;}
            
            Location a(4, 7), b(7, 7), c(5, 7), d(6, 7);
            if (!clear(a, b)) return false;
            if (find(a, whiteRange) || find(c, whiteRange) || find(d, whiteRange) ) return false;
            sq.move(chessboard[4][7], chessboard[6][7]);
            sq.move(chessboard[7][7], chessboard[5][7]);
            blackK.file = 6; blackK.rank = 7;
            getRanges();
            if (find(blackK, whiteRange)) return false;
            turn = white;
            bkc = false; bqc = false;
            enpassant = -2;
            return true;
        }
        
        if (s == "0-0-0" && turn == white)
        {
            if (!wqc) return false;
            if (chessboard[4][0].colour != white || chessboard[4][0].piece != king) {wqc = false; return false;}
            if (chessboard[0][0].colour != white || chessboard[0][0].piece != rook) {wqc = false; return false;}
            
            Location a(4, 0), b(3, 0), c(2, 0), d(0, 0);
            if (!clear(a, d)) return false;
            if (find(a, blackRange) || find(b, blackRange) || find(c, blackRange)) return false;
            sq.move(chessboard[4][0], chessboard[2][0]);
            sq.move(chessboard[0][0], chessboard[3][0]);
            whiteK.file = 2; whiteK.rank = 0;
            getRanges();
            if (find(whiteK, blackRange)) return false;
            turn = black;
            wkc = false; wqc = false;
            enpassant = -2;
            return true;
        }
        
        if (s == "0-0-0" && turn == black)
        {
            if (!bqc) return false;
            if (chessboard[4][7].colour != black || chessboard[4][7].piece != king) {bqc= false; return false;}
            if (chessboard[0][7].colour != black || chessboard[0][7].piece != rook) {bqc= false; return false;}
            
            Location a(4, 7), b(3, 7), c(2, 7), d(0, 7);
            if (!clear(a, d)) return false;
            if (find(a, whiteRange) || find(b, whiteRange) || find(c, whiteRange)) return false;
            sq.move(chessboard[4][7], chessboard[2][7]);
            sq.move(chessboard[0][7], chessboard[3][7]);
            blackK.file = 2; blackK.rank = 7;
            getRanges();
            if (find(blackK, whiteRange)) return false;
            turn = white;
            bkc = false; bqc = false;
            enpassant = -2;
            return true;
        }
        
        if (s.size() != 5) return false;
        
        Location start(convert(s.substr(0, 2)));
        Location end(convert(s.substr(3, 2)));
        
        if (chessboard[start.file][start.rank].colour != turn) return false;
        if (chessboard[end.file][end.rank].colour == turn) return false;
        if (chessboard[start.file][start.rank].piece == none) return false;
        
        if (chessboard[start.file][start.rank].piece == queen)
        {
            if (!clear(start, end)) return false;
            Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
            if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
            switchTurn();
            enpassant = -2;
            return true;
        }
        
        if (chessboard[start.file][start.rank].piece == king)
        {
            if (abs(start.file - end.file) + abs(start.rank - end.rank) > 2) return false;
            if (abs(start.file - end.file) != 1 && abs(start.rank - end.rank) != 1) return false;
            
            Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
            
            if (turn == white) {whiteK.file = end.file; whiteK.rank = end.rank; }
            else {blackK.file = end.file; blackK.rank = end.rank; }
            
            if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; if (turn == white) {whiteK.file = start.file; whiteK.rank = start.rank;} else {blackK.file = start.file; blackK.rank = start.rank;} return false;}
            
            if (turn==white) {wkc=false; wqc=false;}
            if (turn==black) {bkc=false; bqc=false;}
            switchTurn();
            enpassant = -2;
            return true;
        }
        
        if (chessboard[start.file][start.rank].piece == knight)
        {
            if (abs(start.file - end.file) + abs(start.rank - end.rank) != 3) return false;
            if (start.file == end.file || start.rank == end.rank) return false;
            Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
            if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
            switchTurn();
            enpassant = -2;
            return true;
        }
        
        if (chessboard[start.file][start.rank].piece == bishop)
        {
            if (abs(start.file - end.file) != abs(start.rank - end.rank)) return false;
            if (!clear(start, end)) return false;
            Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
            if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
            switchTurn();
            enpassant = -2;
            return true;
        }
        
        if (chessboard[start.file][start.rank].piece == rook)
        {
            if (start.file != end.file && start.rank != end.rank) return false;
            if (!clear(start, end)) return false;
            Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
            if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
            switchTurn();
            enpassant = -2;
            if (start.file == 0 && start.rank == 0) wqc = false;
            if (start.file == 7 && start.rank == 0) wkc = false;
            if (start.file == 0 && start.rank == 7) bqc = false;
            if (start.file == 7 && start.rank == 7) bkc = false;
            
            if (end.file == 0 && end.rank == 0) wqc = false;
            if (end.file == 7 && end.rank == 0) wkc = false;
            if (end.file == 0 && end.rank == 7) bqc = false;
            if (end.file == 7 && end.rank == 7) bkc = false;
            
            
            
            return true;
        }
        
        if (chessboard[start.file][start.rank].piece == pawn)
        {
            if (abs(end.rank - start.rank) > 2 || abs(end.file - start.file) > 1 || start.rank == end.rank) return false;
            
            if (turn == white)
            {
                if (end.rank < start.rank) return false;
                if (end.rank - start.rank == 2)
                {
                    if (start.rank != 1) return false;
                    if (start.file != end.file ) return false;
                    if (!clear(start, end)) return false;
                    if (chessboard[end.file][end.rank].colour != empty) return false;
                    sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                    switchTurn();
                    enpassant = start.file;
                    return true;
                }
                
                if (start.file != end.file) //start.rank and end.rank guaranteed to be right
                {
                    if (chessboard[end.file][end.rank].colour != black)
                    {
                        if (start.rank != 4) return false;
                        else
                        {
                            if (enpassant == end.file)
                            {
                                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                                Square temp = chessboard[end.file][start.rank];
                                chessboard[end.file][start.rank].piece = none;
                                chessboard[end.file][start.rank].colour = empty;
                                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][start.rank] = temp; return false;}
                                switchTurn();
                                enpassant = -2;
                                return true;
                            }
                            else return false;
                        }
                    }
                    Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
                    promoteA(end, piececode);
                    switchTurn();
                    enpassant = -2;
                    return true;
                    
                }
                
                if (chessboard[end.file][end.rank].colour != empty) return false;
                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                promoteA(end, piececode);
                switchTurn();
                enpassant = -2;
                return true;
            }
            
            
            
            
            if (turn == black)
            {
                if (end.rank > start.rank) return false;
                if (end.rank - start.rank == -2)
                {
                    if (start.rank != 6) return false;
                    if (start.file != end.file ) return false;
                    if (!clear(start, end)) return false;
                    if (chessboard[end.file][end.rank].colour != empty) return false;
                    sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                    switchTurn();
                    enpassant = start.file;
                    return true;
                }
                
                if (start.file != end.file) //start.rank and end.rank guaranteed to be right
                {
                    if (chessboard[end.file][end.rank].colour != white)
                    {
                        if (start.rank != 3) return false;
                        else
                        {
                            if (enpassant == end.file)
                            {
                                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                                Square temp = chessboard[end.file][start.rank];
                                chessboard[end.file][start.rank].piece = none;
                                chessboard[end.file][start.rank].colour = empty;
                                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][start.rank] = temp; return false;}
                                switchTurn();
                                enpassant = -2;
                                return true;
                            }
                            else return false;
                        }
                    }
                    Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
                    promoteA(end, piececode);
                    switchTurn();
                    enpassant = -2;
                    return true;
                    
                }
                
                if (chessboard[end.file][end.rank].colour != empty) return false;
                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                promoteA(end, piececode);
                switchTurn();
                enpassant = -2;
                return true;
            }
        }
        
        return false;
    }
    
    bool checkValidity(Location start, Location end)
    {
        Square sq, temporary;
        temporary = chessboard[end.file][end.rank];
        if (chessboard[start.file][start.rank].piece == pawn)
        {
            if (abs(end.rank - start.rank) > 2 || abs(end.file - start.file) > 1 || start.rank == end.rank) return false;
            
            if (turn == white)
            {
                if (end.rank < start.rank) return false;
                if (end.rank - start.rank == 2)
                {
                    if (start.rank != 1) return false;
                    if (start.file != end.file ) return false;
                    if (!clear(start, end)) return false;
                    if (chessboard[end.file][end.rank].colour != empty) return false;
                    sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                    sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]);
                    return true;
                }
                
                if (start.file != end.file) //start.rank and end.rank guaranteed to be right
                {
                    if (chessboard[end.file][end.rank].colour != black)
                    {
                        if (start.rank != 4) return false;
                        else
                        {
                            if (enpassant == end.file)
                            {
                                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                                Square temp = chessboard[end.file][start.rank];
                                chessboard[end.file][start.rank].piece = none;
                                chessboard[end.file][start.rank].colour = empty;
                                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][start.rank] = temp; return false;}
                                sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][start.rank] = temp;
                                return true;
                            }
                            else return false;
                        }
                    }
                    Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
                    sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp;
                    return true;
                    
                }
                
                if (chessboard[end.file][end.rank].colour != empty) return false;
                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]);
                return true;
            }
            
            
            
            
            if (turn == black)
            {
                if (end.rank > start.rank) return false;
                if (end.rank - start.rank == -2)
                {
                    if (start.rank != 6) return false;
                    if (start.file != end.file ) return false;
                    if (!clear(start, end)) return false;
                    if (chessboard[end.file][end.rank].colour != empty) return false;
                    sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                    sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]);
                    return true;
                }
                
                if (start.file != end.file) //start.rank and end.rank guaranteed to be right
                {
                    if (chessboard[end.file][end.rank].colour != white)
                    {
                        if (start.rank != 3) return false;
                        else
                        {
                            if (enpassant == end.file)
                            {
                                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                                Square temp = chessboard[end.file][start.rank];
                                chessboard[end.file][start.rank].piece = none;
                                chessboard[end.file][start.rank].colour = empty;
                                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][start.rank] = temp; return false;}
                                sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][start.rank] = temp;
                                return true;
                            }
                            else return false;
                        }
                    }
                    Square temp = sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                    if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp; return false;}
                    sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); chessboard[end.file][end.rank] = temp;
                    return true;
                    
                }
                
                if (chessboard[end.file][end.rank].colour != empty) return false;
                sq.move(chessboard[start.file][start.rank], chessboard[end.file][end.rank]);
                if (whistle()) {sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]); return false;}
                sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]);
                return true;
            }
        }
        
        string s = convert(start)+'-'+convert(end);
        
        int hold = enpassant;
        bool h1, h2, h3, h4;
        h1 = wkc; h2 = bkc; h3 = wqc; h4 = bqc;
        
       
        
        if (validMoveA(s, 5))
        {
            
            sq.move(chessboard[end.file][end.rank], chessboard[start.file][start.rank]);
            chessboard[end.file][end.rank] = temporary;
            if (chessboard[start.file][start.rank].piece == king)
            {
                if (turn == black)
                {
                    whiteK.file = start.file; whiteK.rank = start.rank;
                }
                else
                {
                    blackK.file = start.file; blackK.rank = start.rank;
                }
            }
            switchTurn();
            enpassant = hold;
            wkc = h1; bkc = h2; wqc = h3; bqc = h4;
            return true;
        }
        
        else return false;
    }
    
    int moveCount()
    {
        int answer = 0;
        for (int i=0; i<8; i++)
        {
            for (int j=0; j<8; j++)
            {
                if (chessboard[i][j].colour != turn) {}
                else
                {
                    for (int p=0; p<8; p++)
                    {
                        for (int q=0; q<8; q++)
                        {
                            if (checkValidity(Location(i, j), Location(p, q))) answer++;
                        }
                    }
                }
            }
        }
        return answer;
    }
    
    
    
    vector <Move> allMoves()
    {
        vector <Move> answer;
        
        Chessboard copy1(*this);
        Move specialk("0-0", 5);
        if (copy1.validMoveA("0-0", 5)) answer.push_back(specialk);
        
        Chessboard copy2(*this);
        Move specialq("0-0-0", 5);
        if (copy2.validMoveA("0-0-0", 5)) answer.push_back(specialq);
        
        for (int i=0; i<8; i++)
        {
            for (int j=0; j<8; j++)
            {
                if (chessboard[i][j].colour != turn) {}
                else
                {
                    for (int p=0; p<8; p++)
                    {
                        for (int q=0; q<8; q++)
                        {
                            if (checkValidity(Location(i, j), Location(p, q)))
                            {
                                int n;
                                string s = convert(Location(i, j))+ '-' + convert(Location(p, q));
                                if (chessboard[i][j].piece == pawn && (q==0 || q== 7))
                                {
                                    for (n=2; n<6; n++)
                                    {
                                        Move m(s, n);
                                        answer.push_back(m);
                                    }
                                }
                                else
                                {
                                    Move m(s, 5);
                                    answer.push_back(m);
                                }
                            }
                        }
                    }
                }
            }
        }
        return answer;
    }
    
    bool check()
    {
        if (turn == white) {return find(whiteK, blackRange);}
        else return find(blackK, whiteRange);
    }
    
    bool checkmate()
    {
        return (check() && moveCount() == 0);
    }
    
    bool stalemate()
    {
        return (!(check())&& moveCount() == 0);
    }
    
    bool dead()
    {
        return (fiftyMove >= 100);
    }
    
};

class Toyboard
{
public:
    occupied turn;
    int enpassant;
    bool wkc, bkc, wqc, bqc;
    vector <int> description;
    
    Toyboard(Chessboard &original)
    {
        turn = original.turn;
        enpassant = original.enpassant;
        wkc = original.wkc; bkc = original.bkc; wqc = original.wqc; bqc = original.bqc;
        for (int i=0; i<8; i++)
        {
            for (int j=0; j<8; j++)
            {
                description.push_back(original.chessboard[i][j].colour*10 + original.chessboard[i][j].piece);
            }
        }
    }
    
    bool operator == (Toyboard &compare)
    {
        if (turn != compare.turn) return false;
        if (enpassant != compare.enpassant) return false;
        if (wkc != compare.wkc | bkc != compare.bkc || wqc != compare.wqc || bqc != compare.bqc) return false;
        bool answer = true;
        int i=0;
        while (i<64 && answer)
        {
            answer = (description.at(i) == compare.description.at(i));
            i++;
        }
        return answer;
    }
};

bool drawByRep(Toyboard recent, vector <Toyboard> &history)
{
    int matches = 0;
    for (int i=0; i<history.size(); i++)
    {
        if (recent == history.at(i)) matches++;
    }
    if (matches < 2) {history.push_back(recent); return false;}
    else return true;
}


Move bestMove(Chessboard &original)
{
    
    //Chessboard duplicate(original);
    //typedef original.allMoves():: moves;
    
    //if (duplicate.validMoveA("0-0", 5))
    //{
        
      //  if (duplicate.checkmate()) return Move("0-0", 5);
    //}
    //Chessboard duplicate2(original);
    //if (duplicate2.validMoveA("0-0-0", 5))
    //{
     //   if (duplicate2.checkmate()) return Move("0-0-0", 5);
    //}
    
    for (int i=0; i<original.allMoves().size(); i++)
    {
        
        Chessboard duplicate1(original);
        if (duplicate1.validMoveA(original.allMoves()[i].s, original.allMoves()[i].n))
        { if (duplicate1.checkmate()) { return original.allMoves()[i];}}
    }
    return Move();
}


bool Mate(int n, Chessboard &original) //to be called when n-1 has failed
{
    if (n==1)
    {
        vector <Move> options;
        options = original.allMoves();
        
        for (int i=0; i<options.size(); i++)
        {
            Chessboard duplicate1(original);
            if (duplicate1.validMoveA(options[i].s, options[i].n))
            { if (duplicate1.checkmate()) {  original.validMoveA(options[i].s, options[i].n); return true;}}
        }
        return false;
    }
    
    for (int pre = 1; pre < n; pre++)
    {
        if (Mate(pre, original)) return true;
    }
    
    vector <Move> ogoptions;
    ogoptions = original.allMoves();
    
    for (int i=0; i<ogoptions.size(); i++)
    {
        
        
        Chessboard duplicate1(original);
        if (duplicate1.validMoveA(ogoptions[i].s, ogoptions[i].n))
        {
            bool response = true;
            vector <Move> duoptions;
            duoptions = duplicate1.allMoves();
            for (int i = 0; i < duoptions.size() && response; i++)
            {
                Chessboard duplicate2(duplicate1);
                if (duplicate2.validMoveA(duoptions[i].s, duoptions[i].n))
                {
                    response = Mate(n-1, duplicate2);
                }
            }
            if (response && duoptions.size() != 0) { original.validMoveA(ogoptions[i].s, ogoptions[i].n); return true;}
        }
    }
    return false;
}

void forceMate(Chessboard &original)
{
    int doom = 1;
    while (!(Mate(doom, original)) && doom < 6)
    {
        cout<<doom;
        doom++;
    }
    
    cout<<doom<<endl;
 
    
}



int main()
{
    cout<<"Default board(1) or set up position(2) or findMate(3)? ";
    int response;
    cin>>response;
    if (response == 1)
    {
        Chessboard chessboard;
        vector <Toyboard> history;
        Toyboard starting(chessboard); history.push_back(starting);
        for (int i=1; i<200; i++)
        {
            string s;
            cin>>s;
            if (chessboard.validMove(s))
            {
                Toyboard recent(chessboard);
                if (drawByRep(recent, history))
                {
                    cout<<"Draw by threefold repetition."<<endl; return 0;
                }
            }
            chessboard.print();
            cout<<chessboard.turn<<endl;
            if (chessboard.checkmate()) {cout<<"Checkmate!"<<endl; return 0;}
            if (chessboard.stalemate()) {cout<<"Stalemate!"<<endl; return 0;}
            if (chessboard.dead()) {cout<<"Draw by 50 move rule."<<endl; return 0;}
        }
    }
    else if (response == 2)
    {
        cout<<"How many chessmen? ";
        int reply;
        cin>>reply;
        Chessboard chessboard(reply);
        
        vector <Toyboard> history;
        Toyboard starting(chessboard); history.push_back(starting);
    
    
        for (int i=1; i<200; i++)
        {
            string s;
            cin>>s;
            if (chessboard.validMove(s))
            {
                Toyboard recent(chessboard);
                if (drawByRep(recent, history))
                {
                    cout<<"Draw by threefold repetition."<<endl; return 0;
                }
            }
            chessboard.print();
            cout<<chessboard.turn<<endl;
            if (chessboard.checkmate()) {cout<<"Checkmate!"<<endl; return 0;}
            if (chessboard.stalemate()) {cout<<"Stalemate!"<<endl; return 0;}
            if (chessboard.dead()) {cout<<"Draw by 50 move rule."<<endl; return 0;}
        }
    }
    
    else
    {
        cout<<"How many chessmen? ";
        int reply;
        cin>>reply;
        Chessboard chessboard(reply);
        
        for (int i=1; i<31; i++)
        {
        
            //vector <Move> moves = chessboard.allMoves();
        
            //int n = moves.size();
            forceMate(chessboard);
        
            
            chessboard.print();
            cout<<chessboard.turn<<endl;
            if (chessboard.checkmate()) {cout<<"Checkmate!"<<endl; return 0;}
            if (chessboard.stalemate()) {cout<<"Stalemate!"<<endl; return 0;}
            string response;
            cin>>response;
            while (!chessboard.validMove(response))
            {
                chessboard.print(); cout<<chessboard.turn<<endl; cin>>response;
            }
            
                
            
        }
    }
    
}
