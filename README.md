# README Projet informatique SICOM 2022

Author: Morgane Magnier, Baptiste Rault

###README

## Directory structure:

#Makefile : Pour créer des fichiers objets (*.o) et des executables

#src : C source code
src/list.c : abstract type list functions implementation
src/queue.c : abstract type queue functions implementation

src/chargroup.c : chargroup structure functions implementation & chargroup parse function
src/lexem.c : abstract type list functions implementation
src/lex_def : lex_def structure for lexeme definition file
src/label.c : structure to gerer les étiquettes dans le bytecode
src/pyobj.c : python object structure

src/regexp_char_group.c : rematch function for regular expression matching
src/read.c : read() function to make a lex_def list with the lexeme definition file given
src/lexem_src.c : lex() function (final function) for lexical analysis
src/parse.c : parse() function to verify that the lex in the source file are in a good order
src/parse_py.c : parse_py() function to create a python object
src/serialise.c : pyasm() function to create a binary file with the python object

#prog : Fichiers executables
prog/matching.c : matching of a regular expression in parameter
prog/parsing.c : parsing of a regular expression in parameter
prog/lexer.c : .pys file to lexem list with lexeme definition file given
prog/parser.c : .pys file to a python object
prog/pyas.c : .pys file to .pyc file

#include : headers (*.h) of the project
include/pyas : headers for the entire source code
include/unitest : headers for unitest
tests/unit : unitary tests for the project
tests/integration :
bin/unit : test and regular executables
files : test files for lexical analysis

#tests
tests/banque_py : useful .pys and py_pyc files for tests
tests/integration : all integration tests
tests/unit : all unit tests    

## Utilisation

commande make dans le terminal pour compiler tout les fichiers.c (grace au Makefile)
bin/"nom_de_lexecutable" pour exectuer un fichier prog
bin/unit/"test_name.exe" pour executer un test unitaire
