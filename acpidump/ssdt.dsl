/*
 * Intel ACPI Component Architecture
 * AML/ASL+ Disassembler version 20170303 (64-bit version)
 * Copyright (c) 2000 - 2017 Intel Corporation
 * 
 * Disassembling to symbolic ASL+ operators
 *
 * Disassembly of ssdt.dat, Fri May  4 21:59:37 2018
 *
 * Original Table Header:
 *     Signature        "SSDT"
 *     Length           0x0000415C (16732)
 *     Revision         0x01
 *     Checksum         0x88
 *     OEM ID           "DptfTb"
 *     OEM Table ID     "DptfTab"
 *     OEM Revision     0x00001000 (4096)
 *     Compiler ID      "INTL"
 *     Compiler Version 0x20120913 (538052883)
 */
DefinitionBlock ("", "SSDT", 1, "DptfTb", "DptfTab", 0x00001000)
{
    /*
     * iASL Warning: There were 2 external control methods found during
     * disassembly, but only 0 were resolved (2 unresolved). Additional
     * ACPI tables may be required to properly disassemble the code. This
     * resulting disassembler output file may not compile because the
     * disassembler did not know how many arguments to assign to the
     * unresolved methods. Note: SSDTs can be dynamically loaded at
     * runtime and may or may not be available via the host OS.
     *
     * To specify the tables needed to resolve external control method
     * references, the -e option can be used to specify the filenames.
     * Example iASL invocations:
     *     iasl -e ssdt1.aml ssdt2.aml ssdt3.aml -d dsdt.aml
     *     iasl -e dsdt.aml ssdt2.aml -d ssdt1.aml
     *     iasl -e ssdt*.aml -d dsdt.aml
     *
     * In addition, the -fe option can be used to specify a file containing
     * control method external declarations with the associated method
     * argument counts. Each line of the file must be of the form:
     *     External (<method pathname>, MethodObj, <argument count>)
     * Invocation:
     *     iasl -fe refs.txt -d dsdt.aml
     *
     * The following methods were unresolved and many not compile properly
     * because the disassembler had to guess at the number of arguments
     * required for each:
     */
    External (_SB_.ADP1._PSR, UnknownObj)
    External (_SB_.PCI0.GFX0.DD01._BCM, MethodObj)    // Warning: Unknown method, guessing 1 arguments
    External (_SB_.PCI0.GFX0.DD01._BQC, IntObj)
    External (_SB_.PCI0.GFX0.DD01._DCS, IntObj)
    External (_SB_.PCI0.I2C1, UnknownObj)
    External (_SB_.PCI0.I2C1.AVBL, UnknownObj)
    External (_SB_.PCI0.I2C1.BATC, UnknownObj)
    External (_SB_.PCI0.I2C1.BATC._BST, IntObj)
    External (_SB_.PCI0.I2C1.BATC.ARTG, IntObj)
    External (_SB_.PCI0.I2C1.BATC.BCCC, UnknownObj)
    External (_SB_.PCI0.I2C1.BATC.BCCE, UnknownObj)
    External (_SB_.PCI0.I2C1.BATC.CTYP, IntObj)
    External (_SB_.PCI0.I2C1.BATC.PMAX, IntObj)
    External (_SB_.PCI0.I2C1.BATC.PSOC, IntObj)
    External (_SB_.PCI0.I2C1.BATC.PSRC, IntObj)
    External (_SB_.PCI0.I2C1.THRM, IntObj)
    External (_SB_.PCI0.I2C3, UnknownObj)
    External (_SB_.PCI0.I2C3.TIDR, UnknownObj)
    External (_SB_.PCI0.I2C3.TIDR.ARTG, IntObj)
    External (_SB_.PCI0.I2C3.TIDR.BATD, IntObj)
    External (_SB_.PCI0.I2C3.TIDR.BCCC, UnknownObj)
    External (_SB_.PCI0.I2C3.TIDR.BCCE, UnknownObj)
    External (_SB_.PCI0.I2C3.TIDR.CTYP, IntObj)
    External (_SB_.PCI0.I2C3.TIDR.PMAX, IntObj)
    External (_SB_.PCI0.I2C3.TIDR.PSOC, IntObj)
    External (_SB_.PCI0.I2C3.TIDR.PSRC, IntObj)
    External (_SB_.PCI0.I2C7, UnknownObj)
    External (_SB_.PCI0.I2C7.BATC, UnknownObj)
    External (_SB_.PCI0.I2C7.BATC._BST, IntObj)
    External (_SB_.PCI0.I2C7.BATC.ARTG, IntObj)
    External (_SB_.PCI0.I2C7.BATC.BCCC, UnknownObj)
    External (_SB_.PCI0.I2C7.BATC.BCCE, UnknownObj)
    External (_SB_.PCI0.I2C7.BATC.CTYP, IntObj)
    External (_SB_.PCI0.I2C7.BATC.PMAX, IntObj)
    External (_SB_.PCI0.I2C7.BATC.PSOC, IntObj)
    External (_SB_.PCI0.I2C7.BATC.PSRC, IntObj)
    External (_SB_.PCI0.I2C7.BMDR, UnknownObj)
    External (_SB_.PCI0.I2C7.BMDR.BCCC, UnknownObj)
    External (_SB_.PCI0.I2C7.BMDR.BCCE, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1.AVBD, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1.AVBG, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1.AX00, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1.AX01, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1.GEN0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1.GMP0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1.GMP3, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1.GX00, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1.GX01, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1.PEN0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI1.TMP0, IntObj)
    External (_SB_.PCI0.I2C7.PMI1.TMP3, IntObj)
    External (_SB_.PCI0.I2C7.PMI2, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI2.AVBD, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI2.AVBG, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI2.AX00, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI2.AX01, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI2.GEN0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI2.GMP0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI2.GMP3, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI2.GX00, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI2.GX01, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI2.PEN0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI2.TMP0, IntObj)
    External (_SB_.PCI0.I2C7.PMI2.TMP3, IntObj)
    External (_SB_.PCI0.I2C7.PMI5, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.AVBD, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.AVBG, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.AX00, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.AX01, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.AX10, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.AX11, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.AX20, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.AX21, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.AX30, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.AX31, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GEN0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GEN1, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GEN2, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GMP0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GMP1, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GMP2, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GMP3, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GX00, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GX01, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GX10, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GX11, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GX20, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GX21, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GX30, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.GX31, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.PEN0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.PEN1, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.PEN2, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.SRCD, UnknownObj)
    External (_SB_.PCI0.I2C7.PMI5.TMP0, IntObj)
    External (_SB_.PCI0.I2C7.PMI5.TMP1, IntObj)
    External (_SB_.PCI0.I2C7.PMI5.TMP2, IntObj)
    External (_SB_.PCI0.I2C7.PMI5.TMP3, IntObj)
    External (_SB_.PCI0.I2C7.PMIC, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.AVBD, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.AVBG, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.AX00, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.AX01, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.AX10, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.AX11, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.AX20, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.AX21, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GEN0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GEN1, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GEN2, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GMP0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GMP1, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GMP2, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GMP3, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GX00, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GX01, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GX10, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GX11, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GX20, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.GX21, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.PEN0, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.PEN1, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.PEN2, UnknownObj)
    External (_SB_.PCI0.I2C7.PMIC.TMP0, IntObj)
    External (_SB_.PCI0.I2C7.PMIC.TMP1, IntObj)
    External (_SB_.PCI0.I2C7.PMIC.TMP2, IntObj)
    External (_SB_.PCI0.I2C7.PMIC.TMP3, IntObj)
    External (_SB_.PCI0.I2C7.WIDR, UnknownObj)
    External (_SB_.PCI0.I2C7.WIDR.ARTG, IntObj)
    External (_SB_.PCI0.I2C7.WIDR.BATD, IntObj)
    External (_SB_.PCI0.I2C7.WIDR.BCCC, UnknownObj)
    External (_SB_.PCI0.I2C7.WIDR.BCCE, UnknownObj)
    External (_SB_.PCI0.I2C7.WIDR.CTYP, IntObj)
    External (_SB_.PCI0.I2C7.WIDR.PMAX, IntObj)
    External (_SB_.PCI0.I2C7.WIDR.PSOC, IntObj)
    External (_SB_.PCI0.I2C7.WIDR.PSRC, IntObj)
    External (_SB_.PCI0.PNIT, UnknownObj)
    External (_SB_.PST0, IntObj)
    External (_SB_.PST2, IntObj)
    External (_SB_.WPPT, IntObj)
    External (ABC0, IntObj)
    External (ABC1, IntObj)
    External (ABC2, IntObj)
    External (ABC3, IntObj)
    External (ABC4, IntObj)
    External (ABC5, IntObj)
    External (ABS0, IntObj)
    External (ABS1, IntObj)
    External (ABS2, IntObj)
    External (ABS3, IntObj)
    External (ABS4, IntObj)
    External (ABS5, IntObj)
    External (ADBG, MethodObj)    // Warning: Unknown method, guessing 1 arguments
    External (AMTE, UnknownObj)
    External (BDID, UnknownObj)
    External (CHGR, UnknownObj)
    External (DDSP, UnknownObj)
    External (DPHL, IntObj)
    External (DPLL, IntObj)
    External (DPTE, UnknownObj)
    External (DSOC, UnknownObj)
    External (GCR0, UnknownObj)
    External (GCR2, UnknownObj)
    External (GCR3, UnknownObj)
    External (HPME, UnknownObj)
    External (OSID, UnknownObj)
    External (PDBG, IntObj)
    External (PDPM, UnknownObj)
    External (PMID, UnknownObj)
    External (PST3, UnknownObj)
    External (THM0, UnknownObj)
    External (THM1, UnknownObj)
    External (THM2, UnknownObj)
    External (THM4, UnknownObj)
    External (VTSP, IntObj)
    External (WCR0, UnknownObj)
    External (WWEN, UnknownObj)

    Name (BDLI, 0x28)
    Name (BDHI, 0x3C)
    Name (PSA0, Zero)
    Name (PSC0, Zero)
    Name (PSA2, Zero)
    Name (PSC2, Zero)
    Name (SCPE, One)
    Name (WAN1, Zero)
    Scope (\_SB)
    {
        Device (DPTF)
        {
            Name (_HID, EisaId ("INT3400") /* Intel Dynamic Power Performance Management */)  // _HID: Hardware ID
            Name (RDEP, Package (0x01)
            {
                \_SB.PCI0.I2C1
            })
            Name (ODEP, Package (0x01)
            {
                \_SB.PCI0.I2C1
            })
            Name (FDEP, Package (0x02)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI5
            })
            Name (DPSP, Package (0x01)
            {
                ToUUID ("42a441d6-ae6a-462b-a84b-4a8ce79027d3")
            })
            Name (DCSP, Package (0x01)
            {
                ToUUID ("97c68ae7-15fa-499c-b8c9-5da81d606e0a")
            })
            Name (DCPP, Package (0x01)
            {
                ToUUID ("16caf1b7-dd38-40ed-b1c1-1b8a1913d531")
            })
            Name (VSPP, Package (0x01)
            {
                ToUUID ("6ed722a7-9240-48a5-b479-31eef723d7cf")
            })
            Name (DBPP, Package (0x01)
            {
                ToUUID ("64568ccd-6597-4bfc-b9d6-9d33854013ce")
            })
            Name (LPMT, Package (0x02)
            {
                One, 
                Package (0x06)
                {
                    \_SB.PCI0.PNIT, 
                    Zero, 
                    0x00010000, 
                    0x3A98, 
                    Zero, 
                    Zero
                }
            })
            Name (TMPP, Package (0x06)
            {
                Buffer (0x10)
                {
                    /* 0000 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* ........ */
                    /* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   /* ........ */
                }, 

                Buffer (0x10)
                {
                    /* 0000 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* ........ */
                    /* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   /* ........ */
                }, 

                Buffer (0x10)
                {
                    /* 0000 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* ........ */
                    /* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   /* ........ */
                }, 

                Buffer (0x10)
                {
                    /* 0000 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* ........ */
                    /* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   /* ........ */
                }, 

                Buffer (0x10)
                {
                    /* 0000 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* ........ */
                    /* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   /* ........ */
                }, 

                Buffer (0x10)
                {
                    /* 0000 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* ........ */
                    /* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   /* ........ */
                }
            })
            Method (IDSP, 0, Serialized)
            {
                Name (TMPI, Zero)
                If ((PDPM == One))
                {
                    TMPP [TMPI] = DerefOf (DPSP [Zero])
                    TMPI++
                    TMPP [TMPI] = DerefOf (DCSP [Zero])
                }

                Return (TMPP) /* \_SB_.DPTF.TMPP */
            }

            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If ((DPTE == Zero))
                {
                    Return (Zero)
                }

                Return (0x0F)
            }

            Name (PDRI, Package (0x01)
            {
                Package (0x04)
                {
                    0x0A, 
                    \_SB.PCI0.PNIT, 
                    Zero, 
                    Package (0x02)
                    {
                        0x00010002, 
                        Zero
                    }
                }
            })
            Name (PDRA, Package (0x02)
            {
                Package (0x04)
                {
                    0x64, 
                    \_SB.PCI0.PNIT, 
                    Zero, 
                    Package (0x04)
                    {
                        0x00010000, 
                        0x1770, 
                        0x00010002, 
                        One
                    }
                }, 

                Package (0x04)
                {
                    0x64, 
                    \_SB.TDSP, 
                    0x0A, 
                    Package (0x02)
                    {
                        0x00050000, 
                        0x64
                    }
                }
            })
            Name (CDRI, Package (0x01)
            {
                Package (0x04)
                {
                    0x0A, 
                    \_SB.PCI0.PNIT, 
                    Zero, 
                    Package (0x02)
                    {
                        0x00070000, 
                        One
                    }
                }
            })
            Name (CDRA, Package (0x01)
            {
                Package (0x04)
                {
                    0x64, 
                    \_SB.PCI0.PNIT, 
                    Zero, 
                    Package (0x02)
                    {
                        0x00070000, 
                        Zero
                    }
                }
            })
            Method (PDRT, 0, NotSerialized)
            {
                If ((BDID == 0x08))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.SRCD == Zero))
                    {
                        Return (PDRI) /* \_SB_.DPTF.PDRI */
                    }
                    Else
                    {
                        Return (PDRA) /* \_SB_.DPTF.PDRA */
                    }
                }
                ElseIf ((\_SB.ADP1._PSR == Zero))
                {
                    If (((BDID == One) || ((BDID == 0x09) || (BDID == 0x0A))))
                    {
                        Return (CDRI) /* \_SB_.DPTF.CDRI */
                    }

                    Return (PDRI) /* \_SB_.DPTF.PDRI */
                }
                Else
                {
                    If (((BDID == One) || ((BDID == 0x09) || (BDID == 0x0A))))
                    {
                        Return (CDRA) /* \_SB_.DPTF.CDRA */
                    }

                    Return (PDRA) /* \_SB_.DPTF.PDRA */
                }
            }

            Name (ETRM, Package (0x09)
            {
                Package (0x04)
                {
                    \_SB.PCI0.PNIT, 
                    "8086_22DC", 
                    Zero, 
                    "0x000B0000"
                }, 

                Package (0x04)
                {
                    \_SB.STR0, 
                    "INT3403", 
                    0x06, 
                    "0"
                }, 

                Package (0x04)
                {
                    \_SB.STR1, 
                    "INT3403", 
                    0x06, 
                    "1"
                }, 

                Package (0x04)
                {
                    \_SB.STR2, 
                    "INT3403", 
                    0x06, 
                    "2"
                }, 

                Package (0x04)
                {
                    \_SB.TDSP, 
                    "INT3406", 
                    0x06, 
                    "0"
                }, 

                Package (0x04)
                {
                    \_SB.TCHG, 
                    "INT3403", 
                    0x06, 
                    "4"
                }, 

                Package (0x04)
                {
                    \_SB.TPWR, 
                    "INT3407", 
                    0x06, 
                    "0"
                }, 

                Package (0x04)
                {
                    \_SB.SAMB, 
                    "INT3409", 
                    0x06, 
                    "0"
                }, 

                Package (0x04)
                {
                    \_SB.WWAN, 
                    "INT3408", 
                    0x06, 
                    "0"
                }
            })
            Name (CTRM, Package (0x04)
            {
                Package (0x04)
                {
                    \_SB.PCI0.PNIT, 
                    "8086_22DC", 
                    Zero, 
                    "0x000B0000"
                }, 

                Package (0x04)
                {
                    \_SB.STR0, 
                    "INT3403", 
                    0x06, 
                    "0"
                }, 

                Package (0x04)
                {
                    \_SB.TDSP, 
                    "INT3406", 
                    0x06, 
                    "0"
                }, 

                Package (0x04)
                {
                    \_SB.TCHG, 
                    "INT3403", 
                    0x06, 
                    "4"
                }
            })
            Name (TRTI, Package (0x03)
            {
                Package (0x08)
                {
                    \_SB.TCHG, 
                    \_SB.STR0, 
                    0x64, 
                    0x02EE, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero
                }, 

                Package (0x08)
                {
                    \_SB.PCI0.PNIT, 
                    \_SB.STR0, 
                    0x50, 
                    0x02EE, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero
                }, 

                Package (0x08)
                {
                    \_SB.TDSP, 
                    \_SB.STR2, 
                    0x64, 
                    0x64, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero
                }
            })
            Name (TRTC, Package (0x03)
            {
                Package (0x08)
                {
                    \_SB.TCHG, 
                    \_SB.STR0, 
                    0x64, 
                    0x02EE, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero
                }, 

                Package (0x08)
                {
                    \_SB.PCI0.PNIT, 
                    \_SB.STR0, 
                    0x50, 
                    0x02EE, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero
                }, 

                Package (0x08)
                {
                    \_SB.TDSP, 
                    \_SB.STR0, 
                    0x3C, 
                    0x64, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero
                }
            })
            Name (TRTH, Package (0x03)
            {
                Package (0x08)
                {
                    \_SB.TCHG, 
                    \_SB.STR2, 
                    0x5A, 
                    0x02EE, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero
                }, 

                Package (0x08)
                {
                    \_SB.PCI0.PNIT, 
                    \_SB.STR0, 
                    0x50, 
                    0x02EE, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero
                }, 

                Package (0x08)
                {
                    \_SB.TDSP, 
                    \_SB.STR2, 
                    0x64, 
                    0x64, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero
                }
            })
            Method (_TRT, 0, NotSerialized)  // _TRT: Thermal Relationship Table
            {
                If ((BDID == One))
                {
                    Return (TRTC) /* \_SB_.DPTF.TRTC */
                }
                ElseIf ((BDID == 0x0A))
                {
                    Return (TRTH) /* \_SB_.DPTF.TRTH */
                }

                Return (TRTI) /* \_SB_.DPTF.TRTI */
            }

            Name (TRTR, One)
            Method (_OSC, 4, Serialized)  // _OSC: Operating System Capabilities
            {
                Name (NUMP, Zero)
                Name (UID2, Buffer (0x10)
                {
                    /* 0000 */  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  /* ........ */
                    /* 0008 */  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF   /* ........ */
                })
                CreateDWordField (Arg3, Zero, STS1)
                CreateDWordField (Arg3, 0x04, CAP1)
                IDSP ()
                NUMP = SizeOf (TMPP)
                CreateDWordField (Arg0, Zero, IID0)
                CreateDWordField (Arg0, 0x04, IID1)
                CreateDWordField (Arg0, 0x08, IID2)
                CreateDWordField (Arg0, 0x0C, IID3)
                CreateDWordField (UID2, Zero, EID0)
                CreateDWordField (UID2, 0x04, EID1)
                CreateDWordField (UID2, 0x08, EID2)
                CreateDWordField (UID2, 0x0C, EID3)
                While (NUMP)
                {
                    UID2 = DerefOf (TMPP [(NUMP - One)])
                    If ((((IID0 == EID0) && (IID1 == EID1)) && ((IID2 == 
                        EID2) && (IID3 == EID3))))
                    {
                        Break
                    }

                    NUMP--
                }

                If ((NUMP == Zero))
                {
                    STS1 &= 0xFFFFFF00
                    STS1 |= 0x06
                    Return (Arg3)
                }

                If ((Arg1 != One))
                {
                    STS1 &= 0xFFFFFF00
                    STS1 |= 0x0A
                    Return (Arg3)
                }

                If ((Arg2 != 0x02))
                {
                    STS1 &= 0xFFFFFF00
                    STS1 |= 0x02
                    Return (Arg3)
                }

                Return (Arg3)
            }

            Method (KTOC, 1, NotSerialized)
            {
                If ((Arg0 > 0x0AAC))
                {
                    Local1 = (Arg0 - 0x0AAC)
                }
                Else
                {
                    Local1 = (0x0AAC - Arg0)
                    Local1 = (0x0A00 - Local1)
                }

                Return ((Local1 / 0x0A))
            }

            Method (CTOK, 1, NotSerialized)
            {
                If ((Arg0 & 0x80))
                {
                    Local0 = (Arg0 & 0xFF)
                    Local0 = (0x0100 - Arg0)
                    Local0 *= 0x0A
                    Local0 = (0x0AAC - Local0)
                }
                Else
                {
                    Local0 = (Arg0 * 0x0A)
                    Local0 += 0x0AAC
                }

                Return (Local0)
            }

            Method (SDBG, 0, NotSerialized)
            {
                Return (PDBG) /* External reference */
            }
        }
    }

    Scope (\_SB)
    {
        Device (TPWR)
        {
            Name (_HID, EisaId ("INT3407") /* DPTF Platform Power Meter */)  // _HID: Hardware ID
            Name (_UID, Zero)  // _UID: Unique ID
            Name (PTYP, 0x11)
            Name (PROP, 0x07D0)
            Name (_STR, Unicode ("Platform Power"))  // _STR: Description String
            Name (RDEP, Package (0x02)
            {
                \_SB.PCI0.I2C1, 
                \_SB.PCI0.I2C1.BATC
            })
            Name (ODEP, Package (0x02)
            {
                \_SB.PCI0.I2C1, 
                \_SB.PCI0.I2C1.BATC
            })
            Name (FDEP, Package (0x03)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI5, 
                \_SB.PCI0.I2C7.BATC
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If (((DSOC == Zero) || (BDID == One)))
                {
                    Return (Zero)
                }

                Return (0x0F)
            }

            Method (_BST, 0, NotSerialized)  // _BST: Battery Status
            {
                If (CondRefOf (\_SB.PCI0.I2C1.BATC._BST, Local0))
                {
                    Return (\_SB.PCI0.I2C1.BATC._BST) /* External reference */
                }
                ElseIf (CondRefOf (\_SB.PCI0.I2C7.BATC._BST, Local0))
                {
                    Return (\_SB.PCI0.I2C7.BATC._BST) /* External reference */
                }
                ElseIf (CondRefOf (\_SB.PCI0.I2C7.WIDR.BATD, Local0))
                {
                    Return (\_SB.PCI0.I2C7.WIDR.BATD) /* External reference */
                }
                Else
                {
                    Return (Package (0x04)
                    {
                        Zero, 
                        Zero, 
                        Zero, 
                        Zero
                    })
                }

                If (CondRefOf (\_SB.PCI0.I2C1.BATC._BST, Local0))
                {
                    Return (\_SB.PCI0.I2C1.BATC._BST) /* External reference */
                }
                ElseIf (CondRefOf (\_SB.PCI0.I2C7.BATC._BST, Local0))
                {
                    Return (\_SB.PCI0.I2C7.BATC._BST) /* External reference */
                }
                ElseIf (CondRefOf (\_SB.PCI0.I2C7.WIDR.BATD, Local0))
                {
                    Return (\_SB.PCI0.I2C7.WIDR.BATD) /* External reference */
                }
                Else
                {
                    Return (Package (0x04)
                    {
                        Zero, 
                        Zero, 
                        Zero, 
                        Zero
                    })
                }

                If ((BDID == One))
                {
                    If (CondRefOf (\_SB.PCI0.I2C3.TIDR.BATD, Local0))
                    {
                        Return (\_SB.PCI0.I2C3.TIDR.BATD) /* External reference */
                    }
                    Else
                    {
                        Return (Package (0x04)
                        {
                            Zero, 
                            Zero, 
                            Zero, 
                            Zero
                        })
                    }
                }
                ElseIf ((BDID == 0x08))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.BATC._BST, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.BATC._BST) /* External reference */
                    }
                    Else
                    {
                        Return (Package (0x04)
                        {
                            Zero, 
                            Zero, 
                            Zero, 
                            Zero
                        })
                    }
                }
                ElseIf (((BDID == 0x09) || (BDID == 0x0A)))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.WIDR.BATD, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.WIDR.BATD) /* External reference */
                    }
                    Else
                    {
                        Return (Package (0x04)
                        {
                            Zero, 
                            Zero, 
                            Zero, 
                            Zero
                        })
                    }
                }
                ElseIf (CondRefOf (\_SB.PCI0.I2C1.BATC._BST, Local0))
                {
                    Return (\_SB.PCI0.I2C1.BATC._BST) /* External reference */
                }
                Else
                {
                    Return (Package (0x04)
                    {
                        Zero, 
                        Zero, 
                        Zero, 
                        Zero
                    })
                }
            }

            Method (PSOC, 0, NotSerialized)
            {
                If ((BDID == One))
                {
                    If (CondRefOf (\_SB.PCI0.I2C3.TIDR.PSOC, Local0))
                    {
                        Return (\_SB.PCI0.I2C3.TIDR.PSOC) /* External reference */
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf ((BDID == 0x08))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.BATC.PSOC, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.BATC.PSOC) /* External reference */
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf (((BDID == 0x09) || (BDID == 0x0A)))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.WIDR.PSOC, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.WIDR.PSOC) /* External reference */
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf (CondRefOf (\_SB.PCI0.I2C1.BATC.PSOC, Local0))
                {
                    Return (\_SB.PCI0.I2C1.BATC.PSOC) /* External reference */
                }
                Else
                {
                    Return (Zero)
                }
            }

            Method (PMAX, 0, NotSerialized)
            {
                If ((BDID == One))
                {
                    If (CondRefOf (\_SB.PCI0.I2C3.TIDR.PMAX, Local0))
                    {
                        Return (\_SB.PCI0.I2C3.TIDR.PMAX) /* External reference */
                    }
                }
                ElseIf ((BDID == 0x08))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.BATC.PMAX, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.BATC.PMAX) /* External reference */
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf (((BDID == 0x09) || (BDID == 0x0A)))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.WIDR.PMAX, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.WIDR.PMAX) /* External reference */
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf (CondRefOf (\_SB.PCI0.I2C1.BATC.PMAX, Local0))
                {
                    Return (\_SB.PCI0.I2C1.BATC.PMAX) /* External reference */
                }
                Else
                {
                    Return (Zero)
                }
            }

            Method (PSRC, 0, NotSerialized)
            {
                If ((BDID == One))
                {
                    If (CondRefOf (\_SB.PCI0.I2C3.TIDR.PSRC, Local0))
                    {
                        Return (\_SB.PCI0.I2C3.TIDR.PSRC) /* External reference */
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf ((BDID == 0x08))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.BATC.PSRC, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.BATC.PSRC) /* External reference */
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf (((BDID == 0x09) || (BDID == 0x0A)))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.WIDR.PSRC, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.WIDR.PSRC) /* External reference */
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf (CondRefOf (\_SB.PCI0.I2C1.BATC.PSRC, Local0))
                {
                    Return (\_SB.PCI0.I2C1.BATC.PSRC) /* External reference */
                }
                Else
                {
                    Return (Zero)
                }
            }

            Method (ARTG, 0, NotSerialized)
            {
                If ((BDID == One))
                {
                    If (CondRefOf (\_SB.PCI0.I2C3.TIDR.ARTG, Local0))
                    {
                        Return (\_SB.PCI0.I2C3.TIDR.ARTG) /* External reference */
                    }
                }
                ElseIf ((BDID == 0x08))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.BATC.ARTG, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.BATC.ARTG) /* External reference */
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf (((BDID == 0x09) || (BDID == 0x0A)))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.WIDR.ARTG, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.WIDR.ARTG) /* External reference */
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf (CondRefOf (\_SB.PCI0.I2C1.BATC.ARTG, Local0))
                {
                    Return (\_SB.PCI0.I2C1.BATC.ARTG) /* External reference */
                }
                Else
                {
                    Return (Zero)
                }
            }

            Method (CTYP, 0, NotSerialized)
            {
                If ((BDID == One))
                {
                    If (CondRefOf (\_SB.PCI0.I2C3.TIDR.CTYP, Local0))
                    {
                        Return (\_SB.PCI0.I2C3.TIDR.CTYP) /* External reference */
                    }
                }
                ElseIf ((BDID == 0x08))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.BATC.CTYP, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.BATC.CTYP) /* External reference */
                    }
                }
                ElseIf (((BDID == 0x09) || (BDID == 0x0A)))
                {
                    If (CondRefOf (\_SB.PCI0.I2C7.WIDR.CTYP, Local0))
                    {
                        Return (\_SB.PCI0.I2C7.WIDR.CTYP) /* External reference */
                    }
                }
                ElseIf (CondRefOf (\_SB.PCI0.I2C1.BATC.CTYP, Local0))
                {
                    Return (\_SB.PCI0.I2C1.BATC.CTYP) /* External reference */
                }
                Else
                {
                    Return (Zero)
                }
            }

            Method (SDBG, 0, NotSerialized)
            {
                Return (PDBG) /* External reference */
            }
        }
    }

    Scope (\_SB)
    {
        Name (RBUF, ResourceTemplate ()
        {
            GpioInt (Level, ActiveLow, Exclusive, PullNone, 0x0000,
                "\\_SB.PCI0.I2C7.PMIC", 0x00, ResourceConsumer, ,
                )
                {   // Pin list
                    0x0015
                }
        })
        Device (STR0)
        {
            Name (_HID, EisaId ("INT3403") /* DPTF Temperature Sensor */)  // _HID: Hardware ID
            Name (_UID, Zero)  // _UID: Unique ID
            Name (PTYP, 0x03)
            Name (_STR, Unicode ("Skin hotspot proxy thermistor"))  // _STR: Description String
            Name (CTYP, Zero)
            Name (LTM0, Zero)
            Name (XDEP, Package (0x02)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI1
            })
            Name (TDEP, Package (0x02)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI2
            })
            Name (CDEP, Package (0x02)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMIC
            })
            Name (_DEP, Package (0x02)  // _DEP: Dependencies
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI5
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If ((THM0 == Zero))
                {
                    Return (Zero)
                }

                Return (0x0F)
            }

            Method (_TSP, 0, NotSerialized)  // _TSP: Thermal Sampling Period
            {
                If (((PMID == Zero) || (PMID == 0x03)))
                {
                    If (((BDID == 0x09) || (BDID == 0x0A)))
                    {
                        Return (Ones)
                    }
                    Else
                    {
                        Return (0xFFFF)
                    }
                }
                Else
                {
                    Return (VTSP) /* External reference */
                }
            }

            Method (_DTI, 1, NotSerialized)  // _DTI: Device Temperature Indication
            {
                LTM0 = Arg0
            }

            Method (_NTT, 0, NotSerialized)  // _NTT: Notification Temperature Threshold
            {
                Return (0x0ADE)
            }

            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                Name (SBUF, ResourceTemplate ()
                {
                })
                Name (WBUF, ResourceTemplate ()
                {
                    GpioInt (Level, ActiveLow, Exclusive, PullNone, 0x0000,
                        "\\_SB.PCI0.I2C7.PMI5", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0015
                        }
                })
                If ((PMID == Zero))
                {
                    Return (^^RBUF) /* \_SB_.RBUF */
                }
                ElseIf ((PMID == 0x03))
                {
                    Return (WBUF) /* \_SB_.STR0._CRS.WBUF */
                }
                Else
                {
                    Return (SBUF) /* \_SB_.STR0._CRS.SBUF */
                }
            }

            Method (_INI, 0, NotSerialized)  // _INI: Initialize
            {
            }

            Method (_TMP, 0, Serialized)  // _TMP: Temperature
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.GMP0 = Zero
                        If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMIC.TMP0) /* External reference */
                        }
                    }
                }

                If ((PMID == One))
                {
                    If ((\_SB.PCI0.I2C7.PMI2.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMI2.GMP0 = Zero
                        If ((\_SB.PCI0.I2C7.PMI2.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMI2.TMP0) /* External reference */
                        }
                    }
                }

                If ((PMID == 0x02))
                {
                    If ((\_SB.PCI0.I2C7.PMI1.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMI1.GMP0 = Zero
                        If ((\_SB.PCI0.I2C7.PMI1.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMI1.TMP0) /* External reference */
                        }
                    }
                }

                If ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.GMP0 = Zero
                        If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMI5.TMP0) /* External reference */
                        }
                    }
                }

                Return (Zero)
            }

            Name (PATC, 0x02)
            Method (PAT0, 1, Serialized)
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.AX00 = Arg0
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GX00 = One
                        }

                        \_SB.PCI0.I2C7.PMIC.PEN0 = Zero
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GEN0 = One
                        }
                    }
                }
                ElseIf ((PMID == 0x02))
                {
                    If ((\_SB.PCI0.I2C7.PMI1.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI1.AX00 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI1.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI1.GX00 = One
                        }

                        \_SB.PCI0.I2C7.PMI1.PEN0 = Zero
                        If ((\_SB.PCI0.I2C7.PMI1.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI1.GEN0 = One
                        }
                    }
                }
                ElseIf ((PMID == One))
                {
                    If ((\_SB.PCI0.I2C7.PMI2.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI2.AX00 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI2.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI2.GX00 = One
                        }

                        \_SB.PCI0.I2C7.PMI2.PEN0 = Zero
                        If ((\_SB.PCI0.I2C7.PMI2.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI2.GEN0 = One
                        }
                    }
                }
                ElseIf ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.AX00 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GX00 = One
                        }

                        \_SB.PCI0.I2C7.PMI5.PEN0 = Zero
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GEN0 = One
                        }
                    }
                }
            }

            Method (PAT1, 1, Serialized)
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.AX01 = Arg0
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GX01 = One
                        }
                    }
                }
                ElseIf ((PMID == One))
                {
                    If ((\_SB.PCI0.I2C7.PMI2.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI2.AX01 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI2.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI2.GX01 = One
                        }
                    }
                }
                ElseIf ((PMID == 0x02))
                {
                    If ((\_SB.PCI0.I2C7.PMI1.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI1.AX01 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI1.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI1.GX01 = One
                        }
                    }
                }
                ElseIf ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.AX01 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GX01 = One
                        }
                    }
                }
            }

            Name (GTSH, 0x06)
            Method (_CRT, 0, Serialized)  // _CRT: Critical Temperature
            {
                Return (\_SB.DPTF.CTOK (GCR0))
            }

            Method (_HOT, 0, Serialized)  // _HOT: Hot Temperature
            {
                Return (\_SB.DPTF.CTOK ((GCR0 - 0x03)))
            }

            Method (_PSV, 0, Serialized)  // _PSV: Passive Temperature
            {
                Name (MAXV, Zero)
                If (((HPME == One) && (PSC0 >= MAXV)))
                {
                    MAXV = PSC0 /* \PSC0 */
                }

                If ((PSA0 >= MAXV))
                {
                    MAXV = PSA0 /* \PSA0 */
                }

                If ((\_SB.PST0 >= MAXV))
                {
                    MAXV = \_SB.PST0 /* External reference */
                }

                Return (\_SB.DPTF.CTOK (MAXV))
            }

            Method (_SCP, 3, Serialized)  // _SCP: Set Cooling Policy
            {
                If (((Arg0 == Zero) || (Arg0 == One)))
                {
                    CTYP = Arg0
                    If ((BDID == 0x08))
                    {
                        If ((\_SB.PCI0.I2C7.PMI5.SRCD == Zero))
                        {
                            PSC0 = Zero
                        }
                        Else
                        {
                            PSC0 = 0x40
                        }
                    }
                    ElseIf ((\_SB.ADP1._PSR == Zero))
                    {
                        PSC0 = Zero
                    }
                    Else
                    {
                        PSC0 = 0x40
                    }

                    If (CondRefOf (\_SB.STR0._PSV, Local0))
                    {
                        Notify (\_SB.STR0, 0x91) // Device-Specific
                    }
                }
            }

            Method (SDBG, 0, NotSerialized)
            {
                Return (PDBG) /* External reference */
            }
        }

        Device (STR1)
        {
            Name (_HID, EisaId ("INT3403") /* DPTF Temperature Sensor */)  // _HID: Hardware ID
            Name (_UID, One)  // _UID: Unique ID
            Name (PTYP, 0x03)
            Name (_STR, Unicode ("Ambient estimation"))  // _STR: Description String
            Name (CTYP, Zero)
            Name (LTM1, Zero)
            Name (DDEP, Package (0x02)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMIC
            })
            Name (_DEP, Package (0x02)  // _DEP: Dependencies
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI5
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If (((THM1 == Zero) || (BDID == One)))
                {
                    Return (Zero)
                }

                Return (0x0F)
            }

            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                Name (RBUF, ResourceTemplate ()
                {
                    GpioInt (Level, ActiveLow, Exclusive, PullNone, 0x0000,
                        "\\_SB.PCI0.I2C7.PMIC", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0016
                        }
                })
                Name (WBUF, ResourceTemplate ()
                {
                    GpioInt (Level, ActiveLow, Exclusive, PullNone, 0x0000,
                        "\\_SB.PCI0.I2C7.PMI5", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0016
                        }
                })
                Name (SBUF, ResourceTemplate ()
                {
                })
                If ((PMID == Zero))
                {
                    Return (RBUF) /* \_SB_.STR1._CRS.RBUF */
                }
                ElseIf ((PMID == 0x03))
                {
                    Return (WBUF) /* \_SB_.STR1._CRS.WBUF */
                }
                Else
                {
                    Return (SBUF) /* \_SB_.STR1._CRS.SBUF */
                }
            }

            Name (PATC, 0x02)
            Method (PAT0, 1, Serialized)
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.AX10 = Arg0
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GX10 = One
                        }

                        \_SB.PCI0.I2C7.PMIC.PEN1 = Zero
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GEN1 = One
                        }
                    }
                }
                ElseIf ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.AX10 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GX10 = One
                        }

                        \_SB.PCI0.I2C7.PMI5.PEN1 = Zero
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GEN1 = One
                        }
                    }
                }
            }

            Method (PAT1, 1, Serialized)
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.AX11 = Arg0
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GX11 = One
                        }
                    }
                }
                ElseIf ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.AX11 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GX11 = One
                        }
                    }
                }
            }

            Method (_DTI, 1, NotSerialized)  // _DTI: Device Temperature Indication
            {
                LTM1 = Arg0
            }

            Method (_NTT, 0, NotSerialized)  // _NTT: Notification Temperature Threshold
            {
                Return (0x0ADE)
            }

            Method (_TMP, 0, Serialized)  // _TMP: Temperature
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.GMP1 = Zero
                        If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMIC.TMP1) /* External reference */
                        }
                        Else
                        {
                            Return (Zero)
                        }
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.GMP1 = Zero
                        If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMI5.TMP1) /* External reference */
                        }
                        Else
                        {
                            Return (Zero)
                        }
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                Return (Zero)
            }

            Name (GTSH, 0x06)
            Method (_SCP, 3, Serialized)  // _SCP: Set Cooling Policy
            {
                If (((Arg0 == Zero) || (Arg0 == One)))
                {
                    CTYP = Arg0
                    Notify (\_SB.PCI0.PNIT, 0x91) // Device-Specific
                }
            }

            Method (SDBG, 0, NotSerialized)
            {
                Return (PDBG) /* External reference */
            }
        }

        Device (STR2)
        {
            Name (_HID, EisaId ("INT3403") /* DPTF Temperature Sensor */)  // _HID: Hardware ID
            Name (_UID, 0x02)  // _UID: Unique ID
            Name (_STR, Unicode ("Charger-display thermistor"))  // _STR: Description String
            Name (PTYP, 0x03)
            Name (CTYP, Zero)
            Name (LTM2, Zero)
            Name (DDEP, Package (0x02)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMIC
            })
            Name (_DEP, Package (0x02)  // _DEP: Dependencies
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI5
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If (((THM2 == Zero) || (BDID == One)))
                {
                    Return (Zero)
                }

                Return (0x0F)
            }

            Method (_DTI, 1, NotSerialized)  // _DTI: Device Temperature Indication
            {
                LTM2 = Arg0
            }

            Method (_NTT, 0, NotSerialized)  // _NTT: Notification Temperature Threshold
            {
                Return (0x0ADE)
            }

            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                Name (RBUF, ResourceTemplate ()
                {
                    GpioInt (Level, ActiveLow, Exclusive, PullNone, 0x0000,
                        "\\_SB.PCI0.I2C7.PMIC", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0017
                        }
                })
                Name (WBUF, ResourceTemplate ()
                {
                    GpioInt (Level, ActiveLow, Exclusive, PullNone, 0x0000,
                        "\\_SB.PCI0.I2C7.PMI5", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0017
                        }
                })
                Name (SBUF, ResourceTemplate ()
                {
                })
                If ((PMID == Zero))
                {
                    Return (RBUF) /* \_SB_.STR2._CRS.RBUF */
                }
                ElseIf ((PMID == 0x03))
                {
                    Return (WBUF) /* \_SB_.STR2._CRS.WBUF */
                }
                Else
                {
                    Return (SBUF) /* \_SB_.STR2._CRS.SBUF */
                }
            }

            Method (_INI, 0, NotSerialized)  // _INI: Initialize
            {
            }

            Method (_TMP, 0, Serialized)  // _TMP: Temperature
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.GMP2 = Zero
                        If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMIC.TMP2) /* External reference */
                        }
                        Else
                        {
                            Return (Zero)
                        }
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.GMP2 = Zero
                        If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMI5.TMP2) /* External reference */
                        }
                        Else
                        {
                            Return (Zero)
                        }
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                Return (Zero)
            }

            Name (PATC, 0x02)
            Method (PAT0, 1, Serialized)
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.AX20 = Arg0
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GX20 = One
                        }

                        \_SB.PCI0.I2C7.PMIC.PEN2 = Zero
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GEN2 = One
                        }
                    }
                }
                ElseIf ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.AX20 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GX20 = One
                        }

                        \_SB.PCI0.I2C7.PMI5.PEN2 = Zero
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GEN2 = One
                        }
                    }
                }
            }

            Method (PAT1, 1, Serialized)
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.AX21 = Arg0
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GX21 = One
                        }
                    }
                }
                ElseIf ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.AX21 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GX21 = One
                        }
                    }
                }
            }

            Name (GTSH, 0x06)
            Method (_CRT, 0, Serialized)  // _CRT: Critical Temperature
            {
                Return (\_SB.DPTF.CTOK (GCR2))
            }

            Method (_HOT, 0, Serialized)  // _HOT: Hot Temperature
            {
                Return (\_SB.DPTF.CTOK ((GCR2 - 0x03)))
            }

            Method (_PSV, 0, Serialized)  // _PSV: Passive Temperature
            {
                Name (MAXV, Zero)
                If (((HPME == One) && (PSC2 >= MAXV)))
                {
                    MAXV = PSC2 /* \PSC2 */
                }

                If ((PSA2 >= MAXV))
                {
                    MAXV = PSA2 /* \PSA2 */
                }

                If ((\_SB.PST2 >= MAXV))
                {
                    MAXV = \_SB.PST2 /* External reference */
                }

                Return (\_SB.DPTF.CTOK (MAXV))
            }

            Method (_SCP, 3, Serialized)  // _SCP: Set Cooling Policy
            {
                If (((Arg0 == Zero) || (Arg0 == One)))
                {
                    CTYP = Arg0
                    If ((BDID == 0x08))
                    {
                        If ((\_SB.PCI0.I2C7.PMI5.SRCD == Zero))
                        {
                            PSC2 = Zero
                        }
                        Else
                        {
                            PSC2 = 0x40
                        }
                    }
                    ElseIf ((\_SB.ADP1._PSR == Zero))
                    {
                        PSC2 = Zero
                    }
                    Else
                    {
                        PSC2 = 0x40
                    }

                    If (CondRefOf (\_SB.STR2._PSV, Local0))
                    {
                        Notify (\_SB.STR2, 0x91) // Device-Specific
                    }
                }
            }

            Method (SDBG, 0, NotSerialized)
            {
                Return (PDBG) /* External reference */
            }
        }

        Device (STR3)
        {
            Name (_HID, EisaId ("INT3403") /* DPTF Temperature Sensor */)  // _HID: Hardware ID
            Name (_UID, 0x03)  // _UID: Unique ID
            Name (_STR, Unicode ("Charger Board Sensor"))  // _STR: Description String
            Name (PTYP, 0x03)
            Name (CTYP, Zero)
            Name (LTM3, Zero)
            Name (XDEP, Package (0x02)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI1
            })
            Name (TDEP, Package (0x02)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI2
            })
            Name (CDEP, Package (0x02)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMIC
            })
            Name (_DEP, Package (0x02)  // _DEP: Dependencies
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI5
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If ((OSID != One))
                {
                    Return (0x0F)
                }

                Return (Zero)
            }

            Method (_DTI, 1, NotSerialized)  // _DTI: Device Temperature Indication
            {
                LTM3 = Arg0
            }

            Method (_NTT, 0, NotSerialized)  // _NTT: Notification Temperature Threshold
            {
                Return (0x0ADE)
            }

            Method (_INI, 0, NotSerialized)  // _INI: Initialize
            {
            }

            Name (BUFF, Buffer (0x07)
            {
                 0xFF                                             /* . */
            })
            CreateByteField (BUFF, Zero, STAT)
            CreateByteField (BUFF, One, LEN)
            CreateByteField (BUFF, 0x02, TMP0)
            CreateByteField (BUFF, 0x03, AX00)
            CreateByteField (BUFF, 0x04, AX01)
            CreateByteField (BUFF, 0x05, AX10)
            CreateByteField (BUFF, 0x06, AX11)
            Method (_TMP, 0, Serialized)  // _TMP: Temperature
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.GMP3 = Zero
                        If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMIC.TMP3) /* External reference */
                        }
                    }
                }

                If ((PMID == One))
                {
                    If ((\_SB.PCI0.I2C7.PMI2.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMI2.GMP3 = Zero
                        If ((\_SB.PCI0.I2C7.PMI2.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMI2.TMP3) /* External reference */
                        }
                    }
                }

                If ((PMID == 0x02))
                {
                    If ((\_SB.PCI0.I2C7.PMI1.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMI1.GMP3 = Zero
                        If ((\_SB.PCI0.I2C7.PMI1.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMI1.TMP3) /* External reference */
                        }
                    }
                }

                If ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.GMP3 = Zero
                        If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                        {
                            Return (\_SB.PCI0.I2C7.PMI5.TMP3) /* External reference */
                        }
                    }
                }

                Return (Zero)
            }

            Name (PATC, 0x02)
            Method (PAT0, 1, Serialized)
            {
                Local0 = Arg0
                Local0 &= 0xFFFF
                If ((Local0 == 0xFFFF))
                {
                    Local1 = 0xFF
                    Local2 = Zero
                }
                Else
                {
                    Local1 = \_SB.DPTF.KTOC (Arg0)
                    Local1 &= 0xFF
                    Local2 = 0x08
                }

                Local3 = (GTSH / 0x0A)
                Local3 &= 0x07
                Local2 |= Local3
                If ((\_SB.PCI0.I2C1.AVBL == One))
                {
                    BUFF = \_SB.PCI0.I2C1.THRM /* External reference */
                    If ((STAT == Zero))
                    {
                        AX00 = Local1
                        AX01 = Local2
                        \_SB.PCI0.I2C1.THRM = BUFF /* \_SB_.STR3.BUFF */
                    }
                }

                If ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.AX30 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GX30 = One
                        }
                    }
                }
            }

            Method (PAT1, 1, Serialized)
            {
                Local0 = Arg0
                Local0 &= 0xFFFF
                If ((Local0 == 0xFFFF))
                {
                    Local1 = 0xFF
                    Local2 = Zero
                }
                Else
                {
                    Local1 = \_SB.DPTF.KTOC (Arg0)
                    Local1 &= 0xFF
                    Local2 = 0x08
                }

                Local3 = (GTSH / 0x0A)
                Local3 &= 0x07
                Local2 |= Local3
                If ((\_SB.PCI0.I2C1.AVBL == One))
                {
                    BUFF = \_SB.PCI0.I2C1.THRM /* External reference */
                    If ((STAT == Zero))
                    {
                        AX10 = Local1
                        AX11 = Local2
                        \_SB.PCI0.I2C1.THRM = BUFF /* \_SB_.STR3.BUFF */
                    }
                }

                If ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.AX31 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GX31 = One
                        }
                    }
                }
            }

            Name (GTSH, 0x06)
            Method (_CRT, 0, Serialized)  // _CRT: Critical Temperature
            {
                Return (\_SB.DPTF.CTOK (GCR3))
            }

            Method (_HOT, 0, Serialized)  // _HOT: Hot Temperature
            {
                Return (\_SB.DPTF.CTOK ((GCR3 - 0x03)))
            }

            Method (_PSV, 0, Serialized)  // _PSV: Passive Temperature
            {
                Return (\_SB.DPTF.CTOK (PST3))
            }

            Method (_SCP, 3, Serialized)  // _SCP: Set Cooling Policy
            {
                If (((Arg0 == Zero) || (Arg0 == One)))
                {
                    CTYP = Arg0
                    Notify (\_SB.PCI0.PNIT, 0x91) // Device-Specific
                }
            }

            Method (SDBG, 0, NotSerialized)
            {
                Return (PDBG) /* External reference */
            }
        }

        Device (SAMB)
        {
            Name (_HID, EisaId ("INT3409"))  // _HID: Hardware ID
            Name (_UID, Zero)  // _UID: Unique ID
            Name (PTYP, 0x16)
            Name (_STR, Unicode ("Ambient Temperature"))  // _STR: Description String
            Name (CPRM, 0x0B54)
            Name (DDEP, Package (0x02)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMIC
            })
            Name (_DEP, Package (0x02)  // _DEP: Dependencies
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI5
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If (((THM4 == Zero) || (BDID == One)))
                {
                    Return (Zero)
                }

                If (((BDID == 0x09) || (BDID == 0x0A)))
                {
                    Return (Zero)
                }
                Else
                {
                    Return (0x0F)
                }
            }

            Method (_CRS, 0, Serialized)  // _CRS: Current Resource Settings
            {
                Name (RBUF, ResourceTemplate ()
                {
                    GpioInt (Level, ActiveLow, Exclusive, PullNone, 0x0000,
                        "\\_SB.PCI0.I2C7.PMIC", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0016
                        }
                })
                Name (WBUF, ResourceTemplate ()
                {
                    GpioInt (Level, ActiveLow, Exclusive, PullNone, 0x0000,
                        "\\_SB.PCI0.I2C7.PMI5", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0016
                        }
                })
                Name (SBUF, ResourceTemplate ()
                {
                })
                If ((PMID == Zero))
                {
                    Return (RBUF) /* \_SB_.SAMB._CRS.RBUF */
                }
                ElseIf ((PMID == 0x03))
                {
                    Return (WBUF) /* \_SB_.SAMB._CRS.WBUF */
                }
                Else
                {
                    Return (SBUF) /* \_SB_.SAMB._CRS.SBUF */
                }
            }

            Method (AMBC, 0, Serialized)
            {
                Name (AMBF, Package (0x0C)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero
                })
                AMBF [Zero] = ABS0 /* External reference */
                AMBF [One] = ABC0 /* External reference */
                AMBF [0x02] = ABS1 /* External reference */
                AMBF [0x03] = ABC1 /* External reference */
                AMBF [0x04] = ABS2 /* External reference */
                AMBF [0x05] = ABC2 /* External reference */
                AMBF [0x06] = ABS3 /* External reference */
                AMBF [0x07] = ABC3 /* External reference */
                AMBF [0x08] = ABS4 /* External reference */
                AMBF [0x09] = ABC4 /* External reference */
                AMBF [0x0A] = ABS5 /* External reference */
                AMBF [0x0B] = ABC5 /* External reference */
                Return (AMBF) /* \_SB_.SAMB.AMBC.AMBF */
            }

            Method (AMTT, 0, Serialized)
            {
                Name (AMTF, Package (0x03)
                {
                    Zero, 
                    Zero, 
                    Zero
                })
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.GMP1 = Zero
                        \_SB.PCI0.I2C7.PMIC.GMP0 = Zero
                        \_SB.PCI0.I2C7.PMIC.GMP2 = Zero
                        If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                        {
                            AMTF [Zero] = \_SB.PCI0.I2C7.PMIC.TMP1 /* External reference */
                            AMTF [One] = \_SB.PCI0.I2C7.PMIC.TMP0 /* External reference */
                            AMTF [0x02] = \_SB.PCI0.I2C7.PMIC.TMP2 /* External reference */
                            Return (AMTF) /* \_SB_.SAMB.AMTT.AMTF */
                        }
                        Else
                        {
                            Return (Zero)
                        }
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                ElseIf ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                {
                    \_SB.PCI0.I2C7.PMI5.GMP1 = Zero
                    \_SB.PCI0.I2C7.PMI5.GMP0 = Zero
                    \_SB.PCI0.I2C7.PMI5.GMP2 = Zero
                    If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                    {
                        AMTF [Zero] = \_SB.PCI0.I2C7.PMI5.TMP1 /* External reference */
                        AMTF [One] = \_SB.PCI0.I2C7.PMI5.TMP0 /* External reference */
                        AMTF [0x02] = \_SB.PCI0.I2C7.PMI5.TMP2 /* External reference */
                        Return (AMTF) /* \_SB_.SAMB.AMTT.AMTF */
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
                Else
                {
                    Return (Zero)
                }
            }

            Method (_INI, 0, NotSerialized)  // _INI: Initialize
            {
            }

            Method (_ATI, 1, Serialized)
            {
                If ((AMTE == One))
                {
                    If ((Arg0 <= 0x0BA6))
                    {
                        If ((BDID == 0x0A))
                        {
                            PSA2 = 0x34
                        }
                        Else
                        {
                            PSA2 = 0x3D
                        }

                        PSA0 = 0x34
                        WAN1 = 0x28
                    }
                    ElseIf ((Arg0 <= 0x0BD8))
                    {
                        If ((BDID == 0x0A))
                        {
                            PSA2 = 0x35
                        }
                        Else
                        {
                            PSA2 = 0x3E
                        }

                        PSA0 = 0x35
                        WAN1 = 0x29
                    }
                    ElseIf ((Arg0 <= 0x0C0A))
                    {
                        If ((BDID == 0x0A))
                        {
                            PSA2 = 0x36
                        }
                        Else
                        {
                            PSA2 = 0x3F
                        }

                        PSA0 = 0x38
                        WAN1 = 0x2A
                    }
                    Else
                    {
                        If ((BDID == 0x0A))
                        {
                            PSA2 = 0x37
                        }
                        Else
                        {
                            PSA2 = 0x40
                        }

                        PSA0 = 0x39
                        WAN1 = 0x2B
                    }

                    Notify (\_SB.STR2, 0x91) // Device-Specific
                    Notify (\_SB.STR0, 0x91) // Device-Specific
                    Notify (\_SB.WWAN, 0x91) // Device-Specific
                }
            }

            Method (AMBT, 0, Serialized)
            {
                Return (0x0AC0)
            }

            Method (TMPC, 0, Serialized)
            {
                If (CondRefOf (\_SB.STR0._TMP, Local0))
                {
                    Return (\_SB.STR0._TMP ())
                }
                Else
                {
                    Return (Zero)
                }
            }

            Method (TMPH, 0, Serialized)
            {
                If (CondRefOf (\_SB.STR2._TMP, Local0))
                {
                    Return (\_SB.STR2._TMP ())
                }
                Else
                {
                    Return (Zero)
                }
            }

            Name (PATC, 0x02)
            Method (PAT0, 1, Serialized)
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.AX10 = Arg0
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GX10 = One
                        }

                        \_SB.PCI0.I2C7.PMIC.PEN1 = Zero
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GEN1 = One
                        }
                    }
                }
                ElseIf ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.AX10 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GX10 = One
                        }

                        \_SB.PCI0.I2C7.PMI5.PEN1 = Zero
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GEN1 = One
                        }
                    }
                }
            }

            Method (PAT1, 1, Serialized)
            {
                If ((PMID == Zero))
                {
                    If ((\_SB.PCI0.I2C7.PMIC.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMIC.AX11 = Arg0
                        If ((\_SB.PCI0.I2C7.PMIC.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMIC.GX11 = One
                        }
                    }
                }
                ElseIf ((PMID == 0x03))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.AVBD == One))
                    {
                        \_SB.PCI0.I2C7.PMI5.AX11 = Arg0
                        If ((\_SB.PCI0.I2C7.PMI5.AVBG == One))
                        {
                            \_SB.PCI0.I2C7.PMI5.GX11 = One
                        }
                    }
                }
            }

            Method (SDBG, 0, NotSerialized)
            {
                Return (PDBG) /* External reference */
            }
        }

        Device (VIR1)
        {
            Name (_HID, EisaId ("INT3409"))  // _HID: Hardware ID
            Name (_UID, "VIR1")  // _UID: Unique ID
            Name (PTYP, 0x15)
            Name (_STR, Unicode ("Virtual Sensor 1"))  // _STR: Description String
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If (((BDID == 0x09) || (BDID == 0x0A)))
                {
                    Return (0x0F)
                }
                Else
                {
                    Return (Zero)
                }
            }

            Name (LSTM, Zero)
            Method (_DTI, 1, Serialized)  // _DTI: Device Temperature Indication
            {
                If ((AMTE == One))
                {
                    If ((Arg0 <= 0x0BA6))
                    {
                        PSA0 = 0x34
                        PSA2 = 0x3D
                        WAN1 = 0x28
                    }
                    ElseIf ((Arg0 <= 0x0BD8))
                    {
                        PSA0 = 0x35
                        PSA2 = 0x3E
                        WAN1 = 0x29
                    }
                    ElseIf ((Arg0 <= 0x0C0A))
                    {
                        PSA0 = 0x38
                        PSA2 = 0x3F
                        WAN1 = 0x2A
                    }
                    Else
                    {
                        PSA0 = 0x39
                        PSA2 = 0x40
                        WAN1 = 0x2B
                    }

                    Notify (\_SB.STR2, 0x91) // Device-Specific
                    Notify (\_SB.STR0, 0x91) // Device-Specific
                    Notify (\_SB.WWAN, 0x91) // Device-Specific
                }
            }

            Method (_NTT, 0, NotSerialized)  // _NTT: Notification Temperature Threshold
            {
                Return (0x32)
            }

            Name (VSCT, Package (0x02)
            {
                One, 
                Package (0x03)
                {
                    Package (0x07)
                    {
                        \_SB.STR0, 
                        0x0E, 
                        Zero, 
                        0x012C, 
                        Zero, 
                        0x03E8, 
                        Zero
                    }, 

                    Package (0x07)
                    {
                        \_SB.STR1, 
                        0x0E, 
                        Zero, 
                        0x012C, 
                        Zero, 
                        0x03E8, 
                        0x0CA0
                    }, 

                    Package (0x07)
                    {
                        \_SB.STR2, 
                        0x0E, 
                        Zero, 
                        0x012C, 
                        Zero, 
                        0x03E8, 
                        Zero
                    }
                }
            })
            Name (VSPT, Package (0x02)
            {
                One, 
                Package (0x01)
                {
                    Package (0x02)
                    {
                        0x0E93, 
                        0x0258
                    }
                }
            })
            Method (_AC0, 0, Serialized)  // _ACx: Active Cooling
            {
                Local1 = Zero
                If ((LSTM >= Local1))
                {
                    Return ((Local1 - 0x14))
                }
                Else
                {
                    Return (Local1)
                }
            }

            Method (_AC1, 0, Serialized)  // _ACx: Active Cooling
            {
                Return ((_AC0 () - 0x28))
            }

            Method (_AC2, 0, Serialized)  // _ACx: Active Cooling
            {
                Return ((_AC0 () - 0x50))
            }

            Method (_AC3, 0, Serialized)  // _ACx: Active Cooling
            {
                Return ((_AC0 () - 0x78))
            }

            Method (_AC4, 0, Serialized)  // _ACx: Active Cooling
            {
                Return ((_AC0 () - 0xA0))
            }

            Method (_AC5, 0, Serialized)  // _ACx: Active Cooling
            {
                Return ((_AC0 () - 0xC8))
            }

            Method (_AC6, 0, Serialized)  // _ACx: Active Cooling
            {
                Return ((_AC0 () - 0xF0))
            }

            Method (_AC7, 0, Serialized)  // _ACx: Active Cooling
            {
                Return ((_AC0 () - 0x0118))
            }

            Method (_AC8, 0, Serialized)  // _ACx: Active Cooling
            {
                Return ((_AC0 () - 0x0140))
            }

            Method (_AC9, 0, Serialized)  // _ACx: Active Cooling
            {
                Return ((_AC0 () - 0x0168))
            }

            Method (_PSV, 0, Serialized)  // _PSV: Passive Temperature
            {
            }

            Method (_CRT, 0, Serialized)  // _CRT: Critical Temperature
            {
            }

            Method (_CR3, 0, Serialized)  // _CR3: Warm/Standby Temperature
            {
            }

            Method (_HOT, 0, Serialized)  // _HOT: Hot Temperature
            {
            }

            Method (_SCP, 3, Serialized)  // _SCP: Set Cooling Policy
            {
                If (((Arg0 == Zero) || (Arg0 == One)))
                {
                    CTYP = Arg0
                    Notify (\_SB.VIR1, 0x91) // Device-Specific
                }
            }

            Name (VERS, Zero)
            Name (CTYP, Zero)
            Name (ALMT, Zero)
            Name (PLMT, Zero)
            Name (WKLD, Zero)
            Name (DSTA, Zero)
            Name (RES1, Zero)
            Method (DSCP, 7, Serialized)
            {
                Name (CHNG, Zero)
                If ((Arg0 != Zero))
                {
                    Return (Zero)
                }

                If (((Arg1 == Zero) || (Arg1 == One)))
                {
                    If ((Arg1 != CTYP))
                    {
                        CHNG = One
                        CTYP = Arg1
                    }
                }

                If (((Arg1 >= Zero) || (Arg1 <= 0x05)))
                {
                    If ((Arg2 != ALMT))
                    {
                        CHNG = One
                        ALMT = Arg2
                    }
                }

                If (((Arg1 >= Zero) || (Arg1 <= 0x05)))
                {
                    If ((Arg3 != PLMT))
                    {
                        CHNG = One
                        PLMT = Arg3
                    }
                }

                If ((Arg4 != WKLD))
                {
                    CHNG = One
                    WKLD = Arg4
                }

                If ((Arg5 != DSTA))
                {
                    CHNG = One
                    DSTA = Arg5
                }

                If ((Arg6 != RES1))
                {
                    CHNG = One
                    RES1 = Arg6
                }

                If (CHNG)
                {
                    Notify (\_SB.VIR1, 0x91) // Device-Specific
                }
            }
        }

        Device (TDSP)
        {
            Name (_HID, EisaId ("INT3406") /* Intel Dynamic Platform & Thermal Framework Display Participant */)  // _HID: Hardware ID
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If ((DDSP == Zero))
                {
                    Return (Zero)
                }

                Return (0x0F)
            }

            Method (DDDL, 0, NotSerialized)
            {
                BDLI = DPLL /* External reference */
                Return (BDLI) /* \BDLI */
            }

            Method (DDPC, 0, NotSerialized)
            {
                BDHI = DPHL /* External reference */
                Return (BDHI) /* \BDHI */
            }

            Method (_BCL, 0, NotSerialized)  // _BCL: Brightness Control Levels
            {
                Return (Package (0x0C)
                {
                    0x50, 
                    0x32, 
                    0x0A, 
                    0x14, 
                    0x1E, 
                    0x28, 
                    0x32, 
                    0x3C, 
                    0x46, 
                    0x50, 
                    0x5A, 
                    0x64
                })
            }

            Method (_BCM, 1, NotSerialized)  // _BCM: Brightness Control Method
            {
                If (((Arg0 >= Zero) && (Arg0 <= 0x64)))
                {
                    \_SB.PCI0.GFX0.DD01._BCM (Arg0)
                }
            }

            Method (_BQC, 0, NotSerialized)  // _BQC: Brightness Query Current
            {
                Return (\_SB.PCI0.GFX0.DD01._BQC) /* External reference */
            }

            Method (_DCS, 0, NotSerialized)  // _DCS: Display Current Status
            {
                Return (\_SB.PCI0.GFX0.DD01._DCS) /* External reference */
            }

            Method (SDBG, 0, NotSerialized)
            {
                Return (PDBG) /* External reference */
            }
        }

        Device (TCHG)
        {
            Name (_HID, EisaId ("INT3403") /* DPTF Temperature Sensor */)  // _HID: Hardware ID
            Name (_UID, 0x04)  // _UID: Unique ID
            Name (PTYP, 0x0B)
            Name (_STR, Unicode ("Intel DPTF Charger Participant"))  // _STR: Description String
            Name (RDEP, Package (0x02)
            {
                \_SB.PCI0.I2C1, 
                \_SB.PCI0.I2C1.BATC
            })
            Name (ODEP, Package (0x02)
            {
                \_SB.PCI0.I2C1, 
                \_SB.PCI0.I2C1.BATC
            })
            Name (FDEP, Package (0x03)
            {
                \_SB.PCI0.I2C7, 
                \_SB.PCI0.I2C7.PMI5, 
                \_SB.PCI0.I2C7.BATC
            })
            Name (TDEP, Package (0x02)
            {
                \_SB.PCI0.I2C3, 
                \_SB.PCI0.I2C3.TIDR
            })
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If ((CHGR == Zero))
                {
                    Return (Zero)
                }

                Return (0x0F)
            }

            Name (HPSS, Package (0x08)
            {
                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    0x9C, 
                    "mA", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    One, 
                    0x80, 
                    "mA", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    0x02, 
                    0x60, 
                    "mA", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    0x03, 
                    0x40, 
                    "mA", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    0x04, 
                    0x30, 
                    "mA", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    0x05, 
                    0x18, 
                    "mA", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    0x06, 
                    0x10, 
                    "mA", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    0x07, 
                    Zero, 
                    "mA", 
                    Zero
                }
            })
            Name (CPSS, Package (0x05)
            {
                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    0x40, 
                    "mA", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    One, 
                    0x30, 
                    "mA", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    0x02, 
                    0x11, 
                    "mA", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    0x03, 
                    0x10, 
                    "mA", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    Zero, 
                    0x04, 
                    Zero, 
                    "mA", 
                    Zero
                }
            })
            Method (PPSS, 0, NotSerialized)
            {
                If ((BDID == 0x0A))
                {
                    Return (HPSS) /* \_SB_.TCHG.HPSS */
                }
                Else
                {
                    Return (CPSS) /* \_SB_.TCHG.CPSS */
                }
            }

            Method (PPPC, 0, NotSerialized)
            {
                If ((BDID == 0x0A))
                {
                    Local2 = SizeOf (HPSS)
                }
                Else
                {
                    Local2 = SizeOf (CPSS)
                }

                Local2--
                If ((BDID == 0x08))
                {
                    If ((\_SB.PCI0.I2C7.PMI5.SRCD == Zero))
                    {
                        ADBG ("PSR DPTF DC")
                        ADBG (Local2)
                        Return (Local2)
                    }
                    Else
                    {
                        ADBG ("PSR DPTF AC")
                        Return (Zero)
                    }
                }
                ElseIf ((\_SB.ADP1._PSR == Zero))
                {
                    ADBG ("PSR DPTF DC")
                    ADBG (Local2)
                    Return (Local2)
                }
                Else
                {
                    ADBG ("PSR DPTF AC")
                    Return (Zero)
                }
            }

            Method (SPPC, 1, NotSerialized)
            {
                ADBG ("SPPC ..")
                If ((BDID == 0x0A))
                {
                    Local2 = SizeOf (HPSS)
                }
                Else
                {
                    Local2 = SizeOf (CPSS)
                }

                Local2--
                If (((Arg0 >= Zero) && (Arg0 <= Local2)))
                {
                    If ((BDID == 0x0A))
                    {
                        Local1 = DerefOf (DerefOf (HPSS [Arg0]) [0x05])
                    }
                    Else
                    {
                        Local1 = DerefOf (DerefOf (CPSS [Arg0]) [0x05])
                    }

                    If ((BDID == One))
                    {
                        If ((PMID == One))
                        {
                            \_SB.PCI0.I2C3.TIDR.BCCC = Local1
                            ADBG ("SPPC BDID 1 TI")
                            If ((Arg0 < Local2))
                            {
                                \_SB.PCI0.I2C3.TIDR.BCCE = One
                            }

                            If ((Arg0 == Local2))
                            {
                                \_SB.PCI0.I2C3.TIDR.BCCE = Zero
                            }

                            Notify (\_SB.PCI0.I2C3.TIDR, 0x86) // Device-Specific
                        }

                        If ((PMID == 0x02))
                        {
                            \_SB.PCI0.I2C7.BMDR.BCCC = Local1
                            ADBG ("SPPC BDID 1 XPWR")
                            If ((Arg0 < Local2))
                            {
                                \_SB.PCI0.I2C7.BMDR.BCCE = One
                            }

                            If ((Arg0 == Local2))
                            {
                                \_SB.PCI0.I2C7.BMDR.BCCE = Zero
                            }

                            Notify (\_SB.PCI0.I2C7.BMDR, 0x86) // Device-Specific
                        }
                    }
                    ElseIf ((BDID == 0x08))
                    {
                        \_SB.PCI0.I2C7.BATC.BCCC = Local1
                        If ((Arg0 < Local2))
                        {
                            \_SB.PCI0.I2C7.BATC.BCCE = One
                        }

                        If ((Arg0 == Local2))
                        {
                            \_SB.PCI0.I2C7.BATC.BCCE = Zero
                        }

                        Notify (\_SB.PCI0.I2C7.BATC, 0x80) // Status Change
                    }
                    ElseIf (((BDID == 0x09) || (BDID == 0x0A)))
                    {
                        \_SB.PCI0.I2C7.WIDR.BCCC = Local1
                        If ((Arg0 < Local2))
                        {
                            \_SB.PCI0.I2C7.WIDR.BCCE = One
                        }

                        If ((Arg0 == Local2))
                        {
                            \_SB.PCI0.I2C7.WIDR.BCCE = Zero
                        }

                        Notify (\_SB.PCI0.I2C7.WIDR, 0x86) // Device-Specific
                    }
                    Else
                    {
                        ADBG ("SPPC BDID ")
                        \_SB.PCI0.I2C1.BATC.BCCC = Local1
                        If ((Arg0 < Local2))
                        {
                            \_SB.PCI0.I2C1.BATC.BCCE = One
                        }

                        If ((Arg0 == Local2))
                        {
                            \_SB.PCI0.I2C1.BATC.BCCE = Zero
                        }

                        Notify (\_SB.PCI0.I2C1.BATC, 0x80) // Status Change
                    }
                }
            }

            Method (SDBG, 0, NotSerialized)
            {
                Return (PDBG) /* External reference */
            }
        }

        Device (WWAN)
        {
            Name (_HID, EisaId ("INT3408"))  // _HID: Hardware ID
            Name (_UID, Zero)  // _UID: Unique ID
            Name (_STR, Unicode ("Intel DPTF WWAN Participant"))  // _STR: Description String
            Name (PTYP, 0x0F)
            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                If (((WWEN == Zero) || (BDID == One)))
                {
                    Return (Zero)
                }

                Return (0x0F)
            }

            Method (_PSV, 0, Serialized)  // _PSV: Passive Temperature
            {
                Name (MAXV, Zero)
                If ((WAN1 >= MAXV))
                {
                    MAXV = WAN1 /* \WAN1 */
                }

                If ((\_SB.WPPT >= MAXV))
                {
                    MAXV = \_SB.WPPT /* External reference */
                }

                Return (\_SB.DPTF.CTOK (MAXV))
            }

            Method (_CRT, 0, Serialized)  // _CRT: Critical Temperature
            {
                Return (\_SB.DPTF.CTOK (WCR0))
            }

            Method (_HOT, 0, Serialized)  // _HOT: Hot Temperature
            {
                Return (\_SB.DPTF.CTOK ((WCR0 - 0x03)))
            }

            Name (LWAN, Zero)
            Method (_DTI, 1, NotSerialized)  // _DTI: Device Temperature Indication
            {
                LWAN = Arg0
                Notify (WWAN, 0x91) // Device-Specific
            }

            Method (SDBG, 0, NotSerialized)
            {
                Return (PDBG) /* External reference */
            }

            Name (GTSH, 0x14)
            Method (_NTT, 0, NotSerialized)  // _NTT: Notification Temperature Threshold
            {
                Return (0x0ADE)
            }

            Name (PPSS, Package (0x02)
            {
                Package (0x08)
                {
                    0x64, 
                    Zero, 
                    Zero, 
                    One, 
                    Zero, 
                    Zero, 
                    "MHz", 
                    Zero
                }, 

                Package (0x08)
                {
                    Zero, 
                    Zero, 
                    Zero, 
                    One, 
                    One, 
                    Zero, 
                    "MHz", 
                    Zero
                }
            })
            Method (PPPC, 0, NotSerialized)
            {
                Return (Zero)
            }

            Method (PPDL, 0, NotSerialized)
            {
                Local2 = SizeOf (PPSS)
                Local2--
                Return (Local2)
            }
        }
    }
}

