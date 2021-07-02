#ifndef ILRD_CALCULATOR_H
#define ILRD_CALCULATOR_H

/* Calculate:
*   - calculates the result of an equation represented as a string
* 
* Args:
*   - input - the equation to solve
*
* Return Value: 
*	- returns the result as a floating point number
*
* Comments: 
*	- assuming input is valid, includes only non negative real numbers
*	- operations can include: + - / * and parentheses ()
*	- following arithmetic precedence order
*/
float Calc(const char *str);

#endif /* ILRD_CALCULATOR_H */
