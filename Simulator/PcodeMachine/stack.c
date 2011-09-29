#include <common.h>
#include <list.h>
#include <pcode.h>

/*
 * Dynamic stack is used in P-machine.
 * We think that P-machine stack is infinite and cannot overflow.
 */
struct stack {
	/* element number for stack bottom stack, normal element for others */
	unsigned int element;
	struct list_head list;
};

#define prev_stack(s) list_entry((s)->list.prev, struct stack, list)
#define next_stack(s) list_entry((s)->list.next, struct stack, list)
#define empty_stack(top, bottom) ((top) == (bottom))

static struct stack bottom;
static struct stack *stack_top;

void init_pm_stack(void)
{
	/* Initial stack element number is zeor.*/
	bottom.element = 0;
	list_init(&bottom.list);
	/* init top */
	stack_top = &bottom;
	printf("[init] stack has been inited\n");
}

void exit_pm_stack(void)
{
	struct stack *s;
	while (!list_empty(&bottom.list)) {
		stack_top = prev_stack(&bottom);
		list_del(&stack_top->list);
		free(stack_top);
	}
	printf("[exit] free stack\n");
}

struct stack *alloc_stack(unsigned int e)
{
	struct stack *s;
	s = xmalloc(sizeof(*s));
	s->element = e;
	list_init(&s->list);
	return s;
}

void push(unsigned int e)
{
	struct stack *new;
	if (next_stack(stack_top) == &bottom) {
		new = alloc_stack(e);
		list_add(&new->list, &stack_top->list);
		stack_top = new;
	} else {
		/* poped stack is cached */
		stack_top = next_stack(stack_top);
		stack_top->element = e;
	}
}

unsigned int top(void)
{
	return stack_top->element;
}

/* lazy delete poped entry, which is cached for new stack entry */
unsigned int pop(void)
{
	unsigned int popone;
	if (stack_top == prev_stack(stack_top)) {
		fprintf(stderr, "the stack is empty\n");
		/* what error code should be returned ? */
		return 0;
	}
	popone = top();
	stack_top = prev_stack(stack_top);
	return popone;
}

/* P-machine push operation */
void pm_push(unsigned int e)
{
	push(e);
}

/* P-machine pop operation */
unsigned int pm_pop(void)
{
	return pop();
}

/* P-machine top operation */
unsigned int pm_top(void)
{
	if (empty_stack(stack_top, &bottom)) {
		fprintf(stderr, "the stack is empty\n");
		/* what error code should be returned ? */
		return 0;
	}
	return top();
}

#ifdef DEBUG

int main(void)
{
	int tmp;
	init_pm_stack();
	pm_push(1);
	pm_push(2);
	pm_push(3);
	pm_push(4);

	tmp = pm_pop();
	fprintf(stderr, "%d\n", tmp);
	tmp = pm_pop();
	fprintf(stderr, "%d\n", tmp);
	tmp = pm_pop();
	fprintf(stderr, "%d\n", tmp);
	tmp = pm_pop();
	fprintf(stderr, "%d\n", tmp);
	/* empty stack */
	tmp = pm_pop();
	fprintf(stderr, "ERROR:%d\n", tmp);
	return 0;
}

#endif
