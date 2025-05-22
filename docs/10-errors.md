# errors

Contains utility templates that can be used for debugging.

## nil::xalt::undefined

A deleted template designed to cause a compilation failure. This failure allows the compiler to display the template's arguments for debugging.

```cpp
int main()
{
    nil::xalt::undefined<int, T>();
    nil::xalt::undefined<1, 2, 3>();
    // will cause compilation error and print out the arguments
}
```
