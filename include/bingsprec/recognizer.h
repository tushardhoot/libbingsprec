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

#ifndef __BINGSPREC_RECOGNIZER_H__
#define __BINGSPREC_RECOGNIZER_H__

#include <bingsprec/msauth.h>
#include <bingsprec/curlutils.h>

struct bingsprec_result {
	char* word;
	double confidence;
};

struct bingsprec_results {
	struct bingsprec_result* results;
	int num_results;
};

void bingsprec_sync(struct msauth_token* token,
			struct bingsprec_params* params,
			char* file_name,
			struct bingsprec_results* results);
void parse_json_to_results(char* json, struct bingsprec_results* results);
void null_bingsprec_results(struct bingsprec_results* results);
void free_bingsprec_results(struct bingsprec_results* results);

#endif /* __BINGSPREC_RECOGNIZER_H__ */
