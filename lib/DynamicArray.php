<?php
/**
 * @author Jefferson González
 * @license MIT
 * @link https://github.com/jgmdev/lessram Source code.
 */

namespace LessRam;

/**
 * An array that can increment dynamically supporting: appending, prepending
 * removal and modification of elements. Consumes less ram than StaticArray
 * but performs slower. Only supports numerical indexes.
 */
class DynamicArray implements \ArrayAccess, \Countable, \Serializable, \Iterator
{
    public $elements;
    protected $elements_len;
    protected $list_size;
    protected $current_item = 0;
    protected $current_position = 0;

    public function __construct()
    {
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

        $value = str_replace("~", "\\~", $value);

        $this->elements .= "~" . $value;

        $this->elements_len += strlen($value) + 1;

        $this->list_size++;
    }

    public function prepend($value): void
    {
        if(!is_string($value))
        {
            $value = "\0:".serialize($value);
        }
        
        $value = str_replace("~", "\\~", $value);

        $this->elements = "~" . $value . $this->elements;

        $this->elements_len += strlen($value) + 1;

        $this->list_size++;
    }

    public function edit(int $position, $value): void
    {
        if($position >= $this->list_size)
            throw new \OutOfRangeException();

        if(!is_string($value))
        {
            $value = "\0:".serialize($value);
        }

        $value = str_replace("~", "\\~", $value);

        $str_pos = 0;
        $current_value = $this->get($position, $str_pos);

        $this->elements = substr_replace(
            $this->elements,
            "~" . $value,
            $str_pos,
            strlen($current_value) + 1
        );

        $this->elements_len -= strlen($current_value) + 1;
        $this->elements_len += strlen($value) + 1;
    }

    public function delete(int $position): void
    {
        if($position >= $this->list_size)
            throw new \OutOfRangeException();

        $str_pos = 0;
        $value = $this->getString($position, $str_pos);

        $this->elements = substr_replace(
            $this->elements,
            "",
            $str_pos,
            strlen($value) + 1
        );

        $this->elements_len -= strlen($value) + 1;

        $this->list_size--;
    }

    public function get(int $position, int &$strpos=-1)
    {
        $proximity = ($position / $this->list_size) * 100;

        $value = "";

        if($proximity <= 50)
        {
            $value = $this->getForward($position, $strpos);
        }
        else
        {
            $value = $this->getBackwards($position, $strpos);
        }

        if(substr($value, 0, 2) == "\0:"){
            return unserialize(substr_replace($value, "", 0, 2));
        }

        return $value;
    }

    public function getString(int $position, int &$strpos=-1): string
    {
        $proximity = ($position / $this->list_size) * 100;

        if($proximity <= 50)
        {
            return $this->getForward($position, $strpos);
        }
        else
        {
            return $this->getBackwards($position, $strpos);
        }
    }

    private function getForward(int $position, int &$strpos=-1): string
    {
        if($position >= $this->list_size)
            throw new \OutOfRangeException();

        $current_position = 0;
        $value = "";

        for($i=0; $i<$this->elements_len; $i++)
        {
            if($current_position == $position)
            {
                $strpos = $i;

                $i++;

                if($this->elements{$i} == "~")
                {
                    break;
                }

                do
                {
                    if(
                        $this->elements{$i} == "\\" 
                        && 
                        $this->elements{$i+1} == "~"
                    )
                    {
                        $i++;
                    }

                    $value .= $this->elements{$i};

                    $i++;
                }
                while(
                    $i < $this->elements_len
                    &&
                    $this->elements{$i} != "~"
                );

                break;
            }
            elseif($this->elements{$i} == "~")
            {
                $current_position++;

                if($this->elements{$i+1} == "~")
                {
                    continue;
                }

                do
                {
                    $i++;

                    if(
                        $this->elements{$i} == "\\" 
                        && 
                        ($i+1) <= $this->elements_len
                        &&
                        $this->elements{$i+1} == "~"
                    )
                    {
                        $i += 2;
                    }
                }
                while(
                    $i < $this->elements_len
                    &&
                    $this->elements{$i} != "~" 
                );

                $i--;
            }
        }

        return $value;
    }

    private function getBackwards(int $position, int &$strpos=-1): string
    {
        if($position >= $this->list_size)
            throw new \OutOfRangeException();

        $current_position = $this->list_size - 1;
        $value = "";

        for($i=$this->elements_len-1; $i>=0; $i--)
        {
            if($current_position == $position)
            {
                $escape = false;

                do
                {
                    $value = $this->elements{$i} . $value;

                    if($escape)
                    {
                        $i--;
                        $escape = false;
                    }

                    $i--;
                }
                while(
                    $i > 0
                    &&
                    (
                        $this->elements{$i} != "~"
                        ||
                        (
                            $this->elements{$i-1} == "\\"
                            &&
                            $escape = true
                        )
                    )
                );

                $strpos = $i;

                break;
            }
            else
            {
                $current_position--;

                do
                {
                    $i--;
                }
                while(
                    $i > 0
                    &&
                    (
                        $this->elements{$i} != "~"
                        ||
                        $this->elements{$i-1} == "\\"
                    )
                );
            }
        }

        return $value;
    }

    public function getAll(): iterable
    {
        for($i=0; $i<$this->elements_len; $i++)
        {
            $value = "";

            if($this->elements{$i} == "~")
            {
                do
                {
                    $i++;

                    if($this->elements{$i} != "~")
                    {
                        if(
                            $this->elements{$i} == "\\" 
                            && 
                            $this->elements{$i+1} == "~"
                        )
                        {
                            $i++;
                            $value .= $this->elements{$i};
                            $i++;
                        }
                        else
                        {
                            $value .= $this->elements{$i};
                        }
                    }
                }
                while($this->elements{$i} != "~" && $i < $this->elements_len);

                $i--;

                if(substr($value, 0, 2) == "\0:"){
                    $value = unserialize(substr_replace($value, "", 0, 2));
                }

                yield $value;
            }
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
        $this->elements = "";
        $this->elements_len = 0;
        $this->list_size = 0;
        $this->current_item = 0;
        $this->current_position = 0;
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
            $this->edit($offset, $value);
        }
    }

    public function offsetUnset ($offset): void
    {
        $this->delete($offset);
    }

    public function count (): int
    {
        return $this->list_size;
    }

    public function serialize (): string
    {
        return serialize($this->elements);
    }

    public function unserialize ($serialized)
    {
        $this->elements = unserialize($serialized);
        $this->elements_len = strlen($this->elements);
        $this->list_size = 0;

        // Get list size
        for($i=0; $i<$this->elements_len; $i++)
        {
            if($this->elements{$i} == "~")
            {
                do
                {
                    $i++;

                    if($this->elements{$i} != "~")
                    {
                        if(
                            $this->elements{$i} == "\\" 
                            && 
                            $this->elements{$i+1} == "~"
                        )
                        {
                            $i += 2;
                        }
                    }
                }
                while($this->elements{$i} != "~" && $i < $this->elements_len);

                $i--;

                $this->list_size++;
            }
        }
    }

    public function rewind() 
    {
        $this->current_item = 0;
        $this->current_position = 0;
    }

    public function current()
    {
        for($i=$this->current_position; $i<$this->elements_len; $i++)
        {
            $value = "";

            if($this->elements{$i} == "~")
            {
                // Handle empty string
                if($this->elements{$i+1} != "~")
                {
                    do
                    {
                        $i++;

                        if($this->elements{$i} != "~")
                        {
                            if(
                                $this->elements{$i} == "\\" 
                                && 
                                $this->elements{$i+1} == "~"
                            )
                            {
                                $i++;
                                $value .= $this->elements{$i};
                                $i++;
                            }
                            else
                            {
                                $value .= $this->elements{$i};
                            }
                        }
                    }
                    while($this->elements{$i} != "~" && $i < $this->elements_len);

                    $i--;
                }
                else
                {
                    $i++; //if empty string move to next char
                }


                $this->current_position = $i;

                if(substr($value, 0, 2) == "\0:"){
                    $value = unserialize(substr_replace($value, "", 0, 2));
                }

                return $value;
            }
        }
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
