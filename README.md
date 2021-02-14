# Optie

Optie is a tiny C++11 library that provides rust-like `optie::result<T,E>` and `optie::option<T>` classes. The `optie::result<T,E>` class can be used to return successful values / errors of different types in a unified way. It can be used to replace exceptions where they are undesired. The `optie::option<T>` class is a `std::optional<T>`-like class with rust-like api interface.

### `optie::result`

Here is an example:

```cpp
#include <optie/result.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

// A function that may fail during execution

optie::result<std::string, char const*> read_content(char const* fname) {
    if (!fname)
        return optie::err("bad argument");

    std::ifstream file(fname, std::ios_base::in);

    if (!file.good())
        return optie::err("file open failed");

    std::string content;
    std::ostringstream sstream;
    sstream << file.rdbuf();
    content = sstream.str();

    return optie::ok(content);
}
```

Then we can get the function result using one of the following methods:

```cpp
std::string s = read_content("nonexistent.txt")
    .or_default("I'm a default string");
```

```cpp
std::string s = read_content("nonexistent.txt").or_call([](char const* err) {
    std::cerr << "Error from λ: " << err << std::endl;
    return "I'm a default string from λ";
});
```

```cpp
std::string s = read_content("nonexistent.txt")
    .or_exit("Error from exit message");
```

### `optie::option`

Here is an example for `optie::option`:

```cpp
#include <optie/option.hpp>

#include <random>

// A function that may not return a value during execution

optie::option<char const*> gimme_or_not() {
    static std::random_device dev;
    static std::mt19937 eng(dev());
    static std::uniform_int_distribution<std::mt19937::result_type> dst(0, 1);

    if (dst(eng))
        return optie::some("Gimme-gimme!");
    else
        return optie::none();
}
```

Then we can get optional value using one of the following methods:

```cpp
char const* s = gimme_or_not().or_default("No gimme, but i have mine :)");
```

```cpp
char const* s = gimme_or_not().or_call([]() {
    std::cerr << "Message from λ: no value" << std::endl;
    return "I'm a default gimme from λ";
});
```

```cpp
char const* s = gimme_or_not().or_exit("No gimme :(");
```
