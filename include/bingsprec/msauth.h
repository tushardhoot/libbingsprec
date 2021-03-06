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

#ifndef __BINGSPREC_MSAUTH_H__
#define __BINGSPREC_MSAUTH_H__

struct msauth_token {
	char* access_token;
	char* token_type;
	int expires_in;
	char* scope;
};

void get_msauth_token(struct msauth_token* token);
void parse_json_to_token(char* json, struct msauth_token* token);
void null_msauth_token(struct msauth_token* token);
void free_msauth_token(struct msauth_token* token);

#endif /* __BINGSPREC_MSAUTH_H__ */
