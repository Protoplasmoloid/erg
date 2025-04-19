/*
 * trace.h
 *
 *  Created on: Apr 19, 2025
 *      Author: Proto
 */

#ifndef TRACE_H_
#define TRACE_H_

#define INFO(...) printf("\033[1;32mINFO:%s:\033[0m", __func__); \
	printf(__VA_ARGS__); fflush(stdout);
#define DEBUG(...) printf("\033[1;34mDEBUG:%s:\033[0m", __func__); \
	printf(__VA_ARGS__); fflush(stdout);
#define ERROR(...) fprintf(stderr, "\033[1;31mERROR:%s:\033[0m", __func__); \
	printf(__VA_ARGS__);

#endif /* TRACE_H_ */
