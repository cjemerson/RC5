# Examples Directory

| File | Word Bit Length / Num Rounds |Secret Key / Key Length | Command to Decrypt |
|---|---|---|---|
| hello_world.txt.rc5_16-12-16 | 16 / 12 | "" / 16 | `rc5-decrypt -v -i hello_world.txt.rc5_16-12-16` |
| success-3195027_640.jpg.rc5_32-12-32 | 32 / 12 |  "A Super Secret Key" / 32 | `rc5-decrypt -v -b 32 -k "A Super Secret Key" -i success-3195027_640.jpg.rc5_32-12-32` |
| invalid.png | 64 / 20 | "12345678" / 8 | `rc5-decrypt -v -w 64 -r 20 -b 8 -k "12345678" -i invalid.png -o output.zip` |
