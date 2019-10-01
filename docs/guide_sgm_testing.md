**Guide for testing on SGM platforms**
=================================

Table of Contents:
- [Introduction](#introduction)
- [Software stack](#software-stack)
- [Linux kernel](#linux-kernel)
  * [Mailbox test driver](#mailbox-test-driver)
  * [Doorbell support patches for mailbox](#doorbell-support-patches-for-mailbox)
  * [Additional changes to enable mailbox test driver](#additional-changes-to-enable-mailbox-test-driver)
- [Kernel build](#kernel-build)

Introduction
-------
This document lists the instructions that must be followed to run SCMI ACS as Linux application using the mailbox test driver interface on SGM platforms.
For an introduction to the System Guidance for Mobile (SGM) platforms, please refer to the [Arm Developer documentation].

Software Stack
-------
Arm provides a [super-project] with guides for building and running a full software stack on Arm platforms. This project provides a convenient wrapper around the various build systems involved in the software stack. Please contact Arm for support on software stack for SGM platform.

Linux kernel
-------
The following changes must be made in the Linux kernel source code after downloading software stack for SGM.

### Mailbox test driver
To use SCMI test agent on SGM platform, the Linux kernel must be rebuilt to include the mailbox test driver with mailbox doorbell support and additional changes. Doorbell support patches for mailbox and additional changes to enable mailbox test driver are tested against Linux kernel version 4.13.

### Doorbell support patches for mailbox
The doorbell support for mailbox driver is enabled by applying a patch series that is currently discussed in LKML. For more information, see [Mailbox doorbell support patches]. Pick up the relevant mailbox patches from [Mailbox doorbell support repo]. These patches must be applied to the linux kernel.

### Additional changes to enable mailbox test driver
In addition to applying the patches, follow these steps before starting the kernel build.

`Enable mailbox test driver`: Set CONFIG_MAILBOX_TEST=y in kernel config to include mailbox test driver in the kernel.

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
Build the kernel and device tree after making the changes and use the run-scripts for running the SGM

For instructions to build the test suite for sgm platform and running it, see relevant sections in [User Guide].

- - - - - - - - - - - - - - - -

_Copyright (c) 2019, Arm Limited and Contributors. All rights reserved._

[ARM Developer documentation]:		https://developer.arm.com/tools-and-software/simulation-models/fixed-virtual-platforms
[Mailbox doorbell support patches]:	https://lkml.org/lkml/2017/5/24/339
[Mailbox doorbell support repo]:	https://git.kernel.org/pub/scm/linux/kernel/git/sudeep.holla/linux.git/log/?h=scmi_mhu_dt_changes
[User Guide]:				./user_guide.md
[super-project]:  https://git.linaro.org/landing-teams/working/arm/arm-reference-platforms.git/about/docs/user-guide.rst
