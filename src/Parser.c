#include "Parser.h"

static Node* parse_command(Scanner *scanner);

Node* parse(Scanner *scanner)
{
	if (!Scanner_has_next(scanner)) {
		return ErrorNode_new("End of text stream");
	}

	Token next = Scanner_peek(scanner);
	if (next.type != WORD_TOKEN) {
		return ErrorNode_new("Expected a word token");
	}

	Node *left = parse_command(scanner);
	if (!Scanner_has_next(scanner) || 
			Scanner_peek(scanner).type != PIPE_TOKEN) {
		return left;
	}
	
	next = Scanner_next(scanner);
	Str_drop(&(next.lexeme));
	
	Node *right = parse(scanner);
	return PipeNode_new(left, right);
}

static Node* parse_command(Scanner *scanner)
{
	StrVec words = StrVec_value(1);
	while (Scanner_has_next(scanner) && 
			Scanner_peek(scanner).type == WORD_TOKEN) {
		StrVec_push(&words, Scanner_next(scanner).lexeme);
	}
	return CommandNode_new(words);
}
