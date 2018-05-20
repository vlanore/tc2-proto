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

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

struct Component {
    virtual ~Component() = default;
};

struct AllocatedMemory {
    virtual ~AllocatedMemory() = default;
};

struct Lifetime {  // deallocates components when destroyed, moveable but non-copyable
    unique_ptr<AllocatedMemory> data;
};

template <class C>
struct ConcreteAllocatedMemory : public AllocatedMemory {
    ConcreteAllocatedMemory(int nb) { data.reserve(nb); }
    vector<C> data;
};

struct AllocatedComponents {
    AllocatedComponents(int nb) { components.reserve(nb); }
    Lifetime lifetime;
    vector<Component*> components;
    template <class C>
    C& at(int i) {
        return dynamic_cast<C&>(*components.at(i));
    }
    template <class C>
    vector<C>& vec() {
        return dynamic_cast<ConcreteAllocatedMemory<C>&>(*lifetime.data.get()).data;
    }
};

template <class C, class... Args>
AllocatedComponents allocate(int nb, Args... args) {
    AllocatedComponents alloc(nb);
    unique_ptr<ConcreteAllocatedMemory<C>> data(new ConcreteAllocatedMemory<C>(nb));
    for (int i = 0; i < nb; ++i) {
        data->data.emplace_back(args...);
        alloc.components.push_back(dynamic_cast<Component*>(&data->data.at(i)));
    }
    alloc.lifetime = {unique_ptr<AllocatedMemory>(dynamic_cast<AllocatedMemory*>(data.release()))};
    return alloc;
}

struct MyCompo : public Component {
    int data;
    MyCompo(int data) : data(data) {}
};

int main() {
    auto alloc = allocate<MyCompo>(5, 17);
    alloc.at<MyCompo>(3).data = 19;
    for (auto&& e : alloc.vec<MyCompo>()) {
        cout << e.data << endl;
    }
}
