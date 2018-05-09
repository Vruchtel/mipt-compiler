#pragma once

#include <irt/ISubtreeWrapper.h>

namespace NIRTree {
    class StmWrapper: public ISubtreeWrapper {
    public:
        explicit StmWrapper(IStm *statement) : statement(statement) {
            location = statement->location;
        }

        IExp * ToExp() override {
            assert(false);
        }

        IStm * ToStm() override {
            return statement.release();
        }

        IStm * ToConditional(CJump::EJumpType, const Label *, const Label *) override {
            assert(false);
        };

        void Accept(IIRVisitor *visitor) const override;

    private:
        std::unique_ptr<IStm> statement;
    };
}