<?php

use Lessram\Storage;

$data = new Storage(Storage::LOWEST);

for($i=0; $i<10; $i++)
{
    $data->append("~Hello world ~ #" . $i . "\n");
}

for($i=0; $i<10; $i++)
{
    echo $data->next();
}

for($i=0; $i<10; $i++)
{
    $data->append(["~Hello ~ #" . $i]);
}

for($i=0; $i<10; $i++)
{
    print_r($data->next());
}

for($i=0; $i<10; $i++)
{
    $data->append("~γειά σου κόσμε ~ #" . $i . "\n");
}

for($i=0; $i<10; $i++)
{
    print $data->next();
}
