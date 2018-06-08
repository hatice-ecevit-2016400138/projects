#include "SurveyClass.h"
#include <map>

SurveyClass::SurveyClass() {
    this->members=new LinkedList();

}
SurveyClass::SurveyClass(const SurveyClass &other) {
    
    this->members=new LinkedList(*other.members);
    
}
SurveyClass::~SurveyClass() {
    if(this->members!= nullptr)
    delete this->members;
}
void SurveyClass::addMember(const Member& newMember){
    this->members->pushTail(newMember);
}
float SurveyClass::calculateAverageAge() {
   //sum is the sum of the ages of members
    float sum=0;
    //tmp is traversing the member list
    Node* tmp;
    tmp=this->members->head;
    //adding the ages of members
    for(int i=0;i<this->members->length;i++){
        sum=sum+tmp->data.age;
        tmp=tmp->next;
    }
    //dividing sum by number of members 
    float f= sum/this->members->length;
    //handling the precision
    f=(int)(f * 100 + .5) / 100.0;
   
    return f;
}
string SurveyClass::findMostFavouriteColor() {
    //map is for keeping colors, keys are the colors and values are the number of people who likes them
    map<string,int> colors;
    Node* tmp=this->members->head;
    if(this->members->length==1){
        return this->members->head->data.color;
    }
    //tmp is traversing the list and adding all colors to the map, if it already exists incrementing the value of it
    while(tmp->next!=0){
        string addColor=tmp->data.color;
        if(colors.count(addColor)==0){
            colors.insert(std::pair<string,int>(addColor,1));
        }
        else{
            colors[addColor]++;
        }
        tmp=tmp->next;
    }
    string addColor=tmp->data.color;
    if(colors.count(addColor)==0){
        colors.insert(std::pair<string,int>(addColor,1));
    }
    else{
        colors[addColor]++;
    }
    //max is the number of maximum amount of people who likes a certain color
    int max=0;
    //s is the most popular color
    string s;
    std::map<string,int>::iterator it=colors.begin();
    for (; it!=colors.end(); ++it){
        if(it->second>max){
            s=it->first;
            max=it->second;
        }
    }

return s;

}
