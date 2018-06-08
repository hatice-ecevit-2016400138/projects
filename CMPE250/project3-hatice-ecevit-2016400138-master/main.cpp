#include <iostream>
#include <sstream>
#include <fstream>
#include <queue>
#include "vector"
#include <map>
#include <iterator>

using namespace std;
class Edge{
public:
    int A;
    int B;
    int time;
    vector<int> thieves;
    Edge(int a, int b, int time_){
        this->A=a;
        this->B=b;
        this->time=time_;
    }
};
class Vertex{
public:
    int num;
    vector<Edge> edges;
    int coin[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};
};

class Traveller{
public:
    int A;
    int time;
    vector<string> path;
    int coins[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};
    map<vector<int>,int> roads;
    Traveller(int a, int time_, vector<string> path_){
        this->A=a;
        this->time=time_;
        this->path.swap(path_);

    }

};
struct CompareToTime{
    bool operator()(Traveller& tr1, Traveller& tr2){
        return tr1.time>tr2.time;
    }
};
template <class Container>
void split1(const string& str, Container& cont)
{
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
};
bool arrayEquality(int x[] , int y[]){
    bool equal=true;
    for(int i=0;i<13;i++){
        if(x[i]!=y[i])
            equal=false;
    }
    return equal;
}
int main(int argc, char* argv[]) {
    // below reads the input file
    // in your next projects, you will implement that part as well
    /*if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }*/
    //cout << "output file: " << argv[2] << endl;
    ifstream infile(argv[1]);
    string line="";
    // process first line
    getline(infile, line);
    vector<string> words;
    split1(line,words);
    int N;//sehir
    int M;//yol
    int P;//thief
    int K;//mucevher;
    N=stoi(words[0]);
    M=stoi(words[1]);
    P=stoi(words[2]);
    K=stoi(words[3]);
    Vertex cities[N];
    for(int i=0;i<K;i++){
        getline(infile, line);
        vector<string> words2;
        split1(line,words2);
        int W=stoi(words2[0]);
        int x=stoi(words2[1]);
        for(int j=0; j<x;j++){
            int a=stoi(words2[j+2]);
            cities[W-1].coin[a-1]=1;
        }
    }
    for(int i=0;i<M;i++){
        getline(infile, line);
        vector<string> words3;
        split1(line, words3);
        int a=stoi(words3[0]);
        a=a-1;
        int b=stoi(words3[1]);
        b=b-1;
        int time=stoi(words3[2]);
        int thieves=stoi(words3[3]);
        Edge e(a, b, time);


        for(int j=0;j<thieves;j++){
            int thief=stoi(words3[j+4]);
            e.thieves.push_back(thief);

        }
        cities[a].edges.push_back(e);
        cities[b].edges.push_back(e);
    }
    priority_queue<Traveller, vector<Traveller>, CompareToTime> states ;
    vector<string> tmp={"1"};
    Traveller t(0, 0, tmp);

    for(int i=0;i<13;i++){
        t.coins[i]=cities[0].coin[i];
    }
    states.push(t);
    vector<string> path;
    while(!states.empty()){
        Traveller temp=states.top();
        states.pop();
        int city = temp.A;
        if(city == N-1){
            path = temp.path;
            break;
        }

        for(int i=0;i<cities[city].edges.size();i++){
            Edge e=cities[city].edges[i];
            int b=e.B;
            if(b == city)
                b=e.A;
            bool canGo=true;
            for(int j=0;j<e.thieves.size();j++){
                if(temp.coins[e.thieves[j]-1]==0){
                    canGo=false;
                }
            }
            vector<int> edge={city,b};
            if(canGo && temp.roads.count(edge)==0){
                temp.roads.insert(pair<vector<int>,int>(edge, 1));
                Traveller temp2(b, temp.time+e.time, temp.path);
                temp2.path.push_back(to_string(b+1));
                temp2.roads.insert(temp.roads.begin(),temp.roads.end());
                for(int j=0;j<13;j++){
                    if(cities[b].coin[j]==1&&temp.coins[j]==0){
                        temp2.coins[j]=1;
                        temp2.roads.clear();
                    }
                    if(temp.coins[j]==1){
                        temp2.coins[j]=1;
                    }
                }

                states.push(temp2);
            }
        }

    }
    ofstream myFile(argv[2]);
    if(path.size()==0)
        myFile<<"-1";
    else {
        for (int i = 0; i<path.size();i++){
            myFile<<path[i]+" ";
        }
    }
    return 0;
}