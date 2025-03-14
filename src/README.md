# Math

By Amy Wilder

This document may help you to find **derivatives**.
The symbol $\tfrac{d}{dt}\underline{\phantom{\dots}}$ means "the **derivative** of $\underline{\phantom{\dots}}$ with respect to $t$".

You may need to find the **derivative** of your `PositionCallback` function to provide rlsplines with a custom `VelocityCallback` function,
and the **derivative** of your `VelocityCallback` function to provide a custom `AccelerationCallback` function.

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

## Process

To find the derivative of an expression, first look at the expression in reverse-order of operations:

1. Addition/subtraction
2. Multiplication/division
3. Exponents/roots
4. Groupings (parentheses/brackets)

Ignore anything that isn't at the order you are currently looking at, and recursively solve each derivative separately until you reach a rule doesn't have a derivative in the answer.

### Example

Let's say you want to find the derivative of

$$\frac{3t}{6t^2 + 4} + 5t - 8$$

```c
(3*t)/(6*t*t + 4) + 5*t - 8
```

First, look at the outermost order of operations. Then, recursively solve the derivative starting from the outside until there are no $\tfrac{d}{dt}$ left.

$\displaystyle\tfrac{d}{dt}(\square + \triangle - \Diamond) \quad=\quad \overbrace{\tfrac{d}{dt}\square}^1 + \overbrace{\tfrac{d}{dt}\triangle}^2 - \overbrace{\tfrac{d}{dt}\Diamond}^3$ \
([Sum/Difference Rule](#sumdifference-rule))

<ol type="1">
<li>    <!-- 1 -->

$\displaystyle\tfrac{d}{dt}\left(\frac{\square}{\triangle}\right) \quad=\quad \frac{\overbrace{\left(\tfrac{d}{dt}\square\right)}^{1.1}\cdot\triangle - \square\cdot\overbrace{\left(\tfrac{d}{dt}\triangle\right)}^{1.2}}{\triangle^2} \quad=\quad \frac{\left(\tfrac{d}{dt}\square\right)\cdot(6t^2+4) - (3t)\cdot\left(\tfrac{d}{dt}\triangle\right)}{{(6t^2+4)}^2}$ \
([Quotient Rule](#quotient-rule))

<ol type="1">
<li>    <!-- 1.1 -->

$\displaystyle\tfrac{d}{dt}(3 \cdot \square) \quad=\quad 3 \cdot \overbrace{\tfrac{d}{dt}\square}^{1.1.1}$ \
([Constant Multiple Rule](#constant-multiple-rule))

<ol type="1">
<li>    <!-- 1.1.1 -->

$\displaystyle\tfrac{d}{dt}t = 1$ \
([Power Rule](#power-rule))

</li>   <!-- 1.1.1 -->
</ol>

$3 \cdot \underset{\mathclap{\gray{\tfrac{d}{dt}t}}}{1} = 3$

</li>   <!-- 1.1 -->
</ol>

</li>   <!-- 1 -->
<li>    <!-- 2 -->

$\displaystyle\tfrac{d}{dt}(5 \cdot \square) \quad=\quad 5 \cdot \overbrace{\tfrac{d}{dt}\square}^{2.1}$ \
([Constant Multiple Rule](#constant-multiple-rule))

<ol>
<li>    <!-- 2.1 -->

$\displaystyle\tfrac{d}{dt}t = 1$ \
([Power Rule](#power-rule))

</li>   <!-- 2.1 -->
</ol>

$5 \cdot \underset{\mathclap{\gray{\tfrac{d}{dt}t}}}{1} = 5$

</li>   <!-- 2 -->
<li>    <!-- 3 -->

$\displaystyle\tfrac{d}{dt}8 = 0$ \
([Constant Rule](#constant-rule))

</li>   <!-- 3 -->
</ol>

```c
#define D_DT(...) // magical pseudocode

(3*t)/(6*t*t + 4) + 5*t - 8;

D_DT(/*(3*t)/(6*t*t + 4)*/ + /*5*t*/ - /*8*/)                   // Sum/difference rule
    == (D_DT(/*(3*t)/(6*t*t + 4)*/) + D_DT(/*5*t*/) - D_DT(/*8*/));

    // 1
    D_DT((/*3*t*/)/(/*6*t*t + 4*/))                             // Quotient rule
        == ((D_DT(/*3*t*/)*(6*t*t + 4) - (3*t)*D_DT(/*6*t*t + 4*/))/pow(6*t*t + 4, 2));

        // 1.1
        D_DT(3*(/*t*/))                                         // Constant multiple rule
            == (3*D_DT(/*t*/));

            // 1.1.1
            D_DT(t) == 1;                                       // Power rule

        (3*1) == 3;                                             // Algebra

        // 1.2
        D_DT(/*6*t*t*/ + /*4*/)                                 // Sum/difference rule
            == (D_DT(/*6*t*t*/) + D_DT(/*4*/));

            // 1.2.1
            D_DT(6*/*t*t*/)                                     // Constant multiple rule
                == (6*D_DT(/*t*t*/));

                // 1.2.1.1
                D_DT(t*t) == (2*t);                             // Power rule

            (6*(2*t)) == (12*t);                                // Algebra

    ((3*(6*t*t + 4) - (3*t)*(12*t))/pow(6*t*t + 4, 2))          // Algebra
        == ((6*(2 - 3*t*t))/pow(6*t*t + 4, 2));

    // 2
    D_DT(5*(/*t*/))                                             // Constant multiple rule
        == (5*D_DT(/*t*/));

        // 2.1
        D_DT(t) == 1;                                           // Power rule

    5*1 == 5;                                                   // Algebra

    // 3
    D_DT(8) == 0;                                               // Constant rule
```
