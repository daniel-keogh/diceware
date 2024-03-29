# Diceware

C implementation of the [diceware](https://en.wikipedia.org/wiki/Diceware) method of generating passphrases.

## Usage

```sh
$ make
$ ./diceware [options]
```

### Options

| Option                        | Description                                                                |
| ----------------------------- | -------------------------------------------------------------------------- |
| `-h, --help`                  | Prints some help text.                                                     |
| `-d, --delimiter <delimiter>` | Provide a character with which to separate the words. Defaults to a space. |
| `-l, --length <length>`       | The length of the passphrase. The default is six words.                    |
| `-f, --file <file>`           | Path to the input file.                                                    |
| `-v, --verbose`               | Prints additional information.                                             |

#### Example

```sh
$ ./diceware -l 5 -v

Roll 1:  55115      sough
Roll 2:  31131      gent
Roll 3:  35521      kp
Roll 4:  21241      coddle
Roll 5:  12436      argot

Your passphrase is: sough gent kp coddle argot
```
