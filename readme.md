# RC5

Charles Emerson

A simple RC5 implementation. Use at your own risk.


## Background

* [RC5 on Wikipedia](https://en.wikipedia.org/wiki/RC5)
* [The Original RC5 Paper](https://people.csail.mit.edu/rivest/Rivest-rc5rev.pdf)
* [Improved Cryptanalysis of RC5](https://link.springer.com/chapter/10.1007/BFb0054119)
* [Active Project to Brute-Force RC5](https://www.distributed.net/RC5)


## Quick-Start

### Install

Requires at least CMake 2.6

```bash
# Quick-Start Bash Script
git clone https://github.com/cjemerson/rc5.git
cd rc5
mkdir build
cd build
cmake ..
make install
```


### Examples to Decrypt

See the [examples directory](examples/).

 * `examples/hello_world.txt.rc5_16-12-16`
 * `examples/success-3195027_640.jpg.rc5_32-12-32`
 * `examples/invalid.png`


### Usage by Example

#### Encrypt

###### List help

`rc5-encrypt --help` lists the help.

###### Verbose Output

`rc5-encrypt -v ...` outputs every program parameter (including the secret key).

###### Default Encrypt (Null-String Secret Key)

`rc5-encrypt -v -i <input file>` will default encrypt a file with a default-length null-string secret key.

###### Default Encrypt (Specified Secret Key)

`rc5-encrypt -v -b 32 -k "A Super Secret Key" -i <input file>` will default encrypt a file with a 32-byte secret key (resized appropriately).

###### Specified Encrypt Parameters

`rc5-encrypt -v -w 64 -r 20 -b 32 -k "A Super Secret Key" -i <input file>` will encrypt a file with 64-byte words, 20 rounds of encryption and a 32-byte secret key.

###### Specify Output File

`rc5-encrypt ... -o <output file>` specifies the output file (!!!Will overwrite an existing file!!!).

### Decrypt

###### List help

`rc5-decrypt --help` lists the help.

###### Verbose Output

`rc5-decrypt -v ...` outputs every parameter used (including the secret key).

###### Decrypt using File Name (Null-String Secret Key)

`rc5-decrypt -v -i input_file.txt.rc5_32-12-16` will use the parameters: 32-byte words, 12 rounds of encryption and 16-byte secret key (!!!This will overwrite an existing input.txt!!!).

###### Decrypt using File Name (Specified Secret Key)

`rc5-decrypt -v -k "A Super Secret Key" -i input_file.txt.rc5_32-12-16` will use the parameters: 32-byte words, 12 rounds of encryption and 16-byte secret key (!!!This will overwrite an existing input.txt!!!).

###### Default Decrypt (Null-String Secret Key)

`rc5-decrypt -v -i <input file> -o <output file>` will default decrypt a file with a null-string secret key.

###### Default Decrypt (Specified Secret Key)

`rc5-decrypt -v -b 32 -k "A Super Secret Key" -i <input file> -o <output file>` will default decrypt a file with a 32-byte secret key (resized appropriately).

###### Specified Decrypt Parameters

`rc5-decrypt -v -w 64 -r 20 -b 32 -k "A Super Secret Key" -i <input file> -o <output file>` will decrypt a file with 64-byte words, 20 rounds of encryption and a 32-byte secret key.

###### Specify Output File

`rc5-decrypt ... -o <output file>` specifies the output file (!!!Will overwrite an existing file!!!).
