# Savefile format
|      Field      |            Type           |
|-----------------|---------------------------|
|Uncompressed size|  8-byte, native ordering  |
| Compressed size |  8-byte, native ordering  |
| Compressed data |`zstd`-compressed JSON data|

## Example
|      Field      | Raw |        Out       |
|-----------------|-----|------------------|
|Uncompressed size| 175 |0xAF00000000000000|
| Compressed size | 151 |0x9700000000000000|
| Compressed data |`{"documentation_example": [1]}`|0x1E000000000000002A0000000000000027B52FFD201E40001E7B22646F63756D656E746174696F6E5F6578616D706C65223A205B315D7DC00000|
