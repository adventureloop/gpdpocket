/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20170303 (64-bit version)
 * Copyright (c) 2000 - 2017 Intel Corporation
 * 
 * Disassembly of tpm2.dat, Fri May  4 21:59:37 2018
 *
 * ACPI Data Table [TPM2]
 *
 * Format: [HexOffset DecimalOffset ByteLength]  FieldName : FieldValue
 */

[000h 0000   4]                    Signature : "TPM2"    [Trusted Platform Module hardware interface table]
[004h 0004   4]                 Table Length : 00000034
[008h 0008   1]                     Revision : 03
[009h 0009   1]                     Checksum : 57
[00Ah 0010   6]                       Oem ID : ""
[010h 0016   8]                 Oem Table ID : "Tpm2Tabl"
[018h 0024   4]                 Oem Revision : 00000001
[01Ch 0028   4]              Asl Compiler ID : "AMI "
[020h 0032   4]        Asl Compiler Revision : 00000000

[024h 0036   2]               Platform Class : 0000
[026h 0038   2]                     Reserved : 0000
[028h 0040   8]              Control Address : 000000007FF00000
[030h 0048   4]                 Start Method : 00000002

Raw Table Data: Length 52 (0x34)

  0000: 54 50 4D 32 34 00 00 00 03 57 00 00 00 00 00 00  // TPM24....W......
  0010: 54 70 6D 32 54 61 62 6C 01 00 00 00 41 4D 49 20  // Tpm2Tabl....AMI 
  0020: 00 00 00 00 00 00 00 00 00 00 F0 7F 00 00 00 00  // ................
  0030: 02 00 00 00                                      // ....
