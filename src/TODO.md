~ 1. An identifier cannot be declared multiple times in the same scope. ~ checkDeclarationsSemantics
~ 2. An identifier must be declared before its use. ~ checkDeclarationsSemantics
~3. An identifier declared globally cannot be declared anywhere else in function definitions. ~ checkDeclarationsSemantics
~4. The types and the number of parameters returned by a function must be the same as that of the parameters used in invoking the function.~ checkFunctionCall
~5. The parameter passed of union data type is reported as error.~ checkFunctionCall
6. The parameter of variant record type (for variant field only) is type checked at run type as described above.
~7. A variant record passed as a parameter must have a discriminator named as tagvalue.~ checkFunctionCall
~9. The function that does not return any value, must be invoked appropriately.~ checkFunctionCall
~10. The types of formal and actual parameters must be same.~ checkFunctionCall
~11. Number of actual parameters must be same as that of formal parameters.~ checkFunctionCall
~12. Function overloading is not allowed.~ checkOverloading
~13. The function cannot be invoked recursively.~ checkFunctionCall
14. An identifier used beyond its scope must be viewed as undefined 
15. A record type definition is visible anywhere in the program.    --??
16. The right hand side expression of an assignment statement must be of the same type as that of the left hand side identifier. --type Checking?
~17. A function definition for a function being used (say F1) by another (say F2) must precede the definition of the function using it(i.e. F2). ~ checkFunctionCall

8. The parameters being returned by a function must be assigned a value. If a parameter does not get a value assigned within the function definition, it  should be reported as an error.
18. A while statement must redefine the variable that participates in the iterations. As an instance, consider the following example