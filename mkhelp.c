/*
 * Copyright (C) 1984-2012  Mark Nudelman
 * Modified for use with illumos by Garrett D'Amore.
 * Copyright 2014 Garrett D'Amore <garrett@damore.org>
 *
 * You may distribute under the terms of either the GNU General Public
 * License or the Less License, as specified in the README file.
 *
 * For more information, see the README file.
 */

/*
 * Silly little program to generate the help.c source file
 * from the less.hlp text file.
 * help.c just contains a char array whose contents are
 * the contents of less.hlp.
 */

#include <stdio.h>

int
main(int argc, char **argv)
{
	int ch;
	int prevch;
	int col;

	(void) printf("/*\n");
	(void) printf(" * This file was generated by mkhelp from less.hlp\n");
	(void) printf(" */\n");
	(void) printf("#include \"less.h\"\n");
	(void) printf("const char helpdata[] = {\n");
	ch = 0;
	col = 0;
	while (prevch = ch, (ch = getchar()) != EOF) {
		if (col >= 74) {
			(void) printf(",\n");
			col = 0;
		} else if (col) {
			col += printf(", ");
		}
		switch (ch) {
		case '\'':
			col += printf("'\\''");
			break;
		case '\\':
			col += printf("'\\'");
			break;
		case '\b':
			col += printf("'\\b'");
			break;
		case '\t':
			col += printf("'\\t'");
			break;
		case '\"':
			col += printf("'\"'");
			break;
		case '\n':
			if (prevch != '\r')  {
				(void) printf("'\\n',\n");
				col = 0;
			}
			break;
		case '\r':
			if (prevch != '\n')  {
				(void) printf("'\\n',\n");
				col = 0;
			}
			break;
		default:
			if (ch >= ' ' && ch < 0x7f)
				col += printf("'%c'", ch);
			else
				col += printf("0x%02x", ch);
			break;
		}
	}
	/* Add an extra null char to avoid having a trailing comma. */
	(void) printf("0\n");
	(void) printf("};\n");
	(void) printf("int size_helpdata = sizeof (helpdata) - 1;\n");
	return (0);
}
