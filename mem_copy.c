#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>

#define SIZE 1000

//PRU shared ram location
//#define MEMLOC 0x4a310000

//arbitrary location in BBB ram
#define MEMLOC 0x80000000

int main()
{
	int image[SIZE];

	//vars to record time
	struct timeval before , after;

	//open /dev/mem which gives us access to physical memory
	//removing O_SYNC improves speed by ~3MB/s, but not sure I should do this?
	//adding volatile improves ~1MB/s
	volatile int fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd == -1) 
	{
		printf("failed to open\n");
		return 0;
	}

	//mmap our physical mem location to a virtual address
	int *ptr = mmap(NULL, SIZE * sizeof(int), PROT_READ , MAP_PRIVATE | MAP_POPULATE, fd, MEMLOC);
	if (ptr == MAP_FAILED)
	{
		close(fd);
		perror("Error mmapping the file");
		exit(EXIT_FAILURE);
	}

	//mlock() the memory space
	mlock(ptr, SIZE);

	printf("====STARTING\n");
	gettimeofday(&before , NULL);

	//copy data	
	memcpy(image, ptr , SIZE*sizeof(int));

	gettimeofday(&after , NULL);
	printf("====ENDING\n");

	//the -3 is how long getting the time takes
	long uSecs = after.tv_usec - before.tv_usec - 3;
	double data =(double)(SIZE * sizeof(int));
	double dataRate = (data / uSecs)*1000000; //bytes per second

	printf("Elapsed time: %ld uSec\n", uSecs);
	printf("Total Data: %f MB\n", data / (float)1000000);
	printf("Data Rate: %f  MB/Second \n", dataRate / (float)1000000);
	printf("Image[1-3] = %x, %x, %x\n", image[0], image[1], image[2]);

	//unmap memory
	munmap(ptr, SIZE);
}
