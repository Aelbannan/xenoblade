// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/hcic/hcicmds
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void *GKI_getpoolbuf(unsigned char pool_id);

int btsnd_hcic_inquiry(unsigned char *bd_addr, unsigned char inquiry_mode, unsigned char inquiry_length)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 8;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 1;
    p[9] = 4;
    p[10] = 5;
    p[11] = bd_addr[2];
    p[12] = bd_addr[1];
    p[13] = bd_addr[0];
    p[14] = inquiry_mode;
    p[15] = inquiry_length;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_inq_cancel(void)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 3;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 2;
    p[9] = 4;
    p[10] = 0;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_per_inq_mode(short max_delay, unsigned short min_delay, unsigned char *bd_addr, unsigned char inquiry_mode, unsigned char inquiry_length)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 0xC;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 3;
    p[9] = 4;
    p[10] = 9;
    p[11] = (unsigned char)max_delay;
    p[12] = (unsigned char)(max_delay >> 8);
    p[13] = (unsigned char)min_delay;
    p[14] = (unsigned char)(min_delay >> 8);
    p[15] = bd_addr[2];
    p[16] = bd_addr[1];
    p[17] = bd_addr[0];
    p[18] = inquiry_mode;
    p[19] = inquiry_length;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_create_conn(unsigned char *bd_addr, short packet_types, unsigned char page_scan_rep_mode, unsigned char page_scan_mode, unsigned short clock_offset, unsigned char allow_switch)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    unsigned char *pp;
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 0x10;
    *(unsigned short *)(p + 4) = 0;
    pp = p + 8;
    *pp++ = 5;
    *pp++ = 4;
    *pp++ = 0xd;
    *pp++ = bd_addr[5];
    *pp++ = bd_addr[4];
    *pp++ = bd_addr[3];
    *pp++ = bd_addr[2];
    *pp++ = bd_addr[1];
    *pp++ = bd_addr[0];
    *pp++ = (unsigned char)packet_types;
    *pp++ = (unsigned char)(packet_types >> 8);
    *pp++ = page_scan_rep_mode;
    *pp++ = page_scan_mode;
    *pp++ = (unsigned char)clock_offset;
    *pp++ = (unsigned char)(clock_offset >> 8);
    *pp++ = allow_switch;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_disconnect(unsigned short handle, unsigned char reason)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 6;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 6;
    p[9] = 4;
    p[10] = 3;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    p[13] = reason;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_add_SCO_conn(short handle, unsigned short packet_types)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 7;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 7;
    p[9] = 4;
    p[10] = 4;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    p[13] = (unsigned char)packet_types;
    p[14] = (unsigned char)(packet_types >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_accept_conn(unsigned char *p, unsigned char *bd_addr, unsigned char role)
{
    *(unsigned short *)(p + 2) = 0xa;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 9;
    p[9] = 4;
    p[10] = 7;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = role;
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_reject_conn(unsigned char* p, unsigned char* bd_addr, unsigned char reason)
{
    *(unsigned short*)(p + 2) = 0xa;
    *(unsigned short*)(p + 4) = 0;
    p[8] = 0xa;
    p[9] = 4;
    p[10] = 7;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = reason;
    btu_hcif_send_cmd(p);
}

int btsnd_hcic_link_key_req_reply(unsigned char *bd_addr, unsigned char *link_key)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    int i;
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 0x19;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0xb;
    p[9] = 4;
    p[10] = 0x16;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    for (i = 0; i < 16; i++)
        p[17 + i] = link_key[15 - i];
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_link_key_neg_reply(unsigned char *bd_addr)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 9;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0xC;
    p[9] = 4;
    p[10] = 6;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_pin_code_req_reply(unsigned char *bd_addr, unsigned char pin_code_len, unsigned char *pin_code)
{
    unsigned char *p;
    unsigned char *pp;
    int i;

    if ((p = (unsigned char *)GKI_getpoolbuf(2)) == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 0x1A;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x0D;
    p[9] = 0x04;
    p[10] = 0x17;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = pin_code_len;
    pp = p + 18;
    for (i = 0; i < pin_code_len; i++)
        *pp++ = *pin_code++;
    for (; i < 16; i++)
        *pp++ = 0;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_pin_code_neg_reply(unsigned char *bd_addr)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 9;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0xE;
    p[9] = 4;
    p[10] = 6;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_change_conn_type(short handle, unsigned short packet_types)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 7;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0xf;
    p[9] = 4;
    p[10] = 4;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    p[13] = (unsigned char)packet_types;
    p[14] = (unsigned char)(packet_types >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_auth_request(unsigned short handle)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x11;
    p[9] = 0x04;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_set_conn_encrypt(unsigned short handle, unsigned char enable)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 6;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x13;
    p[9] = 4;
    p[10] = 3;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    p[13] = enable;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_rmt_name_req(unsigned char *bd_addr, unsigned char page_scan_rep_mode, unsigned char page_scan_mode, unsigned short clock_offset)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 0xd;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x19;
    p[9] = 4;
    p[10] = 0xa;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = page_scan_rep_mode;
    p[18] = page_scan_mode;
    p[19] = (unsigned char)clock_offset;
    p[20] = (unsigned char)(clock_offset >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_rmt_name_req_cancel(unsigned char *bd_addr)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 9;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x1A;
    p[9] = 4;
    p[10] = 6;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_rmt_features_req(unsigned short handle)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x1b;
    p[9] = 0x04;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_rmt_ver_req(unsigned short handle)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x1d;
    p[9] = 0x04;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_read_rmt_clk_offset(unsigned short handle)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x1f;
    p[9] = 4;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_setup_esco_conn(short tx_bw, unsigned int rx_bw, unsigned int max_latency, short voice_settings, short retrans_effort, unsigned char input_format, unsigned short packet_types)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 0x14;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x28;
    p[9] = 4;
    p[10] = 0x11;
    p[11] = (unsigned char)tx_bw;
    p[12] = (unsigned char)(tx_bw >> 8);
    p[13] = (unsigned char)rx_bw;
    p[14] = (unsigned char)(rx_bw >> 8);
    p[15] = (unsigned char)(rx_bw >> 16);
    p[16] = (unsigned char)(rx_bw >> 24);
    p[17] = (unsigned char)max_latency;
    p[18] = (unsigned char)(max_latency >> 8);
    p[19] = (unsigned char)(max_latency >> 16);
    p[20] = (unsigned char)(max_latency >> 24);
    p[21] = (unsigned char)voice_settings;
    p[22] = (unsigned char)(voice_settings >> 8);
    p[23] = (unsigned char)retrans_effort;
    p[24] = (unsigned char)(retrans_effort >> 8);
    p[25] = input_format;
    p[26] = (unsigned char)packet_types;
    p[27] = (unsigned char)(packet_types >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_accept_esco_conn(unsigned char *p, unsigned char *bd_addr, unsigned int tx_coding_format, unsigned int rx_coding_format, short tx_codec_frame_size, unsigned short rx_codec_frame_size, unsigned char input_bandwidth, unsigned short packet_types)
{
    *(unsigned short *)(p + 2) = 0x18;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x29;
    p[9] = 4;
    p[10] = 0x15;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = (unsigned char)tx_coding_format;
    p[18] = (unsigned char)(tx_coding_format >> 8);
    p[19] = (unsigned char)(tx_coding_format >> 16);
    p[20] = (unsigned char)(tx_coding_format >> 24);
    p[21] = (unsigned char)rx_coding_format;
    p[22] = (unsigned char)(rx_coding_format >> 8);
    p[23] = (unsigned char)(rx_coding_format >> 16);
    p[24] = (unsigned char)(rx_coding_format >> 24);
    p[25] = (unsigned char)tx_codec_frame_size;
    p[26] = (unsigned char)(tx_codec_frame_size >> 8);
    p[27] = (unsigned char)rx_codec_frame_size;
    p[28] = (unsigned char)(rx_codec_frame_size >> 8);
    p[29] = input_bandwidth;
    p[30] = (unsigned char)packet_types;
    p[31] = (unsigned char)(packet_types >> 8);
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_reject_esco_conn(void *p_cmd, unsigned char *bd_addr, unsigned char reason)
{
    unsigned char *p = (unsigned char *)p_cmd;
    *(unsigned short *)(p + 2) = 0xa;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x2a;
    p[9] = 4;
    p[10] = 7;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = reason;
    btu_hcif_send_cmd(p_cmd);
}

int btsnd_hcic_hold_mode(void *p_buf, short handle, short max_hold_period, unsigned short min_hold_period)
{
    unsigned char *p = (unsigned char *)p_buf;
    if (p == NULL) {
        p = (unsigned char *)GKI_getpoolbuf(2);
        if (p == NULL)
            return 0;
    }
    *(unsigned short *)(p + 2) = 9;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 1;
    p[9] = 8;
    p[10] = 6;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    p[13] = (unsigned char)max_hold_period;
    p[14] = (unsigned char)(max_hold_period >> 8);
    p[15] = (unsigned char)min_hold_period;
    p[16] = (unsigned char)(min_hold_period >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_sniff_mode(void *p_buf, short handle, short max_sniff_period, short min_sniff_period, short sniff_attempt, unsigned short sniff_timeout)
{
    unsigned char *p = (unsigned char *)p_buf;
    if (p == NULL) {
        p = (unsigned char *)GKI_getpoolbuf(2);
        if (p == NULL)
            return 0;
    }
    *(unsigned short *)(p + 2) = 0xd;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 3;
    p[9] = 8;
    p[10] = 0xa;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    p[13] = (unsigned char)max_sniff_period;
    p[14] = (unsigned char)(max_sniff_period >> 8);
    p[15] = (unsigned char)min_sniff_period;
    p[16] = (unsigned char)(min_sniff_period >> 8);
    p[17] = (unsigned char)sniff_attempt;
    p[18] = (unsigned char)(sniff_attempt >> 8);
    p[19] = (unsigned char)sniff_timeout;
    p[20] = (unsigned char)(sniff_timeout >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_exit_sniff_mode(void *p_buf, unsigned short handle)
{
    unsigned char *p = (unsigned char *)p_buf;
    if (p == NULL) {
        p = (unsigned char *)GKI_getpoolbuf(2);
        if (p == NULL)
            return 0;
    }
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 4;
    p[9] = 8;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_park_mode(void *p_buf, short handle, short max_park_period, unsigned short min_park_period)
{
    unsigned char *p = (unsigned char *)p_buf;
    if (p == NULL) {
        p = (unsigned char *)GKI_getpoolbuf(2);
        if (p == NULL)
            return 0;
    }
    *(unsigned short *)(p + 2) = 9;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 5;
    p[9] = 8;
    p[10] = 6;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    p[13] = (unsigned char)max_park_period;
    p[14] = (unsigned char)(max_park_period >> 8);
    p[15] = (unsigned char)min_park_period;
    p[16] = (unsigned char)(min_park_period >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_exit_park_mode(void *p_buf, unsigned short handle)
{
    unsigned char *p = (unsigned char *)p_buf;
    if (p == NULL) {
        p = (unsigned char *)GKI_getpoolbuf(2);
        if (p == NULL)
            return 0;
    }
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 6;
    p[9] = 8;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_switch_role(unsigned char *bd_addr, unsigned char role)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 0xA;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0xB;
    p[9] = 8;
    p[10] = 7;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = role;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_write_policy_set(short handle, unsigned short settings)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 7;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0xd;
    p[9] = 8;
    p[10] = 4;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    p[13] = (unsigned char)settings;
    p[14] = (unsigned char)(settings >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_reset(void)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 3;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 3;
    p[9] = 0x0c;
    p[10] = 0;
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_set_event_filter(unsigned char *p, unsigned char filt_type, unsigned char filt_cond_type, unsigned char *bd_addr, unsigned char filt_cond)
{
    unsigned char *src;
    unsigned char *dst;
    unsigned char count;
    int i;

    *(unsigned short *)(p + 4) = 0;
    p[8] = 5;
    p[9] = 0xc;
    if (filt_type != 0) {
        *(unsigned short *)(p + 2) = filt_cond + 5;
        p[10] = filt_cond + 2;
        p[11] = filt_type;
        p[12] = filt_cond_type;
        src = bd_addr;
        dst = p + 13;
        count = filt_cond;
        if (filt_cond_type == 1) {
            dst[0] = src[2];
            dst[1] = src[1];
            dst[2] = src[0];
            dst[3] = src[5];
            dst[4] = src[4];
            dst[5] = src[3];
            src += 6;
            dst += 6;
            count = filt_cond - 6;
        } else if (filt_cond_type == 2) {
            dst[0] = src[5];
            dst[1] = src[4];
            dst[2] = src[3];
            dst[3] = src[2];
            dst[4] = src[1];
            dst[5] = src[0];
            src += 6;
            dst += 6;
            count = filt_cond - 6;
        }
        for (i = 0; i < count; i++)
            *dst++ = *src++;
    } else {
        *(unsigned short *)(p + 2) = 4;
        p[10] = 1;
        p[11] = filt_type;
    }
    btu_hcif_send_cmd(p);
}

int btsnd_hcic_write_pin_type(unsigned char pin_type)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 4;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x0a;
    p[9] = 0x0c;
    p[10] = 1;
    p[11] = pin_type;
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_read_stored_key(unsigned char *p, unsigned char *bd_addr, unsigned char read_all)
{
    *(unsigned short *)(p + 2) = 0xa;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0xd;
    p[9] = 0xc;
    p[10] = 7;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = read_all;
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_stored_key(unsigned char *p, unsigned char num_keys, unsigned char *bd_addr, unsigned char *link_key)
{
    unsigned short len;
    unsigned char *pp;
    int i, j;

    len = num_keys * 22 + 4;
    *(unsigned short *)(p + 4) = 0;
    *(unsigned short *)(p + 2) = len;
    p[8] = 0x11;
    p[9] = 0xc;
    p[10] = (unsigned char)(len - 3);
    if (num_keys > 11)
        num_keys = 11;
    p[11] = num_keys;
    pp = p + 12;
    for (i = 0; i < num_keys; i++) {
        pp[0] = bd_addr[5];
        pp[1] = bd_addr[4];
        pp[2] = bd_addr[3];
        pp[3] = bd_addr[2];
        pp[4] = bd_addr[1];
        pp[5] = bd_addr[0];
        for (j = 0; j < 16; j++)
            pp[6 + j] = link_key[15 - j];
        pp += 22;
        bd_addr += 6;
        link_key += 16;
    }
    btu_hcif_send_cmd(p);
}

int btsnd_hcic_delete_stored_key(unsigned char *bd_addr, unsigned char delete_all_flag)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 0xA;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x12;
    p[9] = 0xC;
    p[10] = 7;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = delete_all_flag;
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_change_name(unsigned char *p, unsigned char *name)
{
    unsigned short len = (unsigned short)(strlen(name) + 1);
    unsigned char *dst = p + 11;
    int i;

    *(unsigned short *)(p + 2) = 0xfb;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x13;
    p[9] = 0xc;
    p[10] = 0xf8;
    for (i = 0; i < len; i++)
        *dst++ = name[i];
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_page_tout(void *p, unsigned short timeout)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 5;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x18;
    b[9] = 0x0c;
    b[10] = 2;
    b[11] = (unsigned char)timeout;
    b[12] = (unsigned char)(timeout >> 8);
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_scan_enable(void *p, unsigned char mode)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 4;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x1a;
    b[9] = 0x0c;
    b[10] = 1;
    b[11] = mode;
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_pagescan_cfg(void *p, unsigned short interval, unsigned short window)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 7;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x1c;
    b[9] = 0x0c;
    b[10] = 4;
    b[11] = (unsigned char)interval;
    b[12] = (unsigned char)(interval >> 8);
    b[13] = (unsigned char)window;
    b[14] = (unsigned char)(window >> 8);
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_inqscan_cfg(void* p, unsigned short interval, unsigned short window)
{
    unsigned char* b = (unsigned char*)p;
    *(unsigned short*)(b + 2) = 7;
    *(unsigned short*)(b + 4) = 0;
    b[8] = 0x1e;
    b[9] = 0x0c;
    b[10] = 4;
    b[11] = (unsigned char)interval;
    b[12] = (unsigned char)(interval >> 8);
    b[13] = (unsigned char)window;
    b[14] = (unsigned char)(window >> 8);
    btu_hcif_send_cmd(p);
}

int btsnd_hcic_write_auth_enable(unsigned char flag)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 4;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x20;
    p[9] = 0x0c;
    p[10] = 1;
    p[11] = flag;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_write_encr_mode(unsigned char mode)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 4;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x22;
    p[9] = 0x0c;
    p[10] = 1;
    p[11] = mode;
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_write_dev_class(void *p, unsigned char *dev_class)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 6;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x24;
    b[9] = 0x0c;
    b[10] = 3;
    b[11] = dev_class[2];
    b[12] = dev_class[1];
    b[13] = dev_class[0];
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_auto_flush_tout(void *p, unsigned short handle, unsigned short timeout)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 7;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x28;
    b[9] = 0x0c;
    b[10] = 4;
    b[11] = (unsigned char)handle;
    b[12] = (unsigned char)(handle >> 8);
    b[13] = (unsigned char)timeout;
    b[14] = (unsigned char)(timeout >> 8);
    btu_hcif_send_cmd(p);
}

int btsnd_hcic_set_host_buf_size(short acl_buf_size, unsigned char sco_buf_size, short acl_pkt_count, unsigned short sco_pkt_count)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 0xA;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x33;
    p[9] = 0xC;
    p[10] = 7;
    p[11] = (unsigned char)acl_buf_size;
    p[12] = (unsigned char)(acl_buf_size >> 8);
    p[13] = sco_buf_size;
    p[14] = (unsigned char)acl_pkt_count;
    p[15] = (unsigned char)(acl_pkt_count >> 8);
    p[16] = (unsigned char)sco_pkt_count;
    p[17] = (unsigned char)(sco_pkt_count >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_write_link_super_tout(short handle, unsigned short timeout)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 7;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x37;
    p[9] = 0xc;
    p[10] = 4;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    p[13] = (unsigned char)timeout;
    p[14] = (unsigned char)(timeout >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_write_cur_iac_lap(unsigned char *p, unsigned char num_laps, unsigned char *lap_array)
{
    unsigned short len = (num_laps << 2) - num_laps + 4;
    unsigned char *pp = p + 12;

    *(unsigned short *)(p + 2) = len;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x3a;
    p[9] = 0x0c;
    p[10] = (unsigned char)(len - 3);
    p[11] = num_laps;

    while (num_laps--) {
        pp[0] = lap_array[2];
        pp[1] = lap_array[1];
        pp[2] = lap_array[0];
        pp += 3;
        lap_array += 3;
    }
    btu_hcif_send_cmd(p);
}

int btsnd_hcic_read_local_ver(void)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 3;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 1;
    p[9] = 0x10;
    p[10] = 0;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_read_local_features(void)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 3;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 3;
    p[9] = 0x10;
    p[10] = 0;
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_read_buffer_size(void *p_buf) {
    ((unsigned short *)p_buf)[1] = 3;
    ((unsigned short *)p_buf)[2] = 0;
    ((unsigned char *)p_buf)[8] = 5;
    ((unsigned char *)p_buf)[9] = 0x10;
    ((unsigned char *)p_buf)[10] = 0;
    btu_hcif_send_cmd(p_buf);
}

int btsnd_hcic_read_bd_addr(void)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 3;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 9;
    p[9] = 0x10;
    p[10] = 0;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_get_link_quality(unsigned short handle)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 3;
    p[9] = 0x14;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_read_rssi(unsigned short handle)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 5;
    p[9] = 0x14;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_set_afh_channels(unsigned char first, unsigned char last)
{
    unsigned char channels[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F};
    unsigned char *p;
    int i;

    if ((p = (unsigned char *)GKI_getpoolbuf(2)) == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 0x0D;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x3F;
    p[9] = 0x0C;
    p[10] = 0x0A;

    if (first <= last && last <= 78) {
        for (i = first; i <= last; i++) {
            int byte_offset = i / 8;
            int bit_offset = i % 8;
            channels[byte_offset] &= ~(1 << bit_offset);
        }
    }

    for (i = 0; i < 10; i++)
        p[11 + i] = channels[i];

    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_write_inqscan_type(void *p, unsigned char type)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 4;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x43;
    b[9] = 0x0c;
    b[10] = 1;
    b[11] = type;
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_inquiry_mode(void *buf, unsigned char mode)
{
    unsigned short *ps;
    unsigned char *pb;
    ps = (unsigned short *)((unsigned char *)buf + 2);
    *ps = 4;
    ps = (unsigned short *)((unsigned char *)buf + 4);
    *ps = 0;
    pb = (unsigned char *)buf + 8;
    *pb = 0x45;
    pb = (unsigned char *)buf + 9;
    *pb = 0x0c;
    pb = (unsigned char *)buf + 10;
    *pb = 1;
    pb = (unsigned char *)buf + 11;
    *pb = mode;
    btu_hcif_send_cmd(buf);
}

void btsnd_hcic_write_pagescan_type(void *pBuf, unsigned char type) {
    *(unsigned short*)((unsigned char*)pBuf + 2) = 4;
    *(unsigned short*)((unsigned char*)pBuf + 4) = 0;
    *((unsigned char*)pBuf + 8) = 0x47;
    *((unsigned char*)pBuf + 9) = 0x0C;
    *((unsigned char*)pBuf + 10) = 0x01;
    *((unsigned char*)pBuf + 11) = type;
    btu_hcif_send_cmd(pBuf);
}

void btsnd_hcic_vendor_spec_cmd(unsigned char *p, unsigned short opcode, int len, unsigned char *data)
{
    int cmd = opcode | 0xfc00;
    unsigned char *dst = p + 11;
    int i;

    *(unsigned short *)(p + 2) = len + 3;
    *(unsigned short *)(p + 4) = 0;
    p[8] = (unsigned char)cmd;
    p[9] = (unsigned char)(cmd >> 8);
    p[10] = (unsigned char)len;
    for (i = 0; i < len; i++)
        *dst++ = data[i];
    btu_hcif_send_cmd(p);
}
