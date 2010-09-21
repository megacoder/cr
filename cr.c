/*
 * vim: ts=8 sw=8
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define	LINE_MAX	((unsigned long) BUFSIZ)
#define	DECADE		(10ul)
#define	min(x,y)	( (x) < (y) ? (x) : (y) )

static	char *		me = "cr";
static	unsigned long	wid = 80ul;
static	unsigned long	incr = 0;
static	unsigned	nonfatal;
static	unsigned	numbering;

static	void
usage(
	char const * const	fmt,
	...
)
{
	if( fmt )	{
		va_list		ap;

		fprintf( stderr, "%s: ", me );
		va_start( ap, fmt );
		fprintf( stderr, fmt, ap );
		va_end( ap );
		fprintf( stderr, "\n" );
	}
	fprintf(
		stderr,
		"usage:"
		" %s"
		" [-h]"
		" [-i incr]"
		" [-n]"
		" [-w wid]"
		"\n",
		me
	);
}

static	void
header(
	void
)
{
	static char const	leadin[] = "       ";
	unsigned long		c;

	if( numbering )	{
		printf( "%s ", leadin );
	}
	for( c = 0; c < wid; c += DECADE )	{
		printf( "%10lu", (c + DECADE) / DECADE );
	}
	printf( "\n" );
	if( numbering )	{
		printf( "%s ", leadin );
	}
	for( c = 0; c < wid; c += DECADE )	{
		printf( "1234567890" );
	}
	printf( "\n" );
}

static	void
process(
	char const * const	fn,
	FILE * const		fyle
)
{
	unsigned long		lineno;
	char			buf[ LINE_MAX + 1 ];

	if( !incr )	{
		header();
	}
	lineno = 0;
	while( fgets( buf, sizeof( buf ), fyle ) )	{
		size_t			needed;
		
		if( incr && ( (lineno % incr) == 0 ) )	{
			header();
		}
		++lineno;
		needed = min( wid, strlen( buf ) );
		buf[ needed ] = '\0';
		while( (needed > 0ul) && isspace( buf[ needed - 1 ] ) )	{
			buf[ --needed ] = '\0';
		}
		if( numbering )	{
			printf( "%7lu ", lineno );
		}
		printf( "%s\n", buf );
	}
}

int
main(
	int		argc,
	char * *	argv
)
{
	char *		bp;
	int		c;

	/* Compute our process name					*/
	me = argv[ 0 ];
	if( (bp = strrchr( me, '/' )) != NULL )	{
		me = bp + 1;
	}
	/* Process any command line arguments				*/
	opterr = 0;
	while( (c = getopt( argc, argv, "hi:nw:" )) != EOF )	{
		switch( c )	{
		default:
			fprintf(
				stderr,
				"%s: switch -%c not implemented\n",
				me,
				c
			);
			++nonfatal;
			break;
		case '?':
			fprintf(
				stderr,
				"%s: unknown switch -%c\n",
				me,
				optopt
			);
			++nonfatal;
			break;
		case 'h':
			usage( NULL );
			exit( 1 );
			/*NOTREACHED*/
		case 'i':
			incr = strtoul( optarg, NULL, 0 );
			break;
		case 'n':
			++numbering;
			break;
		case 'w':
			wid = strtoul( optarg, NULL, 0 );
			break;
		}
	}
	if( nonfatal )	{
		usage( "Illegal argument(s)" );
		exit( 1 );
	}
	wid -= (numbering ? 10 : 0);
	/* Take input file names from command line or use stdin		*/
	if( optind < argc )	{
		/* Each command line token is an input file		*/
		while( optind < argc )	{
			char const * const	fn = argv[ optind++ ];
			FILE *		fyle;

			fyle = fopen( fn, "rt" );
			if( !fyle )	{
				fprintf(
					stderr,
					"%s: cannot read file '%s'.\n",
					me,
					fn
				);
				++nonfatal;
				continue;
			}
			process( fn, fyle );
			fclose( fyle );
		}
	} else	{
		/* Process stdin, no prompting				*/
		process( "{stdin}", stdin );
	}
	return( nonfatal ? 1 : 0 );
}
