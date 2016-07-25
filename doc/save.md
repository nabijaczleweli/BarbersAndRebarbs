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
| Compressed data |`{"6357":{"gun":{"left_in_mag":71,"left_mags":5,"trigger_pulled":false},"hp":1,"id":6357,"kind":"player","motion_x":0,"motion_y":0,"x":854.8089599609375,"y":269.0477294921875}}`|0x27B52FFD20AF00008B11F8691A70C502705A2D331A1BAD107244551596FF7D3BCCA22EF5FFF705CFC3D60D8A17ABDD95046E6A094E8D99B14B4E58CC35A93F78F598CB611C3D890DA4C8EB141CCD3E12387AA24261D107444A19E42551CA992C47ACAFA061FB121AAFCEB714915646DEE92F23DA9D0C003F00C062606D02807BC32CD80B940A41D804AEA0828F1900CD5B64952FC00000|
