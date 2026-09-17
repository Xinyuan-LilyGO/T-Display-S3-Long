# ESPHome LVGL Hello World

A minimal, working [ESPHome](https://esphome.io) config for this board: WiFi,
API, OTA, backlight, touch, and a landscape LVGL screen with one label. No
external/custom component required - this is ESPHome's own stock `mipi_spi`
driver with `model: AXS15231`.

If you're here because of a garbage/noise line at one edge of the screen, or
a hard crash a few seconds after boot with the screen staying permanently
blank - that's this board's AXS15231B controller, and this example exists
because of it. See `esphome-lvgl-hello-world.yaml` for the full config; the
five settings that actually matter are marked `FIX 1`-`FIX 5` with inline
explanations. Short version:

1. Declare the panel at its true native geometry (180x640, portrait) - not
   the rotated 640x180 you actually want to see. This chip has no working
   hardware axis-swap bit (MADCTL_MV).
2. Put `rotation: 90` on the `lvgl:` block, not `display:` - ESPHome
   rejects `rotation:` on a mipi_spi display block when LVGL is present, and
   this chip can only rotate in software anyway.
3. `full_refresh: true` - the AXS15231B controller mishandles some
   partial/odd-coordinate address windows (the corruption reported in
   [#44](https://github.com/Xinyuan-LilyGO/T-Display-S3-Long/issues/44), and
   the same reason `fundix`'s comment there sets `full_refresh = 1` on raw
   LVGL). Forcing every flush to redraw the whole screen through one fixed,
   aligned address window avoids that class of bug entirely.
4. `buffer_size: 100%` - `full_refresh` needs the whole frame available to
   redraw as one unit. Pairing it with a fractional buffer size hard-crashed
   our ESPHome version: a watchdog abort inside `lv_display_set_buffers`,
   before `setup()` even finished, leaving the screen permanently blank.
5. `draw_rounding: 4` instead of the default `8` - the panel's native width
   (180px) isn't divisible by 8, so the default rounding wrote a few rows
   past the real buffer edge, showing up as a thin garbage line at one edge.

## Using this example

1. Copy `secrets.yaml.example` to `secrets.yaml` in the same folder and fill
   in your WiFi credentials.
2. `esphome run esphome-lvgl-hello-world.yaml`

Tested on ESPHome 2026.8.2.
