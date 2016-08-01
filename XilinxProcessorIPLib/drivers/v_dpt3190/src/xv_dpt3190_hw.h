// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2016.1
// Copyright (C) 1986-2016 Xilinx, Inc. All Rights Reserved.
//
// ==============================================================

// CTRL
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read)
//        bit 7  - auto_restart (Read/Write)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0  - Channel 0 (ap_done)
//        bit 1  - Channel 1 (ap_ready)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0  - Channel 0 (ap_done)
//        bit 1  - Channel 1 (ap_ready)
//        others - reserved
// 0x10 : Data signal of HwReg_num_samples_V
//        bit 7~0 - HwReg_num_samples_V[7:0] (Read/Write)
//        others  - reserved
// 0x14 : reserved
// 0x18 : Data signal of HwReg_audio_channels_V
//        bit 7~0 - HwReg_audio_channels_V[7:0] (Read/Write)
//        others  - reserved
// 0x1c : reserved
// 0x20 : Data signal of HwReg_pkt_type_V
//        bit 3~0 - HwReg_pkt_type_V[3:0] (Read)
//        others  - reserved
// 0x24 : Control signal of HwReg_pkt_type_V
//        bit 0  - HwReg_pkt_type_V_ap_vld (Read/COR)
//        others - reserved
// 0x28 : Data signal of HwReg_ch_no_V
//        bit 11~0 - HwReg_ch_no_V[11:0] (Read)
//        others   - reserved
// 0x2c : Control signal of HwReg_ch_no_V
//        bit 0  - HwReg_ch_no_V_ap_vld (Read/COR)
//        others - reserved
// 0x30 : Data signal of HwReg_pkt_length_V
//        bit 10~0 - HwReg_pkt_length_V[10:0] (Read)
//        others   - reserved
// 0x34 : Control signal of HwReg_pkt_length_V
//        bit 0  - HwReg_pkt_length_V_ap_vld (Read/COR)
//        others - reserved
// 0x38 : Data signal of HwReg_payload_type_V
//        bit 6~0 - HwReg_payload_type_V[6:0] (Read)
//        others  - reserved
// 0x3c : Control signal of HwReg_payload_type_V
//        bit 0  - HwReg_payload_type_V_ap_vld (Read/COR)
//        others - reserved
// 0x40 : Data signal of HwReg_rx_pkt_cnt_V
//        bit 31~0 - HwReg_rx_pkt_cnt_V[31:0] (Read)
// 0x44 : Control signal of HwReg_rx_pkt_cnt_V
//        bit 0  - HwReg_rx_pkt_cnt_V_ap_vld (Read/COR)
//        others - reserved
// 0x48 : Data signal of HwReg_C31_0_V
//        bit 31~0 - HwReg_C31_0_V[31:0] (Read/Write)
// 0x4c : reserved
// 0x50 : Data signal of HwReg_C63_32_V
//        bit 31~0 - HwReg_C63_32_V[31:0] (Read/Write)
// 0x54 : reserved
// 0x58 : Data signal of HwReg_C95_64_V
//        bit 31~0 - HwReg_C95_64_V[31:0] (Read/Write)
// 0x5c : reserved
// 0x60 : Data signal of HwReg_C127_96_V
//        bit 31~0 - HwReg_C127_96_V[31:0] (Read/Write)
// 0x64 : reserved
// 0x68 : Data signal of HwReg_C159_128_V
//        bit 31~0 - HwReg_C159_128_V[31:0] (Read/Write)
// 0x6c : reserved
// 0x70 : Data signal of HwReg_C191_160_V
//        bit 31~0 - HwReg_C191_160_V[31:0] (Read/Write)
// 0x74 : reserved
// 0x78 : Data signal of HwReg_U31_0_V
//        bit 31~0 - HwReg_U31_0_V[31:0] (Read/Write)
// 0x7c : reserved
// 0x80 : Data signal of HwReg_U63_32_V
//        bit 31~0 - HwReg_U63_32_V[31:0] (Read/Write)
// 0x84 : reserved
// 0x88 : Data signal of HwReg_U95_64_V
//        bit 31~0 - HwReg_U95_64_V[31:0] (Read/Write)
// 0x8c : reserved
// 0x90 : Data signal of HwReg_U127_96_V
//        bit 31~0 - HwReg_U127_96_V[31:0] (Read/Write)
// 0x94 : reserved
// 0x98 : Data signal of HwReg_U159_128_V
//        bit 31~0 - HwReg_U159_128_V[31:0] (Read/Write)
// 0x9c : reserved
// 0xa0 : Data signal of HwReg_U191_160_V
//        bit 31~0 - HwReg_U191_160_V[31:0] (Read/Write)
// 0xa4 : reserved
// 0xa8 : Data signal of HwReg_module_reset_V
//        bit 0  - HwReg_module_reset_V[0] (Read/Write)
//        others - reserved
// 0xac : reserved
// 0xb0 : Data signal of HwReg_stat_reset_V
//        bit 0  - HwReg_stat_reset_V[0] (Read/Write)
//        others - reserved
// 0xb4 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XV_DPT3190_CTRL_ADDR_AP_CTRL                     0x00
#define XV_DPT3190_CTRL_ADDR_GIE                         0x04
#define XV_DPT3190_CTRL_ADDR_IER                         0x08
#define XV_DPT3190_CTRL_ADDR_ISR                         0x0c
#define XV_DPT3190_CTRL_ADDR_HWREG_NUM_SAMPLES_V_DATA    0x10
#define XV_DPT3190_CTRL_BITS_HWREG_NUM_SAMPLES_V_DATA    8
#define XV_DPT3190_CTRL_ADDR_HWREG_AUDIO_CHANNELS_V_DATA 0x18
#define XV_DPT3190_CTRL_BITS_HWREG_AUDIO_CHANNELS_V_DATA 8
#define XV_DPT3190_CTRL_ADDR_HWREG_PKT_TYPE_V_DATA       0x20
#define XV_DPT3190_CTRL_BITS_HWREG_PKT_TYPE_V_DATA       4
#define XV_DPT3190_CTRL_ADDR_HWREG_PKT_TYPE_V_CTRL       0x24
#define XV_DPT3190_CTRL_ADDR_HWREG_CH_NO_V_DATA          0x28
#define XV_DPT3190_CTRL_BITS_HWREG_CH_NO_V_DATA          12
#define XV_DPT3190_CTRL_ADDR_HWREG_CH_NO_V_CTRL          0x2c
#define XV_DPT3190_CTRL_ADDR_HWREG_PKT_LENGTH_V_DATA     0x30
#define XV_DPT3190_CTRL_BITS_HWREG_PKT_LENGTH_V_DATA     11
#define XV_DPT3190_CTRL_ADDR_HWREG_PKT_LENGTH_V_CTRL     0x34
#define XV_DPT3190_CTRL_ADDR_HWREG_PAYLOAD_TYPE_V_DATA   0x38
#define XV_DPT3190_CTRL_BITS_HWREG_PAYLOAD_TYPE_V_DATA   7
#define XV_DPT3190_CTRL_ADDR_HWREG_PAYLOAD_TYPE_V_CTRL   0x3c
#define XV_DPT3190_CTRL_ADDR_HWREG_RX_PKT_CNT_V_DATA     0x40
#define XV_DPT3190_CTRL_BITS_HWREG_RX_PKT_CNT_V_DATA     32
#define XV_DPT3190_CTRL_ADDR_HWREG_RX_PKT_CNT_V_CTRL     0x44
#define XV_DPT3190_CTRL_ADDR_HWREG_C31_0_V_DATA          0x48
#define XV_DPT3190_CTRL_BITS_HWREG_C31_0_V_DATA          32
#define XV_DPT3190_CTRL_ADDR_HWREG_C63_32_V_DATA         0x50
#define XV_DPT3190_CTRL_BITS_HWREG_C63_32_V_DATA         32
#define XV_DPT3190_CTRL_ADDR_HWREG_C95_64_V_DATA         0x58
#define XV_DPT3190_CTRL_BITS_HWREG_C95_64_V_DATA         32
#define XV_DPT3190_CTRL_ADDR_HWREG_C127_96_V_DATA        0x60
#define XV_DPT3190_CTRL_BITS_HWREG_C127_96_V_DATA        32
#define XV_DPT3190_CTRL_ADDR_HWREG_C159_128_V_DATA       0x68
#define XV_DPT3190_CTRL_BITS_HWREG_C159_128_V_DATA       32
#define XV_DPT3190_CTRL_ADDR_HWREG_C191_160_V_DATA       0x70
#define XV_DPT3190_CTRL_BITS_HWREG_C191_160_V_DATA       32
#define XV_DPT3190_CTRL_ADDR_HWREG_U31_0_V_DATA          0x78
#define XV_DPT3190_CTRL_BITS_HWREG_U31_0_V_DATA          32
#define XV_DPT3190_CTRL_ADDR_HWREG_U63_32_V_DATA         0x80
#define XV_DPT3190_CTRL_BITS_HWREG_U63_32_V_DATA         32
#define XV_DPT3190_CTRL_ADDR_HWREG_U95_64_V_DATA         0x88
#define XV_DPT3190_CTRL_BITS_HWREG_U95_64_V_DATA         32
#define XV_DPT3190_CTRL_ADDR_HWREG_U127_96_V_DATA        0x90
#define XV_DPT3190_CTRL_BITS_HWREG_U127_96_V_DATA        32
#define XV_DPT3190_CTRL_ADDR_HWREG_U159_128_V_DATA       0x98
#define XV_DPT3190_CTRL_BITS_HWREG_U159_128_V_DATA       32
#define XV_DPT3190_CTRL_ADDR_HWREG_U191_160_V_DATA       0xa0
#define XV_DPT3190_CTRL_BITS_HWREG_U191_160_V_DATA       32
#define XV_DPT3190_CTRL_ADDR_HWREG_MODULE_RESET_V_DATA   0xa8
#define XV_DPT3190_CTRL_BITS_HWREG_MODULE_RESET_V_DATA   1
#define XV_DPT3190_CTRL_ADDR_HWREG_STAT_RESET_V_DATA     0xb0
#define XV_DPT3190_CTRL_BITS_HWREG_STAT_RESET_V_DATA     1

#define XV_DPT3190_1_BIT_DATA                                   0x1
#define XV_DPT3190_2_BITS_DATA                                  0x3
#define XV_DPT3190_3_BITS_DATA                                  0x7
#define XV_DPT3190_4_BITS_DATA                                  0xF
#define XV_DPT3190_7_BITS_DATA                                  0x7F
#define XV_DPT3190_8_BITS_DATA                                  0xFF
#define XV_DPT3190_16_BITS_DATA                                 0xFFFF
#define XV_DPT3190_32_BITS_DATA                                 0xFFFFFFFF

#define XV_DPT3190_1_BIT_SHIFT                                  1
#define XV_DPT3190_2_BITS_SHIFT                                 2
#define XV_DPT3190_3_BITS_SHIFT                                 3
#define XV_DPT3190_4_BITS_SHIFT                                 4
#define XV_DPT3190_5_BITS_SHIFT                                 5
#define XV_DPT3190_6_BITS_SHIFT                                 6
#define XV_DPT3190_7_BITS_SHIFT                                 7

#define XV_DPT3190_8_BITS_SHIFT                                 8
#define XV_DPT3190_16_BITS_SHIFT                                16
#define XV_DPT3190_24_BITS_SHIFT                                24
