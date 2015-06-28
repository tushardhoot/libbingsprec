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

#include <bingsprec/recognizer.h>
#include <bingsprec/curlutils.h>
#include <bingsprec/constants.h>
#include <bingsprec/msauth.h>
#include <bingsprec/cJSON.h>

#include <uuid/uuid.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bingsprec_sync(struct msauth_token* token,
			struct bingsprec_params* params,
			char* file_name,
			struct bingsprec_results* results) {
	null_bingsprec_results(results);

	uuid_t uuid;
	char uuid_str[36 + 1];
	uuid_generate(uuid);
	uuid_unparse_upper(uuid, uuid_str);
	uuid_str[36] = '\0';

	char* request = (char*) 
		malloc(BINGSPREC_REQUEST_MAX_LEN * sizeof(char));		
	if (request == NULL) {
		exit(-1);
	}

	char instance_id[36 + 1];	
	FILE* fp_read = fopen(".saved_instance_id", "r");
	if (fp_read != NULL) {
		if (fread(instance_id, sizeof(char), 36, fp_read)) {
			instance_id[36] = '\0';
			fclose(fp_read);
		} else {
			fprintf(stderr, "Error reading from file.\n");
			fclose(fp_read);
			exit(-1);
		}
	} else {
		/* This is either the first time making a call, the previous
		 * instance id file was deleted, or we had an error opening the
		 * file. We'll try creating a new instance id to handle the
		 * first two cases.
		 */
			
		uuid_t instance_uuid;
		uuid_generate(instance_uuid);
		uuid_unparse_upper(instance_uuid, instance_id);	
		
		FILE* fp_write = fopen(".saved_instance_id", "w");
		if (fp_write != NULL) {
			fputs(instance_id, fp_write);	
		}
		fclose(fp_write);
	}

	snprintf(request,
		BINGSPREC_REQUEST_MAX_LEN * sizeof(char),
		"%s/recognize"
		"?version=3.0&format=json"
		"&scenarios=%s"
		"&appid=%s"
		"&locale=%s"
		"&device.os=%s"
		"&maxnbest=%d"
		"&result.profanity=%d"
		"&instanceid=%s"
		"&requestid=%s",
		BINGSPREC_BASE_URL,
		scenario_to_string(params->scenario),
	        BINGSPREC_APP_ID,
		locale_to_string(params->locale),
		params->deviceos,
		params->maxnbest,
		params->profanity_markup,
		instance_id,	
		uuid_str);	

	CURL* curl = curl_easy_init();
	if (!curl) {
		exit(-1);
	}

	curl_easy_setopt(curl, CURLOPT_VERBOSE, BINGSPREC_VERBOSE);
	curl_easy_setopt(curl, CURLOPT_URL, request);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);

	FILE* file = fopen(file_name, "r");
	if (file == NULL) {
		fprintf(stderr, "Error opening wav file.\n");
		exit(-1);
	}

	fseek(file, 0, SEEK_END);
	unsigned long fileSize = (unsigned long)ftell(file);
	rewind(file);
	
	char* audio_data = (char*) malloc(sizeof(char) * fileSize);
	if (audio_data == NULL) {
		fprintf(stderr, "No membory to allocate audio buffer.\n");
		exit(-2);
	}
	
	struct bingsprec_wavfile wavFile;
	size_t freadres = fread(audio_data, 1, fileSize, file);
	if (ferror(file) || freadres != fileSize) {
		fprintf(stderr, "Eror reading wav file!\n");
		fclose(file);
		exit(-1);
	}
	
	fclose(file);

	wavFile.readPtr = audio_data;
	wavFile.sizeLeft = fileSize;

	curl_easy_setopt(curl, CURLOPT_READFUNCTION, bingsprec_read_wavfile);
	curl_easy_setopt(curl, CURLOPT_READDATA, &wavFile);


	char* content_type = (char*) 
		malloc(BINGSPREC_REQUEST_MAX_LEN * sizeof(char));
	snprintf(content_type,
		BINGSPREC_REQUEST_MAX_LEN * sizeof(char),
		"Content-Type: audio/wav; codec=\"audio/pcm\"; "
		"samplerate=%d; "
		"sourcerate=%d; "
		"trustsourcerate=%s",
		params->samplerate,
		params->sourcerate,
		bool_to_string(params->trustsourcerate)); 

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, content_type);
	headers = curl_slist_append(headers, 
		"Accept: application/json;text/xml");
	headers = curl_slist_append(headers, 
		"Host: speech.platform.bing.com");	
	headers = curl_slist_append(headers, 
		"Connection: Keep-Alive");
	headers = curl_slist_append(headers, 
		"Transfer-Encoding: chunked");

	free(content_type);
	
	size_t token_length = strlen(token->access_token);
	// strlen("Authorization: Bearer ") = 22
	char auth_header[22 + token_length + 1];
	auth_header[0] = '\0';
	strcat(auth_header, "Authorization: Bearer ");
	strcat(auth_header, token->access_token);
	headers = curl_slist_append(headers, auth_header);

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	
	struct bingsprec_response* resp = bingsprec_new_response();	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, 
		bingsprec_write_response);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
 
	CURLcode res = curl_easy_perform(curl);
	long http_code = 0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	if (res == CURLE_OK && http_code == 200) {
		parse_json_to_results(resp->str, results);
	} else {
		fprintf(stderr, "CURL Error:\n%s\n", curl_easy_strerror(res));
	}	

	bingsprec_free_response(resp);
	free(request); 
	free(audio_data);
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
}

void parse_json_to_results(char* json, struct bingsprec_results* results) {
	cJSON* root = cJSON_Parse(json);
	cJSON* header = cJSON_GetObjectItem(root, "header");

	cJSON* status = cJSON_GetObjectItem(header, "status");
	if (strcmp(status->valuestring, "success") != 0) {
		fprintf(stderr, "Bing Speech Platform returned an error! "
				"JSON Returned:\n %s\n", json);	

		cJSON_Delete(root);
		return;
	}

	cJSON* bing_results = cJSON_GetObjectItem(root, "results");
	
	int n = cJSON_GetArraySize(bing_results);
	fprintf(stderr, "%d Words Found: \n", n);

	results->results = (struct bingsprec_result*) 
		malloc(n * sizeof(struct bingsprec_result));
	results->num_results = n;

	for (int i = 0; i < n; ++i) {
		cJSON* subitem = cJSON_GetArrayItem(bing_results, i);
		cJSON* word = cJSON_GetObjectItem(subitem, "name");
		cJSON* conf = cJSON_GetObjectItem(subitem, "confidence");

		fprintf(stderr, 
			"%s (Conf. %.2f)\n",
			word->valuestring,
			conf->valuedouble);

		struct bingsprec_result* cur_result = &results->results[i];
		cur_result->confidence = conf->valuedouble;
		cur_result->word = bingsprec_str_copy(word->valuestring);
		if (cur_result->word == NULL) {
			// Log the error and return what we have so far.
			fprintf(stderr, "No memory for all %d words!\n", n);
			break;	
		}
	}

	cJSON_Delete(root);	
}

void null_bingsprec_results(struct bingsprec_results* results) {
	results->results = NULL;
}

void free_bingsprec_results(struct bingsprec_results* results) {
	free(results->results);
	null_bingsprec_results(results);
}
