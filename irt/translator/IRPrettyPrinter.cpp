#include "IRPrettyPrinter.h"

namespace NIRTree {
    void IRPrettyPrinter::printVertex(const INode *node, const std::string &label) {
        outPut << "\tnode" << node << "[label=\"" << label << "\"]\n";
    }

    void IRPrettyPrinter::printEdge(const INode *from, const INode *to) {
        outPut << "\tnode" << from << "->" << "node" << to << "\n";
    }

    void IRPrettyPrinter::printEdge(const INode *from, const INode *to, const std::string &label) {
        outPut << "\tnode" << from << "->" << "node" << to << "[label=\"" << label << "\"]\n";
    }

    void IRPrettyPrinter::Visit(const Binop *node) {
        static const std::vector<std::string> types = {"AND", "OR", "PLUS", "MINUS", "MULTIPLY"};

        printVertex(node, std::string("Binop ") + types.at(node->binop));

        node->leftExp->Accept(this);
        printEdge(node, (node->leftExp).get());

        node->rightExp->Accept(this);
        printEdge(node, (node->rightExp).get());
    }

    void IRPrettyPrinter::Visit(const Call *node) {
        printVertex(node, std::string("Call"));

        node->funcExp->Accept(this);
        printEdge(node, (node->funcExp).get());

        node->args->Accept(this);
        printEdge(node, (node->args).get());
    }
    
    void IRPrettyPrinter::Visit(const Const *node) {
        printVertex(node, std::to_string(node->value));
    }
    
    void IRPrettyPrinter::Visit(const ESeq *node) {
        printVertex(node, std::string("ESeq"));
        
        node->stm->Accept(this);
        printEdge(node, (node->stm).get());

        node->exp->Accept(this);
        printEdge(node, (node->exp).get());
    }

    void IRPrettyPrinter::Visit(const Mem *node) {
        printVertex(node, std::string("Mem"));

        node->exp->Accept(this);
        printEdge(node, (node->exp).get());
    }
    
    void IRPrettyPrinter::Visit(const Name *node) {
        printVertex(node, std::string("Name ") + node->label->String());
    }
        
    void IRPrettyPrinter::Visit(const Temp *node) {
        static const std::vector<std::string> types = {"ID", "NAME"};

        if (node->name != "") {
            printVertex(node, std::string("Temp ") + types.at(node->type) + " " + node->name);
        }
        else if (node->localId != -1) {
            printVertex(node, std::string("Temp ") + types.at(node->type) + " " + std::to_string(node->localId));         
        }
        else {
            printVertex(node, std::string("Temp ") + types.at(node->type) + " " + node->name + " " + std::to_string(node->localId));    
        }
    }
        
    void IRPrettyPrinter::Visit(const Unop *node) {
        static const std::vector<std::string> types = {"NOT"};

        printVertex(node, std::string("Unop ") + types.at(node->type));

        node->expression->Accept(this);
        printEdge(node, (node->expression).get());
    }

    void IRPrettyPrinter::Visit(const CJump *node) {
        static const std::vector<std::string> types = {"EQ", "NEQ", "LT"};

        printVertex(node, std::string("CJump ") + types.at(node->jumpType) + " trueLabel: " + 
            node->trueLabel->String() + " falseLabel: " + node->falseLabel->String());

        node->leftExpr->Accept(this);
        printEdge(node, (node->leftExpr).get(), "leftExpr");

        node->rightExpr->Accept(this);
        printEdge(node, (node->rightExpr).get(), "rightExpr");
    }
   
    void IRPrettyPrinter::Visit(const Exp *node) {
        printVertex(node, std::string("Exp"));

        node->expr->Accept(this);
        printEdge(node, (node->expr).get());
    }
        
    void IRPrettyPrinter::Visit(const Jump *node) {
        printVertex(node, std::string("Jump ") + node->label->String());
    }
        
    void IRPrettyPrinter::Visit(const Move *node) {
        printVertex(node, std::string("Move"));
        
        node->dst->Accept(this);
        printEdge(node, (node->dst).get(), "dst");

        node->src->Accept(this);
        printEdge(node, (node->src).get(), "src");
    }

    void IRPrettyPrinter::Visit(const LabelStm* node) {
        printVertex(node, std::string("LabelStm ") + node->label->String());
    }

    void IRPrettyPrinter::Visit(const ExprWrapper *node) {
        printVertex(node, std::string("ExprWrapper"));
        
        node->GetExpression()->Accept(this);
        printEdge(node, (node->GetExpression()).get());
    }
        
    void IRPrettyPrinter::Visit(const StmWrapper *node) {
        printVertex(node, std::string("StmWrapper"));

        node->GetStatement()->Accept(this);
        printEdge(node, (node->GetStatement()).get());
        
    }

    void IRPrettyPrinter::Visit(const StmList *node) {
        printVertex(node, std::string("StmList"));

        node->head->Accept(this);
        printEdge(node, (node->head).get(), "StmList head");

        node->tail->Accept(this);
        printEdge(node, (node->tail).get(), "StmList tail");
    }

    void IRPrettyPrinter::Visit(const ExpList *node) {
        printVertex(node, std::string("ExpList"));

        node->head->Accept(this);
        printEdge(node, (node->head).get(), "ExpList head");

        node->tail->Accept(this);
        printEdge(node, (node->tail).get(), "ExpList tail");
    }

    void IRPrettyPrinter::Visit(const GlobalIRTParent *node) {
        outPut <<  "digraph g {\n" << "\n";

        for (auto root : node->roots) {
            root->Accept(this);
        }

        outPut << "}\n";
    }
}