#include <parsher/input.h>
#include <parsher/assert.h>


int
main()
{
	struct psh_source source = { (uint8_t*) "abc\nabc\nab\na\n\n", 14 };
	psh_sanitize(&source);

	struct psh_input input = { &source, {0} };
	psh_input_init(&input);

	struct psh_position pos;

	pos = psh_input_query(&input, 0);
	psh_assert_eq(pos.line, 1);
	psh_assert_eq(pos.column, 0);
	psh_assert_eq(pos.column_width, 3);

	pos = psh_input_query(&input, 12);
	psh_assert_eq(pos.line, 4);
	psh_assert_eq(pos.column, 1);
	psh_assert_eq(pos.column_width, 1);

	pos = psh_input_query(&input, 13);
	psh_assert_eq(pos.line, 5);
	psh_assert_eq(pos.column, 0);
	psh_assert_eq(pos.column_width, 0);


	psh_input_free(&input);

	source = (struct psh_source) { (uint8_t*) "\ndef", 4 };
	psh_sanitize(&source);

	input = (struct psh_input) { &source, {0} };
	psh_input_init(&input);

	pos = psh_input_query(&input, 0);
	psh_assert_eq(pos.line, 1);
	psh_assert_eq(pos.column, 0);
	psh_assert_eq(pos.column_width, 0);

	pos = psh_input_query(&input, 3);
	psh_assert_eq(pos.line, 2);
	psh_assert_eq(pos.column, 2);
	psh_assert_eq(pos.column_width, 3);


	psh_input_free(&input);

	source = (struct psh_source) { (uint8_t*) "abc\n", 4 };
	psh_sanitize(&source);

	input = (struct psh_input) { &source, {0} };
	psh_input_init(&input);

	pos = psh_input_query(&input, 3);
	psh_assert_eq(pos.line, 1);
	psh_assert_eq(pos.column, 3);
	psh_assert_eq(pos.column_width, 3);


	psh_input_free(&input);

	return 0;
}
