# LessRam

PHP implementations of array data structures that consume less memory
than native php arrays with the drawback of been less performant:

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

To run your own benchmarks run:

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