#include <stdio.h>
#include <curl/curl.h>


typedef struct {
	CURL* handle;
} Downloader;

Downloader* downloader_open();
void downloader_close(Downloader*);

// returns -1 if failed
int downloader_download(Downloader* downloader, char *url, FILE* dst);
