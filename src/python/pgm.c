// Copyright: Ramiro Polla
// License: WTFPL

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pgm.h"

//---------------------------------------------------------------------
uint8_t *pgm_read(const char *fname, size_t *width, size_t *height)
{
	char buf[0x1000];
	int maxval;

	FILE *fp = strcmp(fname, "-") ? fopen(fname, "rb") : stdin;
	if ( fp == NULL )
	{
		fprintf(stderr, "could not open '%s'\n", fname);
		return NULL;
	}

	if ( fgets(buf, sizeof(buf), fp) == NULL
	  || strcmp(buf, "P5\n")
	  || fscanf(fp, "%zd %zd\n", width, height) != 2
	  || fscanf(fp, "%d", &maxval) != 1
	  || fgetc(fp) != '\n' )
	{
		fprintf(stderr, "could not read pgm header\n");
		return NULL;
	}

	uint8_t *pix = (uint8_t *) malloc(*width * *height);
	if ( pix == NULL )
	{
		fprintf(stderr, "could not allocate memory!\n");
		return NULL;
	}

	if ( fread(pix, *width, *height, fp) != *height )
	{
		fprintf(stderr, "could not read data!\n");
		return NULL;
	}

	if ( strcmp(fname, "-") != 0 )
		fclose(fp);

	return pix;
}

//---------------------------------------------------------------------
int pgm_write(const char *fname, uint8_t *pix, size_t width, size_t height)
{
	const int maxval = 255;

	FILE *fp = strcmp(fname, "-") ? fopen(fname, "wb") : stdout;

	if ( fp == NULL )
	{
		fprintf(stderr, "could not open '%s'\n", fname);
		return -1;
	}

	fprintf(fp, "P5\n");
	fprintf(fp, "%zd %zd\n", width, height);
	fprintf(fp, "%d\n", maxval);
	fwrite(pix, width, height, fp);

	if ( strcmp(fname, "-") != 0 )
		fclose(fp);

	return 0;
}
