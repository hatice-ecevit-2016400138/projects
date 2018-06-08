#include "function.h"
#include <math.h>
using namespace std;

Function::Function(int _id, string _name) {
    this->id=_id;
    this->name=_name;
    this->output= new Variable();
    this->outgoings=1;
    this->incomings=0;
}
Function::Function() {
    this->output= new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Function::setOutput(Variable *_output) {
    this->output=_output;
    this->outgoings2=1;
    this->outgoings=1;
}
void Function::addInput(Variable *input) {
    this->inputs.push_back(input);
    this->incomings++;
    this->incomings2++;
}
vector<Node *> Function::getIncomings() {
    vector<Node*> returned;
    for(int i=0;i<this->inputs.size();i++){
        returned.push_back((Node *)inputs[i]);
    }
    return returned;
}
vector<Node *> Function::getOutgoings() {
    vector <Node *> returned;
    returned.push_back((Node *) this->output);
    return returned;
}
/*Function::~Function() {
    delete this->output;
    for (int i =0; i< this->inputs.size();i++)
    {
        delete (this->inputs[i]);
    }
    this->inputs.clear();


}*/
Addition::Addition(int _id, string _name) {
    this->id=_id;
    this->name=_name;
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
    this->output=new Variable();
}
void Addition::doForward() {
   this->output->value=this->inputs[0]->value+this->inputs[1]->value;
}
void Addition::doBackward() {
    this->inputs[0]->derivative+=this->output->derivative;
    this->inputs[1]->derivative+=this->output->derivative;
}
Multiplication::Multiplication(int _id, string _name) {
    this->name=_name;
    this->id=_id;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Multiplication::doForward() {
    this->output->value=this->inputs[0]->value*this->inputs[1]->value;
}
void Multiplication::doBackward() {
 this->inputs[0]->derivative+=this->inputs[1]->value*this->output->derivative;
    this->inputs[1]->derivative+=this->inputs[0]->value*this->output->derivative;
}
Sine::Sine(int _id, string _name) {
    this->id=_id;
    this->name=_name;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Sine::doForward() {
    this->output->value=sin(this->inputs[0]->value);
}
void Sine::doBackward() {
    this->inputs[0]->derivative+=this->output[0].derivative*cos(this->inputs[0]->value);
}
Cosine::Cosine(int _id, string _name) {
    this->id=_id;
    this->name=_name;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Cosine::doForward() {
    this->output->value=cos(this->inputs[0]->value);
}
void Cosine::doBackward() {
    this->inputs[0]->derivative+=-1*this->output[0].derivative*sin(this->inputs[0]->value);
}
Identity::Identity(int _id, string _name) {
    this->id=_id;
    this->name=_name;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Identity::doForward() {
    this->output->value=this->inputs[0]->value;

}
void Identity::doBackward() {
this->inputs[0]->derivative+=this->output->derivative;
}
Tangent::Tangent(int _id, string _name) {
    this->id=_id;
    this->name=_name;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Tangent::doForward() {
    this->output->value=tan(this->inputs[0]->value);
}
void Tangent::doBackward() {
 this->inputs[0]->derivative+=this->output->derivative*(1/cos(this->inputs[0]->value))*(1/cos(this->inputs[0]->value));
}
ArcCosine::ArcCosine(int _id, string _name) {
    this->name=_name;
    this->id=_id;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void ArcCosine::doForward() {
    this->output->value=acos(this->inputs[0]->value);
}
void ArcCosine::doBackward() {
    this->inputs[0]->derivative+=-1*this->output->derivative*(1/sqrt(1-(this->inputs[0]->value*this->inputs[0]->value)));
}
ArcSine::ArcSine(int _id, string _name) {
    this->name=_name;
    this->id=_id;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void ArcSine::doForward() {
    this->output->value=asin(this->inputs[0]->value);
}
void ArcSine::doBackward() {
this->inputs[0]->derivative+=this->output->derivative*(1/sqrt(1-(this->inputs[0]->value*this->inputs[0]->value)));
}
ArcTangent::ArcTangent(int _id, string _name) {
    this->name=_name;
    this->id=_id;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void ArcTangent::doForward() {
    this->output->value=atan(this->inputs[0]->value);
}
void ArcTangent::doBackward() {
 this->inputs[0]->derivative+=this->output->derivative*(1/(1+(this->inputs[0]->value*this->inputs[0]->value)));
}
Exponential::Exponential(int _id, string _name) {
    this->name=_name;
    this->id=_id;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Exponential::doForward() {
    this->output->value=exp(this->inputs[0]->value);
}
void Exponential::doBackward() {
    this->inputs[0]->derivative+=this->output->derivative*exp(this->inputs[0]->value);
}
Log::Log(int _id, string _name) {
    this->name=_name;
    this->id=_id;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Log::doForward() {
    this->output->value=log(this->inputs[0]->value);
}
void Log::doBackward() {
this->inputs[0]->derivative+=this->output->derivative/this->inputs[0]->value;
}
Log10::Log10(int _id, string _name) {
    this->name=_name;
    this->id=_id;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Log10::doForward() {
    this->output->value=log10(this->inputs[0]->value);
}
void Log10::doBackward() {
 this->inputs[0]->derivative+=this->output->derivative*(1/this->inputs[0]->value)*(1/log(10));
}
Power::Power(int _id, string _name) {
    this->name=_name;
    this->id=_id;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Power::doForward() {
    this->output->value=pow(this->inputs[0]->value,this->inputs[1]->value);
}
void Power::doBackward() {
    this->inputs[0]->derivative+=this->output->derivative*this->inputs[1]->value*pow(this->inputs[0]->value,this->inputs[1]->value-1);
    this->inputs[1]->derivative+=this->output->derivative*log(this->inputs[0]->value)*pow(this->inputs[0]->value,this->inputs[1]->value);
}
Sqrt::Sqrt(int _id, string _name) {
    this->name=_name;
    this->id=_id;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Sqrt::doForward() {
    this->output->value=sqrt(this->inputs[0]->value);
}
void Sqrt::doBackward() {
 this->inputs[0]->derivative+=(this->output->derivative/sqrt(this->inputs[0]->value))/2.0;
}
Subtraction::Subtraction(int _id, string _name) {
    this->name=_name;
    this->id=_id;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
}
void Subtraction::doForward() {
    this->output->value=(this->inputs[0]->value-this->inputs[1]->value);
}
void Subtraction::doBackward() {
    this->inputs[0]->derivative+=this->output->derivative;
    this->inputs[1]->derivative+=-1*this->output->derivative;
}
Division::Division(int _id, string _name) {
    this->name=_name;
    this->id=_id;
    this->output=new Variable();
    this->outgoings=0;
    this->incomings=0;
    this->outgoings2=0;
    this->incomings2=0;
}
void Division::doForward() {
    this->output->value=(this->inputs[0]->value/this->inputs[1]->value);
}
void Division::doBackward() {
   this->inputs[0]->derivative+=this->output->derivative/this->inputs[1]->value;
    this->inputs[1]->derivative+=-1*this->output->derivative*this->inputs[0]->value/(this->inputs[1]->value*this->inputs[1]->value);
}
