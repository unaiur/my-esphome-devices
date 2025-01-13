#include "delta_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace delta_sensor {

static const char *const TAG = "delta_sensor";

void DeltaSensor::setup() {
  this->parent_->add_on_state_callback([this](std::string state) { this->process_new_state_(state); });
}

void DeltaSensor::dump_config() { LOG_SENSOR("", "Delta Sensor", this); }

void DeltaSensor::process_new_state_(std::string const &state) {
  char *endptr;
  uint64_t value = strtoull(parent_->get_state().c_str(), &endptr, 10);
  if (*endptr != 0) {
    ESP_LOGV(TAG, "Parent text sensor does not contain a number");
    return;
  }

  uint32_t prev_update = this->last_update_;
  uint32_t now = millis();
  if (this->initialised_) {
    float delta = (value - this->last_value_) * 1000.0 / (now - this->last_update_);
    this->publish_state(delta);
  } else {
    this->initialised_ = true;
  }
  this->last_value_ = value;
  this->last_update_ = now;
}

}  // namespace delta_sensor
}  // namespace esphome
