# Recur

A library to compute occurrences of recurring events.


## Recurring Events

Occurrences of recurring events are defined using special characters:

    H   Hours (00-23)
    M   Minutes (00-59)
    d   Day of month (00-31)
    w   Day of week (0-6; Sunday = 0)
    u   Day of week (1-7; Sunday = 7)
    y   Day in the year (mmdd)


## Uses

This library is suitable for applications that supports recurring events
for humans. Other libraries such as
[cron](https://en.wikipedia.org/wiki/Cron),[RFC
5545](http://tools.ietf.org/html/rfc5545#section-3.3.10) and [RFC
2445](http://tools.ietf.org/html/rfc2445#section-4.3.10) support
recurring events for computers.


## Examples

The following are examples of various tasks:

Weekly events:

    {
      task : "Weekly Monday morning meeting at 10am",
      recur: "H10w1"
    }

Several occurrences can be defined:

    {
      task : "Replace water in potted plants",
      recur: "H06w2w4w6"            # At 10am every Tue, Thu and Sat
    }

Monthly events. If time is not specified, defaults to 6am:

    {
      task : "Pay monthly bills",
      recur: "d25"                  # Equivalent to "H06d25"
    }

Yearly events:

    {
      task : "Boxing Day",
      recur: "y1226"
    }

Recurrences can be linearly combined:

    {
      task : "Weekly meetings",
      recur: "H10w1,H17w5"          # 10am on Mon and 5pm on Fri
    }


## Building the library

Running "make" will create "main", a sample executable to try out the
various recurring events.

See the Makefile for other targets. In particular, the "lib/librecur.a"
creates a static library suitable to for linking.


## Using the library

The library provides the following function:

    time_t recur(char *str);    # Returns next occurrence in number of
                                # seconds since Unix epoch


