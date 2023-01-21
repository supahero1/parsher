/* C code produced by gperf version 3.1 */
/* Command-line: gperf -CGT -m 4 -L C -N psh_lookup_word -K str -t words.gperf  */
/* Computed positions: -k'1-2' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "words.gperf"

#include "../include/parsher/token.h"

#define TOTAL_KEYWORDS 51
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 13
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 53
/* maximum key range = 51, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register size_t len;
{
  static const unsigned char asso_values[] =
    {
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 36, 54, 54, 54, 54, 38, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54,  0,  2,  2,
      17, 15,  0,  8, 32,  7, 54, 54, 28,  9,
       1,  1,  3, 54, 21, 11,  7,  7, 31, 16,
      30, 36, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
      54, 54, 54, 54, 54, 54
    };
  return len + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[0]];
}

static const struct psh_word_lut wordlist[] =
  {
    {""}, {""}, {""},
#line 44 "words.gperf"
    {"of", psh_word_of},
#line 33 "words.gperf"
    {"for", psh_word_for},
#line 31 "words.gperf"
    {"false", psh_word_false},
#line 18 "words.gperf"
    {"case", psh_word_case},
#line 19 "words.gperf"
    {"catch", psh_word_catch},
#line 21 "words.gperf"
    {"const", psh_word_const},
#line 37 "words.gperf"
    {"if", psh_word_if},
#line 39 "words.gperf"
    {"in", psh_word_in},
#line 22 "words.gperf"
    {"continue", psh_word_continue},
#line 43 "words.gperf"
    {"null", psh_word_null},
#line 14 "words.gperf"
    {"as", psh_word_as},
#line 32 "words.gperf"
    {"finally", psh_word_finally},
#line 35 "words.gperf"
    {"function", psh_word_function},
#line 15 "words.gperf"
    {"async", psh_word_async},
#line 56 "words.gperf"
    {"undefined", psh_word_undefined},
#line 40 "words.gperf"
    {"instanceof", psh_word_instanceof},
#line 42 "words.gperf"
    {"new", psh_word_new},
#line 26 "words.gperf"
    {"do", psh_word_do},
#line 16 "words.gperf"
    {"await", psh_word_await},
#line 38 "words.gperf"
    {"import", psh_word_import},
#line 49 "words.gperf"
    {"super", psh_word_super},
#line 48 "words.gperf"
    {"static", psh_word_static},
#line 34 "words.gperf"
    {"from", psh_word_from},
#line 36 "words.gperf"
    {"get", psh_word_get},
#line 60 "words.gperf"
    {"with", psh_word_with},
#line 17 "words.gperf"
    {"break", psh_word_break},
#line 47 "words.gperf"
    {"set", psh_word_set},
#line 13 "words.gperf"
    {"arguments", psh_word_arguments},
#line 54 "words.gperf"
    {"try", psh_word_try},
#line 53 "words.gperf"
    {"true", psh_word_true},
#line 50 "words.gperf"
    {"switch", psh_word_switch},
#line 57 "words.gperf"
    {"var", psh_word_var},
#line 20 "words.gperf"
    {"class", psh_word_class},
#line 58 "words.gperf"
    {"void", psh_word_void},
#line 45 "words.gperf"
    {"preregexp_end", psh_word_preregexp_end},
#line 25 "words.gperf"
    {"delete", psh_word_delete},
#line 24 "words.gperf"
    {"default", psh_word_default},
#line 23 "words.gperf"
    {"debugger", psh_word_debugger},
#line 12 "words.gperf"
    {"NaN", psh_word_NaN},
#line 46 "words.gperf"
    {"return", psh_word_return},
#line 51 "words.gperf"
    {"this", psh_word_this},
#line 52 "words.gperf"
    {"throw", psh_word_throw},
#line 11 "words.gperf"
    {"Infinity", psh_word_Infinity},
#line 41 "words.gperf"
    {"let", psh_word_let},
#line 27 "words.gperf"
    {"else", psh_word_else},
#line 61 "words.gperf"
    {"yield", psh_word_yield},
#line 55 "words.gperf"
    {"typeof", psh_word_typeof},
#line 28 "words.gperf"
    {"eval", psh_word_eval},
#line 29 "words.gperf"
    {"export", psh_word_export},
#line 30 "words.gperf"
    {"extends", psh_word_extends},
#line 59 "words.gperf"
    {"while", psh_word_while}
  };

const struct psh_word_lut *
psh_lookup_word (str, len)
     register const char *str;
     register size_t len;
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].str;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
