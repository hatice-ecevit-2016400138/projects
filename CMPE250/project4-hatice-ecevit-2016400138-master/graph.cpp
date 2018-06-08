#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "graph.h"
#include "variable.h"
#include "function.h"
#include <queue>
#include <sstream>
#include <iterator>
#include <cmath>

using namespace std;
template <class Container>
void split1(const string& str, Container& cont)
{
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
};
// helper function that checks whether the given string is number or not.
bool isNumber(const string& s)
{
    char* end = 0;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && val != HUGE_VAL;
}

int Graph::getVariable(string inp){
    int res;
    if( isNumber(inp) ){
        double val = stod(inp.c_str());
        idCount++;
        name[idCount] = inp;
        vars[idCount] = new Variable(idCount, inp, val);
        res = idCount;
        type[idCount] = VARIABLE;
    }
    else{
        if(id.find(inp)==id.end()){
            idCount++;
            id[inp] = idCount;
            name[idCount] = inp;
            vars[idCount] = new Variable(idCount, inp);
            res = idCount;
            type[idCount] = VARIABLE;
        }
        else{
            res = id[inp];
        }
    }
    // printf("%s -> %d\n",inp.c_str(), res);
    return res;
}

int Graph::getFunction(string fnc){
    idCount++;
    name[idCount] = fnc;
    type[idCount] = FUNCTION;
    Function *f;
    // cout << "new Function: " << fnc << endl;
    if(fnc.compare("mult")==0)
        f = new Multiplication(idCount, fnc);
    else if(fnc.compare("add")==0)
        f = new Addition(idCount, fnc);
    else if(fnc.compare("subs")==0)
        f = new Subtraction(idCount, fnc);
    else if(fnc.compare("divide")==0)
        f = new Division(idCount, fnc);

    else if(fnc.compare("sin")==0)
        f = new Sine(idCount, fnc);
    else if(fnc.compare("cos")==0)
        f = new Cosine(idCount, fnc);
    else if(fnc.compare("identity")==0)
        f = new Identity(idCount, fnc);
    else if(fnc.compare("tan")==0)
        f = new Tangent(idCount, fnc);
    else if(fnc.compare("acos")==0)
        f = new ArcCosine(idCount, fnc);
    else if(fnc.compare("asin")==0)
        f = new ArcSine(idCount, fnc);
    else if(fnc.compare("atan")==0)
        f = new ArcTangent(idCount, fnc);
    else if(fnc.compare("exp")==0)
        f = new Exponential(idCount, fnc);
    else if(fnc.compare("log")==0)
        f = new Log(idCount, fnc);
    else if(fnc.compare("log10")==0)
        f = new Log10(idCount, fnc);
    else if(fnc.compare("pow")==0)
        f = new Power(idCount, fnc);
    else if(fnc.compare("sqrt")==0)
        f = new Sqrt(idCount, fnc);
    fncs[idCount] = f;
    return idCount;
}
void Graph:: addAssignment(vector<string> assingment){
    if(assingment.size()==3){
        addUnaryFunction("identity",assingment[2],assingment[0]);
    }
    else{
        if(assingment[2].compare("mult")==0||assingment[2].compare("add")==0||assingment[2].compare("subs")==0||assingment[2].compare("divide")==0||assingment[2].compare("pow")==0){
            addBinaryFunction(assingment[2],assingment[3],assingment[4],assingment[0]);
        }
        else{
            addUnaryFunction(assingment[2],assingment[3],assingment[0]);
        }

    }
}


void Graph::addUnaryFunction(string fnc, string inp, string out){

    int fId = getFunction(fnc);
    int inpId = getVariable(inp);
    int outId = getVariable(out);
    fncs[fId]->addInput(vars[inpId]);
    fncs[fId]->setOutput(vars[outId]);

    vars[inpId]->addTo(fncs[fId]);
    vars[outId]->setFrom(fncs[fId]);
}

void Graph::addBinaryFunction(string fnc, string inp1, string inp2, string out){
    int fId= getFunction(fnc);
    int inpId1=getVariable(inp1);
    int inpId2=getVariable(inp2);
    int outId=getVariable(out);
    fncs[fId]->addInput(vars[inpId1]);
    fncs[fId]->addInput(vars[inpId2]);
    fncs[fId]->setOutput(vars[outId]);

    vars[inpId1]->addTo(fncs[fId]);
    vars[inpId2]->addTo(fncs[fId]);
    vars[outId]->setFrom(fncs[fId]);
}

void Graph::readGraph(string fileName){
    ifstream infile(fileName);
    string line="";
    do{
        line="";
        getline(infile, line);

        vector<string> words;

        split1(line,words);


        if(words.size()!=0){
            if(words[0].compare("input")==0){
                int id=getVariable(words[1]);
                this->inputNodes.push_back(id);

            }
            else if(words[0].compare("output")==0){
                int id=getVariable(words[1]);
                outputNode=id;
            }
            else{
                addAssignment(words);
            }
        }

    }while(line.length()>0);

}

void Graph::initTopologicalOrder(queue<Node *> &q, int *incSizes){


}

bool Graph::isCyclic(){
    queue<Node *> q;
    //cout<<inputNodes.size();
    /* for(int i=0;i<this->inputNodes.size();i++){
         q.push((Node*)this->vars[inputNodes[i]]);

     }*/
    for(int i=0;i<this->vars.size();i++){
        if(vars[i]!= nullptr && vars[i]->incomings2==0)
            q.push((Node*)vars[i]);
    }

    Node *tmp;
//cout<<q.size()<<endl;


    while(!q.empty()){
        tmp=q.front();
        q.pop();
        //cout<<tmp->name<<endl;
        for(int i=0;i<tmp->getOutgoings().size();i++){
            tmp->getOutgoings()[i]->incomings--;
            if(tmp->getOutgoings()[i]->incomings==0)
                q.push(tmp->getOutgoings()[i]);
            //cout<<tmp->getOutgoings()[i]->name<<endl;

        }

        //cout<<"hi there"<<endl;
    }

    return !((tmp->name.compare(this->vars[this->outputNode]->name))==0);
}

double Graph::forwardPass(vector<double> inputValues){
    queue<Variable *> q;
    for(int i=0;i<inputValues.size();i++){
        this->vars[inputNodes[i]]->value=inputValues[i];
        //q.push(vars[inputNodes[i]]);
        //cout<<vars[inputNodes[i]]->value<<endl;
    }
    for(int i=0;i<this->vars.size();i++){
        if(vars[i]!= nullptr && vars[i]->incomings2==0)
            q.push(vars[i]);
    }

    /*for(int i=1;i<=this->vars.size();i++){
        if(vars[i]!= nullptr && this->vars[i]->incomings2==0 && this->vars[i]->value!=0){
            q.push(vars[i]);
            cout<<"value is"<<vars[i]->value<<endl;
        }
    }*/
    //cout<<q.size()<<endl;
    Variable* tmp;
    //cout<<"hat"<<endl;

    while(q.size()!=0) {
        tmp = q.front();
        // cout<<tmp->value<<endl;
        q.pop();
        for(int i=0;i<tmp->to.size();i++){
            tmp->to[i]->doForward();
            tmp->to[i]->incomings2--;

            if(tmp->to[i]->incomings2==0){
                q.push(tmp->to[i]->output);
                tmp->to[i]->output->incomings2--;
                this->reversed.push(tmp->to[i]);
            }
        }
        /* if(q.empty())
             return tmp->value;*/

    }

    return this->vars[outputNode]->value;

}

vector<double> Graph::backwardPass(){
    vars[outputNode]->derivative=1;
    while(!reversed.empty()){
        Function* tmp=reversed.top();
        reversed.pop();
        tmp->doBackward();
    }
    vector<double > derivatived;
    for(int i=0;i<this->inputNodes.size();i++){
        derivatived.push_back(this->vars[inputNodes[i]]->derivative);
    }
    return derivatived;
}
int Graph::getSizeInputNodes(){
    return this->inputNodes.size();
}
string Graph::getLastNode() {
    return this->vars[this->outputNode]->name;
}
vector<string> Graph::getName() {
    vector<string> names;
    for(int i=0;i<this->inputNodes.size();i++){
        names.push_back(this->name[inputNodes[i]]);
    }
    return names;
}
