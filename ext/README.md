# PHP-LESSRAM

This is the C extension version of the data structures, it is a work in
progress but the base to implement most of the other functionality should
already be here. It is proving to be faster than PHP version and using the
same technique, memory savings are huge. The API will mostly change to follow
the container standards of PHP.

## Example

```php
$view = new LessRam\DynamicArray();
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
   DynamicArray |        0.24s |    0.15s |      0.38s |          19MB
  SplFixedArray |        0.11s |    0.03s |      0.14s |          65MB
      Ds\Vector |        0.14s |    0.02s |      0.19s |          72MB
         Ds\Map |        0.21s |    0.03s |      0.26s |          85MB
         Native |        0.12s |    0.02s |      0.17s |          81MB


=======================================================================
Array store test with native C extension:
=======================================================================
     Structures |  add 1048576 | loop all | total time |  memory usage
   DynamicArray |        0.52s |    0.58s |      1.10s |          32MB
  SplFixedArray |        0.33s |    0.24s |      0.59s |         441MB
      Ds\Vector |        0.23s |    0.26s |      0.57s |         448MB
         Ds\Map |        0.31s |    0.24s |      0.64s |         461MB
         Native |        0.22s |    0.15s |      0.45s |         457MB
```

As you can see the difference in performance is not that huge now :)