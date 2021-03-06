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

#include <memory>  // for unique_ptr in AllocatedComponents
#include <vector>  // for vector in AllocatedComponents
#include "container.hpp"
#include "interfaces.hpp"

// class representing an array of already allocated components
class AllocatedComponents {
    struct ComponentContainer {
        virtual ~ComponentContainer() = default;
    };

    template <class C>
    struct ConcreteComponentContainer : public ComponentContainer {
        ConcreteComponentContainer(int nb) : data(nb) {}
        Container<C> data;
    };

    std::unique_ptr<ComponentContainer> lifetime;
    std::vector<Component*> components;

    // builder pattern with allocate function
    AllocatedComponents(int nb) { components.reserve(nb); }
    template <class C>
    friend class Allocator;

  public:
    int size() const { return components.size(); }

    template <class C>
    C& at(int i) {
        return dynamic_cast<C&>(*components.at(i));
    }

    template <class C>
    const C& at(int i) const {
        return dynamic_cast<C&>(*components.at(i));
    }

    template <class C>
    Container<C>& vec() {
        return dynamic_cast<ConcreteComponentContainer<C>&>(*lifetime.get()).data;
    }

    template <class C>
    const Container<C>& vec() const {
        return dynamic_cast<ConcreteComponentContainer<C>&>(*lifetime.get()).data;
    }
};

template <class C>
class Allocator {
  public:
    template <class... Args>
    static AllocatedComponents allocate(int nb, Args... args) {
        using CCC = AllocatedComponents::ConcreteComponentContainer<C>;
        using ACC = AllocatedComponents::ComponentContainer;
        AllocatedComponents alloc(nb);
        std::unique_ptr<CCC> data(new CCC(nb));
        for (int i = 0; i < nb; ++i) {
            data->data.at(i) = C(args...);
            alloc.components.push_back(dynamic_cast<Component*>(&data->data.at(i)));
        }
        alloc.lifetime = {std::unique_ptr<ACC>(dynamic_cast<ACC*>(data.release()))};
        return alloc;
    }
};
