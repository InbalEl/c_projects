/************************************
* Project name: calculator	    	*
* Develoer: Inbal Elmalech	    	*
* Reviewer:	Tali Raphael	    	*
* Submitted on: Aug 1, 2020         *
************************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stdlib.h> /* strtod, allocs, frees, size_t, exit status */
#include <assert.h> /* assert */
#include <string.h> /* strlen */
#include <stdio.h> /* printf, puts */
#include "stack.h" /* stack_t */
#include "calculator.h"

#define ASCII 128

/****************************   DECLARATIONS   *******************************/
/*************************    Parsing Functions    ***************************/

char *ParseOperator(char *ch, stack_t *op_s, stack_t *num_s);
char *PushToNumStack(char *ch, stack_t *op_s, stack_t *num_s);
char *PushToOpStack(char *ch, stack_t *op_s, stack_t *num_s);
int DoOpAndPushRes(char *ch, stack_t *op_s, stack_t *num_s);
char *DoOpInLoop(char *ch, stack_t *op_s, stack_t *num_s);

/************************    operator Functions    ***************************/

double Add(double operand1, double operand2);
double Subtract(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double Divide(double operand1, double operand2);

/*************************    Utility Functions    ***************************/

int IsPresHigher(char ch1, char ch2);
char *InvalidSym(char *ch, stack_t *op_s, stack_t *num_s);
void InitAsciiParsingLut(void);

/************************   TYPEDEFS AND STRUCTURES   ************************/

typedef char *(*sym_parse_func_t)(char *ch, stack_t *op_s, stack_t *num_s);
typedef double (*do_op_func_t)(double operand1, double operand2);

typedef struct 
{
    sym_parse_func_t parsing_func;
    do_op_func_t do_op_func;
    int precedence;
}lut_elem_t;

static lut_elem_t ascii_parsing_lut[ASCII] = {{InvalidSym, NULL, 0}};

/*************************    General Functions    ***************************/

float Calc(const char *str)
{
    double *tmp_result = NULL;
    float result = 0.0 / 0.0;
    char *curr_ch = (char *)str;
    stack_t *op_stack = NULL;
    stack_t *num_stack = NULL;

    assert(str);

    printf("char is %s\n", curr_ch);
    /* creating stacks */
    op_stack = StackCreate(strlen(str));
    num_stack = StackCreate(strlen(str));

    /* making sure stacks were created */
    if (NULL == op_stack)
    {
        return (result);
    }

    if (NULL == num_stack)
    {
        free(op_stack);
        op_stack = NULL;
        return (result);
    }

    /*
    * Parsing next symbol while we haven't reached the end of the string
    * (symbol being either an operator or an operand)
    */
    while (curr_ch && *curr_ch)
    {
        curr_ch = ascii_parsing_lut[(int)*curr_ch].parsing_func
        (curr_ch, op_stack, num_stack);
    }

    /* TODO: GET RID OF THIS LOOP */
    while (!StackIsEmpty(op_stack))
    {
        DoOpAndPushRes(curr_ch, op_stack, num_stack);
    }

    tmp_result = (double *)StackPeek(num_stack);
    StackPop(num_stack);

    return *((float *)tmp_result);
}

/**************************** Parsing Functions ******************************/

char *ParseOperator(char *ch, stack_t *op_s, stack_t *num_s)
{
    return (ascii_parsing_lut[(int)*ch].(ch, op_s, num_s));
}

/*---------------------------------------------------------------------------*/

char *PushToNumStack(char *ch, stack_t *op_s, stack_t *num_s)
{
    char **endptr = (char **)ch;
    double *num = (double *)malloc(sizeof(double));

    if (NULL == num)
    {
        return (NULL);
    }

    (void)op_s;

    *num = strtod(ch, endptr);
    StackPush(num_s, (void *)num);
    return (*endptr);
}

/*---------------------------------------------------------------------------*/

char *PushToOpStack(char *ch, stack_t *op_s, stack_t *num_s)
{
    char *operator = (char *)malloc(sizeof(char));
    
    if (NULL == operator)
    {
        return (NULL);
    }
    
    (void)num_s;
    
    *operator = *ch;

    StackPush(op_s, (void *)operator);

    return (ch + 1);
}

/*---------------------------------------------------------------------------*/

int DoOpAndPushRes(char *ch, stack_t *op_s, stack_t *num_s)
{
    double *operand2 = (double *)StackPeek(num_s);
    StackPop(num_s);
    double *operand1 = (double *)StackPeek(num_s);
    StackPop(num_s);
    char *operator = (char *)StackPeek(op_s);
    StackPop(op_s);

    (void)ch;

    *operand1 = ascii_parsing_lut
                [(int)*operator].do_op_func(*operand1, *operand2);

    free(operand2);
    operand2 = NULL;

    free(operator);
    operator = NULL;

    return (StackPush(num_s, operand1));
}

/*---------------------------------------------------------------------------*/

char *DoOpInLoop(char *ch, stack_t *op_s, stack_t *num_s)
{
    while (!(StackIsEmpty(op_s)) &&
           !(IsPresHigher(*ch, *(char *)StackPeek(op_s))))
    {
        DoOpAndPushRes(ch, op_s, num_s);
    }
    
    PushToOpStack(ch, op_s, num_s);

    return (ch + 1);
}

/*---------------------------------------------------------------------------*/

char *DoMathOp(char *ch, stack_t *op_s, stack_t *num_s)
{
    double *operand2 = (double *)StackPeek(num_s);
    StackPop(num_s);
    double *operand1 = (double *)StackPeek(num_s);
    StackPop(num_s);
    char *operator = (char *)StackPeek(op_s);
    StackPop(op_s);

    (void)ch;

    *operand1 = ascii_parsing_lut
                [(int)*operator].do_op_func(*operand1, *operand2);

    free(operand2);
    operand2 = NULL;

    free(operator);
    operator = NULL;

    return ();
}

/***************************** Utilty Functions ******************************/

int IsPresHigher(char ch1, char ch2)
{
    return (ascii_parsing_lut[(int) ch1].precedence >
            ascii_parsing_lut[(int) ch2].precedence);
}

/*---------------------------------------------------------------------------*/

char *InvalidSym(char *ch, stack_t *op_s, stack_t *num_s)
{
    (void)ch;
    (void)op_s;
    (void)num_s;

    return (NULL);
}

/************************    LUT Initialazation    ***************************/

void InitAsciiParsingLut(void)
{
    int i = 0;

    ascii_parsing_lut[40].parsing_func = ParseOperator; /* ) */
    ascii_parsing_lut[40].do_op_func = PushToOpStack;
    ascii_parsing_lut[40].precedence = 1;

    ascii_parsing_lut[40].parsing_func = ParseOperator; /* ( */
    ascii_parsing_lut[40].do_op_func = DoOpInLoop;
    ascii_parsing_lut[40].precedence = 2;

    ascii_parsing_lut[40].parsing_func = ParseOperator; /* * */
    ascii_parsing_lut[40].do_op_func = Multiply;
    ascii_parsing_lut[40].precedence = 4;

    ascii_parsing_lut[40].parsing_func = ParseOperator; /* + */
    ascii_parsing_lut[40].do_op_func = Add;
    ascii_parsing_lut[40].precedence = 3;

    ascii_parsing_lut[40].parsing_func = ParseOperator; /* - */
    ascii_parsing_lut[40].do_op_func = Subtract;
    ascii_parsing_lut[40].precedence = 3;

    ascii_parsing_lut[40].parsing_func = ParseOperator; /* / */
    ascii_parsing_lut[40].do_op_func = Divide;
    ascii_parsing_lut[40].precedence = 4;

    /* 0 - 9 */
    for (i = 48; i < 58; ++i)
    {
        ascii_parsing_lut[i].parsing_func = PushToNumStack;
        ascii_parsing_lut[i].do_op_func = PushToNumStack;
        ascii_parsing_lut[i].precedence = 0;
    }
} 

/************************    operator Functions    ***************************/

double Add(double operand1, double operand2)
{
    return (operand1 + operand2);
}

/*---------------------------------------------------------------------------*/

double Subtract(double operand1, double operand2)
{
    return (operand1 - operand2);
}

/*---------------------------------------------------------------------------*/

double Multiply(double operand1, double operand2)
{
    return (operand1 * operand2);
}

/*---------------------------------------------------------------------------*/

double Divide(double operand1, double operand2)
{
    return (operand1 / operand2);
}

/*****************************************************************************/



