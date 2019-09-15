# Diceware

Simple C implementation of the [diceware](http://world.std.com/~reinhold/diceware.html) mechanism of generating passphrases.

## Usage

```sh
$ diceware [OPTIONS]
```

### Options

- `-h` - Prints some help text.
- `-d <delimeter>` - Provide a character with which to separate the words. Defaults to a space.
- `-l <length>` - Specify the number of words to be contained in the passphrase.

#### Example

```sh
$ diceware -l 7 -d "_"
```

## Sample Output

```
Roll 1:  55115      sough
Roll 2:  31131      gent
Roll 3:  35521      kp
Roll 4:  21241      coddle
Roll 5:  12436      argot

Your passphrase is: sough gent kp coddle argot
```
