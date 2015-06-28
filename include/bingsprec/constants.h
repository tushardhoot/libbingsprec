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

#ifndef __BINGSPREC_CONSTANTS_H__
#define __BINGSPREC_CONSTANTS_H__

/* Change the client id and client secret before use */
#define BINGSPREC_CLIENT_ID "changethis"
#define BINGSPREC_CLIENT_SECRET "changethis"

/* Set to 1 for verbose output, 0 otherwise */
#define BINGSPREC_VERBOSE 0

/* You don't have to change the following in most cases */
#define BINGSPREC_DATAMARKET_URL\
	"https://datamarket.accesscontrol.windows.net/v2/OAuth2-13"
#define BINGSPREC_BASE_URL "https://speech.platform.bing.com"
#define BINGSPREC_APP_ID "D4D52672-91D7-4C74-8AD8-42B1D98141A5"
#define BINGSPREC_REQUEST_MAX_LEN 512

#endif /* __BINGSPREC_CONSTANTS_H__ */
