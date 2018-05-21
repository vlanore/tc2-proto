/* Copyright or © or Copr. Centre National de la Recherche Scientifique (CNRS) (2017/05/03)
Contributors:
- Vincent Lanore <vincent.lanore@gmail.com>

This software is a computer program whose purpose is to provide the necessary classes to write ligntweight component-based
c++ applications.

This software is governed by the CeCILL-B license under French law and abiding by the rules of distribution of free software.
You can use, modify and/ or redistribute the software under the terms of the CeCILL-B license as circulated by CEA, CNRS and
INRIA at the following URL "http://www.cecill.info".

As a counterpart to the access to the source code and rights to copy, modify and redistribute granted by the license, users
are provided only with a limited warranty and the software's author, the holder of the economic rights, and the successive
licensors have only limited liability.

In this respect, the user's attention is drawn to the risks associated with loading, using, modifying and/or developing or
reproducing the software by the user in light of its specific status of free software, that may mean that it is complicated
to manipulate, and that also therefore means that it is reserved for developers and experienced professionals having in-depth
computer knowledge. Users are therefore encouraged to load and test the software's suitability as regards their requirements
in conditions enabling the security of their systems and/or data to be ensured and, more generally, to use and operate it in
the same conditions as regards security.

The fact that you are presently reading this means that you have had knowledge of the CeCILL-B license and that you accept
its terms.*/

#include <iostream>  // for the test at the end
#include <memory>    // for unique_ptr in AllocatedComponents
#include <vector>    // for vector in AllocatedComponents
#include "container.hpp"

using namespace std;

// class from which all components should inherit
struct Component {
    Component() = default;
    Component(const Component&) = delete;
    virtual ~Component() = default;
};

// class representing an array of already allocated components
class AllocatedComponents {
    struct AllocatedMemory {
        virtual ~AllocatedMemory() = default;
    };

    template <class C>
    struct ConcreteAllocatedMemory : public AllocatedMemory {
        ConcreteAllocatedMemory(int nb) : data(nb) {}
        Container<C> data;
    };

    unique_ptr<AllocatedMemory> lifetime;
    vector<Component*> components;

    // builder pattern with allocate function
    AllocatedComponents(int nb) { components.reserve(nb); }
    template <class C, class... Args>
    friend AllocatedComponents allocate(int, Args...);

  public:
    template <class C>
    C& at(int i) {
        return dynamic_cast<C&>(*components.at(i));
    }

    template <class C>
    Container<C>& vec() {
        return dynamic_cast<ConcreteAllocatedMemory<C>&>(*lifetime.get()).data;
    }
};

template <class C, class... Args>
AllocatedComponents allocate(int nb, Args... args) {
    using CAM = AllocatedComponents::ConcreteAllocatedMemory<C>;
    using AM = AllocatedComponents::AllocatedMemory;
    AllocatedComponents alloc(nb);
    unique_ptr<CAM> data(new CAM(nb));
    for (int i = 0; i < nb; ++i) {
        data->data.at(i) = C(args...);
        alloc.components.push_back(dynamic_cast<Component*>(&data->data.at(i)));
    }
    alloc.lifetime = {unique_ptr<AM>(dynamic_cast<AM*>(data.release()))};
    return alloc;
}

struct MyCompo : public Component {
    int data;
    MyCompo(int data = 0) : data(data) {}
};

int main() {
    auto alloc = allocate<MyCompo>(5, 17);
    for (auto&& e : alloc.vec<MyCompo>()) {
        e.data = 13;
    }
    alloc.at<MyCompo>(3).data = 19;
    for (auto&& e : alloc.vec<MyCompo>()) {
        cout << e.data << endl;
    }
}
