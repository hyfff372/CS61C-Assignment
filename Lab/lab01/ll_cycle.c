#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* your code here */
    struct node *tortoise, *hare;
    if (!head){
        return 0;
    }
    tortoise = hare = head;
    while(hare != NULL && hare->next != NULL){
        hare = hare->next->next;
        tortoise = tortoise->next;
        if (tortoise == hare){
            return 1;
        }
    }
    return 0;
    
}