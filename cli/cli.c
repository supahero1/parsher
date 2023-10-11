#include <parsher/parsher.h>

#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>


enum cli_option
{
	cli_option_invalid											= 0x00000000,

	/* ACTIONS */
	cli_option_help												= 0x00000001,
	cli_option_lex												= 0x00000002,

	/* GENERAL OPTIONS */
	cli_option_input											= 0x00000010,
	cli_option_output											= 0x00000020,
	cli_option_dump												= 0x00000040,
	cli_option_verbose											= 0x00000080,
	cli_option_no_warnings										= 0x00000100,
	cli_option_stats											= 0x00000200,

	/* LEXER OPTIONS */
	cli_option_lexer_itc										= 0x00001000,
};


static char* cli_dict[] =
{
	/* ACTIONS */
	"-h", "--help",
	"-l", "--lex",

	/* GENERAL OPTIONS */
	"-i", "--input",
	"-o", "--output",
	"-d", "--dump",
	"-v", "--verbose",
	"-W", "--no-warnings",
	"-s", "--stats",

	/* LEXER OPTIONS */
	"-itc", "--ignore-trailing-code",
};


static enum cli_option cli_options[] =
{
	/* ACTIONS */
	cli_option_help, cli_option_help,
	cli_option_lex, cli_option_lex,

	/* GENERAL OPTIONS */
	cli_option_input, cli_option_input,
	cli_option_output, cli_option_output,
	cli_option_dump, cli_option_dump,
	cli_option_verbose, cli_option_verbose,
	cli_option_no_warnings, cli_option_no_warnings,
	cli_option_stats, cli_option_stats,

	/* LEXER OPTIONS */
	cli_option_lexer_itc, cli_option_lexer_itc
};


static struct psh_hashes cli_option_hashes;


static const char* cli_help =
"Usage: %s METHOD [OPTIONS]\n"
"\n"
"Methods:\n"
"  -h  --help          Prints this help message\n"
"  -l  --lex           Lex the input, log debug info on output\n"
"General options:\n"
"  -i  --input         Read input from the given file\n"
"  -o  --output        Write output to the given file\n"
"  -d  --dump          Dump internal representation of input\n"
"  -v  --verbose       Show 'info' level messages\n"
"  -W  --no-warnings   Do not show 'warning' level messages\n"
"  -s  --stats         Print parser statistics of input\n"
"Lexer options:\n"
"  -itc  --ignore-trailing-code\n"
"                      Ignore incomplete code at the end of input\n"
;


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

		printf(cli_help, argv[0]);

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
			printf("Unrecognized option: '%s'.\n", argv[i]);

			return 1;
		}

		option = cli_options[option];

		int32_t maybe_method = option & 0x0000000f;

		if(maybe_method)
		{
			method = maybe_method;
		}

		int32_t maybe_option = option & 0x8ffffff0;

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
		puts("No method has been chosen.");

		return 0;
	}

	case cli_option_help:
	{
		goto goto_help;
	}

	default: break;

	}

	struct psh_memory input = {0};
	psh_memory_init(&input, sizeof(uint8_t));

	int input_fileno;

	if(options & cli_option_input)
	{
		input_fileno = open(input_file, O_RDONLY);

		if(input_fileno == -1)
		{
			printf("Error while opening the input file: %s\n", strerror(errno));

			return 1;
		}
	}
	else
	{
		input_fileno = STDIN_FILENO;

		puts("Waiting for input...");
	}

	psh_memory_atleast(&input, 4096);

	ssize_t bytes;

	while(1)
	{
		bytes = read(
			input_fileno,
			input.ptr + input.used,
			input.size - input.used
		);

		if(!bytes)
		{
			break;
		}

		if(bytes == -1)
		{
			printf("Error while reading input: %s\n", strerror(errno));

			return 1;
		}

		input.used += bytes;

		if(input.used == input.size)
		{
			psh_memory_atleast(&input, input.used + 1);
		}
	}

	(void) close(input_fileno);


	struct psh_source source = { input.ptr, input.used };
	psh_sanitize(&source);

	struct psh_input p_input = { &source, {0} };
	psh_input_init(&p_input);

	struct psh_options opt;
	psh_default_options(&opt);
	opt.lexer_ignore_trailing_code = !!(options & cli_option_lexer_itc);

	struct psh_state state = { &p_input, &opt, {0} };
	psh_state_init(&state);

	struct parsher psh = {0};
	psh.state = &state;

	switch(method)
	{

	case cli_option_lex:
	{
		psh.stage = psh_stage_lex;
		break;
	}

	}

	parsher_init(&psh);


	double start = clock_ms();
	enum psh_status status = parsher_execute(&psh);
	double end = clock_ms();


	struct psh_message* message = state.messages.ptr;
	struct psh_message* message_end = message + state.messages.used;

	while(message != message_end)
	{
		int should_log =
			(message->level == psh_level_info &&
				(options & cli_option_verbose)) ||
			(message->level == psh_level_warning &&
				!(options & cli_option_no_warnings)) ||
			message->level == psh_level_error;

		if(should_log)
		{
			char str[psh_max_status_str_len];
			psh_state_str(&state, message, str);

			if(*str != 0)
			{
				puts(str);
			}
		}

		++message;
	}


	if(status != psh_status_ok)
	{
		goto goto_free;
	}


	if(options & cli_option_stats)
	{
		if(psh.stage >= psh_stage_lex)
		{
			uint32_t white = 0;
			uint32_t space = 0;
			uint32_t white_space = 0;

			for(uint32_t i = 0; i < psh.lexer.tokens.used; ++i)
			{
				struct psh_token* token =
					(struct psh_token*) psh.lexer.tokens.ptr + i;

				switch(token->type)
				{

				case psh_token_whitespace:
				case psh_token_comment:
				{
					++white;
					white_space += token->len;
				}
				/* fallthrough */
				default:
				{
					space += token->len;

					break;
				}

				}
			}

			printf(
				"Tokens: %" PRIu32 "\n"
				"Excess: %" PRIu32 " (%.02lf%% of tokens, %.2lf%% of space)\n"
				"Status: %d\n"
				"Time  : %.03lfms\n"
				, psh.lexer.tokens.used, white,
				(double) white * 100.0 / psh.lexer.tokens.used,
				(double) white_space * 100.0 / space, status, end - start);
		}
	}


	if(options & cli_option_dump)
	{

	switch(method)
	{

	case cli_option_lex:
	{
		static const char* highlight_colors[] =
		{
			[psh_token_none]			= NULL,
			[psh_token_whitespace]		= "\033[39m",
			[psh_token_comment]			= "\033[36m",
			[psh_token_regexp]			= "\033[33m",
			[psh_token_string]			= "\033[32m",
			[psh_token_template]		= "\033[32m",
			[psh_token_template_end]	= "\033[32m",
			[psh_token_number]			= "\033[34m",
			[psh_token_word]			= "\033[39m",
			[psh_token_symbol]			= "\033[35m"
		};

		printf(
			"Legend:\n"
			"    %s/* comment */\n"
			"    %s/regexp/\n"
			"    %s\"s\" 't' `r ${}`\n"
			"    %s3.14\n"
			"    %sword\n"
			"    %s+-*/\n"
			,
			highlight_colors[psh_token_comment],
			highlight_colors[psh_token_regexp],
			highlight_colors[psh_token_string],
			highlight_colors[psh_token_number],
			highlight_colors[psh_token_word],
			highlight_colors[psh_token_symbol]
			);

		for(uint32_t i = 0; i < psh.lexer.tokens.used; ++i)
		{
			struct psh_token* token =
				(struct psh_token*) psh.lexer.tokens.ptr + i;

			printf("%s%.*s", highlight_colors[token->type],
				(int) token->len, (char*) token->token);
		}

		break;
	}

	}

	}


	struct psh_memory output = {0};
	psh_memory_init(&output, sizeof(uint8_t));

	int output_fileno;


	switch(method)
	{

	case cli_option_lex:
	{
		break;
	}

	}


	if(!output.used)
	{
		goto goto_free;
	}

	output.size = output.used;
	output.used = 0;

	if(options & cli_option_output)
	{
		output_fileno = open(output_file, O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);

		if(output_fileno == -1)
		{
			printf("Error while opening the output file: %s\n",
				strerror(errno));

			goto goto_output_free;
		}

		if(ftruncate(output_fileno, output.used) == -1)
		{
			printf("Error while resizing the output file: %s\n",
				strerror(errno));

			goto goto_output_close;
		}
	}
	else
	{
		output_fileno = STDOUT_FILENO;
	}

	while(1)
	{
		bytes = write(
			output_fileno,
			output.ptr + output.used,
			output.size - output.used
		);

		if(!bytes)
		{
			break;
		}

		if(bytes == -1)
		{
			printf("Error while writing output: %s\n", strerror(errno));

			goto goto_output_close;
		}

		output.used += bytes;
	}

	goto_output_close:

	(void) close(output_fileno);

	goto_output_free:

	psh_memory_free(&output);

	goto_free:

	parsher_free(&psh);
	psh_state_free(&state);
	psh_input_free(&p_input);
	psh_memory_free(&input);

	return 0;
}
