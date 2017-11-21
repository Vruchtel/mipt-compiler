#include "SymbolTable.h"

namespace NSymbolTable {
    bool SymbolTable::HasClass(const NUtil::Symbol* id) const {
        return classes.find(id) != classes.end();
    }

    const ClassInfo &SymbolTable::GetClassInfo(const Symbol* id) const {
        return classes.at(id);
    }

    void SymbolTable::InsertClassInfo(const ClassInfo& classInfo) {
        classes.insert({classInfo.GetId(), classInfo});
    }

    const std::unordered_map<const Symbol *, ClassInfo>& SymbolTable::GetClasses() const {
        return classes;
    }

    void SymbolTable::SetMainClass(const Symbol* id) {
        mainClass = id;
    }
}
