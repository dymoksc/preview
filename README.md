### Multithreaded web server prototype

This repo contains a simple web server prototype that can only serve text files on 'GET' requests. 
Requests are received via epoll and socket and then distributed among a thread pool's threads. A
couple of classes are also covered with Boost unit tests.

#### Dependencies

* Linux (tested on kernel 5.4.95, Manjaro 20.2.1)
* Boost library v. 1.75 installed in system
* Ext Boost DI (from GitHub, see [Preparations](Preparations))
* Cmake (tested with version 3.19.4)

#### Preparations

1. While in the root dir, clone Boost Ext DI project to `libs` dir:

   ```sh
   git clone https://github.com/boost-ext/di.git libs/di
   ```

2. Fill `defintions.h` with suitable parameters:
   1. `THREADS_COUNT` - integer. It might be suboptimal to use more threads, that there are CPU cores
   2. `WEB_ROOT` - absolute path ending with '/'
   3. `PORT_TO_LISTEN`
   
3. Compile on all cores:

   ```sh
   $ cmake --build cmake-build-debug --target preview -- -j $(nproc)
   ```
   
4. Run with the following command:

   ```sh
   $ ./cmake-build-debug/preview 
   ```

#### Tests

1. Compilation:

   ```shell
   $ cmake --build cmake-build-debug --target preview_boost_tests -- -j $(nproc)
   ```

2. Running:

   ```shell
   $ ./cmake-build-debug/boost_tests/preview_boost_tests
   ```
