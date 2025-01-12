#include "gpio_monitor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace gpio_monitor {

static const char *const TAG = "gpio_monitor.binary_sensor";

void GPIOMonitor::setup() {
  this->pin_->setup();
  this->publish_initial_state(true);
}

void GPIOMonitor::dump_config() {
  LOG_BINARY_SENSOR("", "GPIO Monitor", this);
  LOG_PIN("  Pin: ", this->pin_);
}

static bool pin_set(GPIOPin *pin, bool value) {
  pin->pin_mode(gpio::FLAG_OUTPUT);
  pin->digital_write(value);
  pin->pin_mode(gpio::FLAG_INPUT);
  return pin->digital_read();
}

void GPIOMonitor::loop() {
  this->publish_state(pin_set(this->pin_, true) && !pin_set(this->pin_, false));
}


float GPIOMonitor::get_setup_priority() const {
  return setup_priority::HARDWARE;
}

}  // namespace gpio_monitor
}  // namespace esphome
