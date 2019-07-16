<?php

$data = new LessRam\DynamicArray();

for($i=0; $i<10; $i++)
{
    $data->appendData("~Hello ~ #" . $i . "\n");
}

for($i=0; $i<10; $i++)
{
    echo $data->next();
}