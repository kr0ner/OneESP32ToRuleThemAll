#pragma once

#include "esphome/core/application.h"
#include "esphome/core/component.h"

namespace esphome {
namespace custom_component {

class CustomComponentConstructor {
   public:
    CustomComponentConstructor(const std::function<std::vector<Component*>()>& init) {
        this->components_ = init();

        for (auto* comp : this->components_) {
            App.register_component(comp);
        }
    }

    Component* get_component(int i) const { return this->components_[i]; }

   protected:
    std::vector<Component*> components_;
};

}  // namespace custom_component
}  // namespace esphome
