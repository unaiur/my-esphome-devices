import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, text_sensor
from esphome.const import (
    CONF_ICON,
    CONF_ID,
    CONF_UNIT_OF_MEASUREMENT,
    CONF_ACCURACY_DECIMALS,
)
from esphome.core.entity_helpers import inherit_property_from

CONF_PARENT_ID = "parent_id"
delta_sensor_ns = cg.esphome_ns.namespace("delta_sensor")
DeltaSensor = delta_sensor_ns.class_(
    "DeltaSensor", sensor.Sensor, cg.Component
)


def inherit_unit_of_measurement(uom, config):
    if uom.endswith("h"):
        return uom[:-1]
    return uom


CONFIG_SCHEMA = (
    sensor.sensor_schema(
        DeltaSensor,
    )
    .extend(
        {
            cv.Required(CONF_PARENT_ID): cv.use_id(text_sensor.TextSensor),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

FINAL_VALIDATE_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.use_id(DeltaSensor),
            cv.Optional(CONF_ICON): cv.icon,
            cv.Optional(CONF_UNIT_OF_MEASUREMENT): sensor.validate_unit_of_measurement,
            cv.Optional(CONF_ACCURACY_DECIMALS): sensor.validate_accuracy_decimals,
            cv.Required(CONF_PARENT_ID): cv.use_id(text_sensor.TextSensor),
        },
        extra=cv.ALLOW_EXTRA,
    ),
    inherit_property_from(CONF_ICON, CONF_PARENT_ID),
    inherit_property_from(
        CONF_UNIT_OF_MEASUREMENT, CONF_PARENT_ID, transform=inherit_unit_of_measurement
    ),
    inherit_property_from(CONF_ACCURACY_DECIMALS, CONF_PARENT_ID),
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    sens = await cg.get_variable(config[CONF_PARENT_ID])
    cg.add(var.set_parent(sens))
