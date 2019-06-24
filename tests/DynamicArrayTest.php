<?php

use LessRam\DynamicArray;
use PHPUnit\Framework\TestCase;

class DynamicArrayTest extends TestCase
{
    /**
     * Object to test.
     *
     * @var \LessRam\DynamicArray
     */
    protected static $object;

    public static function setUpBeforeClass(): void
    {
        self::$object = new DynamicArray();
    }

    public function testEmptyAppend()
    {
        self::$object[] = "";

        $this->assertEquals("", self::$object[0]);
    }

    public function testAppendString()
    {
        self::$object[] = "some string";

        $this->assertEquals("some string", self::$object[1]);
    }

    public function testAppendArray()
    {
        self::$object[] = ["field" => "value"];

        $this->assertEquals("value", self::$object[2]["field"]);
    }

    public function testPrepend()
    {
        self::$object->prepend("prepended");

        $this->assertEquals("prepended", self::$object[0]);
        $this->assertEquals("", self::$object[1]);
    }

    public function testForeachLoop()
    {
        self::$object[] = "new value";

        $new_value = "";
        foreach(self::$object as $value)
        {
            $new_value = $value;
        }

        $this->assertEquals("new value", $new_value);
    }

    public function testSerializeUnserialize()
    {
        $data = serialize(self::$object);
        $new_object = unserialize($data);

        $this->assertEquals("prepended", $new_object[0]);
    }

    public function testOutOfRange()
    {
        $this->expectException("OutOfRangeException");

        self::$object[6];
    }

    public function testModify()
    {
        self::$object[0] = 1;

        $this->assertEquals(1, self::$object[0]);
    }

    public function testUnset()
    {
        unset(self::$object[1]);

        $this->assertEquals("some string", self::$object[1]);
    }

    public function testLen()
    {
        $this->assertEquals(4, self::$object->len());
    }

    public function testClear()
    {
        self::$object->clear();
        $this->assertEquals(0, self::$object->len());
    }
}