import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, sensor, binary_sensor
from esphome.const import CONF_ID, CONF_TYPE

DEPENDENCIES = ["wp_core"]

wp_climate_ns = cg.esphome_ns.namespace("wp_climate")
Heating = wp_climate_ns.class_("Heating", climate.Climate, cg.Component)
HotWater = wp_climate_ns.class_("HotWater", climate.Climate, cg.Component)

CONF_CURRENT_TEMP = "current_temp"
CONF_TARGET_TEMP = "target_temp"
CONF_HEATING = "heating"
CONF_COOLING = "cooling"
CONF_FAN = "fan"
CONF_TARGET_PROPERTY = "target_property"
CONF_COOL_PROPERTY = "cool_property"

HEATING_SCHEMA = climate.climate_schema(Heating).extend({
    cv.Required(CONF_CURRENT_TEMP): cv.use_id(sensor.Sensor),
    cv.Required(CONF_TARGET_TEMP): cv.use_id(sensor.Sensor), 
    cv.Required(CONF_HEATING): cv.use_id(binary_sensor.BinarySensor),
    cv.Optional(CONF_COOLING): cv.use_id(binary_sensor.BinarySensor),
    cv.Optional(CONF_FAN): cv.use_id(binary_sensor.BinarySensor),
    cv.Required(CONF_TARGET_PROPERTY): cv.string,
    cv.Optional(CONF_COOL_PROPERTY): cv.string,
}).extend(cv.COMPONENT_SCHEMA)

HOT_WATER_SCHEMA = climate.climate_schema(HotWater).extend({
    cv.Required(CONF_CURRENT_TEMP): cv.use_id(sensor.Sensor),
    cv.Required(CONF_TARGET_TEMP): cv.use_id(sensor.Sensor),
    cv.Required(CONF_HEATING): cv.use_id(binary_sensor.BinarySensor),
    cv.Required(CONF_TARGET_PROPERTY): cv.string,
}).extend(cv.COMPONENT_SCHEMA)

CONFIG_SCHEMA = cv.typed_schema({
    "heating": HEATING_SCHEMA,
    "hot_water": HOT_WATER_SCHEMA,
})

async def to_code(config):
    current_temp = await cg.get_variable(config[CONF_CURRENT_TEMP])
    target_temp = await cg.get_variable(config[CONF_TARGET_TEMP])
    heating_sens = await cg.get_variable(config[CONF_HEATING])
    target_prop = cg.RawExpression(f"Property::{config[CONF_TARGET_PROPERTY]}")

    if config[CONF_TYPE] == "heating":
        has_cool = CONF_COOLING in config
        has_fan = CONF_FAN in config
        
        if has_cool and has_fan and CONF_COOL_PROPERTY in config:
            cool_sens = await cg.get_variable(config[CONF_COOLING])
            fan_sens = await cg.get_variable(config[CONF_FAN])
            cool_prop = cg.RawExpression(f"Property::{config[CONF_COOL_PROPERTY]}")
            var = cg.new_Pvariable(config[CONF_ID], current_temp, target_temp, heating_sens, cool_sens, fan_sens, target_prop, cool_prop)
        elif has_fan and not has_cool:
            fan_sens = await cg.get_variable(config[CONF_FAN])
            var = cg.new_Pvariable(config[CONF_ID], current_temp, target_temp, heating_sens, fan_sens, target_prop)
        else:
            var = cg.new_Pvariable(config[CONF_ID], current_temp, target_temp, heating_sens, target_prop)
    else:
        var = cg.new_Pvariable(config[CONF_ID], current_temp, target_temp, heating_sens, target_prop)

    await cg.register_component(var, config)
    await climate.register_climate(var, config)
