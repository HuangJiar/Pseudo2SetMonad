This is the repo of the programming project of CS2205-2025Fall in SJTU.

### Tokens:

- Operations + - * / % < <= == != > >= ! && ||
- Assignment <-
- Spacer ( ) : ;; , { }
- Natural Number 0|[1-9][0-9]*
- Identity [_A-Za-z][_A-Za-z0-9]*  
  **Note: We additionally require that the identity can not be '\_', since this is used as a reserved word.**
- Reserved Words _ if else loop_init loop_body continue break skip true false 

### Grammar:

#### Indentation

Similar to Python, we use indentation to distinguish code blocks.

4 spaces per indent level. In practice we automatically add "{" when indent level increase end "}" when decrease. (Adding braces manually to avoid add indentation is allowed. But **don't use both of them simutaniously**.)

The commands enclosed in a pair of braces is regarded as a command block.

#### Expression

 Very similar to that of C, with the same priority.


#### Commands

 In order to handle the colon properly, We divide basic command structures into two types: single commands and blocked commands.

 And the final result must form a "command block", which will also be specified later.

###### Single Commands

- break, continue, skip
- Assignment: [expr] <- [expr]
  e.g. `a <- 1`
- Declaration: Var [var_name]
  e.g. `Var a`
- Function expression: [expr_fun]
  e.g. `cmp(a1, a2)`
  Note: expr of other types can be parsed properly, but can not be translated to Coq program properly. e.g. `1+1;;`

###### Blocked Commands

- If-else structure: 
  Type 1: if ([expr]): {[command block]}
  Type 2: if ([expr]): {[command block]} else: {[command block]}
  e.g.
  ```
  // example 1
  if (z > 25):
      result <- 3
  else:
      result <- 2
  // example 2
  // We can also write in a more compact form 
  // by mannually add braces.
  if (total > 500): {flag <- true}
  else: {flag <- false}
  ```
- Loop structure: loop_begin: {[command block]} loop_body: {[command block]}
  e.g.
  ```
  loop_init:
      sum <- 0;;
      i <- 0;;
  loop_body:
      if (i < 5):
          j <- 0;;
      else:
          break;;
  report(sum)
  ```
  Also, we allow the loop_init block to be empty.
  e.g.
  ```
  loop_init:
  loop_body:
      a <- a+i;;
      i <- i+1;;
      if (i > 10):
          break
  ```
  similar to if, we can manually add braces to write in a more compact form.

###### Command Block

  The block is comprised of a series of single commands, blocked commands and colons.

  For any single commands, if it is not the last command, then it must be ended with a colon.

  We do not allow empty command block.

  Such design is aimed to fit the example pseudo code.

  e.g.
  ```
  a <- -1;;
  __12a <-
  3
  ;; ;; ;; ;;
  if (a > b):
      _ <- 4-a
  else:
      
  
      fun(a, b)

  exe(path)
  // valid.
  ```  
  ```
  a <- 3
  b <- 6;;
  // invalid
  if (a > 3): {}
  else: {}
  // invalid
  ```

#### Notice

- Do not use indentation and braces at will. braces and indentations are only used in `if` and `loop` blocks. e.g.

  ```
  a <- 1
  {b <- 1}
  // invalid

  a <- 1
      b <- 1
  // invalid

  if (a < 0):{
    a <- 1
  }
  // invalid
  if (a < 0): a <- 1
  // invalid
  ```
- We have not supported comments in the code. all `//`-style comments in the above examples are just for convinience. They can not be recognized or paresed (and result in "syntax error").

### Translating to Coq Program

  We assume that all functions that are used as a single command (e.g. `cmp(a1, a2)`) are of the type `T1 -> T2 -> ... -> Tn -> SetMonad.M Z`, where `n` is the number of variables. And all functions that are used in arithmatic calculations (e.g. `fib(n1)+fib(n2)`), are of the type `T1 -> T2 -> ... -> Tn -> Z`. Such assumption is for convenience. We further assume that the function does not change the value of variables. (We are not sure whether we need to support that)

  We assume that our program have name `main`, which is of type SetMonad.unit. First let's take a look at a simple example.

  example:
  initial psedue code:
  ```
  a1 <- average(S1);;
  a2 <- average(S2);;
  cmp(a1, a2)
  ```
  Our translation:
  ```
  Definition main : SetMonad.M unit :=
    a1 <- average S1;;
    a2 <- average S2;;
    _ <- cmp a1 a2;;
    ret tt.
  ```
  A possible series of definition of `average`,`cmp`,`S1`,`S2`:
  ```
  Fixpoint sum(L: list Z) : SetMonad.M Z := 
  match L with
  | nil => ret 0
  | a :: L1 =>
      x <- sum(L1);;
      ret (x+a)
  end.

  Definition average(L: list Z): SetMonad.M Z :=
    x <- sum L ;; y <- ret(Z.of_nat (length L)) ;; ret (x/y).

  Definition S1 : list Z := 1::2::nil.
  Definition S2 : list Z := 4::nil.

  Definition cmp: Z -> Z -> SetMonad.M Z :=
    fun x y => ret(1).
  ```

  Although the above example shows that Coq program with `list Z` can also fit with the code we generate, we recommand that you only use this program to translate psedue code only involving `Z` to Coq program.

  ###### Adjustment involving `if` structure

  When we need to deal with if, we will make small changes to the original code. We will paste the commands after the `if` block into the two command blocks of `if`. More specifically, we will translate the structure

  ```
  if ([expr]):
      [command block1]
  else: 
      [command block2]
  [command block3]
  ```

  to

  ```
  if ([expr]):
      [command block1](;;)
      [command block3]
  else: 
      [command block2](;;)
      [command block3]
  ```

  Here (;;\) means we only add colons when needed. We will repeat this process if the last command in either command block1 or command block2 is `if` block.

  Easy to observe that the two forms are equivalent in result. Such adjustment is to fit the grammer of SetMonad.

  ###### Adjustment involving `break` and `continue`

  We will automatically delete all commands after break and continue. This does not change the result of the program. Such design is also aimed to fit Coq.

  We will not check whether `break` or `continue` is used outside a loop.(In that case the result Coq code can not be properly parsed by Coq) We assume that the input pseudo code is valid.

### How to run

 ```
 make
 ./main < [code file]
 ```
 
 If you want to see the code generated after adding braces, you can add parameter `-V`. Then the code will be printed to `stderr`.


TO DO LIST.

- [x] Design the grammer
- [x] Lexer
- [x] Parser
- [x] Print to SetMonad
- [x] Write the documenet

