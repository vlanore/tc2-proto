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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <iostream>
#include "doctest.h"

#include "../src/allocation.hpp"

using namespace std;

TEST_CASE("Basic allocation test.") {
    struct MyCompo : public Component {
        int data;
        MyCompo(int data = 0) : data(data) {}
    };

    Allocator<MyCompo> allocator;
    auto alloc = allocator.allocate(5, 17);
    for (auto&& e : alloc.vec<MyCompo>()) {
        e.data = 13;
    }
    alloc.at<MyCompo>(3).data = 19;
    auto&& vec = alloc.vec<MyCompo>();
    vec[2] = 12;
    vector<int> expected_result{13, 13, 12, 19, 13};
    for (int i = 0; i < vec.size(); i++) {
        CHECK(vec[i].data == expected_result[i]);
    }
}
