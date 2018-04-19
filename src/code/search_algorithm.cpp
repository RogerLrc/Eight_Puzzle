#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cmath>
#include <set>
#include <queue>
#include <time.h>
#include <algorithm>
#include <unordered_set>
#include "widget.h"
#include "search_algorithm.h"

using namespace std;
struct node;

class State{ //start from 0
public:
    State():table(3,vector<int>(3,0)){
        int num;
        for(int i =0;i !=3; i++){
            for(int j = 0; j !=3; j++){
                cin >> num;
                table[i][j] = num;
                if(num == 0){
                    blank_loc.first =j;
                    blank_loc.second = i;
                }

            }
        }
    }
    State(const vector<vector<int> >& v):table(3,vector<int>(3,0)){
        int num;
        for(int i =0;i !=3; i++){
            for(int j = 0; j !=3; j++){
                num = v[i][j];
                table[i][j] = num;
                if(num == 0){
                    blank_loc.first =j;
                    blank_loc.second = i;
                }

            }
        }
    }

    void move(Direction op){
        switch(op){
            case UP:
                table[blank_loc.second][blank_loc.first] = table[blank_loc.second - 1][blank_loc.first];
                blank_loc.second--;
                table[blank_loc.second][blank_loc.first] = 0;
                break;
            case DOWN:
                table[blank_loc.second][blank_loc.first] = table[blank_loc.second + 1][blank_loc.first];
                blank_loc.second++;
                table[blank_loc.second][blank_loc.first] = 0;
                break;
            case LEFT:
                table[blank_loc.second][blank_loc.first] = table[blank_loc.second][blank_loc.first - 1];
                blank_loc.first--;
                table[blank_loc.second][blank_loc.first] = 0;
                break;
            case RIGHT:
                table[blank_loc.second][blank_loc.first] = table[blank_loc.second][blank_loc.first + 1];
                blank_loc.first++;
                table[blank_loc.second][blank_loc.first] = 0;
                break;
        }
    }
    int heu_fun(){
        int result = 0;
        for(int i = 0;i != 3;i++){
            for(int j = 0; j != 3; j++){

                    result += abs(i - table[i][j] / 3) + abs(j - table[i][j] % 3);

            }
        }
        return result;
    }

    int x(){return blank_loc.first;}
    int y(){return blank_loc.second;}

    vector<vector<int> > table;
private:

    pair<int,int> blank_loc;
};
struct node{
    State s;
    vector<Direction> path;
    int g;
    int h;
    int f;
    node():s(),path(){}
    node(State& ori):s(ori),path(){
        g = 0;
        f = h = s.heu_fun();
    }
    node(const node& ori,Direction op):path(ori.path),s(ori.s){
        path.push_back(op);
        g = ori.g + 1;
        s.move(op);
        h = s.heu_fun();
        f = g + h;
    }
    bool operator<(const node& n1)const{
        return f < n1.f;
    }
    bool operator==(const node& n1)const{
        for(int i = 0; i != 3; i++){
            for(int j = 0; j != 3; j++)
                if(s.table[i][j] != n1.s.table[i][j])
                    return false;
        }
        return true;
    }
    friend bool check(vector<node>& found, node& n){
        for(int i = found.size() - 1; i != -1; i--)
            if(found[i] == n)
                return true;
        return false;
    }
    struct myHash{
        size_t operator()(const node& n)const{
            int val = 0;
            for(int i = 0; i != 3; i++){
                for(int j = 0; j != 3; j++){
                    val += pow(2,3*i + j) * n.s.table[i][j];
                }
            }
            return static_cast<size_t>(val);
        }
    };
    void generate(multiset<node>& q,const unordered_set<node,node::myHash>& found){
        int x = s.x();
        int y = s.y();
       // node cur;
        if(x == 0 ){
            if(y == 0){
                node cur1(*this,RIGHT);
                if(found.count(cur1) == 0)
                    q.insert(cur1);
                node cur2(*this,DOWN);
                if(found.count(cur2) == 0)
                    q.insert(cur2);
            }
            else if(y == 1){
                node cur1 (*this,RIGHT);
                if(found.count(cur1) == 0)
                    q.insert(cur1);
                node cur2(*this,DOWN);
                if(found.count(cur2) == 0)
                    q.insert(cur2);
                node cur3(*this,UP);
                if(found.count(cur3) == 0)
                    q.insert(cur3);
            }
            else{
                node cur(*this,RIGHT);
                if(found.count(cur) == 0)
                    q.insert(cur);
                node cur1(*this,UP);
                if(found.count(cur1) == 0)
                    q.insert(cur1);
            }
        }
        else if (x == 1){
            if(y == 0){
                node cur(*this,RIGHT);
                if(found.count(cur) == 0)
                    q.insert(cur);
                node cur1(*this,DOWN);
                if(found.count(cur1) == 0)
                    q.insert(cur1);
                node cur2(*this,LEFT);
                if(found.count(cur2) == 0)
                    q.insert(cur2);
            }
            else if(y == 1){
                node cur(*this,RIGHT);
                if(found.count(cur) == 0)
                    q.insert(cur);
                node cur1(*this,DOWN);
                if(found.count(cur1) == 0)
                    q.insert(cur1);
                node cur2(*this,LEFT);
                if(found.count(cur2) == 0)
                    q.insert(cur2);
                node cur3(*this,UP);
                if(found.count(cur3) == 0)
                    q.insert(cur3);
            }
            else{
                node cur(*this,UP);
                if(found.count(cur) == 0)
                    q.insert(cur);
                node cur1(*this,LEFT);
                if(found.count(cur1) == 0)
                    q.insert(cur1);
                node cur2(*this,RIGHT);
                if(found.count(cur2) == 0)
                    q.insert(cur2);
            }
        }
        else if(x == 2){
            if(y == 0){
                node cur(*this,DOWN);
                if(found.count(cur) == 0)
                    q.insert(cur);
                node cur1(*this,LEFT);
                if(found.count(cur1) == 0)
                    q.insert(cur1);

            }
            else if(y == 1){
                node cur(*this,DOWN);
                if(found.count(cur) == 0)
                    q.insert(cur);
                node cur1(*this,LEFT);
                if(found.count(cur1) == 0)
                    q.insert(cur1);
                node cur2(*this,UP);
                if(found.count(cur2) == 0)
                    q.insert(cur2);
            }
            else{
                node cur(*this,UP);
                if(found.count(cur) == 0)
                    q.insert(cur);
                node cur1(*this,LEFT);
                if(found.count(cur1) == 0)
                    q.insert(cur1);
            }
        }
    }
};

vector<Direction> solve(vector<vector<int> >&v){
    State s(v);
    clock_t start, end;
    start = clock();
    unordered_set<node,node::myHash> q;
    node cur(s);

    multiset<node> frontier;    //set require each element is unique,so multiset seems better
    q.insert(cur);
    while(cur.h != 0){
        cur.generate(frontier,q);
        cur = *(frontier.begin());
        frontier.erase(frontier.begin());
        q.insert(cur);
    }
    end = clock();
    cout << "time: " << (double)(end - start) / CLOCKS_PER_SEC << endl;
  /*  for(int i = 0;i != 3; i++){
        for(int j = 0; j != 3; j++){
            cout << cur.s.table[i][j] << ' ';
        }
        cout << endl;

    }*/
    cout << "path length: " << cur.path.size() << endl;
    return cur.path;

}

