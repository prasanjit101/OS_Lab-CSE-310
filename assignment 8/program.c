#include <stdio.h>
#include <unistd.h>

typedef struct mem_block
{
    size_t size;
    struct mem_block *next;

} mem_block;

mem_block mem_block_list_head = {0, 0};

void *malloc(size_t size)
{
    size = (size + sizeof(mem_block) + (4 - 1)) & ~(4 - 1);
    mem_block *block = mem_block_list_head.next;
    mem_block **head = &(mem_block_list_head.next);
    while (block != 0)
    {
        if (block->size >= size)
        {
            *head = block->next;
            return ((char *)block) + sizeof(mem_block);
        }
        head = &(block->next);
        block = block->next;
    }

    block = (mem_block *)sbrk(size);
    block->size = size;

    return ((char *)block) + sizeof(mem_block);
}

void free(void *ptr)
{
    mem_block *block = (mem_block *)(((char *)ptr) - sizeof(mem_block));
    block->next = mem_block_list_head.next;
    mem_block_list_head.next = block;
}

int main(void)
{
    int i, *array;
    printf("\nEnter 20 Numbers: ");
    array = malloc(sizeof(int) * 20);

    for (i = 0; i < 20; i++)
    {
        scanf("%d", &array[i]);
    }

    printf("\nThe Numbers are : \n");

    for (i = 0; i < 20; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    free(array);
    return 0;
}
