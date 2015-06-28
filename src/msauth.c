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

#include <bingsprec/msauth.h>
#include <bingsprec/curlutils.h>
#include <bingsprec/constants.h>
#include <bingsprec/cJSON.h>

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_msauth_token(struct msauth_token* token) {
	null_msauth_token(token);

	CURL* curl = curl_easy_init();
	if (!curl) {
		exit(-1);
	}

	curl_easy_setopt(curl, CURLOPT_VERBOSE, BINGSPREC_VERBOSE);
	curl_easy_setopt(curl, CURLOPT_URL, BINGSPREC_DATAMARKET_URL);
	curl_easy_setopt(curl, CURLOPT_POST, 1);

	char* request = 
		(char*) malloc(BINGSPREC_REQUEST_MAX_LEN);
	if (request == NULL) {
		exit(-1);	
	}

	char* escaped_client_id = 
		curl_easy_escape(curl, BINGSPREC_CLIENT_ID, 0);
	char* escaped_client_secret = 
		curl_easy_escape(curl, BINGSPREC_CLIENT_SECRET, 0);
	
	snprintf(request,
		BINGSPREC_REQUEST_MAX_LEN * sizeof(char),
		"scope=%s"
		"&grant_type=client_credentials"
		"&client_id=%s"
		"&client_secret=%s",
		BINGSPREC_BASE_URL,
		escaped_client_id,
		escaped_client_secret); 

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(request));

	struct bingsprec_response* resp = bingsprec_new_response();
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, 
		bingsprec_write_response);	
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
	
	CURLcode res = curl_easy_perform(curl);
	long http_code = 0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	if (res == CURLE_OK && http_code == 200) {
		parse_json_to_token(resp->str, token);
	} else {
		fprintf(stderr, "CURL Error:\n%s\n", curl_easy_strerror(res));
	}
	
	bingsprec_free_response(resp);
	curl_free(escaped_client_id);
	curl_free(escaped_client_secret);
	curl_easy_cleanup(curl); 
}

void parse_json_to_token(char* json, struct msauth_token* token) {
	cJSON* root = cJSON_Parse(json);
	cJSON* access_token = cJSON_GetObjectItem(root, "access_token");
	cJSON* token_type = cJSON_GetObjectItem(root, "token_type");
	cJSON* expires_in = cJSON_GetObjectItem(root, "expires_in");
	cJSON* scope = cJSON_GetObjectItem(root, "scope");

	token->access_token = bingsprec_str_copy(access_token->valuestring);
	token->token_type = bingsprec_str_copy(token_type->valuestring);
	token->expires_in = expires_in->valueint;
	token->scope = bingsprec_str_copy(scope->valuestring);

	cJSON_Delete(root);
	if (token->access_token == NULL ||
	    token->token_type == NULL ||
	    token->scope == NULL) {

		free_msauth_token(token);
		fprintf(stderr, "No memory to copy strings!\n");	
		exit(-1);
	}
}

void null_msauth_token(struct msauth_token* token) {
	token->access_token = NULL;
	token->token_type = NULL;
	token->scope = NULL;
}

void free_msauth_token(struct msauth_token* token) {
	free(token->access_token);
	free(token->token_type);
	free(token->scope);	
	null_msauth_token(token);
} 
