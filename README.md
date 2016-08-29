# four-fours

My sister-in-law challenged me to find expressions that evaluate to all the numbers between 1 and 25 composed of four fours and any math operators I chose. Here is my solution.

## Build steps

```
make
```

## Run steps

```
./fours 32
  0 = ((4 + 4) - (4 + 4))
  1 = ((4 + 4) / (4 + 4))
  2 = ((4 * 4) / (4 + 4))
  3 = (4 + ((4 / 4) - √(4)))
  4 = (4 + ((4 - 4) / 4))
  5 = (4 + (√(4) - (4 / 4)))
  6 = (4 + ((4 + 4) / 4))
  7 = ((4 + 4) - (4 / 4))
  8 = ((4 + 4) + (4 - 4))
  9 = ((4 + 4) + (4 / 4))
 10 = (4 + ((4 + 4) - √(4)))
 11 = (((4!) / √(4)) - (4 / 4))
 12 = (4 + ((4!) - (4 * 4)))
 13 = ((4 / 4) + ((4!) / √(4)))
 14 = (4 + ((4 + 4) + √(4)))
 15 = ((4 * 4) - (4 / 4))
 16 = ((4 + 4) + (4 + 4))
 17 = ((4 / 4) + (4 * 4))
 18 = (4 + ((4 * 4) - √(4)))
 19 = (((4!) - (4 / 4)) - 4)
 20 = (4 + ((4!) - (4 + 4)))
 21 = (((4 / 4) + (4!)) - 4)
 22 = (4 + ((4 * 4) + √(4)))
 23 = ((4 / 4) + ((4!) - √(4)))
 24 = ((4 + 4) + (4 * 4))
 25 = ((√(4) + (4!)) - (4 / 4))
 26 = ((4 - 4) + (√(4) + (4!)))
 27 = (4 + ((4!) - (4 / 4)))
 28 = (4 + ((4 - 4) + (4!)))
 29 = (4 + ((4 / 4) + (4!)))
 30 = ((4 + 4) + ((4!) - √(4)))
 31 = ((4!) + ((4 + (4!)) / 4))
 32 = ((4 * 4) + (4 * 4))
```

More info on the game can be found [here](https://en.wikipedia.org/wiki/Four_fours). Note that my solution does not include `.4` or other more sophisticated operators.

