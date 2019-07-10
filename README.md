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
    add 1048577 | 0.41s           | 0.30s           | 0.10s           | 0.14s
  get two items | 0.00s           | 0.00s           | 0.00s           | 0.00s
       loop all | 1.42s           | 2.22s           | 0.03s           | 0.02s
      serialize | 0.08s           | 0.04s           | 0.19s           | 0.29s
    unserialize | 0.05s           | 0.03s           | 0.33s           | 0.14s
     total time | 1.96s           | 2.59s           | 0.67s           | 0.64s
   memory usage | 29MB            | 19MB            | 129MB           | 113MB

=======================================================================
String store test gzipped (StaticArray and DynamicArray not compressed):
=======================================================================
        Measure | StaticArray     | DynamicArray    | SplFixedArray   | Native
    add 1048577 | 0.42s           | 0.34s           | 31.72s          | 10.99s
  get two items | 0.00s           | 0.00s           | 0.00s           | 0.00s
       loop all | 1.28s           | 2.21s           | 0.03s           | 0.02s
      serialize | 0.08s           | 0.04s           | 0.41s           | 0.17s
    unserialize | 0.05s           | 0.03s           | 0.28s           | 0.15s
     total time | 1.83s           | 2.62s           | 32.44s          | 11.38s
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
    add 1048577 | 0.88s           | 0.66s           | 0.35s           | 0.23s
  get two items | 0.00s           | 0.00s           | 0.00s           | 0.00s
       loop all | 2.09s           | 6.23s           | 0.24s           | 0.13s
      serialize | 0.15s           | 0.12s           | 0.44s           | 0.87s
    unserialize | 0.12s           | 0.09s           | 1.36s           | 0.69s
     total time | 3.25s           | 7.11s           | 2.41s           | 2.12s
   memory usage | 57MB            | 46MB            | 505MB           | 489MB

=======================================================================
Array store test gzipped (StaticArray and DynamicArray not compressed):
=======================================================================
        Measure | StaticArray     | DynamicArray    | SplFixedArray   | Native
    add 1048577 | 0.75s           | 0.69s           | 71.37s          | 13.97s
  get two items | 0.00s           | 0.00s           | 0.00s           | 0.00s
       loop all | 2.09s           | 6.05s           | 0.03s           | 0.03s
      serialize | 0.15s           | 0.11s           | 0.58s           | 0.69s
    unserialize | 0.10s           | 0.08s           | 0.39s           | 0.17s
     total time | 3.09s           | 6.94s           | 72.38s          | 14.92s
   memory usage | 57MB            | 46MB            | 161MB           | 145MB
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

## TODO

Add associative data structures also known as maps where the index could
be any string.