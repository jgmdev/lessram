<?php
/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
 */

namespace LessRam;

/**
 * An array that can increment dynamically, but its elements can not be 
 * modified. Consumes more memory than DynamicArray but has better access 
 * performance. Only supports numerical indexes.
 */
class StaticArray implements \ArrayAccess, \Countable, \Serializable, \Iterator
{
    public $index;
    public $elements;
    protected $elements_len;
    protected $list_size;
    protected $current_item = 0;
    protected $current_position = 0;

    public function __construct()
    {
        $this->index = "";
        $this->elements = "";
        $this->elements_len = 0;
        $this->list_size = 0;
    }

    public function append($value): void
    {
        if(!is_string($value))
        {
            $value = "\0:".serialize($value);
        }

        $pos = $this->elements_len;
        $len = strlen($value);

        $this->index .= "|$pos,$len";

        $this->elements .= $value;

        $this->elements_len += strlen($value);

        $this->list_size++;
    }

    public function get(int $position)
    {
        if($position >= $this->list_size)
            throw new \OutOfRangeException();

        $index_size = strlen($this->index);

        $proximity = ($position / $this->list_size) * 100;

        if($proximity <= 50)
        {
            $index_pos = 0;

            for($i=0; $i<$this->list_size; $i++)
            {
                $index_pos = strpos($this->index, "|", $index_pos) + 1;

                if($i == $position)
                {
                    break;
                }
            }
        }
        else
        {
            $index_pos = strrpos(
                $this->index, "|"
            ) + 1;

            $i=$this->list_size-1;

            while($i != $position)
            {
                $i--;

                $index_pos = strrpos(
                    $this->index, 
                    "|", 
                    (($index_size - $index_pos) * -1) - 2
                ) + 1;
            }
        }

        $index = "";
        
        while($index_pos < $index_size && $this->index{$index_pos} != "|")
        {
            $index .= $this->index{$index_pos};
            $index_pos++;
        }

        $pos = explode(",", $index);

        $value = substr($this->elements, $pos[0], $pos[1]);

        if(substr($value, 0, 2) == "\0:"){
            return unserialize(substr_replace($value, "", 0, 2));
        }

        return $value;
    }

    public function getAll(): iterable
    {
        $index_pos = 0;
        $index_size = strlen($this->index);
        for($position=0; $position<$this->list_size; $position++)
        {
            $index_pos = strpos($this->index, "|", $index_pos) + 1;

            $index = "";
            
            while($index_pos < $index_size && $this->index{$index_pos} != "|")
            {
                $index .= $this->index{$index_pos};
                $index_pos++;
            }

            $pos = explode(",", $index);

            $value = substr($this->elements, $pos[0], $pos[1]);

            if(substr($value, 0, 2) == "\0:"){
                $value = unserialize(substr_replace($value, "", 0, 2));
            }

            yield $value;
        }
    }

    public function find(string $value): int
    {
        $position = 0;

        foreach($this->getAll() as $position_value)
        {
            if($position_value == $value)
            {
                return $position;
            }

            $position++;
        }

        return -1;
    }

    public function startsWith(string $value): string
    {
        foreach($this->getAll() as $position_value)
        {
            if(mb_strpos($position_value, $value) === 0)
            {
                return $position_value;
            }
        }

        return "";
    }

    public function len(): int
    {
        return $this->list_size;
    }

    public function clear(): void
    {
        $this->index = "";
        $this->elements = "";
        $this->elements_len = 0;
        $this->list_size = 0;
    }

    public function offsetExists($offset): bool
    {
        if($offset < $this->list_size)
            return true;

        return false;
    }

    public function offsetGet($offset)
    {
        return $this->get($offset);
    }

    public function offsetSet ($offset, $value): void
    {
        if(is_null($offset))
        {
            $this->append($value);
        }
        else
        {
            throw new \Exception("Cannot modify elements on a StaticArray");
        }
    }

    public function offsetUnset ($offset): void
    {
        throw new \Exception("Cannot remove elements on a StaticArray");
    }

    public function count (): int
    {
        return $this->list_size;
    }

    public function serialize (): string
    {
        return serialize([
            "index" => $this->index, 
            "elements" => $this->elements,
            "elements_len" => $this->elements_len,
            "list_size" => $this->list_size
        ]);
    }

    public function unserialize ($serialized)
    {
        $data = unserialize($serialized);
        
        $this->index = $data["index"];
        $this->elements = $data["elements"];
        $this->elements_len = $data["elements_len"];
        $this->list_size = $data["list_size"];
    }

    public function rewind() 
    {
        $this->current_item = 0;
        $this->current_position = 0;
    }

    public function current()
    {
        $index_size = strlen($this->index);

        $index_pos = $this->current_position;
        $index_pos = strpos($this->index, "|", $index_pos) + 1;

        $index = "";
        
        while($index_pos < $index_size && $this->index{$index_pos} != "|")
        {
            $index .= $this->index{$index_pos};
            $index_pos++;
        }

        $this->current_position = $index_pos;

        $pos = explode(",", $index);

        $value = substr($this->elements, $pos[0], $pos[1]);

        if(substr($value, 0, 2) == "\0:"){
            return unserialize(substr_replace($value, "", 0, 2));
        }

        return $value;
    }

    public function key() 
    {
        return $this->current_item;
    }

    public function next() 
    {
        $this->current_item++;
    }

    public function valid() 
    {
        return $this->offsetExists($this->current_item);
    }
}
