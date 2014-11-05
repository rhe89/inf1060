decode_dict = {
	0x00: u"    ",
	0x01: u"   :",
	0x02: u"   @",
	0x03: u"   \n",
	0x04: u"  : ",
	0x05: u"  ::",
	0x06: u"  :@",
	0x07: u"  :\n",
	0x08: u"  @ ",
	0x09: u"  @:",
	0x0a: u"  @@",
	0x0b: u"  @\n",
	0x0c: u"  \n ",
	0x0d: u"  \n:",
	0x0e: u"  \n@",
	0x0f: u"  \n\n",
	0x10: u" :  ",
	0x11: u" : :",
	0x12: u" : @",
	0x13: u" : \n",
	0x14: u" :: ",
	0x15: u" :::",
	0x16: u" ::@",
	0x17: u" ::\n",
	0x18: u" :@ ",
	0x19: u" :@:",
	0x1a: u" :@@",
	0x1b: u" :@\n",
	0x1c: u" :\n ",
	0x1d: u" :\n:",
	0x1e: u" :\n@",
	0x1f: u" :\n\n",
	0x20: u" @  ",
	0x21: u" @ :",
	0x22: u" @ @",
	0x23: u" @ \n",
	0x24: u" @: ",
	0x25: u" @::",
	0x26: u" @:@",
	0x27: u" @:\n",
	0x28: u" @@ ",
	0x29: u" @@:",
	0x2a: u" @@@",
	0x2b: u" @@\n",
	0x2c: u" @\n ",
	0x2d: u" @\n:",
	0x2e: u" @\n@",
	0x2f: u" @\n\n",
	0x30: u" \n  ",
	0x31: u" \n :",
	0x32: u" \n @",
	0x33: u" \n \n",
	0x34: u" \n: ",
	0x35: u" \n::",
	0x36: u" \n:@",
	0x37: u" \n:\n",
	0x38: u" \n@ ",
	0x39: u" \n@:",
	0x3a: u" \n@@",
	0x3b: u" \n@\n",
	0x3c: u" \n\n ",
	0x3d: u" \n\n:",
	0x3e: u" \n\n@",
	0x3f: u" \n\n\n",
	0x40: u":   ",
	0x41: u":  :",
	0x42: u":  @",
	0x43: u":  \n",
	0x44: u": : ",
	0x45: u": ::",
	0x46: u": :@",
	0x47: u": :\n",
	0x48: u": @ ",
	0x49: u": @:",
	0x4a: u": @@",
	0x4b: u": @\n",
	0x4c: u": \n ",
	0x4d: u": \n:",
	0x4e: u": \n@",
	0x4f: u": \n\n",
	0x50: u"::  ",
	0x51: u":: :",
	0x52: u":: @",
	0x53: u":: \n",
	0x54: u"::: ",
	0x55: u"::::",
	0x56: u":::@",
	0x57: u":::\n",
	0x58: u"::@ ",
	0x59: u"::@:",
	0x5a: u"::@@",
	0x5b: u"::@\n",
	0x5c: u"::\n ",
	0x5d: u"::\n:",
	0x5e: u"::\n@",
	0x5f: u"::\n\n",
	0x60: u":@  ",
	0x61: u":@ :",
	0x62: u":@ @",
	0x63: u":@ \n",
	0x64: u":@: ",
	0x65: u":@::",
	0x66: u":@:@",
	0x67: u":@:\n",
	0x68: u":@@ ",
	0x69: u":@@:",
	0x6a: u":@@@",
	0x6b: u":@@\n",
	0x6c: u":@\n ",
	0x6d: u":@\n:",
	0x6e: u":@\n@",
	0x6f: u":@\n\n",
	0x70: u":\n  ",
	0x71: u":\n :",
	0x72: u":\n @",
	0x73: u":\n \n",
	0x74: u":\n: ",
	0x75: u":\n::",
	0x76: u":\n:@",
	0x77: u":\n:\n",
	0x78: u":\n@ ",
	0x79: u":\n@:",
	0x7a: u":\n@@",
	0x7b: u":\n@\n",
	0x7c: u":\n\n ",
	0x7d: u":\n\n:",
	0x7e: u":\n\n@",
	0x7f: u":\n\n\n",
	0x80: u"@   ",
	0x81: u"@  :",
	0x82: u"@  @",
	0x83: u"@  \n",
	0x84: u"@ : ",
	0x85: u"@ ::",
	0x86: u"@ :@",
	0x87: u"@ :\n",
	0x88: u"@ @ ",
	0x89: u"@ @:",
	0x8a: u"@ @@",
	0x8b: u"@ @\n",
	0x8c: u"@ \n ",
	0x8d: u"@ \n:",
	0x8e: u"@ \n@",
	0x8f: u"@ \n\n",
	0x90: u"@:  ",
	0x91: u"@: :",
	0x92: u"@: @",
	0x93: u"@: \n",
	0x94: u"@:: ",
	0x95: u"@:::",
	0x96: u"@::@",
	0x97: u"@::\n",
	0x98: u"@:@ ",
	0x99: u"@:@:",
	0x9a: u"@:@@",
	0x9b: u"@:@\n",
	0x9c: u"@:\n ",
	0x9d: u"@:\n:",
	0x9e: u"@:\n@",
	0x9f: u"@:\n\n",
	0xa0: u"@@  ",
	0xa1: u"@@ :",
	0xa2: u"@@ @",
	0xa3: u"@@ \n",
	0xa4: u"@@: ",
	0xa5: u"@@::",
	0xa6: u"@@:@",
	0xa7: u"@@:\n",
	0xa8: u"@@@ ",
	0xa9: u"@@@:",
	0xaa: u"@@@@",
	0xab: u"@@@\n",
	0xac: u"@@\n ",
	0xad: u"@@\n:",
	0xae: u"@@\n@",
	0xaf: u"@@\n\n",
	0xb0: u"@\n  ",
	0xb1: u"@\n :",
	0xb2: u"@\n @",
	0xb3: u"@\n \n",
	0xb4: u"@\n: ",
	0xb5: u"@\n::",
	0xb6: u"@\n:@",
	0xb7: u"@\n:\n",
	0xb8: u"@\n@ ",
	0xb9: u"@\n@:",
	0xba: u"@\n@@",
	0xbb: u"@\n@\n",
	0xbc: u"@\n\n ",
	0xbd: u"@\n\n:",
	0xbe: u"@\n\n@",
	0xbf: u"@\n\n\n",
	0xc0: u"\n   ",
	0xc1: u"\n  :",
	0xc2: u"\n  @",
	0xc3: u"\n  \n",
	0xc4: u"\n : ",
	0xc5: u"\n ::",
	0xc6: u"\n :@",
	0xc7: u"\n :\n",
	0xc8: u"\n @ ",
	0xc9: u"\n @:",
	0xca: u"\n @@",
	0xcb: u"\n @\n",
	0xcc: u"\n \n ",
	0xcd: u"\n \n:",
	0xce: u"\n \n@",
	0xcf: u"\n \n\n",
	0xd0: u"\n:  ",
	0xd1: u"\n: :",
	0xd2: u"\n: @",
	0xd3: u"\n: \n",
	0xd4: u"\n:: ",
	0xd5: u"\n:::",
	0xd6: u"\n::@",
	0xd7: u"\n::\n",
	0xd8: u"\n:@ ",
	0xd9: u"\n:@:",
	0xda: u"\n:@@",
	0xdb: u"\n:@\n",
	0xdc: u"\n:\n ",
	0xdd: u"\n:\n:",
	0xde: u"\n:\n@",
	0xdf: u"\n:\n\n",
	0xe0: u"\n@  ",
	0xe1: u"\n@ :",
	0xe2: u"\n@ @",
	0xe3: u"\n@ \n",
	0xe4: u"\n@: ",
	0xe5: u"\n@::",
	0xe6: u"\n@:@",
	0xe7: u"\n@:\n",
	0xe8: u"\n@@ ",
	0xe9: u"\n@@:",
	0xea: u"\n@@@",
	0xeb: u"\n@@\n",
	0xec: u"\n@\n ",
	0xed: u"\n@\n:",
	0xee: u"\n@\n@",
	0xef: u"\n@\n\n",
	0xf0: u"\n\n  ",
	0xf1: u"\n\n :",
	0xf2: u"\n\n @",
	0xf3: u"\n\n \n",
	0xf4: u"\n\n: ",
	0xf5: u"\n\n::",
	0xf6: u"\n\n:@",
	0xf7: u"\n\n:\n",
	0xf8: u"\n\n@ ",
	0xf9: u"\n\n@:",
	0xfa: u"\n\n@@",
	0xfb: u"\n\n@\n",
	0xfc: u"\n\n\n ",
	0xfd: u"\n\n\n:",
	0xfe: u"\n\n\n@",
	0xff: u"\n\n\n\n",
}

encode_dict = {
	"": 0x00,
	": @:": 0x49,
	"\n@\n@": 0xee,
	"\n\n:\n": 0xf7,
	": \n ": 0x4c,
	"\n: ": 0xd0,
	"@ :@": 0x86,
	"@:\n:": 0x9d,
	": ": 0x40,
	":@ :": 0x61,
	"@\n@@": 0xba,
	":: @": 0x52,
	" ::@": 0x16,
	" :  ": 0x10,
	"::": 0x50,
	"\n::@": 0xd6,
	"\n@\n:": 0xed,
	": @ ": 0x48,
	": \n:": 0x4d,
	":\n@ ": 0x78,
	"\n :": 0xc4,
	":@@:": 0x69,
	" ": 0x00,
	" @": 0x20,
	" @@": 0x28,
	" @@@": 0x2a,
	"\n:": 0xd0,
	"\n @ ": 0xc8,
	":\n@:": 0x79,
	" \n ": 0x30,
	" @\n ": 0x2c,
	":  \n": 0x43,
	" \n@@": 0x3a,
	" \n:": 0x34,
	" @\n:": 0x2d,
	"\n \n@": 0xce,
	" \n :": 0x31,
	"\n\n@:": 0xf9,
	" :: ": 0x14,
	"\n@:": 0xe4,
	":@@ ": 0x68,
	"\n @@": 0xca,
	"@ ::": 0x85,
	"@@:\n": 0xa7,
	"@@@\n": 0xab,
	":\n\n\n": 0x7f,
	"  ": 0x00,
	" @ ": 0x20,
	" @@ ": 0x28,
	"@ : ": 0x84,
	"\n:@": 0xd8,
	"\n@::": 0xe5,
	" \n@": 0x38,
	" @\n@": 0x2e,
	"\n@ ": 0xe0,
	" \n@ ": 0x38,
	":\n\n@": 0x7e,
	"\n: @": 0xd2,
	"\n\n@@": 0xfa,
	" \n\n\n": 0x3f,
	":\n:\n": 0x77,
	"@   ": 0x80,
	" :": 0x10,
	" @:": 0x24,
	":@@@": 0x6a,
	"@\n  ": 0xb0,
	"\n\n @": 0xf2,
	" @@:": 0x29,
	": \n@": 0x4e,
	"@\n:@": 0xb6,
	"\n\n \n": 0xf3,
	"\n@ \n": 0xe3,
	" : \n": 0x13,
	":: :": 0x51,
	":@": 0x60,
	"\n:::": 0xd5,
	"\n  \n": 0xc3,
	"@\n@ ": 0xb8,
	"::  ": 0x50,
	"@\n \n": 0xb3,
	":: \n": 0x53,
	"\n\n\n": 0xfc,
	"@  :": 0x81,
	"\n:: ": 0xd4,
	"\n": 0xc0,
	": @@": 0x4a,
	"@:\n": 0x9c,
	"  :\n": 0x07,
	"@@\n\n": 0xaf,
	"@\n :": 0xb1,
	"@@": 0xa0,
	"  @": 0x08,
	"    ": 0x00,
	"\n\n@\n": 0xfb,
	"\n \n\n": 0xcf,
	"\n \n:": 0xcd,
	":@@": 0x68,
	"::\n ": 0x5c,
	"  \n ": 0x0c,
	"\n@\n ": 0xec,
	" :\n@": 0x1e,
	": ::": 0x45,
	"@\n:": 0xb4,
	"  \n:": 0x0d,
	"\n@:@": 0xe6,
	": : ": 0x44,
	"\n@@\n": 0xeb,
	"\n\n:": 0xf4,
	"@  @": 0x82,
	"\n  @": 0xc2,
	"\n:\n@": 0xde,
	"   :": 0x01,
	"\n:\n": 0xdc,
	"::\n\n": 0x5f,
	":\n \n": 0x73,
	"\n@ :": 0xe1,
	" ::\n": 0x17,
	"::::": 0x55,
	"\n :@": 0xc6,
	" :@@": 0x1a,
	"\n: \n": 0xd3,
	" : :": 0x11,
	"@\n\n\n": 0xbf,
	"::: ": 0x54,
	"\n \n": 0xcc,
	"@::@": 0x96,
	"@\n": 0xb0,
	":@\n:": 0x6d,
	"\n\n::": 0xf5,
	"  \n": 0x0c,
	" @ \n": 0x23,
	":@\n@": 0x6e,
	"@:::": 0x95,
	"@@  ": 0xa0,
	"\n@\n\n": 0xef,
	"@\n:\n": 0xb7,
	"::\n@": 0x5e,
	" :::": 0x15,
	" : @": 0x12,
	": @\n": 0x4b,
	"\n@:\n": 0xe7,
	" :\n\n": 0x1f,
	" :@ ": 0x18,
	":::@": 0x56,
	"@:: ": 0x94,
	"@@\n ": 0xac,
	":@:\n": 0x67,
	"@ :\n": 0x87,
	"@@ :": 0xa1,
	":@\n\n": 0x6f,
	"\n  :": 0xc1,
	"@ ": 0x80,
	"   ": 0x00,
	" @  ": 0x20,
	"\n :\n": 0xc7,
	"\n:  ": 0xd0,
	":@ ": 0x60,
	" \n \n": 0x33,
	"  \n@": 0x0e,
	" :\n ": 0x1c,
	":@::": 0x65,
	":\n@@": 0x7a,
	":@\n": 0x6c,
	":@:": 0x64,
	": :@": 0x46,
	"\n\n  ": 0xf0,
	" :\n:": 0x1d,
	"@:": 0x90,
	" :@:": 0x19,
	"@ @\n": 0x8b,
	"\n   ": 0xc0,
	":\n\n:": 0x7d,
	" @ :": 0x21,
	"  @\n": 0x0b,
	": \n\n": 0x4f,
	"@:\n\n": 0x9f,
	"\n: :": 0xd1,
	"@@@@": 0xaa,
	":::": 0x54,
	": \n": 0x4c,
	"@ \n@": 0x8e,
	":\n ": 0x70,
	" \n\n@": 0x3e,
	"@:@": 0x98,
	":@ \n": 0x63,
	"::@\n": 0x5b,
	"\n : ": 0xc4,
	":@: ": 0x64,
	":\n:": 0x74,
	" \n": 0x30,
	" @ @": 0x22,
	":@  ": 0x60,
	" \n  ": 0x30,
	"\n@\n": 0xec,
	" \n\n": 0x3c,
	" @\n\n": 0x2f,
	":: ": 0x50,
	":  :": 0x41,
	":\n\n ": 0x7c,
	"@:@ ": 0x98,
	"@\n\n": 0xbc,
	":\n :": 0x71,
	" \n:\n": 0x37,
	"  :@": 0x06,
	"@@\n@": 0xae,
	":@@\n": 0x6b,
	"@: @": 0x92,
	"@\n::": 0xb5,
	"::@@": 0x5a,
	"@:@:": 0x99,
	"@ \n\n": 0x8f,
	":\n::": 0x75,
	"@\n@:": 0xb9,
	":   ": 0x40,
	":": 0x40,
	":\n  ": 0x70,
	"  \n\n": 0x0f,
	"@@ @": 0xa2,
	"\n@@@": 0xea,
	"@@:": 0xa4,
	"\n@": 0xe0,
	"\n:@:": 0xd9,
	"\n\n: ": 0xf4,
	"\n@@:": 0xe9,
	"@\n: ": 0xb4,
	"@: \n": 0x93,
	"  : ": 0x04,
	":\n\n": 0x7c,
	"@: ": 0x90,
	"@:  ": 0x90,
	"  ::": 0x05,
	"@@\n:": 0xad,
	"@ @": 0x88,
	"@: :": 0x91,
	"\n @\n": 0xcb,
	"@::": 0x94,
	"   @": 0x02,
	":  @": 0x42,
	":\n @": 0x72,
	"@@ \n": 0xa3,
	"\n@ @": 0xe2,
	"@@@ ": 0xa8,
	"\n @": 0xc8,
	"\n @:": 0xc9,
	"\n\n\n@": 0xfe,
	"\n:\n\n": 0xdf,
	"\n@@": 0xe8,
	"\n\n ": 0xf0,
	"@ \n ": 0x8c,
	":\n@": 0x78,
	" \n\n ": 0x3c,
	"@\n @": 0xb2,
	"\n  ": 0xc0,
	"@ \n:": 0x8d,
	":\n: ": 0x74,
	" \n\n:": 0x3d,
	"@:@\n": 0x9b,
	"\n\n\n\n": 0xff,
	":@:@": 0x66,
	":@ @": 0x62,
	"@\n@": 0xb8,
	"@@@:": 0xa9,
	"::@": 0x58,
	":::\n": 0x57,
	"\n\n:@": 0xf6,
	"@ @@": 0x8a,
	"@": 0x80,
	":  ": 0x40,
	"@@: ": 0xa4,
	"@::\n": 0x97,
	"\n:\n ": 0xdc,
	"\n \n ": 0xcc,
	"@\n\n ": 0xbc,
	"@@::": 0xa5,
	"\n:@ ": 0xd8,
	"\n::": 0xd4,
	": :": 0x44,
	"@\n\n:": 0xbd,
	"\n:\n:": 0xdd,
	"@ :": 0x84,
	":\n@\n": 0x7b,
	" :\n": 0x1c,
	" @:\n": 0x27,
	"@ \n": 0x8c,
	"   \n": 0x03,
	" \n::": 0x35,
	"  @ ": 0x08,
	"\n\n\n ": 0xfc,
	"\n@: ": 0xe4,
	"\n\n@": 0xf8,
	" @: ": 0x24,
	" : ": 0x10,
	": :\n": 0x47,
	" @::": 0x25,
	"@ @ ": 0x88,
	"\n@  ": 0xe0,
	"@  ": 0x80,
	"@ @:": 0x89,
	" ::": 0x14,
	" \n: ": 0x34,
	"\n ::": 0xc5,
	":\n": 0x70,
	" \n@\n": 0x3b,
	"  @:": 0x09,
	"\n::\n": 0xd7,
	"@@@": 0xa8,
	"  @@": 0x0a,
	"@:\n@": 0x9e,
	":@\n ": 0x6c,
	"\n\n :": 0xf1,
	" :@\n": 0x1b,
	" @:@": 0x26,
	" :@": 0x18,
	"@:\n ": 0x9c,
	"\n:@\n": 0xdb,
	"::\n:": 0x5d,
	"@@ ": 0xa0,
	"@:@@": 0x9a,
	"@\n ": 0xb0,
	"  :": 0x04,
	" \n:@": 0x36,
	" \n @": 0x32,
	" @\n": 0x2c,
	" @@\n": 0x2b,
	"::@ ": 0x58,
	"\n@@ ": 0xe8,
	": @": 0x48,
	"@@:@": 0xa6,
	"\n\n\n:": 0xfd,
	"::@:": 0x59,
	"@\n\n@": 0xbe,
	"\n\n@ ": 0xf8,
	"\n:@@": 0xda,
	":\n:@": 0x76,
	"::\n": 0x5c,
	"@@\n": 0xac,
	"\n ": 0xc0,
	"@  \n": 0x83,
	"\n\n": 0xf0,
	"@\n@\n": 0xbb,
	" \n@:": 0x39,
}
