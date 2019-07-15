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
     Structures | add 104857 | loop all | total time |  memory usage
   DynamicArray |      0.24s |    0.15s |      0.39s |          19MB
  SplFixedArray |      0.10s |    0.03s |      0.14s |          65MB
      Ds\Vector |      0.15s |    0.02s |      0.19s |          72MB
         Ds\Map |      0.27s |    0.03s |      0.32s |         121MB
         Native |      0.15s |    0.02s |      0.20s |         113MB


=======================================================================
Array store test with natice C extension:
=======================================================================
     Structures | add 104857 | loop all | total time |  memory usage
   DynamicArray |      0.54s |    0.56s |      1.10s |          32MB
  SplFixedArray |      0.34s |    0.23s |      0.58s |         441MB
      Ds\Vector |      0.23s |    0.25s |      0.56s |         448MB
         Ds\Map |      0.34s |    0.24s |      0.66s |         497MB
         Native |      0.26s |    0.14s |      0.48s |         489MB
```

As you can see the difference in performance is not that huge now :)