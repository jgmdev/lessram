<?php

use LessRam\Storage;

$list = new Storage(Storage::LOWEST);

$time_add = microtime(true);
for($i=0; $i<10; $i++)
{
    $list[] = ["hello ~ $i"];
}
$time_add_end = (microtime(true) - $time_add);

foreach($list as $value)
{
    print_r($value);
}

$time_loop = microtime(true);
foreach($list as $value){}
$time_loop_end = (microtime(true) - $time_loop);

$time_edit = microtime(true);
for($i=0; $i<10; $i++)
{
    $list[$i] = ["hola ~ $i"];
}
$time_edit_end = (microtime(true) - $time_edit);

// This kind of stuff isn't supported (we don't store zvals)
$list[4][0] .= "something";

unset($list[9]);
unset($list[8]);
foreach($list as $index => $value)
{
    print "$index => {$value[0]}\n";
}

echo "Add time: " . $time_add_end . "\n";
echo "Loop time: " . $time_loop_end . "\n";
echo "Edit time: " . $time_edit_end . "\n";

// Generate exception
print $list[20];