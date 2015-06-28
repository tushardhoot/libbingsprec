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

#ifndef __BINGSPREC_CURLUTILS_H__
#define __BINGSPREC_CURLUTILS_H__

#include <stdlib.h>
#include <stdbool.h>

struct bingsprec_response {
	char* str;
	size_t len;
};

struct bingsprec_wavfile {
	const char* readPtr;
	long sizeLeft;	
};

enum LOCALE {
	enUS,
	enGB,
	enIN,
	enAU,
	enCA,
	esES,
	esMX,
	deDE,
	frFR,
	frCA,
	itIT,
	zhCN,
	zhHK,
	zhTW,
	ruRU,
	jaJP,
	ptBR,
	koKR
};

enum SCENARIO {
	ULM,
	WEBSEARCH
};

struct bingsprec_params {
	
	/* HTTP Query Parameters */
	enum LOCALE locale;
	char* deviceos;
	enum SCENARIO scenario;
	int maxnbest;
	bool profanity_markup;	

	/* HTTP Headers */
	int samplerate;
	int sourcerate;
	bool trustsourcerate;
	
};

extern const struct bingsprec_params BINGSPREC_PARAMS_DEFAULT;

static inline const char* locale_to_string(enum LOCALE locale) {
	static const char* locales[18] = { "en-US", "en-GB", "en-IN", "en-AU", 
		"en-CA", "es-ES", "es-MX", "de-DE", "fr-FR", "fr-CA", "it-IT",
		"zh-CN", "zh-HK", "zh-TW", "ru-RU", "ja-JP", "pt-BR", "ko-KR" 
	};
	return locales[locale];
}

static inline const char* scenario_to_string(enum SCENARIO scenario) {
	static const char* scenarios[2] = { "ulm", "websearch" };
	return scenarios[scenario];
}

static inline const char* bool_to_string(bool boolean) {
	return (boolean) ? "true" : "false"; 
}

struct bingsprec_response* bingsprec_new_response();
void bingsprec_free_response(struct bingsprec_response* resp);
size_t bingsprec_write_response(void* ptr, size_t size, size_t nmemb, 
	struct bingsprec_response* resp);
size_t bingsprec_read_wavfile(void* ptr, size_t size, size_t nmemb,
	 struct bingsprec_wavfile* file);
char* bingsprec_str_copy(char* src);

#endif /* __BINGSPREC_CURLUTILS_H__ */
