# Compiler Final Project
In this final project, your goal is to craft a Mini-LISP interpreter. All resources you need are on the LMS, including the language specifications, a standard interpreter (called smli) and some Mini-LISP program examples for testing. 
  
Your tasks are to :  
    1. Read the language specifications of Mini-LISP.  
    2. Read and run Mini-LISP program examples to understand the language behavior.  
    3. Write an interpreter for Mini-LISP, implement features of the language.  

按照給予的Compiler Final Project.pdf的要求和MiniLisp.pdf的文法完成一個簡易的Mini-LISP interpreter。  
須完成以下可選項目:  
| **Feature** | **Feature** |
|-------|:-----:|
| Syntax Validation | Print “syntax error” when parsing invalid syntax |
| Print | Implement print-num statement |
| Numerical Operations | Numerical Operations |
| Logical Operations | Implement all logical operations |
| if Expression | Implement if expression |
| Variable Definition | Able to define a variable  |
| Function | Able to declare and call an anonymous function |
| Named Function | Able to declare and call a named function |

## Grammar Overview
PROGRAM ::= STMT+  
STMT ::= EXP | DEF-STMT | PRINT-STMT  
PRINT-STMT ::= (print-num EXP) | (print-bool EXP)  
EXP ::= bool-val | number | VARIABLE | NUM-OP | LOGICAL-OP | FUN-EXP | FUN-CALL | IF-EXP  
NUM-OP ::= PLUS | MINUS | MULTIPLY | DIVIDE | MODULUS | GREATER | SMALLER | EQUAL  
    PLUS ::= (+ EXP EXP+)  
    MINUS ::= (- EXP EXP)  
    MULTIPLY ::= (* EXP EXP+)  
    DIVIDE ::= (/ EXP EXP)  
    MODULUS ::= (mod EXP EXP)  
    GREATER ::= (> EXP EXP)  
    SMALLER ::= (< EXP EXP)  
    EQUAL ::= (= EXP EXP+)  
LOGICAL-OP ::= AND-OP | OR-OP | NOT-OP  
    AND-OP ::= (and EXP EXP+)  
    OR-OP ::= (or EXP EXP+)  
    NOT-OP ::= (not EXP)  
DEF-STMT ::= (define VARIABLE EXP)  
    VARIABLE ::= id  
FUN-EXP ::= (fun FUN_IDs FUN-BODY)  
    FUN-IDs ::= (id*)  
    FUN-BODY ::= EXP  
    FUN-CALL ::= (FUN-EXP PARAM*) | (FUN-NAME PARAM*)  
    PARAM ::= EXP  
    LAST-EXP ::= EXP  
    FUN-NAME ::= id  
IF-EXP ::= (if TEST-EXP THAN-EXP ELSE-EXP)  
    TEST-EXP ::= EXP  
    THEN-EXP ::= EXP  
    ELSE-EXP ::= EXP  
