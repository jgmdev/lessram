<?php

use Lessram\Storage;

$data = new Storage(Storage::LOWEST);

for($i=0; $i<10; $i++)
{
    $data->append("~Hello world ~ #" . $i . "\n");
}

for($i=0; $i<10; $i++)
{
    echo $data->getNext();
}

for($i=0; $i<10; $i++)
{
    $data->append(["~Hello ~ #" . $i]);
}

for($i=0; $i<10; $i++)
{
    print_r($data->getNext());
}

for($i=0; $i<10; $i++)
{
    $data->append("~γειά σου κόσμε ~ #" . $i . "\n");
}

for($i=0; $i<10; $i++)
{
    print $data->getNext();
}

for($i=0; $i<10; $i++)
{
    $data->prepend(["~Hello ~ #" . $i]);
}

$data->rewind();

for($i=0; $i<10; $i++)
{
    print_r($data->getNext());
}