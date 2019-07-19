<?php

namespace LessRam;

use ArrayAccess;
use Countable;
use Iterator;

class Storage implements ArrayAccess, Countable, Iterator
{
    public function __construct(int $stack = 30){}

    public function append($data): void {}

    public function prepend($data): void {}

    public function edit(int $position, $value): void {}

    public function remove(int $position): void {}

    public function getNext() {}

    public function clear(): void {}
}