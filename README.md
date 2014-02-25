rest-cpp
========
REST-like framework and server for blazing fast web applications in
C++11.


Quick start
-----------
    $ rest-cpp new blog
    $ cd blog
    $ rest-cpp server

Navigate your browser to http://127.0.0.1:8080 now.


Building
--------
On OS X Commanand Line Tools are required; on Linux gcc-4.8 or newer is
required. Not tested on other platforms.


### Library
Build library using `make` on root directory.

### Installation
Run `make install` on project folder - it will build the library and
copy headers, shared library and generator to `/usr/local`.

### Example
After bulding the [Library](#library), go to `example/todo_server` and use `make`. 


Usage
-----
You may build your own apps from scratch, but you can just `#include <rest/rest.h>`
and use simplified workflow. To make it even easier, you can use `rest-cpp` utility
to do some work for you.

### rest-cpp utility
`rest-cpp` allows you to create and manage app.
It is installed to `/usr/local/bin` and requires Python in version at
least 2.5 (which probably you have already installed).

#### New application
Use `rest-cpp new [directory]` to create new application. `directory`
isn't required, app will be created in current directory if omitted.

Created are two files:
  - `init.cpp` with basic "hello world"
  - `Makefile` with build commands

Existing files are skipped, to update file to new version, just remove
previous one.

##### Makefile
Available tasks:
  - `make server` - default action, build and start server
  - `make build` - build server

Available options:
  - `address=ip_or_host` - address for server to bind, default: `0.0.0.0`
  - `port=number` - port to listen, default: `8080` (ports lower than 1024 may require superuser privileges)
  - `workers=number` - number of workers, default: `4`
  - `dispatcher=lc/rr` - workers dispatcher algorithm - `lc` for `LeastConnections`, `rr` for `RoundRobin`, default: `lc`

To use options pass them to `make`, i.e. `make server workers=2 port=9000`.
Options are complitation-time, not runtime - this means, to i.e. change
port, you must pass port to make during building process.

*`rest-cpp` wraps make, so you can use `rest-cpp build` and `rest-cpp server` instead of
make (you can use the same options as above).*

#### Generators
`rest-cpp` has builtin generators for resources and services. They work
assuming you made your app using `rake-cpp new`, that is you use
`init.cpp` file. Generators must be run in root directory of the
project.

Resources classes will be stored in `resources` directory, `services`
for services. Generators *do not check* if route or other symbol
already exists in `init.cpp`. However, they *do not override* service or resource class file.

##### Inline Service
Inline Service is simple service implemented using C++11 lambda. Use
`rest-cpp generate inline PATH` to generate one, where `PATH` is URI to
the new service.

The following code will be added to `routes()` in init.cpp:

    r->match("PATH", [](REST::Service* service) {
      throw REST::HTTP::NotImplemented();
    });

##### Simple Service
Simple Service is implemented as function in `init.cpp` file.
Use `rest-cpp generate simple NAME PATH` to generate one, where `NAME`
is name of function implementing service and PATH is the URI.

Two things are added to `init.cpp`:

    create_service(NAME) {
      throw REST::HTTP::NotImplemented();
    }

    // inside routes()
    r->match("PATH", NAME);

###### Simple JSON Service
This is variation of simple service with JSON response enabled by
default. Instead of `create_service`, `create_json_service` is used.

##### Service

##### Resource

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
