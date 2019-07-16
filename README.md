# LessRam

PHP implementations of array data structures that consume less memory
than native php arrays with the drawback of been less performant:

## Usage

Currently the project only includes two array structures that implement
ArrayAccess, Countable, Serializable and Iterator, these two structures are:

* StaticArray - Can dynamically grow but elements on it can not be modified.
* DynamicArray - Can dynamically grow and elements on it can be modified or removed.

Both of them only support numeric keys, additional implementations with support
for string indexes may be added later.

**StaticArray Example:**

```php
$list = new LessRam\StaticArray();
$list[] = "value1";
$list[] = "value2";
$list[] = ["index" => "somearray"];

foreach($list as $value)
{
  echo $value;
}

$list->clear();
```

**DynamicArray Example:**

```php
$list = new LessRam\DynamicArray();
$list[] = "value1";
$list[] = "value2";
$list[] = ["index" => "somearray"];

foreach($list as $value)
{
  echo $value;
}

$list[0] = "newvalue";
unset($list[1]); // Remove value2
print_r($list[1]); // show array which now took place of value2

$list->clear();
```

As you can see on the examples above, beside storing strings they can also
store arrays and even objects, when inserting such variables they are serialized
which results in memory savings, and unserilizing when access to it is needed.
This saves a lot of memory but impacts performance, so you should weight your
requirements. For more info on performance check the benchmarks below.

## Benchmark

The first results shown below are inserting a string to each of the tested
data structures 1048577 times, the first test doesn't uses gzip compression while
the second test does for SplFixedArray and PHP native array:

```php
$list[] = "hello world" . $i;
```

### Results

```
=======================================================================
String store test:
=======================================================================
        Measure | StaticArray     | DynamicArray    | SplFixedArray   | Native
    add 1048577 | 0.42s           | 0.29s           | 0.10s           | 0.15s
  get two items | 0.00s           | 0.00s           | 0.00s           | 0.00s
       loop all | 1.31s           | 2.11s           | 0.03s           | 0.02s
      serialize | 0.08s           | 0.04s           | 0.20s           | 0.31s
    unserialize | 0.05s           | 0.03s           | 0.34s           | 0.15s
     total time | 1.87s           | 2.48s           | 0.69s           | 0.66s
   memory usage | 29MB            | 19MB            | 129MB           | 113MB

=======================================================================
String store test gzipped (StaticArray and DynamicArray not compressed):
=======================================================================
        Measure | StaticArray     | DynamicArray    | SplFixedArray   | Native
    add 1048577 | 0.44s           | 0.35s           | 31.95s          | 10.72s
  get two items | 0.00s           | 0.00s           | 0.00s           | 0.00s
       loop all | 1.28s           | 2.10s           | 0.03s           | 0.02s
      serialize | 0.08s           | 0.05s           | 0.44s           | 0.16s
    unserialize | 0.05s           | 0.03s           | 0.28s           | 0.15s
     total time | 1.86s           | 2.53s           | 32.71s          | 11.09s
   memory usage | 29MB            | 19MB            | 129MB           | 113MB
```

This second test stores an array inside the data structure 1048577 times
as follows:

```php
$list[] = ["name" => "hello world" . $i];
```

### Results

```
=======================================================================
Array store test:
=======================================================================
        Measure | StaticArray     | DynamicArray    | SplFixedArray   | Native
    add 1048577 | 0.79s           | 0.60s           | 0.34s           | 0.24s
  get two items | 0.00s           | 0.00s           | 0.00s           | 0.00s
       loop all | 1.99s           | 4.36s           | 0.23s           | 0.12s
      serialize | 0.12s           | 0.08s           | 0.43s           | 0.84s
    unserialize | 0.08s           | 0.06s           | 1.24s           | 0.67s
     total time | 2.98s           | 5.10s           | 2.25s           | 2.06s
   memory usage | 42MB            | 32MB            | 505MB           | 489MB

=======================================================================
Array store test gzipped (StaticArray and DynamicArray not compressed):
=======================================================================
        Measure | StaticArray     | DynamicArray    | SplFixedArray   | Native
    add 1048577 | 0.76s           | 0.66s           | 70.51s          | 13.86s
  get two items | 0.00s           | 0.00s           | 0.00s           | 0.00s
       loop all | 1.96s           | 4.36s           | 0.03s           | 0.03s
      serialize | 0.11s           | 0.08s           | 0.60s           | 0.74s
    unserialize | 0.07s           | 0.05s           | 0.38s           | 0.17s
     total time | 2.91s           | 5.15s           | 71.53s          | 14.85s
   memory usage | 42MB            | 32MB            | 161MB           | 145MB
```

I can say that the best balance between memory usage and performance is
achieved with the **StaticArray** implementation. To run your own benchmarks run:

```sh
cd lessram
php bench.php
```

## Testing

Some tests have been written to check the reliability of these structures but
maybe more tests are needed. To run the tests execute the php unit executable
as follows:

```sh
cd lessram
./vendor/bin/phpunit
```

## PHP C Extension (WIP)

I started implementing the same algorithm used on the PHP implementation
of the data structures, lots of stuff needs to be done but the base work
should be in place. Check the [ext](https://github.com/jgmdev/lessram/tree/master/ext) 
directory for more information and benchmarks.


## TODO

Add associative data structures also known as maps where the index could
be any string.