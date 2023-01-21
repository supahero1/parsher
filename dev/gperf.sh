#!/bin/bash

cat words_base.gperf > words.gperf

echo "$(cat ../include/parsher/words.h | sed -rn \
's/	psh_word_([[:alnum:]]+),?/\1, psh_word_\1/p')" >> words.gperf

gperf -CGT -S 1 -m 4 -N psh_lookup_word -K str -t \
words.gperf > ../src/words/words.c

echo "const size_t psh_max_word_len = MAX_WORD_LENGTH;" >> ../src/words/words.c
