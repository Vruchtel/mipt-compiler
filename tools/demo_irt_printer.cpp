#include <ast/TreeBuilder.h>
#include <ast/tree/visitors/PrettyPrinterVisitor.h>

#include <irt/translator/IRPrettyPrinter.h>
#include <irt/translator/Canoniser.h>


#include <fstream>
#include <iostream>

int main(int /*argc*/, char** /*argv[]*/) {

    std::ofstream outIrt("./graph_irt.gv");

    NIRTree::IRPrettyPrinter irPrinter(outIrt);

//    auto globalRoot = NIRTree::GlobalIRTParent();

    auto node1 = NIRTree::Const(12, {});
    auto binop = NIRTree::Binop(NIRTree::Binop::AND, new NIRTree::Const(43, {}), new NIRTree::Const(44, {}), {});
    auto eseq = NIRTree::ESeq(new NIRTree::Exp(new NIRTree::Const(42, {}), {}),
        new NIRTree::Const(43, {}), {});
    auto call = NIRTree::Call(new NIRTree::Const(43, {}),
        new NIRTree::ExpList(new NIRTree::Const(43, {}), new NIRTree::Const(43, {}), {}), {});
    auto mem = NIRTree::Mem(new NIRTree::Const(12, {}), {});
    auto unop = NIRTree::Unop(NIRTree::Unop::NOT, new NIRTree::Const(12, {}), {});
    auto temp1 = NIRTree::Temp("temp1", {});
    auto temp2 = NIRTree::Temp(123, {});
    auto name = NIRTree::Name("namename", {});

    auto node2 = NIRTree::Exp(new NIRTree::Const(42, {}), {});
    NIRTree::Label *helperLabel = NIRTree::LabelHolder::GetLabel(name.label->String());
    auto jump = NIRTree::Jump(helperLabel, {});
    auto move = NIRTree::Move(new NIRTree::Const(42, {}), new NIRTree::Const(42, {}), {});
    auto cjump = NIRTree::CJump(NIRTree::CJump::EQ, 
        new NIRTree::Const(42, {}),
        new NIRTree::Const(42, {}),
        helperLabel, {});
    auto labelStm = NIRTree::LabelStm(helperLabel, {});
    auto seq = NIRTree::Seq(new NIRTree::Jump(helperLabel, {}),
        new NIRTree::Jump(helperLabel, {}), {});

    std::unique_ptr<NIRTree::StmWrapper> stmWrapper(
        new NIRTree::StmWrapper(
            //new NIRTree::LabelStm(helperLabel, {})
            new NIRTree::Exp(new NIRTree::Binop(NIRTree::Binop::AND, new NIRTree::Const(43, {}), new NIRTree::Const(44, {}), {}), {})
        )
    );
    std::unique_ptr<NIRTree::ExprWrapper> expWrapper(
        new NIRTree::ExprWrapper (
            new NIRTree::Call (
                new NIRTree::Const(43, {}),
                new NIRTree::ExpList(new NIRTree::Const(43, {}), new NIRTree::Const(43, {}), {}),
                {}
            )
        )
    );

    std::unique_ptr<NIRTree::ExprWrapper> expWrapper2(
        new NIRTree::ExprWrapper(
            new NIRTree::Binop(NIRTree::Binop::AND, new NIRTree::Const(43, {}), new NIRTree::Const(44, {}), {})
        )
    );

    std::unique_ptr<NIRTree::StmWrapper> stmWrapperDeep(
        new NIRTree::StmWrapper(
            new NIRTree::Seq(new NIRTree::Move(new NIRTree::Binop(NIRTree::Binop::OR, new NIRTree::Const(43, {}),
                    new NIRTree::Const(44, {}), {}), new NIRTree::Const(42, {}), {}),
                new NIRTree::Jump(helperLabel, {}), {}
            )
        )
    );

    // book (1)
    std::unique_ptr<NIRTree::ExprWrapper> expWrapper3(
        new NIRTree::ExprWrapper(
            new NIRTree::ESeq(
                new NIRTree::LabelStm(NIRTree::LabelHolder::GetLabel("s1"), {}),
                new NIRTree::ESeq(
                    new NIRTree::LabelStm(NIRTree::LabelHolder::GetLabel("s2"), {}),
                    new NIRTree::Name("e", {}),
                    {}
                ),
                {}
            )
        )
    );

    // book (2)
    std::unique_ptr<NIRTree::ExprWrapper> expWrapper4(
        new NIRTree::ExprWrapper(
            new NIRTree::Binop(
                NIRTree::Binop::AND,
                new NIRTree::ESeq(
                    new NIRTree::LabelStm(NIRTree::LabelHolder::GetLabel("s"), {}),
                    new NIRTree::Name("e1", {}),
                    {}
                ),
                new NIRTree::Name("e2", {}),
                {}
            )
        )
    );

    std::unique_ptr<NIRTree::ExprWrapper> expWrapper5(
        new NIRTree::ExprWrapper(
            new NIRTree::Mem(
                new NIRTree::ESeq(
                    new NIRTree::LabelStm(NIRTree::LabelHolder::GetLabel("s"), {}),
                    new NIRTree::Name("e1", {}),
                    {}
                ),
                {}
            )
        )
    );

    std::unique_ptr<NIRTree::StmWrapper> stmWrapper6(
        new NIRTree::StmWrapper(
            new NIRTree::CJump(
                NIRTree::CJump::EQ, 
                new NIRTree::ESeq(
                    new NIRTree::LabelStm(NIRTree::LabelHolder::GetLabel("s"), {}),
                    new NIRTree::Name("e1", {}),
                    {}
                ),
                new NIRTree::Name("e2", {}),
                NIRTree::LabelHolder::GetLabel("l1"),
                {}
            )
        )
    );

    // book (4)
    std::unique_ptr<NIRTree::ExprWrapper> expWrapper7(
        new NIRTree::ExprWrapper(
            new NIRTree::Binop(
                NIRTree::Binop::AND,
                new NIRTree::Name("e1", {}),
                new NIRTree::ESeq(
                    new NIRTree::LabelStm(NIRTree::LabelHolder::GetLabel("s"), {}),
                    new NIRTree::Name("e2", {}),
                    {}
                    ),
                {}
            )
        )
    );

    std::unique_ptr<NIRTree::StmWrapper> stmWrapper8(
        new NIRTree::StmWrapper(
            new NIRTree::CJump(
                NIRTree::CJump::EQ,
                new NIRTree::Name("e1", {}),
                new NIRTree::ESeq(
                    new NIRTree::LabelStm(NIRTree::LabelHolder::GetLabel("s"), {}),
                    new NIRTree::Name("e2", {}),
                    {}
                ),
                NIRTree::LabelHolder::GetLabel("l1"),
                {}
            )
        )
    );

    // book (3)
    std::unique_ptr<NIRTree::ExprWrapper> expWrapper9(
        new NIRTree::ExprWrapper(
            new NIRTree::Binop(
                NIRTree::Binop::AND,
                new NIRTree::Unop(NIRTree::Unop::NOT, new NIRTree::Name("e1", {}), {}),
                new NIRTree::ESeq(
                    new NIRTree::LabelStm(NIRTree::LabelHolder::GetLabel("s"), {}),
                    new NIRTree::Name("e2", {}),
                    {}
                    ),
                {}
            )
        )
    );

    std::unique_ptr<NIRTree::StmWrapper> stmWrapper10(
        new NIRTree::StmWrapper(
            new NIRTree::CJump(
                NIRTree::CJump::EQ,
                new NIRTree::Unop(NIRTree::Unop::NOT, new NIRTree::Name("e1", {}), {}),
                new NIRTree::ESeq(
                    new NIRTree::LabelStm(NIRTree::LabelHolder::GetLabel("s"), {}),
                    new NIRTree::Name("e2", {}),
                    {}
                ),
                NIRTree::LabelHolder::GetLabel("l1"),
                {}
            )
        )
    );

    // call
    std::unique_ptr<NIRTree::ExprWrapper> expWrapper11(
        new NIRTree::ExprWrapper(
            new NIRTree::Call(
                //new NIRTree::Const(41, {}),
                new NIRTree::ESeq(
                    new NIRTree::LabelStm(NIRTree::LabelHolder::GetLabel("s"), {}),
                    new NIRTree::Name("e2", {}),
                    {}
                ),
                new NIRTree::ExpList(
                    new NIRTree::Const(42, {}),
                    new NIRTree::Const(43, {}),
                    {}),
                {}
            )
        )
    );

    // binop with call
    std::unique_ptr<NIRTree::ExprWrapper> expWrapper12(
        new NIRTree::ExprWrapper(
            new NIRTree::Binop(
                NIRTree::Binop::AND,
                new NIRTree::Unop(NIRTree::Unop::NOT, new NIRTree::Name("e1", {}), {}),
                new NIRTree::Call(
                    new NIRTree::Const(41, {}),
                    new NIRTree::ExpList(
                        new NIRTree::Const(42, {}),
                        new NIRTree::Const(43, {}),
                        {}
                    ),
                    {}
                ),
                {}    
            )
        )
    );

    std::unique_ptr<NIRTree::ExprWrapper> expWrapper13(
        new NIRTree::ExprWrapper(
            new NIRTree::Call(
                //new NIRTree::Const(41, {}),
                new NIRTree::ESeq(
                    new NIRTree::LabelStm(NIRTree::LabelHolder::GetLabel("s"), {}),
                    new NIRTree::Name("e2", {}),
                    {}
                ),
                new NIRTree::ExpList(
                    new NIRTree::Unop(NIRTree::Unop::NOT, new NIRTree::Name("e1", {}), {}),
                    new NIRTree::Const(43, {}),
                    {}),
                {}
            )
        )
    );

    /*globalRoot.roots = {&node1, &node2, &binop, &eseq, &call, &mem, &unop, &temp1, &temp2,
        &name, &jump, &move, &cjump, &labelStm, &seq,
        &stmWrapper, &expWrapper, stmWrapperDeep.get()};*/

    //auto &wrapperO = expWrapper12;
    //auto &wrapperO = stmWrapperDeep;
    //auto &wrapperO = expWrapper11;
    //auto &wrapperO = stmWrapper8;
    //auto &wrapperO = expWrapper13;
    //auto &wrapperO = stmWrapper;
    //auto &wrapperO = expWrapper;
    //auto &wrapperO = expWrapper2;
    //auto &wrapperO = expWrapper3;
    //auto &wrapperO = expWrapper4;
    //auto &wrapperO = expWrapper5;
    //auto &wrapperO = stmWrapper6;
    //auto &wrapperO = expWrapper7;
    //auto &wrapperO = expWrapper9;
    auto &wrapperO = stmWrapper10;

//    globalRoot.forest = NIRTree::IRForest{std::make_pair(nullptr, std::move(wrapperO))};

//    irPrinter.Visit(&globalRoot);

    outIrt.close();

    {
        /*std::unique_ptr<NIRTree::StmWrapper> wrapper = 
            NIRTree::Canoniser::RemoveEseqsFromSubtree(std::move(wrapperO));    
        {
            std::ofstream outIrt("./graph_irt_canonise.gv");
            NIRTree::IRPrettyPrinter irPrinter(outIrt);
            auto globalRoot = NIRTree::GlobalIRTParent();

            globalRoot.roots = {wrapper.get()};
            irPrinter.Visit(&globalRoot);
            outIrt.close();
        }
        auto vec = NIRTree::Canoniser::Linearise(std::move(wrapper));
        {
            std::ofstream outIrt("./graph_irt_linearise.gv");
            NIRTree::IRPrettyPrinter irPrinter(outIrt);
            auto globalRoot = NIRTree::GlobalIRTParent();

            for (auto& elem : vec) {
                globalRoot.roots.push_back(elem.get());
            }
            irPrinter.Visit(&globalRoot);
            outIrt.close();
        }*/

//        auto vec = NIRTree::Canoniser::Canonise(std::move(globalRoot.forest.at(nullptr)));
//        std::ofstream outIrt("./graph_irt_canon.gv");
//        NIRTree::IRPrettyPrinter irPrinter(outIrt);
//        auto globalRoot = NIRTree::GlobalIRTParent();
//
//        for (auto& elem : vec) {
//            globalRoot.forest.insert(std::make_pair(std::move(elem)));
//        }
//        irPrinter.Visit(&globalRoot);
//        outIrt.close();
    }
    
    return 0;
}