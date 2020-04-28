#include "Node.h"
#include "Guards.h"

Node* ErrorNode_new(const char *msg)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = ERROR_NODE;
    node->data.error = msg;
    return node;
}

Node* CommandNode_new(StrVec words)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = COMMAND_NODE;
    node->data.command = words;
    return node;
}

Node* PipeNode_new(Node *left, Node *right)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = PIPE_NODE;
    node->data.pipe.left = left;
    node->data.pipe.right = right;
    return node;
}

void* Node_drop(Node *self)
{
	// Any memory leak you see isn't from Node_drop
	// See comment under Scanner_next
	switch (self->type) {
		case ERROR_NODE:
			break;
		case COMMAND_NODE:
			StrVec_drop(&(self->data.command));
			break;
		case PIPE_NODE:
			Node_drop(self->data.pipe.left);
			Node_drop(self->data.pipe.right);
			break;
	}
	free(self);
	return NULL;
}
