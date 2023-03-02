//created by: Padraic Bergin
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))

#define PADDED_SIZE(x) ((x) + GET_PAD(x))

#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

#define REQUIRED_SPACE(padded_request_space, padded_struct_block_size) (padded_request_space + padded_struct_block_size + ALIGNMENT)

//global head
struct block *head = NULL;

struct block {
    struct block *next;
    struct block *prev;
    int size;     // Bytes
    int in_use;   // Boolean
};

void split_space(struct block *node, int size_of_block){
  int big = size_of_block + PADDED_SIZE(sizeof(struct block));
  struct block *new_block = PTR_OFFSET(node, big);
  new_block->next = node->next;
  node->next = new_block;
  int total_size = node->size;
  
  node->size = size_of_block;
  new_block->size = total_size - size_of_block;
  new_block->in_use = 0;
  new_block->prev = node;
  
}

void myfree(struct block *node){
  int size_of_block = PADDED_SIZE((sizeof(struct block)));
  node = PTR_OFFSET(node, -size_of_block);
  node->in_use = 0;
  if (node->prev != NULL){
    if (node->prev->in_use == 0){
      node->prev->size = node->size + size_of_block;
      node = node->prev;
    }
  }
  if (node->next != NULL){
    if (node->next->in_use == 0){
      node->size = node->next->size + size_of_block;
    }
  }
}

void* myalloc(int size){
  if (head == NULL){
    head = mmap(NULL, 1024, PROT_READ|PROT_WRITE,
                MAP_ANON|MAP_PRIVATE, -1, 0);
    head->next = NULL;
    head->prev = NULL;
    head->size = 1024 - PADDED_SIZE(sizeof(struct block));
    head->in_use = 0;
  }
  //printf("%lu", sizeof(struct block));
  int actual_size = PADDED_SIZE(size);
  int padded_block_size;
  struct block *n = head;
  
  padded_block_size = PADDED_SIZE(sizeof(struct block));
  while(n != NULL){
    if(n->size >= size && n->in_use == 0){
      if (REQUIRED_SPACE(actual_size, padded_block_size) <= n->size){
        split_space(n, actual_size);
      }
      n->in_use = 1;

      return PTR_OFFSET(n, padded_block_size);
    }
    n = n->next;
  }

  return NULL;
}

void print_data(void)
{
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

int main(void){

  void *p, *q, *r, *s;

  p = myalloc(10); print_data();
  q = myalloc(20); print_data();
  r = myalloc(30); print_data();
  s = myalloc(40); print_data();

  myfree(q); print_data();
  myfree(p); print_data();
  myfree(s); print_data();
  myfree(r); print_data();
  return 0;
}