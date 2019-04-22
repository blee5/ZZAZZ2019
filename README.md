These are my solutions to TheZZAZZGlitch's April Fools 2019 Event, which can be found [here](https://zzazzdzz.github.io/fools2019/).

## Pwnage Kingdom 1
The challenge was to bypass two trainers (they will crash the game if you attempt to fight them) and reach the otherside.
I found the [disassembly for Pokemon Crystal](https://github.com/pret/pokecrystal/blob/master/wram.asm),
which contained information about its data structure. 
There were probably many posible methods, but what I did was find where map objects were stored and
zeroed some objects out, deleting the trainers from existence. Probably not the simplest method??

## Pwnage Kingdom 2
You are given a save file with two maps available, but the other map is inaccessible because you're stuck in one with no exits.
The goal was to enter the other hidden map.

My solution was to find [where the current map ID was stored](https://github.com/pret/pokecrystal/blob/d5f5e6d5dbfef49fac2c436d34f766497c8c0a5f/wram.asm#L2829)
and to freeze that address into another value as the map was first being loaded. The map ID of the hidden map wasn't given, but the first thing I tried (adding 1 to the first map's ID) worked, so yay?

## Pwnage Kingdom 3
tl;dr
- Find the decryption routine
- Translate into C
- Recognize (with the help of other people) that this is an implementation of a [linear congruential generator](https://en.wikipedia.org/wiki/Linear_congruential_generator)
- Find the period of the outer LCG, where the numbers start repeating
- Recognize that the XORs from the first period will cancel out the XORs from the next period, which significantly reduces the number of iterations needed

## Pwnage Kingdom 4
lol no
