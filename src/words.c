#include <parsher/words.h>

#define CREATE_HASH(name, ...)									\
																\
char* psh_##name##_dict [ psh_##name##__count ] = __VA_ARGS__;	\
																\
struct psh_hashes psh_##name##_hashes = {0};					\
																\
__attribute__((constructor))									\
static void														\
psh_init_##name##_hashes (void)									\
{																\
	psh_hash_init(& psh_##name##_hashes ,						\
		psh_##name##_dict , psh_##name##__count );				\
}																\
																\
__attribute__((destructor))										\
static void														\
psh_free_##name##_hashes (void)									\
{																\
	psh_hash_free(& psh_##name##_hashes );						\
}


CREATE_HASH(keyword, {
	"as",
	"async",
	"await",
	"break",
	"case",
	"catch",
	"class",
	"const",
	"continue",
	"debugger",
	"default",
	"delete",
	"do",
	"else",
	"export",
	"extends",
	"finally",
	"for",
	"from",
	"function",
	"get",
	"if",
	"import",
	"in",
	"instanceof",
	"let",
	"new",
	"of",
	"return",
	"set",
	"static",
	"switch",
	"throw",
	"try",
	"typeof",
	"var",
	"void",
	"while",
	"with",
	"yield"
})


CREATE_HASH(symbol, {
	"++", "--", "+", "-", "*", "/", "%", "**", ">>", "<<", ">>>",

	"<", ">", "<=", ">=", "==", "===", "!=", "!==",

	"?", ":", ",", ".", ";", "{", "}", "(", ")", "[", "]",

	"=", "+=", "-=", "*=", "/=", "%=", "**=", ">>=", "<<=", ">>>=",

	"~", "&", "|", "^", "&=", "|=", "^=",

	"!", "!!", "&&", "||", "?.", "&&=","||=", "?\?="
})


#undef CREATE_HASH
