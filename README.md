# Recur

A library to compute occurrences of recurring events.

This library is suitable for applications that supports recurring events
for humans. Other libraries such as
[cron](https://en.wikipedia.org/wiki/Cron), [RFC
5545](http://tools.ietf.org/html/rfc5545#section-3.3.10) and [RFC
2445](http://tools.ietf.org/html/rfc2445#section-4.3.10) support
recurring events for computers.


## Recurring Events

The library lets user define recurring events using special characters:

    H   Hours (00-23)
    M   Minutes (00-59)
    d   Day of month (00-31)
    w   Day of week (0-6; Sunday = 0)
    u   Day of week (1-7; Sunday = 7)
    y   Day in the year (mmdd)


## Examples

The following are examples of how recurring events maybe be used to
define tasks with varying frequencies.

Weekly events:

    {
      task : "Weekly Monday morning meeting at 10am",
      recur: "H10w1"
    }

Several occurrences can be defined:

    {
      task : "Replace water in potted plants",
      recur: "H10w2w4w6"            # At 10am every Tue, Thu and Sat
    }

Monthly events. If a time is not specified, defaults to 6am:

    {
      task : "Pay monthly bills",   # Every 25th of the month
      recur: "d25"                  # Equivalent to "H06d25"
    }

Yearly events:

    {
      task : "Boxing Day",
      recur: "y1226"
    }

Recurrences can also be linearly combined:

    {
      task : "Weekly meetings",
      recur: "H10w1,H17w5"          # 10am on Mondays and 5pm on Fridays
    }


## Building the library

Running "make" will create a sample executable called "main" to try out
the various recurring events.

    $ make
    $ ./main "H10w1,H17w5"

See the Makefile for other targets. In particular, the "lib/librecur.a"
creates a static library suitable for linking with your own project.


## Using the library

The library provides the following function:

    time_t recur(char *str);    # Returns next occurrence in number of
                                # seconds since Unix epoch

The grammar for recurring events are defined in file "recur.y".
