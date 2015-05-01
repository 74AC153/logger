# Remote control logging

## Overview

This software has three parts:
-  logging subsystem functionality: logger.{c,h}
-  remote control: logger_ctl.c
-  log items: log_items.{c,h}
-  example code: logger_test.c

To use:
1.  pull all sources into your tree except the example code.
2.  add a build target for logger_ctl.c
3.  modify log_items.h to suit your application
4.  modify your application to use the log items defined in log_items.h in
    a manner similar to that shown in the example logger_test.c

## logger_ctl

Run logger_ctl with no arguments to see an overview of its command-line
arguments.

Example: Assuming that all error log items contain the string 'error':

    logger_ctl list | grep 'error' | xargs logger_ctl enable
