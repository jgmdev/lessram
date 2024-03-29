# PHP-LESSRAM

This is the C extension version of the data structures, it is a work in
progress but the base to implement most of the other functionality should
already be here. It is proving to be faster than PHP version and using the
same technique, memory savings are huge. The API will mostly change to follow
the container standards of PHP.

## Usage

Now the library supports 3 modes of operation:

**LOWEST** - Lower memory saving and faster performance

Everything is farely fast including edits and removes.

**MODERATE** - Moderate memory saving and moderate performance

Looping is farely fast but edits and removes are really slow.

**HIGHEST** - Higher memory saving and lower performance

Memory saving is cool and looping is bearable, but as in moderate edits and
removes are really slow.

### Examples

```php
$view = new LessRam\Storage(LessRam\Storage::LOWEST);
$view->append("Hello world lowest!");
echo $view->next();

$view = new LessRam\Storage(LessRam\Storage::MODERATE);
$view->append("Hello world lowest!");
echo $view->next();

$view = new LessRam\Storage(LessRam\Storage::HIGHEST);
$view->append("Hello world lowest!");
echo $view->next();
```

### Note

Editing after unreferencing is not possible eg:

```php
$view = new LessRam\Storage();
$view[] = ["my value"];
$view[0][0] = "something else"; // <-- not possible
```
Unaware if there is a mechanism on the zend engine to handle such cases in which
the array doesn't stores references to zvals.

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

Then enable the extension on your php.ini or invoke php as follows:

```sh
php -d extension=lessram.so script.php
```

## Benchmarks

And here are some benchmarks that show the nice improvement over PHP
implementation of same algorithm.

```
=======================================================================
String store test with native C extension:
=======================================================================
      Structures |  add 1048576 | loop all | total time |  memory usage
 LessRam Highest |        0.21s |    0.14s |      0.36s |          19MB
LessRam Moderate |        0.19s |    0.05s |      0.25s |          34MB
  LessRam Lowest |        0.18s |    0.05s |      0.23s |          40MB
   SplFixedArray |        0.10s |    0.03s |      0.15s |          65MB
SplDoublyLinkedL |        0.23s |    0.03s |      0.28s |          89MB
       Ds\Vector |        0.14s |    0.02s |      0.19s |          72MB
          Ds\Map |        0.24s |    0.03s |      0.29s |          85MB
       PHP Array |        0.12s |    0.02s |      0.17s |          81MB


=======================================================================
Array store test with native C extension:
=======================================================================
      Structures |  add 1048576 | loop all | total time |  memory usage
 LessRam Highest |        0.47s |    0.56s |      1.03s |          32MB
LessRam Moderate |        0.41s |    0.52s |      0.94s |          47MB
  LessRam Lowest |        0.36s |    0.39s |      0.75s |          49MB
   SplFixedArray |        0.35s |    0.23s |      0.61s |         441MB
SplDoublyLinkedL |        0.31s |    0.29s |      0.67s |         481MB
       Ds\Vector |        0.23s |    0.23s |      0.55s |         448MB
          Ds\Map |        0.29s |    0.25s |      0.62s |         461MB
  PHP Array      |        0.22s |    0.12s |      0.42s |         457MB
   -> JSON       |        0.47s |    0.42s |      1.03s |         289MB
   -> Serialized |        0.29s |    0.28s |      0.60s |         289MB


=======================================================================
Integers store test with native C extension:
=======================================================================
      Structures |  add 1048576 | loop all | total time |  memory usage
 LessRam Highest |        0.17s |    0.11s |      0.28s |           8MB
LessRam Moderate |        0.15s |    0.05s |      0.20s |          23MB
  LessRam Lowest |        0.16s |    0.05s |      0.21s |          25MB
   SplFixedArray |        0.02s |    0.02s |      0.05s |          17MB
SplDoublyLinkedL |        0.15s |    0.02s |      0.18s |          41MB
       Ds\Vector |        0.09s |    0.02s |      0.13s |          24MB
          Ds\Map |        0.14s |    0.02s |      0.17s |          37MB
       PHP Array |        0.06s |    0.01s |      0.08s |          33MB
```

## TODO

* Implement additional data storage container to store objects and resources.
* Implement Serializable interfaces
* Add support for associative indexes.