#include <stdarg.h>     // va_start, et al.
#include <stdio.h>      // vsnprintf
#include <stdlib.h>
#include <string.h>

#include "string.h"


char * string_create(const char * string_with_format, ...) {
    char *str = NULL;
    char *fmt;

    // Make a copy of our formatted string to work with
    if (string_with_format != NULL) {
        fmt = strdup(string_with_format);
    } else {
        fmt = strdup("");
    }

    // Now apply the formatting on a trial run to determine how long the formatted string should be
    va_list argp;
    va_start(argp, string_with_format);
    char one_char[1];
    int len = vsnprintf(one_char, 1, fmt, argp);
    if (len < 1) {
        return NULL;
    }
    va_end(argp);

    // Allocate enough memory, and generate the formatted string for reals
    str = calloc(len + 1, sizeof(char));
    if (!str) {
        return NULL;
    }
    va_start(argp, string_with_format);
    vsnprintf(str, len + 1, fmt, argp);
    va_end(argp);

    free(fmt);

    return str;
}

char * string_append(char *string, const char * string_with_format, ...) {
    char *string_to_append = NULL;
    char *fmt;

    // Make a copy of our formatted string to work with
    if (string_with_format != NULL) {
        fmt = strdup(string_with_format);
    } else {
        fmt = strdup("");
    }

    // Now apply the formatting on a trial run to determine how long the formatted string should be
    va_list argp;
    va_start(argp, string_with_format);
    char one_char[1];
    int len = vsnprintf(one_char, 1, fmt, argp);
    if (len < 1) {
        string_to_append = "";
    }
    va_end(argp);

    // Allocate enough memory, and generate the formatted string for reals
    string_to_append = calloc(len + 1, sizeof(char));
    if (!string_to_append) {
        string_to_append = "";
    }
    va_start(argp, string_with_format);
    vsnprintf(string_to_append, len + 1, fmt, argp);
    va_end(argp);

    free(fmt);

    // Allocate enough memory and concatenate the two strings
    int32_t totalLength = strlen(string) + strlen(string_to_append) + 1;
    char *combined_string = calloc(totalLength, sizeof(char));
    strcpy(combined_string, string);
    strcat(combined_string, string_to_append);

    return combined_string;    
}

void string_destroy(char *string) {
    free(string);
}



bool string_equals(char *str1, char *str2) {
    return (strcmp(str1, str2) == 0);
}



/* Test Harness - define __TEST_STRING__ to test */

#ifdef __TEST__

int main () {
    char *str1 = string_create("Formatted string: %d", 1);
    printf("FS 1: %s\n", str1);
    char *str2 = string_create("Formatted string: %s", "two");
    printf("FS 2: %s\n", str2);
    char *str3 = string_create("Three");
    printf("FS 3: %s\n", str3);

    char *app1 = string_append(str3, " + %s = %d", "one", 4);
    printf("AS 1: %s\n", app1);

    string_destroy(str1);
    string_destroy(str2);
    string_destroy(str3);
    string_destroy(app1);

    char *s1 = string_create("Testing equals");
    char *s2 = string_create("Testing equals");
    printf("Strings are equal: %s\n", string_equals(s1, s2) ? "true" : "false");

}

#endif



