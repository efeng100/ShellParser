#include <stdio.h>
#include <ctype.h>

#include "Scanner.h"

static void update_next_token(Scanner *self);

Scanner Scanner_value(CharItr char_itr)
{
    Token next = {
        END_TOKEN,
        Str_from("")
    };

    Scanner itr = {
        char_itr,
        next
    };

    Str_drop(&(next.lexeme));
    update_next_token(&itr);

    return itr;
}

bool Scanner_has_next(const Scanner *self)
{
    return self->next.type != END_TOKEN;
}

Token Scanner_peek(const Scanner *self)
{
    return self->next;
}

Token Scanner_next(Scanner *self)
{
    Token next = self->next;
    update_next_token(self);
    /* Note: Because self->next always holds the next peekable token value
     * (even if it's a repeating end token) which contains heap memory
     * in its Str, a Scanner will always produce a memory leak because
     * it has no drop function to free the heap memory in self->next
     * Given the assignment instructions, I couldn't think of a workaround
     */
    return next;
}

static void skip_spaces(CharItr *itr);
static void take_pipe(Scanner *self);
static void take_end(Scanner *self);
static void take_word(Scanner *self);

static void update_next_token(Scanner *self)
{
	CharItr *itr = &(self->char_itr);
	skip_spaces(itr);

	if (!CharItr_has_next(itr)) {
		self->next.type = END_TOKEN;
    		self->next.lexeme = Str_from("");
		return;
	}

	switch (CharItr_peek(itr)) {
		case '|':
			take_pipe(self);
			break;
		case '\0': case EOF: 
			// Doubt this case would ever happen, but this is here just in case
			// Also a char can't even equal an EOF because EOF is an int quantity 
			take_end(self);
			break;
		default:
			take_word(self);
			break;
	}
}

static void skip_spaces(CharItr *itr)
{
	char nextChar;
	while (CharItr_has_next(itr) &&
			((nextChar = CharItr_peek(itr)) == ' ' ||
			 nextChar == '\t' || nextChar == '\n')) {
		CharItr_next(itr);
	}
}

static void take_pipe(Scanner *self)
{
	CharItr_next(&(self->char_itr));
	self->next.type = PIPE_TOKEN;
	self->next.lexeme = Str_from("|");
}

// This function doesn't really make sense if end tokens are meant to
// only occur at the end of the stream of a CharItr, so if that's the
// case, this function is never called.
static void take_end(Scanner *self)
{
	char nextChar = CharItr_next(&(self->char_itr));
	Str nextLexeme = Str_value(1);
	Str_set(&nextLexeme, 0, nextChar);
	self->next.type = END_TOKEN;
	self->next.lexeme = nextLexeme;
}

static void take_word(Scanner *self)
{
	char nextChar;
	CharItr *itr = &(self->char_itr);
	Str nextLexeme = Str_value(1);

	while (CharItr_has_next(itr) && (nextChar = CharItr_peek(itr)) != ' ' && 
			nextChar != '\t' && nextChar != '\n' && nextChar != '|' && 
			nextChar != '\0' && nextChar != EOF) {
		Str_set(&nextLexeme, Str_length(&nextLexeme), nextChar);
		CharItr_next(itr);
	}

	self->next.type = WORD_TOKEN;
	self->next.lexeme = nextLexeme;
}
