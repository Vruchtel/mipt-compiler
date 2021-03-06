#include "X86MiniJavaFrame.h"

#include "irt/activation_records/InRegAccess.h"
#include "irt/activation_records/InFrameAccess.h"

namespace NIRTree {
    const int X86MiniJavaFrame::MaxInReg = 4;

    void X86MiniJavaFrame::AddLocal(const NSymbolTable::VariableInfo &variable) {
        idToInfo.insert({variable.GetId(), variable});

        // TODO: 4 is typeSize
        IAccess* var = new InFrameAccess(IAccess::FORMAL, 4, localTopPointer );
        idToAccess.insert(std::make_pair(variable.GetId(), std::unique_ptr<IAccess>(var)));
        localTopPointer += 4;
    }

    void X86MiniJavaFrame::AddFormal(const NSymbolTable::VariableInfo &variable) {
        formalIds.push_back(variable.GetId());

        // TODO: 4 is typeSize
        idToInfo.insert({variable.GetId(), variable});
        IAccess* var = createFormal(IAccess::FORMAL, 4);
        idToAccess.insert(std::make_pair(variable.GetId(), std::unique_ptr<IAccess>(var)));
    }

    int X86MiniJavaFrame::GetFormalsCount() const {
        return static_cast<int>(formalIds.size());
    }

    std::shared_ptr<const IAccess> X86MiniJavaFrame::GetFormal(int index) const {
        return idToAccess.at(formalIds[index]);
    }

    std::shared_ptr<const IAccess> X86MiniJavaFrame::GetFormalOrLocal(const NUtil::Symbol *id) const {
        return idToAccess.at(id);
    }

    int X86MiniJavaFrame::TypeSize(const NSymbolTable::TypeInfo &/*type*/) const {
        // TODO: change to some valid implementation
        return 4;
    }

    X86MiniJavaFrame::X86MiniJavaFrame(const NSymbolTable::ClassInfo &classInfo,
                                       const NSymbolTable::MethodInfo &methodInfo,
                                       const NSymbolTable::SymbolTable &_symbolTable)
            : symbolTable(_symbolTable) {
        NSymbolTable::VariableInfo thisVariable(symbolTable.GetInterner()->GetIntern("this"),
                                                Location(),
                                                classInfo.GetType());
        AddFormal(thisVariable);
        for (const auto &name: methodInfo.GetArgsInfo()) {
            AddFormal(name);
        }

        for (const auto &id2info: methodInfo.GetVarsInfo()) {
            AddLocal(id2info.second);
        }
        AddAddressExit();
        AddAddressReturnValue(methodInfo.GetReturnType());
    }

    void X86MiniJavaFrame::AddAddressExit() {
        // TODO: typeSpec instead of 4
        IAccess *var = createFormal(IAccess::ADDRESS_EXIT, 4);
        const Symbol* id = symbolTable.GetInterner()->GetIntern("@EXIT_ADDRESS@");
        idToAccess.insert(std::make_pair(id,
                                         std::unique_ptr<IAccess>(var)));
        addressExitIndex = formalIds.size();
        formalIds.push_back(id);
    }

    IAccess *X86MiniJavaFrame::createFormal(IAccess::ERecordsType type, int size) {
        if (formalIds.size() < MaxInReg) {
            return new InRegAccess(type, size, formalIds.size());
        } else {
            IAccess *access = new InFrameAccess(type, size, formalTopPointer);
            formalTopPointer += size;
            return access;
        }
    }

    IFrame *X86MiniJavaFrameBuilder::GetFrame(const NSymbolTable::ClassInfo &classInfo,
                                              const NSymbolTable::MethodInfo &methodInfo,
                                              const NSymbolTable::SymbolTable &symbolTable) const {
        return new X86MiniJavaFrame(classInfo, methodInfo, symbolTable);
    }
}