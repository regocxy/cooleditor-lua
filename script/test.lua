print(string.format("hello"))

function foo(k, v)
    print(k, table.unpack(v))
end

foo("hello", {"world", "hi"})