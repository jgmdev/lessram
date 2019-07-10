<?php
/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
 */

require "lib/Autoloader.php";

echo "=======================================================================\n";
echo "String store test:\n";
echo "=======================================================================\n";
test_storeStrings();

echo "\n\n";

echo "=======================================================================\n";
echo "String store test gzipped (StaticArray and DynamicArray not compressed):\n";
echo "=======================================================================\n";
test_storeStrings(1048577, true);

echo "\n\n";

echo "=======================================================================\n";
echo "Array store test:\n";
echo "=======================================================================\n";
test_storeArrays();

echo "\n\n";

echo "=======================================================================\n";
echo "Array store test gzipped (StaticArray and DynamicArray not compressed):\n";
echo "=======================================================================\n";
test_storeArrays(1048577, true);

echo "\n";

function test_storeStrings(
    $amount=1048577, 
    $compression=false, 
    $compression_level=-1,
    $compress_lessram = false
)
{
    // Measure StaticArray
    $static_total_time = microtime(true);
    $list = new LessRam\StaticArray();

    // adding items
    $static_add_time = microtime(true);
    if($compression && $compress_lessram)
    {
        for($i=0; $i<=$amount; $i++)
        {
            $list->append(gzdeflate("hello world" . $i, $compression_level));
        }
    }
    else
    {
        for($i=0; $i<=$amount; $i++)
        {
            $list->append("hello world" . $i);
        }
    }
    $static_add_time = microtime(true) - $static_add_time;

    // get items
    $static_get_time = microtime(true);
    if($compression && $compress_lessram)
    {
        gzinflate($list[10]);
        gzinflate($list[$amount]);
    }
    else
    {
        $list[10];
        $list[$amount];
    }
    $static_get_time = microtime(true) - $static_get_time;

    // loop all data
    $static_loop_time = microtime(true);
    foreach($list->getAll() as $value){}
    $static_loop_time = microtime(true) - $static_loop_time;

    // serialize/unserialize
    $static_serialize_time = microtime(true);
    $data = serialize($list);
    $static_serialize_time = microtime(true) - $static_serialize_time;

    $static_unserialize_time = microtime(true);
    unserialize($data);
    unset($data);
    $static_unserialize_time = microtime(true) - $static_unserialize_time;

    $static_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $static_total_time = microtime(true) - $static_total_time;
    

    // Measure DynamicArray
    $dynamic_total_time = microtime(true);
    $list = new LessRam\DynamicArray();

    // adding items
    $dynamic_add_time = microtime(true);
    if($compression && $compress_lessram)
    {
        for($i=0; $i<=$amount; $i++)
        {
            $list->append(gzdeflate("hello world" . $i, $compression_level));
        }
    }
    else
    {
        for($i=0; $i<=$amount; $i++)
        {
            $list->append("hello world" . $i);
        }
    }
    $dynamic_add_time = microtime(true) - $dynamic_add_time;

    // get items
    $dynamic_get_time = microtime(true);
    if($compression && $compress_lessram)
    {
        gzinflate($list[10]);
        gzinflate($list[$amount]);
    }
    else
    {
        $list[10];
        $list[$amount];
    }
    $dynamic_get_time = microtime(true) - $dynamic_get_time;

    // loop all data
    $dynamic_loop_time = microtime(true);
    foreach($list->getAll() as $value){}
    $dynamic_loop_time = microtime(true) - $dynamic_loop_time;

    // serialize/unserialize
    $dynamic_serialize_time = microtime(true);
    $data = serialize($list);
    $dynamic_serialize_time = microtime(true) - $dynamic_serialize_time;

    $dynamic_unserialize_time = microtime(true);
    unserialize($data);
    unset($data);
    $dynamic_unserialize_time = microtime(true) - $dynamic_unserialize_time;

    $dynamic_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $dynamic_total_time = microtime(true) - $dynamic_total_time;


    // Mesure SplFixedArray
    $splfixed_total_time = microtime(true);
    $list = new SplFixedArray($amount+1);

    // adding items
    $splfixed_add_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list[$i] = $compression ? 
            gzdeflate("hello world" . $i, $compression_level)
            :
            "hello world" . $i
        ;
    }
    $splfixed_add_time = microtime(true) - $splfixed_add_time;

    // get items
    $splfixed_get_time = microtime(true);
    if($compression)
    {
        gzinflate($list[10]);
        gzinflate($list[$amount]);
    }
    else
    {
        $list[10];
        $list[$amount];
    }
    $splfixed_get_time = microtime(true) - $splfixed_get_time;

    // loop all data
    $splfixed_loop_time = microtime(true);
    foreach($list as $value){}
    $splfixed_loop_time = microtime(true) - $splfixed_loop_time;

    // serialize/unserialize
    $splfixed_serialize_time = microtime(true);
    $data = serialize($list);
    $splfixed_serialize_time = microtime(true) - $splfixed_serialize_time;

    $splfixed_unserialize_time = microtime(true);
    unserialize($data);
    unset($data);
    $splfixed_unserialize_time = microtime(true) - $splfixed_unserialize_time;

    $splfixed_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $splfixed_total_time = microtime(true) - $splfixed_total_time;


    // Measure native
    $native_total_time = microtime(true);
    $list = [];

    // adding items
    $native_add_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list[] = $compression ? 
            gzdeflate("hello world" . $i, $compression_level)
            :
            "hello world" . $i
        ;
    }
    $native_add_time = microtime(true) - $native_add_time;

    // get items
    $native_get_time = microtime(true);
    if($compression)
    {
        gzinflate($list[10]);
        gzinflate($list[$amount]);
    }
    else
    {
        $list[10];
        $list[$amount];
    }
    $native_get_time = microtime(true) - $native_get_time;

    // loop all data
    $native_loop_time = microtime(true);
    foreach($list as $value){}
    $native_loop_time = microtime(true) - $native_loop_time;

    // serialize/unserialize
    $native_serialize_time = microtime(true);
    $data = serialize($list);
    $native_serialize_time = microtime(true) - $native_serialize_time;

    $native_unserialize_time = microtime(true);
    unserialize($data);
    unset($data);
    $native_unserialize_time = microtime(true) - $native_unserialize_time;

    $native_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $native_total_time = microtime(true) - $native_total_time;

    $format = "%15.15s | %-15.15s | %-15.15s | %-15.15s | %-15.15s\n";
    
    printf(
        $format, 
        'Measure', 
        'StaticArray',  
        "DynamicArray",
        'SplFixedArray', 
        "Native"
    );
    
    printf(
        $format, 
        'add '.$amount, 
        number_format($static_add_time, 2) . "s", 
        number_format($dynamic_add_time, 2) . "s",
        number_format($splfixed_add_time, 2) . "s",  
        number_format($native_add_time, 2) . "s"
    );
    
    printf(
        $format, 
        'get two items', 
        number_format($static_get_time, 2) . "s", 
        number_format($dynamic_get_time, 2) . "s",
        number_format($splfixed_get_time, 2) . "s",  
        number_format($native_get_time, 2) . "s"
    );
    
    printf(
        $format, 
        'loop all', 
        number_format($static_loop_time, 2) . "s", 
        number_format($dynamic_loop_time, 2) . "s",
        number_format($splfixed_loop_time, 2) . "s",  
        number_format($native_loop_time, 2) . "s"
    );

    printf(
        $format, 
        'serialize', 
        number_format($static_serialize_time, 2) . "s", 
        number_format($dynamic_serialize_time, 2) . "s",
        number_format($splfixed_serialize_time, 2) . "s",  
        number_format($native_serialize_time, 2) . "s"
    );

    printf(
        $format, 
        'unserialize', 
        number_format($static_unserialize_time, 2) . "s", 
        number_format($dynamic_unserialize_time, 2) . "s",
        number_format($splfixed_unserialize_time, 2) . "s",  
        number_format($native_unserialize_time, 2) . "s"
    );
    
    printf(
        $format, 
        'total time', 
        number_format($static_total_time, 2) . "s", 
        number_format($dynamic_total_time, 2) . "s",
        number_format($splfixed_total_time, 2) . "s",  
        number_format($native_total_time, 2) . "s"
    );

    printf(
        $format, 
        'memory usage', 
        $static_memory_usage,
        $dynamic_memory_usage,
        $splfixed_memory_usage,
        $native_memory_usage
    );
}

function test_storeArrays(
    $amount=1048577,
    $compression=false, 
    $compression_level=-1,
    $compress_lessram = false
)
{
    // Mesure StaticArray
    $static_total_time = microtime(true);
    $list = new LessRam\StaticArray();

    // adding items
    $static_add_time = microtime(true);
    if($compression && $compress_lessram)
    {
        for($i=0; $i<=$amount; $i++)
        {
            $list->append(gzdeflate(
                serialize(["name" => "hello world" . $i]), $compression_level
            ));
        }
    }
    else
    {
        for($i=0; $i<=$amount; $i++)
        {
            $list->append(["name" => "hello world" . $i]);
        }
    }
    $static_add_time = microtime(true) - $static_add_time;

    // get items
    $static_get_time = microtime(true);
    if($compression && $compress_lessram)
    {
        gzinflate($list[10]);
        gzinflate($list[$amount]);
    }
    else
    {
        $list[10];
        $list[$amount];
    }
    $static_get_time = microtime(true) - $static_get_time;

    // loop all data
    $static_loop_time = microtime(true);
    foreach($list->getAll() as $value){}
    $static_loop_time = microtime(true) - $static_loop_time;

    // serialize/unserialize
    $static_serialize_time = microtime(true);
    $data = serialize($list);
    $static_serialize_time = microtime(true) - $static_serialize_time;

    $static_unserialize_time = microtime(true);
    unserialize($data);
    unset($data);
    $static_unserialize_time = microtime(true) - $static_unserialize_time;

    $static_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $static_total_time = microtime(true) - $static_total_time;
    

    // Mesure DynamicArray
    $dynamic_total_time = microtime(true);
    $list = new LessRam\DynamicArray();

    // adding items
    $dynamic_add_time = microtime(true);
    if($compression && $compress_lessram)
    {
        for($i=0; $i<=$amount; $i++)
        {
            $list->append(gzdeflate(
                serialize(["name" => "hello world" . $i]), $compression_level
            ));
        }
    }
    else
    {
        for($i=0; $i<=$amount; $i++)
        {
            $list->append(["name" => "hello world" . $i]);
        }
    }
    $dynamic_add_time = microtime(true) - $dynamic_add_time;

    // get items
    $dynamic_get_time = microtime(true);
    if($compression && $compress_lessram)
    {
        gzinflate($list[10]);
        gzinflate($list[$amount]);
    }
    else
    {
        $list[10];
        $list[$amount];
    }
    $dynamic_get_time = microtime(true) - $dynamic_get_time;

    // loop all data
    $dynamic_loop_time = microtime(true);
    foreach($list->getAll() as $value){}
    $dynamic_loop_time = microtime(true) - $dynamic_loop_time;

    // serialize/unserialize
    $dynamic_serialize_time = microtime(true);
    $data = serialize($list);
    $dynamic_serialize_time = microtime(true) - $dynamic_serialize_time;

    $dynamic_unserialize_time = microtime(true);
    unserialize($data);
    unset($data);
    $dynamic_unserialize_time = microtime(true) - $dynamic_unserialize_time;

    $dynamic_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $dynamic_total_time = microtime(true) - $dynamic_total_time;


    // Mesure SplFixedArray
    $splfixed_total_time = microtime(true);
    $list = new SplFixedArray($amount+1);

    // adding items
    $splfixed_add_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list[$i] = $compression ? 
            gzdeflate(
                serialize(["name" => "hello world" . $i]), $compression_level
            )
            :
            ["name" => "hello world" . $i]
        ;
    }
    $splfixed_add_time = microtime(true) - $splfixed_add_time;

    // get items
    $splfixed_get_time = microtime(true);
    if($compression)
    {
        gzinflate($list[10]);
        gzinflate($list[$amount]);
    }
    else
    {
        $list[10];
        $list[$amount];
    }
    $splfixed_get_time = microtime(true) - $splfixed_get_time;

    // loop all data
    $splfixed_loop_time = microtime(true);
    foreach($list as $value){}
    $splfixed_loop_time = microtime(true) - $splfixed_loop_time;

    // serialize/unserialize
    $splfixed_serialize_time = microtime(true);
    $data = serialize($list);
    $splfixed_serialize_time = microtime(true) - $splfixed_serialize_time;

    $splfixed_unserialize_time = microtime(true);
    unserialize($data);
    unset($data);
    $splfixed_unserialize_time = microtime(true) - $splfixed_unserialize_time;

    $splfixed_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $splfixed_total_time = microtime(true) - $splfixed_total_time;


    // Mesure native
    $native_total_time = microtime(true);
    $list = [];

    // adding items
    $native_add_time = microtime(true);
    for($i=0; $i<=$amount; $i++)
    {
        $list[] = $compression ? 
            gzdeflate(
                serialize(["name" => "hello world" . $i]), $compression_level
            )
            :
            ["name" => "hello world" . $i]
        ;
    }
    $native_add_time = microtime(true) - $native_add_time;

    // get items
    $native_get_time = microtime(true);
    if($compression)
    {
        gzinflate($list[10]);
        gzinflate($list[$amount]);
    }
    else
    {
        $list[10];
        $list[$amount];
    }
    $native_get_time = microtime(true) - $native_get_time;

    // loop all data
    $native_loop_time = microtime(true);
    foreach($list as $value){}
    $native_loop_time = microtime(true) - $native_loop_time;

    // serialize/unserialize
    $native_serialize_time = microtime(true);
    $data = serialize($list);
    $native_serialize_time = microtime(true) - $native_serialize_time;

    $native_unserialize_time = microtime(true);
    unserialize($data);
    unset($data);
    $native_unserialize_time = microtime(true) - $native_unserialize_time;

    $native_memory_usage = ceil(memory_get_usage(false) / 1024 / 1024) . "MB";

    $native_total_time = microtime(true) - $native_total_time;

    $format = "%15.15s | %-15.15s | %-15.15s | %-15.15s | %-15.15s\n";
    
    printf(
        $format, 
        'Measure', 
        'StaticArray', 
        'DynamicArray',
        'SplFixedArray',  
        "Native"
    );
    
    printf(
        $format, 
        'add '.$amount, 
        number_format($static_add_time, 2) . "s", 
        number_format($dynamic_add_time, 2) . "s", 
        number_format($splfixed_add_time, 2) . "s", 
        number_format($native_add_time, 2) . "s"
    );
    
    printf(
        $format, 
        'get two items', 
        number_format($static_get_time, 2) . "s", 
        number_format($dynamic_get_time, 2) . "s", 
        number_format($splfixed_get_time, 2) . "s", 
        number_format($native_get_time, 2) . "s"
    );
    
    printf(
        $format, 
        'loop all', 
        number_format($static_loop_time, 2) . "s",  
        number_format($dynamic_loop_time, 2) . "s", 
        number_format($splfixed_loop_time, 2) . "s",
        number_format($native_loop_time, 2) . "s"
    );

    printf(
        $format, 
        'serialize', 
        number_format($static_serialize_time, 2) . "s", 
        number_format($dynamic_serialize_time, 2) . "s",
        number_format($splfixed_serialize_time, 2) . "s",  
        number_format($native_serialize_time, 2) . "s"
    );

    printf(
        $format, 
        'unserialize', 
        number_format($static_unserialize_time, 2) . "s", 
        number_format($dynamic_unserialize_time, 2) . "s", 
        number_format($splfixed_unserialize_time, 2) . "s", 
        number_format($native_unserialize_time, 2) . "s"
    );
    
    printf(
        $format, 
        'total time', 
        number_format($static_total_time, 2) . "s", 
        number_format($dynamic_total_time, 2) . "s",
        number_format($splfixed_total_time, 2) . "s",  
        number_format($native_total_time, 2) . "s"
    );

    printf(
        $format, 
        'memory usage', 
        $static_memory_usage,
        $dynamic_memory_usage,
        $splfixed_memory_usage,
        $native_memory_usage
    );
}