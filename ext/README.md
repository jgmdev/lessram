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
        Measure | DynamicArray    | SplFixedArray   | Native         
    add 1048577 | 0.24s           | 0.10s           | 0.15s          
       loop all | 0.15s           | 0.03s           | 0.02s          
     total time | 0.39s           | 0.14s           | 0.18s          
   memory usage | 19MB            | 65MB            | 113MB          


=======================================================================
Array store test with native C extension:
=======================================================================
        Measure | DynamicArray    | SplFixedArray   | Native         
    add 1048577 | 0.53s           | 0.34s           | 0.24s          
       loop all | 0.54s           | 0.23s           | 0.13s          
     total time | 1.07s           | 0.58s           | 0.44s          
   memory usage | 32MB            | 441MB           | 489MB
```

As you can see the difference in performance is not that huge now :)