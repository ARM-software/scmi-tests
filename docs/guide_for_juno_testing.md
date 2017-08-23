**Guide for testing on Juno ADP**
=================================

Table of Contents:
- [Introduction](#introduction)
- [Juno ADP software stack](#juno-adp-software-stack)
- [Using mailbox test driver](#using-mailbox-test-driver)
  * [Doorbell support patches for mailbox](#doorbell-support-patches-for-mailbox)
  * [Additional changes to enable mailbox test driver](#additional-changes-to-enable-mailbox-test-driver)

Introduction
-------
This document outlines the instructions to be followed to prepare the Juno images to test SCMI from Linux kernel userspace on top of mailbox test driver interface.

Juno ADP software stack
-------
The ARM connected community maintains the information about the software stack support for Juno ADP. This can be seen at [ARM connected community].

Using mailbox test driver
-------
In order to use scmi test agent on Juno platform, the Juno kernel needs to be rebuilt with mailbox test driver with mailbox doorbell support and additional changes. The below mentioned steps are tested against Linux kernel version 4.13-rc5. 

### Doorbell support patches for mailbox
The door bell support for mailbox driver is done with a patch series currently discussed in LKML. This can be seen at [Mailbox doorbell support patches]. These patches are hosted at [Mailbox doorbell support repo]. User need to apply this on Juno mainline tracker kernel got from the connected community page information.

### Additional changes to enable mailbox test driver

`Enable mailbox test driver`: By default the Juno ADP kernels do not have mailbox test driver enabled. Set CONFIG_MAILBOX_TEST=y in kernel config to include mailbox test driver in the kernel.

`Update device tree`: The Juno device trees currently have a node describing mailbox prior to the doorbell mode support. Additionally the current device tree do not have a node describing the mailbox test. User should do the following in arch/arm64/boot/dts/arm/juno-base.dtsi. Replace existing node describing mailbox with the following:

```
	mailbox: mhu@2b1f0000 {
			compatible = "arm,mhu-doorbell", "arm,primecell";
			reg = <0x0 0x2b1f0000 0x0 0x1000>;
			interrupts = <GIC_SPI 36 IRQ_TYPE_LEVEL_HIGH>,
				     <GIC_SPI 35 IRQ_TYPE_LEVEL_HIGH>;
			interrupt-names = "mhu_lpri_rx",
					  "mhu_hpri_rx";
			#mbox-cells = <2>;
			mbox-name = "ARM-MHU";
			clocks = <&soc_refclk100mhz>;
			clock-names = "apb_pclk";
	};
```
And then add a new node describing the mailbox test as below:

```
	mailbox-test@2e000000 {
		compatible = "mailbox-test";
		mbox-names = "tx";
		mboxes = <&mailbox 0 0>;
		reg = <0x0 0x2e000000 0x0 0x80>, <0x0 0x2e000000 0x0 0x80>;
	};
```

`Modify mailbox driver to prevent format conversion`: Current version of mailbox driver always do convert raw binary data to hex format. For scmi test agent we expect raw data unmodified for processing. This change prevents the format change. In future the driver will support this as a configurable option preferrably controlled via a sys fs entry. Now modify mbox_test_message_read function in drivers/mailbox/mailbox-test.c as below:

```
	}
	*(touser + l) = '\0';

+	ret = simple_read_from_buffer(userbuf, count, ppos, tdev->rx_buffer,
+					MBOX_HEXDUMP_MAX_LEN);
+
	memset(tdev->rx_buffer, 0, MBOX_MAX_MSG_LEN);
	mbox_data_ready = false;

	spin_unlock_irqrestore(&tdev->lock, flags);

-	ret = simple_read_from_buffer(userbuf, count, ppos, touser, MBOX_HEXDUMP_MAX_LEN);
waitq_err:
	__set_current_state(TASK_RUNNING);
	remove_wait_queue(&tdev->waitq, &wait);
```
Do note the current change described above in mailbox test driver function is not an elegant solution. A good solution to control this possibly via a sys fs entry will be upstreamed in future for this mailbox test driver, which will make this change un-necessary.

Build kernel and device tree after the above changes for Juno ADP and flash the images in target.

For the instructions to build the test suite for arm/juno platform and running it please refer back to relevant sections in [User Guide].

- - - - - - - - - - - - - - - -

_Copyright (c) 2017, ARM Limited and Contributors. All rights reserved._

[ARM connected community]:		https://community.arm.com/dev-platforms
[Mailbox doorbell support patches]:	https://lkml.org/lkml/2017/5/24/339
[Mailbox doorbell support repo]:	https://git.kernel.org/pub/scm/linux/kernel/git/sudeep.holla/linux.git/log/?h=for-list/arm_mhu_doorbell
[User Guide]:				./user_guide.md
