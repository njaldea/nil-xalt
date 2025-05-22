# cast

Contains utility templates that can be used for casting.

## nil::xalt::explicit_cast

```cpp
int main()
{
    int v = 100;

    nil::xalt::explicit_cast<int&> e_l(&v);
    e.cast(); // to int&
    
    nil::xalt::explicit_cast<int&&> e_r(&v);
    e.cast(); // to int&&

    // useful in template programming where
    // the type has to be stored and re-casted
}
```

## nil::xalt::implicit_cast (WIP)