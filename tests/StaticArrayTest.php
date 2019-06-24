<?php

use LessRam\StaticArray;
use PHPUnit\Framework\TestCase;

class StaticArrayTest extends TestCase
{
    /**
     * Object to test.
     *
     * @var \LessRam\DynamicArray
     */
    protected static $object;

    public static function setUpBeforeClass(): void
    {
        self::$object = new StaticArray();
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

        $this->assertEquals("some string", $new_object[1]);
    }

    public function testOutOfRange()
    {
        $this->expectException("OutOfRangeException");

        self::$object[4];
    }

    public function testModifyException()
    {
        $this->expectException("Exception");

        self::$object[0] = 1;
    }

    public function testUnsetException()
    {
        $this->expectException("Exception");

        unset(self::$object[0]);
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