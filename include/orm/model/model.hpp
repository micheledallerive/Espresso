#pragma once
#include "orm/queryset/queryset.hpp"

namespace espresso::orm {

template<typename Child>
class BaseModel {
public:
    using ConcreteModel = Child;
    [[maybe_unused]] static QuerySet<Child> objects()
    {
        return {};
    }

    void save() {

    }
};

}// namespace espresso::orm