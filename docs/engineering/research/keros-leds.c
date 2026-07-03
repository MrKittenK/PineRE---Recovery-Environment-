// SPDX-License-Identifier: GPL-3.0-only

/***************************************************************************
 *   keros-leds.c  --  This file is part of apple1emu.                     *
 *                                                                         *
 *   Copyright (C) 2026 Imanol-Mikel Barba Sabariego                       *
 *                                                                         *
 *   keros-leds is free software: you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published     *
 *   by the Free Software Foundation, either version 3 of the License,     *
 *   or (at your option) any later version.                                *
 *                                                                         *
 *   keros-leds is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty           *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               *
 *   See the GNU General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.   *
 *                                                                         *
 ***************************************************************************/

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/leds.h>
#include <linux/mutex.h>
#include <linux/irqflags.h>
#include <linux/workqueue.h>
#include <linux/input.h>

#define KEROS_MODE_OFF   0
#define KEROS_MODE_MAX   9
#define KEROS_MODE_INIT  11

#define SYNC_US          6000  /* sync pulse width */
#define DATA_PULSE_US    1000  /* 1ms high + 1ms low per pulse */
#define END_DELAY_US     5000  /* 5ms end-of-frame */

struct keros_led {
	struct led_classdev cdev;
	struct gpio_desc *ctrl;
	struct gpio_desc *enable;
	struct mutex lock;
	struct work_struct btn_work;
	struct input_handle input_handle;
	bool input_registered;
	int current_mode;
};

/* Global pointer — single instance device */
static struct keros_led *keros_instance;

static void keros_send_mode(struct keros_led *led, int mode)
{
	unsigned long flags;
	int i;

	/* Ensure ctrl is LOW and stable before starting frame */
	gpiod_set_value(led->ctrl, 0);
	udelay(1000);
	local_irq_save(flags);

	/* SYNC: hold ctrl HIGH for ~6ms */
	gpiod_set_value(led->ctrl, 1);
	udelay(SYNC_US);

	/* DROP: ctrl LOW, wait 1ms */
	gpiod_set_value(led->ctrl, 0);
	udelay(DATA_PULSE_US);

	/* PULSE: mode times (1ms high + 1ms low) */
	for (i = 0; i < mode; i++) {
		gpiod_set_value(led->ctrl, 1);
		udelay(DATA_PULSE_US);
		gpiod_set_value(led->ctrl, 0);
		udelay(DATA_PULSE_US);
	}

	local_irq_restore(flags);

	/* END: 5ms gap, ensure LOW */
	udelay(END_DELAY_US);
	gpiod_set_value(led->ctrl, 0);
}

static void keros_btn_work_handler(struct work_struct *work)
{
	struct keros_led *led = container_of(work, struct keros_led, btn_work);
	int next;

	mutex_lock(&led->lock);

	/* Cycle: off -> 1 -> 2 -> ... -> 9 -> off */
	next = led->current_mode + 1;
	if (next > KEROS_MODE_MAX)
		next = KEROS_MODE_OFF;

	if (next == KEROS_MODE_OFF)
		keros_send_mode(led, KEROS_MODE_INIT);
	else
		keros_send_mode(led, next);

	led->current_mode = next;

	mutex_unlock(&led->lock);
}

/* --- input handler: listen for BTN_MODE across all input devices --- */

static void keros_input_event(struct input_handle *handle, unsigned int type,
			      unsigned int code, int value)
{
	if (type == EV_KEY && code == BTN_MODE && value == 1)
		schedule_work(&keros_instance->btn_work);
}

static int keros_input_connect(struct input_handler *handler,
			       struct input_dev *dev,
			       const struct input_device_id *id)
{
	struct keros_led *led = keros_instance;
	int ret;

	if (led->input_registered)
		return -EBUSY;

	led->input_handle.dev = dev;
	led->input_handle.handler = handler;
	led->input_handle.name = "keros-btn";

	ret = input_register_handle(&led->input_handle);
	if (ret)
		return ret;

	ret = input_open_device(&led->input_handle);
	if (ret) {
		input_unregister_handle(&led->input_handle);
		return ret;
	}

	led->input_registered = true;
	dev_info(&dev->dev, "keros-leds: listening for BTN_MODE on %s\n",
		 dev->name);

	return 0;
}

static void keros_input_disconnect(struct input_handle *handle)
{
	struct keros_led *led = keros_instance;

	input_close_device(handle);
	input_unregister_handle(handle);
	led->input_registered = false;
}

static const struct input_device_id keros_input_ids[] = {
	{
		.flags = INPUT_DEVICE_ID_MATCH_EVBIT |
			 INPUT_DEVICE_ID_MATCH_KEYBIT,
		.evbit  = { BIT_MASK(EV_KEY) },
		.keybit = { [BIT_WORD(BTN_MODE)] = BIT_MASK(BTN_MODE) },
	},
	{ }
};
MODULE_DEVICE_TABLE(input, keros_input_ids);

static struct input_handler keros_input_handler = {
	.event      = keros_input_event,
	.connect    = keros_input_connect,
	.disconnect = keros_input_disconnect,
	.name       = "keros-btn",
	.id_table   = keros_input_ids,
};

/* --- LED class interface --- */

static void keros_brightness_set(struct led_classdev *cdev,
				 enum led_brightness brightness)
{
	struct keros_led *led = container_of(cdev, struct keros_led, cdev);
	int mode;

	if (brightness == 0)
		mode = KEROS_MODE_INIT;  /* off */
	else if (brightness > KEROS_MODE_MAX)
		mode = KEROS_MODE_MAX;
	else
		mode = brightness;

	mutex_lock(&led->lock);
	keros_send_mode(led, mode);
	led->current_mode = (mode == KEROS_MODE_INIT) ? 0 : mode;
	mutex_unlock(&led->lock);
}

static enum led_brightness keros_brightness_get(struct led_classdev *cdev)
{
	struct keros_led *led = container_of(cdev, struct keros_led, cdev);

	return led->current_mode;
}

/* --- platform driver --- */

static int keros_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct keros_led *led;
	int ret;

	led = devm_kzalloc(dev, sizeof(*led), GFP_KERNEL);
	if (!led)
		return -ENOMEM;

	led->ctrl = devm_gpiod_get(dev, "ctrl", GPIOD_OUT_LOW);
	if (IS_ERR(led->ctrl))
		return dev_err_probe(dev, PTR_ERR(led->ctrl),
				     "failed to get ctrl GPIO\n");

	led->enable = devm_gpiod_get(dev, "enable", GPIOD_OUT_HIGH);
	if (IS_ERR(led->enable))
		return dev_err_probe(dev, PTR_ERR(led->enable),
				     "failed to get enable GPIO\n");

	mutex_init(&led->lock);
	INIT_WORK(&led->btn_work, keros_btn_work_handler);
	led->current_mode = 0;
	led->input_registered = false;

	led->cdev.name = "keros::ambient";
	led->cdev.max_brightness = KEROS_MODE_MAX;
	led->cdev.brightness_set = keros_brightness_set;
	led->cdev.brightness_get = keros_brightness_get;

	ret = devm_led_classdev_register(dev, &led->cdev);
	if (ret)
		return dev_err_probe(dev, ret, "failed to register LED\n");

	platform_set_drvdata(pdev, led);
	keros_instance = led;

	/* Register input handler to catch BTN_MODE */
	ret = input_register_handler(&keros_input_handler);
	if (ret)
		dev_warn(dev, "input handler registration failed: %d\n", ret);

	/* Init: enable chip + send init command */
	msleep(100);
	keros_send_mode(led, KEROS_MODE_INIT);

	dev_info(dev, "Keros LED controller ready\n");
	return 0;
}

static void keros_remove(struct platform_device *pdev)
{
	cancel_work_sync(&keros_instance->btn_work);
	input_unregister_handler(&keros_input_handler);
	keros_instance = NULL;
}

static int keros_suspend(struct device *dev)
{
	struct keros_led *led = dev_get_drvdata(dev);

	cancel_work_sync(&led->btn_work);

	mutex_lock(&led->lock);
	keros_send_mode(led, KEROS_MODE_INIT);
	gpiod_set_value(led->enable, 0);
	mutex_unlock(&led->lock);

	return 0;
}

static int keros_resume(struct device *dev)
{
	struct keros_led *led = dev_get_drvdata(dev);

	mutex_lock(&led->lock);
	gpiod_set_value(led->enable, 1);
	msleep(100);
	keros_send_mode(led, KEROS_MODE_INIT);
	if (led->current_mode > 0)
		keros_send_mode(led, led->current_mode);
	mutex_unlock(&led->lock);

	return 0;
}

static DEFINE_SIMPLE_DEV_PM_OPS(keros_pm_ops, keros_suspend, keros_resume);

static const struct of_device_id keros_of_match[] = {
	{ .compatible = "keros,pulse-leds" },
	{ }
};
MODULE_DEVICE_TABLE(of, keros_of_match);

static struct platform_driver keros_driver = {
	.probe = keros_probe,
	.remove = keros_remove,
	.driver = {
		.name = "keros-leds",
		.of_match_table = keros_of_match,
		.pm = pm_sleep_ptr(&keros_pm_ops),
	},
};
module_platform_driver(keros_driver);

MODULE_AUTHOR("Imanol-Mikel Barba Sabariego");
MODULE_DESCRIPTION("Keros Lite pulse-count LED controller");
MODULE_LICENSE("GPL");
