<?php

echo "=======================================================================\n";
echo "String store test with native C extension:\n";
echo "=======================================================================\n";
test_storeStrings();

echo "\n\n";

echo "=======================================================================\n";
echo "Array store test with natice C extension:\n";
echo "=======================================================================\n";
test_storeArrays();

echo "\n";

function test_storeStrings($amount=1048577)
{
    // Measure DynamicArray
    $dynamic_total_time = microtime(true);
    $list = new LessRam\DynamicArray();

    // adding items
    $dynamic_add_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list->appendData("hello world" . $i);
    }
    $dynamic_add_time = microtime(true) - $dynamic_add_time;

    // loop all data
    $dynamic_loop_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list->next();
    }
    $dynamic_loop_time = microtime(true) - $dynamic_loop_time;

    $dynamic_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $dynamic_total_time = microtime(true) - $dynamic_total_time;


    // Mesure SplFixedArray
    $splfixed_total_time = microtime(true);
    $list = new SplFixedArray($amount+1);

    // adding items
    $splfixed_add_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list[$i] = "hello world" . $i;
    }
    $splfixed_add_time = microtime(true) - $splfixed_add_time;

    // loop all data
    $splfixed_loop_time = microtime(true);
    foreach($list as $value){}
    $splfixed_loop_time = microtime(true) - $splfixed_loop_time;

    $splfixed_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $splfixed_total_time = microtime(true) - $splfixed_total_time;


    // Measure native
    $native_total_time = microtime(true);
    $list = [];

    // adding items
    $native_add_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list[] = "hello world" . $i;
    }
    $native_add_time = microtime(true) - $native_add_time;

    // loop all data
    $native_loop_time = microtime(true);
    foreach($list as $value){}
    $native_loop_time = microtime(true) - $native_loop_time;

    $native_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $native_total_time = microtime(true) - $native_total_time;

    $format = "%15.15s | %-15.15s | %-15.15s | %-15.15s\n";
    
    printf(
        $format, 
        'Measure', 
        "DynamicArray",
        'SplFixedArray', 
        "Native"
    );
    
    printf(
        $format, 
        'add '.$amount, 
        number_format($dynamic_add_time, 2) . "s",
        number_format($splfixed_add_time, 2) . "s",  
        number_format($native_add_time, 2) . "s"
    );
    
    printf(
        $format, 
        'loop all', 
        number_format($dynamic_loop_time, 2) . "s",
        number_format($splfixed_loop_time, 2) . "s",  
        number_format($native_loop_time, 2) . "s"
    );
    
    printf(
        $format, 
        'total time', 
        number_format($dynamic_total_time, 2) . "s",
        number_format($splfixed_total_time, 2) . "s",  
        number_format($native_total_time, 2) . "s"
    );

    printf(
        $format, 
        'memory usage',
        $dynamic_memory_usage,
        $splfixed_memory_usage,
        $native_memory_usage
    );
}

function test_storeArrays($amount=1048577)
{
    // Mesure DynamicArray
    $dynamic_total_time = microtime(true);
    $list = new LessRam\DynamicArray();

    // adding items
    $dynamic_add_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list->appendData(["name" => "hello world" . $i]);
    }
    $dynamic_add_time = microtime(true) - $dynamic_add_time;

    // loop all data
    $dynamic_loop_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list->next();
    }
    $dynamic_loop_time = microtime(true) - $dynamic_loop_time;

    $dynamic_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $dynamic_total_time = microtime(true) - $dynamic_total_time;


    // Mesure SplFixedArray
    $splfixed_total_time = microtime(true);
    $list = new SplFixedArray($amount+1);

    // adding items
    $splfixed_add_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list[$i] = ["name" => "hello world" . $i];
    }
    $splfixed_add_time = microtime(true) - $splfixed_add_time;

    // loop all data
    $splfixed_loop_time = microtime(true);
    foreach($list as $value){}
    $splfixed_loop_time = microtime(true) - $splfixed_loop_time;

    $splfixed_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $splfixed_total_time = microtime(true) - $splfixed_total_time;


    // Mesure native
    $native_total_time = microtime(true);
    $list = [];

    // adding items
    $native_add_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list[] = ["name" => "hello world" . $i];
    }
    $native_add_time = microtime(true) - $native_add_time;

    // loop all data
    $native_loop_time = microtime(true);
    foreach($list as $value){}
    $native_loop_time = microtime(true) - $native_loop_time;

    $native_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $native_total_time = microtime(true) - $native_total_time;

    $format = "%15.15s | %-15.15s | %-15.15s | %-15.15s\n";
    
    printf(
        $format, 
        'Measure', 
        'DynamicArray',
        'SplFixedArray',  
        "Native"
    );
    
    printf(
        $format, 
        'add '.$amount, 
        number_format($dynamic_add_time, 2) . "s", 
        number_format($splfixed_add_time, 2) . "s", 
        number_format($native_add_time, 2) . "s"
    );
    
    printf(
        $format, 
        'loop all', 
        number_format($dynamic_loop_time, 2) . "s", 
        number_format($splfixed_loop_time, 2) . "s",
        number_format($native_loop_time, 2) . "s"
    );
    
    printf(
        $format, 
        'total time', 
        number_format($dynamic_total_time, 2) . "s",
        number_format($splfixed_total_time, 2) . "s",  
        number_format($native_total_time, 2) . "s"
    );

    printf(
        $format, 
        'memory usage', 
        $dynamic_memory_usage,
        $splfixed_memory_usage,
        $native_memory_usage
    );
}