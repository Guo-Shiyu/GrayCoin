#include "mem.h"                      
extern BLOCK_HEADER* first_header;

// void swap(int* x, int* y)
// {
//     int* tmp = x;
//     x = y;
//     y = tmp;
// }

// return a pointer to the payload
// if a large enough free block isn't available, return NULL
void* Mem_Alloc(int size){
    // find a free block that's big enough
    BLOCK_HEADER* block_ptr = first_header;
    while (block_ptr->size_alloc != 1)
    {
        // allocate the block
        if (block_ptr->payload >= size)
        {
            if (block_ptr->payload != size) // pad size is greater than user required
            {
                //printf("required size: %d\n", size);
                // split if necessary (if padding size is greater than or equal to 16 split the block)
                int old_size = block_ptr->size_alloc;

                //printf("oldsize: %d\n", old_size);
                void* mem = block_ptr + sizeof(BLOCK_HEADER);

                block_ptr->payload = size;
                block_ptr->size_alloc = size + sizeof(BLOCK_HEADER) + 1;   // mark alloc
                
                BLOCK_HEADER* new_header = block_ptr + block_ptr->size_alloc - 2;
                //printf("attempt new header begin: %x\n", new_header);

                while((unsigned long)new_header%16)
                {
                    block_ptr->size_alloc++;
                    new_header += 1;
                }
                //printf("find suitable new header: %x\n", new_header);

                BLOCK_HEADER* next_header = (BLOCK_HEADER*)new_header;
                memset(next_header, 0, sizeof(BLOCK_HEADER));
                next_header->size_alloc = old_size - block_ptr->size_alloc + 1;
                next_header->payload = next_header->size_alloc - 8;
                //printf("next header: %x, alloc: %d, payload: %d", next_header, next_header->size_alloc, next_header->payload);

                return mem;
            }
            else    // pad size is equal to user required
            {
                block_ptr->size_alloc += 1; // mark allocated
                return block_ptr + 8;
            }
        }
        else
        {
            int offset = block_ptr->size_alloc;
            if ((offset - 1) % 16 == 0)
            {
                offset -= 1;
            }

            // check next block
            block_ptr = (BLOCK_HEADER*)(block_ptr + offset);
        }
    }

    // return NULL if we didn't find a block
    return NULL;
}


// return 0 on success
// return -1 if the input ptr was invalid
int Mem_Free(void *ptr){
    // traverse the list and check all pointers to find the correct block 
    // if you reach the end of the list without finding it return -1
    BLOCK_HEADER* block_ptr = first_header;
    while(block_ptr->size_alloc != 1) 
    {
        // free the block 
        if (block_ptr + sizeof(BLOCK_HEADER) == ptr)
        {
            block_ptr->size_alloc -= 1; // mark free
            block_ptr->payload = 0;

            // coalesce adjacent free blocks
            BLOCK_HEADER* next_header = block_ptr + block_ptr->size_alloc;
            if (next_header->size_alloc % 16 == 0)
            {
                block_ptr->size_alloc += next_header->size_alloc;
                next_header->payload = 0;
            }

            return 0;
        }

    }
    return -1;
}

