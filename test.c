#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "drawfunctions.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "defaultfunctions.h"

int intHash(HashSet *set, void *add){
    //array of primes for each value in the Boundary struct
    int prime = 23;
    //initial prime to seed function 
    int seed = 11;
    //combines all the values into one hash mod the size 
    int hash = (seed * (prime + *(int*)add)) % set->size;

    return hash;
}

bool intEquality(void *num1, void *num2){
    if(num1==NULL||num2==NULL){
        return false;
    }
    return *(int*)num1==*(int*)num2;
}

int main(int argc, char *argv[]){
    LinkedList *test=createLinkedList();
    int num=5;
    printf("%d len\n", test->length);
    int *nump=&(int){5};
    test=LinkedListadd(test, nump);
    nump=&(int){6};
    test=LinkedListadd(test, nump);
    nump=&(int){10};
    test=LinkedListadd(test, nump);
    nump=&(int){50};
    test=LinkedListadd(test, nump);
    printf("length %d\n", test->length);
    while(test!=NULL){
        printf("pop: %d\n",*(int*)LinkedListpop(&test));
    }
    // int *temp=(int*)LinkedListpop(&test);
    // printf("hi1\n");
    // printf("%d\n", *temp);
    // temp=(int*)LinkedListpop(&test);
    // printf("hi2\n");
    // printf("%d\n", *temp);
    // temp=(int*)LinkedListpop(&test);
    // printf("hi3\n");
    // printf("%d\n", *temp);
    // temp=(int*)LinkedListpop(&test);
    // printf("hi4\n");
    // printf("%d\n", *temp);
    // temp=(int*)LinkedListpop(&test);
    // printf("hi5\n");
    // printf("%d\n", *temp==NULL);

    HashSet *hash = createHashSet();
    HashSetadd(hash, &(int){5}, intHash);
    HashSetadd(hash, &(int){15}, intHash);
    HashSetadd(hash, &(int){556}, intHash);
    HashSetadd(hash, &(int){84865}, intHash);
    HashSetadd(hash, &(int){546546}, intHash);
    HashSetadd(hash, &(int){51646068}, intHash);
    printf("TESTING 11111 !!!!!!!!! \n");
    printf("contain %d\n",HashSetcontains(hash, &(int){5}, intHash, intEquality));
    printf("contain %d\n",HashSetcontains(hash, &(int){15}, intHash, intEquality));
    printf("contain %d\n",HashSetcontains(hash, &(int){556}, intHash, intEquality));
    printf("contain %d\n",HashSetcontains(hash, &(int){84865}, intHash, intEquality));
    printf("contain %d\n",HashSetcontains(hash, &(int){546546}, intHash, intEquality));
    printf("contain %d\n",HashSetcontains(hash, &(int){51646068}, intHash, intEquality));
    resizeHash(hash, intHash);
    printf("TESTING !!!!!!!!! \n");
    printf("contain %d\n",HashSetcontains(hash, &(int){5}, intHash, intEquality));
    printf("contain %d\n",HashSetcontains(hash, &(int){15}, intHash, intEquality));
    printf("contain %d\n",HashSetcontains(hash, &(int){556}, intHash, intEquality));
    printf("contain %d\n",HashSetcontains(hash, &(int){84865}, intHash, intEquality));
    printf("contain %d\n",HashSetcontains(hash, &(int){546546}, intHash, intEquality));
    printf("contain %d\n",HashSetcontains(hash, &(int){51646068}, intHash, intEquality));

    BinaryTreeNode *bst = CreateBinaryTree(&(int){10});
    printf("%d\n", *(int*)bst->value);
}

