import esphome.codegen as cg
import esphome.config_validation as cv

def to_code(config):
    cg.add_global(cg.RawStatement('#include "esphome/components/wp_core/callback_handler.h"'))
    cg.add_global(cg.RawStatement('#include "esphome/components/wp_core/communication.h"'))
    cg.add_global(cg.RawStatement('#include "esphome/components/wp_core/mapper.h"'))
    cg.add_global(cg.RawStatement('#include "esphome/components/wp_core/property.h"'))
    cg.add_global(cg.RawStatement('#include "esphome/components/wp_core/simple_variant.h"'))
    cg.add_global(cg.RawStatement('#include "esphome/components/wp_core/type.h"'))
    cg.add_global(cg.RawStatement('#include "esphome/components/wp_core/util.h"'))

CONFIG_SCHEMA = cv.Schema({ cv.GenerateID(): cv.declare_id(cg.Component) })
