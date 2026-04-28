
#include <stdbool.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/init.h>
#include <dt-bindings/zmk/hid_indicators.h>
#include <zmk/event_manager.h>
#include <zmk/events/hid_indicators_changed.h>
#if DT_NODE_EXISTS(DT_NODELABEL(caps_lock_led)) && defined(CONFIG_ZMK_SPLIT_PERIPHERAL_HID_INDICATORS)

static const struct gpio_dt_spec caps_lock_led =
    GPIO_DT_SPEC_GET(DT_NODELABEL(caps_lock_led), gpios);

    static int caps_lock_led_init(void) {
    if (!gpio_is_ready_dt(&caps_lock_led)) {
        return -ENODEV;
    }
    return gpio_pin_configure_dt(&caps_lock_led, GPIO_OUTPUT_INACTIVE);
}
static int caps_lock_led_listener(const zmk_event_t *eh) {
    const struct zmk_hid_indicators_changed *ev = as_zmk_hid_indicators_changed(eh);
    if (!ev) {
        return ZMK_EV_EVENT_BUBBLE;
    }
    const bool on = (ev->indicators & HID_INDICATOR_CAPS_LOCK) != 0;
    gpio_pin_set_dt(&caps_lock_led, on);
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(caps_lock_led_lis, caps_lock_led_listener);
ZMK_SUBSCRIPTION(caps_lock_led_lis, zmk_hid_indicators_changed);
SYS_INIT(caps_lock_led_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
#endif