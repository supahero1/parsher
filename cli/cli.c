#include <parsher/token.h>
#include <parsher/except.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>


enum cli_option
{
	cli_option_invalid				= 0x0000,

	/* ACTIONS */
	cli_option_help					= 0x0001,
	cli_option_tokenize				= 0x0002,

	/* OPTIONS */
	cli_option_input				= 0x0100,
	cli_option_output				= 0x0200,
};


static char* cli_dict[] =
{
	/* ACTIONS */
	"-h", "--help",
	"-t", "--tokenize",

	/* OPTIONS */
	"-i", "--input",
	"-o", "--output",
};


static enum cli_option cli_options[] =
{
	/* ACTIONS */
	cli_option_help, cli_option_help,
	cli_option_tokenize, cli_option_tokenize,

	/* OPTIONS */
	cli_option_input, cli_option_input,
	cli_option_output, cli_option_output,
};


static struct psh_hashes cli_option_hashes;


static const char* cli_help =
"Usage: %s METHOD [OPTIONS]\n"
"\n"
"Methods:\n"
"    -h --help                 Prints this help message\n"
"    -t --tokenize             Tokenize the input, log debug info on output\n"
"\n"
"Tokenizer options:\n"
"    -i --input                Read input from the given file\n"
"    -o --output               Save output to the given file\n"
"\n";


static double
clock_ms(void)
{
	struct timespec tp;

	(void) clock_gettime(CLOCK_REALTIME, &tp);

	return tp.tv_sec * 1000 + tp.tv_nsec / (double) 1000000.0;
}


int
main(int argc, char** argv)
{
	if(argc == 1)
	{
		goto_help:

		(void) printf(cli_help, argv[0]);

		return 0;
	}

	psh_hash_init(&cli_option_hashes, cli_dict,
		sizeof(cli_dict) / sizeof(cli_dict[0]));

	int32_t method = 0;
	int32_t options = 0;

	char* input_file = NULL;
	char* output_file = NULL;

	for(int i = 1; i < argc; ++i)
	{
		int32_t option =
			psh_hash_lookup(&cli_option_hashes, argv[i], strlen(argv[i]));

		if(option == -1)
		{
			(void) printf("Unrecognized option: '%s'.\n", argv[i]);

			return 1;
		}

		option = cli_options[option];

		int32_t maybe_method = option & 0x00ff;

		if(maybe_method)
		{
			method = maybe_method;
		}

		int32_t maybe_option = option & 0xff00;

		if(maybe_option)
		{
			options |= maybe_option;

			switch(maybe_option)
			{

			case cli_option_input:
			{
				++i;

				input_file = argv[i];

				break;
			}

			case cli_option_output:
			{
				++i;

				output_file = argv[i];

				break;
			}

			default: break;

			}
		}
	}

	psh_hash_free(&cli_option_hashes);

	switch(method)
	{

	case cli_option_invalid:
	{
		(void) puts("You have not chosen a method. This defaults to noop.");

		return 0;
	}

	case cli_option_help:
	{
		goto goto_help;
	}

	default: break;

	}

	char* input;
	uint64_t input_len;
	int input_fileno;

	if(options & cli_option_input)
	{
		input_fileno = open(input_file, O_RDONLY);

		if(input_fileno == -1)
		{
			(void) printf("Error while opening the input file: %s\n",
				strerror(errno));

			return 1;
		}
	}
	else
	{
		input_fileno = STDIN_FILENO;

		puts("Waiting for input...");
	}

	input = malloc(4096);

	if(!input)
	{
		psh_nomem();
	}

	input_len = 4096;
	uint64_t off = 0;

	ssize_t bytes;

	while(1)
	{
		bytes = read(input_fileno, input + off, input_len - off);

		if(!bytes)
		{
			break;
		}

		if(bytes == -1)
		{
			(void) printf("Error while reading input: %s\n", strerror(errno));

			return 1;
		}

		off += bytes;

		if(off == input_len)
		{
			input_len <<= 1;

			input = realloc(input, input_len);

			if(!input)
			{
				psh_nomem();
			}
		}
	}

	(void) close(input_fileno);

	input_len = off;


	char* output;
	uint64_t output_len;
	int output_fileno;


	switch(method)
	{

	case cli_option_tokenize:
	{
		struct psh_source source = { (uint8_t*) input, input_len };
		struct psh_tokens out = {0};

		psh_tokens_init(&out);

		double start = clock_ms();

		enum psh_status status = psh_tokenize(&source, &out);

		double end = clock_ms();

		output = malloc(4096);

		output_len = sprintf(output,
			"Tokens: %" PRIu64 "\n"
			"Status: %d\n"
			"Time  : %.03lfms\n"
			, out.used, status, end - start);

		psh_tokens_free(&out);

		break;
	}

	}


	if(options & cli_option_output)
	{
		output_fileno = open(output_file, O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);

		if(output_fileno == -1)
		{
			(void) printf("Error while opening the output file: %s\n",
				strerror(errno));

			return 1;
		}

		if(ftruncate(output_fileno, output_len) == -1)
		{
			(void) printf("Error while resizing the output file: %s\n",
				strerror(errno));

			return 1;
		}
	}
	else
	{
		output_fileno = STDOUT_FILENO;
	}

	off = 0;

	while(1)
	{
		bytes = write(output_fileno, output + off, output_len - off);

		if(!bytes)
		{
			break;
		}

		if(bytes == -1)
		{
			(void) printf("Error while writing output: %s\n", strerror(errno));

			return 1;
		}

		off += bytes;
	}

	(void) close(output_fileno);

	free(output);

	return 0;
}
