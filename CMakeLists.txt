set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wno-maybe-uninitialized -Wno-reorder")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wno-uninitialized")

cmake_minimum_required(VERSION 3.1)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

bison_target(parser
        ast/parser.y
        ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc)

flex_target(lexer
        ast/lexer.l
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)

add_flex_bison_dependency(lexer parser)

set(SyntaxTreeSrc
        ast/tree/Program.cpp
        ast/tree/Class.cpp
        ast/tree/VarDeclaration.cpp
        ast/tree/MethodDeclaration.cpp
        ast/tree/Statement.cpp
        ast/tree/Expression.cpp
        )

set(Visitors
        ast/tree/visitors/PrettyPrinterVisitor.cpp
        irt/translator/IRPrettyPrinter.cpp
        )

set(SymbolTable
        symbol_table/TypeInfo.cpp
        symbol_table/VariableInfo.cpp
        symbol_table/MethodInfo.cpp
        symbol_table/ClassInfo.cpp
        symbol_table/SymbolTable.cpp
        symbol_table/SymbolTableCreatorVisitor.cpp
        symbol_table/ScopeSwitcher.cpp)

set(Util
        util/Symbol.cpp
        util/StringInterner.cpp
        )

set(TreeBuilder
        ast/TreeBuilder.cpp
        ${SyntaxTreeSrc}
        ${FLEX_lexer_OUTPUTS}
        ${BISON_parser_OUTPUTS}
        ${Util}
        )

set(SymbolTableBuilder
        symbol_table/SymbolTableBuilder.cpp
        symbol_table/SymbolTableBuilder.h
        ${SymbolTable}
        ${TreeBuilder}
        )

set(TypeChecker
        ${SymbolTableBuilder}
        type_checker/DependencyChecker.cpp
        type_checker/TypeCheckerVisitor.cpp
        type_checker/Exceptions.cpp)

set(ActivationRecords
        ${SymbolTable}
        irt/activation_records/X86/X86MiniJavaFrame.cpp
        irt/activation_records/InRegAccess.cpp
        irt/activation_records/InFrameAccess.cpp
        irt/activation_records/X86/X86MiniJavaClassStruct.cpp
        irt/activation_records/ArrayStruct.cpp
        )

set(IRTree
        ${ActivationRecords}
        irt/INode.h
        irt/Label.cpp

        irt/expressions/Temp.cpp
        irt/Accept.cpp
        )

set(IRTranslate
        irt/translator/IRBuilderVisitor.cpp
        irt/translator/NameConventions.h
        )

set(IRTCanonise
        irt/translator/Canoniser.cpp
        )

set(CodeGeneration
        code_generation/Instructions.h
        code_generation/X86/X86CodeGeneration.cpp
        code_generation/X86/InstructionSet.cpp
        )

add_executable(printer
        tools/dot_printer.cpp
        ${TreeBuilder}
        ${SymbolTableBuilder}
        ${Visitors}
        ${IRTree}
        ${IRTranslate}
        ${IRTCanonise}
        ${CodeGeneration}
        irt/translator/X86IRBuilder.cpp
        )

add_executable(symbol_table_test
        tools/symbol_table_test.cpp
        ${SymbolTableBuilder}
        )

add_executable(type_checker
        tools/type_checker.cpp
        ${TypeChecker}
        ${IRTree}
        ${IRTranslate}
        )


include_directories(. ${CMAKE_CURRENT_BINARY_DIR})
