## egb-lang syntax

### Variables
Variables may be assigned to a literal or the result of an expression. If the
function returns a tuple, multiple variables may be initialized or assigned with
the respective elements of the tuple.

```
  uint32 v;       # Declaration

  uint32 v = 10;  # Declaration with initialization
  uint32 sum = 2 + 4 + v;

  [int16, int16] get_pair() {
    return [2 + 4, 16];
  }

  uint16 x, uint16 y = get_pair(); # Tuple initialization
  
  uint16 u;
  uint16 v;

  u, v = get_pair();
```

Type narrowing is not an error. When assigning a value that won't fit in
the destination type, the most sig. bits will be truncated, and an error will
be thrown. The user may opt to treat these warnings as errors.

```
  uint64 big_ol_number = 10;
  uint8 teeny_tiny = big_ol_number; # Truncation and warning
```

### Functions
Functions may return a single value, or a list of elements with
heterogenous types.

```
  uint32 main() {                    # Signature with body
    return 0;
  }

  uint16 add(uint16 x, uint16 y) {  # Signature with params and body
    return x + y;
  }  

  [int16, int16] get_pair() {       # Tuple
    return [2 + 4, 16];
  }

  add();                            # Call

  add(5, get_num());                # Call with arguments
```

### Structures
```
struct Vector3 {
  uint32 x;       # Members are always public
  uint32 y;
  uint32 z; 

  double get_magnitude() {
    return sqrt(x*x + y*y + z*z);
  }
}

Vector3 vec;
vec.x = 10;
vec.y = 20;
vec.x = 5;

uint32 magnitude = vec.get_magnitude();
```
