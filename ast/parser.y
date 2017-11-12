%skeleton "lalr1.cc"

%{
    #include <iostream>
    using std::cerr;
    using std::endl;
%}

%debug
%defines
%require "3.0"

%locations

%define api.value.type variant

%code requires {
    #include "ast/tree/NodeTypes.h"
    class Scanner;
}

%parse-param { Scanner &scanner }
%parse-param { NSyntaxTree::Program& program }

%code {
    #include "ast/Scanner.h"

    #undef yylex
    #define yylex scanner.yylex
}

%token T_CLASS
%token T_MAIN
%token T_PRINT
%token T_STRING_ARRAY
%token T_PUBLIC
%token T_RETURN

%token T_IF
%token T_ELSE
%token T_WHILE

%token T_INT_ARRAY
%token T_INT
%token T_BOOL
%token T_TRUE
%token T_FALSE

%token T_EXTENDS
%token T_THIS
%token T_NEW
%token T_LENGTH

%token T_PLUS
%token T_MINUS
%token T_MULTIPLY

%token T_ASSIGN

%token T_AND
%token T_OR
%token T_NOT

%token T_LESS

%token T_DOT
%token T_COMMA

%token T_SEMICOLON ";"

%token T_RBRACE "}"
%token T_LBRACE "{"

%token T_RPAREN ")"
%token T_LPAREN "("

%token T_RBRACKET "]"
%token T_LBRACKET "["

%token <std::string> T_ID
%token <int> T_INT_LITERAL

%token T_EOF 0

%type<NSyntaxTree::MainClass*> MainClass;
%type<std::vector<std::unique_ptr<NSyntaxTree::ClassDeclaration>>*> ClassDeclarations;
%type<NSyntaxTree::ClassDeclaration*> ClassDeclaration;

%type<std::vector<std::unique_ptr<NSyntaxTree::VarDeclaration>>*> VarDeclarations NotEmptyVarDeclarationList;
%type<NSyntaxTree::VarDeclaration*> VarDeclaration;

%type<NSyntaxTree::Type> Type;

%type<std::vector<std::unique_ptr<NSyntaxTree::MethodDeclaration>>*> MethodDeclarations;
%type<NSyntaxTree::MethodDeclaration*> MethodDeclaration;
%type<std::vector<std::pair<NSyntaxTree::Type, std::string>>> ArgumentList NotEmptyArgumentList;


%type<std::vector<std::unique_ptr<NSyntaxTree::IStatement>>*> Statements NotEmptyStatementList;
%type<NSyntaxTree::IStatement*> Statement;

%type<NSyntaxTree::IExpression*> Expression;
%type<std::vector<std::unique_ptr<NSyntaxTree::IExpression>>*> ArgumentExpressions NotEmptyArgumentExpressions;

%nonassoc T_OR
%nonassoc T_AND
%left T_PLUS T_MINUS
%left T_MULTIPLY

%right T_NOT

%left T_DOT
%left T_LBRACKET

%nonassoc T_ASSIGN
%nonassoc T_LESS

%%
Goal
    : MainClass ClassDeclarations T_EOF {
        program.mainClass.reset($1);
        program.classes.reset($2);
    }
;
MainClass
    : T_CLASS T_ID[name] "{"
        T_MAIN "(" T_STRING_ARRAY T_ID[args] ")" "{"
            Statement
        "}"
    "}" {
        $$ = new NSyntaxTree::MainClass($name, $args, $Statement);
    }
;
ClassDeclarations
    : %empty {
        $$ = new std::vector<std::unique_ptr<NSyntaxTree::ClassDeclaration>>();
    }
    | ClassDeclarations ClassDeclaration {
        $$ = $1;
        $$->emplace_back($2);
    }
;
ClassDeclaration
    : T_CLASS T_ID[name] "{"
        VarDeclarations
        MethodDeclarations
    "}" {
        $$ = new NSyntaxTree::ClassDeclaration($name, std::string(), $VarDeclarations, $MethodDeclarations);
    }
    | T_CLASS T_ID[name] T_EXTENDS T_ID[parent] "{"
        VarDeclarations
        MethodDeclarations
    "}" {
        $$ = new NSyntaxTree::ClassDeclaration($name, $parent, $VarDeclarations, $MethodDeclarations);
    }
;
NotEmptyVarDeclarationList
    : VarDeclaration {
        $$ = new std::vector<std::unique_ptr<NSyntaxTree::VarDeclaration>>();
        $$->emplace_back($1);
    }
    | NotEmptyVarDeclarationList VarDeclaration {
        $$ = $1;
        $$->emplace_back($2);
    }
;
VarDeclarations
    : %empty {
        $$ = new std::vector<std::unique_ptr<NSyntaxTree::VarDeclaration>>();
    }
    | NotEmptyVarDeclarationList {
        $$ = $1;
    }
;
VarDeclaration
    : Type T_ID ";" {
        $$ = new NSyntaxTree::VarDeclaration($1, $2);
    }
;
Type
    : T_INT {
        $$ = NSyntaxTree::Type{NSyntaxTree::EType::INT, std::string()};
    }
    | T_INT_ARRAY {
        $$ = NSyntaxTree::Type{NSyntaxTree::EType::INT_ARRAY, std::string()};
    }
    | T_BOOL {
        $$ = NSyntaxTree::Type{NSyntaxTree::EType::BOOL, std::string()};
    }
    | T_ID {
        $$ = NSyntaxTree::Type{NSyntaxTree::EType::CLASS, $1};
    }
;
MethodDeclarations
    : %empty {
        $$ = new std::vector<std::unique_ptr<NSyntaxTree::MethodDeclaration>>();
    }
    | MethodDeclarations MethodDeclaration {
        $$ = $1;
        $$->emplace_back($2);
    }
;
MethodDeclaration
    : T_PUBLIC Type[type] T_ID[name] "(" ArgumentList ")" "{"
        NotEmptyVarDeclarationList
        NotEmptyStatementList
        T_RETURN Expression ";"
    "}" {
        $$ = new NSyntaxTree::MethodDeclaration($type, $name, $ArgumentList, $NotEmptyVarDeclarationList, $NotEmptyStatementList, $Expression);
    }
    | T_PUBLIC Type[type] T_ID[name] "(" ArgumentList ")" "{"
        NotEmptyStatementList
        T_RETURN Expression ";"
    "}" {
        auto varDeclarations = new std::vector<std::unique_ptr<NSyntaxTree::VarDeclaration>>();
        $$ = new NSyntaxTree::MethodDeclaration($type, $name, $ArgumentList, varDeclarations, $NotEmptyStatementList, $Expression);
    }
    | T_PUBLIC Type[type] T_ID[name] "(" ArgumentList ")" "{"
        NotEmptyVarDeclarationList
        T_RETURN Expression ";"
    "}" {
        auto statements = new std::vector<std::unique_ptr<NSyntaxTree::IStatement>>();
        $$ = new NSyntaxTree::MethodDeclaration($type, $name, $ArgumentList, $NotEmptyVarDeclarationList, statements, $Expression);
    }
    | T_PUBLIC Type[type] T_ID[name] "(" ArgumentList ")" "{"
        T_RETURN Expression ";"
    "}" {
        auto varDeclarations = new std::vector<std::unique_ptr<NSyntaxTree::VarDeclaration>>();
        auto statements = new std::vector<std::unique_ptr<NSyntaxTree::IStatement>>();
        $$ = new NSyntaxTree::MethodDeclaration($type, $name, $ArgumentList, varDeclarations, statements, $Expression);
    }
;
NotEmptyArgumentList
    : Type T_ID {
        $$.push_back({$1, $2});
    }
    | NotEmptyArgumentList T_COMMA Type T_ID {
        $$ = std::move($1);
        $$.push_back({$3, $4});
    }
;
ArgumentList
    : %empty {}
    | NotEmptyArgumentList {
        $$ = std::move($1);
    }
NotEmptyStatementList
    : Statement {
        $$ = new std::vector<std::unique_ptr<NSyntaxTree::IStatement>>();
        $$->emplace_back($1);
    }
    | NotEmptyStatementList Statement {
        $$ = $1;
        $$->emplace_back($2);
    }
Statements
    : %empty {
        $$ = new std::vector<std::unique_ptr<NSyntaxTree::IStatement>>();
    }
    | NotEmptyStatementList {
        $$ = $1;
    }
;
Statement
    : "{"
        Statements
    "}" {
        $$ = new NSyntaxTree::Statements($Statements);
    }
    | T_IF "(" Expression ")" Statement[trueStatement] T_ELSE Statement[falseStatement] {
        $$ = new NSyntaxTree::IfStatement($Expression, $trueStatement, $falseStatement);
    }
    | T_WHILE "(" Expression ")" Statement[statement] {
        $$ = new NSyntaxTree::WhileStatement($Expression, $statement);
    }
    | T_PRINT "(" Expression ")" ";" {
        $$ = new NSyntaxTree::PrintlnStatement($Expression);
    }
    | T_ID T_ASSIGN Expression ";" {
        $$ = new NSyntaxTree::AssignStatement($1, $3);
    }
    | T_ID "[" Expression[index] "]" T_ASSIGN Expression[rvalue] ";" {
        $$ = new NSyntaxTree::ArrayElementAssignmentStatement($1, $index, $rvalue);
    }
;
Expression
    : Expression[left] T_AND Expression[right] {
        $$ = new NSyntaxTree::BinaryExpression(NSyntaxTree::EBinaryExprType::AND, $left, $right);
    }
    | Expression[left] T_LESS Expression[right] {
        $$ = new NSyntaxTree::BinaryExpression(NSyntaxTree::EBinaryExprType::LESS, $left, $right);
    }
    | Expression[left] T_PLUS Expression[right] {
        $$ = new NSyntaxTree::BinaryExpression(NSyntaxTree::EBinaryExprType::PLUS, $left, $right);
    }
    | Expression[left] T_MINUS Expression[right] {
        $$ = new NSyntaxTree::BinaryExpression(NSyntaxTree::EBinaryExprType::MINUS, $left, $right);
    } 
    | Expression[left] T_MULTIPLY Expression[right] {
        $$ = new NSyntaxTree::BinaryExpression(NSyntaxTree::EBinaryExprType::MULTIPLY, $left, $right);
    }

    | Expression[array] "[" Expression[index] "]" {
        $$ = new NSyntaxTree::ArrayElementAccessExpression($array, $index);
    }
    | Expression[array] T_DOT T_LENGTH {
        $$ = new NSyntaxTree::ArrayLengthExpression($1);
    }
    | Expression[object] T_DOT T_ID[methodName] "(" ArgumentExpressions[args] ")" {
        $$ = new NSyntaxTree::MethodCallExpression($object, $methodName, $args);
    }
    
    | T_INT_LITERAL {
        $$ = new NSyntaxTree::IntegerLiteralExpression($1);
    }
    | T_TRUE {
        $$ = new NSyntaxTree::BoolLiteralExpression(true);
    }
    | T_FALSE {
        $$ = new NSyntaxTree::BoolLiteralExpression(false);
    }
    | T_ID {
        $$ = new NSyntaxTree::IdentifierExpression($1);
    }
    | T_THIS {
        $$ = new NSyntaxTree::ThisExpression();
    }
    | T_NEW T_INT "[" Expression[size] "]" {
        $$ = new NSyntaxTree::NewIntArrayExpression($size);
    }
    | T_NEW T_ID[clazz] "(" ")" {
        $$ = new NSyntaxTree::NewExpression($clazz);
    }
    | T_NOT Expression[expression] {
        $$ = new NSyntaxTree::NegateExpression($expression);
    }
    | "(" Expression[expression] ")" {
        $$ = $expression;
    }
;
NotEmptyArgumentExpressions
    : Expression {
        $$ = new std::vector<std::unique_ptr<NSyntaxTree::IExpression>>();
        $$->emplace_back($1);
    }
    | ArgumentExpressions T_COMMA Expression {
        $$ = $1;
        $$->emplace_back($3);
    }
;
ArgumentExpressions
    : %empty {
        $$ = new std::vector<std::unique_ptr<NSyntaxTree::IExpression>>();
    }
    | NotEmptyArgumentExpressions {
        $$ = $1;
    }
;
%%

void yy::parser::error(const yy::parser::location_type& l, const std::string &err_message) {
    cerr << l << ": " << err_message << endl;
}


int main() {
    Scanner scanner(&std::cin);
    NSyntaxTree::Program program;
    yy::parser parser(scanner, program);
    parser.parse();
}
