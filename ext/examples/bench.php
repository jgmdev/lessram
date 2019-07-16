<?php

echo "=======================================================================\n";
echo "String store test with native C extension:\n";
echo "=======================================================================\n";
test_storeStrings();

echo "\n\n";

echo "=======================================================================\n";
echo "Array store test with native C extension:\n";
echo "=======================================================================\n";
test_storeArrays();

echo "\n";

function test_storeStrings($amount=1024*1024)
{
    // Measure DynamicArray
    $dynamic_total_time = microtime(true);
    $list = new LessRam\DynamicArray();

    // adding items
    $dynamic_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->appendData("hello world" . $i);
    }
    $dynamic_add_time = microtime(true) - $dynamic_add_time;

    // loop all data
    $dynamic_loop_time = microtime(true);
    for($i=0; $i<$amount; $i++)
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
    for($i=0; $i<$amount; $i++)
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


    // Mesure Ds
    if(class_exists("\\Ds\\Vector"))
    {
        // Vector
        $dsvector_total_time = microtime(true);
        $list = new \Ds\Vector();

        // adding items
        $dsvector_add_time = microtime(true);
        for($i=0; $i<$amount; $i++)
        {
            $list->push("hello world" . $i);
        }
        $dsvector_add_time = microtime(true) - $dsvector_add_time;

        // loop all data
        $dsvector_loop_time = microtime(true);
        foreach($list as $value){}
        $dsvector_loop_time = microtime(true) - $dsvector_loop_time;

        $dsvector_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

        $dsvector_total_time = microtime(true) - $dsvector_total_time;


        // Map
        $dsmap_total_time = microtime(true);
        $list = new \Ds\Map();

        // adding items
        $dsmap_add_time = microtime(true);
        for($i=0; $i<$amount; $i++)
        {
            $list->put($i, "hello world" . $i);
        }
        $dsmap_add_time = microtime(true) - $dsmap_add_time;

        // loop all data
        $dsmap_loop_time = microtime(true);
        foreach($list as $value){}
        $dsmap_loop_time = microtime(true) - $dsmap_loop_time;

        $dsmap_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

        $dsmap_total_time = microtime(true) - $dsmap_total_time;
    }


    // Measure native
    $native_total_time = microtime(true);
    $list = [];

    // adding items
    $native_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
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

    $format = "%15.15s | %12.12s | %8.8s | %10.10s | %13.13s\n";

    printf(
        $format,
        'Structures',
        'add '.$amount,
        'loop all',
        'total time',
        'memory usage',
    );

    printf(
        $format,
        'DynamicArray',
        number_format($dynamic_add_time, 2) . "s",
        number_format($dynamic_loop_time, 2) . "s",
        number_format($dynamic_total_time, 2) . "s",
        $dynamic_memory_usage
    );

    printf(
        $format,
        'SplFixedArray',
        number_format($splfixed_add_time, 2) . "s",
        number_format($splfixed_loop_time, 2) . "s",
        number_format($splfixed_total_time, 2) . "s",
        $splfixed_memory_usage
    );

    if(class_exists("\\Ds\\Vector"))
    {
        printf(
            $format,
            'Ds\Vector',
            number_format($dsvector_add_time, 2) . "s",
            number_format($dsvector_loop_time, 2) . "s",
            number_format($dsvector_total_time, 2) . "s",
            $dsvector_memory_usage
        );

        printf(
            $format,
            'Ds\Map',
            number_format($dsmap_add_time, 2) . "s",
            number_format($dsmap_loop_time, 2) . "s",
            number_format($dsmap_total_time, 2) . "s",
            $dsmap_memory_usage
        );
    }

    printf(
        $format,
        'Native Array',
        number_format($native_add_time, 2) . "s",
        number_format($native_loop_time, 2) . "s",
        number_format($native_total_time, 2) . "s",
        $native_memory_usage
    );
}

function test_storeArrays($amount=1024*1024)
{
    // Mesure DynamicArray
    $dynamic_total_time = microtime(true);
    $list = new LessRam\DynamicArray();

    // adding items
    $dynamic_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->appendData(["name" => "hello world" . $i]);
    }
    $dynamic_add_time = microtime(true) - $dynamic_add_time;

    // loop all data
    $dynamic_loop_time = microtime(true);
    for($i=0; $i<$amount; $i++)
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
    for($i=0; $i<$amount; $i++)
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


    // Mesure Ds
    if(class_exists("\\Ds\\Vector"))
    {
        // Vector
        $dsvector_total_time = microtime(true);
        $list = new \Ds\Vector();

        // adding items
        $dsvector_add_time = microtime(true);
        for($i=0; $i<$amount; $i++)
        {
            $list->push(["name" => "hello world" . $i]);
        }
        $dsvector_add_time = microtime(true) - $dsvector_add_time;

        // loop all data
        $dsvector_loop_time = microtime(true);
        foreach($list as $value){}
        $dsvector_loop_time = microtime(true) - $dsvector_loop_time;

        $dsvector_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

        $dsvector_total_time = microtime(true) - $dsvector_total_time;


        // Map
        $dsmap_total_time = microtime(true);
        $list = new \Ds\Map();

        // adding items
        $dsmap_add_time = microtime(true);
        for($i=0; $i<$amount; $i++)
        {
            $list->put($i, ["name" => "hello world" . $i]);
        }
        $dsmap_add_time = microtime(true) - $dsmap_add_time;

        // loop all data
        $dsmap_loop_time = microtime(true);
        foreach($list as $value){}
        $dsmap_loop_time = microtime(true) - $dsmap_loop_time;

        $dsmap_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

        $dsmap_total_time = microtime(true) - $dsmap_total_time;
    }


    // Mesure native
    $native_total_time = microtime(true);
    $list = [];

    // adding items
    $native_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
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

    $format = "%15.15s | %12.12s | %8.8s | %10.10s | %13.13s\n";

    printf(
        $format,
        'Structures',
        'add '.$amount,
        'loop all',
        'total time',
        'memory usage',
    );

    printf(
        $format,
        'DynamicArray',
        number_format($dynamic_add_time, 2) . "s",
        number_format($dynamic_loop_time, 2) . "s",
        number_format($dynamic_total_time, 2) . "s",
        $dynamic_memory_usage
    );

    printf(
        $format,
        'SplFixedArray',
        number_format($splfixed_add_time, 2) . "s",
        number_format($splfixed_loop_time, 2) . "s",
        number_format($splfixed_total_time, 2) . "s",
        $splfixed_memory_usage
    );

    if(class_exists("\\Ds\\Vector"))
    {
        printf(
            $format,
            'Ds\Vector',
            number_format($dsvector_add_time, 2) . "s",
            number_format($dsvector_loop_time, 2) . "s",
            number_format($dsvector_total_time, 2) . "s",
            $dsvector_memory_usage
        );

        printf(
            $format,
            'Ds\Map',
            number_format($dsmap_add_time, 2) . "s",
            number_format($dsmap_loop_time, 2) . "s",
            number_format($dsmap_total_time, 2) . "s",
            $dsmap_memory_usage
        );
    }

    printf(
        $format,
        'Native Array',
        number_format($native_add_time, 2) . "s",
        number_format($native_loop_time, 2) . "s",
        number_format($native_total_time, 2) . "s",
        $native_memory_usage
    );
}