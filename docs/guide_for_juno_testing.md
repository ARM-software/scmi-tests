**Guide for testing on Juno ADP**
=================================

Table of Contents:
- [Introduction](#introduction)
- [Juno ADP software stack](#juno-adp-software-stack)
- [Using mailbox test driver](#mailbox-test-driver)
  * [Doorbell support patches for mailbox](#doorbell-support-patches-for-mailbox)
  * [Additional changes to enable mailbox test driver](#additional-changes-to-enable-mailbox-test-driver)

Introduction
-------
This document lists the instructions that must be followed to enable the Juno Linux image to run SCMI Linux application using the mailbox test driver interface.

Juno ADP software stack
-------
The [Arm connected community] maintains the information about the software stack support for Juno ADP. 

Linux kernel 
-------
The following changes must be made in the Linux kernel source code:

### Mailbox test driver
To use SCMI test agent on Juno platform, the Linux kernel must be rebuilt to include the mailbox test driver with mailbox doorbell support and additional changes. <br>
Doorbell support patches for mailbox and additional changes to enable mailbox test driver are tested against Linux kernel version 4.13-rc5. 

### Doorbell support patches for mailbox
The doorbell support for mailbox driver is enabled by applying a patch series that is currently discussed in LKML. For more information, see [Mailbox doorbell support patches]. These patches are hosted on the [Mailbox doorbell support repo]. This patch must be applied to the Juno mainline tracker kernel. The kernel can be downloaded using the steps that are mentioned on [Arm connected community].

### Additional changes to enable mailbox test driver
In addition to applying the patches, follow these steps before starting the kernel build:

`Enable mailbox test driver`: By default, Juno ADP kernels do not have the mailbox test driver enabled. Set CONFIG_MAILBOX_TEST=y in kernel config to include mailbox test driver in the kernel.

`Update device tree`: The Juno device trees currently have a node describing mailbox prior to the doorbell mode support. Also, the current device trees do not have a node describing the mailbox test. <br>
You must do the following in arch/arm64/boot/dts/arm/juno-base.dtsi. Replace existing node describing mailbox with the following:

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
Then, add a new node describing the mailbox test as shown below:

```
	mailbox-test@2e000000 {
		compatible = "mailbox-test";
		mbox-names = "tx";
		mboxes = <&mailbox 0 0>;
		reg = <0x0 0x2e000000 0x0 0x80>, <0x0 0x2e000000 0x0 0x80>;
	};
```

`Modify mailbox driver to prevent format conversion`: The current version of mailbox driver always converts raw binary data to hex format. For SCMI test agent, we expect raw data unmodified for processing. The change that is shown below prevents the format change. 
<br> The driver support to add this as a configurable option is planned for the future.
<br> Until that change is added, the change that is shown below is required in the mbox_test_message_read function in drivers/mailbox/mailbox-test.c:

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
The current change in mailbox test driver is not an ideal solution. A better solution is to use sysfs entry for configurability. This enhancement will be upstreamed in the future, thereby making this change redundant.

## Kernel build
Build the kernel and device tree after making the changes for Juno ADP, and flash the images in target.

For instructions to build the test suite for arm/juno platform and running it, see relevant sections in [User Guide].

- - - - - - - - - - - - - - - -

_Copyright (c) 2017, Arm Limited and Contributors. All rights reserved._

[ARM connected community]:		https://community.arm.com/dev-platforms
[Mailbox doorbell support patches]:	https://lkml.org/lkml/2017/5/24/339
[Mailbox doorbell support repo]:	https://git.kernel.org/pub/scm/linux/kernel/git/sudeep.holla/linux.git/log/?h=for-list/arm_mhu_doorbell
[User Guide]:				./user_guide.md
