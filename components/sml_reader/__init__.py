import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, esp32, sensor
from esphome.const import CONF_ID, CONF_UART_ID

DEPENDENCIES = ['uart', 'esp32']
AUTO_LOAD = ['uart']

SMLReader = cg.global_ns.class_('SMLReader', cg.Component, uart.UARTDevice)

CONF_MQTT_SERVER = 'mqtt_server'
CONF_MQTT_USER = 'mqtt_user'
CONF_MQTT_PASSWORD = 'mqtt_password'

CONF_SENSORS = [
    'instantaneous_power', 'instantaneous_power_l1', 'instantaneous_power_l2', 'instantaneous_power_l3',
    'instantaneous_voltage_l1', 'instantaneous_voltage_l2', 'instantaneous_voltage_l3',
    'instantaneous_current_l1', 'instantaneous_current_l2', 'instantaneous_current_l3',
    'frequency', 'total_incoming', 'total_outgoing'
]

schema = cv.Schema({
    cv.GenerateID(): cv.declare_id(SMLReader),
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required(CONF_MQTT_SERVER): cv.string,
    cv.Required(CONF_MQTT_USER): cv.string,
    cv.Required(CONF_MQTT_PASSWORD): cv.string,
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

for s_conf in CONF_SENSORS:
    schema = schema.extend({cv.Optional(s_conf): cv.use_id(sensor.Sensor)})

CONFIG_SCHEMA = schema

async def to_code(config):
    esp32.add_idf_component(
        name="libsml",
        repo="https://github.com/kr0ner/libsml",
        ref="master"
    )

    uart_comp = await cg.get_variable(config[CONF_UART_ID])

    var = cg.new_Pvariable(
        config[CONF_ID],
        uart_comp,
        config[CONF_MQTT_SERVER],
        config[CONF_MQTT_USER],
        config[CONF_MQTT_PASSWORD]
    )

    for s_conf in CONF_SENSORS:
        if s_conf in config:
            sens = await cg.get_variable(config[s_conf])
            cg.add(getattr(var, f"set_{s_conf}")(sens))

    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
