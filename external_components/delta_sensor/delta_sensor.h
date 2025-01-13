#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

namespace esphome {
namespace delta_sensor {

class DeltaSensor : public sensor::Sensor, public Component {
 public:
  void set_parent(text_sensor::TextSensor *parent) { parent_ = parent; }
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void process_new_state_(std::string const &state);

  text_sensor::TextSensor *parent_;
  bool initialised_{false};
  uint32_t last_update_{0};
  uint64_t last_value_{0};
};

}  // namespace delta_sensor
}  // namespace esphome
