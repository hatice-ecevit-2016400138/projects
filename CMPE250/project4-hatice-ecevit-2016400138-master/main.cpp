#include <sstream>
#include <iomanip>
#include <iterator>
#include "graph.h"
using namespace std;
template <class Container>
void split1(const string& str, Container& cont)
{
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
};
int main(int argc, char* argv[]){
    Graph* g=new Graph();
    g->readGraph(argv[1]);

    ofstream funcOutput(argv[3]);
    ofstream derivOutput(argv[4]);
    if(g->isCyclic()){
        funcOutput<<"ERROR: COMPUTATION GRAPH HAS CYCLE!"<<endl;
        derivOutput<<"ERROR: COMPUTATION GRAPH HAS CYCLE!"<<endl;
        return 0;
    }
    ifstream myFile(argv[2]);

    string line="";
    getline(myFile, line);
    vector<string> words;
    split1(line,words);
    vector<double> h;
    funcOutput<<setprecision(16);
    int size=g->getSizeInputNodes();
    funcOutput<<g->getLastNode()<<endl;

    derivOutput<<setprecision(16);
    derivOutput<<fixed;


    double d;
    vector<string> names=g->getName();
    for(int i=0;i<names.size();i++){
        derivOutput<<"d"<<g->getLastNode()<<"/"<<"d"<<names[i]<<" ";
    }
    derivOutput<<endl;
    while(!myFile.eof()){

        Graph* g2=new Graph();
        g2->readGraph(argv[1]);
        h.clear();
        // cout<<"hi there"<<endl;
        for(int i=0;i<size;i++){
            myFile>>d;
            // cout<<d<<endl;
            h.push_back(d);
        }
        if(myFile.eof())
            break;
        double res = g2->forwardPass(h);
        // cout<<res<<endl;
        funcOutput<<res<<endl;

        vector<double> h=g2->backwardPass();
        for(int i=0;i<h.size();i++){
            derivOutput<<h[i]<<" ";
        }
        derivOutput<<endl;
    }

}