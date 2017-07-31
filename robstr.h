

/*
 * ceeq_str.h
 *
 * These are straightforward implementations of str functions.
 * Some of the standard library implementations are giving a 
 * malloc error.
 *
 * Part of the ceeq_ library. These software are released as is.
 *
 */

#ifndef CEEQ_STR_H_
#define CEEQ_STR_H_


/* 
 * Duplicate a string.
 *
 * dupstr takes a pointer string and returns a pointer to a copy of that string
 */
char *dupstr(const char *s);

/*
 * Concatenate two strings
 *
 * catstr takes two pointers to strings and returns a new pointer to the concatenation of the two strings 
 */
char *catstr(const char *s, const char *t);

#endif /* CEEQ_STR_H_ */
