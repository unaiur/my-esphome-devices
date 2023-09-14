#include "esphome.h"
#include <cmath>

static const char *DIFF_SENSOR = "ReaderSensor";

class Reader {
  public:
    virtual float read_next() = 0;
};

class NumericTextDifferentialReader: public Reader {
  public:
    NumericTextDifferentialReader(esphome::text_sensor::TextSensor *parent): parent_(parent), has_state_() {
    }

    float read_next() override {
      if (!parent_->has_state()) {
        return NAN;
      }

      char *endptr;
      uint64_t value = strtoull(parent_->get_state().c_str(), &endptr, 10);
      if (*endptr != 0) {
        ESP_LOGE(DIFF_SENSOR, "Parent text sensor does not contain a number");
        return NAN;
      }

      float result = NAN;
      if (has_state_) {
        result = value - prev_value_;
      }
      prev_value_ = value;
      has_state_ = true;
      return result;
    }

  private:
    esphome::text_sensor::TextSensor *parent_;
    bool has_state_;
    uint64_t prev_value_;
};

class SubstractReader: public Reader {
  public:
    SubstractReader(Reader *plus, Reader *minus): plus_(plus), minus_(minus) {}

    float read_next() override {
      return plus_->read_next() - minus_->read_next();
    }

  private:
    Reader *plus_;
    Reader *minus_;
};

class ReaderSensor: public esphome::PollingComponent, public esphome::sensor::Sensor {
 public:
  ReaderSensor(Reader *reader, uint32_t update_interval = 1000)
    : PollingComponent(update_interval), reader_(reader) {}

  void update() override {
    auto val = reader_->read_next();
    if (!isnan(val)) {
      publish_state(val);
    }
  }

 private:
  Reader *reader_;
};

