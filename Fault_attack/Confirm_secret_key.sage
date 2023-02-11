# d is the secret key we guessed.
d = 0b111011001100110011010000110010001000001000010011100101111101111001100010111110100111110010001001010000000100011111000000000111001101101100111000010100110000001001011100011110010111011110010010101011011001100100110110011000010001100001011010111111110001100011100011110000000100011000011101111000100111010000010000111010110110100111111000111111100110001111010100001001111011100000101110011000001010011001111111100111100011100011101101000111011010100101111100101101111001000010110100100010111100011111110000011111101011101111001101111100010001010000000001010110111100001010001010111000100011100101011011001001010011110001010011001101101100010001000001110010000000101111111001011111010011100110111101100011110100000111010001110110001111010110100110011001101101110110101111010110111000111100000100011100000000000111100010111101100010000110101001101101000100000100101011001000011011011000011000111110100110111100110111101010110000110110100101000010100100001000010111001101101110111100011011000000001000000010111001100011001100101011101101001110000111000010010101111000111001110100100101101000001011001110100001111010011101101011000011101100001101001110000111111001101001000001001101010011100110011110001101101011001111010000000100000001110111111100111001111010111111110101010001001100101110100111111001001000000110001101010101000100001000111111001011001011100101011010010111001001101000000100010100100100000000111110101100101111011001111000111000000000101000001100000111111001011000101111101001110101010101110011000101110000101001110011111010011110101111000100011001101011001000011100000100011010100111110011101010111100011100010011000110010110000101001101001111100100000100100101000000010010101011011001100011111011110001010001111011010001100010101010010100100000011000001110010000100000101111000101011010000010000001010000000110101110010010000110101001011110001110011101001100101111011101010001011001011001011100101011100001111100010010111110101001100010000000010011000101111110100010101100010111110100101110001011101100011110110100000011101100101001
################################
N = 0xbec9b02a8a91abbf5b67885bfcb3b9692044030e17e8fce85177faea815a7b35e1beadbe90725308c2d76c7ca656ef35a677a70c5015e0efbd13c2171419e9148317c2e5b66a8b7d3e209c836c8d1987fb736f29fcc4e588aa2a00762c6297ac4f7fba7f241e3b49ae806898d49cf7027b20b049bc03939b5a651bbd9afceed431f2214b739083bae5c21475966f1a4aa3137006bb6b26a779ae0b9a584151001406ba024c689c018794cbcc4f237532461514a21e1a57aa56504b436e0939acaedfddafabac08e3a575207d6ab274c066674732362ad78925ef646414f038bbbb2b6f754e57f5ed0ab8aff1b541679e16aa959a40785bb748214d5a24a87b29
c = 0x1f549e6512a3a24c86b6dfaa159faca736c56ab6114ea2abcf16fe1bbf09469dfadba49f46bc16a96ecf0c0c46cd3eb4051ce70b720a9d92c6983900efef2d7d3337a0f2397554e6957fda21f860f8731822fa520dbe6f4d7f8ec8a207514e532b13f5e22a59bd3ccd0b6a437a175c2cc0528ea0fcf064d2f8b9a8ba88768010549143950d54451ced3799fa39eb14c8afed652c9fee36872984a7a1a42d800aaa5657ef3e9a3ef530e3745021a76286548f381bbc583f65008d117cd5f4791644c1aa38246e7397811bbca171920689a291b6228c9cf1a08b5ec9c46850dd2f76207d961590ca364dca203379afcd033e47370750f991dc84f27ea2d48b6290
e = 0x275a37b7d3bbfe4bf119fd22438c44c223e58d0c882fcd37d4e18126d7b0e34b2820b0caa1e35895ef736125a76df23b46f847011e24af4bf613fa753219a8020390b5ef2c5eead63d382786c8b2eae2e7a2c05660c7a0626e7c58d587b499a6529b85058c7a4b207c95e2d88935a70f2c2e894c39c305467bebb4d8d66106539fac24b114635e668b8ddf56e77d8033d7ec92c50e4fa8250ff441c8ff60dbecb2ea7702fa667eede86fa3784cd98e30adc9d3651c1ba551c410826c89267f81db23d9953c1c845e8416b13ef7553b18dad13d1bb92ff2de5f78dfbd5677c09849ddc1663369472cf9924575a64f2f99aecab654a8737d5d77bda684b48a2819
m = 0x4661554c7441547461434b40725341
time = pow(c,d,N)
print("secret key is",time == m)