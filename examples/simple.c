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

#include <bingsprec/bingsprec.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	struct msauth_token oauthtoken;

	get_msauth_token(&oauthtoken);
	if (oauthtoken.access_token == NULL) {
		fprintf(stderr, "Unable to get MSAuth Token!\n");	
		exit(-1);
	}

	struct bingsprec_params param = BINGSPREC_PARAMS_DEFAULT;
	param.samplerate = 16000;
	param.sourcerate = 16000;

	struct bingsprec_results results; 
	bingsprec_sync(&oauthtoken, &param, "examples/simple.wav", &results);	
	
	for (int i = 0; i < results.num_results; ++i) {
		printf("Word Found: %s (CONF %.2f)\n",
		 	results.results[i].word,
			results.results[i].confidence);
	}

	free_bingsprec_results(&results);	
	free_msauth_token(&oauthtoken);

	return 0;
}
