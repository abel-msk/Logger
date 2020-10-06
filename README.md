# Logger
Logging library for stripped down с++ framework.  (Arduino).
Log messages is formatted according to specified format. 
Messages for different log level can be filtered.

Logging levels are:

> LogLevel::INFO\
> LogLevel::WARNING\
> LogLevel::DEBUG\
> LogLevel::ERROR\

Uses channels for send log messages to various logging system.
Currently realized logging to Serial port (uses HardwareSerial.h from Arduino framework )
or stdout for use outside Arduino framework.



## Installing

```
git clone https://github.com/abel-msk/Logger.git
```

Required `BinTreeStore`. 
To install:
```
git clone https://github.com/abel-msk/BinTreeStore.git
```

## Usage

### Base usage

```
#include "Logger.h"

Logger logg("main");

void setup() {

}

void loop() {
    logg.error("This is log message");
}
```

###  Change default  log message format

```
#include "Logger.h"

logFactory.setChannelFormat("%date% [%level%] %clname%: %msg%");
Logger logg("main");
```


##  Format string

Default format is:\
     `%date% [%level%] %clname%: %msg%`\
where:
> `%date%`    - will be replaced with time stamp\
> `%clname%`  - class name\
> `%level%`   - log level  ERROR DEBUG INFO\
> `%msg%`     -  logging message\


##  Filter output
```
#include "Logger.h"

Logger logg("main");

logFactory.setFilter("main",LogLevel::DEBUG,true);

```