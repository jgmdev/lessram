<?php

namespace LessRam;

use ArrayAccess;
use Countable;
use Iterator;

class Storage implements ArrayAccess, Countable, Iterator
{
    const LOWEST = 0,
        MODERATE = 1,
        HIGHEST = 2
    ;

    public function __construct(int $savings = self::LOWEST){}

    public function append($data): void {}

    public function prepend($data): void {}

    public function edit(int $position, $value): void {}

    public function remove(int $position): void {}

    public function getNext() {}

    public function clear(): void {}
}