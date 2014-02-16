rest-cpp
========

REST-like framework and server for blazing fast web applications in
C++11.


Building
--------

On OS X Commanand Line Tools are required; on Linux gcc-4.8 or newer is
required. Not tested on other platforms.


### Library

Build library using `make` on root directory. Then copy created library to
`/usr/local/lib` or to project directory.

### Example

Example contains symlinks to library, so copying it isn't needed. To
build example, go to `example/todo_server` and use `make`. 
You may copy Makefile and change it accordingly to your project and needs.

However few more tasks are available:
  - `make run` - build and start server
  - `make library` - build library
  - `make server` - default action, build server

There are configuration options available such as:
  - `address=ip_or_host` - address for server to bind, default: `0.0.0.0`
  - `port=number` - port to listen, default: `8080` (ports lower than 1024 may require superuser privileges)
  - `workers=number` - number of workers, default: `4`
  - `dispatcher=lc/rr` - workers dispatcher algorithm - lc for LeastConnections, rr for RoundRobin, default: `lc`

To use options pass them to make, i.e. `make run workers=2 port=9000`.


Example
-------

lorem ipsum


Basis of operation
------------------

lorem ipsum


Authors
-------

- Amadeusz Juskowiak - amadeusz[at]me.com
- Błażej Kotowski - kotowski.blazej[at]gmail.com

Made with love, inspired by put.poznan.pl
