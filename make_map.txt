#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <time.h>

char	*gen_map_name()
{
	char *name;

	srand(time(NULL));
	asprintf(&name, "map%d.cub", rand()%100);
	return name;

}

int		is_border(int x, int y, int size)
{
	if(x == 0 || y == 0 || x == size || y == size)
		return 1;
	return 0;
}
int main (int argc, char *argv[])
{
	FILE *fd;

	fd = fopen("mapp.cub", "w");

	// for(int i; i < 10; i++)
	// 	fprintf(fd, "1000000001\n");

	int size = 50;

	for(int y = 0; y <= size; y++)
	{
		for(int x = 0; x <= size; x++)
			fprintf(fd, "%d", is_border(x, y, size));
		fprintf(fd, "\n");
	}
	//close(fd);
	return 0;
}
