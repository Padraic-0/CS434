#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MEM_SIZE 256 * 64//16384  // MUST equal PAGE_SIZE * PAGE_COUNT
#define PAGE_SIZE 256  // MUST equal 2^PAGE_SHIFT
#define PAGE_COUNT 64
#define PAGE_SHIFT 8  // Shift page number this much

#define PTP_OFFSET 64 // How far offset in page 0 is the page table pointer table

// Simulated RAM
unsigned char mem[MEM_SIZE];

//
// Convert a page,offset into an address
//
int get_address(int page, int offset)
{
    return (page << PAGE_SHIFT) | offset;
}

//
// Initialize RAM
//
void initialize_mem(void)
{
    memset(mem, 0, MEM_SIZE);

    int zpfree_addr = get_address(0, 0);
    mem[zpfree_addr] = 1;  // Mark zero page as allocated
}

//
// Get the page table page for a given process
//
unsigned char get_page_table(int proc_num)
{
    int ptp_addr = get_address(0, PTP_OFFSET + proc_num);
    return mem[ptp_addr];
}

int find_free_page(){
    int index = 0;
    while (index < PAGE_COUNT){
        if(mem[index] == 0){
            mem[index] = 1;
            return index;
        }
        index ++;
    }
    return -1;
    
    
}

void set_process_page_table(int proc_num, int process_page_table_index){
    int process_page_table = get_address(0,proc_num + PAGE_COUNT);
    mem[process_page_table] = process_page_table_index;
}

void fill_process_page_table(int proc_num, int process_page_used, int page_index){
    int process_page_table = get_address(0,proc_num + PAGE_COUNT);
    int process_page_table_page = mem[process_page_table];
    int address_of_page_from_process_page_table = get_address(process_page_table_page, page_index);
    mem[address_of_page_from_process_page_table] = process_page_used;
    

}

//
// Allocate pages for a new process
//
// This includes the new process page table and page_count data pages.
//
void new_process(int proc_num, int page_count)
{
    
    int process_page_table_index = find_free_page();
    if (process_page_table_index == -1){
        printf("OOM: proc %d: page table\n", proc_num);
        return;
    }

    set_process_page_table(proc_num, process_page_table_index);
    for (int i = 0; i < page_count; i++){
        int process_page_used = find_free_page();
        if (process_page_used == -1){
            printf("OOM: proc %d: data page\n", proc_num);
            return;
        }
        fill_process_page_table(proc_num, process_page_used, i);
    }

}

//
// Print the free page map
//
// Don't modify this
//
void print_page_free_map(void)
{
    printf("--- PAGE FREE MAP ---\n");

    for (int i = 0; i < 64; i++) {
        int addr = get_address(0, i);

        printf("%c", mem[addr] == 0? '.': '#');

        if ((i + 1) % 16 == 0)
            putchar('\n');
    }
}

//
// Print the address map from virtual pages to physical
//
// Don't modify this
//
void print_page_table(int proc_num)
{
    printf("--- PROCESS %d PAGE TABLE ---\n", proc_num);

    // Get the page table for this process
    int page_table = get_page_table(proc_num);

    // Loop through, printing out used pointers
    for (int i = 0; i < PAGE_COUNT; i++) {
        int addr = get_address(page_table, i);

        int page = mem[addr];

        if (page != 0) {
            printf("%02x -> %02x\n", i, page);
        }
    }
}

//
// Main -- process command line
//
int main(int argc, char *argv[])
{
    assert(PAGE_COUNT * PAGE_SIZE == MEM_SIZE);

    if (argc == 1) {
        fprintf(stderr, "usage: ptsim commands\n");
        return 1;
    }
    
    initialize_mem();

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "pfm") == 0) {
            print_page_free_map();
        }
        else if (strcmp(argv[i], "ppt") == 0) {
            int proc_num = atoi(argv[++i]);
            print_page_table(proc_num);
        }
        else if (strcmp(argv[i], "nm") ==0) {
            int proc_num = atoi(argv[++i]);
            int page_count = atoi(argv[++i]);
            new_process(proc_num, page_count);
        }

        // TODO: more command line arguments
    }
}
