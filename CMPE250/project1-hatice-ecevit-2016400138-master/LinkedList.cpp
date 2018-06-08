#include "LinkedList.h"

LinkedList::LinkedList() {
    //initializing an empy LinkedList
    this->length=0;
    this->head= nullptr;
    this->tail= nullptr;

}
LinkedList::LinkedList(const LinkedList &list) {
   //if given list is null, making this list null too.
    if(list.head== nullptr){
        this->length=0;
        this->head= nullptr;
        this->tail= nullptr;
        return;
    }

    this->length=list.length;
//traversing the list and declaring a new Node in each
    if(list.head!= nullptr){
        Node* tmp;
        Node* tmp2;
        this->head=new Node(*list.head);
        
        tmp=this->head;
        tmp2=list.head;
        while(tmp2!=list.tail&&tmp2!= nullptr){
            tmp->next=new Node(*tmp2->next);
            tmp=tmp->next;
            tmp2=tmp2->next;
        }
        
        this->tail=tmp;
        }

}
LinkedList& LinkedList::operator=(const LinkedList &list) {
    //basically doing the same thing as copy constructor expect deleting the first list
    delete this->head;
    this->length=list.length;

    if(list.head!= nullptr){
        Node* tmp;
        Node* tmp2;
        this->head=new Node(*list.head);
        //this->tail=new Node(*list.tail);
        tmp=this->head;
        tmp2=list.head;
        while(tmp2!=list.tail&&tmp2!= nullptr){
            tmp->next=new Node(*tmp2->next);
            tmp=tmp->next;
            tmp2=tmp2->next;
        }
        
        this->tail=tmp;
    }
    return *this;
}
void LinkedList::pushTail(Member data) {

//initializing a node with member data inside
    Node *addThis=new Node(data);
//inserting to head
    if(this->length==0){
        this->head=addThis;

       this->tail=this->head;


        this->length++;
    }
    //inserting to tail
    else{
       

        this->tail->next=addThis;

        this->tail=this->tail->next;
        this->length++;
    }


}
LinkedList::~LinkedList() {
    //deleting head will delete this list completely
    delete this->head;
   
}
