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
data structures 1048577 times as follows:

```php
$list[] = "hello world" . $i;
```

### Results

```
===============================
String store test:
===============================
        Measure | StaticArray     | DynamicArray    | Native         
    add 1048577 | 0.43s           | 0.33s           | 0.16s          
  get two items | 0.00s           | 0.00s           | 0.00s          
       loop all | 1.28s           | 2.23s           | 0.03s          
      serialize | 0.08s           | 0.04s           | 0.23s          
    unserialize | 0.05s           | 1.36s           | 0.18s          
     total time | 1.84s           | 3.96s           | 0.60s          
   memory usage | 29MB            | 19MB            | 113MB        
```

This second test stores an array inside the data structure 1048577 times 
as follows:

```php
$list[] = ["name" => "hello world" . $i];
```

### Results

```
===============================
Array store test:
===============================
        Measure | StaticArray     | DynamicArray    | Native         
    add 1048577 | 0.79s           | 0.80s           | 0.40s          
  get two items | 0.00s           | 0.00s           | 0.00s          
       loop all | 1.86s           | 6.16s           | 0.11s          
      serialize | 0.14s           | 0.13s           | 0.34s          
    unserialize | 0.10s           | 3.45s           | 0.82s          
     total time | 2.89s           | 10.54s          | 1.66s          
   memory usage | 57MB            | 46MB            | 489MB              
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