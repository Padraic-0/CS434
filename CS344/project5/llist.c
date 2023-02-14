//created by: Padraic Bergin
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "llist.h"

struct node *node_alloc(int value){
  struct node* ptr;
  ptr = (struct node*) malloc(sizeof(struct node));
  ptr->value = value;
  ptr->next = NULL;
  return ptr;
}

void node_free(struct node *n){
  free(n);
}

void llist_insert_head(struct node **head, struct node *n){
  if (head == NULL){
    head = &n;
  }else{
  n->next = *head;
  *head = n;
  }
}

void llist_print(struct node *head){
  struct node* runner;
  runner = head;
  printf("Linked List: ");
  if (runner == NULL){
    printf("[EMPTY]");
  }else{
    while(runner != NULL){
      printf("%d",runner->value);
      runner = runner->next;
      if (runner != NULL){
        printf(" -> ");
      }
    }
  }
  printf("\n");
  
}

void llist_insert_tail(struct node **head, struct node *n){
  struct node* runner;
  runner = *head;
  if (runner == NULL){
    *head = n;
  }else{
    while(runner->next != NULL){
      runner = runner->next;
    }
    runner->next = n;
  }
}

struct node *llist_delete_head(struct node **head){
  fflush(stdout);
  if(*head != NULL){
    struct node* tmp;
    tmp = *head;
    if (tmp->next == NULL){
      *head = NULL;
    }else{
    *head = tmp->next;
    }
    return tmp; 
  }else{
    return NULL;
  }
}

void llist_free(struct node **head){
  struct node* free;
  struct node* next;
  free = *head;
  while(free->next != NULL){
    next = free->next;
    node_free(free);
    free = next;
  }
}

int main(int argc, char *argv[]){
  struct node** head = malloc(sizeof(struct node*));
  *head = NULL;
  int value;
  int freed;
  char* command;
  for (int i = 1; i < argc; i++){
    freed = 1;
    struct node* ptr;
    command = argv[i];
    if (strcmp(command,"ih") == 0){
      i++;
      if ((value = atoi(argv[i])) == 0){
        ptr = node_alloc(value);
        llist_insert_head(head,ptr);
      }
    }
    else if (strcmp(command,"it") == 0){
      i++;
      if ((value = atoi(argv[i])) != 0){
        ptr = node_alloc(value);
        llist_insert_tail(head,ptr);
      }
    }
    else if (strcmp(command,"dh") == 0){
      ptr = llist_delete_head(head);
      node_free(ptr);
    }
    else if (strcmp(command,"f") == 0){
      if (*head != NULL){
        
        llist_free(head);
        *head = (struct node*) NULL;
        freed = 0;
      }
    }
    else if (strcmp(command,"p") == 0){
      llist_print(*head);
    }
    else{
      printf("error with format %s : %s", argv[i], argv[i+1]);
      break;
    }

  }
  if (freed == 1){
    if (*head != NULL){
      llist_free(head);
    }
  }
  return 0;
}