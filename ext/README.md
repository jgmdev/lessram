# PHP-LESSRAM

This is the C extension version of the data structures, it is a work in
progress but the base to implement most of the other functionality should
already be here. It is proving to be faster than PHP version and using the
same technique, memory savings are huge. The API will mostly change to follow
the container standards of PHP.

## Usage

Now the library supports 3 modes of operation:

* LOWEST - Lower memory saving and faster performance
* MODERATE - Moderate memory saving and moderate performance
* HIGHEST - Higher memory saving and lower performance

### Example

```php
$view = new LessRam\DynamicArray(LessRam\DynamicArray::LOWEST);
$view->appendData("Hello world!");
echo $view->next();
```

# Installation

For now the extension only works on Linux but since it doesn't requires any 
external dependencies creating a config.w32 should not be that hard.

## Steps to install

```sh
git clone https://github.com/jgmdev/lessram
cd lessram/ext
phpize
./configure
make
sudo make install
```

## Benchmarks

And here are some benchmarks that show the nice improvement over plain PHP
using the same algorithm on C.

```
=======================================================================
String store test with native C extension:
=======================================================================
     Structures |  add 1048576 | loop all | total time |  memory usage
LessRam Highest |        0.25s |    0.13s |      0.38s |          19MB
LessRam Moderat |        0.32s |    0.05s |      0.37s |          35MB
 LessRam Lowest |        0.23s |    0.05s |      0.28s |          40MB
  SplFixedArray |        0.10s |    0.03s |      0.16s |          65MB
      Ds\Vector |        0.15s |    0.02s |      0.19s |          72MB
         Ds\Map |        0.22s |    0.03s |      0.27s |          85MB
   Native Array |        0.12s |    0.02s |      0.17s |          81MB


=======================================================================
Array store test with native C extension:
=======================================================================
     Structures |  add 1048576 | loop all | total time |  memory usage
LessRam Highest |        0.52s |    0.53s |      1.05s |          32MB
LessRam Moderat |        0.51s |    0.39s |      0.90s |          48MB
 LessRam Lowest |        0.41s |    0.37s |      0.78s |          49MB
  SplFixedArray |        0.34s |    0.23s |      0.59s |         441MB
      Ds\Vector |        0.23s |    0.25s |      0.55s |         448MB
         Ds\Map |        0.27s |    0.23s |      0.57s |         461MB
   Native Array |        0.22s |    0.14s |      0.44s |         457MB
```

As you can see the difference in performance is not that huge now :)

## TODO

* Implement additional data storage container to store objects and resources.
* Implement Countable, Iterable, etc. interfaces
* Add support for associative indexes.
* Give a better name to the class: LessRam\Storage?