#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))

#define PADDED_SIZE(x) ((x) + GET_PAD(x))

#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

//global head
struct block *head = NULL;

struct block {
    struct block *next;
    int size;     // Bytes
    int in_use;   // Boolean
};

void* myalloc(int size){
  if (head == NULL){
    head = mmap(NULL, 1024, PROT_READ|PROT_WRITE,
                MAP_ANON|MAP_PRIVATE, -1, 0);
    head->next = NULL;
    head->size = 1024 - PADDED_SIZE(sizeof(struct block));
    head->in_use = 0;
  }

  int actual_size = PADDED_SIZE(size);
  int padded_block_size;
  struct block *n = head;

  while(n != NULL){
    if(n->size >= size && n->in_use == 0){
      n->in_use = 1;
      padded_block_size = PADDED_SIZE(sizeof(struct block));
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
  void *p;

  print_data();
  p = myalloc(16);
  print_data();
  p = myalloc(16);
  printf("%p\n", p);
  return 0;
}