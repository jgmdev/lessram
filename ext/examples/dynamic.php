<?php

$data = new LessRam\DynamicArray(LessRam\DynamicArray::LOWEST);

for($i=0; $i<10; $i++)
{
    $data->append("~Hello ~ #" . $i . "\n");
}

for($i=0; $i<10; $i++)
{
    echo $data->next();
}

for($i=0; $i<1024*1024; $i++)
{
    $data->append(["~Hello ~ #" . $i]);
}

for($i=0; $i<1024*1024; $i++)
{
    print_r($data->next());
}