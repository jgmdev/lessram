<?php

echo "=======================================================================\n";
echo "String store test with native C extension:\n";
echo "=======================================================================\n";
test_storeStrings((1024*1024)*1, false);

echo "\n\n";

echo "=======================================================================\n";
echo "Array store test with native C extension:\n";
echo "=======================================================================\n";
test_storeArrays((1024*1024)*1, false);

echo "\n\n";

echo "=======================================================================\n";
echo "Integers store test with native C extension:\n";
echo "=======================================================================\n";
test_storeIntegers((1024*1024)*1, false);

echo "\n";

function test_storeStrings($amount=1024*1024, $real=false)
{
    // Measure Highest
    $dynamic_total_time = microtime(true);
    $list = new LessRam\Storage(LessRam\Storage::HIGHEST);

    // adding items
    $dynamic_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->append("hello world" . $i);
    }
    $dynamic_add_time = microtime(true) - $dynamic_add_time;

    // loop all data
    $dynamic_loop_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->getNext();
    }
    $dynamic_loop_time = microtime(true) - $dynamic_loop_time;

    $dynamic_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $dynamic_total_time = microtime(true) - $dynamic_total_time;


    // Measure Moderate
    $dynamic_m_total_time = microtime(true);
    $list = new LessRam\Storage(LessRam\Storage::MODERATE);

    // adding items
    $dynamic_m_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->append("hello world" . $i);
    }
    $dynamic_m_add_time = microtime(true) - $dynamic_m_add_time;

    // loop all data
    $dynamic_m_loop_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->getNext();
    }
    $dynamic_m_loop_time = microtime(true) - $dynamic_m_loop_time;

    $dynamic_m_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $dynamic_m_total_time = microtime(true) - $dynamic_m_total_time;


    // Measure Lowest
    $dynamic_l_total_time = microtime(true);
    $list = new LessRam\Storage(LessRam\Storage::LOWEST);

    // adding items
    $dynamic_l_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->append("hello world" . $i);
    }
    $dynamic_l_add_time = microtime(true) - $dynamic_l_add_time;

    // loop all data
    $dynamic_l_loop_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->getNext();
    }
    $dynamic_l_loop_time = microtime(true) - $dynamic_l_loop_time;

    $dynamic_l_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $dynamic_l_total_time = microtime(true) - $dynamic_l_total_time;


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

    $splfixed_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $splfixed_total_time = microtime(true) - $splfixed_total_time;

    // Mesure SplDoublyLinkedList
    $spldllist_total_time = microtime(true);
    $list = new SplDoublyLinkedList($amount+1);

    // adding items
    $spldllist_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list[] = "hello world" . $i;
    }
    $spldllist_add_time = microtime(true) - $spldllist_add_time;

    // loop all data
    $spldllist_loop_time = microtime(true);
    foreach($list as $value){}
    $spldllist_loop_time = microtime(true) - $spldllist_loop_time;

    $spldllist_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $spldllist_total_time = microtime(true) - $spldllist_total_time;


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

        $dsvector_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

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

        $dsmap_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

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

    $native_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $native_total_time = microtime(true) - $native_total_time;

    $format = "%16.16s | %12.12s | %8.8s | %10.10s | %13.13s\n";

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
        'LessRam Highest',
        number_format($dynamic_add_time, 2) . "s",
        number_format($dynamic_loop_time, 2) . "s",
        number_format($dynamic_total_time, 2) . "s",
        $dynamic_memory_usage
    );

    printf(
        $format,
        'LessRam Moderate',
        number_format($dynamic_m_add_time, 2) . "s",
        number_format($dynamic_m_loop_time, 2) . "s",
        number_format($dynamic_m_total_time, 2) . "s",
        $dynamic_m_memory_usage
    );

    printf(
        $format,
        'LessRam Lowest',
        number_format($dynamic_l_add_time, 2) . "s",
        number_format($dynamic_l_loop_time, 2) . "s",
        number_format($dynamic_l_total_time, 2) . "s",
        $dynamic_l_memory_usage
    );

    printf(
        $format,
        'SplFixedArray',
        number_format($splfixed_add_time, 2) . "s",
        number_format($splfixed_loop_time, 2) . "s",
        number_format($splfixed_total_time, 2) . "s",
        $splfixed_memory_usage
    );

    printf(
        $format,
        'SplDoublyLinkedList',
        number_format($spldllist_add_time, 2) . "s",
        number_format($spldllist_loop_time, 2) . "s",
        number_format($spldllist_total_time, 2) . "s",
        $spldllist_memory_usage
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
        'PHP Array',
        number_format($native_add_time, 2) . "s",
        number_format($native_loop_time, 2) . "s",
        number_format($native_total_time, 2) . "s",
        $native_memory_usage
    );
}

function test_storeArrays($amount=1024*1024, $real=false)
{
    // Mesure Highest
    $dynamic_total_time = microtime(true);
    $list = new LessRam\Storage(LessRam\Storage::HIGHEST);

    // adding items
    $dynamic_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->append(["name" => "hello world" . $i]);
    }
    $dynamic_add_time = microtime(true) - $dynamic_add_time;

    // loop all data
    $dynamic_loop_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->getNext();
    }
    $dynamic_loop_time = microtime(true) - $dynamic_loop_time;

    $dynamic_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $dynamic_total_time = microtime(true) - $dynamic_total_time;


    // Measure Moderate
    $dynamic_m_total_time = microtime(true);
    $list = new LessRam\Storage(LessRam\Storage::MODERATE);

    // adding items
    $dynamic_m_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->append(["name" => "hello world" . $i]);
    }
    $dynamic_m_add_time = microtime(true) - $dynamic_m_add_time;

    // loop all data
    $dynamic_m_loop_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->getNext();
    }
    $dynamic_m_loop_time = microtime(true) - $dynamic_m_loop_time;

    $dynamic_m_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $dynamic_m_total_time = microtime(true) - $dynamic_m_total_time;


    // Measure Lowest
    $dynamic_l_total_time = microtime(true);
    $list = new LessRam\Storage(LessRam\Storage::LOWEST);

    // adding items
    $dynamic_l_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->append(["name" => "hello world" . $i]);
    }
    $dynamic_l_add_time = microtime(true) - $dynamic_l_add_time;

    // loop all data
    $dynamic_l_loop_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->getNext();
    }
    $dynamic_l_loop_time = microtime(true) - $dynamic_l_loop_time;

    $dynamic_l_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $dynamic_l_total_time = microtime(true) - $dynamic_l_total_time;


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

    $splfixed_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $splfixed_total_time = microtime(true) - $splfixed_total_time;


    // Mesure SplDoublyLinkedList
    $spldllist_total_time = microtime(true);
    $list = new SplDoublyLinkedList($amount+1);

    // adding items
    $spldllist_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list[] = ["name" => "hello world" . $i];
    }
    $spldllist_add_time = microtime(true) - $spldllist_add_time;

    // loop all data
    $spldllist_loop_time = microtime(true);
    foreach($list as $value){}
    $spldllist_loop_time = microtime(true) - $spldllist_loop_time;

    $spldllist_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $spldllist_total_time = microtime(true) - $spldllist_total_time;


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

        $dsvector_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

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

        $dsmap_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

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

    $native_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $native_total_time = microtime(true) - $native_total_time;

    $format = "%16.16s | %12.12s | %8.8s | %10.10s | %13.13s\n";


    // Mesure native storing as json
    $native_json_total_time = microtime(true);
    $list = [];

    // adding items
    $native_json_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list[] = json_encode(["name" => "hello world" . $i]);
    }
    $native_json_add_time = microtime(true) - $native_json_add_time;

    // loop all data
    $native_json_loop_time = microtime(true);
    foreach($list as $value){ json_decode($value); }
    $native_json_loop_time = microtime(true) - $native_json_loop_time;

    $native_json_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $native_json_total_time = microtime(true) - $native_json_total_time;


    // Mesure native storing as serialized
    $native_serial_total_time = microtime(true);
    $list = [];

    // adding items
    $native_serial_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list[] = serialize(["name" => "hello world" . $i]);
    }
    $native_serial_add_time = microtime(true) - $native_serial_add_time;

    // loop all data
    $native_serial_loop_time = microtime(true);
    foreach($list as $value){ unserialize($value); }
    $native_serial_loop_time = microtime(true) - $native_serial_loop_time;

    $native_serial_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $native_serial_total_time = microtime(true) - $native_serial_total_time;

    $format = "%16.16s | %12.12s | %8.8s | %10.10s | %13.13s\n";

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
        'LessRam Highest',
        number_format($dynamic_add_time, 2) . "s",
        number_format($dynamic_loop_time, 2) . "s",
        number_format($dynamic_total_time, 2) . "s",
        $dynamic_memory_usage
    );

    printf(
        $format,
        'LessRam Moderate',
        number_format($dynamic_m_add_time, 2) . "s",
        number_format($dynamic_m_loop_time, 2) . "s",
        number_format($dynamic_m_total_time, 2) . "s",
        $dynamic_m_memory_usage
    );

    printf(
        $format,
        'LessRam Lowest',
        number_format($dynamic_l_add_time, 2) . "s",
        number_format($dynamic_l_loop_time, 2) . "s",
        number_format($dynamic_l_total_time, 2) . "s",
        $dynamic_l_memory_usage
    );

    printf(
        $format,
        'SplFixedArray',
        number_format($splfixed_add_time, 2) . "s",
        number_format($splfixed_loop_time, 2) . "s",
        number_format($splfixed_total_time, 2) . "s",
        $splfixed_memory_usage
    );

    printf(
        $format,
        'SplDoublyLinkedList',
        number_format($spldllist_add_time, 2) . "s",
        number_format($spldllist_loop_time, 2) . "s",
        number_format($spldllist_total_time, 2) . "s",
        $spldllist_memory_usage
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
        'PHP Array     ',
        number_format($native_add_time, 2) . "s",
        number_format($native_loop_time, 2) . "s",
        number_format($native_total_time, 2) . "s",
        $native_memory_usage
    );

    printf(
        $format,
        ' -> JSON      ',
        number_format($native_json_add_time, 2) . "s",
        number_format($native_json_loop_time, 2) . "s",
        number_format($native_json_total_time, 2) . "s",
        $native_json_memory_usage
    );

    printf(
        $format,
        ' -> Serialized',
        number_format($native_serial_add_time, 2) . "s",
        number_format($native_serial_loop_time, 2) . "s",
        number_format($native_serial_total_time, 2) . "s",
        $native_serial_memory_usage
    );
    
}

function test_storeIntegers($amount=1024*1024, $real=false)
{
    // Measure Highest
    $dynamic_total_time = microtime(true);
    $list = new LessRam\Storage(LessRam\Storage::HIGHEST);

    // adding items
    $dynamic_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->append($i);
    }
    $dynamic_add_time = microtime(true) - $dynamic_add_time;

    // loop all data
    $dynamic_loop_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->getNext();
    }
    $dynamic_loop_time = microtime(true) - $dynamic_loop_time;

    $dynamic_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $dynamic_total_time = microtime(true) - $dynamic_total_time;


    // Measure Moderate
    $dynamic_m_total_time = microtime(true);
    $list = new LessRam\Storage(LessRam\Storage::MODERATE);

    // adding items
    $dynamic_m_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->append($i);
    }
    $dynamic_m_add_time = microtime(true) - $dynamic_m_add_time;

    // loop all data
    $dynamic_m_loop_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->getNext();
    }
    $dynamic_m_loop_time = microtime(true) - $dynamic_m_loop_time;

    $dynamic_m_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $dynamic_m_total_time = microtime(true) - $dynamic_m_total_time;


    // Measure Lowest
    $dynamic_l_total_time = microtime(true);
    $list = new LessRam\Storage(LessRam\Storage::LOWEST);

    // adding items
    $dynamic_l_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->append($i);
    }
    $dynamic_l_add_time = microtime(true) - $dynamic_l_add_time;

    // loop all data
    $dynamic_l_loop_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list->getNext();
    }
    $dynamic_l_loop_time = microtime(true) - $dynamic_l_loop_time;

    $dynamic_l_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $dynamic_l_total_time = microtime(true) - $dynamic_l_total_time;


    // Mesure SplFixedArray
    $splfixed_total_time = microtime(true);
    $list = new SplFixedArray($amount+1);

    // adding items
    $splfixed_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list[$i] = $i;
    }
    $splfixed_add_time = microtime(true) - $splfixed_add_time;

    // loop all data
    $splfixed_loop_time = microtime(true);
    foreach($list as $value){}
    $splfixed_loop_time = microtime(true) - $splfixed_loop_time;

    $splfixed_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $splfixed_total_time = microtime(true) - $splfixed_total_time;

    // Mesure SplDoublyLinkedList
    $spldllist_total_time = microtime(true);
    $list = new SplDoublyLinkedList($amount+1);

    // adding items
    $spldllist_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list[] = $i;
    }
    $spldllist_add_time = microtime(true) - $spldllist_add_time;

    // loop all data
    $spldllist_loop_time = microtime(true);
    foreach($list as $value){}
    $spldllist_loop_time = microtime(true) - $spldllist_loop_time;

    $spldllist_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $spldllist_total_time = microtime(true) - $spldllist_total_time;


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
            $list->push($i);
        }
        $dsvector_add_time = microtime(true) - $dsvector_add_time;

        // loop all data
        $dsvector_loop_time = microtime(true);
        foreach($list as $value){}
        $dsvector_loop_time = microtime(true) - $dsvector_loop_time;

        $dsvector_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

        $dsvector_total_time = microtime(true) - $dsvector_total_time;


        // Map
        $dsmap_total_time = microtime(true);
        $list = new \Ds\Map();

        // adding items
        $dsmap_add_time = microtime(true);
        for($i=0; $i<$amount; $i++)
        {
            $list->put($i, $i);
        }
        $dsmap_add_time = microtime(true) - $dsmap_add_time;

        // loop all data
        $dsmap_loop_time = microtime(true);
        foreach($list as $value){}
        $dsmap_loop_time = microtime(true) - $dsmap_loop_time;

        $dsmap_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

        $dsmap_total_time = microtime(true) - $dsmap_total_time;
    }


    // Measure native
    $native_total_time = microtime(true);
    $list = [];

    // adding items
    $native_add_time = microtime(true);
    for($i=0; $i<$amount; $i++)
    {
        $list[] = $i;
    }
    $native_add_time = microtime(true) - $native_add_time;

    // loop all data
    $native_loop_time = microtime(true);
    foreach($list as $value){}
    $native_loop_time = microtime(true) - $native_loop_time;

    $native_memory_usage = ceil(memory_get_usage($real) / 1024 / 1024) . "MB";

    $native_total_time = microtime(true) - $native_total_time;

    $format = "%16.16s | %12.12s | %8.8s | %10.10s | %13.13s\n";

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
        'LessRam Highest',
        number_format($dynamic_add_time, 2) . "s",
        number_format($dynamic_loop_time, 2) . "s",
        number_format($dynamic_total_time, 2) . "s",
        $dynamic_memory_usage
    );

    printf(
        $format,
        'LessRam Moderate',
        number_format($dynamic_m_add_time, 2) . "s",
        number_format($dynamic_m_loop_time, 2) . "s",
        number_format($dynamic_m_total_time, 2) . "s",
        $dynamic_m_memory_usage
    );

    printf(
        $format,
        'LessRam Lowest',
        number_format($dynamic_l_add_time, 2) . "s",
        number_format($dynamic_l_loop_time, 2) . "s",
        number_format($dynamic_l_total_time, 2) . "s",
        $dynamic_l_memory_usage
    );

    printf(
        $format,
        'SplFixedArray',
        number_format($splfixed_add_time, 2) . "s",
        number_format($splfixed_loop_time, 2) . "s",
        number_format($splfixed_total_time, 2) . "s",
        $splfixed_memory_usage
    );

    printf(
        $format,
        'SplDoublyLinkedList',
        number_format($spldllist_add_time, 2) . "s",
        number_format($spldllist_loop_time, 2) . "s",
        number_format($spldllist_total_time, 2) . "s",
        $spldllist_memory_usage
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
        'PHP Array',
        number_format($native_add_time, 2) . "s",
        number_format($native_loop_time, 2) . "s",
        number_format($native_total_time, 2) . "s",
        $native_memory_usage
    );
}