#include "variable.h"

using namespace std;

Variable::Variable() {

    //this->from= new Function();
    this->isFunction=false;
}
Variable::Variable(int _id, string _name, double _value) {
    this->id=_id;
    this->name=_name;
    this->value=_value;
    //this->from=new Function();
    this->incomings=0;
    this->outgoings=0;
    this->incomings2=0;
    this->outgoings2=0;
    this->isFunction=false;
}
void Variable::setFrom(Function *_from) {
    this->from=_from;
    this->incomings=1;
    this->incomings2=1;
}
void Variable::addTo(Function *_to) {
    this->to.push_back(_to);
    this->outgoings++;
    this->outgoings2++;
}
vector<Node *> Variable::getIncomings() {
    vector<Node *> returned;
    returned.push_back((Node *)this->from);
    return returned;
}
vector <Node *> Variable::getOutgoings() {
    vector<Node *> returned;
    for(int i=0;i<this->to.size();i++){
        returned.push_back((Node *)this->to[i]);
    }
    return returned;
}
/*Variable::~Variable() {
    delete this->from;
    for (int i =0; i< this->to.size();i++)
    {
        delete (this->to[i]);
    }
    this->to.clear();

}*/