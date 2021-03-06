#pragma once

#include "IVisitor.h"

#include <ast/tree/NodeTypes.h>
#include <util/DotPrinter.h>

#include <iostream>
#include <string>

namespace NSyntaxTree {
    class PrettyPrinterVisitor : public IVisitor, public NUtil::DotPrinter {

        std::ostream& outPut;
    public:
        inline explicit PrettyPrinterVisitor(std::ostream& _outPut = std::cout) : outPut(_outPut) {}
        ~PrettyPrinterVisitor() = default;

        void printVertex(const INode* node, const std::string& label);
        void printEdge(const INode* from, const INode * to);
        void printEdge(const INode* from, const INode * to, const std::string& label);
        void printEdge(const std::string& from, const INode* to);
        void printEdge(const INode* from, const std::string& to);

        void Visit(const Program *) override;

        void Visit(const ClassDeclaration *) override;
        void Visit(const MainClass *) override;

        void Visit(const VarDeclaration *) override;

        void Visit(const MethodDeclaration *) override;

        void Visit(const Statements *) override;
        void Visit(const IfStatement *) override;
        void Visit(const WhileStatement *) override;
        void Visit(const PrintlnStatement *) override;
        void Visit(const AssignStatement *) override;
        void Visit(const ArrayElementAssignmentStatement *) override;

        void Visit(const BinaryExpression *) override;
        void Visit(const ArrayElementAccessExpression *) override;
        void Visit(const ArrayLengthExpression *) override;
        void Visit(const MethodCallExpression *) override;
        void Visit(const IntegerLiteralExpression *) override;
        void Visit(const BoolLiteralExpression *) override;
        void Visit(const IdentifierExpression *) override;
        void Visit(const ThisExpression *) override;
        void Visit(const NewIntArrayExpression *)  override;
        void Visit(const NewExpression *) override;
        void Visit(const NegateExpression *) override;
    };
}
