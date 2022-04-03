## egb-lang syntax

### Variables
Variables may be assigned to a literal or the result of an expression. If the
function returns a tuple, multiple variables may be initialized or assigned with
the respective elements of the tuple.

```cpp
  u32 v;       # Declaration

  u32 v = 10;  # Declaration with initialization
  u32 sum = 2 + 4 + v;

  [int16, int16] get_pair() {
    return [2 + 4, 16];
  }

  u16 x, u16 y = get_pair(); # Tuple initialization
  
  u16 u;
  u16 v;

  u, v = get_pair();
```

Type narrowing is not an error. When assigning a value that won't fit in
the destination type, the most sig. bits will be truncated, and warning will
be thrown. The user may opt to treat these warnings as errors.

```cpp
  u64 big_ol_number = 10;
  u8 teeny_tiny = big_ol_number; # Truncation and warning
```

### Functions
Functions may return a single value, or a list of elements with
the same or different types.

```cpp
  u32 main() {                    # Signature with body
    return 0;
  }

  u16 add(u16 x, u16 y) {  # Signature with params and body
    return x + y;
  }  

  [int16, int16] get_pair() {       # Tuple
    return [2 + 4, 16];
  }

  add();                            # Call

  add(5, get_num());                # Call with arguments
```

Variables in outers scopes will be shadowed by variables with the same name in
nested scopes.

```cpp
  u32 n = 10;

  u32 main() {
    u16 n = 50; # Global "n" is now inaccessable until local "n" goes out of scope
  }
```

### Structures

```cpp
struct Vector3 {
  u32 x;       # Members are always public
  u32 y;
  u32 z; 

  double get_magnitude() {
    return sqrt(x*x + y*y + z*z);
  }
}

Vector3 vec;
vec.x = 10;
vec.y = 20;
vec.x = 5;

u32 magnitude = vec.get_magnitude();
```
