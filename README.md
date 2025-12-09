This is the repo of the programming project of CS2205-2025Fall in SJTU.

Tokens:

- Operations + - * / % < <= == != > >= ! && ||
- Assignment <-
- Spacer ( ) : ;; , { }
- Natural Number 0|[1-9][0-9]*
- Identity [_A-Za-z][_A-Za-z0-9]*
- Reserved Words if else loop_init loop_body continue break skip true false

Grammar:

Similar to Python, we use indentation to distinguish code blocks.


I have add -g in Makefile. Remember of delete it while the project is handed in.

TO DO LIST.

- [x] Design the grammer
- [x] Lexer
- [x] Parser
- [ ] Print to SetMonad
- [ ] Write the documenet

