This is the repo of the programming project of CS2205-2025Fall in SJTU.

### Tokens:

- Operations + - * / % < <= == != > >= ! && ||
- Assignment <-
- Spacer ( ) : ;; , { }
- Natural Number 0|[1-9][0-9]*
- Identity [_A-Za-z][_A-Za-z0-9]*
- Reserved Words if else loop_init loop_body continue break skip true false

### Grammar:

#### Indentation

Similar to Python, we use indentation to distinguish code blocks.

4 spaces per indent level. We automatically add "{" when indent level increase end "}" when decrease.

The commands enclosed in a pair of braces is regarded as a command block.

#### Expression

 Very similar to that of C, with the same priority.

#### Commands

 In order to handle the colon properly, We divide basic command structures into two types: single commands and blocked commands.

 And the final result must form a "command block", which will also be specified later.

###### Single Commands

- break, continue, skip
- Assignment: [expr] <- [expr]
  e.g. a <- 1
- Declaration: Var [var_name]
  e.g. Var a
- Expression: [expr]
  e.g. cmp(a1, a2)

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
  ```
- We have not supported comments in the code. all `//`-style comments in the above examples are just for convinience. They can not be recognized or paresed (and result in "syntax error").



TO DO LIST.

- [x] Design the grammer
- [x] Lexer
- [x] Parser
- [ ] Print to SetMonad
- [ ] Write the documenet

