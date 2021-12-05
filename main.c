#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jansson.h>
static int randm(int a, int b)
{
    int dif = b - a;
    int min = a;
    return (rand() % dif) + min;
}

static int my_rdr(void *buf, int l, void *d)
{
    return fread(buf, 1, l, d);
}

int main(argsc, args, env)
int argsc;
char **args, **env;
{
    FILE *f = fopen(args[1], "rb");
    if (!f)
	return 1;
    do {
	json_t *stoics_array = json_load_callback(my_rdr, f, 0, 0);
	if (!stoics_array)
	    break;
	srand(time(0));
	int len = json_array_size(stoics_array);
	json_t *sel = json_array_get(stoics_array, randm(0, len));
	if (sel)
	    json_dumpf(sel, stdout, 0);
    } while (0);
    fclose(f);
    return 0;
}
