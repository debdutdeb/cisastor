#include "macros.h"

#include "download.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include <stdbool.h>
#include <stdlib.h>

Downloader *downloader_open() {
  // TODO: curl_global_init;

  Downloader *downloader = cast(Downloader *, calloc(1, sizeof(Downloader)));
  if (null == downloader) {
    return null;
  }

  downloader->handle = curl_easy_init();

  return downloader;
}

void downloader_close(Downloader* downloader) {
	curl_easy_cleanup(downloader->handle);
	free(downloader);
}

int downloader_download(Downloader* downloader, char *url, FILE *dst) {
	CURLcode code = curl_easy_perform(downloader->handle);
	if (CURLE_OK != code) {
		return -1;
	}

	return -1;
}
