#include <stdbool.h>
#include "defaultfunctions.h"

//LinkedList function implementations

LinkedList *createLinkedList(){
    LinkedList *ret=(LinkedList*)malloc(sizeof(LinkedList));;
    ret->length=0;
    ret->next=NULL;
    ret->value=NULL;
    return ret;
}

LinkedList *LinkedListadd(LinkedList *list, void *value){
    if(list->length==0){
        list->length=1;
        list->value=value;
        return list;
    }
    LinkedList *ret=(LinkedList*)malloc(sizeof(LinkedList));
    ret->next=list;
    ret->value=value;
    ret->length=list->length+1;
    return ret;
}

void *LinkedListpop(LinkedList **listptr){
    LinkedList *list=*listptr;
    if(list==NULL){
        return NULL;
    }
    //stores the head's value
    void* val=list->value;
    //updates the head of the list to the next value
    *listptr=list->next;
    //frees the head of the list
    free(list); 
    return val;
}

HashSet *createHashSet(){
    HashSet *ret = (HashSet*)malloc(sizeof(HashSet));
    //defaults value to 50
    ret->set = (LinkedList**)malloc(sizeof(LinkedList*)*25);
    ret->size = 25;
    ret->length = 0;
    //sets all elements in the array to a default value
    for(int i=0;i<50;i++){
        ret->set[i]=createLinkedList();
    }
    return ret;
}

//HashSet function implementations

void HashSetadd(HashSet *set, void *add, int (*hashFunc)(HashSet*, void*)){
    if((set->length>=(set->size*7)/10)&&set->length>0){
        resizeHash(set, hashFunc);
    }
    //calls the user defines hashing function
    int hash=hashFunc(set, add);
    //insert into the linked list at that spot
    set->length=set->length+1;
    set->set[hash]=LinkedListadd(set->set[hash], add);
}

bool HashSetcontains(HashSet *set, void *item, int (*hashFunc)(HashSet*, void*), bool (*equalsFunc)(void*, void*)){
    //checks for null references
    if(set==NULL||set->set==NULL){
        return false;
    }
    //the LinkedList contained inside the HashSet
    LinkedList *temp = set->set[hashFunc(set, item)];
    //goes through the LinkedList to check for a match in the case multiple values have matched to it
    while(temp!=NULL){
        if(equalsFunc(temp->value, item)){
            return true;
        }
        temp=temp->next;
    }
    return false;
}

void resizeHash(HashSet *set, int (*hashFunc)(HashSet*, void*)){
    //puts size to an int for convenience
    int size=set->size;
    //Old array of items
    LinkedList **old=set->set;
    //new larger array
    LinkedList **ret = (LinkedList**)malloc(sizeof(LinkedList*)*(size*size));
    //initializes array
    for(int i=0;i<size*size;i++){
        ret[i]=createLinkedList();
    }
    //new HashSet of new size
    HashSet *newSize=createHashSet();
    newSize->size=size*size;
    //sets new HashSet
    newSize->set=ret;
    //goes through the original array and reinserts all values
    for(int i=0;i<size;i++){
        //if the value is null then its an empty space, should only reallocate when full but just incase
        if(old[i]->value==NULL){
            continue;
        }
        //loop through all items in the linkedlist 
        LinkedList *temp = old[i];
        while(temp!=NULL){
            HashSetadd(newSize, temp->value, hashFunc);
            //goes to the next spot
            LinkedListpop(&temp);
        }
    }
    //frees the old array
    free(*set->set);
    *set=*newSize;
}

BinaryTreeNode *CreateBinaryTree(void *val){
    BinaryTreeNode *temp = (BinaryTreeNode*)malloc(sizeof(BinaryTreeNode));
    temp->value=val;
}

void AddBST(BinaryTreeNode *node, void *value, int (*compareFunc)(void*, void*)){
    BinaryTreeNode *temp = (BinaryTreeNode*)malloc(sizeof(BinaryTreeNode));
    temp->value=value;
    if(compareFunc(node->value, value)>1){
        if(node->more==NULL){
            node->more=temp;
        }
    }
}
