/*  
    GNU Lesser General Public License

    Copyright (c) 2015 Tushar Dhoot

    This file is part of libbingsprec.

    libbingsprec is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libbingsprec is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libbingsprec.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <bingsprec/curlutils.h>
#include <string.h>
#include <math.h>

const struct bingsprec_params BINGSPREC_PARAMS_DEFAULT = {
	enUS,
	"Ubuntu",
	ULM,
	1,
	true,
	8000,
	8000,
	true
};

struct bingsprec_response* bingsprec_new_response() {
	struct bingsprec_response* resp = (struct bingsprec_response*) 
		malloc(sizeof(struct bingsprec_response));
	resp->str = (char *) malloc(sizeof(char));
	if (resp->str == NULL) {
		exit(-1);
	}
	resp->str[0] = '\0';
	resp->len = 0;
	return resp;
}

void bingsprec_free_response(struct bingsprec_response* resp) {
	free(resp->str);
	free(resp);

	resp->str = NULL;
	resp = NULL;
}

size_t bingsprec_write_response(void* ptr, size_t size, size_t nmemb, 
	struct bingsprec_response* resp) {
	size_t new_len = resp->len + size*nmemb;
	resp->str = (char *) realloc(resp->str, new_len+1);

	if (resp->str == NULL) {
		exit(-1);
	}
	memcpy(resp->str + resp->len, ptr, size*nmemb);
	resp->str[new_len] = '\0';
	resp->len = new_len;
	
	return size*nmemb;
}	

size_t bingsprec_read_wavfile(void* ptr, size_t size, size_t nmemb, 
	struct bingsprec_wavfile* file) {
	if (size*nmemb < 1 || file->sizeLeft == 0) {
		return 0;
	}
	
	size_t toTransfer = fmin(file->sizeLeft, (long)1024);
	toTransfer = fmin(toTransfer, size*nmemb);
	memcpy(ptr, file->readPtr, toTransfer);
	file->readPtr += toTransfer;
	file->sizeLeft -= toTransfer;
	
	return toTransfer;	
}

char* bingsprec_str_copy(char* src) {
	char* copy = (char*) malloc((strlen(src) + 1) * sizeof(char));
	if (copy == NULL) {
		return NULL;
	}	

	strcpy(copy, src);
	return copy;
}
