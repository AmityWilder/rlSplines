# Math

By Amy Wilder

The symbol $\tfrac{d}{dt}\_$ means "the **derivative** of $\_$ with respect to $t$".

## Process

To find the derivative of an expression, first look at the expression in reverse-order of operations:

1. Addition/subtraction
2. Multiplication/division
3. Exponents/roots
4. Groupings (parentheses/brackets)

Ignore anything that isn't at the order you are currently looking at, and recursively solve each derivative separately until you reach a rule doesn't have a derivative in the answer.

### Example

Say you want to find the derivative of

$$\frac{3t}{6t^2 + 4} + 5t - 8$$

```c
(3*t)/(6*t*t + 4) + 5*t - 8
```

First, group this up by order of operations.

$$
\tfrac{d}{dt}
\left(
    \left(
        \frac{
        \left(
            3
            \cdot
            t
        \right)
        }{
        \left(
            \left(
                6
                \cdot
                \left(
                    t^2
                \right)
            \right)
            +
            4
        \right)
        }
    \right)
    +
    \left(
        5
        \cdot
        t
    \right)
    -
    8
\right)
$$

Then, recursively solve the derivative starting from the outside until there are no $\tfrac{d}{dt}$ left.

$$
\tfrac{d}{dt}(\square + \square - \square) = \underbrace{\tfrac{d}{dt}\square}_1 + \underbrace{\tfrac{d}{dt}\square}_2 - \underbrace{\tfrac{d}{dt}\square}_3 \\~\\
\begin{align*}
1.~& \begin{aligned}
\tfrac{d}{dt}\left(\frac{\square}{\triangle}\right) = \frac{\overbrace{\left(\tfrac{d}{dt}\square\right)}^{1.1}\triangle - \square\overbrace{\left(\tfrac{d}{dt}\triangle\right)}^{1.2}}{\triangle^2}
1.1
\end{aligned}\\~\\
2.~& \tfrac{d}{dt} \\~\\
3.~& \tfrac{d}{dt}8 = 0
\end{align*}
$$

## Rules of Derivatives

In the following list, keep in mind that $f(t)$ and $g(t)$ can be **any expression**, and $n$ can be **any value or expression** that *doesn't care about $t$*.

We will use `D_DT` to represent a hypothetical macro that somehow finds the derivative (with respect to `t`) of whatever expression is passed to it.

### Sum/Difference Rule

The derivative of a sum of expressions is the sum of the derivatives of each expression being added together.

$$\tfrac{d}{dt}\big(f(t) \pm g(t)\big) = \tfrac{d}{dt}f(t) \pm \tfrac{d}{dt}g(t)$$

```c
D_DT(Foo(t) + Bar(t)) == (D_DT(Foo(t)) + D_DT(Bar(t)));
D_DT(Foo(t) - Bar(t)) == (D_DT(Foo(t)) - D_DT(Bar(t)));
```

#### Examples

- $
\frac{d}{dt}\left(3t^2 + 5t + 2\right) = \frac{d}{dt}(3t^2) + \frac{d}{dt}(5t) + \frac{d}{dt}(2)
$

### Product Rule

**NOTE:**
- Recursion ends here for the following parts:
  - First $g(t)$.
  - Second $f(t)$.

$$\tfrac{d}{dt}\big(f(t)g(t)\big) = \left(\tfrac{d}{dt}f(t)\right)g(t) + f(t)\left(\tfrac{d}{dt}g(t)\right)$$

```c
D_DT(Foo(t)*Bar(t)) == (D_DT(Foo(t))*Bar(t) + Foo(t)*D_DT(Bar(t)));
```

#### Examples

### Quotient Rule

**NOTE:**
- Recursion ends here for the following parts:
  - First $g(t)$ in the numerator.
  - Second $f(t)$ in the numerator.
  - Entire denominator.

$$\tfrac{d}{dt}\left(\frac{f(t)}{g(t)}\right) = \frac{\left(\tfrac{d}{dt}f(t)\right)g(t) - f(t)\left(\frac{d}{dt}g(t)\right)}{{\big(g(t)\big)}^2}$$

```c
D_DT(Foo(t)/Bar(t)) == ((D_DT(Foo(t))*Bar(t) - Foo(t)*D_DT(Bar(t)))/(Bar(t)*Bar(t)));
```

#### Examples

### Power Rule

**NOTE:**
- Recursion ends here.
- Base must be $t$, not just any expression *containing* $t$.
- Exponent must not depend on $t$.
- **Use the [Chain Rule](#chain-rule) instead if the base is anything but $t$, or if the exponent depends on $t$.**

$$\tfrac{d}{dt}(t^n) = nt^{n - 1}$$

```c
// ...
D_DT(1/(t*t*t*t*t)) == (-5/(t*t*t*t*t*t));
D_DT(1/(t*t*t*t)) == (-4/(t*t*t*t*t));
D_DT(1/(t*t*t)) == (-3/(t*t*t*t));
D_DT(1/(t*t)) == (-2/(t*t*t));
D_DT(1/t) == (-1/(t*t));                    // pow(t, -1) == 1/t
D_DT(1) == 0;                               // pow(t, 0) == 1
D_DT(sqrt(t)) == (0.5/(sqrt(t)));           // sqrt(t) == pow(t, 0.5)
D_DT(t) == 1;
D_DT(t*t) == (2*t);
D_DT(t*t*t) == (3*t*t);
D_DT(t*t*t*t) == (4*t*t*t);
D_DT(t*t*t*t*t) == (5*t*t*t*t);
// ...
D_DT(pow(t, n)) == (n*pow(t, n - 1));
```

#### Examples

### Chain Rule

**NOTE:**
- Recursion ends here for $u$.

$$\tfrac{d}{dt}\Big(f\big(g(t)\big)\Big) = \big(\tfrac{d}{dt}\underbrace{f(u)}_{u=g(t)}\big)\big(\tfrac{d}{dt}g(t)\big)$$

```c
float u;
D_DT(Foo(Bar(t))) == (
    u = Bar(t),
    (D_DT(Foo(u))*D_DT(Bar(t)))
);
```

#### Examples
