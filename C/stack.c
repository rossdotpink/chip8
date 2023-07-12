#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"

typedef struct _ch8_stack_t ch8_stack_t;
struct _ch8_stack_t {
  ch8_stack_t* previous;
  uint8_t value;
  int count;
};

ch8_stack_t* new_stack() {
  ch8_stack_t* stack = (ch8_stack_t*) malloc(sizeof(ch8_stack_t));
  stack->count = 0;
  return stack;
}

void push(ch8_stack_t** stack, uint8_t value) {
  // if((*stack)->count == 0) {
  //   // printf("It's zero\n");
  //   (*stack)->value = value;
  //   (*stack)->count += 1;
  //   (*stack)->previous = NULL;
  //   return;
  // }

  // printf("Update\n\n");
  // printf("Current value is %d\n", (*stack)->count);
  ch8_stack_t* new_stack = (ch8_stack_t*) malloc(sizeof(ch8_stack_t));
  new_stack->value = value;
  new_stack->count = ((*stack)->count + 1);
  printf("stack \t= %p\tstack->previous \t= %p\tnew_stack->previous \t= %p\t\n", stack, (*stack)->previous, new_stack->previous);
  new_stack->previous = (*stack);
  printf("stack \t= %p\tstack->previous \t= %p\tnew_stack->previous \t= %p\t\n", stack, (*stack)->previous, new_stack->previous);


  printf("PUSH\t(*stack)->previous \t= %p\t(*stack)->count \t= %d\t(*stack)->value \t= %d\n", (*stack)->previous, (*stack)->count, (*stack)->value);
  printf("PUSH\t(*new_stack)->previous \t= %p\t(*new_stack)->count \t= %d\t(*new_stack)->value \t= %d\n", new_stack->previous, new_stack->count, new_stack->value);

  
  (*stack) = new_stack;
  return;

}

uint8_t pop(ch8_stack_t** stack) {
  // If it's all no good, return zero;
  if((*stack)->previous == NULL || (*stack)->count == NULL || (*stack)->count == 0) {
    return 0;
  }
  printf("stack \t= %p\tstack->previous \t= %p\n", stack, (*stack)->previous);

  // // Save the top of (*stack) value
  uint8_t return_value = (*stack)->value;

  // // Save pointer to the new head
  // ch8_stack_t** new_head = (*stack)->previous;
  // printf("POP\t(*stack)->previous \t= %p\t(*stack)->count \t= %d\t(*stack)->value \t= %d\n", (*stack)->previous, (*stack)->count, (*stack)->value);
  // printf("POP\t(*new_head)->previous \t= %p\t(*new_head)->count \t= %d\t(*new_head)->value \t= %d\n", (*new_head)->previous, (*new_head)->count, (*new_head)->value);

  // // Free the current head's memory
  // // free(*stack);

  // // Point the existing stack variable to the new head
  // *stack = *new_head;

  *stack = (*stack)->previous;


  // Return the value
  return return_value;

  // return 0;
}
