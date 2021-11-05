#pragma once

extern char * restrict optarg;
int getopt(const int argc, char * const * const restrict argv, const char * const arglist);
