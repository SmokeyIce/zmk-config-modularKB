#include <stdbool.h>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/init.h>

#include <zmk/event_manager.h>
#include <zmk/events/hid_indicators_changed.h>

#if  DT_NODE_EXISTS(DT_NODELABEL(num_lock_led))

#define KP_HID_INDICATOR_NUM_LOCK 0x01

static const struct gpio_dt_spec num_lock_led =
    GPIO_DT_SPEC_GET(DT_NODELABEL(num_lock_led), gpios);

static int kp_numlock_led_init(void) {
    if (!gpio_is_ready_dt(&num_lock_led)) {
        return -ENODEV;
    }

    // LED STARTS IN OFF STATE
    return gpio_pin_configure_dt(&num_lock_led, GPIO_OUTPUT_INACTIVE);
}

//listen event
static int kp_numlock_led_listener(const zmk_event_t *eh) {
    const struct zmk_hid_indicators_changed *ev = as_zmk_hid_indicators_changed(eh);
    if (!ev) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    const bool on = (ev -> indicators & KP_HID_INDICATOR_NUM_LOCK) != 0;
    gpio_pin_set_dt(&num_lock_led, on);
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(kp_numlock_led, kp_numlock_led_listener);
ZMK_SUBSCRIPTION(kp_numlock_led, zmk_hid_indicators_changed);

SYS_INIT(kp_numlock_led_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);


#endif