#include <parsher/macro.h>
#include <parsher/state.h>

#include <stdio.h>
#include <string.h>


void
psh_default_options(struct psh_options* opt)
{
	*opt =
	(struct psh_options)
	{
		.lexer_ignore_trailing_code = 0
	};
}


void
psh_state_init(struct psh_state* state)
{
	psh_memory_init(&state->messages, sizeof(struct psh_message));
}


void
psh_state_free(struct psh_state* state)
{
	psh_memory_free(&state->messages);
}


void
psh_state_msg(struct psh_state* state, struct psh_message msg)
{
	*(struct psh_message*) psh_memory_get(&state->messages, 1) = msg;
}


#define VT100_RESET "\033[0m"
#define VT100_BOLD "\033[1m"
#define VT100_UNDERSCORE "\033[4m"

#define VT100_BLACK "\033[30m"
#define VT100_RED "\033[31m"
#define VT100_GREEN "\033[32m"
#define VT100_YELLOW "\033[33m"
#define VT100_BLUE "\033[34m"
#define VT100_MAGENTA "\033[35m"
#define VT100_CYAN "\033[36m"
#define VT100_WHITE "\033[37m"


void
psh_state_str(struct psh_state* state,
	struct psh_message* msg, char out[psh_max_status_str_len])
{
	if(msg->status == psh_status_ok)
	{
		out[0] = 0;
		return;
	}


	const char* level = NULL;

	switch(msg->level)
	{

	case psh_level_info:
	{
		level = "[" VT100_BOLD VT100_CYAN "Info" VT100_RESET "]";
		break;
	}

	case psh_level_warning:
	{
		level = "[" VT100_BOLD VT100_YELLOW "Warning" VT100_RESET "]";
		break;
	}

	case psh_level_error:
	{
		level = "[" VT100_BOLD VT100_RED "Error" VT100_RESET "]";
		break;
	}

	}


	const char* status_msg = NULL;

	switch(msg->status)
	{

	case psh_status_ok: __builtin_unreachable();

	case psh_status_unexpected_end:
	{
		status_msg = "Unexpected end-of-file";
		break;
	}

	case psh_status_newline_in_string:
	{
		status_msg = "Strings starting with \" or ' must not contain explicit newlines";
		break;
	}

	case psh_status_newline_in_regexp:
	{
		status_msg = "Regexes must not contain explicit newlines";
		break;
	}

	}


	struct psh_position start_pos = psh_input_query(state->input, msg->start);
	struct psh_position end_pos = psh_input_query(state->input, msg->end);

	uint32_t code_len = MIN(end_pos.column, psh_max_status_occlusion);
	uint32_t code_start = msg->end - code_len;

	uint32_t marker_start;
	uint32_t marker_len;

	if(start_pos.line == end_pos.line)
	{
		marker_start = MAX(msg->start, code_start) - code_start;
		marker_len = code_len - marker_start;
	}
	else
	{
		marker_start = 0;
		marker_len = code_len;
	}

	uint32_t to_end = end_pos.column_width - end_pos.column;
	code_len += MIN(to_end, psh_max_status_occlusion);

	char* code = __builtin_alloca(code_len + 1);
	memcpy(code, state->input->src->arr + code_start, code_len);
	code[code_len] = 0;

	char* whitespace = __builtin_alloca(marker_start + 1);
	memset(whitespace, ' ', marker_start);
	whitespace[marker_start] = 0;

	char* marker = __builtin_alloca(marker_len + 1);
	memset(marker, '^', marker_len);
	marker[marker_len] = 0;

	snprintf(out, psh_max_status_str_len, "%s %s\n%10u | %s\n%10u | %s%s",
		level, status_msg, end_pos.line, code,
		marker_start + marker_len, whitespace, marker);
}

#undef VT100_WHITE
#undef VT100_CYAN
#undef VT100_MAGENTA
#undef VT100_BLUE
#undef VT100_YELLOW
#undef VT100_GREEN
#undef VT100_RED
#undef VT100_BLACK

#undef VT100_UNDERSCORE
#undef VT100_BOLD
#undef VT100_RESET
