---
name: create-heat-pump-variant
description: Use when creating a new ESPHome heat pump variant configuration. Generates a new YAML variant file based on an existing variant template, updating the short name and global identifiers. Requires base variant selection and variant name input.
---

# Create Heat Pump Variant

This skill guides the creation of a new heat pump variant configuration file. Each variant is a YAML file that extends a base configuration with device-specific properties and settings.

## Workflow

### Input & Decisions

**Ask user for:**
1. **Short variant name** — Device model code (e.g., `WPL13`, `WPL17`, `THZ504`, `TTF07`)
   - Pattern: Alphanumeric, uppercase, underscores allowed
   - Example: `WPL23`, `THZ5_5_ECO`

2. **Base variant** — Which existing variant to use as template
   - List available variants from `yaml/` directory (e.g., `wpl13.yaml`, `thz504.yaml`)
   - Cannot use `wpl_base.yaml` directly (it's inherited by all WPL variants)
   - Suggest similar device types

### Creation Steps

1. **Open base variant** — Read the selected base variant file (e.g., `yaml/wpl17.yaml`)

2. **Create new file** — Generate `yaml/{shortname_lowercase}.yaml` with:
   - Updated `platformio_options.build_flags` — Change flag to `-D{SHORTNAME_UPPERCASE}`
   - Updated `globals.heat_pump_variant` — Change initial value to `"{SHORTNAME_UPPERCASE}"`
   - Keep all `packages` section as-is (copy from base; cleanup happens later)

3. **Verify structure** — Confirm:
   - YAML syntax is valid
   - Build flag matches variant name
   - Global variable matches variant name
   - All package includes are intact

### Completion & Next Steps

**Output:**
- New YAML file at `yaml/{shortname}.yaml`
- File is ready for compilation

**User must then:**
- Compile/test the new variant manually
- Clean up unused properties in subsequent pass (not part of this skill)

## Example

**Input:**
- Short name: `WPL25`
- Base variant: `wpl17.yaml`

**Output:**
```yaml
esphome:
  platformio_options:
    build_flags:
      - "-DWPL_25"

globals:
  - id: heat_pump_variant
    type: std::string
    initial_value: '"WPL25"'

packages:
  wpl_base:                     !include { file: wpl_base.yaml }
  # ... all packages copied from wpl17.yaml ...
```

## Decision Points

- **Base variant selection**: Choose a variant with similar feature set and device class
- **Name validation**: Ensure name doesn't conflict with existing variants in `yaml/`
- **Property inheritance**: All properties are copied from base; filtering/cleanup is a separate workflow

## Quality Checks

After creation:
1. ✓ File exists at correct path: `yaml/{shortname}.yaml`
2. ✓ Build flag and global variable match the variant name
3. ✓ All YAML syntax is valid (indentation, quotes, includes)
4. ✓ No duplicate variant names in `yaml/` directory
5. ✓ `wpl_base` include is present (for WPL variants)

## Troubleshooting

| Issue | Resolution |
|-------|-----------|
| File already exists | Choose a different short name, check for existing variants |
| Invalid YAML syntax | Verify indentation (spaces, not tabs), quote values with special chars |
| Build flag mismatch | Ensure `-D{NAME}` matches global variable value |
| Missing base includes | Copy all `packages` section from base variant file |
