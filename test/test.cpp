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
#include <unordered_map>

#include "../src/allocation.hpp"

using namespace std;

class Registrar {
    unordered_map<string, AllocatedComponents> allocations;

  public:
    template <class C, class... Args>
    void component(string name, Args... args) {
        component_array<C>(name, 1, std::forward<Args>(args)...);
    }

    template <class C, class... Args>
    void component_array(string name, int size, Args... args) {
        auto allocation = Allocator<C>::allocate(size, std::forward<Args>(args)...);
        allocations.insert(make_pair(name, move(allocation)));
    }

    void print() {
        for (auto&& entry : allocations) {
            for (int i = 0; i < entry.second.size(); ++i) {
                auto&& component = entry.second.at<Component>(i);
                cout << "Allocation " << entry.first << "." << i << ": " << component.tc_debug() << endl;
            }
        }
    }
};

struct MyCompo : Component {
    int data;
    MyCompo(int data = 0) : data(data) {}
    string tc_debug() const final { return "MyCompo[" + to_string(data) + "]"; }
};

int main() {
    Registrar r;
    r.component<MyCompo>("c0", 3);
    r.component_array<MyCompo>("c1", 5, 9);
    r.print();
}
