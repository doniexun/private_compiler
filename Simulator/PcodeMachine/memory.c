#include <common.h>

#define MEMORY_TRUCK_SIZE 4096
/* P-machine should not be alloc extra memory! */
#define TRUCK_ALIGN(size) (size)

/* parameter memory index */
unsigned int memindex = 0;
/* P-machine memory */
unsigned int *memory = NULL;

void init_pm_memory(void)
{
	int size;
	size = TRUCK_ALIGN(memindex * sizeof(unsigned int));
	memory = xmalloc(size);
	printf("[init] memory has been inited: %d bytes\n", size);
}

void exit_pm_memory(void)
{
	if (memory)
		free(memory);
}
