#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stdint.h>


/*
 * Formatted character string helper functions.
 * Usable with already-created char * strings.
 *
 * Written by PeteyCodes (@peteycodes, twitch.tv/peteycodes)
 */


char * string_new(const char * string_with_format, ...);
char * string_append(char *string, const char * string_with_format, ...);
void string_destroy(char *string);

//string_split (return List of strings)
//string_join (combine list of strings, with given separator string)

bool string_equals(char *str1, char *str2);


#endif


