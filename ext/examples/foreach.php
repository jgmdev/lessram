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

unset($list[9]);
unset($list[8]);
unset($list[8]);
foreach($list as $value)
{
    var_dump($value);
}

echo "Add time: " . $time_add_end . "\n";
echo "Loop time: " . $time_loop_end . "\n";
echo "Edit time: " . $time_edit_end . "\n";