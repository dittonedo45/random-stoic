#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jansson.h>
#include <libavformat/avio.h>
#include <libavutil/opt.h>
#include <time.h>

static int randm(int a, int b)
{
    int dif = b - a;
    int min = a;
    return (rand() % dif) + min;
}

static unsigned int my_rdr(void *buf, unsigned int l, void *d)
{
    return fread((void *) buf, 1, l, (FILE *) d);
}

static int my_json_wr(const char *b, unsigned int l, void *d)
{
    avio_write((AVIOContext *) d, (uint8_t *) b, l);
    return 0;
}

int main(argsc, args, env)
int argsc;
char **args, **env;
{
    int ret = 0;
    AVIOContext *http = 0;
    AVDictionary *oots = 0;
    FILE *f = fopen(args[1], "rb");

    av_dict_set(&oots, "listen", "2", 0);

    ret = avio_open2(&http, "http://:9003", AVIO_FLAG_WRITE, NULL, &oots);

    if (ret < 0)
	return 2;

    if (!f)
	return 1;
    do {
	json_t *stoics_array = json_load_callback(my_rdr, f, 0, 0);
	if (!stoics_array)
	    break;
	srand(time(0));
	int len = json_array_size(stoics_array);

	while (1) {
	    AVIOContext *client = 0;
	    uint8_t *res = 0;

	    ret = avio_accept(http, &client);
	    if (ret < 0)
		break;

	    while (0 < avio_handshake(client));
	    av_opt_get(client, "resource", &res, 0);
	    while (0 < avio_handshake(client));

	    json_t *sel = json_array_get(stoics_array, randm(0, len));
	    if (sel) {
		json_dump_callback(sel, my_json_wr, client, 0);
	    }
	    avio_flush(client);
	    avio_close(client);
	}
    } while (0);
    fclose(f);
    return 0;
}
